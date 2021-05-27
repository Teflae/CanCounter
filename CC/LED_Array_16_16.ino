//matrix code
#define LATCH 2
#define CLK 3
#define DATA 4
#define EN 5
//ROW(1) => Pin 6; change to your set up if different.
#define ROW(x) (x+5)
int glyph[1][16] = {{0b1110000000000000, 0b0010000000000000, 0b1110000000000000, 0b1000000000000000, 0b1110000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000, 0b0000000000000000}};
int digit = 1;
int coordinatesX = 1;
int coordinatesY = 1;
int empty = 0b0000000000000000;

int peaks[16] = {0};
int peakChanges[16] = {0};

void setup() {

  Serial.begin(57600);
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40;  // use adc0
  DIDR0 = 0x01;  // turn off the digital input for adc0

  pinMode( LATCH, OUTPUT);  //set up pins
  pinMode( CLK, OUTPUT);
  pinMode( DATA, OUTPUT);
  pinMode( EN, OUTPUT);
  pinMode( ROW(1), OUTPUT);
  pinMode( ROW(2), OUTPUT);
  pinMode( ROW(3), OUTPUT);
  pinMode( ROW(4), OUTPUT);
  for (short n = 0; n < 16; n++) {
    for (short k = (digit - 1); k < digit; k++) {
      if (coordinatesY > 0) {
        if (n < 5) {
          glyph[k][((coordinatesY + 4) - (n))] = glyph[k][(3 - (n - 1))];
          glyph[k][(3 - (n - 1))] = empty;
        }
      }
    }
  }
}

void loop() {
  for (short i = 0; i < 16; i++) {
    for (short j = (digit - 1); j < digit; j++) {
      matrixData(i, ((unsigned int)glyph[j][i] >> coordinatesX));
      delay(10);
    }
  }
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
}
