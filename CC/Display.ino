#define LATCH 2
#define CLK 3
#define DATA 4
#define EN 5
//ROW(1) => Pin 6; change to your set up if different.
#define ROW(x) (x+5)
const int LIBRARY_DIGITS[10][5][3] = {
  {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}},
  {{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}},
  {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
  {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
  {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
  {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
  {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
  {{1, 1, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}},
  {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
  {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
};
short DisplaySplash[16] = {
  0b1111111111111111,
  0b1110000111000011,
  0b1100111110011111,
  0b1101111110111111,
  0b1100111110011111,
  0b1110000111000011,
  0b1111111111111111,
  0b0000000000000000,
  0b1111101001001100,
  0b0010001001010000,
  0b0010001111001100,
  0b0010001001000010,
  0b0010001001001100,
  0b0000000000000000,
  0b0000000000000000,
  0b1111111111111111,
};

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
}

void matrixLoop() {
  if (!USE_MATRIX) return; // If not using matrix, exit function
  if (Debugging) { //display splash screen in debug mode
    for (int i = 0; i < 16; i++) {
      DisplayBuffer[i] = DisplaySplash[i];
      for (short i = 0; i < 16; i++) {
        matrixData(i, DisplayBuffer[i]); // Print buffer row 'i'
        delay(1); //can't be removed
      }
    }
  }
  else {
    DisplayDigit(AllCount);
    for (short i = 0; i < 16; i++) {
      matrixData(i, DisplayBuffer[i]); // Print buffer row 'i'
      delay(1); //can't be removed
    }
  }
}

void DisplayDigit(int digit) {
  int n = digit;
  int digits[4];
  int rowprint = 0;
  int bitprint = 2;
  for (int i = 0; i < 16; i++) { //clearing buffer
    DisplayBuffer[i] = 0b0000000000000000;
  };
  for (int i = 0; i < 4; i++) { //Splitting digit value into separate integers
    digits[i] = n % 10;
    n /= 10;
  };
  int j = 0;
  for (int i = 0; i < 4; i++) {
    switch (i) { //switching between each digit
      case 1:
        j = 0;
        rowprint = 4;
        for (int q = 0; q < 5; q++) {
          do {
            bitWrite(DisplayBuffer[j], rowprint, LIBRARY_DIGITS[digits[i]][q][bitprint]); //sets buffer to digit based on library array
            rowprint++;
            bitprint--;
          } while (rowprint < 7);
          rowprint = 4;
          bitprint = 2;
          j = j + 1;
        }
        break;
      case 2:
        j = 0;
        rowprint = 8;
        for (int q = 0; q < 5; q++) {
          do {
            bitWrite(DisplayBuffer[j], rowprint, LIBRARY_DIGITS[digits[i]][q][bitprint]);
            rowprint++;
            bitprint--;
          } while (rowprint < 11);
          rowprint = 8;
          bitprint = 2;
          j = j + 1;
        }
        break;
      case 3:
        j = 0;
        rowprint = 12;
        for (int q = 0; q < 5; q++) {
          do {
            bitWrite(DisplayBuffer[j], rowprint, LIBRARY_DIGITS[digits[i]][q][bitprint]);
            rowprint++;
            bitprint--;
          } while (rowprint < 15);
          rowprint = 12;
          bitprint = 2;
          j = j + 1;
        }
        break;
      default:
        j = 0;
        rowprint = 0;
        for (int q = 0; q < 5; q++) {
          do {
            bitWrite(DisplayBuffer[j], rowprint, LIBRARY_DIGITS[digits[i]][q][bitprint]);
            rowprint++;
            bitprint--;
          } while (rowprint < 3);
          rowprint = 0;
          bitprint = 2;
          j = j + 1;
        }
        break;
    }
  }
