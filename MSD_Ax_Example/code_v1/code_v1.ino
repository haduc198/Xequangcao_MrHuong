#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 13); //Define PIN12 & PIN13 as software UART

const int ledPin =     10; //LED_BUILTIN;// the number of the LED pin
// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
double interval = 4000;           // interval at which to blink (milliseconds)

void setup() {
  mySerial.begin(19200);   // set the data rate for the SoftwareSerial port
   pinMode(10, OUTPUT);          // sets the digital pin 13 as output
  Serial.begin(19200);
 
}

void loop() {
  unsigned long currentMillis = millis();
  if (mySerial.available()) {
    Serial.write(mySerial.read());
   // Serial.println(mySerial.read());
   // Serial.write("hello");
  }
 
  if (currentMillis - previousMillis >= interval) { //delay 4S
    // save the last time you blinked the LED
    previousMillis = currentMillis;
     

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
      //-Send command to Motor Driver:
      //+Nx: x address of driver, (note x=0 Brocast address)
      //+Py: y position, unit : Rad,
      //+Vz: z Velocity, unit : Rad/S
      //+Aj: j Acceleration, unit : Rad/S2
      //+G: (Option) Get a current positon from Driver. The driver will return to UART a position information
      //+S: Stop motor 
      //+r: Reset position
      //Mx: change mode, x= mode
      //Tx: set duty (pwm mode)
      /*
       * typedef enum{
           MODE_TURNING,
          MODE_SF_POSITION,
          MODE_PID_POSITION,
          MODE_PI_VELOCITY,
          MODE_SMART_POSITION,
          MODE_PI_VELOCITY_BY_ADC,
          MODE_PWM,
        }RUNING_MODE;
        Example:  mySerial.println("{N2  M6 }"); //change to Pwm
       */
      
      }
      mySerial.println("{N1  M4 }"); 
      mySerial.println("{ N1 P200 V500 A2000   }");    
     
    
    } 
    else 
    {
      ledState = LOW;
       
   
       
       mySerial.println("{ N1 P0 V50 A2000 }");
       
        
  }
}

  
