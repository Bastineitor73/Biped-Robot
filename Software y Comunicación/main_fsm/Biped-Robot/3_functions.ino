// ------------------------- Logging -------------------------------
// Appends the event to the log file
bool appendLog(const char* message) {
  // Checks SD card presence
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD not initialized.");
    return false;
  }
  // Checks file availability
  File file = SD.open(LOG_PATH, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open log file.");
    return false;
  }
  //Gets the time
  char ts[20];
  rtcNowISO(ts, sizeof(ts));
  //Writes to file
  file.print(ts);
  file.print(" - ");
  file.print(userName);
  file.print(" - ");
  file.println(message);
  file.close();
  return true;
}

/* ------------------ RTC -------------------- */

// Format current RTC time as "YYYY-MM-DD HH:MM:SS"
void rtcNowISO(char* buf, size_t buflen) {
  time_t t = now();
  snprintf(buf, buflen, "%04d-%02d-%02d%02d:%02d:%02d",
           year(t), month(t), day(t),
           hour(t), minute(t), second(t));
}

// Prints the time to console
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

// Adds leading 0 in single digit number
void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

// Parses the incoming serial message in the Unix format
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1757442497;  // Sep 9 2025
  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if (pctime >= DEFAULT_TIME) {  // Check valid integer
      setTime(pctime);             // Sync Arduino clock to the time received on the serial port
    }
  }
}
