#include <TimerOne.h>
#include <SoftwareSerial.h>
#define QUAYAM 7
#define QUAYDUONG 8
#define RX_PIN      50
#define TX_PIN      51
SoftwareSerial mySerial(50, 51);
int signal = -1;
double T, xung = 0;
double tocdo, Tocdomongmuon;
double E, pre_E;
static double I_error = 0;
static double P_error = 0;
static double D_error = 0;
double  Kp, Kd, Ki;
volatile double Output, LastOutput;
unsigned long t = 0;
int sensorValue = 0;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
volatile int outputValue = 0;  
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(24, OUTPUT);
  while (!Serial) {
    // Waiting to connect...
  };
  Serial.begin(9600);
  mySerial.begin(9600);
  Tocdomongmuon = 10; tocdo = 0;
  Output = 0; LastOutput = 0;
  T = 0.1; // Thoi gian lay mau
  Kp = 40; Kd = 5; Ki =2;
  attachInterrupt(0, Demxung, FALLING);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(pid);
  delay(1000);
}
void Demxung()
{
  {
    if (digitalRead(4) == LOW)
      xung++;
    else
      xung--;
  }
}

void pid() {
  tocdo = (xung / 374) * (1 / T) * 60;
  //xung = 0;
  E = Tocdomongmuon - tocdo;
  P_error = E;
  D_error = E - pre_E;
  I_error = I_error + E;
  Output = Kp * P_error + Ki * I_error + Kd * D_error;
  pre_E = E;
}

void bluetooth()
{
  if (mySerial.available() > 0) {
    signal = mySerial.read();
    switch (signal) {
      case '0':
        digitalWrite(24, LOW);
        break;
      case '1':
        digitalWrite(24, HIGH);
        break;
    }
  }
}
void dkDC(int V) {
  if (V > 0) {
    if (V > 255) V = 255;
    digitalWrite( 7, LOW);
    digitalWrite(  8, HIGH);
    analogWrite( 9, V);
  } else {
    if (V < 0) {
      if (V < -255) V = -255;
      digitalWrite( 7, HIGH);
      digitalWrite(  8, LOW);
      analogWrite( 9, -V);
    } else {
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      analogWrite(9, 0);
    }
  }
}
void loop() {
 /* sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 100);
  analogWrite(analogOutPin, outputValue);
  Kp=outputValue;
  /*int i;
    for (i = 0; i < 10; i++)
    delay(100);*/

  /*Serial.println(xung);
    Serial.println(Output);
    Serial.println(E);*/
  bluetooth();
  // pid();
  dkDC(Output);
  Serial.println(E);
  /*if ((millis() - t) >= 10000) {
    t = millis();
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    analogWrite(9, 0);
    delay(1000);
    Tocdomongmuon = -Tocdomongmuon;
  }*/


  //delay(10);



}
