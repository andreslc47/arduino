#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <Wire.h>

#include <PubSubClient.h>

const char* ssid = "MY_SSID";                 // Credenciales SSId
const char* password = "MY_PASSWORD";           // Credenciales Pass
const char* mqtt_server = "MY AWS ENDPOINT";   // Reemplazar su endpoint de su cuenta
const int mqtt_port = 8883;

String Read_rootca;
String Read_cert;
String Read_privatekey;
//********************************
#define BUFFER_LEN  256
long lastMsg = 0;
char msg[BUFFER_LEN];
int value = 0;
byte mac[6];
char mac_Id[18];
int count = 1;
//********************************

WiFiClientSecure espClient;
PubSubClient client(espClient);

//********************************

StaticJsonDocument<200> current_doc;
String messageData;
DeserializationError error_parsing;
const char* current_direction;
int current_speed;

//********************************

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

// Motor B
int motor2Pin1 = 25; 
int motor2Pin2 = 33; 
int enable2Pin = 32; 


// Setting PWM properties
//const int freq = 30000;
const int freq = 8000;
const int pwmChannel_0 = 0;
const int pwmChannel_1 = 1;
const int resolution = 8;
//int current_duty_cycle = 255;
//int current_duty_cycle = 160;
//********************************

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando.. ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
}



void robot_forward(){
  // Move the robot forward at maximum speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); 
  Serial.println("Robot Moving Forward");
}

void robot_stop(){
  // Stop the robot
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("Robot stopped");
}

void robot_backward(){
  // Move robot backwards at maximum speed
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); 
  Serial.println("Robot Moving Backwards");
}


void robot_right(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); 
  Serial.println("Robot Turning Right");
}


void robot_left(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); 
  Serial.println("Robot Turning Left");
}


void robot_right_forward(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW); 
  Serial.println("Robot Turning Right-Forward");
}

void robot_left_forward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); 
  Serial.println("Robot Turning Left-Forward");
}


void robot_right_backward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW); 
  Serial.println("Robot Turning Right-Backward");
}

void robot_left_backward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); 
  Serial.println("Robot Turning Left-Backward");
}





void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  messageData="";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageData += (char)payload[i];
  }
  Serial.println();
  Serial.println("Contenido de messageData: " + messageData);

  // Deserialize the new JSON data received
  error_parsing = deserializeJson(current_doc, messageData);

  // Test if parsing succeeds.
  if (error_parsing) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error_parsing.f_str());
    return;
  }

  current_direction = current_doc["direction"];
  current_speed = current_doc["speed"];

  // Printing the values after parsing
  Serial.print("Direction: ");
  Serial.println(current_direction);
  Serial.print("Speed: ");
  Serial.println(current_speed);
    
  ledcWrite(pwmChannel_0, current_speed);   
  ledcWrite(pwmChannel_1, current_speed);
 
    switch (*current_direction){
    case 'F':
      robot_forward();
      break;
    case 'B':
      robot_backward();
      break;
    case 'S':
      robot_stop();
      break; 
    case 'R':
      robot_right();
      break; 
    case 'L':
      robot_left();
      break;
    default:
      robot_stop();
      break;
  }

  
}


void reconnect() {
  
  // Loop para reconección
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Creando un ID como ramdon
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    
    // Intentando conectarse
    if (client.connect(clientId.c_str())) {
      Serial.println("conectada");
      
    // Conectado, publicando un payload...
      client.publish("ei_out", "hello world");
    
    // ... y suscribiendo
      client.subscribe("ei_in");
      client.subscribe("control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" Esperando 5 segundos");
      
      // Tiempo muerto de 5 segundos
      delay(5000);
    }
  }
}




void setup() {
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel_0, freq, resolution);
  ledcSetup(pwmChannel_1, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel_0);
  ledcAttachPin(enable2Pin, pwmChannel_1);

  Serial.begin(115200);
  Serial.println("Testing a Mobile Robot");
  
  ledcWrite(pwmChannel_0, 255);   
  ledcWrite(pwmChannel_1, 255);
  robot_stop();
  
  Serial.setDebugOutput(true); 
  // Inicializa LED para indicar que el programa está funcionando
  pinMode(22, OUTPUT);
  setup_wifi();
  delay(1000);

  //=====================================================
  if (!SPIFFS.begin(true)) {
    Serial.println("Se ha producido un error al montar SPIFFS");
    return;
  }
  //**********************
  //Root CA leer archivo.
  File file2 = SPIFFS.open("/AmazonRootCA1.pem", "r");
  if (!file2) {
    Serial.println("No se pudo abrir el archivo para leerlo");
    return;
  }
  Serial.println("Root CA File Content:");
  while (file2.available()) {
    Read_rootca = file2.readString();
    Serial.println(Read_rootca);
  }
  //*****************************
  // Cert leer archivo
  File file4 = SPIFFS.open("/my-certificate.pem.crt", "r");
  if (!file4) {
    Serial.println("No se pudo abrir el archivo para leerlo");
    return;
  }
  Serial.println("Cert File Content:");
  while (file4.available()) {
    Read_cert = file4.readString();
    Serial.println(Read_cert);
  }
  //***************************************
  //Privatekey leer archivo
  File file6 = SPIFFS.open("/my-private.pem.key", "r");
  if (!file6) {
    Serial.println("No se pudo abrir el archivo para leerlo");
    return;
  }
  Serial.println("privateKey contenido:");
  while (file6.available()) {
    Read_privatekey = file6.readString();
    Serial.println(Read_privatekey);
  }
  //=====================================================

  char* pRead_rootca;
  pRead_rootca = (char *)malloc(sizeof(char) * (Read_rootca.length() + 1));
  strcpy(pRead_rootca, Read_rootca.c_str());

  char* pRead_cert;
  pRead_cert = (char *)malloc(sizeof(char) * (Read_cert.length() + 1));
  strcpy(pRead_cert, Read_cert.c_str());

  char* pRead_privatekey;
  pRead_privatekey = (char *)malloc(sizeof(char) * (Read_privatekey.length() + 1));
  strcpy(pRead_privatekey, Read_privatekey.c_str());

  Serial.println("================================================================================================");
  Serial.println("Certificados que pasan adjuntan al espClient");
  Serial.println();
  Serial.println("Root CA:");
  Serial.write(pRead_rootca);
  Serial.println("================================================================================================");
  Serial.println();
  Serial.println("Cert:");
  Serial.write(pRead_cert);
  Serial.println("================================================================================================");
  Serial.println();
  Serial.println("privateKey:");
  Serial.write(pRead_privatekey);
  Serial.println("================================================================================================");

  espClient.setCACert(pRead_rootca);
  espClient.setCertificate(pRead_cert);
  espClient.setPrivateKey(pRead_privatekey);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //******************************************
  
  WiFi.macAddress(mac);
  snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(mac_Id);
  //****************************************
  delay(2000);
 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  //************************
  // El LED parpadea:
  digitalWrite(22, HIGH);   
  delay(1000);                      
  digitalWrite(22, LOW);   
  delay(1000);        
  //************************
  
}
