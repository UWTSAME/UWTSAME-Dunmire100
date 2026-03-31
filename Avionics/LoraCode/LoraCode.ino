
#define RX1 16
#define TX1 17

HardwareSerial LoraSerial(2);

void setup() {
  Serial.begin(115200);

  while (!Serial) delay(10);

  LoraSerial.begin(115200, SERIAL_8N1, RX1, TX1);

  Serial.print("Ok we are starting off good?");
}

void loop() {

  delay(3000);
  Serial.println(loraStatus());
  Serial.println(sendData("Hello"));

  if (sendData("Hello")) {
    Serial.println("Success");
  } else {
    Serial.println("failure");
  }
}

bool sendData(String theMessage) {
  Serial.println("Sending message: " + theMessage);
  bool returner = false;
  if (loraStatus()) {
    String compiledMessage = "AT+SEND=0," + String(theMessage.length()) + "," + theMessage;
    LoraSerial.println(compiledMessage);
    returner = true;
    Serial.println("It worked?");
  }

  return false;
}

bool loraStatus() {
  bool statusGood = false;
  unsigned long deltaTime = millis();
  unsigned int attempts = 0;
  unsigned const int ATTEMPTS_ALLOWED = 5;

  while (ATTEMPTS_ALLOWED > attempts && !statusGood) {

    attempts++;
    LoraSerial.println("AT");

    delay(200);

    if (LoraSerial.available()) {
      String response = LoraSerial.readString();
      Serial.print("Module says: ");
      Serial.println(response);

      if (response.indexOf("+OK") != -1) {
        statusGood = true;
      }
    }
  }

  return statusGood;
}
