#include <SoftwareSerial.h>
#include<TimerOne.h>
#define RX_PIN 50
#define TX_PIN 51
#define minPWM 150
#define Vspeed 5
#define Mspeed 15
#define In1 7
#define In2 8
#define PWM_pin 9
#define Apoint 0
#define Bpoint 100
#define Reset 11
SoftwareSerial mySerial(50, 51);
int dir = 0;
int dataIn = -1;

volatile double target ;
// defines variables

int PWM;
double distance;
double T, xung = 0;


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, INPUT);
  attachInterrupt(0, Demxung, FALLING);
  Timer1.initialize(10000);


  //distance = (xung * 0.0128) / 10;
  if (distance <= Apoint) {
    target = Bpoint;
    dir = 1; // hướng chuyển động từ A đến B
  }
  else if (distance >= Bpoint) {
    target = Apoint;
    dir = 0; // hướng chuyển động từ B đến A
  }
  else {
    target = Apoint;
    dir = 0; // hướng chuyển động sang A
  }
}
void Demxung()
{
  if (digitalRead(4) == LOW)
    xung++;
  else
    xung--;
}

void loop()
{
  if (digitalRead(11) == LOW)
  {
    xung = 0;
  }

  distance = (xung * 0.0128) / 10;

  if (mySerial.available() > 0)
  {
    dataIn = mySerial.parseInt(); //CODE doc bluetooth: Gui 0: che do AUTO, Gui so bat ky: trong khoang gioi han - Chay den roi dung.
  }
  Serial.print("Distance: ");
  Serial.println(distance);
    // Kiem tra vi tri dau vao
  if (dataIn >= 410)
  {
    Serial.println("qua hanh trinh");
    digitalWrite(In1, 0);
    digitalWrite(In2, 0);
    analogWrite(PWM_pin, PWM);
  }
  if (dataIn > 0 && dataIn < 410)
  {
    target = dataIn;
    if (distance < target)
    {
      dir = 1;
    }
    else
    {
      dir = 0;
    }

    PWM = minPWM + Mspeed * abs(distance - target);
    if (abs(distance - target ) <= 3 )
    {
      PWM = 0;
    }
    if (PWM > 255)
    {
      PWM = 255;
    }
  }
  else
  {
    if (distance <= Apoint)
    {
      target = Bpoint;
      dir = 1; // đổi hướng chuyển động từ A đến B
    }
    if (distance >= Bpoint)
    {
      target = Apoint;
      dir = 0; // đổi hướng chuyển động từ B đến A
    }
    //Tính vận tốc:
    PWM = minPWM + Vspeed * abs(distance - target) - (distance - target) * (distance - target) / 2;
    if (PWM >= 255) PWM = 255;
    if (PWM < minPWM) PWM = minPWM;
  }
  Serial.print("pwm: ");
  Serial.println(PWM);

  //dieu khien dong co
  if (dir == 1)
  {
    digitalWrite(In2, 0);
    digitalWrite(In1, 1);
    analogWrite(PWM_pin, PWM);
  }
  else if (dir == 0)
  {
    digitalWrite(In1, 0);
    digitalWrite(In2, 1);
    analogWrite(PWM_pin, PWM);
  }
  delay(100);

}
