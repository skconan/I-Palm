#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EspSoftwareSerial.h>
#include <math.h>

SoftwareSerial se_read(D5, D6); // write only
SoftwareSerial se_write(D0, D1); // read only
String const url = "http://ecourse.cpe.ku.ac.th:1515/api/";

struct ProjectData {
  int32_t sw;
  int32_t buzzer;
  int32_t led_Y;
  int32_t led_B;
  int32_t led_W;
} project_data = { 0, 0, 0 , 0, 0};

struct ServerData {
  int32_t swPressed;
  int32_t airOn;
  int32_t lightIn;
  int32_t lightOut;
  int32_t doorOn;
  int32_t ringOn;
} server_data = { 0, 0, 0, 0, 0, 0};

const char GET_SERVER_DATA = 1;
const char GET_SERVER_DATA_RESULT = 2;
const char UPDATE_PROJECT_DATA = 3;

const char GET_SERVER_DATA_ACTION = 4;
const char GET_SERVER_DATA_RESULT_ACTION = 5;
const char UPDATE_PROJECT_DATA_ACTION = 6;

// wifi configuration
//const char SSID[] = "Unknow";
//const char PASSWORD[] = "1q2w3e4r";
const char SSID[] = "KUWIN_QUEEN_2.4GHz";
const char PASSWORD[] = "1234567890";
// for nodemcu communication
uint32_t last_sent_time = 0;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;

void send_to_arduino(char code, void *data, char data_size) {
  char *b = (char*)data;
  int sent_size = 0;
  while (se_write.write(code) == 0) {
    delay(1);
  }
  while (sent_size < data_size) {
    sent_size += se_write.write(b, data_size);
    delay(1);
  }
}

void wifi_initialization() {
  Serial.println("WIFI INITIALIZING.");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    yield();
    delay(10);
  }

  Serial.println("WIFI INITIALIZED.");
}

void serial_initialization() {
  Serial.begin(115200);
  se_read.begin(38400);
  se_write.begin(38400);

  while (!se_read.isListening()) {
    se_read.listen();
  }

  Serial.println();
  Serial.println("SERIAL INITIALIZED.");
}

String set_builder(const char *key, int32_t value) {
  String str = url;
  str = str + key;
  str = str + "/set?value=";
  str = str + value;
  Serial.println(str);
  return str;
}

String get_builder(const char *key) {
  String str = url;
  str = str + key;
  str = str + "/view/";
  return str;
}

void update_data_to_server_callback(String const &str) {
  Serial.println("update_data_to_server_callback FINISHED!");
}

bool GET(const char *url, void (*callback)(String const &str,int32_t &value), int32_t &value) {
  HTTPClient main_client;
  main_client.begin(url);
  if (main_client.GET() == HTTP_CODE_OK) {
    Serial.println("GET REQUEST RESPONSE BEGIN");
    if (callback != 0) {
      callback(main_client.getString(),value);
    }
    delay(100);
    return true;
  }
  Serial.println("GET REQUEST RESPONSE BEGIN");
  return false;
}
bool GET(const char *url, void (*callback)(String const &str,float &value), float &value) {
  HTTPClient main_client;
  main_client.begin(url);
  if (main_client.GET() == HTTP_CODE_OK) {
    Serial.println("GET REQUEST RESPONSE BEGIN");
    if (callback != 0) {
      callback(main_client.getString(),value);
    }
    delay(100);
    return true;
  }
  Serial.println("GET REQUEST RESPONSE BEGIN");
  
  return false;
}

bool POST(const char *url, void (*callback)(String const &str)) {
  HTTPClient main_client;
  main_client.begin(url);
  if (main_client.GET() == HTTP_CODE_OK) {
    Serial.println("POST REQUEST RESPONSE BEGIN");
    if (callback != 0) {
      callback(main_client.getString());
    }
    delay(100);
    return true;
  }
  Serial.println("POST REQUEST RESPONSE BEGIN");
  return false;
}

int get_request_int(String const &str) {
  int32_t tmp = str.toInt();
  return tmp;
}

float get_request_float(String const &str) {
  float tmp = str.toFloat();
  return tmp;
}
void get_request(String const &str, int32_t &value){
  value = get_request_int(str);
}
void get_request(String const &str, float &value){
  value = get_request_float(str);  
}

void get_request_raw_callback(String const &str) {
  Serial.println("GET REQUEST RESPONSE BEGIN");
  Serial.println("========================");
  Serial.println(str.c_str());
  Serial.println("========================");
  Serial.println("GET REQUEST RESPONSE END");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  serial_initialization();
  wifi_initialization();

  Serial.print("sizeof(ServerData): ");
  Serial.println((int)sizeof(ServerData));
  Serial.print("ESP READY!");
}
void print_get_server(String const &str,int32_t a, int32_t b){
  Serial.print(str);
  if (a == b)
    Serial.println(": On");
  else
    Serial.println(": Off");
} 

void loop() {
  /* 1) Server polling data from server every 1500 ms
     2) Arduino always get local data
  */
  Serial.println("LOOP");
  uint32_t cur_time = millis();
  if (cur_time - last_sent_time > 1000) {
    //int variable
    GET("http://ecourse.cpe.ku.ac.th:1515/api/ipalm-switchStatus/view/", get_request,server_data.swPressed);
    GET("http://ecourse.cpe.ku.ac.th:1515/api/ipalm-doorOn/view/", get_request,server_data.doorOn);
    GET("http://ecourse.cpe.ku.ac.th:1515/api/ipalm-airOn/view/", get_request,server_data.airOn);
    GET("http://ecourse.cpe.ku.ac.th:1515/api/ipalm-lightOut/view/", get_request,server_data.lightOut);
    GET("http://ecourse.cpe.ku.ac.th:1515/api/ipalm-lightIn/view/", get_request,server_data.lightIn);
    Serial.println("<---- From Server ---->");
    
    print_get_server("switchStatus", server_data.swPressed, 0);
    print_get_server("doorOn", server_data.doorOn, 1);
    print_get_server("airOn", server_data.airOn, 1);
    print_get_server("lightOut", server_data.lightOut, 1);
    print_get_server("lightIn", server_data.lightIn, 1);
    
    last_sent_time = cur_time;
  }

  while (se_read.available()) {
    char ch = se_read.read();
    if (cur_buffer_length == -1) {
      cur_data_header = ch;
      switch (cur_data_header) {
        case UPDATE_PROJECT_DATA:
          expected_data_size = sizeof(ProjectData);
          cur_buffer_length = 0;
          break;
        case GET_SERVER_DATA:
          expected_data_size = sizeof(ServerData);
          cur_buffer_length = 0;
          break;
      }
    } else if (cur_buffer_length < expected_data_size) {
      buffer[cur_buffer_length++] = ch;
      if (cur_buffer_length == expected_data_size) {
        switch (cur_data_header) {
          case UPDATE_PROJECT_DATA: {
              ProjectData *project_data = (ProjectData*)buffer;
              Serial.println("<---- From arduino ---- >");
         
              int32_t switchStatus = project_data->sw;         
              Serial.println(switchStatus);  
              POST(set_builder("iPalm-switchStatus", switchStatus).c_str(), update_data_to_server_callback);
              POST(set_builder("iPalm-switchStatus", switchStatus).c_str(), update_data_to_server_callback);

            }
            break;
          case GET_SERVER_DATA:

            Serial.println("<---- Send data to arduino ---->");
            print_get_server("switchStatus", server_data.swPressed, 0);
            print_get_server("doorOn", server_data.doorOn, 1);
            print_get_server("airOn", server_data.airOn, 1);
            print_get_server("lightOut", server_data.lightOut, 1);
            print_get_server("lightIn", server_data.lightIn, 1);
            
            send_to_arduino(GET_SERVER_DATA_RESULT, &server_data, sizeof(ServerData));
            break;
        }
        cur_buffer_length = -1;
      }
    }
  }
  delay(500);
}
