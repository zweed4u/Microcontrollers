/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#define SW_PIN 5
#define LED1 11
boolean isSwPressed, prevIsSwPressed, isSwRise, isSwFall, isSwChange;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(LED1, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println(F("Lab 2\n"));

}

// the loop function runs over and over again forever
void loop() {
  isSwPressed = !digitalRead(SW_PIN);
  if (isSwPressed==1){
    digitalWrite(LED1, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else{
    digitalWrite(LED1, LOW);
  }
  
}
