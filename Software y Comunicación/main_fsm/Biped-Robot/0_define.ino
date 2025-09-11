#include <Arduino.h>	// Necesario para otros compiladores
#include <Wire.h>			// Comunicaciones Serial, I2C, SDA
#include "MP2790.h"		// Medidor de bateria MP2790
#include <SD.h>       // Ranura SD integrada
#include <TimeLib.h>  // Teensyduino's time helpers

/* ---------------- Functions -----------------*/

void rtcNowISO(char*, size_t);
bool appendLog(const char*);
void digitalClockDisplay();
void printDigits(int);
void processSyncMessage();

// ------------------------- IO Config -------------------------
static const int BUTTON_PIN = 40;              // Active-low with pull-up
static const uint32_t DEBOUNCE_MS = 40;
bool lastButtonState = HIGH;
uint32_t lastChangeMs = 0;

static const char* LOG_PATH = "/log/log.txt";  // SD root file


/* ----------------------- Global Vars ---------------------- */
char userName[20] = null;

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

  //Interrupts
  bool intStatus = false;
  bool intFlags[32] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
                      0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

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






