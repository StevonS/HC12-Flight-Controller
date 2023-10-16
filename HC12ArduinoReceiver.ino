// See SoftwareSerial example 
// https://www.arduino.cc/en/tutorial/SoftwareSerialExample

#include <SoftwareSerial.h>
#include <Servo.h>;

#define RX 2 //Connect to the TX pin of the HC-12
#define TX 3 //Connect to the RX pin of the HC-12

Servo ESC; 
Servo Elevators;
Servo AileronL;
Servo AileronR;
Servo Rudder;
SoftwareSerial HC12(RX, TX);


bool armed = false;
byte incomingByte;
String readBuffer;
bool startup = false;
String input;
int boundLow;
int boundHigh;
int throttle = 0;
int LStickX = 0;
int LStickY = 0;
int RStickX = 0;
int RStickY = 0;
int rudderRight = 0;
int rudderLeft = 0;
int status = 0;
int rearm = 0;
int buffer;
int d1; //setup to reference delimiter locations so its easier to follow
int d2;
int d3;
int d4;
int d5;
int d6;
int d7;
int d8;
int elevatorValue;
int aileronValue;
int speed;

const char delimiter = ",";

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  

}

void arm(){
  setSpeed(0); //Sets speed variable
  delay(2000);
}

void loop() {

  while (HC12.available() && armed == false) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    //Serial.println(char(incomingByte));

    if (readBuffer == "Connection Request"){
      HC12.write("Connection Established");  
      Serial.print("Connection");
    }
  }
  
  
  if (readBuffer == "Armed") {
    Serial.println("ARMED");
    armed = true;
  }
  if (readBuffer == "Disarmed") {
    Serial.println("DISARMED");
    armed = false;
  }

  if (armed == true && startup == false) {
    //SYSTEMS STARTUP
    Serial.println("startup");
    ESC.attach(9,800,2000);
    Elevators.attach(11);
    AileronL.attach(10);
    AileronR.attach(6);
    Rudder.attach(5);
    arm();
    startup = true;
    HC12.write("Onboard Armed");
  }
  
  if (armed == true && startup == true) {
      input = HC12.readStringUntil('\n');
      if (input.length() > 0) {
        Serial.println(input);

        // Split the input into substrings using the delimiter ","
        String values[9];
        int numValues = splitString(input, values, 9);
        Serial.println(numValues);
        if (numValues == 9) {
          throttle = values[0].toInt();
          LStickX = values[1].toInt();
          LStickY = values[2].toInt();
          RStickX = values[3].toInt();
          RStickY = values[4].toInt();
          rudderRight = values[5].toInt();
          rudderLeft = values[6].toInt();
          status = values[7].toInt();
          rearm = values[8].toInt();
        }
        
      }
    }

  if(status != 1 && armed == true && startup == true){
    emercencyProcedures();
  }

  if(rearm == 1){
    arm();
    Serial.println("rearmed");
  } else{
    setSpeed(throttle);
  }
  
  aileronController();
  elevatorControl();
  rudderControl();
  
  readBuffer = "";
  status = 0;
  delay(200);
}

void emercencyProcedures(){
 Serial.println("Emergency!");
}

int splitString(String input, String values[], int maxValues) {
  int index = 0;
  int lastIndex = 0;
  int numValues = 0;

  while (index < input.length() && numValues < maxValues) {
    if (input.charAt(index) == ',') {
      values[numValues] = input.substring(lastIndex, index);
      lastIndex = index + 1;
      numValues++;
    }
    index++;
  }

  if (lastIndex < index && numValues < maxValues) {
    // Add the last value (if any) to the array
    values[numValues] = input.substring(lastIndex);
    numValues++;
  }

  return numValues;
}

void aileronControl(){
  if (RStickX > 15 && RStickX < -15){
      aileronValue = map(RStickX, -2,2 , 40,120);
    
      AileronL.write(aileronValue);
      AileronR.write(aileronValue);
  }  
}

void eleavtorControl(){
  if (RStickY > 15 && RStickY < -15 ){
    elevatorValue = map(RStickY, -2,2 , 40,120);
    Elevators.write(elevatorValue);
  }
}

void rudderControl(){
  if (rudderRight == 1){
    Serial.println("RudderRight");
    Rudder.write(40);
  }
  if (rudderLeft == 1){
    Serial.println("RudderLeft");
    Rudder.write(120);
  }
  if(rudderRight == 0 && rudderLeft == 0) {
    Rudder.write(80);
  }

}


void setSpeed(int speed){
  int angle = map(speed, 0, 255, 0, 200); //Sets servo positions to different speeds 
  ESC.write(angle);
}
