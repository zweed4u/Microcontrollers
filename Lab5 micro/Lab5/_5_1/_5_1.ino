//// Zack Weeden
//// 3/28/2016
//// CPET-202 RIT Lab 5 


#define CLK_FF_PIN       13
#define D_FF_PIN         12
#define SPKR1_PIN        A5
#define SPKR2_PIN        9

#define SW1_PIN          5
#define SW2_PIN          4

#define LED_PIN          A0
#define QTR1_PIN         A3
#define QTR2_PIN         A4

boolean isSw1Pressed, prevIsSw1Pressed = false, isSw1Up2Dn, isSw1Dn2Up, isSw1Change;
boolean isSw2Pressed, prevIsSw2Pressed = false, isSw2Up2Dn, isSw2Dn2Up, isSw2Change;
int beepPin = 0; // a variable since it is not known yet which pin has the speaker attached
int numTimesPressed = 0;
long startMicroseconds = 0, elapsedMicroseconds = 0;


//***********************************************************************************
void setup()
{
  Serial.begin(9600);
  Serial.println(F("Lab 5: Configuration Detection"));
  configureBoardPins();
  getActiveBeepPin();
  displayBeepPin(beepPin);
  TCCR1A=0b01000000;
  TCCR1B=0b00001001;
  OCR1A = 8000; 
  //DDRB |= 00000010;
  DDRC |= 00100000;
  //pinMode(LED_PIN,OUTPUT);
  //DDRB |= (1<<PORTB1);
  DDRC |= (1<<PORTC5);
  beepPin = getActiveBeepPin();  // will affect beeper sound when in main loop
}

//***********************************************************************************
void loop()
{  
  startMicroseconds = micros(); // to measure execution time
  //digitalWrite(QTR2_PIN, HIGH); // connect oscilloscope probe to this pin.
  //PORTC |= (1<<PORTC4);
  
  updateSwitchStates(); // function call to update state of switches (pressed/not pressed)
  countSwitchPresses(); // increment or decrement counter of switch presses
  //  countSwitchPressesUsingStateMachine(); // avoids any risk of switch bounce
  lightUpBarGraph(numTimesPressed); // light all LEDs from bottom up to numTimesPressed level
  
 
  
  if (isSw1Pressed || isSw2Pressed) {
    OCR1A = 8000-numTimesPressed*300;
    DDRB |= 00000010;
    //pinMode(LED_PIN,OUTPUT);
    DDRB |= (1<<PORTB1);
  }
    //tone(beepPin, 880 + numTimesPressed * 20); //change tone
  else if (!isSw1Pressed){
      DDRB &= 0b11111101;
      PORTB &= 0b11111101;
     
  }
  delay(10);   //remove delay to see switch bounce glitch

  //digitalWrite(QTR2_PIN, LOW);
  //PORTC &= ~(1<<PORTC4);
  
//  elapsedMicroseconds = micros() - startMicroseconds; // to measure execution time
//  Serial.print("loop execution time was ");
//  Serial.print(elapsedMicroseconds);
//  Serial.println(" microseconds.");
  
} // Arduino loop()

//***********************************************************************************
void displayBeepPin(int beepPin)
{ // for spam reduction use static variable
  // declare variable here
  // add anti-spam code here
  // Report speaker drive configuration (SPKR1, SPKR2, DISABLED) to console.

  static int lastbeepPin;
  switch (beepPin) {
    case (SPKR1_PIN):
      if (lastbeepPin != SPKR1_PIN) {
        Serial.println(F("Speaker drive from SPKR1 pin."));
      }
      break;
    case (SPKR2_PIN):
      if (lastbeepPin != SPKR2_PIN) {
        Serial.println(F("Speaker drive from SPKR2 pin."));
      }
      break;
    default:
      Serial.println(F("Speaker drive DISABLED at jumper block."));
      break;
  }
  lastbeepPin = beepPin;
}

//***********************************************************************************
int getActiveBeepPin(void)
{
  // Determine switchblock signal routing. Speaker/resistor will pull down
  // soft pullup of digital I/O.
  //
  // add:
  //    Configure SPKR1 and SPKR2 drive pins as pulled up inputs.
  //    Read pin levels.
  //    If a pin is high, it is not connected: leave as input.
  //    If a pin is low it is connected. tone() will automatically configure it for output.
  //    Return value of beepPin as SPKR1, SPKR2 or -1, depending on discovery

  //pinMode(SPKR1_PIN, INPUT_PULLUP);
  DDRD &= (0<<PORTD5);
  PORTD |= (1<<PORTD5);
  //pinMode(SPKR2_PIN, INPUT_PULLUP);
  DDRD &= (0<<PORTD4);
  PORTD |= (1<<PORTD4);
 
  beepPin = SPKR1_PIN;
  
  //int x = digitalRead(SPKR1_PIN);
  int x = (PINC & (1<<PINC5));  
  //int y = digitalRead(SPKR2_PIN);
  int y = (PINB & (1<<PINC1));
//  Serial.println(x);
//  Serial.println(y);
//  Serial.println(PINC);
  
  if (x == 0 && y==0) {
    beepPin = SPKR1_PIN;
  }
  else if (x == 32 && y==0) {
      beepPin = SPKR2_PIN;
  }
  else if (y == 1) {
      beepPin = -1;
  }
  return beepPin;
}

//*******************************************************************************************
void configureBoardPins(void)
{
  // Use pin names per schematic.
  // Schematic inputs are uC outputs.
  // Initialize outputs to benign levels (usually LOW)
  // Schematic outputs are uC inputs.
  // Assign pullups (INPUT_PULLUP) to indeterminate or unattached inputs.
  
  //pinMode(SW1_PIN, INPUT_PULLUP);  // SPST active low pushbutton
  DDRD &= (0<<PORTD5);
  PORTD |= (1<<PORTD5);
  
  
  //pinMode(SW2_PIN, INPUT_PULLUP);
  
  DDRD &= (0<<PORTD4);
  PORTD |= (1<<PORTD4);

  //pinMode(SPKR1_PIN, INPUT_PULLUP);
  DDRC &= (0<<PORTC5);
  PORTC |= (1<<PORTC5);
  
  //pinMode(SPKR2_PIN, INPUT_PULLUP);
  DDRB &= (0<<PORTB1);
  PORTB |= (1<<PORTB1);
  
  //pinMode(D_FF_PIN, OUTPUT);
  DDRB |= (1<<PORTB4);
  //pinMode(CLK_FF_PIN, OUTPUT);
  DDRB |= (1<<PORTB5);
  
  //digitalWrite(D_FF_PIN, LOW);
  PORTB &= (0<<PORTB4);
  //digitalWrite(CLK_FF_PIN, LOW);
  PORTB &= (0<<PORTB5);
  
  //pinMode(QTR2_PIN, OUTPUT);  // add for timing measurement using oscilloscope
  PORTC |= (1<<PORTC4);

}

//*******************************************************************************************
void updateSwitchStates (void)
{ // Signal conditioning: Log switch states and transitions
  prevIsSw1Pressed = isSw1Pressed;
  isSw1Pressed = !digitalRead(SW1_PIN); // convert switch active low to active high
  isSw1Up2Dn = (isSw1Pressed && !prevIsSw1Pressed); // switch just pressed
  isSw1Dn2Up = (!isSw1Pressed && prevIsSw1Pressed);   // switch just released
  isSw1Change = (isSw1Up2Dn || isSw1Dn2Up);             // switch just changed state

  prevIsSw2Pressed = isSw2Pressed;
  isSw2Pressed = !digitalRead(SW2_PIN); // convert switch active low to active high
  isSw2Up2Dn = (isSw2Pressed && !prevIsSw2Pressed); // switch just pressed
  isSw2Dn2Up = (!isSw2Pressed && prevIsSw2Pressed);   // switch just released
  isSw2Change = (isSw2Up2Dn || isSw2Dn2Up);

}

//*******************************************************************************************
void countSwitchPresses(void)
{ // if SW1 just pressed down, count up by one,
  // if SW2 just pressed down, count down by one
  // if  numTimesPressed<0, force numTimesPressed to be zero
  // if  numTimesPressed>10, force numTimesPressed to be 10.
  // only if the value of numTimesPressed has changed, print the new value of numTimesPressed

  if (isSw1Up2Dn == true) {
    numTimesPressed++;

    if (numTimesPressed > 10) {
      numTimesPressed = 10;
    }

    Serial.print("Switch 1 has been pressed ");
    Serial.print(numTimesPressed);
    Serial.println(" times.");
  }

  if (isSw2Up2Dn == true) {
    numTimesPressed--;

    if (numTimesPressed < 0) {
      numTimesPressed = 0;
    }

    Serial.print("Switch 2 has been pressed ");
    Serial.print(numTimesPressed);
    Serial.println(" times.");
  }
}

//***********************************************************************************
void pulseClk(void)
{
  // Pulse clock line: LOW, HIGH to get guaranteed rising edge
  // and leave function with LED on i.e. clock high.
  
  //digitalWrite(CLK_FF_PIN, LOW);
  PORTB &= ~(1<<PORTB5);
  
  //digitalWrite(CLK_FF_PIN, HIGH);
  PORTB |= (1<<PORTB5);


}

//***********************************************************************************
void lightUpBarGraph(int barNum)
{ // start by calling clearShiftRegisterToZero()
  // then clock out a 1 (HIGH) then follow it by a for loop that clocks
  // out a HIGH value barNum of times
  // use the pulseClk() function to perform clocking
  // if the barNum>=9 turn on the CLK_FF_PIN LED or if not, turn off the LED
  // if the barNum>=10 turn on the D_FF_PIN LED or if not, turn off the LED

  clearShiftRegisterToZero();
  
  //digitalWrite(D_FF_PIN, HIGH);
  PORTB |= (1<<PORTB4);
  
  for (int i = 0; i < barNum; i++) {
    pulseClk();
  }
  //digitalWrite(D_FF_PIN, LOW);
  PORTB &= ~(1<<PORTB4);
  
  //digitalWrite (CLK_FF_PIN, LOW);
  PORTB &= ~(1<<PORTB5);

  if (barNum >= 9) {
    //digitalWrite(CLK_FF_PIN, LOW);
    PORTB &= ~(1<<PORTB5);
    
    //digitalWrite(D_FF_PIN, HIGH);
    PORTB |= (1<<PORTB4);
  }
  if (barNum >= 10) {
    //digitalWrite(D_FF_PIN, HIGH);
    PORTB |= (1<<PORTB4);
    
    //digitalWrite(CLK_FF_PIN, HIGH);
    PORTB |= (1<<PORTB5);
  }
}

//***********************************************************************************
void clearShiftRegisterToZero(void)
{ // clear out the shift register by clocking out 8 zeroes.
  // leave the D_FF and CLK_FF pins LOW when done.
  // call the pulseClk() function to perform clocking.
  for (int i = 0; i < 9; i++) {
    //digitalWrite(CLK_FF_PIN, LOW);
    PORTB &= ~(1<<PORTB5);
    
    //digitalWrite(D_FF_PIN, LOW);
    PORTB &= ~(1<<PORTB4);
    
    //digitalWrite(CLK_FF_PIN, HIGH);
    PORTB |= (1<<PORTB5);
  }
  //digitalWrite(CLK_FF_PIN, LOW);
  PORTB &= ~(1<<PORTB5);
}

//***********************************************************************************
void countSwitchPressesUsingStateMachine (void)
{
  // Interpret changes in SW1 and SW2 presses
  // This function is complete as is. No modifications required.

  if ((isSw1Pressed && !prevIsSw1Pressed) &&  prevIsSw2Pressed ) numTimesPressed--; // falling SW1,  hi SW2 -> CW
  if ((!isSw1Pressed && prevIsSw1Pressed) && !isSw2Pressed ) numTimesPressed--; // rising SW1, low SW2 -> CW
  if ((isSw2Pressed && !prevIsSw2Pressed) && !isSw1Pressed ) numTimesPressed--; // falling SW2, low SW1 -> CW
  if ((!isSw2Pressed && prevIsSw2Pressed) &&  isSw1Pressed ) numTimesPressed--; // rising SW2,  hi SW1 -> CW

  if ((isSw1Pressed && !prevIsSw1Pressed) && !isSw2Pressed ) numTimesPressed++; // falling SW1, low SW2 -> CCW
  if ((!isSw1Pressed && prevIsSw1Pressed) &&  isSw2Pressed ) numTimesPressed++; // rising SW1,  hi SW2 -> CCW
  if ((isSw2Pressed && !prevIsSw2Pressed) &&  isSw1Pressed ) numTimesPressed++; // falling SW2,  hi SW1 -> CCW
  if ((!isSw2Pressed && prevIsSw2Pressed) && !isSw1Pressed ) numTimesPressed++; // rising SW2, low SW1 -> CCW}

  // check bounds on number, limit to 0 to 10
  if (numTimesPressed < 0) numTimesPressed = 0;
  if (numTimesPressed > 10) numTimesPressed = 10;
}
