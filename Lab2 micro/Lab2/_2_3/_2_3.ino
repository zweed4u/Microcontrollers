#define SW1_PIN 5
#define LED1_PIN 11
#define MSEC_SAMPLE 100
boolean isSwPressed, prevIsSwPressed, isSwRise, isSwFall, isSwChange;
// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);  digitalWrite(LED1_PIN, LOW);
   
  Serial.begin(9600);
  Serial.println(F("Lab 2: Switch State Decoding v0.0\n"));
}

void loop() {
  prevIsSwPressed = isSwPressed;
  isSwPressed = !digitalRead(SW1_PIN);  
  // When the switch is pressed, the SW_PIN is low, so !low is high or true
  isSwRise = (isSwPressed && !prevIsSwPressed);  // switch edge detection
  isSwFall = (!isSwPressed && prevIsSwPressed);    
  isSwChange = (isSwRise || isSwFall);

// uncomment just one line below at time to see how each input 
//  condition is detected. (enable just one output function at a time)

  digitalWrite(LED1_PIN, isSwPressed);
//  digitalWrite(LED1_PIN, isSwRise);
//  digitalWrite(LED1_PIN, isSwFall);
//  digitalWrite(LED1_PIN, isSwChange);  
//  digitalWrite(LED1_PIN, !digitalRead(LED1_PIN)); // see the sample rate  

  delay(MSEC_SAMPLE);
}


