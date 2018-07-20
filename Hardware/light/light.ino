#define ldr A3
#define LED 9

int ldrValue = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(ldr, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  ldrValue = analogRead(ldr);
  Serial.print("ldrValue: ");
  Serial.println(ldrValue);
  delay(300);
  if(ldrValue <= 300) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
}
