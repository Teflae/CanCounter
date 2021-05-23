/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/
#define Dampen 10
#define Avg 500
double avg = 1;
double damp = 1;
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(56000);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A1);
  damp = (damp * (Dampen - 1) + sensorValue)/Dampen;
  avg = (avg * (Avg - 1) + sensorValue)/Avg;
  // print out the value you read:
  Serial.print(sensorValue);
  Serial.print('|');
  Serial.print(damp);
  Serial.print('|');
  Serial.print(avg);
  Serial.println();
  delay(10);        // delay in between reads for stability
}
