const byte DEBUG_COMMANDS_LENGTH = 7;
const String DEBUG_COMMANDS[DEBUG_COMMANDS_LENGTH] = {
  // Assigns a number to the following commands. Change DEBUG_COMMANDS_LENGTH when adding to this translation
  "unknown",
  "echo",
  "mod",
  "exit",
  "buffer",
  "setbuffer",
  "printglyph",
};

void LoopDebug()
{
  if (Serial.available() > 0) // Recived command
  {
    // Get message
    String message;
    while (Serial.available() > 0)
    {
      char in = Serial.read();
      if (in == '\n')
        break;     // '\n' is message seperator
      message += in; // Add to buffer
      delay(10);     // Wait for more chars
    }

    // Extract the command
    int i = message.indexOf(' ');
    String command = (i != -1) ? message.substring(0, i) : message;

    // Count 'n' number of words, if any, and extract them
    int wl = 0;
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
    DebugEnd = millis() + DEBUG_TIMEOUT; // Reset timeout

    // Find Command
    n = 0; // let n be command number
    for (i = 0; i < DEBUG_COMMANDS_LENGTH; i++)
    {
      if (DEBUG_COMMANDS[i] == command)
      {
        n = i;
        break;
      }
    }
    Serial.print("> ");
    Serial.println(command); //TEST

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
            Serial.print(((DisplayBuffer[i] & (1 << j)) ? "##" : "  ")); // Print "##" if that LED is on
          }
          Serial.println(); // Next row
        }
        break;
      case 6:
      if (wl < 2)
      for (size_t i = 0; i < 16; i++) // For every row
      {
        DisplayBuffer[i] = words[i+1].toInt();
      }
        break;
      case 7:
        for (short i = 0; i < 16; i++) {
          matrixData(i, DisplayBuffer[i]); // Replace '0b1001' with correct buffer row
          delay(2);
        }
        break;
    }
  }
  else if (millis() > DebugEnd)
  {
    Debugging = false;
  }
  delay(DELAY_MODE_DEBUG);
}
