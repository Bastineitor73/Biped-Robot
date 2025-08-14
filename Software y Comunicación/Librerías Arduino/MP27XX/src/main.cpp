/*
 * I2C MP27xx Read Write
 *
 * Reads the value of each byte of the I2C MP27xx and prints it
 * to the computer.
 * This example code is in the public domain.
 */

#include <Arduino.h>
#include "MP2790.h"

//Global Variables
#define RED_BUTTON GPIO_NUM_14
uint16_t mp2790_nCells = 0x06;  //6 cells
uint16_t mp2790_address = 0x01; // Default address, can be changed later
bool mpStatus = false;

//Function declarations
void setupMP2790();
void setupValues();
void checkSensors();

// Initialize using MP27XX(i2c_address, number of cells).  
MP2790 mp27 = MP2790(mp2790_address, mp2790_nCells); //empty for default

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

  //Check MP2790 connection
  mpStatus = mp27.testConnection();

  //Initialize GPIO
  pinMode(RED_BUTTON, INPUT_PULLUP);
}


void loop() {

  //Setup sequence when button is pressed
  if (digitalRead(RED_BUTTON) == LOW) {
    Serial.println("Button pressed, initializing MP2790 Setup...");
    setupMP2790();
    setupValues();
    checkSensors();
  }

}

void setupMP2790() {

  // Check Address
  // uint16_t addressi2c = mp27.getAddress(); //reads address from object
  uint16_t addressi2c = mp27.readReg(MP2790_Reg::DEVICE_ADD); //reads address using register
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
  mp27.writeAdd(CELLS_CTRL, 0x00C3);
  mp27.writeAdd(ACT_CFG, 0x0000);
  mp27.writeAdd(STB_CFG, 0x0022);
  mp27.writeAdd(SAFE_CFG, 0x000D);
  mp27.writeAdd(RGL_CFG, 0x0005);
  mp27.writeAdd(LOAD_CHARGER_CFG, 0x0500);
  mp27.writeAdd(GPIO_OUT, 0x0000);
  mp27.writeAdd(GPIO_CFG, 0x0444);
  mp27.writeAdd(PINS_CFG, 0x0061);
  mp27.writeAdd(WDT_CFG_X, 0x4E9D);
  mp27.writeAdd(FET_CTRL, 0x0000);
  mp27.writeAdd(FET_MODE, 0x051B);
  mp27.writeAdd(FET_CFG_X, 0x68F0);
  mp27.writeAdd(INT0_EN, 0xC7FF);
  mp27.writeAdd(INT1_EN, 0x2FFE);
  mp27.writeAdd(INT_TYPE0, 0x0000);
  mp27.writeAdd(INT_TYPE1, 0x0000);
  mp27.writeAdd(INT_TYPE2, 0x0000);
  mp27.writeAdd(MASK_INT0, 0x00FF);
  mp27.writeAdd(MASK_INT1, 0x0140);
  mp27.writeAdd(OCFT_CTRL, 0x01BF);
  mp27.writeAdd(DSGOC_LIM, 0x2C19);
  mp27.writeAdd(DSGOC_DEG, 0x0428);
  mp27.writeAdd(CHGOC_DEG, 0x0404);
  mp27.writeAdd(SCFT_CTRL, 0x003F);
  mp27.writeAdd(DSGSC_CFG, 0x0115);
  mp27.writeAdd(CHGSC_CFG, 0x0804);
  mp27.writeAdd(PACKFT_CTRL, 0x1B33);
  mp27.writeAdd(CELLFT_CTRL, 0x6036);
  mp27.writeAdd(CELL_HYST, 0x0AA0);
  mp27.writeAdd(PACK_UV_OV, 0x8080);
  mp27.writeAdd(CELL_UV_X, 0x009F);
  mp27.writeAdd(CELL_OV_X, 0x00D7);
  mp27.writeAdd(PACK_UV, 0x0276);
  mp27.writeAdd(PACK_OV, 0x0355);
  mp27.writeAdd(CELL_DEAD_THR, 0x0068);
  mp27.writeAdd(CELL_MSMT, 0x0002);
  mp27.writeAdd(NTC_CLR, 0x0000);
  mp27.writeAdd(DIE_CFG, 0x000A);
  mp27.writeAdd(NTC_CFG, 0xE4F5);
  mp27.writeAdd(NTCC_OTHR_DSG, 0x012E);
  mp27.writeAdd(NTCC_UTHR_DSG, 0x0294);
  mp27.writeAdd(NTCC_OTHR_CHG, 0x012E);
  mp27.writeAdd(NTCC_UTHR_CHG, 0x8A94);
  mp27.writeAdd(NTCM_OTHR, 0x80EB);
  mp27.writeAdd(DIE_OT, 0xAAEC);
  mp27.writeAdd(SELF_STS, 0x0000);
  mp27.writeAdd(SFT_GO, 0x0501);
  mp27.writeAdd(SELF_CFG, 0x470F);
  mp27.writeAdd(OPEN_CFG, 0x0407);
  mp27.writeAdd(REGIN_UV, 0x016D);
  mp27.writeAdd(V3P3_UV, 0x00F0);
  mp27.writeAdd(VDD_UV, 0x0084);
  mp27.writeAdd(SELF_THR, 0x6555);
  mp27.writeAdd(FT_REC, 0x1000);
  mp27.writeAdd(FT0_CFG, 0x3DD0);
  mp27.writeAdd(FT1_CFG, 0x1000);
  mp27.writeAdd(ADC_CTRL, 0x0000);
  mp27.writeAdd(CC_CFG, 0x5F0A);
  mp27.writeAdd(TRIMG_IPCB, 0x4000);
  mp27.writeAdd(HR_SCAN0, 0x0373);
  mp27.writeAdd(HR_SCAN1, 0x003C);
  mp27.writeAdd(HR_SCAN2, 0x01E0);
  mp27.writeAdd(SILC_INFO1, 0x0000);
  mp27.writeAdd(COMM_CFG, 0x0100);
  mp27.writeAdd(BAL_LIST, 0x0000);
  mp27.writeAdd(BAL_CTRL, 0x0000);
  mp27.writeAdd(BAL_CFG, 0x00F8);
  mp27.writeAdd(BAL_THR, 0x0A21);

}

void checkSensors() {
  // Read and print some sensor values
  // Temperature 
  uint16_t dieTemp = mp27.getDieTemperature();
  Serial.print("Die Temperature: ");
  Serial.println(dieTemp);
  
  // Temperature HR
  uint16_t dieTempHR = mp27.getHRDieTemperature();
  Serial.print("Die Temperature HR: ");
  Serial.println(dieTempHR);

  // ADC Readings
  uint16_t adcValues[4];
  mp27.getADCReadings(adcValues);
  Serial.print("ADC Readings: 1v8, 3v3, regin, Vself: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(adcValues[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Vtop Readings
  Serial.print("VTop Readings: "); 
  Serial.println(mp27.getVTopReadings());
  

}