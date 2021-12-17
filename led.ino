/*
Led blink conectado al pin D2
By: www.smelpro.com
Version: V1.0
 */

int ledgpio = 19;

void setup() 
{
  pinMode(ledgpio, OUTPUT);
  //INPUT , entrada
}

void loop() 
{
  digitalWrite(ledgpio, HIGH);
  delay(1000);
  digitalWrite(ledgpio, LOW);
  delay(1000);
}
