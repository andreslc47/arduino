
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
//int max_dutyCycle = (2^resolution)-1;
int dutyCycle = 160;
//********************************

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

}

void loop() {

  ledcWrite(pwmChannel_0, 255);   
  ledcWrite(pwmChannel_1, 255);
  
  robot_forward();
  delay(2000);

  robot_stop();
  delay(2000);
  
  robot_backward();
  delay(2000);

  robot_stop();
  delay(2000);

  robot_right();
  delay(2000);

  robot_stop();
  delay(2000);

  robot_left();
  delay(2000);

  robot_stop();
  delay(2000);

  robot_right_forward();
  delay(2000);

  robot_stop();
  delay(2000);

  robot_left_forward();
  delay(2000);

  robot_stop();
  delay(2000);

  robot_right_backward();
  delay(2000);

  robot_stop();
  delay(2000);

  robot_left_backward();
  delay(2000);

  robot_stop();
  delay(2000);
  
  // Move robot forward with increasing speed
  robot_forward();
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel_0, dutyCycle);   
    ledcWrite(pwmChannel_1, dutyCycle);
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 160;
  // Stop the robot
  robot_stop();
  delay(2000);

}
