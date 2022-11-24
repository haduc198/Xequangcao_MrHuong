#include <SoftwareSerial.h>
#define RX_PIN 51
#define TX_PIN 53
#define In1 7
#define In2 8
#define PWM_pin 9
#define ctht1 18
#define ctht2 19
#define emergency 20
#define khoidong 48

// Cách nối các chân trên Encoder quay

#define  encoderPinA  2   // Tương ứng chân DT trên Encoder quay
#define  encoderPinB  3   // Tương ứng chân CLK trên Encoder quay

// Chân + nối nguồn 5V và chân GND nối cực âm

volatile long int encoderPos = 0;  // Cho vị trí đầu bằng 0

int lastReportedPos = 1;   // Vị trí cuối bằng 1

static boolean rotating = false;    // Quản lý debounce (giống như là chống nhiễu)

// các biến cho trình phục vụ ngắt interrupt service routine vars

boolean A_set = false;
boolean B_set = false;

//Đo vi tri

int newposition;
int oldposition = 0;
long newtime;
long oldtime = 0;
int ganxung = 0;


SoftwareSerial mySerial(53, 51);
static int dataIn = 0;
volatile double target ;
static int PWM;
static int distance;

static byte ct1;
static byte ct2;
static byte dir = 1;
static byte role = 0;
static int batdau;
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
  rotating = true;  // Khởi động bộ debounce (có thể hiểu như bộ chống nhi���u)
  newtime = millis();
  newposition = encoderPos;
  detachInterrupt(0);
  detachInterrupt(1);
  oldposition = newposition;
  oldtime = newtime;
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);
  distance = (encoderPos * 0.0137); // khoang cach Don vi mm
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
  Serial.print("*");
  Serial.print(distance, DEC);
  Serial.println("#");
}


void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(35, OUTPUT);
  pinMode(48, INPUT);
  pinMode(20, INPUT_PULLUP);
  digitalWrite(35, LOW);

  pinMode(encoderPinA, INPUT_PULLUP); // INPUT-PULLUP tương đương Mode INPUT và tự động nhận trạng thái HIGH hoặc LOW
  pinMode(encoderPinB, INPUT_PULLUP);


  attachInterrupt(3, Dungkhancap, LOW);
  attachInterrupt(0, doEncoderA, CHANGE);// Chân encoder trên ngắt 0 (chân 2)
  attachInterrupt(1, doEncoderB, CHANGE);// Chân encoder trên ngắt 1 (chân 3)

  DongCo(2, 150); // Mac dinh dong co ve vi tri goc
}


void Dungkhancap()
{
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

void loop()
{
  
  ct1 = digitalRead(ctht1);
  if (ct1 == LOW)
  {
    encoderPos = 0;
    digitalWrite(In1, 0);
    digitalWrite(In2, 0);
  }
  batdau = digitalRead(48); // Kiem tra nut start
  //Serial.println(batdau);
  if (batdau == HIGH )
  {
    if (mySerial.available())
    {
      signal = mySerial.read();
      digitalWrite(In1, 0);
      digitalWrite(In2, 0);
      DongCo(0, 0);
    }
  
    switch (signal) {
      case 'a':   // Che do auto
        digitalWrite(35, LOW);
        rotating = true;  // Khởi động bộ debounce (có thể hiểu như bộ chống nhi���u)
        newtime = millis();
        newposition = encoderPos;
     
        detachInterrupt(0);
        detachInterrupt(1);

        oldposition = newposition;
        oldtime = newtime;


        attachInterrupt(0, doEncoderA, CHANGE);
        attachInterrupt(1, doEncoderB, CHANGE);

        distance = (encoderPos * 0.0137); // khoang cach don vi mm

      
        Serial.print("*");
        Serial.print(distance, DEC);
        Serial.println("#");
        ct1 = digitalRead(ctht1);
        ct2 = digitalRead(ctht2);
            if (ct1 == LOW) {
               encoderPos = 0; // Reset encoder
                dir = 1;
        }    else {
            if (ct2 == LOW) {
                dir = 2;
        }
      }
            DongCo (dir, 150);
            break;


    case 'p':   // Che do ban diem
      digitalWrite(35, LOW);
      chedo2();
      break;
    case 'm':   // Che do bang tay

      digitalWrite(35, HIGH);
      delay(100);

      rotating = true;  // Khởi động bộ debounce (có thể hiểu như bộ chống nhi���u)
      newtime = millis();
      newposition = encoderPos;
     
      detachInterrupt(0);
      detachInterrupt(1);

      oldposition = newposition;
      oldtime = newtime;


      attachInterrupt(0, doEncoderA, CHANGE);
      attachInterrupt(1, doEncoderB, CHANGE);

      distance = (encoderPos * 0.0137); // khoang cach don vi mm

       
        Serial.print("*");
        Serial.print(distance, DEC);
        Serial.println("#");
      break;
    default:
      break;
    }
  }
   else {
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
  }
  delay(100);
 }


// Ngắt khi chuyển trạng thái của A

void doEncoderA() {

  // debounce

  if ( rotating ) delay (1);  // Chờ 1 chút

  // Kiểm tra việc chuyển đổi trạng thái, xem có thật sự thay đổi trạng thái chưa

  if ( digitalRead(encoderPinA) != A_set ) { // debounce một lần nữa

    A_set = !A_set;

    // Cộng 1 nếu có tín hiệu A rồi có tín hiệu B

    if ( A_set && !B_set )

      encoderPos += 1;

    ganxung += 1;

    if (ganxung == 42) {
      ganxung = 0;
    }

    rotating = false;  // Không cần debounce nữa cho đến khi được nhấn lần nữa

  }

}

// Ngắt khi thay đổi trạng thái ở B, tương tự như ở A

void doEncoderB() {

  if ( rotating ) delay (1);

  if ( digitalRead(encoderPinB) != B_set ) {

    B_set = !B_set;

    //  Trừ 1 nếu B rồi đến A

    if ( B_set && !A_set )

      encoderPos -= 1;

    rotating = false;

  }

}
