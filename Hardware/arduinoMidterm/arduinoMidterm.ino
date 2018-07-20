#include <SoftwareSerial.h>
#define SW 2
#define BUZZER 3
#define LED_Y 4
#define LED_B 5
#define LED_W 6
#define SERVO 7
Servo myservo;
SoftwareSerial se_read(12, 13); // write only
SoftwareSerial se_write(10, 11); // read only

struct ProjectData {
  int32_t sw;
  int32_t buzzer;
  int32_t led_Y;
  int32_t led_B;
  int32_t led_W;
  int32_t servo;
  int32_t lastServo;
} project_data = { 0, 0, 0 , 0, 0, 0};

struct ServerData {
  int32_t swPressed;
  int32_t airOn;
  int32_t lightIn;
  int32_t lightOut;
  int32_t doorOn;
  int32_t ringOn;
} server_data = { 0, 0, 0, 0, 0};

const char GET_SERVER_DATA = 1;
const char GET_SERVER_DATA_RESULT = 2;
const char UPDATE_PROJECT_DATA = 3;

void send_to_nodemcu(char code, void *data, char data_size) {
  char *b = (char*)data; 
  char sent_size = 0;
  while (se_write.write(code) == 0) {
    delay(1);
  }
  while (sent_size < data_size) {
    sent_size += se_write.write(b, data_size);
    delay(1);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SW, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_W, OUTPUT);
  myservo.attach(SERVO);
  myservo.write(0);
  
  Serial.begin(115200);
  se_read.begin(38400);
  se_write.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!se_read.isListening()) {
    se_read.listen();
  }
  //pinMode(SW,INPUT_PULLUP);
  Serial.println((int)sizeof(ServerData));
  Serial.println("ARDUINO READY!");
}

uint32_t last_sent_time = 0;
boolean is_data_header = false;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;
int32_t b = -1;
void loop() {
  uint32_t cur_time = millis();
  
  //read from sensor....
  int a = digitalRead(SW);
  project_data.sw = a; //set sw to project data
  if(a == 0){
    Serial.println("Press");
    delay(1000);
  }
  
  /*project_data.plus = b;
  if(server_data.plus == 1){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }*/
  
  if (cur_time - last_sent_time > 500) {//always update
//    Serial.println(project_data.plus);
    send_to_nodemcu(GET_SERVER_DATA, &server_data, sizeof(ServerData));
    send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ServerData));
    last_sent_time = cur_time;
  }
  //send to nodemcu
  
  //read data from server pass by nodemcu
  while (se_read.available()) {
    char ch = se_read.read();
    Serial.print("RECV: ");
    Serial.println((byte)ch);
    if (cur_buffer_length == -1) {
      cur_data_header = ch;
      switch (cur_data_header) {
        case GET_SERVER_DATA_RESULT:
        //unknown header
          expected_data_size = sizeof(ServerData);
          cur_buffer_length = 0;
          break;
      }
    } else if (cur_buffer_length < expected_data_size) {
      buffer[cur_buffer_length++] = ch;
      if (cur_buffer_length == expected_data_size) {
        switch (cur_data_header) {
          case GET_SERVER_DATA_RESULT: {
            ServerData *data = (ServerData*)buffer;
            //use data to control sensor
            if(data -> swPressed == 1) {
              tone(BUZZER, 2000);
              delay(1000);
              noTone(BUZZER);
              Serial.print("Buzzer: ON");
            }
              /*tone(buzzer, 2000);
              delay(200);
              noTone(buzzer);
              tone(buzzer, 2000);
              delay(200);
              noTone(buzzer);*/
            if(data -> airOn == 1) {
              digitalWrite(LED_Y, HIGH);
              Serial.print("Air: ON");
            }
            else if(data -> airOn == 0) {
              digitalWrite(LED_Y, LOW);
              Serial.print("Air: OFF");
            }
            
            if(data -> lightOut == 1) {
              digitalWrite(LED_B, HIGH);
              Serial.print("LightOut: ON");
            }
            else if(data -> lightOut == 0) {
              digitalWrite(LED_B, LOW);
              Serial.print("LightOut: OFF");
            }

            if(data -> lightIn == 1) {
              digitalWrite(LED_W, HIGH);
              Serial.print("LightIn: ON");
            }
            else if(data -> lightIn == 0) {
              digitalWrite(LED_W, LOW);
              Serial.print("LightIn: OFF");
            }

            if(data -> doorOn == 1 and data -> doorOn != ) {
              
            }
            
            /*Serial.print("temp status: ");
            Serial.println(data->temp);
            Serial.print("light: ");
            Serial.println(data->light_lux);
            Serial.print("sound status: ");
            Serial.println(data->sound);
            Serial.print("door: ");
            Serial.println(data->door);
            Serial.print("PLUS: ");
            Serial.println(data->plus);*/
            /*if(data->door == 1) {
              digitalWrite(LED_BUILTIN, HIGH);
            } else {
              digitalWrite(LED_BUILTIN, LOW);
            }*/
            //server_data.plus = data->plus;
            
            send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
            Serial.println("Send data");
          } break;
        }
        cur_buffer_length = -1;
      }
    }
  }
}

