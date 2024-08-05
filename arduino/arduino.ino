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

//math constants for calculating the arm positon in space
const float arms = 6.0;
const float pi = 3.1415;

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
    
    //converts the string into a char list 
    char* inputChar = new char[receivedString.length()+1];
    strcpy(inputChar, receivedString.c_str());
    
    //Serial.println(inputChar); //for testing

    //goes through the array to find the strings for each command
    for(int i =0; i<sizeof(inputChar)/sizeof(inputChar[0]); i+=6){
      /*
      Serial.println(receivedString);
      Serial.println(inputChar[i+0]);
      Serial.println(inputChar[i+1]);
      Serial.println(inputChar[i+2]);
      Serial.println(inputChar[i+3]);
      Serial.println(inputChar[i+4]);
      Serial.println(inputChar[i+5]);
      */
     //for testing

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
    //Serial.println(receivedString); //for testing
    
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

//automitcally moves the claw to correct location
boolean closeClaw(){
  moveServo(claw, 180); //close with torque
}
boolean openClaw(){
  moveServo(claw, 90); //open halfway
}

//helper function for mainMover to move the 2 arm servos and base to the correct location
void moveToAngle(float base, float angle1, float angle2){
  //just the arm not the wrist or claw 
  servo1.write(base);
  servo2.write(angle1);
  servo3.write(angle2);
}

//based on a position in 3d space calculates the angles that the servos need to move to work
void mainMover(float x, float y, float z){
  float base = atan2(y, x) * (180/pi);
  float hyonpot = sqrt(x * x + y * y); //pythagorean
  float height = sqrt(hyonpot * hyonpot + z * z); 
  float phi = atan(z/hyonpot) * (180/pi); \\horizontal angle
  float theta = acos((hyonpoy / 2)/arms) * (180/pi);

  float angle1 = phi + theta;
  float angle2 = phi - theta;

  moveToAngle(base, angle1, angle2);
}

//coverts the input to int locations in space and feeds them to mainMover
boolean automove(char x, char y, char z){
  int xInt = int(x);
  int yInt = int(y);
  int zInt = int(z);

  mainMover(subChar(xInt), subChar(yInt), subChar(zInt));
  return true;
}

//helper function for autoMove
//converts an int in the x y or z into the correct floating dimension for autoMove
float subChar(int x){
  return 0.5(x - 97);
}