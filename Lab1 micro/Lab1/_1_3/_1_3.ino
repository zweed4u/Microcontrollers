//Lab_1_hello_arduino
#define LED 13
char inChar;
boolean isFreshInChar;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  // Set serial monitor console termination for 'No line ending'
  Serial.begin(9600);
  Serial.println("Lab 1: hello arduino v0.4\n");
  delay(5000);
}

void loop() {
  isFreshInChar = false;
  if (Serial.available()) {
    inChar = Serial.read();
    Serial.print("Serial input detected: ");
    Serial.println(inChar);
    isFreshInChar = true;
  }
  if (inChar == 'n') digitalWrite(LED, HIGH); // oN
  if (inChar == 'f') digitalWrite(LED, LOW); // oFf
  if (inChar == 'b') { // blink with 25% duty cycle
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(750);
  }

  // function for readability: blink with 75% duty cycle
  if (inChar == 'B') blinkLED(750,1000);

  // Discover 't' persistence bug by observing high rate LED blink
  if (inChar == 't') { // toggle
    digitalWrite(LED, !digitalRead(LED));
  }

  // Add state change detection to get proper toggle action.
  if (inChar == 'T') { // toggle
    if (isFreshInChar) digitalWrite(LED, !digitalRead(LED));
  }
} // loop()


//********************************************************************************
void blinkLED(unsigned int msecT, unsigned int msecOn) {
  // Check for valid input parameters, if not valid, fix them so they are
  if (msecT < 2) msecT = 2; // need at least 2 msec to blink with msec resolution
  if (msecOn >= msecT) msecOn = msecT/2; // 50% if ill posed
  digitalWrite(LED, HIGH);
  delay(msecOn);
  digitalWrite(LED, LOW);
  delay(msecT - msecOn);
}
