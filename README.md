# MacOS Storage Monitor with ESP32 OLED Display

This project monitors the storage usage on a macOS system and displays the information on a Heltec ESP32 OLED board. The project consists of two main components: a Rust program running on the macOS system and a C++ program running on the ESP32.

![WhatsApp Image 2024-09-29 at 11 49 06](https://github.com/user-attachments/assets/180998b7-4c6f-4a60-8f85-05536197f6de)
![WhatsApp Image 2024-09-29 at 11 48 53](https://github.com/user-attachments/assets/8be56feb-33d1-43c3-b101-0f2a012f3014)


## Overview

The Rust program on the macOS system:
- Monitors the total and used storage space.
- Retrieves the macOS version.
- Sends the storage information and macOS version to the ESP32 via HTTP POST requests.

The C++ program on the ESP32:
- Connects to a WiFi network.
- Hosts a web server to receive storage information and macOS version.
- Displays the received information on the OLED screen.

## Dependencies

### Rust Program

- Rust (https://www.rust-lang.org/)
- `reqwest` crate for HTTP requests
- `std` library for system commands and threading

### ESP32 Program

- Arduino IDE (https://www.arduino.cc/en/software)
- Heltec ESP32 library (https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series)
- WiFi library
- WebServer library
- U8g2 library for OLED display

![137672252-1cd1f805-63fc-47e1-98c5-f1d9968d2539](https://github.com/user-attachments/assets/5c446299-c1bb-4678-9b04-6ab492363d98)


## Setup Instructions

### macOS System

1. **Install Rust:**
   Follow the instructions at https://www.rust-lang.org/tools/install to install Rust.

2. **Clone the Repository:**
   ```sh
   git clone https://github.com/yourusername/macos-storage-monitor.git
   cd macos-storage-monitor
   ```

3. **Add Dependencies:**
   Add the `reqwest` crate to your `Cargo.toml` file:
   ```toml
   [dependencies]
   reqwest = "0.11"
   ```

4. **Run the Rust Program:**
   ```sh
   cargo run
   ```

### ESP32 Setup

1. **Install Arduino IDE:**

   Download and install the Arduino IDE from https://www.arduino.cc/en/software.

2. **Install Heltec ESP32 Library:**
   - Open the Arduino IDE.
   - Go to `File` > `Preferences`.
   - In the "Additional Board Manager URLs" field, add:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Tools` > `Board` > `Boards Manager`.
   - Search for "Heltec ESP32" and install the latest version.

3. **Install Required Libraries:**
   - Go to `Sketch` > `Include Library` > `Manage Libraries`.
   - Install the following libraries:
     - `WiFi`
     - `WebServer`
     - `U8g2`

4. **Connect the Heltec ESP32 Board:**
   - Connect the Heltec ESP32 board to your computer using a USB cable.
   - Select the correct board and port in the Arduino IDE:
     - Go to `Tools` > `Board` and select `Heltec WiFi Kit 32`.
     - Go to `Tools` > `Port` and select the port corresponding to your ESP32.

5. **Upload the C++ Program:**
   - Open the `esp32_storage_monitor.ino` file in the Arduino IDE.
   - Click the "Upload" button to upload the program to the ESP32.

## Heltec ESP32 OLED Board

The Heltec ESP32 OLED board is a development board that features an ESP32 microcontroller and a built-in OLED display. It is ideal for IoT projects that require both connectivity and display capabilities.

### Features

- ESP32 microcontroller with WiFi and Bluetooth capabilities
- Built-in 0.96-inch OLED display
- Multiple GPIO pins for connecting sensors and peripherals

### Connecting to the Computer

1. **Connect the Board:**
   - Use a USB cable to connect the Heltec ESP32 board to your computer.

2. **Install Drivers:**
   - If necessary, install the USB-to-serial drivers for the ESP32. You can find the drivers at:
     https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers

3. **Select the Board and Port:**
   - In the Arduino IDE, go to `Tools` > `Board` and select `Heltec WiFi Kit 32`.
   - Go to `Tools` > `Port` and select the port corresponding to your ESP32.

## How It Works

1. **Rust Program:**
   - The Rust program runs on the macOS system and periodically retrieves the total and used storage space using the `diskutil` command.
   - It also retrieves the macOS version using the `sw_vers` command.
   - The program sends the storage information and macOS version to the ESP32 via HTTP POST requests.

2. **ESP32 Program:**
   - The ESP32 connects to a WiFi network and hosts a web server.
   - It receives the storage information and macOS version from the Rust program.
   - The information is displayed on the built-in OLED screen.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Rust Programming Language](https://www.rust-lang.org/)
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Heltec WiFi Kit 32](https://heltec.org/project/wifi-lora-32v2)
- [U8g2 Library](https://github.com/olikraus/u8g2)
```
### Conclusion
