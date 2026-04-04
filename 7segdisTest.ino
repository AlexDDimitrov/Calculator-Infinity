// Tens digit BCD pins (4511 #1)
int a1 = 8;
int a2 = 11;
int a3 = 10;
int a4 = 9;

// Ones digit BCD pins (4511 #2)
int b1 = 3;
int b2 = 6;
int b3 = 5;
int b4 = 4;

// Input bits
int S0_pin = A1;   // bit0
int S1_pin = A2;   // bit1
int S2_pin = A3;   // bit2
int S3_pin = A4;   // bit3
int S4_pin = A5;   // Sign bit (1 = NEGATIVE, 0 = POSITIVE)

// Subtract pin
int SUB_pin = A0;

// Negative output pin
int NEG_pin = 12;

void setup() {
  Serial.begin(9600);

  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a4, OUTPUT);

  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);

  pinMode(NEG_pin, OUTPUT);
}

int readMagnitude() {
  int b0 = (analogRead(S0_pin) > 512);
  int b1 = (analogRead(S1_pin) > 512);
  int b2 = (analogRead(S2_pin) > 512);
  int b3 = (analogRead(S3_pin) > 512);

  return (b0 << 0) |
         (b1 << 1) |
         (b2 << 2) |
         (b3 << 3);
}

void loop() {

  bool subtractMode = (analogRead(SUB_pin) > 512);

  int magnitude = readMagnitude();
  bool signBit = (analogRead(S4_pin) > 512); // A5 = 1 means NEGATIVE

  // Convert to signed number
  int inputValue = signBit ? -magnitude : magnitude;

  int result;

  //Add or subtract based on mode
  if (subtractMode) {
    result = 0 - inputValue;
  } else {
    result = inputValue;
  }

  // Negative check
  bool isNegative = (result < 0);

  if (isNegative) {
    digitalWrite(NEG_pin, HIGH);
    result = -result;  // show absolute value
  } else {
    digitalWrite(NEG_pin, LOW);
  }

  // Clamp to 0–31
  if (result > 31) result = 31;

  // Split into tens + ones
  int tens = result / 10;
  int ones = result % 10;

  displayDigit(tens, a1, a2, a3, a4);
  displayDigit(ones, b1, b2, b3, b4);

  // Debug
  Serial.print("Magnitude: ");
  Serial.print(magnitude);
  Serial.print("  Sign: ");
  Serial.print(signBit ? "-" : "+");
  Serial.print("  Mode: ");
  Serial.print(subtractMode ? "SUB" : "ADD");
  Serial.print("  Result: ");
  Serial.print(result);
  Serial.print("  Negative: ");
  Serial.println(isNegative);

  delay(30);
}

void displayDigit(int value, int pA, int pB, int pC, int pD) {
  digitalWrite(pA, (value & 1) ? HIGH : LOW);
  digitalWrite(pB, (value & 2) ? HIGH : LOW);
  digitalWrite(pC, (value & 4) ? HIGH : LOW);
  digitalWrite(pD, (value & 8) ? HIGH : LOW);
}
