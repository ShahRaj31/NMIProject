#include <Servo.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THRESHOLD 300
#define EMG_pin 3
#define Servopin 6     //#define Servo_pin 6
float tempmpu = 0;     // temperature MPU5060
float angle = 0;      //angle x MPU5060
int t1 = 0;           // timer for printing the values
int pause = 400 ;     // pause for serial print
const int anglewrist = 75;
Servo servo1;
int pos = 90;
RF24 radio(9, 10);  // Initialize the RF24 object with the CE and CSN pins
const uint64_t pipe = 0xF0F0F0F0E1LL;  // Unique address for communication
int16_t emgData = 0;

MPU6050 mpu6050(Wire);

void setup() {
 Serial.begin(9600);
 servo1.attach(Servopin);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  t1 = millis();
  radio.begin();
  radio.openWritingPipe(pipe);  // Set the writing pipe
}

void loop() {
   int value = analogRead(EMG_pin);
   if ( value > THRESHOLD) {
    servo1.write(90);
   }
   else {
    servo1.write(10);
   }
   Serial.println(value);
   radio.write(&EMG_pin, sizeof(EMG_pin));
   delay(1000);

  mpu6050.update();
  tempmpu = mpu6050.getTemp(); // temperature
  angle = mpu6050.getAngleZ(); // angle x 
  pos = map((int)angle, -anglewrist, anglewrist, 0, 180); // we map the function from the values ​​read by the IMU into the range of the servo
  if (millis() > 3000 ) { // we put a block on the engine while the IMU does the calibration
    servo1.write(pos);
  }
  if (millis() - t1 > pause) {
    Serial.print("anglez : ");
    Serial.print(angle);
    Serial.print("\t temp : ");
    Serial.println(tempmpu);
  }
  t1 = millis();
}