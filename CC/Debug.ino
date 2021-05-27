const byte DebugCommandsLength = 5;
const String DebugCommands[DebugCommandsLength] = {
  // Assigns a number to the following commands. Change DebugCommandsLength when adding to this translation
  "unknown",
  "echo",
  "mod",
  "exit",
  "buffer",
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
    DebugEnd = millis() + DebugTimeut; // Reset timeout

    // Find Command
    n = 0; // let n be command number
    for (i = 0; i < DebugCommandsLength; i++)
    {
      if (DebugCommands[i] == command)
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
        SerialPrintError("Not implemented");
        break;
    }
  }
  else if (millis() > DebugEnd)
  {
    Debugging = false;
  }
  delay(DelayModeDebug);
}
