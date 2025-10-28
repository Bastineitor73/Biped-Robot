#include <Arduino.h>	// Necesario para otros compiladores
#include <Wire.h>			// Comunicaciones Serial, I2C, SDA
#include "MP2790.h"		// Medidor de bateria MP2790
#include <SD.h>       // Ranura SD integrada
#include <TimeLib.h>  // Teensyduino's time helpers


// T1757676691
/* ---------------- Functions -----------------*/
// void xAlertInterrupt();
void rtcNowISO(char*, size_t);
bool appendLog(const char*);
void digitalClockDisplay();
void printDigits(int);
void processSyncMessage();
void serialInput();
//MP2790
void setupMP2790();
void setupValues();
void checkSensors();
void checkInt();
void handleInt(bool *intFlags);

// ------------------------- IO Config -------------------------
static const int BUTTON_PIN = 40;              // Active-low with pull-up
static const int XALERT_PIN = 10;
static const uint32_t DEBOUNCE_MS = 300;
bool lastButtonState = HIGH;
uint32_t lastChangeMs = 0;

static const char* LOG_PATH = "/log/log.txt";  // SD root file


/* ----------------------- Global Vars ---------------------- */
// char userName[20] = {0};
int userName = 0;

//Global Variables
#define BUTTON_DEBOUNCE 1000        //in milliseconds
bool mpStatus = false;
uint8_t buttonCounter = 0;
uint32_t lastMillis = 0;

int fetcounter = 0;






// ------------------------- Time -------------------------
#define TIME_HEADER "T"  // Header tag for serial time sync message

/* ---------------- FSM ---------------*/
enum class robotMode {
    isAlert,
    isError,
    isIdle,
    IsActive
};

/* ------------------ MP2790 ------------------*/
//Main values
const uint8_t mp2790_nCells = 6;
const uint8_t mp2790_address = 0x01;

  // Initialize using MP27XX(i2c_address, number of cells).  
  MP2790 mp27 = MP2790(mp2790_address, mp2790_nCells); //empty for default

//Interrupts
bool intStatus = false;
bool intFlags[32] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

//Quick interrupt toggle
// bool intEn0[16] =    {1,1,0,0, 0,1,1,1, 1,1,1,1, 1,1,1,1};
// bool intEn1[16] =    {0,0,1,0, 1,1,1,1, 1,1,1,0, 1,1,1,0};
// bool intMask0[16] =  {0,1,0,0, 0,0,0,0, 1,1,1,1, 1,1,1,1};
// bool intMask1[16] =  {0,0,0,0, 0,0,0,1, 0,1,0,0, 0,0,1,0};

//Alert and error flags
bool cellUVStatus = false;
bool cellOVStatus = false;
bool cellMismatchStatus = false;
bool cellDeadStatus = false;
bool cellOpenWireStatus = false;
bool cellUVFlags[mp2790_nCells] = {0};
bool cellOVFlags[mp2790_nCells] = {0};
bool cellMismatchFlags[mp2790_nCells] = {0};
bool cellDeadFlags[mp2790_nCells] = {0};
bool cellOpenWireFlags[mp2790_nCells] = {0};
uint16_t adcValues[4] = {0}; // Array to hold ADC readings
uint16_t ntcValues[4] = {0}; // Array to hold NTC readings

uint8_t pwrStatus;




