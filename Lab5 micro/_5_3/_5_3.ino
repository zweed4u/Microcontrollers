
void setup() {
  Serial.begin(9600);
  
  TCCR1A=0b01000000;
  TCCR1B=0b00001001;
  
  OCR1A = 8000; 
  
  //Output on pin 9
  DDRB |= 00000010;
  
  DDRB &= (0<<PORTB1);
  PORTB |= (1<<PORTB1);
  //pinMode(9,OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
}



