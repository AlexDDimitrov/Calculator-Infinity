// Tens digit BCD pins (4511 #1)
int a1 = 8;   // A (1)
int a2 = 11;  // B (2)
int a3 = 10;  // C (4)
int a4 = 9;   // D (8)

// Ones digit BCD pins (4511 #2)
int b1 = 3;   // A (1)
int b2 = 6;   // B (2)
int b3 = 5;   // C (4)
int b4 = 4;   // D (8)

// 5‑bit input pins (analog or digital)
int S0_pin = A1;   // bit 0 (LSB)
int S1_pin = A2;   // bit 1
int S2_pin = A3;   // bit 2
int S3_pin = A4;   // bit 3
int S4_pin = A5;   // bit 4 (MSB)

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
}

void loop() {

  // Read 5 input bits (thresholded analog)
  int raw0 = analogRead(S0_pin);
  int raw1 = analogRead(S1_pin);
  int raw2 = analogRead(S2_pin);
  int raw3 = analogRead(S3_pin);
  int raw4 = analogRead(S4_pin);

  int bit0 = (raw0 > 512);
  int bit1 = (raw1 > 512);
  int bit2 = (raw2 > 512);
  int bit3 = (raw3 > 512);
  int bit4 = (raw4 > 512);

  // Convert to decimal 0–31
  int result = 
      (bit0 << 0) |
      (bit1 << 1) |
      (bit2 << 2) |
      (bit3 << 3) |
      (bit4 << 4);

  // Split into tens + ones
  int tens = result / 10;
  int ones = result % 10;

  // Output to 4511 drivers
  displayDigit(tens, a1, a2, a3, a4);
  displayDigit(ones, b1, b2, b3, b4);

  // Debug
  Serial.print("Bits: ");
  Serial.print(bit4); Serial.print(bit3);
  Serial.print(bit2); Serial.print(bit1); Serial.print(bit0);
  Serial.print("  Result: ");
  Serial.println(result);

  delay(30);
}

void displayDigit(int value, int pA, int pB, int pC, int pD) {
  digitalWrite(pA, (value & 1) ? HIGH : LOW);
  digitalWrite(pB, (value & 2) ? HIGH : LOW);
  digitalWrite(pC, (value & 4) ? HIGH : LOW);
  digitalWrite(pD, (value & 8) ? HIGH : LOW);
}
