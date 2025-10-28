// Init
void serialInput(){
  // Serial input
  //Time
  if (timeStatus() == timeNotSet){
    Serial.println("Input time");
    while (!Serial.available()) {
    }
    processSyncMessage();
  }

  //UserName
  if(userName == 0){
    Serial.println("Input User Name");
    while (!Serial.available()) {
      Serial.print("waiting");
    }
    userName = Serial.read();
  }
}

// Mp2790

void setupMP2790() {

  // Check Address
  // uint16_t addressi2c = mp27.getAddress(); //reads address from object
  uint16_t addressi2c = mp27.readAdd(MP2790_Reg::DEVICE_ADD); //reads address using register
  Serial.print("MP2790 I2C Address: ");
  Serial.println(addressi2c, HEX);
  
  // //Checking MTP status
  // Serial.println("Checking MTP status");
  // uint16_t mtpStatus = mp27.readReg(MP2790_Reg::MTP_STATUS); //by register
  // Serial.print("MTP Status: ");
  // Serial.println(mtpStatus, BIN);
  // Serial.print("MEMORY STATUS: ");
  // Serial.println(mp27.readAdd(MEM_STATUS), BIN); //by address

  // // //Writing new address
  // Serial.println("Writing new address");
  // mp27.setAddress(11);                                                   //writes 11 as device address on the go
  // // mp27.writeReg(MP2790_Reg::DEVICE_ADD, 11);                          //writes 11 to device register, needs to initialize
  // //// Serial.println(mp27.writeAdd(COMM_CFG, 0b00000000101100000000));  //writes 11 to device address, needs to initialize
  // Serial.println("Initializing new address");
  // mp27 = MP2790(11);                                                     //initializes with new address
  // // Re-Check Address
  // Serial.print("MP2790 I2C Address2: ");
  // addressi2c = mp27.readAdd(COMM_CFG);
  // Serial.println(addressi2c, HEX);
  // //Re-Check Connection
  // mp27.testConnection();


  // Storing toNVM page
  // Serial.println("Access code input");
  // mp27.storeAccessCode();
  // Serial.println("Storing NVM page");
  // if (mp27.storeNVM() != true) {
  //   Serial.println("Failed to store NVM page.");
  // } else {
  //   Serial.println("NVM page stored successfully.");
  // }

  // //Checking MTP satus
  // Serial.println("Checking MTP status");
  // uint16_t mtpStatus = mp27.checkMTPStatus();
  // Serial.print("MTP Status: ");
  // Serial.println(mtpStatus, HEX);

}
void setupValues(){
  // // Importing data
  Serial.println("Loading configuration values...");
  mp27.writeAdd(0x00, 0x00C5);   // CELLS_CTRL C3: 4cells ; c5: 6cells
  mp27.writeAdd(0x05, 0x0008);   // ACT_CFG 0x0000: all off 0x0008: all on  
  mp27.writeAdd(0x06, 0x0022);   // STB_CFG
  mp27.writeAdd(0x07, 0x000D);   // SAFE_CFG
  mp27.writeAdd(0x08, 0x0005);   // RGL_CFG
  mp27.writeAdd(0x09, 0x0500);   // LOAD_CHARGER_CFG
  mp27.writeAdd(0x0B, 0x0000);   // GPIO_OUT
  mp27.writeAdd(0x0C, 0x0444);   // GPIO_CFG
  mp27.writeAdd(0x0D, 0x0061);   // PINS_CFG
  mp27.writeAdd(0x10, 0x4E9C);   // WDT_CFG 0x4E9D: WDT on 0x4E9C: WDT off
  mp27.writeAdd(0x12, 0x0000);   // FET_CTRL
  mp27.writeAdd(0x13, 0x051B);   // FET_MODE
  mp27.writeAdd(0x14, 0x68F0);   // FET_CFG 0x68F0: 10Vgs, 0x08F0: 5Vgs
  mp27.writeAdd(0x19, 0xC7FF);   // INTO_EN
  mp27.writeAdd(0x1A, 0x2FEE);   // INT1_EN 0x2FFE: normal, 0x2FEE: ow off
  mp27.writeAdd(0x1B, 0x0000);   // INT_TYPE0
  mp27.writeAdd(0x1C, 0x0000);   // INT_TYPE1
  mp27.writeAdd(0x1D, 0x0000);   // INT_TYPE2
  mp27.writeAdd(0x1E, 0x40FF);   // MASK_INTO
  mp27.writeAdd(0x1F, 0x0142);   // MASK_INT1
  mp27.writeAdd(0x23, 0x01BF);   // OCFT_CTRL
  mp27.writeAdd(0x24, 0x2C10);   // DSGOC_LIM && 0x2C10: 42.5mV
  mp27.writeAdd(0x25, 0x0428);   // DSGOC_DEG
  mp27.writeAdd(0x26, 0x0404);   // CHGOC_DEG
  mp27.writeAdd(0x2A, 0x003F);   // SCFT_CTRL
  mp27.writeAdd(0x2B, 0x0115);   // DSGSC_CFG
  mp27.writeAdd(0x2C, 0x0804);   // CHGSC_CFG
  mp27.writeAdd(0x34, 0x1A33);   // PACKFT_CTRL (dead cell) 0x1B33: report latch, 0x1F33: immediate, 0x1A33: disable 
  mp27.writeAdd(0x35, 0x6036);   // CELLFT_CTRL
  mp27.writeAdd(0x36, 0x0AA0);   // CELL_HST
  mp27.writeAdd(0x37, 0x8080);   // PACK_UV_OV
  mp27.writeAdd(0x38, 0x009F);   // CELL_UV
  mp27.writeAdd(0x39, 0x00D7);   // CELL_OV
  mp27.writeAdd(0x3A, 0x03B3);   // PACK_UV
  mp27.writeAdd(0x3B, 0x0500);   // PACK_OV
  mp27.writeAdd(0x3C, 0x0068);   // CELL_DEAD_THR
  mp27.writeAdd(0x3D, 0x0002);   // CELL_MSMT
  mp27.writeAdd(0x44, 0x0000);   // NTC_CLR
  mp27.writeAdd(0x46, 0x000A);   // DIE_CFG
  mp27.writeAdd(0x47, 0xA4F5);   // NTC_CFG  0xE4F5: def faults on. 0xA4F5: ntc dsg cold off
  mp27.writeAdd(0x48, 0x012E);   // NTCC_OTHR_DSG
  mp27.writeAdd(0x49, 0x0294);   // NTCC_UTHR_DSG
  mp27.writeAdd(0x4A, 0x012E);   // NTCC_OTHR_CHG
  mp27.writeAdd(0x4B, 0x8A94);   // NTCC_UTHR_CHG
  mp27.writeAdd(0x4C, 0x80EB);   // NTCM_OTHR
  mp27.writeAdd(0x4D, 0xAAEC);   // DIE_OT
  mp27.writeAdd(0x4E, 0x0000);   // SELF_STS
  mp27.writeAdd(0x55, 0x0501);   // SFT_GO
  mp27.writeAdd(0x56, 0x410F);   // SELF_CFG (Open wire) 0x470F: enabled, 0x430F: disable on Pwr on, 0x410F: full disable
  mp27.writeAdd(0x58, 0x016D);   // REGIN_UV
  mp27.writeAdd(0x59, 0x00F0);   // V3P3_UV
  mp27.writeAdd(0x5A, 0x0084);   // VDD_UV
  mp27.writeAdd(0x5B, 0x6555);   // SELF_THR
  mp27.writeAdd(0x60, 0x1000);   // FT_REC
  mp27.writeAdd(0x61, 0x3DD0);   // FT0_CFG
  mp27.writeAdd(0x62, 0x1000);   // FT1_CFG
  mp27.writeAdd(0x99, 0x0000);   // ADC_CTRL
  mp27.writeAdd(0x9A, 0x5F0A);   // CC_CFG
  mp27.writeAdd(0x9B, 0x4000);   // TRIMG_IPCB
  mp27.writeAdd(0x9C, 0x0373);   // HR_SCANO
  mp27.writeAdd(0x9D, 0x003F);   // HR_SCAN1
  mp27.writeAdd(0x9E, 0x0001);   // HR_SCAN2
  mp27.writeAdd(0xA0, 0x0000);   // SILC_INFO1
  mp27.writeAdd(0xA3, 0x0100);   // COMM_CFG
  mp27.writeAdd(0xA5, 0x0000);   // BAL_LIST
  mp27.writeAdd(0xA6, 0x0000);   // BAL_CTRL
  mp27.writeAdd(0xA7, 0x00F8);   // BAL_CFG
  mp27.writeAdd(0xA8, 0x0A21);   // BAL_THR

}
void checkSensors() {

  //Clear dead cell
  // mp27.writeAdd(MP2790_Reg::CELL_DEAD_DET_CLEAR, 1);

  // Read and print some sensor values
  // Temperature 
  uint16_t dieTemp = mp27.getDieTemperature();
  Serial.print("Die Temperature: ");
  Serial.println(dieTemp);
  
  // Temperature HR
  // uint16_t dieTempHR = mp27.getHRDieTemperature();
  // Serial.print("Die Temperature HR: ");
  // Serial.println(dieTempHR);

  // ADC Readings
  uint16_t adcValues[4];
  mp27.getADCReadings(adcValues);
  Serial.print("ADC Readings: 1v8, 3v3, regin, Vself: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(adcValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  // mp27.initHR();

  // ADC Readings HR
  uint16_t adcHRValues[3];
  mp27.getHRADCReadings(adcHRValues);
    Serial.print("ADC HR Readings: 1v8, 3v3, regin: ");
  for (int i = 0; i < 3; i++) {
    Serial.print(adcHRValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Vtop Readings
  Serial.print("VTop Readings: "); 
  Serial.println(mp27.getVTopReadings());

  // Power state
  Serial.print("Power status: "); 
  pwrStatus = mp27.powerStatus();
  Serial.println(pwrStatus, BIN);
}
void checkInt(){

  bool intFlags[32];

  // Interrupts
  Serial.println("Checking Interrupts...");
  if (mp27.getInt(intFlags)){
    Serial.println("Interrupts active");
    handleInt(intFlags);
    return;
  }
  Serial.println("No Interrupts...");
}
void handleInt(bool *intFlags) {
  Serial.println("Handling Interrupts...");
  for (int i = 0; i < 32; i++) {
    if (intFlags[i]){ // Skip if no interrupt flag is set
      Serial.print("Interrupt ");
      Serial.println(i);
      switch (i) {
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
        case 5: break;
        case 6: break;
        case 7: break;
        case 8: break;
        case 9:{
          Serial.println("Fault recovered.");
          mp27.clearInt(i);
          break;
        }
        case 10:{
          Serial.println("Mode change detected.");
          mp27.powerStatus();
          mp27.clearInt(i);
        break;
        }        
        case 11: break;
        case 12: break;
        case 13: break;
        case 14:{
          Serial.println("Current change detected.");
          mp27.currentDirection();
          mp27.clearInt(i);
          break;
        }
        case 15: break;
        case 16:{          
          Serial.println("CRC done.");
          mp27.clearInt(i);
          break;
        }
        case 17: break;
        case 18: break;
        case 19: {
          Serial.println("Vtop OV.");
          mp27.clearInt(i);
          break;
        }
        case 20:{
          int openWires = mp27.readAdd(RD_OPENH);  
          for (int j = 0; j < 11; j++) {
            if ((openWires >> j) & 0x01){ 
              Serial.print("Cell "); Serial.print(j); Serial.println(" Open.");
            }
          }
          break;
        }
        case 21:{         
          int deadCell = mp27.readAdd(RD_CELL_DEAD);  
          for (int k = 0; k < mp2790_nCells; k++) {
            if ((deadCell >> k) & 0x01){
              Serial.print("Cell "); Serial.print(k); Serial.println(" Dead.");
            }
          }
        break;}
        case 22: break;
        case 23: break;
        case 24: break;
        case 25:{
          Serial.println("Scheduler busy.");
          mp27.clearInt(i);
          break;
        }
        case 26: break;
        case 27: break;
        default:{ 
          Serial.println("Interrupt out of boundaries");
        break;}
      }
    }
  }
}

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
  if (millis() - lastChangeMs < 1000){
    return;
  }
  lastChangeMs = millis();
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
