#define LATCH 2
#define CLK 3
#define DATA 4
#define EN 5
//ROW(1) => Pin 6; change to your set up if different.
#define ROW(x) (x+5)

void MatrixSetup() {
  pinMode( LATCH, OUTPUT);  //set up pins
  pinMode( CLK, OUTPUT);
  pinMode( DATA, OUTPUT);
  pinMode( EN, OUTPUT);
  pinMode( ROW(1), OUTPUT);
  pinMode( ROW(2), OUTPUT);
  pinMode( ROW(3), OUTPUT);
  pinMode( ROW(4), OUTPUT);
}

void matrixData(short row, int bitmask) {
  digitalWrite(EN, HIGH);
  digitalWrite(ROW(1), row & 1); // 0001
  digitalWrite(ROW(2), row & 2); // 0010
  digitalWrite(ROW(3), row & 4); // 0100
  digitalWrite(ROW(4), row & 8); // 1000
  digitalWrite(LATCH, LOW);

  shiftOut(DATA, CLK, LSBFIRST, ~((bitmask)      & 0xFF)); //lower portion
  shiftOut(DATA, CLK, LSBFIRST, ~((bitmask >> 8) & 0xFF)); //higher portion
  digitalWrite(LATCH, HIGH);
  digitalWrite(EN, LOW);
  delay(1); //TODO: Remove if you can
}

void LoopMatrix() {
  if (!USE_MATRIX) return; // If not using matrix, exit function
  for (short i = 0; i < 16; i++) {
    matrixData(i, DisplayBuffer[i]); // Print buffer row 'i'
  }
}
