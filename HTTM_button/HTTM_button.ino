// Example Code of using HTTM touch button
const int touch_button_pin = D1;      // GPIO5 // the number of the pushbutton pin
const int led_inbuilt_gpio =  2;      // the number of the buildin LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(115200);
  // initialize the LED pin as an output:
  pinMode(led_inbuilt_gpio, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(touch_button_pin, INPUT);

}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(touch_button_pin);
  //  Serial.print("buttonState = ");
  //  Serial.println(buttonState);
  
  // check if the touch button is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(led_inbuilt_gpio, LOW);
  } else {
    // turn LED off:
    digitalWrite(led_inbuilt_gpio, HIGH);
  }
  delay(100);
}
