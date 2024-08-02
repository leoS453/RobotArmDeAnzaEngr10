//imports servo library to control servos
#include <Servo.h> //ignore the error it works in arduino.ide
#include <math.h>
using namespace std;  //for the c++ stuff

//defines servo objects
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

Servo claw; //specifically for the claw cause it has different controls 


//defines pin for led used for testing
int pinNum = 10;

void setup() 
{ 
  //sets light led pin as output
  pinMode(pinNum, OUTPUT);

  //sets pins for servo objects
  //set as sequential order starting from the first digital pin
  //might be better to use just the outer ones
  servo1.attach(22);
  servo2.attach(23);
  servo3.attach(24);
  servo4.attach(25);
  servo5.attach(26);
  servo6.attach(27);

  claw = servo6; //assumes the claw is servo6

  // Initialize the Serial
  Serial.begin(9600);
  
}

void loop() {

  // Check to see if Serial data is being received
  if (Serial.available() > 0) {
    
    // Create a new string variable to receive Serial data
    String receivedString = "";
    // Loop through received data and append to the receivedString variable
    while (Serial.available() > 0) {
      receivedString += Serial.readString();
    }
    
    //kinda undoes the thing from before ill make this better later
    //char inputChar[receivedString.length()+1]= {receivedString.c_str()}; //this is like giving an error i think 
    char* inputChar = new char[receivedString.length()+1];
    strcpy(inputChar, receivedString.c_str());
    Serial.println(inputChar);
    //goes through the array to find the strings for each command
    for(int i =0; i<sizeof(inputChar)/sizeof(inputChar[0]); i+=6){
      Serial.println(receivedString);
      Serial.println(inputChar[i+0]);
      Serial.println(inputChar[i+1]);
      Serial.println(inputChar[i+2]);
      Serial.println(inputChar[i+3]);
      Serial.println(inputChar[i+4]);
      Serial.println(inputChar[i+5]);

      //ensures its reading it properly
      if(inputChar[i] != '!'){
        //bad
      }
      //finds what command
      switch(inputChar[i+1]){
        //***************** Manual Control **************

        //control servo 1,2,3,4,5,6 individually 
        char* str;
        case '1':
          str = inputChar[i+3] + inputChar[i+4] + inputChar[i+5];
          moveServo(servo1, atoi(str));
          Serial.write("works");
          break;
        case '2':
          str = inputChar[i+3] + inputChar[i+4] + inputChar[i+5];
          moveServo(servo2, atoi(str));
          break;
        case '3':
          str = inputChar[i+3] + inputChar[i+4] + inputChar[i+5];
          moveServo(servo3, atoi(str));
          break;
        case '4':
          str = inputChar[i+3] + inputChar[i+4] + inputChar[i+5];
          moveServo(servo4, atoi(str));
          break;
        case '5':
          str = inputChar[i+3] + inputChar[i+4] + inputChar[i+5];
          moveServo(servo5, atoi(str));
          break;
        case '6':
          str = inputChar[i+3] + inputChar[i+4] + inputChar[i+5];
          moveServo(servo6, atoi(str));
          break;
        
        //***************** Automatic Control **************
        case 'A':
          autoMove(inputChar[i+3],inputChar[i+4],inputChar[i+5]);
          break;

        //***************** Claw Control **************
        case 'C':
          closeClaw();
          break;
        case 'O':
          openClaw();
          break;

        //control the led
        case 'L':
          Serial.write("lights work");
          if(inputChar[i+3] == 'I'){
            digitalWrite(pinNum, HIGH);
          }else{
            digitalWrite(pinNum, LOW);
          }
      }
    }

    // Print received Serial data
    //Serial.println(receivedString);
    
  }

}

//moves servo in a less violent way
boolean moveServo(Servo &servo, int degree){
  const int timing = 10;
  currentpos = servo.read();
  if(currentpos== degree){
    //do nothing
    return true;
  }
  if(currentpos>degree){
    for(int i = servo.read()-1; i>=degree; i--){
      servo.write(i);
      delay(timing);
    }
  }else{
    for(int i = currentpos+1; i<=degree; i++){
      servo.write(i);
      delay(timing);
    }
  }
  return true;
}

boolean autoMove(char char1, char char2, char char3){
  return true; 
}

boolean closeClaw(){
  moveServo(claw, 180);
}
boolean openClaw(){
  moveServo(claw, 90);
}

float baseservo(float z){
   float angle = cos(radians(z));
   if (angle >= 0 && angle <= 180){
    moveServo(servo1, x);
   }
}