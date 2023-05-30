const int emgPin = A0;
const int threshold = 400;
const int movementThreshold = 500;
const int consecutiveMovementThreshold = 500;
const int consecutiveMovementDelay = 3000;

unsigned long lastMovementTime = 0;
bool wristMovementDetected = false;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int emgValue = analogRead(emgPin);
  

  if (emgValue > threshold) {
    // Check for wrist movement
    if (!wristMovementDetected) {
      if (emgValue > movementThreshold) {
        wristMovementDetected = true;
        lastMovementTime = millis();
      }
    } else {
      unsigned long currentTime = millis();
      unsigned long timeDifference = currentTime - lastMovementTime;

      if (emgValue > consecutiveMovementThreshold && timeDifference < consecutiveMovementDelay) {
        // Two consecutive wrist movements within the specified time
        Serial.println("fan");
        wristMovementDetected = false;
      } else if (timeDifference >= consecutiveMovementDelay) {
        // Only one wrist movement detected within the time window
        Serial.println("light");
        wristMovementDetected = false;
      }
    }
  }

  delay(100);  // Adjust delay as needed
}
