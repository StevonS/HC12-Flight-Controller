

// See SoftwareSerial example 
// https://www.arduino.cc/en/tutorial/SoftwareSerialExample

#include <SoftwareSerial.h>;
#define RX 2 //Connect to the TX pin of the HC-12
#define TX 3 //Connect to the RX pin of the HC-12
SoftwareSerial HC12(RX, TX);
bool armed = false;
byte incomingByte;
String readBuffer = "";

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);

}

void loop() { // run over and over

   while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    Serial.println(readBuffer);
  }

  if (readBuffer == "Armed") {
    Serial.println("ARMED");
    armed = true;
  }

  if (armed == true) {
    HC12.write("On-Board armed");
    if (HC12.available()) {
      Serial.write(HC12.read());
    }
    if (Serial.available()) {
      HC12.write(Serial.read());
    }
  }
  


}
