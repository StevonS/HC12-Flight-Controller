// See SoftwareSerial example 
// https://www.arduino.cc/en/tutorial/SoftwareSerialExample

#include <SoftwareSerial.h>
#include <Servo.h>

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

const char delimiter = ",";

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  ESC.attach(9,1000,2000);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);   
}

void loop() { // run over and over

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
    HC12.write("Onboard Armed");
    startup = true;
  }
  
  if (armed == true && startup == true){
    input = HC12.readStringUntil('\n');
    if (input.length() > 0) {
        Serial.println(input);

       d1 = input.indexOf(delimiter);
        throttle = input.substring(0, d1).toInt();
    
        d2 = input.indexOf(delimiter, d1+1);
        LStickX = input.substring(d1, d2).toInt();

        d3 = input.indexOf(delimiter,d2+1);//you can add 2 variables, or 4... by adding these 4 lines 
        LStickY = input.substring(d2, d3).toInt();//you can add 2 variables, or 4... by adding these 4 lines
    
        d4 = input.indexOf(delimiter, d3+1);//you can add 2 variables, or 4... by adding these 4 lines 
        RStickX = input.substring(d3, d4).toInt();//     under this line

        d5 = input.indexOf(delimiter, d4+1);//you can add 2 variables, or 4... by adding these 4 lines 
        RStickY = input.substring(d4, d5).toInt();//you can add 2 variables, or 4... by adding these 4 lines
    
        d6 = input.indexOf(delimiter, d5+1);//you can add 2 variables, or 4... by adding these 4 lines 
        rudderRight = input.substring(d5, d6).toInt();//     under this line

        d7 = input.indexOf(delimiter, d6+1);//you can add 2 variables, or 4... by adding these 4 lines 
        rudderLeft = input.substring(d6, d7).toInt();//     under this line

        d8 = input.indexOf(delimiter, d7+1);//you can add 2 variables, or 4... by adding these 4 lines 
        status = input.substring(d7, d8).toInt();//  

        buffer = input.substring(d8).toInt();//but before this one
      //if you change the number of variables, follow instructions in comments above
      //duplicating the lines as explained adds 2 variables, then you can duplicate them again, and then change the variable names so they are in the right order
      delay(10);
    }
  }

  if(status != 1 && armed == true && startup == true){
    emercencyProcedures();
  }
  motorController();
  servoController();

  readBuffer = "";
  status = 0;
  delay(200);
}

void emercencyProcedures(){
  Serial.println(d1);
  Serial.println(d2);
  Serial.println(d3);
  Serial.println(d4);
  Serial.println(d5);
  Serial.println(d6);
  Serial.println(d7);
  Serial.println(d8);
}

void servoController(){
  if (RStickX > 15 && RStickX < -15){
      aileronValue = map(RStickX, -2,2 , 60,120);
    
      AileronL.write(aileronValue);
      AileronR.write(aileronValue);
  }
  if (RStickY > 15 && RStickY < -15 ){
    elevatorValue = map(RStickY, -2,2 , 60,120);
    Elevators.write(elevatorValue);

  }
  if (rudderRight == 1){
  Rudder.write(70);
  }
  if (rudderLeft == 1){
  Rudder.write(110);
  }
}


void motorController(){
  
  throttle = map(throttle, 0, 255, 0, 240);
  ESC.write(throttle);

}
