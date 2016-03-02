// CPET-202 RIT Lab 4 starting code

#define CLK_FF_PIN       13    
#define D_FF_PIN         12  
#define SPKR1_PIN        A5   

#define SW1_PIN          5     
//Added
#define SW2_PIN          4
#define SPKR2_PIN        9
//end
#define QTR1_PIN         A3 
#define QTR2_PIN         A4 

boolean isSw1Pressed, prevIsSw1Pressed = false, isSw1Up2Dn, isSw1Dn2Up, isSw1Change;
boolean isSw2Pressed, prevIsSw2Pressed = false, isSw2Up2Dn, isSw2Dn2Up, isSw2Change;
int beepPin; // a variable since it is not known yet which pin has the speaker attached
int numTimesPressed=0;
long startMicroseconds=0, elapsedMicroseconds=0;


   
//***********************************************************************************
void setup()
{
  configureBoardPins();
 
  Serial.begin(9600);
  Serial.println(F("Lab 4: Configuration Detection"));
  beepPin = getActiveBeepPin();
}

//***********************************************************************************
void loop()
{ 
  startMicroseconds=micros(); // to measure execution time
  digitalWrite(QTR2_PIN,HIGH); // connect oscilloscope probe to this pin. 

  
  
  updateSwitchStates(); // function call to update state of switches (pressed/not pressed)
  countSwitchPresses(); // increment or decrement counter of switch presses
//  countSwitchPressesUsingStateMachine(); // avoids any risk of switch bounce
  lightUpBarGraph(numTimesPressed); // light all LEDs from bottom up to numTimesPressed level
  
  digitalWrite(QTR2_PIN,LOW);  
  elapsedMicroseconds=micros()-startMicroseconds; // to measure execution time      
  Serial.print("loop execution time was "); 
  Serial.print(elapsedMicroseconds); 
  Serial.println(" microseconds."); 

  
  

    //beepPin = SPKR1_PIN;
    // will affect beeper sound when in main loop

  if (isSw1Pressed || isSw2Pressed) {
    tone(beepPin,880+numTimesPressed*20);  //change tone
    displayBeepPin(beepPin);

  }
  else if (isSw1Pressed==0){
    noTone(beepPin);
  }
  

  // to stop the speaker tone, call the function noTone e.g. ---> noTone(beepPin);
 
  delay(1);   //remove delay to see switch bounce glitch
    
} // Arduino loop()

//***********************************************************************************
void displayBeepPin(int beepPin)
{ // for spam reduction use static variable
  static int LastBeepPin; 
  // declare variable here
  
  
  // add anti-spam code here

  // Report speaker drive configuration (SPKR1, SPKR2, DISABLED) to console.
  if (LastBeepPin != beepPin){
    switch (beepPin) {
      case (SPKR1_PIN): Serial.println(F("Speaker drive from SPKR1 pin.")); break;
      case (SPKR2_PIN): Serial.println(F("Speaker drive from SPKR2 pin.")); break;
      default:     Serial.println(F("Speaker drive DISABLED at jumper block."));
    }
  }
  LastBeepPin = beepPin;
  
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
  
  pinMode(SPKR1_PIN, INPUT_PULLUP); // I/O pin speaker drive connection sense
  pinMode(SPKR2_PIN, INPUT_PULLUP);
  //int beepPin = SPKR1_PIN;
 
  
  if (digitalRead(SPKR1_PIN)==0){
    beepPin = SPKR1_PIN;
  }
  else if (digitalRead(SPKR2_PIN)==0){
    beepPin = SPKR2_PIN;
  }
  else {
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
  
  pinMode(SW1_PIN, INPUT_PULLUP);  // SPST active low pushbutton
  pinMode(SW2_PIN, INPUT_PULLUP);


  pinMode(SPKR1_PIN, INPUT_PULLUP); // I/O pin speaker drive connection sense
  pinMode(SPKR2_PIN, INPUT_PULLUP);

  pinMode(QTR2_PIN, OUTPUT);  // add for timing measurement using oscilloscope
}

//*******************************************************************************************
void updateSwitchStates (void) 
{ // Signal conditioning: Log switch states and transitions
  prevIsSw1Pressed = isSw1Pressed;
  isSw1Pressed = !digitalRead(SW1_PIN); // convert switch active low to active high
  isSw1Up2Dn = (isSw1Pressed && !prevIsSw1Pressed); // switch just pressed
  isSw1Dn2Up = (!isSw1Pressed && prevIsSw1Pressed);   // switch just released
  isSw1Change = (isSw1Up2Dn || isSw1Dn2Up);             // switch just changed state
  
  // add code here to read switch 2
  prevIsSw2Pressed = isSw2Pressed;
  isSw2Pressed = !digitalRead(SW2_PIN); // convert switch active low to active high
  isSw2Up2Dn = (isSw2Pressed && !prevIsSw2Pressed); // switch just pressed
  isSw1Dn2Up = (!isSw2Pressed && prevIsSw2Pressed);   // switch just released
  isSw2Change = (isSw2Up2Dn || isSw2Dn2Up);             // switch just changed state
}

//*******************************************************************************************
void countSwitchPresses(void) 
{ 
 static int LastNumTimesPressed; 

  // if SW1 just pressed down, count up by one, 
  if(isSw1Up2Dn){     
    numTimesPressed += 1;
  }
  
  // if SW2 just pressed down, count down by one
  if(isSw2Up2Dn){
    numTimesPressed -= 1;
  }
  
  
  // if  numTimesPressed<0, force numTimesPressed to be zero
  if(numTimesPressed < 0){
    numTimesPressed = 0;
  }
  
  // if  numTimesPressed>10, force numTimesPressed to be 10.
  if(numTimesPressed > 10){
    numTimesPressed = 10;
  }
  
  // only if the value of numTimesPressed has changed, print the new value of numTimesPressed
  if(isSw1Up2Dn || isSw2Up2Dn){
    if (numTimesPressed!=LastNumTimesPressed){
      Serial.print("Switch 1 has been pressed ");
      Serial.print(numTimesPressed);
      Serial.println(" times."); 
    }
  }
  LastNumTimesPressed=numTimesPressed;
}

//***********************************************************************************
void pulseClk(void)
{
   // Pulse clock line: LOW, HIGH to get guaranteed rising edge
  // and leave function with LED on i.e. clock high.

 digitalWrite(CLK_FF_PIN, LOW);
 digitalWrite(CLK_FF_PIN, HIGH);
 digitalWrite(CLK_FF_PIN, LOW);

}

//***********************************************************************************
void lightUpBarGraph(int barNum)
{ // start by calling clearShiftRegisterToZero()
  clearShiftRegisterToZero();
  for(int i = 0; i < barNum; i++){
    digitalWrite(D_FF_PIN, HIGH);
    pulseClk();
   
  if (barNum<9){
    digitalWrite(D_FF_PIN, LOW);
    
  }
  if(barNum >= 9){
      digitalWrite(D_FF_PIN, HIGH);
  }
  if(barNum >= 10){
      digitalWrite(CLK_FF_PIN, HIGH);
   } 
  }
 
  // if the barNum>=9 turn on the CLK_FF_PIN LED or if not, turn off the LED
  // if the barNum>=10 turn on the D_FF_PIN LED or if not, turn off the LED
}


//***********************************************************************************
void clearShiftRegisterToZero(void)
{ // clear out the shift register by clocking out 8 zeroes.
int clkoutzeros = 0;
while(clkoutzeros !=8){
  digitalWrite(D_FF_PIN, LOW);
  pulseClk(); // call the pulseClk() function to perform clocking.
  clkoutzeros++;
}

  // leave the D_FF and CLK_FF pins LOW when done.
  digitalWrite(CLK_FF_PIN, LOW);
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
  if (numTimesPressed<0) numTimesPressed=0;
  if (numTimesPressed>10) numTimesPressed=10;
 }
