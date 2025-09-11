void loop() {

  // Serial input
  //Time
  if (timeStatus() == timeNotSet){
    Serial.println("Input time");
    if (Serial.available()) {
      processSyncMessage();
    }
    return;
  }
  //UserName
  if( userName == null){
    Serial.println("Input User Name");
    if(Serial.available()){
      userName = Serial.read();
    }
    return;
  }

  switch (robotMode)
  {
  case isIdle:{
    checkAlerts();
    if (mp27.getInterrupts(intFlags)){
      robotMode = isAlert;
    }
    break;}
  case isActive:
      checkAlerts();
      break;

  case isAlert:
      mp27.getInterrupts(intFlags)
      break;
      
  case isError:
      break;

  default:{
    bool reading = digitalRead(BUTTON_PIN);
    uint32_t nowMs = millis();

    if (reading != lastButtonState) {
        lastChangeMs = nowMs;
        lastButtonState = reading;
    }
    static bool stableState = HIGH;

    if ((nowMs - lastChangeMs) > DEBOUNCE_MS) {
        if (reading != stableState) {
            stableState = reading;
            if (stableState == LOW) {  // button pressed
                if (appendLog("Button pressed")) {
                    Serial.println("Log written: Button pressed");
                } else {
                    Serial.println("Failed to write log.");
                }
            }
        }
    }
    break;
  }
  }
}
