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
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin(); 
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
  #endif

  //Initialize MP2790
  MP2790 mp27 = MP2790(mp2790_address); //empty for default
  bool mp2790Open = mp27.testConnection()
  if(mp2790Open!){
    robotMode = isAlert;
  }

  // Syncing RTC
  Serial.write(7);  //7: ASCII value for time request







}