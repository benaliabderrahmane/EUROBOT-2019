#include<Wire.h>
bool execute = 1;
int i;
int k = 7;
int Direction = 1;
int  x[10];
int last_fc = 7;
void setup(){
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(A3,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  Wire.begin(3);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
}
void loop(){
 
  
  
  
  
  
  
  
  
  while(execute){
  get_plates ();         //real function
  execute = 1;
  }
   /*readfcs(); //testing fcs function
   printer(); 
   */
   
}

    
///////////////////////////////////////////////////////////functions////////////////////////////////////////////////////////////////////////////////////////////
void get_plates ()
{ 
 if(k > 1)
 {
  readfcs();
 
  if(x[1]!= 1 && Direction == 1)
   { 
    suction_cup_out ();
   }
  if(x[1] == 1 && Direction == 1){
    motor_off ();
    Direction = -1;
    pompeon();
    electro_vanne_passant();
    delay(2000);
    suction_cup_in ();
}
if (x[k] == 1 && Direction == -1){
  motor_off(); 
  Direction = 1;
  k--;
  delay(2000);
  pompeoff();
  electro_vanne_ouvrant();
  delay(1000);
}
}}
void pompeon(){
  digitalWrite(A2,LOW);
  }
void pompeoff(){
  digitalWrite(A2,HIGH);
  }
void electro_vanne_passant(){
digitalWrite(A3,LOW);  
}
void electro_vanne_ouvrant(){
digitalWrite(A3,HIGH);  
}
void suction_cup_in ()
{
digitalWrite(A0,HIGH);
digitalWrite(A1,LOW);
}
void suction_cup_out()
{
  digitalWrite(A0,LOW);
  digitalWrite(A1,HIGH);
}
void motor_off () 
{
digitalWrite(A0,HIGH);
digitalWrite(A1,HIGH);
}
void receiveEvent(int howMany){
 execute = Wire.read();
}
void readfcs(){
 x[1] = digitalRead(5);
  x[2] = digitalRead(6);
  x[3] = digitalRead(7);
  x[4] = digitalRead(8);
  x[5] = digitalRead(9);
  x[6] = digitalRead(2);
  x[7] = digitalRead(3);
  x[8] = digitalRead(4); 
}
void printer(){
  for (i=1;i<=8;i++)
{
  Serial.print(x[i]);
}
Serial.println();
}
