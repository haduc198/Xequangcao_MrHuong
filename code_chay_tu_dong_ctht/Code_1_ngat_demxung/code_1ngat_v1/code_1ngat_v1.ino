#include <SoftwareSerial.h>
#define RX_PIN 51
#define TX_PIN 53
#define minPWM 150
#define Vspeed 5
#define Mspeed 15
#define In1 7
#define In2 8
#define PWM_pin 9
#define ctht1 18
#define ctht2 19



SoftwareSerial mySerial(53, 51);
static int dataIn = 0;
volatile double target ;
static int PWM;
static int distance;
static byte ct1;
static byte ct2;
static byte dir = 1;
static byte role = 0;
long int xung =0;
char signal;


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
  target = dataIn;

}
void chedo2()
{
  DocBluetooth();
  
  distance = (xung * 0.0137) / 10;
  if (target > 390 || target < 0)
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
  }

  else if (distance > target)
  {
    DongCo(2, 150);
  }
  if (abs(distance - target) < 2)
  {
    DongCo(1, 0);
    DongCo(2, 0);
  }
  Serial.print("distance=");
  Serial.println(distance, DEC);
}


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(9, OUTPUT); //khai bao chan Enable
  pinMode(7, OUTPUT); //khai bao chan In1
  pinMode(8, OUTPUT); //khai bao chan In2
  pinMode(18, INPUT_PULLUP); //khai bao dao vao ctht1
  pinMode(19, INPUT_PULLUP); //khai bao dao vao ctht2
  pinMode(35, OUTPUT); // //khai bao dau ra role
  pinMode(2, INPUT_PULLUP); //Pha A encoder
  pinMode(3, INPUT_PULLUP); //Pha B encoder
  attachInterrupt(0, Demxung, FALLING); // Dem xung suon xuong

  DongCo(2, 150); // Mac dinh dong co ve vi tri goc
}


void Demxung()
{
  if (digitalRead(3) == HIGH)
    xung++;
  else
    xung--;
}

void loop()
{

  ct1 = digitalRead(ctht1);
  if (ct1 == HIGH)
  {
    digitalWrite(In1, 0);
    digitalWrite(In2, 0);
  }
  if (mySerial.available())
  {
    signal = mySerial.read();
    digitalWrite(In1, 0);
    digitalWrite(In2, 0);
    DongCo(0, 0);
  }
  switch (signal) {
    case 'a':

      digitalWrite(35, LOW); 
      distance = (xung * 0.0137) / 10; // Tinh khoang cach
      Serial.print("distance=");
      Serial.println(distance);
     // PWM = minPWM + Vspeed * abs(distance - target) - (distance - target) * (distance - target) / 2;   //Tính vận tốc:
     // if (PWM >= 255) PWM = 255;
     // if (PWM < minPWM) PWM = minPWM;
      ct1 = digitalRead(ctht1);
      ct2 = digitalRead(ctht2);
      Serial.print("ct1");
       Serial.println(ct1);
       Serial.print("ct2");
       Serial.println(ct2);
      if (ct1 == LOW) {
        xung = 0;
        dir = 1;
      } else {
        if (ct2 == LOW) {
          dir = 2;
        }
      }
      DongCo (dir, 120);
      break;


    case 'p':
      digitalWrite(35, LOW);
      chedo2();
      break;
    case 'm':

      digitalWrite(35, HIGH);
      delay(100);
      distance = (xung * 0.0137) / 10;
      Serial.print("distance=");
      Serial.println(distance, DEC);
      break;
    default:
      break;

  }


  delay(100);
}
