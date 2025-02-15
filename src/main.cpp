#include <Arduino.h> 
#include <Wire.h> 
#include <WiFi.h> 

// I2C address of MPU-6050 
const int MPU_addr = 0x68;  
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ; 

// Wi-Fi network credentials 
const char *ssid = "Alex"; 
const char *pass = "Sacha3232"; 
WiFiServer server(80); 

// GPIO for the blue LED 
const int ledPin = 2; 

// Function declarations 
void mpu_read(); 
void check_conditions(); 
void handle_client(); 

void setup() { 
   Serial.begin(115200); 
   Wire.begin(); 
   delay(1000);

   // Initialize MPU-6050 
   Wire.beginTransmission(MPU_addr); 
   Wire.write(0x6B); // PWR_MGMT_1 register 
   Wire.write(0);   // Activate the sensor 
   Wire.endTransmission(true); 
   Serial.println("MPU6050 initialized."); 

   // LED configuration 
   pinMode(ledPin, OUTPUT); 
   digitalWrite(ledPin, LOW); // LED off initially 

   // Wi-Fi connection 
   Serial.println("Connecting to WiFi..."); 
   WiFi.begin(ssid, pass); 
   while (WiFi.status() != WL_CONNECTED) { 
      delay(500); 
      Serial.print("."); 
   } 
   Serial.println("\nWiFi connected."); 
   Serial.print("IP Address: "); 
   Serial.println(WiFi.localIP()); 

   server.begin(); 
   Serial.println("Server started."); 
} 

void loop() { 
   mpu_read(); 
   check_conditions(); 
   handle_client(); 
} 

void mpu_read() { 
   Wire.beginTransmission(MPU_addr); 
   Wire.write(0x3B); // ACCEL_XOUT_H register 
   Wire.endTransmission(false); 
   delay(10); 
   Wire.requestFrom((uint8_t)MPU_addr, (size_t)14, true); // Specifying types to avoid ambiguity 

   if (Wire.available() == 14) { 
      AcX = Wire.read() << 8 | Wire.read(); 
      AcY = Wire.read() << 8 | Wire.read(); 
      AcZ = Wire.read() << 8 | Wire.read(); 
      GyX = Wire.read() << 8 | Wire.read(); 
      GyY = Wire.read() << 8 | Wire.read(); 
      GyZ = Wire.read() << 8 | Wire.read(); 
   } else { 
      Serial.println("Error reading MPU6050."); 
   } 

   Serial.print("AcX: "); Serial.print(AcX); 
   Serial.print(", AcY: "); Serial.print(AcY); 
   Serial.print(", AcZ: "); Serial.print(AcZ); 
   Serial.print(", GyX: "); Serial.print(GyX); 
   Serial.print(", GyY: "); Serial.print(GyY); 
   Serial.print(", GyZ: "); Serial.println(GyZ); 
} 

void check_conditions() { 
   float accelX = AcX / 16384.0; 
   float gyroZ = GyZ / 131.0; 

   if (fabs(accelX) > 1.0 || fabs(gyroZ) > 100.0) {  // Use fabs() for absolute value
      digitalWrite(ledPin, HIGH); 
      Serial.println("LED on: Conditions met."); 
   } else { 
      digitalWrite(ledPin, LOW); 
      Serial.println("LED off: Conditions not met."); 
   } 
} 

void handle_client() {
    WiFiClient client = server.available();

    if (client) {
        String request = "";
        while (client.available()) {
            char c = client.read();
            request += c;
        }

        if (request.indexOf("/data") >= 0) {
            client.print("HTTP/1.1 200 OK\r\n");
            client.print("Content-Type: application/json\r\n");
            client.print("Connection: close\r\n\r\n");
            client.print("{");
            client.print("\"AcX\":" + String(AcX / 16384.0) + ",");
            client.print("\"AcY\":" + String(AcY / 16384.0) + ",");
            client.print("\"AcZ\":" + String(AcZ / 16384.0) + ",");
            client.print("\"GyX\":" + String(GyX / 131.0) + ",");
            client.print("\"GyY\":" + String(GyY / 131.0) + ",");
            client.print("\"GyZ\":" + String(GyZ / 131.0));
            client.print("}");
        } else {
            client.print("HTTP/1.1 200 OK\r\n");
            client.print("Content-Type: text/html; charset=UTF-8\r\n");
            client.print("Connection: close\r\n\r\n");
            client.print("<!DOCTYPE HTML><html>");
            client.print("<head>");
            client.print("<title>ESP32 Motion Data Monitoring</title>");
            client.print("<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>");
            client.print("<script>");
            client.print("function updateData() {");
            client.print("fetch('/data').then(response => response.json()).then(data => {");
            client.print("document.getElementById('AcX').innerText = data.AcX;");
            client.print("document.getElementById('AcY').innerText = data.AcY;");
            client.print("document.getElementById('AcZ').innerText = data.AcZ;");
            client.print("document.getElementById('GyX').innerText = data.GyX;");
            client.print("document.getElementById('GyY').innerText = data.GyY;");
            client.print("document.getElementById('GyZ').innerText = data.GyZ;");
            client.print("});");
            client.print("}");
            client.print("setInterval(updateData, 1000);");
            client.print("</script>");
            client.print("</head><body>");
            client.print("<h1>ESP32 Motion Data</h1>");
            client.print("<p><strong>Acceleration on X-axis (forward/backward):</strong> <span id='AcX'>0</span> g</p>");
            client.print("<p><strong>Acceleration on Y-axis (left/right):</strong> <span id='AcY'>0</span> g</p>");
            client.print("<p><strong>Acceleration on Z-axis (up/down):</strong> <span id='AcZ'>0</span> g</p>");
            client.print("<p><strong>Gyroscope X-axis (tilt forward/backward):</strong> <span id='GyX'>0</span> °/s</p>");
            client.print("<p><strong>Gyroscope Y-axis (tilt left/right):</strong> <span id='GyY'>0</span> °/s</p>");
            client.print("<p><strong>Gyroscope Z-axis (turning):</strong> <span id='GyZ'>0</span> °/s</p>");
            client.print("<canvas id='myChart' width='400' height='200'></canvas>");
            client.print("<script>");
            client.print("const ctx = document.getElementById('myChart').getContext('2d');");
            client.print("const myChart = new Chart(ctx, {");
            client.print("type: 'line',");
            client.print("data: {");
            client.print("labels: [],");
            client.print("datasets: [{");
            client.print("label: 'AcX', data: [], borderColor: 'rgb(255, 99, 132)', fill: false},");
            client.print("{label: 'AcY', data: [], borderColor: 'rgb(54, 162, 235)', fill: false},");
            client.print("{label: 'AcZ', data: [], borderColor: 'rgb(75, 192, 192)', fill: false},");
            client.print("{label: 'GyX', data: [], borderColor: 'rgb(153, 102, 255)', fill: false},");
            client.print("{label: 'GyY', data: [], borderColor: 'rgb(255, 159, 64)', fill: false},");
            client.print("{label: 'GyZ', data: [], borderColor: 'rgb(255, 205, 86)', fill: false}");
            client.print("]}");
            client.print("});");
            client.print("function updateChart() {");
            client.print("fetch('/data').then(response => response.json()).then(data => {");
            client.print("const labels = myChart.data.labels;");
            client.print("const datasets = myChart.data.datasets;");
            client.print("if (labels.length > 20) {");
            client.print("labels.shift();");
            client.print("datasets.forEach((dataset) => dataset.data.shift());");
            client.print("}");
            client.print("labels.push(new Date().toLocaleTimeString());");
            client.print("datasets[0].data.push(data.AcX);");
            client.print("datasets[1].data.push(data.AcY);");
            client.print("datasets[2].data.push(data.AcZ);");
            client.print("datasets[3].data.push(data.GyX);");
            client.print("datasets[4].data.push(data.GyY);");
            client.print("datasets[5].data.push(data.GyZ);");
            client.print("myChart.update();");
            client.print("});");
            client.print("}");
            client.print("setInterval(updateChart, 1000);");
            client.print("</script>");
            client.print("</body></html>");
        }
        client.stop();  // Close the connection
    }
}
