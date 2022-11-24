#include <TimerOne.h>
#include <SoftwareSerial.h>
#define QUAYAM 7
#define QUAYDUONG 8
#define RX_PIN      50
#define TX_PIN      51
SoftwareSerial mySerial(50,51);
int signal = 0;
double T, xung;
double vitri, vitrimongmuon;
static double E, E1, E2;
double  alpha, beta, gamma, Kp, Kd, Ki;
double Output, LastOutput;


void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(24, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  while (!Serial) {
    // Waiting to connect...
  };
  Serial.begin(9600);
  mySerial.begin(9600);
  vitrimongmuon = 5; vitri = 0;
  Output = 0; LastOutput = 0;
  //thong so pid
  T =0.1; // Thoi gian lay mau
  Kp =300; Kd = 0.01; Ki =0.5;
  attachInterrupt(0, Demxung, FALLING);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(PID);
}
void loop() {
 if (mySerial.available() > 0) {
   signal = mySerial.read();
    switch (signal) {
    case '0':
      digitalWrite(24, LOW);
      break;
    case '1':
      digitalWrite(24,HIGH);
      break;
    }
 }
  
  int i;
  for (i = 0; i < 10; i++)
   delay(100);
  Serial.println(vitri);
  /*Serial.println(xung);*/
  //Serial.println(Output);
  //Serial.println(E);
 
}
void Demxung()
{
  if (digitalRead(4) == LOW)
    xung++;
  else
    xung--;

}
void PID(){
  vitri=((xung*360)/7000);
  xung=0;
  E=vitrimongmuon - vitri;
    alpha = 2*T*Kp + Ki*T*T + 2*Kd; 
    beta = T*T*Ki - 4*Kd - 2*T*Kp;
    gamma = 2*Kd;
   Output = (alpha*E + beta*E1 + gamma*E2 + 2*T*LastOutput)/(2*T) ;
   LastOutput = Output;
   E2=E1;
   E1=E;
    if (Output>255)
      Output=255;
      if (Output<0)
       Output=0;
       if (Output>0)
       {
        digitalWrite( 7, HIGH);
        digitalWrite(  8, LOW);
        analogWrite( 9, Output);
     }
     else 
       {
        analogWrite(9, 0);
        digitalWrite( 8, HIGH);
        digitalWrite( 7, LOW);
       }
}
