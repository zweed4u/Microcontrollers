#define SW2_PIN 4
#define LED2_PIN 8

#define SW1_PIN 5 //Going to have this act as reset once last state reached
boolean isSwPressed, prevIsSwPressed, isSwRise, isSwFall, isSwChange;

enum {red_LED_BLINK, red_LED_ON, red_LED_OFF};

boolean isSw2Pressed, prevIsSw2Pressed, isSw2Rise, isSw2Fall, isSw2Change;
int state = red_LED_BLINK, prevState = !state;
boolean isNewState;

void setup() {
  pinMode(SW1_PIN, INPUT_PULLUP);
  
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(LED2_PIN, OUTPUT);  
  
  //Start at known state - LED OFF
  digitalWrite(LED2_PIN, LOW);
  
  Serial.begin(9600);
  Serial.println(F("Exam 1_2"));
} // setup()

//****************************************************************************
void redBlink(void) {
  digitalWrite(LED2_PIN, HIGH);
  
  //Conditionals to check if next state 
  //transition requirements met
  if (isSw2Pressed){
    state = red_LED_ON;
    return;
  }
  
  delay(500);
  
  if (isSw2Pressed){
    state = red_LED_ON;
    return;
  }
  
  digitalWrite(LED2_PIN, LOW);
  
  if (isSw2Pressed){
    state = red_LED_ON;
    return;
  }
  
  delay(500);
  
  if (isSw2Pressed){
    state = red_LED_ON;
    return;
  }
}

//****************************************************************************
void redOn(void) {
  digitalWrite(LED2_PIN, HIGH);
}

//****************************************************************************
void redOff(void) {
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  prevIsSwPressed = isSwPressed;
  isSwPressed = !digitalRead(SW1_PIN);
  isSwRise = (isSwPressed && !prevIsSwPressed);
  
  prevIsSw2Pressed = isSw2Pressed;
  isSw2Pressed = !digitalRead(SW2_PIN);
  isSw2Rise = (isSw2Pressed && !prevIsSw2Pressed);  // switch edge detection
  isSw2Fall = (!isSw2Pressed && prevIsSw2Pressed);    
  isSw2Change = (isSw2Rise || isSw2Fall);
  
  isNewState = (state != prevState);
  prevState = state;
  
  switch (state) {
 
    case red_LED_BLINK:
      if (isNewState) Serial.println("red_LED_BLINK");
      //Because delay is in redBlink function - delay in state switch detection (ISR here?)
      redBlink();
      if (isSw2Pressed) state = red_LED_ON;
    break;
    
    case red_LED_ON:
      if (isNewState) Serial.println("red_LED_ON");
      redOn();
      if (isSw2Change) state = red_LED_OFF;
    break;

  case red_LED_OFF:
    if (isNewState) Serial.println("red_LED_OFF");
      redOff();
      if (isSwRise) state = red_LED_BLINK;
    break;

    default: state = red_LED_BLINK;
  } // switch (state)
} // loop()
