void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial && millis() < 2000) { //wait for USB w/ init
  }

  //GPIO
  pinMode(BUTTON_PIN, INPUT_PULLUP); //Log

  // SD card
  Serial.print("SD card ");
  if (SD.begin(BUILTIN_SDCARD)) {
    Serial.println("initialized.");
  } else {
    Serial.println("init failed!");
  }

  //Initialize Wire/I2C
  Wire.setSDA(18);
  Wire.setSCL(19);
  Wire.begin();

  //MP2790

  // Initialize interrutps
  // bool intFlags[32];

  // Syncing RTC
  Serial.write(7);  //7: ASCII value for time request

  //Check MP2790 connection
  mpStatus = mp27.testConnection();

  //Data input
  serialInput();
  
  Serial.print("Routine start.");
}