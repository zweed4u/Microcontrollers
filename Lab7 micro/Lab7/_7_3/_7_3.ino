//#define TP_5_PIN 5  // pin where you will connect oscilloscope probe
#define INT0_PIN 2  // note this is interrupt pin zero (0) not the word "into"
#define INT1_PIN 3

#define SET_TP PORTD |= (1 << 5) 
#define CLR_TP PORTD &= ~(1 << 5)

// Define volatile global variables for ISR system interface.
volatile int pulseCount = 0, prevpulseCount = -1;
volatile boolean PCINT1_was_triggered=false;

//***********************************************************************************
void setup()
{    
  Serial.begin(9600);
  Serial.println("Lab 7 ISR INT0 counter");
  delay(100);
  configurePins();

  cli(); // global interrupt disable during setup. 
         // Prevents interrupts from occurring while you are getting them setup.

  EICRA |= (1 << ISC01) | (0 << ISC00); // configure to detect falling edge
  EICRA |= (1 << ISC11) | (0 << ISC10);
         // ISC0x refers to Interrupt Sense Control for interrupt 0 (two bits 0 and 1)
  sei(); // global interrupt enable

  delay(5000); // during this delay, press the switch a few times.
  Serial.print("EIFR before clearing any pending flags ");
  Serial.println(EIFR,BIN);
  Serial.print("EICRA before clearing any pending flags ");
  Serial.println(EICRA,BIN);
  Serial.print("EIMSK before clearing any pending flags ");
  Serial.println(EIMSK,BIN);
  Serial.println();
  delay(4000);
  initInterrupts();
  Serial.print("EIFR after clearing any pending flags ");
  Serial.println(EIFR,BIN);
  Serial.print("EICRA after clearing any pending flags ");
  Serial.println(EICRA,BIN);
  Serial.print("EIMSK after clearing any pending flags ");
  Serial.println(EIMSK,BIN);
  Serial.println();
 
}

//***********************************************************************************
void loop()
{ 
  // only print the pulse count if it has changed
  if (pulseCount != prevpulseCount)  {
    prevpulseCount = pulseCount;
    Serial.println(pulseCount);
  }
  
//  if(PCINT1_was_triggered==true) {
//     Serial.println("PCINT1 was triggered");
//     PCINT1_was_triggered=false;
//   };
  

} // Arduino loop()

//***********************************************************************************
void configurePins(void) {
  pinMode(INT0_PIN, INPUT_PULLUP);
  pinMode(INT1_PIN, INPUT_PULLUP);
  //pinMode(TP_5_PIN, OUTPUT);
}

//***********************************************************************************
void initInterrupts(void)  {
  cli(); // global interrupt disable during setup. 
         // Prevents interrupts from occurring while you are getting them setup.

  EICRA |= (1 << ISC01) | (0 << ISC00); // configure to detect falling edge
         // ISC0x refers to Interrupt Sense Control for interrupt 0 (two bits 0 and 1)
  EICRA |= (1 << ISC11) | (0 << ISC10);
  EIMSK |= (1 << INT0); // unmask INT0 so that it is enabled
  EIMSK |= (1 << INT1);
  EIFR  |= (1 << INTF0); // clear any pending interrupt flags by writing a 1 to register
  EIFR  |= (1 << INTF1);
  
  
  PCICR |= 0x02;
  PCIFR |= 0x02;
  //Analog pins 0 and 1 (A1 and A0)
  PCMSK1 |= 0b000011;
  sei(); // global interrupt enable
}

//*********************************************************************************** 
ISR(PCINT1_vect) // the ISR name must match the interrupt (e.g. INT0_vect)
{ 
  static byte prevPINC;
  byte newPINC;
  byte changeMap;
  
  newPINC = PINC;
  changeMap = newPINC ^ prevPINC;
  PCINT1_was_triggered=true; //sets boolean that is checked in loop that will take care of serial monitor display


  if (changeMap & 0x01) 
  {
    pulseCount++;
  }
  if (changeMap & 0x02) 
  {
    pulseCount--;
  }
}
