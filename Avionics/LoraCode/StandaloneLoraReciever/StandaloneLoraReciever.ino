#define RX1 15 // RX Pin connected to LoRa TXD 
#define TX1 4 // TX Pin connected to LoRa RXD
HardwareSerial LoraSerial(1);

void setup() {
    Serial.begin(115200);
    LoraSerial.begin(115200, SERIAL_8N1, RX1, TX1);
}

void loop() {
    while (LoraSerial.available()) {
        String response = LoraSerial.readStringUntil('\n');
        response.trim(); 
        if (response.indexOf("+") != -1) {
            // Between second comma and last comma
            Serial.println(response.substring(response.indexOf(',', response.indexOf(',') + 1) + 1, response.lastIndexOf(',')));
        }
    }
}
