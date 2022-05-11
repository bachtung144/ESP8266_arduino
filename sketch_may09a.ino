#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Thông tin về wifi
#define ssid "Chu Tao"
#define password "0906692879"
#define mqtt_server "broker.hivemq.com"
const uint16_t mqtt_port = 1883; //Port của CloudMQTT TCP
const char *mqtt_username = "bachtung1234";
const char *mqtt_password = "Bachtung1234";
int ledPin = 16;  

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  pinMode(LED_BUILTIN, OUTPUT);
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
  Serial.print("Co tin nhan moi tu topic:");
  Serial.println(topic);
  for (int i = 0; i < length; i++){
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
    if (receivedChar == '0')
      // Kiểm tra nếu tin nhận được là 1 thì bật LED và ngược lại
//      digitalWrite(ledPin, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
    if (receivedChar == '1')
//      digitalWrite(ledPin, LOW);
      digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println();
}
// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
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
//  if(millis() - t > 500) //nếu 500 mili giây trôi qua
//  {
//     t=millis();
//     Serial.print("Gui tin nhan \"Xin chao\" vao topic IoT47_MQTT_Test");
//     client.publish("IoT47_MQTT_Test", "Xin chao !"); // gửi dữ liệu lên topic IoT47_MQTT_Test
//  }
}
