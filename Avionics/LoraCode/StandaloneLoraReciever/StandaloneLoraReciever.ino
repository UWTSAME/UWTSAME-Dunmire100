// barebones file for now
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
          Serial.println(response);
        }
    }
}
