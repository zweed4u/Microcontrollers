//Lab_1_hello_arduino
#define LED 13
char inChar;


void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  // Set serial monitor console termination for 'No line ending'
  Serial.begin(9600);
  Serial.println("Lab 1: hello arduino v0.1\n");
  delay(5000);
}
void loop() {
if (Serial.available()) {
inChar = Serial.read();
Serial.print("Serial input detected: ");
Serial.println(inChar);
}
if (inChar == 'n') digitalWrite(LED, HIGH); // oN
if (inChar == 'f') digitalWrite(LED, LOW); // oFf
} // loop()
