#include<Wire.h>
#include <Servo.h>
int angle;
//int servoPin =  ; // Declare the Servo pin 
Servo Servo1; // Create a servo object
int i;
int k;
int msg;
int trig1 = 2 ;
int trig2 = 4 ;
int trig3 = 6 ;
int trig4 = 8 ;
int trig5 = 10 ;
int trig6 = 12 ;
int echo1 = 3 ;
int echo2 = 5 ;
int echo3 = 7 ;
int echo4 = 9 ;
int echo5 = 11 ;
int echo6 = 13 ;
long d1,d2,d3,d4,d5,d6,duration1,duration2,duration3,duration4,duration5,duration6;
 //int seg1=2;
 //int seg2=9;
 //int seg3=;
 int chiffre1=0;
 int chiffre2=0;
 int chiffre3=0;
 int score = 0;
 bool execute_picking = 0;
 bool execute_releasing = 0; 
 void setup () 
 {
  pinMode (trig1, OUTPUT);
  pinMode (trig2, OUTPUT);
  pinMode (trig3, OUTPUT);
  pinMode (trig4, OUTPUT);
  pinMode (trig5, OUTPUT);
  pinMode (trig6, OUTPUT);
  pinMode (echo1, INPUT);
  pinMode (echo2, INPUT);
  pinMode (echo3, INPUT);
  pinMode (echo4, INPUT);
  pinMode (echo5, INPUT);
  pinMode (echo6, INPUT); 
  
  for (k=seg1;k<=(seg1+7);k++)
{  pinMode (k, OUTPUT);
 }
  for (k=seg2;k<=(seg2+7);k++)
 { pinMode (k, OUTPUT);
 }
  for (k=seg3;k<=(seg3+7);k++)
 { pinMode (k, OUTPUT);
 }  
 pinMode(12;OUTPUT);
 pinMode(13;OUTPUT);
 Wire.begin(4); 
 Wire.onReceive(receiveEvent);
 Wire.onRequest(requestEvent);
 Servo1.attach(servoPin); // We need to attach the servo to the used pin number
  }
   void loop ()
   {
    checkback();
    checkfront();
    
    afficheScore(score);
    
    if(execute_picking){
           gold();
           execute_picking = 0;
    }
    if(execute_releasing)
    {
      gold_release();
      execute_releasing=0;
    }
}

void initialisation (int t)
{
   for (i=t;i<=(t+7);i++)
    {
      digitalWrite(i,LOW);
      }
  }
  void writeSignal (int a)
  {
   
    digitalWrite(a,HIGH);
    }
 
 
 void ledaffiche (int nombre,int seg)
    {
      if (nombre==9)
      {
     writeSignal(seg);
     writeSignal(seg+1);
     writeSignal(seg+2);
     writeSignal(seg+5);
     writeSignal(seg+6);
        }
        if (nombre==8)
        {
     writeSignal(seg);
     writeSignal(seg+1);
     writeSignal(seg+2);
     writeSignal(seg+3);
     writeSignal(seg+4);
     writeSignal(seg+5);
     writeSignal(seg+6);
          }
              if (nombre==7)
        {
     writeSignal(seg);
     writeSignal(seg+1);
     writeSignal(seg+2);
          }
              if (nombre==6)
        
    {writeSignal(seg);
     writeSignal(seg+2);
     writeSignal(seg+3);
     writeSignal(seg+4);
     writeSignal(seg+5);
     writeSignal(seg+6);
          }
              if (nombre==5)
        {
     writeSignal(seg);
     writeSignal(seg+2);
     writeSignal(seg+3);
     writeSignal(seg+5);
     writeSignal(seg+6);
          }
              if (nombre==4)
        
     {
      writeSignal(seg+1);
      writeSignal(seg+2);
      writeSignal(seg+5);
      writeSignal(seg+6);
          }
              if (nombre==3)
        
     {writeSignal(seg);
      writeSignal(seg+1);
      writeSignal(seg+2);
      writeSignal(seg+3);
      writeSignal(seg+6);
         }
          if (nombre==2)
        
     {writeSignal(seg);
      writeSignal(seg+1);
      writeSignal(seg+3);
      writeSignal(seg+4);
      writeSignal(seg+6);
     }
         if (nombre==1)
        {
  
      writeSignal(seg+1);
      writeSignal(seg+2);

          }
              if (nombre==0)
        
     {
      writeSignal(seg);
      writeSignal(seg+1);
      writeSignal(seg+2);
      writeSignal(seg+3);
      writeSignal(seg+4);
      writeSignal(seg+5);
     }}
void  afficheScore(int score)
  {chiffre1=score%10;
  chiffre2=((score%100)-chiffre1)/10;
  chiffre3=((score%1000)-chiffre2*10-chiffre1)/100;
  initialisation(seg1);
  ledaffiche(chiffre1,seg1);
  initialisation(seg2);
  ledaffiche(chiffre2,seg2);  
    initialisation(seg3);
  ledaffiche(chiffre3,seg3);
  }
  void gold(){
    angle=120;
    map(angle,0,270,0,180);
    Servo1.write(angle); 
    delay(2000);
    pick_gold();
    //Servo1.write(0);
  }
  void receiveEvent(int howMany){
 msg = Wire.read();
 if(msg == 1)
 {
  execute_picking = 1;
 }
 if(msg == 2)
 {
  execute_releasing = 1;
 }
 if (msg > 2) {
  score = msg; }
}  
void pick_gold(){
 pompeon(); 
 electro_vanne_passant();
 delay(2000);
 pompeoff();
}
void release_gold()
{
  electro_vanne_ouvrant();
  
}

void pompeon(){
  digitalWrite(12,LOW);
  }
void pompeoff(){
  digitalWrite(12,HIGH);
  }
void electro_vanne_passant(){
digitalWrite(13,HIGH);  
}
void electro_vanne_ouvrant(){
digitalWrite(13,LOW);  
}
void checkfront(){
  digitalWrite(trig1,LOW);
  delayMicroseconds(2);
  digitalWrite(trig1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1,LOW);
  duration1 = pulseIn(echo1,HIGH);
  d1 = duration1/29/2;
  
  delayMicroseconds(50);
  
  digitalWrite(trig2,LOW);
  delayMicroseconds(2);
  digitalWrite(trig2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2,LOW);
  duration2 = pulseIn(echo2,HIGH);
  d2 = duration2/29/2;
  
  delayMicroseconds(50);
  
  digitalWrite(trig3,LOW);
  delayMicroseconds(2);
  digitalWrite(trig3,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3,LOW);
  duration3 = pulseIn(echo3,HIGH);
  d3 = duration3/29/2;
  //might need a delayMicroseconds(50);
}
void checkback(){
  digitalWrite(trig4,LOW);
  delayMicroseconds(2);
  digitalWrite(trig4,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig4,LOW);
  duration4 = pulseIn(echo4,HIGH);
  d4 = duration4/29/2;
  
  delayMicroseconds(50);
  
  digitalWrite(trig5,LOW);
  delayMicroseconds(2);
  digitalWrite(trig5,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig5,LOW);
  duration5 = pulseIn(echo5,HIGH);
  d5 = duration5/29/2;
  
  delayMicroseconds(50);
  
  digitalWrite(trig6,LOW);
  delayMicroseconds(2);
  digitalWrite(trig6,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig6,LOW);
  duration6 = pulseIn(echo6,HIGH);
  d6 = duration6/29/2;
  //might need a delayMicroseconds(50);
}
void requestEvent(){
  Wire.write(d1);
  Wire.write(d2);
  Wire.write(d3);
  Wire.write(d4);
  Wire.write(d5);
  Wire.write(d6);
  delay(100);
}
