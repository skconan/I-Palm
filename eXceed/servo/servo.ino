#include <Servo.h>
#define servo 9
Servo myservo;
//red power, brown ground, yellow port
void setup() {
  // put your setup code here, to run once:
  myservo.attach(servo);
  myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  myservo.write(180);
  delay(1000);
  myservo.write(0); 
  delay(1000);
}
