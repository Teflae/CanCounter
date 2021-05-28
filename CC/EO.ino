const double DAMPEN = 20.0;
const double DAMPEN_LONG = 2000.0;
const uint8_t PIN_LIGHT_SENSOR = A0;
const uint8_t PIN_LASER = 11;
double Average = 1;
double Reading = 1;

double Dampen (double original, double in, double rate) {
  return (original * (rate - 1) + in) / rate;
}

void SetupEO()
{
  pinMode(PIN_LASER, OUTPUT);
  digitalWrite(PIN_LASER, HIGH);
}

void LoopEO()
{
  int sensorValue = analogRead(PIN_LIGHT_SENSOR);
  Reading = (Reading * (DAMPEN - 1) + sensorValue) / DAMPEN;
  Average = (Average * (DAMPEN_LONG - 1) + sensorValue) / DAMPEN_LONG;
  // print out the value you read:
  Serial.print(sensorValue);
  Serial.print('|');
  Serial.print(Reading);
  Serial.print('|');
  Serial.print(Average);
  Serial.println();
  if (Serial.available() > 0)
  {
    Debugging = true;
    DebugEnd = millis() + DEBUG_TIMEOUT;
    Serial.println(DEBUG_MESSAGE);
  }
}