#include "Odometer.h"
#include "Arduino.h"
#include <Encoder.h>
#define RADIUS 3.2
#define WHEEL_DISTANCE 27.6
#define Kp 0.20*2
#define Ka 0.76*10
#define KpA 0.20*2.5                       ////2.5 et 10 et boost de 3.5 pour distance de 20 avec 0.7 pour les moteur
#define KaA 0.76*10                        ////2.5 - 10 0.8 - 3.5
#define VMAX 8
#define MAPMAX 255


Encoder myEncR(19,18);
Encoder myEncL(3,2);

Odometer* odometer;
long oldPrintTime = 0;
double errorTheta = 1000.0;
double errorDistance = 1000.0;
double RegulationCoefficient = 0.82;
double RegulationCoefficientA = 0.7;
double desX = 10.0;
double desY = 0.0;
double V = 0.0;
double W = 0.0;
double V1 = 0.0;
double V2 = 0.0;
double U1 = 0.0;
double U2 = 0.0;
double sensed = 0.0;
double senseg = 0.0;
int in1g = 4 ;//6 7 4 5
int in2g = 5 ;
int in1d = 7 ;
int in2d = 6 ;
int ed = 8 ;
int eg = 9 ;
bool execute = 1;
int boost = 1;

void setup() {
  //analogWrite(in1g,255);
  //analogWrite(in1d,255);   //////TEST
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");

  odometer = new Odometer(20.0,WHEEL_DISTANCE);
  pinMode(in1g,OUTPUT);
  pinMode(in2g,OUTPUT);
  pinMode(in1d,OUTPUT);
  pinMode(in2d,OUTPUT);
  pinMode(ed,OUTPUT);
  pinMode(eg,OUTPUT);
  digitalWrite(ed,HIGH);
  digitalWrite(eg,HIGH);
}

void odometryUpdate();
void goalReach(double desiredX, double desiredY);
void printState();
////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void loop() 
{
if(execute)
{ execute = 0;
  /********************odometry update*********************/
  odometryUpdate();
  /**************end odometry update**************/

  /*******************control inputs**************/
  
 /*while( errorDistance > 1.0) {marcheArriere(-20.0, 0.0); odometryUpdate();printState();}
  Serial.println("done!");
  printState();
   errorTheta = 1000.0;
  errorDistance = 1000.0;
 odometryUpdate();
  delay(1000);*/
   while( errorDistance > 1.0) {goalReach(20.0, 0.0); odometryUpdate();printState();}
  //Serial.println("done!");
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorDistance > 1.0) {goalReach(30.0, 0.0); odometryUpdate();printState();}
  //Serial.println("done!");
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorDistance > 1.0) {goalReach(40.0, 0.0); odometryUpdate();printState();}
  //Serial.println("done!");
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorDistance > 1.0) {goalReach(50.0, 0.0); odometryUpdate();printState();}
  //Serial.println("done!");
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
 /* while( errorTheta > 0.1) {turnInPlace(50.0, 0.0,1.70); odometryUpdate();}
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorDistance > 1) {goalReach(50.0, 50.0); odometryUpdate();}
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorTheta > 0.1) {turnInPlace(50.0, 50.0,2*1.70 ); odometryUpdate();}
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorDistance > 1) {goalReach(0.0, 50.0); odometryUpdate();}
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorTheta > 0.1) {turnInPlace(0.0, 50.0 , 3*1.70 ); odometryUpdate();}
  printState(); 
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  delay(1000);
  while( errorDistance > 1) {goalReach(0.0, 0.0); odometryUpdate();}
  printState();
  errorTheta = 1000.0;
  errorDistance = 1000.0;
  
  //turnInPlace(0.0,0.0,1.58);
*/
  /**********************print state*********************/
  printState();
  /**********************end print state*****************/
    
}  
}

void turng()
{
  if(senseg == 1){
  analogWrite(in1g,U1);
  analogWrite(in2g,0);  
  }
  if(senseg == -1){
  analogWrite(in2g,U1);
  analogWrite(in1g,0);  
  }
}

void turnd()
{
  if(sensed == 1){
  analogWrite(in1d,U2);
  analogWrite(in2d,0);  
  }
  if(sensed == -1){
  analogWrite(in2d,U2);
  analogWrite(in1d,0);  
  }
}

void odometryUpdate()
{
  long positionR = myEncR.read();
  long positionL = myEncL.read();
  odometer->process(positionR*2*PI*RADIUS/(480*4),positionL*2*PI*RADIUS/(480*4));
}

void goalReach(double desiredX, double desiredY)
{ 
  errorTheta = atan2(desiredY - odometer->getCurrentY(), desiredX - odometer->getCurrentX()) - odometer->getCurrentTheta();
  errorTheta = convertAngle(errorTheta);
  errorDistance = sqrt((desiredY - odometer->getCurrentY())*(desiredY - odometer->getCurrentY()) + (desiredX - odometer->getCurrentX())*(desiredX - odometer->getCurrentX()) );
  if(errorDistance >1.0)
  {
      if ( errorDistance < 3) boost = 2;
      V = boost*Kp*errorDistance*cos(errorTheta);
      
      W =  Kp*sin(errorTheta)*cos(errorTheta) + Ka*errorTheta;
      V2=(2*V + W*WHEEL_DISTANCE)/2;
      V1=(2*V - W*WHEEL_DISTANCE)/2;
      V1 = max (-VMAX,min(V1,VMAX));
      V2 = max (-VMAX,min(V2,VMAX));
      if ( V1 < 0){senseg =-1; }
      if ( V1 > 0){senseg =1; }
      if ( V2 < 0){sensed =-1; }
      if ( V2 > 0){sensed =1; }
      U1 = map(abs(V1) , 0 , 12, 0 , MAPMAX);
      U2 = map(abs(V2) , 0 , 12, 0 , MAPMAX);
      U2 = RegulationCoefficient * U2 ;

  }
  else
  {
      U1 = 0.0;
      U2 = 0.0;
  }
  turnd();
  turng();

}

void printState()
{
  if (millis() - oldPrintTime >= 1000) 
  {
    oldPrintTime = millis();
    Serial.println(errorDistance);
    Serial.println(odometer->getCurrentX());
    Serial.println(odometer->getCurrentY());
    Serial.println(odometer->getCurrentTheta()*(180/PI));
        Serial.println("///////////////////////////////////");
    
    //Serial.println(V1);
    //Serial.println(V2);
    //Serial.println(U1);
    //Serial.println(U2);
    //Serial.println(V);
    //Serial.println(W);
    Serial.println("++++++++++++++++++++++++++++++++++++++++++++");
  }
}

void turn(double desiredTheta)
{
  errorTheta = desiredTheta - odometer->getCurrentTheta();
  errorTheta = convertAngle(errorTheta);
  if(abs(errorTheta) > 0.1)
  {
      //V = Kp*errorDistance*cos(errorTheta);
      W = Kp*sin(errorTheta)*cos(errorTheta) + 0.1*Ka*errorTheta;
      V2=W*WHEEL_DISTANCE/2;
      V1=-W*WHEEL_DISTANCE/2;
      V1 = max (-VMAX,min(V1,VMAX));
      V2 = max (-VMAX,min(V2,VMAX));
      if ( V1 < 0){senseg =1; }
      if ( V1 > 0){senseg =-1; }
      if ( V2 < 0){sensed =1; }
      if ( V2 > 0){sensed =-1; }
      U1 = map(abs(V1) , 0 , 12, 0 , MAPMAX);
      U2 = map(abs(V2) , 0 , 12, 0 , MAPMAX);


  }
  else
  {
      U1 = 0.0;
      U2 = 0.0;
  }
  turnd();
  turng();

}

double convertAngle(double angle)
 { 
  // Represent the angle between -PI and PI
  while( abs(angle) > PI )
  {
    if (angle > 0)
    {
      angle = angle - 2*PI;
    }
    else
    {
      angle = angle + 2*PI;
    }
  }
  
  return angle;
 
 }

void turnInPlace(double desiredX, double desiredY, double desiredTheta)
{
  errorTheta = desiredTheta - odometer->getCurrentTheta();
  errorTheta = convertAngle(errorTheta);
  errorDistance = sqrt((desiredY - odometer->getCurrentY())*(desiredY - odometer->getCurrentY()) + (desiredX - odometer->getCurrentX())*(desiredX - odometer->getCurrentX()) );
  if(abs(errorTheta) > 0.1)
  {
      V = 0.1*Kp*errorDistance*cos(errorTheta);
      W = Kp*sin(errorTheta)*cos(errorTheta) + 0.1*Ka*errorTheta;
      V2=(2*V + W*WHEEL_DISTANCE)/2;
      V1=(2*V - W*WHEEL_DISTANCE)/2;
      V1 = max (-VMAX,min(V1,VMAX));
      V2 = max (-VMAX,min(V2,VMAX));
      if ( V1 < 0){senseg =-1; }
      if ( V1 > 0){senseg =1; }
      if ( V2 < 0){sensed =-1; }
      if ( V2 > 0){sensed =1; }
      U1 = map(abs(V1) , 0 , 12, 0 , MAPMAX);
      U2 = map(abs(V2) , 0 , 12, 0 , MAPMAX);


  }
  else
  {
      U1 = 0.0;
      U2 = 0.0;
  }
  turnd();
  turng();

}

void marcheArriere(double desiredX, double desiredY)
{ 
  errorTheta = atan2(desiredY - odometer->getCurrentY(), desiredX - odometer->getCurrentX()) - odometer->getCurrentTheta();
  errorTheta = PI - errorTheta ;
  errorTheta = convertAngle(errorTheta);
  errorDistance = sqrt((desiredY - odometer->getCurrentY())*(desiredY - odometer->getCurrentY()) + (desiredX - odometer->getCurrentX())*(desiredX - odometer->getCurrentX()) );
  if(errorDistance >1.0)
  {
      if ( errorDistance < 3) boost = 3.5;
      V = - boost*KpA*errorDistance*cos(errorTheta);
      boost=1;
      W =  (KpA*sin(errorTheta)*cos(errorTheta) + KaA*errorTheta);
      V1 = (2*V + W*WHEEL_DISTANCE)/2;
      V2 = (2*V - W*WHEEL_DISTANCE)/2;
      V1 = max (-VMAX,min(V1,VMAX));
      V2 = max (-VMAX,min(V2,VMAX));
      if ( V1 < 0){senseg =-1; }
      if ( V1 > 0){senseg =1; }
      if ( V2 < 0){sensed =-1; }
      if ( V2 > 0){sensed =1; }
      U1 = map(abs(V1) , 0 , 12, 0 , MAPMAX);
      U2 = map(abs(V2) , 0 , 12, 0 , MAPMAX);
      U2 = RegulationCoefficientA * U2 ;

  }
  else
  {
      U1 = 0.0;
      U2 = 0.0;
  }
  turnd();
  turng();

}
