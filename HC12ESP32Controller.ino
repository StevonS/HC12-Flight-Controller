#include <PS4Controller.h>
#define RXD2 16	//(RX2)
#define TXD2 17	//(TX2)
#define HC12 Serial2  //Hardware serial 2 on the ESP32

bool armed = false;
byte incomingByte;
String readBuffer;
bool startup = false;
int connectionStatus = 1;
bool reply = false;
bool connection = false;
int LStickX;
int LStickY;
int RStickX;
int RStickY;


void setup() {
  Serial.begin(115200);
  PS4.begin("EC:62:60:1E:E1:6A");
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);           //Normally HIGH, LOW for settings
  Serial.begin(115200);           // Serial port to computer
  HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);   
  Serial.println("Ready.");
}

void loop() {


  
  
  controller();
  if (connection == false){
    HC12.write("Connection Request");
  }
  
 
  while (HC12.available()) {
    incomingByte = HC12.read();
    readBuffer += char(incomingByte);        
  }

  Serial.println(readBuffer);
  if(readBuffer == "Connection Established" && connection == false){
    connection = true;
    Serial.println("Connection");
  }
  
  
  if (readBuffer == "Onboard Armed"){
    startup = true;
    Serial.println("Starting Transmission:");
  }
  readBuffer = "";
  delay(200);
}


void controller(){

  //Arm/Disarm
  if (PS4.PSButton()){
    delay(500);
    armed = !armed;
    if (armed == true){
      Serial.println("!System Armed!");
      HC12.write("Armed");
    }else if (armed == false){
      Serial.println("System Disarmed");
      HC12.write("Disarmed");
    }
  }
  
  if (PS4.LStickX() > 15 || PS4.LStickX() < -15){
    LStickX = PS4.LStickX();
  }else{
    LStickX = 0;
  } 
  if (PS4.LStickY() > 15 || PS4.LStickY() < -15){
    LStickY = PS4.LStickY();
  }else{
    LStickY = 0;
  } 
  if (PS4.RStickX() > 15 || PS4.RStickX() < -15){
    RStickX = PS4.RStickX();
  }else{
    RStickX = 0;
  } 
  if (PS4.RStickY() > 15 || PS4.RStickY() < -15){
    RStickY = PS4.RStickY();
  }else{
    RStickY = 0;
  } 

  //PS4.setLed(uint8_t r, uint8_t g, uint8_t b)
  if (armed == true && startup == true){
    HC12.print(PS4.R2Value());//sends the variables
    HC12.print(",");
    HC12.print(LStickX);
    HC12.print(",");
    HC12.print(LStickY);//if you just need to send 2 variables,simply change this value and the following to 0
    HC12.print(",");
    HC12.print(RStickX);//if you just need to send 3 variables,simply change this value and the next to 0
    HC12.print(",");
    HC12.print(RStickY);//if you just need to send 4 variables,simply change this value to 0
    HC12.print(",");
    HC12.print(PS4.R1());//if you just need to send 3 variables,simply change this value and the next to 0
    HC12.print(",");
    HC12.print(PS4.L1());//if you just need to send 4 variables,simply change this value to 0
    HC12.print(",");
    HC12.print(connectionStatus);
    HC12.print(",");
    HC12.print(PS4.Triangle());
    HC12.println("");
    
  }


}
