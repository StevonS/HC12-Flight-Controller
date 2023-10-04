#include <PS4Controller.h>
#define RXD2 16	//(RX2)
#define TXD2 17	//(TX2)
#define HC12 Serial2  //Hardware serial 2 on the ESP32
bool armed = false;
byte incomingByte;
String readBuffer = "";
bool startup = false;
int connectionStatus;

void setup() {
  Serial.begin(115200);
  PS4.begin("EC:62:60:1E:E1:68");
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);           //Normally HIGH, LOW for settings
  Serial.begin(115200);           // Serial port to computer
  HC12.begin(9600, SERIAL_8N1, RXD2, TXD2);   
  Serial.println("Ready.");
}

void loop() {

  while (connection == false){
    HC12.write("Connection");
    
  }
  
  controller();
  while (HC12.available()) {
    incomingByte = HC12.read();
    readBuffer += char(incomingByte);        
    Serial.write(HC12.read());  

  }
  while (Serial.available()) {      
    HC12.write(Serial.read());   

  }

  
  
  if (readBuffer == "On-Board Armed"){
    startup = true;
    Serial.println("Starting Transmission:");
  }
  delay(200);
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
  //PS4.setLed(uint8_t r, uint8_t g, uint8_t b)
  if (armed == true && startup == true){
    HC12.print(PS4.R2Value());//sends the variables
    HC12.print(",");
    HC12.print(PS4.LStickX());
    HC12.print(",");
    HC12.print(PS4.LStickY());//if you just need to send 2 variables,simply change this value and the following to 0
    HC12.print(",");
    HC12.print(PS4.RStickX());//if you just need to send 3 variables,simply change this value and the next to 0
    HC12.print(",");
    HC12.print(PS4.RStickY());//if you just need to send 4 variables,simply change this value to 0
    HC12.print(",");
    HC12.print(PS4.R1());//if you just need to send 3 variables,simply change this value and the next to 0
    HC12.print(",");
    HC12.print(PS4.L1());//if you just need to send 4 variables,simply change this value to 0
    HC12.println("");
    HC12.print(ConnectionStatus);
  //you can add even numbers of variables, ex add 2, 4, 6 ...(so 3, 5, 7, 9... variables in total) before this line by adding a variable line, then a comma line, another variable line then a second comma line under the 5th variable line but before the println.  
  //for the purpose of simplicity, it is easier to add in groups of 2, so that less modification is necessary on the other end
  //if you have more variables than you are using, change the variable to 0, as above explained, again for the purpose of simplicity
  }


}
