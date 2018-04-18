   /* This minimal example shows how to get single-shot range
measurements from the VL6180X.

The range readings are in units of mm. */

#include <Wire.h>
#include <VL6180X.h>

#define defaultSpeed 85
#define correctionSpeed 120

VL6180X sensor1;
VL6180X sensor2;
VL6180X sensor3;

uint16_t s1_val;
uint16_t s2_val;
uint16_t s3_val;

void setup() 
{
  //set pin modes
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  //set pins to low
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

  //setup serial and wire
  Serial.begin(9600);
  Wire.begin();

  //setup sensor 1
  digitalWrite(2, HIGH);
  delay(100);
  sensor1.init();
  sensor1.configureDefault();
  sensor1.setTimeout(500);
  sensor1.setAddress(0x20);

  //setup sensor 2
  digitalWrite(3, HIGH);
  delay(100);
  sensor2.init();
  sensor2.configureDefault();
  sensor2.setTimeout(500);
  sensor2.setAddress(0x22);
  
  //setup sensor 3 
  digitalWrite(4, HIGH);
  delay(100);
  sensor3.init();
  sensor3.configureDefault();
  sensor3.setTimeout(500);
  sensor3.setAddress(0x24);
  //setup motor left wheel
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(11, OUTPUT);
  //setup motor right wheel
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

//front sensor = 1
//lhs sensor = 2
//rhs sensor = 3
void loop() 
{ 
  s1_val = sensor1.readRangeSingleMillimeters();
  s2_val = sensor2.readRangeSingleMillimeters();
  s3_val = sensor3.readRangeSingleMillimeters();
  printLaserReadings();
  Serial.println( (int)s2_val - (int)s3_val);
  if(sensor1.readRangeSingleMillimeters() > 80){
    drive_straight();
  }
  else{
    car_stop();
  }
  
  delay(100);
}

void drive_straight(){
    
    if( (int)s2_val - (int)s3_val < -4){
    //slight turn right
    analogWrite(10, correctionSpeed); 
    Serial.println("Value was negative");
    Serial.println((int)s2_val - (int)s3_val);
  }
  else if( (int)s2_val - (int)s3_val > 4){
    //slight turn left
    Serial.println("Value was positive");
    Serial.println((int)s2_val - (int)s3_val);
    analogWrite(9, correctionSpeed);
  }
  else{
    forward();
  }
}

void backward(){
  analogWrite(9, defaultSpeed);
  digitalWrite(8, HIGH);
  digitalWrite(11, LOW);
  analogWrite(10, defaultSpeed);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
}
void car_stop(){
  analogWrite(9, 0);
  analogWrite(10, 0);
}
void forward(){
  analogWrite(9, defaultSpeed);
  digitalWrite(8, LOW);
  digitalWrite(11, HIGH);
  analogWrite(10, defaultSpeed);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);  
}

void printLaserReadings(){
  Serial.print("Sensor 1: ");
  Serial.print(sensor1.readRangeSingleMillimeters());
  if (sensor1.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print("     Sensor 2: ");
  Serial.print(sensor2.readRangeSingleMillimeters());
  if (sensor2.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.print("     Sensor 3: ");
  Serial.print(sensor3.readRangeSingleMillimeters());
  if (sensor3.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  
  Serial.println();
}


