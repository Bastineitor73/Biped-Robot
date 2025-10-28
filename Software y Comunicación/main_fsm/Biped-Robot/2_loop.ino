void loop() {


  if(mp27._scanInProgress){
    mp27.initHR();
  }

//FSM - WIP
/*

  // switch (robotMode)
  // {
  // case isIdle:
  //   // checkAlerts();
  //   // if (mp27.getInterrupts(intFlags)){
  //   //   robotMode = isAlert;
  //   // }
  //   Serial.println("isIdle")
  //   break;
  // case isActive:
  //     // checkAlerts();
  //   Serial.println("isActive")
  //     break;
  // case isAlert:
  //     // mp27.getInterrupts(intFlags)
  //   Serial.println("isAlert")
  //     break;
  // case isError:
  //   Serial.println("isError")
  //     break;
  // default:
    // break;
  //   bool reading = digitalRead(BUTTON_PIN);
  //   uint32_t nowMs = millis();

  //   if (reading != lastButtonState) {
  //       lastChangeMs = nowMs;
  //       lastButtonState = reading;
  //   }
  //   static bool stableState = HIGH;

  //   if ((nowMs - lastChangeMs) > DEBOUNCE_MS) {
  //       if (reading != stableState) {
  //           stableState = reading;
  //           if (stableState == LOW) {  // button pressed
  //               if (appendLog("Button pressed")) {
  //                   Serial.println("Log written: Button pressed");
  //               } else {
  //                   Serial.println("Failed to write log.");
  //               }
  //           }
  //       }
  //   }
*/


// digitalClockDisplay();

// if (!digitalRead(BUTTON_PIN) && ((millis()- lastChangeMs)>DEBOUNCE_MS)){
//   Serial.println("button pressed");
//   lastChangeMs = millis();
// }


//Pre config sequence
 //Setup sequence when button is pressed
  if ((digitalRead(BUTTON_PIN) == LOW) && (millis() - lastMillis > BUTTON_DEBOUNCE)){
    lastMillis = millis();
    Serial.println("Button pressed");
    if (buttonCounter < 1){
      Serial.println(" initializing MP2790 Setup...");
      setupMP2790();
      setupValues();
      buttonCounter++;
      // return;
    }
    checkSensors();
    checkInt();
    mp27.getFault();
    if (pwrStatus == 8 || pwrStatus == 1){
      mp27.writeAdd(MP2790_Reg::DRIVER_FAULT_CLR, 1);
      mp27.writeAdd(MP2790_Reg::ACTIVE_CTRL, 1);
      Serial.println("Fet on.");
    }
    if (pwrStatus == 16){
      mp27.writeAdd(MP2790_Reg::ACTIVE_CTRL, 0);
      Serial.println("Fet off.");
    }
  }

  // //Fet testing
  // if (millis() > 30000){
  //   if((second() % 3) == 0 && (second()) % 8 != 0 && fetcounter == 0){
  //     mp27.writeAdd(MP2790_Reg::DRIVER_FAULT_CLR, 1);
  //     mp27.writeAdd(MP2790_Reg::ACTIVE_CTRL, 1);
  //     Serial.println("FET on");
  //     fetcounter++;
  //   }
  //   if(((second()) % 8 == 0) && (fetcounter == 1)){
  //     mp27.writeAdd(MP2790_Reg::DRIVER_FAULT_CLR, 1);
  //     mp27.writeAdd(MP2790_Reg::ACTIVE_CTRL, 0 );
  //     Serial.println("FET off");
  //     fetcounter = 0;
  //   }   
  // }


}
