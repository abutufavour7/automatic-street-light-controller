// Automatic Street Light Controller
// Arduino Uno

const int LDR_PIN = A0;        // LDR voltage divider
const int POT_PIN = A1;        // Adjustable threshold
const int BUTTON_PIN = 2;      // Manual override button
const int RELAY_PIN = 7;       // Relay control
const int GREEN_LED = 8;       // AUTO mode indicator
const int RED_LED = 9;         // FORCE-ON indicator

const int HYSTERESIS = 30;     // Dead band (ADC units)
const int DEBOUNCE_DELAY = 50; // Button debounce (ms)
const unsigned long SWITCH_DELAY = 3000; // 3s before switching

// Modes
#define AUTO       0
#define FORCE_ON   1
#define FORCE_OFF  2

int mode = AUTO;
bool lampState = false;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long lightChangeTime = 0;
bool pendingChange = false;
bool pendingState = false;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(9600);
}

void updateLEDs() {
  if (mode == AUTO) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  } else if (mode == FORCE_ON) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    // FORCE_OFF
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void setLamp(bool state) {
  lampState = state;
  digitalWrite(RELAY_PIN, state ? HIGH : LOW);
}

void loop() {
  // ── Button handling (debounced) ──
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading == LOW) { // Button pressed
      mode = (mode + 1) % 3; // Cycle AUTO → FORCE_ON → FORCE_OFF
      pendingChange = false;  // Cancel any pending switch
      updateLEDs();
      delay(300); // Prevent multiple triggers
    }
  }
  lastButtonState = reading;

  // ── Manual override modes ──
  if (mode == FORCE_ON) {
    setLamp(true);
    return;
  }
  if (mode == FORCE_OFF) {
    setLamp(false);
    return;
  }

  // ── AUTO mode ──
  int ldrValue = analogRead(LDR_PIN);
  int threshold = analogRead(POT_PIN);

  Serial.print("LDR: "); Serial.print(ldrValue);
  Serial.print("  Threshold: "); Serial.print(threshold);
  Serial.print("  Lamp: "); Serial.println(lampState ? "ON" : "OFF");

  // Determine desired state with hysteresis
  bool desiredState = lampState; // Hold current state by default

  if (ldrValue > threshold + HYSTERESIS) {
    desiredState = true;  // Dark → lamp ON
  } else if (ldrValue < threshold - HYSTERESIS) {
    desiredState = false; // Bright → lamp OFF
  }

  // 3-second delay before switching
  if (desiredState != lampState) {
    if (!pendingChange) {
      pendingChange = true;
      pendingState = desiredState;
      lightChangeTime = millis();
    } else if (desiredState != pendingState) {
      // Condition reversed before delay elapsed — cancel
      pendingChange = false;
    } else if (millis() - lightChangeTime >= SWITCH_DELAY) {
      setLamp(pendingState);
      pendingChange = false;
    }
  } else {
    pendingChange = false;
  }

  updateLEDs();
  delay(100);
}