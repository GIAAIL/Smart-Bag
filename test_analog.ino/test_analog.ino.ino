/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

const int ledPin = D6; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle < 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(40);
    Serial.println(dutyCycle);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle > 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(40);
    Serial.println(dutyCycle);
  }
}
