#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

/************ WIFI and MQTT Information (CHANGE THESE FOR YOUR SETUP) ******************/
const char* ssid = "@HAO"; //type your WIFI information inside the quotes
const char* password = "password";
const char* mqtt_server = "192.168.2.110";
const char* mqtt_username = "homeassistant";
const char* mqtt_password = "password";
const int mqtt_port = 1883;

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* state_topic = "home/doorbell";
const char* cmd_topic = "home/doorbell/set";

int simulatePickUpPin = 12; // GPIO12 = D6 = 摘机信号
int simulateUnlockPin = 13; // GPIO13 = D7 = 开门解锁

WiFiClient espClient;
PubSubClient client(espClient);

void setup_simulatePins() {
  pinMode(simulatePickUpPin, OUTPUT);
  digitalWrite(simulatePickUpPin, LOW);

  pinMode(simulateUnlockPin, OUTPUT);
  digitalWrite(simulateUnlockPin, LOW);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.macAddress());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message = "";
  for (int i = 0; i < length; i++) {
    message = message+(char)payload[i];
  }
  
  Serial.print("\n");
  Serial.println(message);

  // Switch on the LED if an 1 was received as first character
  if (message == "open" || message == "ON") {
     Serial.print("\nOpen called\n");
    client.publish(state_topic, "ON");
    digitalWrite(simulatePickUpPin, HIGH);
    delay(2000);
    digitalWrite(simulateUnlockPin, HIGH);
    delay(200);
    digitalWrite(simulateUnlockPin, LOW);
    delay(2000);
    digitalWrite(simulatePickUpPin, LOW);
    client.publish(state_topic, "OFF");
  } else if(message == "close" || message == "OFF"){
     Serial.print("\nClose called\n");
    //digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(simulateUnlockPin, LOW);
    digitalWrite(simulatePickUpPin, LOW);
    client.publish(state_topic, "OFF");
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(state_topic, "OFF");
      // ... and resubscribe
      client.subscribe(cmd_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  setup_simulatePins();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(1000);
  Serial.print("."); // still gets power
}
