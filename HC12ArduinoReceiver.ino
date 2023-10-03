// See SoftwareSerial example 
// https://www.arduino.cc/en/tutorial/SoftwareSerialExample

#include <SoftwareSerial.h>;
#include <Servo.h>

#define RX 2 //Connect to the TX pin of the HC-12
#define TX 3 //Connect to the RX pin of the HC-12
Servo ESC; 
SoftwareSerial HC12(RX, TX);

bool armed = false;
byte incomingByte;
char readBuffer;
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
int rudderLeft;
int d1; //setup to reference delimiter locations so its easier to follow
int d2;
int d3;
int d4;
int d5;
int d6;

const char delimiter = ",";

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
  ESC.attach(9,1000,2000);
}

void loop() { // run over and over

  while (HC12.available() && armed == false) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    //readBuffer = HC12.read();
    Serial.write(readBuffer);
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
    HC12.write("On-Board Armed");
    startup = true;
  }
  
  if (armed == true && startup == true){
    input = HC12.readStringUntil('\n');
    if (input.length() > 0) {
        Serial.println(input);

        d1 = input.indexOf(delimiter);
        throttle = input.substring(0, d1).toInt();
    
        d2 = input.indexOf(delimiter, d1+1);
        LStickX = input.substring(d1+1, d2).toInt();

        d3 = input.indexOf(delimiter,d2+1);//you can add 2 variables, or 4... by adding these 4 lines 
        LStickY = input.substring(d2+1, d3).toInt();//you can add 2 variables, or 4... by adding these 4 lines
    
        d4 = input.indexOf(delimiter, d3+1);//you can add 2 variables, or 4... by adding these 4 lines 
        RStickX = input.substring(d3+1, d4).toInt();//     under this line

        d5 = input.indexOf(delimiter, d4+1);//you can add 2 variables, or 4... by adding these 4 lines 
        RStickY = input.substring(d4+1, d5).toInt();//you can add 2 variables, or 4... by adding these 4 lines
    
        d6 = input.indexOf(delimiter, d5+1);//you can add 2 variables, or 4... by adding these 4 lines 
        rudderRight = input.substring(d5+1, d6).toInt();//     under this line
    
        rudderLeft = input.substring(d6+1).toInt();//but before this one
      //if you change the number of variables, follow instructions in comments above
      //duplicating the lines as explained adds 2 variables, then you can duplicate them again, and then change the variable names so they are in the right order
    }
  }
  motorControl();
  //Serial.println(throttle);

  readBuffer = "";
  delay(100);
}

void motorControl(){
  
  throttle = map(throttle, 0, 255, 0, 240);
  //Serial.println(throttle);
  ESC.write(throttle);

}
