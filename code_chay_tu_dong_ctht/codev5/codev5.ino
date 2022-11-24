#include <SoftwareSerial.h>
#define RX_PIN 50
#define TX_PIN 51
#define minPWM 150
#define Vspeed 5
#define Mspeed 15
#define In1 7
#define In2 8
#define PWM_pin 9
#define ctht1 18
#define ctht2 19



SoftwareSerial mySerial(50, 51);
static int dataIn = 0;
volatile double target ;
static int PWM;
static int distance;
static long int T, xung = 0;
static byte ct1;
static byte ct2;
static byte dir = 1;
char signal;

void Demxung()
  
  {
  if (digitalRead(4) == HIGH)
    xung++;
  else
    xung--;
  }

void DongCo(byte temp_dir, int temp_v) {
  if (temp_dir == 1)
  { // CHAY THUAN
    digitalWrite(In2, 1);
    digitalWrite(In1, 0);
    analogWrite(PWM_pin, temp_v);
  } else {
    if (temp_dir == 2)
    { // CHAY NGƯỢC
      digitalWrite(In1, 1);
      digitalWrite(In2, 0);
      analogWrite(PWM_pin, temp_v);
    } else {
      //DIR=0 DỪNG
      digitalWrite(In1, 0);
      digitalWrite(In2, 0);
      analogWrite(PWM_pin, 0);
    }
  }
}
void DocBluetooth()
{ 
  if (mySerial.available())
  dataIn = mySerial.parseInt();
  target= dataIn;
  
}

void chedo2()
{   
    
    DocBluetooth();
    distance = (xung * 0.0128) / 10;
    if (target > 410 || target < 0)
    {
      Serial.println("qua hanh trinh");
      digitalWrite(In1, 0);
      digitalWrite(In2, 0);
      DongCo(1, 0);
      DongCo(2, 0);
      delay(200);
    }
    else if (distance < target)
      {
        DongCo(1, 150);
        Serial.println(distance);
      }
    
    else if (distance > target)
      {
        DongCo(2,150); 
        Serial.println(distance);
      }
    if (abs(distance-target)<5)
      {
        DongCo(1,0);
        DongCo(2,0);
        Serial.println(distance);
      }
    }


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  attachInterrupt(0, Demxung, FALLING);
  DongCo(2, 150);
}




void loop()
{
 

  ct1 = digitalRead(ctht1);
  if (ct1 == HIGH)
  {
    xung=0;
    digitalWrite(In1, 0);
    digitalWrite(In2, 0);
  }
  while (Serial.available())
  {
    signal = Serial.read();
    signal = mySerial.read();
    DongCo(0, 0);
  }
  switch (signal) {
    case 'a':
     
      distance = (xung * 0.0128) / 10;
      //Serial.println(xung);
      Serial.println(distance);
      PWM = minPWM + Vspeed * abs(distance - target) - (distance - target) * (distance - target) / 2;   //Tính vận tốc:
      if (PWM >= 255) PWM = 255;
      if (PWM < minPWM) PWM = minPWM;
      ct1 = digitalRead(ctht1);
      ct2 = digitalRead(ctht2);
      if (ct1 == HIGH) {
        xung = 0;
        //DongCo(1, 150);
        dir = 1;
      } else {
        if (ct2 == HIGH) {

          //DongCo(2, 150);
          dir = 2;
        }
      }
      DongCo (dir, 120);
      break;


    case 'p':
      
      chedo2();
      break;
      
    case 'm':
      digitalWrite(In1, 0);
      digitalWrite(In2, 0);
      analogWrite(PWM_pin, 0);
      break;
    default:
      break;

  }


  delay(100);

}
