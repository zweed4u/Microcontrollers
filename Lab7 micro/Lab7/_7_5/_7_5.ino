#define TP_5_PIN 5  // pin where you will connect oscilloscope probe
#define SET_TP PORTD |= (1 << 5) 
#define CLR_TP PORTD &= ~(1 << 5)

// Define volatile global variables for ISR system interface.
volatile int pulseCount = 0, prevpulseCount = -1;

//***********************************************************************************
void setup()
{    
  Serial.begin(9600);

  Serial.println("Lab 8 ISR Timer1 counter using interrupt");
  delay(100);
  configurePins();
  initInterrupts();
  Serial.println();
  TCCR1A=0b00000011;
  TCCR1B=0b00011101;
  // T_desired = (Clk_freq)/((Prescaler)*(OCR1A+1))
  // 1HZ = (16MHZ)/((1024)*(OCR1A+1))
  // 1024 = 16MHZ/(OCR1A+1)
  // (OCR1A+1) = 16MHZ/1024
  // OCR1A = (16MHZ/1024)-1
  // OCR1A = 15625-1
  OCR1A=15624;
  
 
}

//***********************************************************************************
void loop()
{ 
  // only print the pulse count if it has changed
  if (pulseCount != prevpulseCount)  {
    prevpulseCount = pulseCount;
    Serial.println(pulseCount);
  }
  delay(50);
} // Arduino loop()

//***********************************************************************************
void configurePins(void) {
  pinMode(TP_5_PIN, OUTPUT);
}

//***********************************************************************************
void initInterrupts(void)  {
  cli(); // global interrupt disable during setup. 
         // Prevents interrupts from occurring while you are getting them setup.
  TIMSK1=0b00000010;
  sei(); // global interrupt enable
}

//*********************************************************************************** 
ISR(TIMER1_COMPA_vect)
{
    pulseCount++;
}
