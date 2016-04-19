#define SW1_PIN 5 //Going to have this act as reset once last state reached
boolean isSwPressed, prevIsSwPressed, isSwRise, isSwFall, isSwChange;

enum {Mtr25, Mtr50, Mtr75};

int state = Mtr25, prevState = !state;
boolean isNewState;

void setup() {
  //pinMode(SW1_PIN, INPUT_PULLUP);
  DDRD &= (0<<PORTD5);
  PORTD |= (1<<PORTD5);  
  
  //Output on pin 9
  DDRB |= 00000010;
  //pinMode(9,OUTPUT); 
  DDRB |= (1<<PORTB1);
  
  Serial.begin(9600);
  Serial.println(F("Exam 2_2"));
} // setup()

//****************************************************************************
void Mtr25_PWM(void) {  
  //PWM 25% at 15KHZ

  TCCR1A = 0; // set to defaults
  TCCR1B = 0; // set to defaults
      
  //PWM, Phase and Frequency Correct
  TCCR1A  = (0 << WGM11)|(0 << WGM10);    // first part of setup for FPFC PWM (mode 8)
  TCCR1B  = (1 << WGM13)|(0 << WGM12);    // second part of setup for FPFC PWM (mode 8)
      
  //Clear on 
  TCCR1A |= (1 << COM1B1)|(1 << COM1B0);  // non-inverting PWM on channel B: PORTB2, UNOd10 
  TCCR1A |= (1 << COM1A1)|(0 << COM1A0);  // non-inverting PWM on channel A: PORTB1, UNOd9 
      
  //Clock prescaler
  TCCR1B |= (0 << CS12)|(0 << CS11)|(1 << CS10); // no prescale
      
  //Waveform information
  ICR1 = 10000;  // period
  OCR1A = 2500;  // dc 2500/10000
  
}

//****************************************************************************
void Mtr50_PWM(void) {
  //PWM 50% at 15KHZ
  OCR1A = 5000;  // dc 5000/10000 
}

//****************************************************************************
void Mtr75_PWM(void) {
  //PWM 75% at 15KHZ
  OCR1A = 7500;  // dc 7500/10000
}

void loop() {
  prevIsSwPressed = isSwPressed;
  isSwPressed = !digitalRead(SW1_PIN);
  isSwRise = (isSwPressed && !prevIsSwPressed);
  isNewState = (state != prevState);
  prevState = state;
    
  switch (state) {
    case Mtr25:
      if (isNewState) Serial.println("Mtr25 State");
      //Function for 25%DC
      Mtr25_PWM();
      //Delay to see output
      delay(50);
      //State transition on sw1 press
      if (isSwRise) state = Mtr50;  
     break;
    
    
    case Mtr50:
      if (isNewState) Serial.println("Mtr50 State");
      //Function for 50%DC
      Mtr50_PWM();
      //Delay to see output
      delay(50);
      //State transition on sw1 press
      if (isSwRise) state = Mtr75;
    break;


    case Mtr75:
      if (isNewState) Serial.println("Mtr75 State");
      //Function for 75%DC
      Mtr75_PWM();
      delay(50);
      //State transition on sw1 press (loop back to idle (25% DC) state)
      if (isSwRise) state = Mtr25;
    break;
    
    //Default/idle state is 25% DC
    default: state = Mtr25;
    
  } // switch (state)
} // loop()
