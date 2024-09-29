use reqwest::blocking::Client;
use std::process::Command;
use std::thread;
use std::time::Duration;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let client = Client::new();
    let esp32_ip = "http://###.###.#.##"; // Replace with your ESP32's actual IP

    loop {
        // Run `diskutil info` command to get detailed disk usage information
        let output = Command::new("diskutil").arg("info").arg("/").output()?;

        let output_str = String::from_utf8_lossy(&output.stdout);
        println!("diskutil output:\n{}", output_str); // Print the entire output for debugging

        let lines: Vec<&str> = output_str.lines().collect();

        let mut total_gb = 0.0;
        let mut free_gb = 0.0;

        for line in lines {
            println!("Line: {}", line); // Debug print each line

            if line.contains("Container Total Space") {
                if let Some(total_str) = line.split_whitespace().nth(3) {
                    let total = total_str.trim_end_matches("GB").replace(",", "");
                    total_gb = total.parse::<f64>().unwrap_or(0.0);
                    println!("Total GB: {}", total_gb); // Debug print
                }
            } else if line.contains("Container Free Space") {
                if let Some(free_str) = line.split_whitespace().nth(3) {
                    let free = free_str.trim_end_matches("GB").replace(",", "");
                    free_gb = free.parse::<f64>().unwrap_or(0.0);
                    println!("Free GB: {}", free_gb); // Debug print
                }
            }
        }

        let used_gb = total_gb - free_gb; // Calculate used space

        // Format the values to match the precision (e.g., 2 decimal places)
        let total_formatted = format!("{:.2}", total_gb);
        let used_formatted = format!("{:.2}", used_gb);

        // Run `sw_vers` command to get macOS version
        let os_output = Command::new("sw_vers").arg("-productVersion").output()?;

        let os_version = String::from_utf8_lossy(&os_output.stdout)
            .trim()
            .to_string();
        println!("OS Version: {}", os_version); // Debug print

        // Send data to the ESP32
        let response = client
            .post(format!("{}/update", esp32_ip))
            .form(&[
                ("total", &total_formatted),
                ("used", &used_formatted),
                ("os_version", &os_version),
            ])
            .send()?;

        println!("Sent update to ESP32. Status: {}", response.status());

        // Sleep for 60 seconds before sending the next update
        thread::sleep(Duration::from_secs(60));
    }
}
