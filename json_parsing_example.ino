#include <ArduinoJson.h>


StaticJsonDocument<200> doc2;
String messageData;
DeserializationError error2;
const char* sensor2;
long timed2;
double latitude2;
double longitude2;
const char* direction2;
int speed2
void setup() {
  // Initialize serial port
  Serial.begin(115200);
  while (!Serial) continue;

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the capacity of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonDocument<N> allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument doc(200);

  // JSON input string.
  //
  // Using a char[], as shown here, enables the "zero-copy" mode. This mode uses
  // the minimal amount of memory because the JsonDocument stores pointers to
  // the input buffer.
  // If you use another type of input, ArduinoJson must copy the strings from
  // the input to the JsonDocument, so you need to increase the capacity of the
  // JsonDocument.

  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc["sensor"];
  long timed = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println("========= Printing first values ===============");
  Serial.print("Sensor: ");
  Serial.println(sensor);
  Serial.print("Time: ");
  Serial.println(timed);
  Serial.print("Latitude: ");
  Serial.println(latitude, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude, 6);
  Serial.println("==============================================");

//**********************************************************************************
//**********************************************************************************
  
  
}

void loop() {
  // not used in this example

  messageData = "";
  messageData = 
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038],\"direction\":\"L\",\"speed\":189}";
      /*
  //************
  // Length (with one extra character for the null terminator)
  const int messageData_len = messageData.length() + 1; 
  
  // Prepare the character array (the buffer) 
  char char_array[messageData_len];
  
  // Copy it over 
  messageData.toCharArray(char_array, messageData_len);
  //************
      */
      
  // Deserialize the new JSON document
  error2 = deserializeJson(doc2, messageData);

  // Test if parsing succeeds.
  if (error2) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error2.f_str());
    return;
  }

  sensor2 = doc2["sensor"];
  timed2 = doc2["time"];
  latitude2 = doc2["data"][0];
  longitude2 = doc2["data"][1];
  direction2 = doc2["direction"];
  speed2 = doc2["speed"];
  
  // Print new values.
  Serial.println("============ Printing new values ==============");
  Serial.println(messageData);
  Serial.print("Sensor: ");
  Serial.println(sensor2);
  Serial.print("Time: ");
  Serial.println(timed2);
  Serial.print("Latitude: ");
  Serial.println(latitude2, 6);
  Serial.print("Longitude: ");
  Serial.println(longitude2, 6);
  Serial.print("Direction: ");
  Serial.println(direction2);
  Serial.print("Speed: ");
  Serial.println(speed2);
  Serial.println("==============================================");

  if (*direction2 == 'L'){
    Serial.println("TODO BIEN CON DIRECTION");
  }else{
    Serial.println("SIGUE CORRIGIENDO");
  }

  if (speed2 == 189){
    Serial.println("TODO BIEN CON SPEED");
  }else{
    Serial.println("SIGUE CORRIGIENDO");
  }
 
  switch (*direction2){
    case 'F':
      Serial.println("SE OBTUVO UNA F");
      break;
    case 'B':
      Serial.println("SE OBTUVO UNA B");
      break;
    case 'S':
      Serial.println("SE OBTUVO UNA S");
      break; 
    case 'R':
      Serial.println("SE OBTUVO UNA R");
      break; 
    case 'L':
      Serial.println("SE OBTUVO UNA L");
      break;
    default:
      Serial.println("SE OBTUVO UN ERROR");
      break;
  }

  delay(5000);

}
