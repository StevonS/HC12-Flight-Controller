#include <PS4Controller.h>
#define RXD2 16	//(RX2)
#define TXD2 17	//(TX2)
#define HC12 Serial2  //Hardware serial 2 on the ESP32
bool armed = false;
byte incomingByte;
String readBuffer = "";

void setup() {
  Serial.begin(115200);
  PS4.begin("EC:62:60:1E:E1:68");
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);           //Normally HIGH, LOW for settings
  Serial.begin(115200);           // Serial port to computer
  HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);   
  Serial.println("Ready.");

}


void sendData(){

}


void loop() {

  controller();

  while (HC12.available()) {
    incomingByte = HC12.read();
    readBuffer += char(incomingByte);        
    Serial.write(HC12.read());  

  }
  while (Serial.available()) {      
    HC12.write(Serial.read());   


  }
}


void controller(){

  //Arm/Disarm
  if (PS4.PSButton()){
    armed = !armed;
    if (armed == true){
      Serial.println("!System Armed!");
      HC12.write("Armed");
    }else if (armed == false){
      Serial.println("System Disarmed");
      HC12.write("Disarmed");
    }
  }
  if (PS4.R2())){
    
  }
}  

  delay(100);

}
