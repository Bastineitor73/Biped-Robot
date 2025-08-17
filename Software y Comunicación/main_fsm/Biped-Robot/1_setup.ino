void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(38400);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
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








}