#define RX1 15 // RX Pin connected to LoRa TXD 
#define TX1 4 // TX Pin connected to LoRa RXD

HardwareSerial LoraSerial(1); // UART1 port

void setup() {
  Serial.begin(115200);

  while (!Serial) delay(10); // Waits until serial monitor is ready

  // Start UART communication with LoRa
  LoraSerial.begin(115200, SERIAL_8N1, RX1, TX1);
  delay(1000);

  Serial.println("Setup Complete");
}

void loop() {

  delay(3000);

  if (sendData("Hello")) { // Attempt to pass "Hello" into sendData
    Serial.println("Success");
  } else {
    Serial.println("Failure");
  }
}

bool sendData(String theMessage) { // sendData function with theMessage string
  Serial.println("Sending message: " + theMessage);
  if (loraStatus()) { // If LoRa works
    // compiledMessage = AT+SEND=0,theMessage.length,theMessage
    String compiledMessage = "AT+SEND=0," + String(theMessage.length()) + "," + theMessage;
    Serial.println("Command: " + compiledMessage);
    LoraSerial.println(compiledMessage); // Print to LoRa

    delay(300);

    while (LoraSerial.available()) { // While data is available
      // Read characters until newline
      String response = LoraSerial.readStringUntil('\n');
      response.trim(); // Remove spaces

      Serial.print("Send response: ");
      Serial.println(response); // Print response
    }

    Serial.println("It worked?"); // Debug message
    return true;
  }
  return false;
}

bool loraStatus() { // Function checking LoRa status
  bool statusGood = false; // Assume bad
  unsigned int attempts = 0;
  unsigned const int ATTEMPTS_ALLOWED = 5; // Retry count

  // While attempts are allowed and module not responding
  while (ATTEMPTS_ALLOWED > attempts && !statusGood) {

    // Increment attempts
    attempts++;
    Serial.print("Attempt: "); // Show # of attempts
    Serial.println(attempts);

    // Flush old data
    while (LoraSerial.available()) {
      LoraSerial.read();
    }

    // Send AT command to LoRa
    LoraSerial.println("AT");
    delay(300);

    if (LoraSerial.available()) { // If data exists
      // Read characters until newline
      String response = LoraSerial.readStringUntil('\n');
      response.trim(); // Remove spaces

      Serial.print("Module says: ");
      Serial.println(response); // Print response

      // If LoRa responds with +OK, it is running well
      if (response.indexOf("+OK") != -1) {
        statusGood = true;
      }

      } else {
      Serial.println("No response from module");
      }
    }

    if (!statusGood) { // Timeout warning
      Serial.println("LoRa module not responding");
    }

    return statusGood;
  }
