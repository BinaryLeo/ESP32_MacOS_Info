
#include <WiFi.h>
#include <WebServer.h>
#include <U8g2lib.h>

// WiFi credentials (to match "your ssid" networks)
const char* password = "your password here";

// Web server and display setup
WebServer server(80);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);

String totalStorage = "0";
String usedStorage = "0";
String osVersion = "Unknown";

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);

  // Start scanning for WiFi networks
  Serial.println("Scanning for networks...");
  scanAndConnectWiFi();
}

void loop() {
  server.handleClient();  // Handle incoming client requests
  updateDisplay();        // Update the OLED display with current storage info
}

// Function to scan for WiFi networks and attempt connection
void scanAndConnectWiFi() {
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete");

  if (n == 0) {
    Serial.println("No networks found");
    displayMessage("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    displayMessage(String(n) + " networks found");

    for (int i = 0; i < n; ++i) {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);

      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(ssid);
      Serial.print(" (");
      Serial.print(rssi);
      Serial.println(" dBm)");

      // Display network information on OLED
      updateDisplay(i + 1, ssid, rssi);

      // If the network starts with "your ssid", attempt to connect
      if (ssid.startsWith(" your ssid")) {
        Serial.println("Found your ssid network, attempting to connect...");
        displayMessage("Connecting to: " + ssid);
        connectToWiFi(ssid.c_str(), password);
        break;  // Stop scanning after finding and connecting to the network
      }
    }
  }
}

// Function to connect to WiFi
void connectToWiFi(const char* ssid, const char* pass) {
  WiFi.begin(ssid, pass);
  unsigned long startAttemptTime = millis();
  const unsigned long connectionTimeout = 20000; // 20 seconds timeout

  Serial.print("Connecting to WiFi...");
  displayMessage("Connecting...");

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < connectionTimeout) {
    delay(1000);
    Serial.print(".");
  }

  // Check if connected or if the connection failed
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    displayMessage("Connected! IP: " + WiFi.localIP().toString());

    // Once connected, start the web server and set up routes
    startWebServer();
  } else {
    Serial.println("\nFailed to connect to WiFi");
    displayMessage("Failed to connect");
  }
}

// Function to start the web server and set up routes
void startWebServer() {
  // Handle POST request to /update
  server.on("/update", HTTP_POST, handleUpdate);
  server.begin();
  Serial.println("Web server started");
}

// Function to handle updates to total and used storage
void handleUpdate() {
  if (server.hasArg("total") && server.hasArg("used") && server.hasArg("os_version")) {
    totalStorage = server.arg("total");
    usedStorage = server.arg("used");
    osVersion = server.arg("os_version");
    server.send(200, "text/plain", "Updated");
    Serial.println("Update received: total = " + totalStorage + " GB, used = " + usedStorage + " GB, OS version = " + osVersion);
  } else {
    server.send(400, "text/plain", "Bad Request");
    Serial.println("Bad Request received on /update");
  }
}

// Function to update the OLED display with storage information
void updateDisplay() {
  // Calculate free storage
  float total = totalStorage.toFloat();
  float used = usedStorage.toFloat();
  float freeStorage = total - used;

  u8g2.clearBuffer();
  u8g2.drawStr(0, 10, ("Total: " + totalStorage + " GB").c_str()); // Adjusted y-coordinate to 26
  u8g2.drawStr(0, 26, ("Used: " + usedStorage + " GB").c_str()); // Adjusted y-coordinate to 42
  u8g2.drawStr(0, 42, ("Free: " + String(freeStorage) + " GB").c_str()); // Adjusted y-coordinate to 58
  u8g2.drawStr(0, 58, ("OS: " + osVersion).c_str()); // Adjusted y-coordinate to 74
  u8g2.sendBuffer();
}

// Function to update the OLED display with network scan results
void updateDisplay(int index, String ssid, int rssi) {
  u8g2.clearBuffer();  // Clear the display buffer
  u8g2.setCursor(0, 10);
  u8g2.print("WiFi Scan Result:");

  String networkInfo = String(index) + ": " + ssid + " (" + rssi + " dBm)";
  u8g2.setCursor(0, 30);
  u8g2.print(networkInfo);  // Print the network info on the display

  u8g2.sendBuffer();  // Send the buffer to the display
}

// Function to display a simple message on the OLED screen
void displayMessage(String message) {
  u8g2.clearBuffer();  // Clear the display buffer
  u8g2.setCursor(0, 20);
  u8g2.print(message);  // Print the message
  u8g2.sendBuffer();    // Send the buffer to the display
}
