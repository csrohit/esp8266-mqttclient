#include <ESP8266WiFi.h>
#include <ArduinoMqttClient.h>

// used for connecting to the wifi
WiFiClient client;

// used to connect with the broker
MqttClient mqttClient(client);
void onMqttMessage(int messageSize);

// wifi credentials
const char *ssid = "csrohit_s22";
const char *password = "buwg6226";

void setup() {
  // 1: start serial
  Serial.begin(115200);
  Serial.flush();
  Serial.println("started the mqtt example");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);

  // 2: connect WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // 3: connect to the MQTT broker
  // please visit this following URL to get the original source code :)
  // https://github.com/arduino-libraries/ArduinoMqttClient/blob/master/examples/WiFiSimpleReceive/WiFiSimpleReceive.ino
  Serial.println();
  if (!mqttClient.connect("192.168.12.189", 1883)) {
    Serial.println("broker is not connecting... ");
    while (1);
  }
  
  Serial.println("connected with the MQTT broker");

  // set the function pointer which will get called when a message is received
  mqttClient.onMessage(onMqttMessage);

  // subscribe to a topic
  mqttClient.subscribe("led");
}

void onMqttMessage(int messageSize) {
  
  char message[messageSize];
  for (int index = 0; index < messageSize; index++) {
    message[index] = (char)mqttClient.read();
  }

  Serial.println("a message is received: " + String(message) + ": " + messageSize);
  
  if (strcmp(message, "off") == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else if (strcmp(message, "on") == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void loop() {
  float temperature = analogRead(A0);

  // send a message
  mqttClient.beginMessage("temperature");
  mqttClient.print(String(temperature));
  mqttClient.endMessage();
  delay(1000);
  
  mqttClient.poll();
}