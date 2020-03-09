#include<Wire.h>
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
#define home_switch 9
bool execute = 0;
bool activate = 0;
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(home_switch, INPUT_PULLUP);
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
  }
void loop() {
if(execute){
stepper();
execute = 0;
}
}
void stepper()
{while (activate) 
  { 
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
 digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500);
if (!digitalRead(home_switch)) {
       activate = 1;
}}
  if (digitalRead(home_switch)){
    activate = 0;} 
  }
void receiveEvent(int howMany){
 execute = Wire.read();
}
