#include <TimerOne.h>
#include <SoftwareSerial.h>
#define QUAYAM 7
#define QUAYDUONG 8
#define RX_PIN      50
#define TX_PIN      51
SoftwareSerial mySerial(50,51);
int signal = -1;
double T, xung;
double vitri, vitrimongmuon;
static double E, I_error, D_error,E_cu;
double  Kp, Kd, Ki;
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
  vitrimongmuon = 500; vitri = 0;
  static long int E_cu= 0; I_error = 0; D_error = 0;
  Output = 0; LastOutput = 0;
  //thong so pid
  T =0.1; // Thoi gian lay mau
  Kp = 5; Kd = 0; Ki =0;
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
  //Serial.println(tocdo);
  /*Serial.println(xung);
  Serial.println(Output);*/
  Serial.println(E);
 
}
void Demxung()
{
  if (digitalRead(4) == LOW)
    xung++;
  else
    xung--;

}
void PID() {
  vitri=((xung*360)/7000);
  xung = 0;
  E = vitrimongmuon - vitri;
  D_error=E-E_cu;
  I_error=I_error+ E;
  Output=Kp*E+Ki*I_error+Kd*D_error;
  if (Output > 255)
    Output = 255;
  if (Output < 0)
    Output = 0;
    if (Output > 0)
  {
    digitalWrite(QUAYAM, 0);
    digitalWrite(QUAYDUONG, 1);
    analogWrite(9, Output);
  }
  else
  {
    digitalWrite(QUAYDUONG, 0);
    digitalWrite(QUAYAM, 0);
    analogWrite(9, Output);
  }
}

  
