// Pin connected to the EMG sensor output
const int emgPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from the EMG sensor
  int emgValue = analogRead(emgPin);

  // Print the EMG value to the serial monitor
  Serial.println(emgValue);

  delay(100);  // Adjust delay as needed
}