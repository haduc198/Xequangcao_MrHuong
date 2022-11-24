
#include <SoftwareSerial.h>
#define QUAYAM 7
#define QUAYDUONG 8
#define RX_PIN      50
#define TX_PIN      51
SoftwareSerial mySerial(50, 51);
int signal = -1;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
double xung = 0;
int sensorValue = 0;        // value read from the pot
double PWM ;
volatile int outputValue = 0;        // value output to the PWM (analog out)
// defines pins numbers
const int trigPin = 40;
const int echoPin = 41;
volatile double target ;
// defines variables
long duration;
long int distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
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
  //attachInterrupt(0, Demxung, FALLING);


}
/*void Demxung()
  {
  {
    if (digitalRead(4) == LOW)
      xung++;
    else
      xung--;
  }
  }
*/

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
void sensor_distance()
{
  digitalWrite(24, HIGH);
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

}

void mode_auto()
{
 if (distance < 30)
  {
    dkDC(outputValue);
  }

   if (distance > 50)
  {
    dkDC(-outputValue);
  }
  if (distance<35 && distance>30)
  {
    PWM= outputValue*abs(distance-target)/100;
    dkDC(PWM);
  }
}

void emergency()
{
  if (distance <= 25 || distance >= 60 )
  {
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    analogWrite(9, 0);
  }
}
void point()
{


  target = 50;
  if (distance - target > 0)
  {
    dkDC(outputValue);
    if (abs(distance - target) <= 10)
    {
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(9, 0);
    }
  }
  if (distance - target < 0)
  {
    dkDC(-outputValue);
    if (abs(distance - target) <= 10)
    {
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(9, 0);
    }
  }

}

void loop() {


  sensor_distance();
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(analogOutPin, outputValue);

  if (mySerial.available() > 0)

  {
    signal = mySerial.read();
    /*if (signal == 49 )
      {
      dkDC(outputValue);
      if ( distance >= 50 )
      {
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
        analogWrite(9, 0);
      }
      }*/
  }

  switch (signal) {
    case '1':
      mode_auto();
      emergency();
      break;
    case '2':
      point();
      emergency();
      break;


    case '3':
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      analogWrite(9, 0);
      break;
    default:
      break;
  }
  //Serial.println(signal);

  //Serial.println(xung);

  //Serial.println(target);
  Serial.print("Distance: ");
  Serial.println(distance);

  //Serial.println(outputValue);
  //delay(1000);
}
