#define SW1_PIN 5
#define SW2_PIN 4
#define SERIAL_DATA_AB_PIN 12
#define CLOCK_PIN 13

boolean isSwPressed, prevIsSwPressed;
boolean isSwPressed2, prevIsSwPressed2;
// declare variables here

void setup() {
  //Configuring pullups and ins and outs
  pinMode(SW1_PIN, INPUT_PULLUP);pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SERIAL_DATA_AB_PIN, OUTPUT);pinMode(CLOCK_PIN, OUTPUT);
  //Initiating serial monitor 9600 baud
  Serial.begin(9600);
  Serial.println(F("Lab 3: single LED in motion"));
} // setup()


void loop() {
  //boolean values for history
  prevIsSwPressed = isSwPressed;
  prevIsSwPressed2 = isSwPressed2;
  
  //alternate boolean values for button history
  isSwPressed = !digitalRead(SW1_PIN);
  isSwPressed2 = !digitalRead(SW2_PIN);
  
  //if sw1 pressed down set serial pin high
  if (isSwPressed==1){
    digitalWrite(SERIAL_DATA_AB_PIN,HIGH);
    Serial.println("SW1 pressed");
  }
  //if sw2 pressed down set clock pin high
  if (isSwPressed2==1){
    digitalWrite(CLOCK_PIN,HIGH);
    Serial.println("SW2 pressed");
  }
  
} // loop()

