const byte DEBUG_COMMANDS_LENGTH = 8;
const String DEBUG_COMMANDS[DEBUG_COMMANDS_LENGTH] = {
  // Assigns a number to the following commands. Change DEBUG_COMMANDS_LENGTH when adding to this translation
  "unknown",
  "echo",
  "mod",
  "exit",
  "buffer",
  "set",
  "printglyph",
  "test",
};
const byte TEST_FLAGS_LENGTH = 2;
const String TEST_FLAGS[TEST_FLAGS_LENGTH] = {
  "null",
  "EO",
};
const int TEST_EO = 1;

int Translate(String dictionary[], byte dictionaryLength, String in) { // Returns the index of the word in the dictionary that matches the input
  in.toLowerCase();
  int n = in.toInt();
  if (n == 0) {
    for (int i = 0; i < dictionaryLength; i++)
    {
      if (dictionary[i] == in) n = i;
    }
  }
  if (n < 0 || n > dictionaryLength) n = 0;
  return n;
}

void LoopDebug()
{
  if (Serial.available() > 0) // Recived command
  {
    DebugEnd = millis() + DEBUG_TIMEOUT; // Reset timeout
    // Get message
    String message;
    while (Serial.available() > 0)
    {
      char in = Serial.read();
      if (in == '\n' || in == '/')
        break;     // '\n' and '/' are message seperator
      message += in; // Add to buffer
      delay(10);     // Wait for more chars
    }

    // Extract the command
    message.trim();
    int i = message.indexOf(' ');
    String command = (i != -1) ? message.substring(0, i) : message;

    // Count 'n' number of words, if any, and extract them
    int wl = 0; // number of words
    while (i != -1)
    {
      i = message.indexOf(' ', i + 1);
      wl++;
    }
    String words[wl + 1];
    i = 0;
    int n = 0; //# of Commands
    while (i != -1)
    {
      int j = message.indexOf(' ', i + 1);
      words[n] = (j != -1) ? message.substring(i + 1, j) : message.substring(i + 1);
      i = j;
      n++;
    }

    // Find Command
    n = Translate(DEBUG_COMMANDS, DEBUG_COMMANDS_LENGTH, command); // let n be command number
    // for (i = 0; i < DEBUG_COMMANDS_LENGTH; i++)
    // {
    //   if (DEBUG_COMMANDS[i] == command)
    //   {
    //     n = i;
    //     break;
    //   }
    // }
    Serial.print("> ");
    Serial.println(DEBUG_COMMANDS[n]); //TEST

    // Run Command. There's no try-catch blocks in arduino so be careful
    switch (n)
    {
      case 0: // Not identified;
        SerialPrintError("Command '" + command + "' is not recognosed. Consult manual, available at https://github.com/Teflae/CanCounter");
        break;
      case 1: // Echo
        if (wl > 0)
          Serial.println(words[1]);
        else
          Serial.println(command);
        break;
      case 2: // Modulo operator
        if (wl < 2)
          SerialPrintError("2 parameters needed");
        else
          Serial.println(words[1].toInt() % words[2].toInt());
        break;
      case 3: // Exit
        Debugging = false;
        break;
      case 4: // Print display buffer
        for (size_t i = 0; i < 16; i++) // For every row
        {
          for (size_t j = 0; j < 16; j++) // For every column
          {
            Serial.print(((DisplayBuffer[i] & (1 << (15 - j))) != 0) ? "<>" : ". "); // Print "<>" if that LED is on
          }
          //Serial.print(DisplayBuffer[i], BIN); // For testing
          Serial.println(); // Next row
        }
        break;
      case 5: // Set buffer
        if (wl < 2)
          SerialPrintError("2 parameters needed: a, b");
        else {
          DisplayBuffer[words[1].toInt()] = words[2].toInt();
        }
        break;
      case 6: // Um.... print buffer to Matrix
        if (!USE_MATRIX) break;
        for (short i = 0; i < 16; i++) {
          MatrixData(i, DisplayBuffer[i]); // Replace '0b1001' with correct buffer row
          delay(2);
        }
        break;
      case 7: // Change test parameters
        if (wl < 1)
          SerialPrintError("1 parameters needed: ");
        else {
          Test = Translate(TEST_FLAGS, TEST_FLAGS_LENGTH, words[1]);
          Serial.print("Test mode: ");
          Serial.println(TEST_FLAGS[n]); //TEST
        }
    }
  }
  else if (millis() > DebugEnd) // Exit debug mode ontimeout
  {
    Debugging = false;
  }
  delay(DELAY_MODE_DEBUG);
}
