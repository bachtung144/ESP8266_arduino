#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>   

// Thông tin về wifi
#define ssid "Honor Play"
#define password "11051999"
#define mqtt_server "broker.hivemq.com"
const uint16_t mqtt_port = 1883; //Port của CloudMQTT TCP
const char *mqtt_username = "bachtung1234";
const char *mqtt_password = "Bachtung1234";

//const int relayInput = 2;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  pinMode (5, OUTPUT); //initialAze pin as OUTPUT
  pinMode (4, OUTPUT);
  pinMode (0, OUTPUT);
  pinMode (2, OUTPUT);
  pinMode (14, OUTPUT);
  pinMode (12, OUTPUT);
  pinMode (13, OUTPUT);
}
// Hàm kết nối wifi
void setup_wifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) 
{
  char str[length+1];
   Serial.print("Message arrived [");
   Serial.print(topic);
   Serial.print("] ");
   int i=0;
   for (i=0;i<length;i++) {
     str[i]=(char)payload[i];
   }
  str[i] = 0;

  StaticJsonDocument <256> doc;
  deserializeJson(doc,payload);
  const char* chipId = doc["chipId"];
  int relayInput = doc["gpio"];
  int status = doc["status"];
  
  char buffer[33];
  ultoa(ESP.getChipId(), buffer, 10);
  if(strcmp(buffer, chipId) == 0) 
  {
    if (status == 0)
      {
        digitalWrite(relayInput, HIGH);
      }
    if (status == 1)
      {
        digitalWrite(relayInput, LOW);
      }
  }
  
//  Serial.println(buffer);
//  Serial.println(chipId);
//  Serial.println(device);
//  Serial.println(status);
  
  Serial.println();
}
// Hàm reconnec a1t thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() 
{
  String client_id = "esp8266-client-";
  while (!client.connected()) // Chờ tới khi kết nối
  {
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect(client_id.c_str()))  //kết nối vào broker
    {
      Serial.println("connect to mqtt");
      client.subscribe("IoT47_MQTT_Test"); //đăng kí nhận dữ liệu từ topic IoT47_MQTT_Test
    }
    else 
    {
      Serial.print("error");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
unsigned long t;
void loop() 
{
  if (!client.connected())// Kiểm tra kết nối
    reconnect();
  client.loop();
}
