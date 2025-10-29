#include <Arduino.h>
#include <ODriveTeensyCAN.h>
#include <FlexCAN_T4.h>        // <–– esto debe estar
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <pitches.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET    -1          // Reset por software (GPIO no usado)
#define LOGO_WIDTH  84
#define LOGO_HEIGHT 52
#define BUZZER_PIN 6
#define PIN_BOTON 38
#define PRESSED LOW
#define NOT_PRESSED HIGH
ODriveTeensyCAN odriveCAN(250000);  // Configuración CAN
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//----------------CONFIGURACIÓN BUZZER-------------------------------
int melodynokia[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
};
int durations[] = {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
};
const int melody[] = { 262, 294, 330, 349, 330, 294, 262 };   // C4–D4–E4–F4–E4–D4–C4
const int noteDurations[] = { 200, 200, 200, 200, 200, 200, 400 };
const int melodyLength = sizeof(melody) / sizeof(melody[0]);

const int melodyHigh[] = { 392, 440, 494, 523, 494, 440, 392 };
const int noteDurationsHigh[] = { 150, 150, 150, 200, 150, 150, 300 };
const int melodyHighLength = sizeof(melodyHigh) / sizeof(melodyHigh[0]);

//------ Marcha imperial

int ImperialMarch [] = {
  
  // Dart Vader theme (Imperial March) - Star wars 
  // Score available at https://musescore.com/user/202909/scores/1141521
  // The tenor saxophone part was used
  
  // NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
  // NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
  NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16,

  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,//4
  NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_F5,-8, NOTE_C5,16,
  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
  
  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,

  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
  NOTE_C5,4, NOTE_A4,-8, NOTE_C5,16, NOTE_E5,2,

  // NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
  // NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,

  // NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
  // NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
  
};

int tempo = 97;
int wholenote = (60000 * 4) / tempo;
int divider = 0;
int noteDuration = 0;
int notes = sizeof(ImperialMarch) / sizeof(ImperialMarch[0]) / 2;

//-----------CONFIGURACIÓN DISPLAY-----------------------
struct frameOLA{
  char fecha[11];
  char hora[11];
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float magX, magY, magZ;
  float temp;
  float freq;
} data;
constexpr int Buff_len = 128;
char Buff[Buff_len];

const unsigned char PROGMEM logo[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x05, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x6c, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xdf, 0x00, 0x00, 0x00, 0x03, 0xce, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xde, 0x1c, 0x00, 0x00, 0x0f, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x9f, 0x10, 0x0f, 0xc7, 0xde, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x9e, 0x1f, 0x3f, 0x97, 0xde, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xbc, 0x9e, 0x3f, 0xb3, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xb9, 0xde, 0xbf, 0x3b, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb3, 0xdd, 0x9f, 0x79, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xa7, 0xd9, 0xdf, 0x7d, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xdb, 0xde, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xd3, 0xde, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xec, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


//-------------------BOTONES E IMU-----------------
bool modoIMU = false;               // false: muestra imagen, true: muestra datos
bool currentState;
const unsigned long shortPress = 30;
const unsigned long  longPress = 1500;
const int debounce = 30;
unsigned long counter=0;
bool prevState = NOT_PRESSED;

bool flagShort = false;
bool flagLong  = false;

//-------------CONFIGURACIÓN MOTORES------------------------

// La estructura 'MotorConfig' define las propiedades de cada motor.
struct MotorConfig {          //Estructura de Configuración
  uint8_t node_id;            // Node ID en el bus CAN
  float pos_a;                // Posición 1 (vueltas)
  float pos_b;                // Posición 2 (vueltas)
  float current_target;       // Último objetivo enviado
  unsigned long lastMoveTime; // Último cambio
  unsigned long intervalo_ms; // Intervalo entre movimientos
  float last_position_deg;    // Última posición en grados
  float last_current_A;       // Última corriente
};

// ---------------NOO CAAMBIAAR!!--------------

MotorConfig motores[] = {
  {0, 0.0, -2.5, 0.0, 0, 4000, 0.0, 0.0},      // Motor con Node ID 1
  {1, 0.0, 2.5, 0.0, 0, 4000, 0.0, 0.0},        // Motor con Node ID 0
  {2, 0.0, -0.5, 0.0, 0, 4000, 0.0, 0.0},       // Motor con Node ID 2
  {3, 0.0, 0.5, 0.0, 0, 4000, 0.0, 0.0},         // Motor con Node ID 3
  {4, 0.0, -0.3, 0, 0, 5000, 0,0},
  {5, 0.0, 0.3, 0, 0, 5000, 0,0}     
};

const int num_motores = sizeof(motores) / sizeof(MotorConfig);  //Se calcula automáticamente para saber cuántos motores hay.
const int espera = 700;

// ------------VARIABLES DE CONTROL DE ESTADOS----------------
enum Estado { MENU, EJECUTANDO, DETENIENDO };  // Se define un 'enum' para los estados del programa (código más legible)
Estado estado = MENU; // Estado inicial del programa.
char rutina_seleccionada = 0;// Almacena la opción de rutina ('1', '2').
bool detener_rutina = false;// Bandera para indicar si se debe detener la rutina actual.

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);         // UART2 (pines 7-8)
  while (!Serial);
  while (!Serial2);
  Wire1.begin();                  // inicia bus I²C en pins 17/16
  Wire1.setClock(400000);         // 400 kHz (rápido y estable)

  pinMode(PIN_BOTON, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // dirección 0x3C
    while (true) {
      // Si falla, parpadea el LED integrado
      digitalToggle(LED_BUILTIN);
      delay(200);
    }
  }

  //Sonrisa inicial
  display.clearDisplay();
  display.drawBitmap(0,0,logo,128,64,WHITE);
  display.display();


  bool ejesOk[num_motores] = {false};
  bool errorMostrado[num_motores] = {false};

  //------ LÓGICA DE INICIALIZACIÓN Y CHEQUEO DE MOTORES ----------
  while (true) {                  // Bucle principal para asegurar que todos los motores estén en el estado correcto.
    int motoresPendientes = 0;
    for (int i = 0; i < num_motores; i++) {
      if (ejesOk[i]) continue;      // Si este motor ya está OK, salta al siguiente.
      
      odriveCAN.RunState(motores[i].node_id, 8);// Envía un comando CAN (id=8) para poner el motor modo closed-loop control

      uint32_t axisError = 0;
      uint8_t estadoActual = 0;
      bool ok = false;
      unsigned long start = millis();
      while (millis() - start < 1200) {
        CAN_message_t msg;
        // Procesa los mensajes CAN entrantes.
        while (odriveCAN.ReadMsg(msg)) {
          uint8_t node_id = (msg.id >> 5);
          if (node_id == motores[i].node_id &&
              (msg.id & 0x1F) == ODriveTeensyCAN::CMD_ID_ODRIVE_HEARTBEAT_MESSAGE) {
            HeartbeatMsg_t hb;
            odriveCAN.Heartbeat(hb, msg);
            axisError = hb.axisError;
            estadoActual = hb.currentState;
            if (axisError == 0 && estadoActual == 8) {
              ok = true;
              break;
            }
          }
        }
        if (ok) break;
        delay(10);
      }

      if (ok) {
        ejesOk[i] = true;
        Serial.print("Axis ");
        Serial.print(motores[i].node_id);
        Serial.println(" en modo cerrado OK.");
      } else {
        motoresPendientes++;
        if (!errorMostrado[i]) {
          Serial.print("ERROR: Axis "); Serial.print(motores[i].node_id);
          Serial.print(" NO entró en lazo cerrado (estado actual: ");
          Serial.print(estadoActual); Serial.print("). axisError: 0x");
          Serial.println(axisError, HEX);
          errorMostrado[i] = true;
        }
      }
      
    }
    imprimirHeartbeats();

    if (motoresPendientes == 0) break; // Todos OK, salimos del bucle
    delay(1000); // Espera antes de reintentar (silenciosamente)
  }

  ImperialMarchfun();

  Serial.println("Sistema iniciado.");
  Serial.println("Seleccione un modo:");
  Serial.println("[1] Axis Tests");
  Serial.println("[2] Walking On Sunshine");
  Serial.println("[3] Ejecutar extension");
  Serial.println("[9] Para cambiar modo IMU");
  Serial.println("Presione brevemente el botón para Axis_Test");
  Serial.println("Presione por 2 segundos el botón para Walking On Sunshine");
  Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
}


void loop() {
  //leerDatosCAN();
  leerErroresCAN();

  size_t n = Serial2.readBytesUntil('\n', Buff, Buff_len - 1);
  if (n > 0) {
    Buff[n] = '\0';
  } else {
    Buff[0] = '\0';
  }
  // 2) Parseo de los datos recién leídos
  parsearIMU();
  if (modoIMU) {
    visualizarIMU(); 
  }
  millisBoton();
  // ========================= MENU =========================
  
  static char c = 0;   // guarda el último dígito tecleado
  if (estado == MENU) {
    while (Serial.available() > 0) {
      char Car = Serial.read();
      Serial.write(Car);
      if (Car >= '0' && Car <= '9') {
      // Guarda el último dígito que escribiste
      c = Car;
      }
      if (Car == '\n'){        
        if (c == '1') {
          //playLowMelody(); 
          rutina_seleccionada = '1';
          estado = EJECUTANDO;
        } else if (c == '2') {
          //playHighMelody();
          rutina_seleccionada = '2';
          estado = EJECUTANDO;
        } else if (c == '3'){
          rutina_seleccionada = '3';
          estado = EJECUTANDO;
        } else if (c == '9') {
          // Alterna la bandera
          modoIMU = !modoIMU;


          // Si volvemos a carita, la dibujamos de una vez
          if (modoIMU) {
            visualizarIMU();
            Serial.println(">> Mostrando senseo IMU");
            Serial.println(" ");
            Serial.println("Seleccione un modo:");
            Serial.println("[1] Axis Tests");
            Serial.println("[2] Walking On Sunshine");
            Serial.println("[3] Ejecutar extension");
            Serial.println("[9] Para cambiar modo IMU");
            Serial.println("Presione brevemente el botón para Axis_Test");
            Serial.println("Presione por 2 segundos el botón para Walking On Sunshine");
            Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
    
          }
          else {
            mostrarSonrisa();
            Serial.println(" ");
            Serial.println("Seleccione un modo:");
            Serial.println("[1] Axis Tests");
            Serial.println("[2] Walking On Sunshine");
            Serial.println("[3] Ejecutar extension");
            Serial.println("[9] Para cambiar modo IMU");
            Serial.println("Presione brevemente el botón para Axis_Test");
            Serial.println("Presione por 2 segundos el botón para Walking On Sunshine");
            Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
 
          }
        }
        c = 0;
      }              
     }   
    }

  else if (estado == EJECUTANDO) {
    detener_rutina = false;
    if (rutina_seleccionada == '1') {
      playMelody(); 
      Rutina_A();
    } 
    else if (rutina_seleccionada == '2') {
      playHighMelody();
      Rutina_B();
    }
    else if (rutina_seleccionada == '3') {
      playNokia();
      Rutina_C();
    }
    //millisBoton();
    if (flagLong) {
      PushLong();   
      flagLong = false; 
    }

    if (flagShort) {
      PushShort();        
      flagShort = false;
    }

    // Al terminar, volver a posiciones originales
    Serial.println(">> Volviendo a posición original...");
    delay(100);
    moverMotoresAPosicionInicial();
    estado = MENU;
    rutina_seleccionada = 0;
    R2D2_Beep();
    Serial.println(" ");
    Serial.println("Seleccione un modo:");
    Serial.println("[1] Axis Tests");
    Serial.println("[2] Walking On Sunshine");
    Serial.println("[3] Ejecutar extension");
    Serial.println("[9] Para cambiar modo IMU");
    Serial.println("Presione brevemente el botón para Axis_Test");
    Serial.println("Presione por 2 segundos el botón para Walking On Sunshine");
    Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
    }
    
  delay(10);
}

// Rutina A: Pero chequea si hay stop
void Rutina_A() {
  Serial.println(">> Ejecutando Axis Tests...");
  for (int m = 0; m < num_motores; m++) {
    leerErroresCAN();
    if (checkearStop()) return;
    odriveCAN.SetPosition(motores[m].node_id, motores[m].pos_a);
    esperarConInterrupcion(espera);
    if (checkearStop()) return;
    odriveCAN.SetPosition(motores[m].node_id, motores[m].pos_b);
    esperarConInterrupcion(espera);
    if (checkearStop()) return;
    odriveCAN.SetPosition(motores[m].node_id, motores[m].pos_a);
    esperarConInterrupcion(espera);
    
  }
  //Serial.println(">> Modo completado.");
}

// Rutina B: Simular caminata
void Rutina_B() {
  Serial.println(">> Ejecutando Walking On Sunshine...");
  for (int contador = 0; contador < 15; contador++){
    //Mover Pierna Izquierda
//Mover Pierna Izquierda
    odriveCAN.SetPosition(motores[0].node_id, motores[0].pos_b);  // Posición objetivo 0 extensión pantorilla
    esperarConInterrupcion(100);
    odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_b);  // Posición objetivo 2 extensión rodilla
    esperarConInterrupcion(900);
    odriveCAN.SetPosition(motores[0].node_id, motores[0].pos_a);  // Regreso a inicial      -1
    esperarConInterrupcion(20);
    odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_a);  // Regreso a inicial

    esperarConInterrupcion(10);

    //Mover Pierna Derecha
    odriveCAN.SetPosition(motores[1].node_id, motores[1].pos_b);  // Posición objetivo 1 extensión pantorrilla
    esperarConInterrupcion(100);
    odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_b);  // Posición objetivo 3 extensión rodilla
    esperarConInterrupcion(900);
    odriveCAN.SetPosition(motores[1].node_id, motores[1].pos_a);  // Regreso a inicial      1
    esperarConInterrupcion(20);
    odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_a);  // Regreso a inicial
  }
  esperarConInterrupcion(1000);
  //Serial.println(">> Modo completado");
}
void Rutina_C() {
  Serial.println(">> Ejecutando última rutina...");
  odriveCAN.SetPosition(motores[0].node_id, -3.5);  // der
  esperarConInterrupcion(800);
  odriveCAN.SetPosition(motores[1].node_id, 3.5);  // der
  esperarConInterrupcion(800);
  //odriveCAN.SetPosition(motores[4].node_id, motores[4].pos_b);  // Posición objetivo 1 extensión pantorrilla
  //esperarConInterrupcion(2000);
  //odriveCAN.SetPosition(motores[5].node_id, motores[5].pos_b);  // Posición objetivo 1 extensión pantorrilla
  //esperarConInterrupcion(2000);

float pos_b_forward3= -0.9;    //Hacia atrás 0.5
float pos_b_forward2= 0.9;     //Hacia atrás -0.5

for (int contador = 0; contador < 8; contador++){
  odriveCAN.SetPosition(motores[3].node_id,pos_b_forward3);  // Posición objetivo 3 extensión rodilla
  esperarConInterrupcion(600);
  odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_a);  // Posición objetivo 1 extensión pantorrilla
  esperarConInterrupcion(500);
  odriveCAN.SetPosition(motores[2].node_id,pos_b_forward2);  // Regreso a inicial      1
  esperarConInterrupcion(600);
  odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_a);  // Posición objetivo 3 extensión rodilla
  esperarConInterrupcion(500);
  }
  esperarConInterrupcion(1000);;
}

void esperarConInterrupcion(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (Serial.available() > 0 && Serial.read() == 's') {
      Serial.println("Rutina detenida por el usuario.");
      detener_rutina = true;
      break;
    }
    delay(10);  // Evita saturar el micro
  }
}

bool checkearStop() {
  if (detener_rutina) {
    Serial.println("Deteniendo rutina, saliendo...");
    return true;
  }
  return false;
}

// Mueve todos los motores a pos_a (posición original)
void moverMotoresAPosicionInicial() {
  const int repeticiones = 5;
  const int intervalo = 20;  // ms entre cada repetición (debe ser menor que el watchdog timeout)

  for (int r = 0; r < repeticiones; r++) {
    for (int i = 0; i < num_motores; i++) {
      odriveCAN.SetPosition(motores[i].node_id, motores[i].pos_a);
    }
    delay(intervalo);
  }
}


void leerErroresCAN() {
  CAN_message_t msg;
  while (odriveCAN.ReadMsg(msg)) {
    uint8_t node_id = (msg.id >> 5);
    if ((msg.id & 0x1F) == ODriveTeensyCAN::CMD_ID_ODRIVE_HEARTBEAT_MESSAGE) {
      HeartbeatMsg_t hb;
      odriveCAN.Heartbeat(hb, msg);
      if (hb.axisError != 0) {
        Serial.print("[ERROR] Axis "); Serial.print(node_id);
        Serial.print(": axisError = 0x"); Serial.println(hb.axisError, HEX);
        odriveCAN.RunState(node_id, 1); // Pone el eje en IDLE
      }
      if (hb.currentState != 8) { // Aquí va currentState, no axisState
        Serial.print("[ADVERTENCIA] Axis "); Serial.print(node_id);
        Serial.print(" fuera de Closed Loop. Estado: "); Serial.println(hb.currentState);
      }
    }
    // Agrega chequeos de otros mensajes aquí si lo necesitas
  }
}

void imprimirHeartbeats() {
  Serial.println("===== Heartbeats ODrive =====");
  for (int i = 0; i < num_motores; i++) {
    uint8_t node_id = motores[i].node_id;
    bool encontrado = false;
    unsigned long t0 = millis();
    while (millis() - t0 < 300) {
      CAN_message_t msg;
      if (odriveCAN.ReadMsg(msg)) {
        uint8_t id_msg = (msg.id >> 5);
        uint8_t cmd = (msg.id & 0x1F);
        if (id_msg == node_id && cmd == ODriveTeensyCAN::CMD_ID_ODRIVE_HEARTBEAT_MESSAGE) {
          // Acceso manual según documentación CANSimple
          uint32_t axisError;
          memcpy(&axisError, &msg.buf[0], 4);    // Bytes 0-3: axisError
          uint8_t axisState         = msg.buf[4]; // Byte 4: axisState
          uint8_t procedureResult   = msg.buf[5]; // Byte 5: procedureResult
          uint8_t trajectoryDone    = msg.buf[6]; // Byte 6: trajectoryDone

          Serial.print("Heartbeat Axis ");
          Serial.print(node_id);
          Serial.print(": axisError=0x"); Serial.print(axisError, HEX);
          Serial.print(", state="); Serial.print(axisState);
          Serial.print(", procedureResult="); Serial.print(procedureResult);
          Serial.print(", trajectoryDone="); Serial.println(trajectoryDone);
          encontrado = true;
          break;
        }
      }
    }
    if (!encontrado) {
      Serial.print("Heartbeat Axis ");
      Serial.print(node_id);
      Serial.println(": [NO RECIBIDO]");
    }
  }
  Serial.println("============================");
}
void parsearIMU() {
  // Lista de punteros a los campos float dentro de 'data'
  float *variables[] = {
    &data.accX, &data.accY, &data.accZ,
    &data.gyroX, &data.gyroY, &data.gyroZ,
    &data.magX,  &data.magY,  &data.magZ,
    &data.temp,  &data.freq
  };

  // Primer token: fecha
  char *tok = strtok(Buff, ",");
  if (tok) {
    strlcpy(data.fecha, tok, sizeof(data.fecha));
  }

  // Segundo token: hora
  tok = strtok(NULL, ",");
  if (tok) {
    strlcpy(data.hora, tok, sizeof(data.hora));
  }

  // Los siguientes 11 tokens son los valores float
  for (int i = 0; i < 11; i++) {
    tok = strtok(NULL, ",");
    if (tok) {
      *variables[i] = atof(tok);   
    }
  }
}

void visualizarIMU() {
  display.clearDisplay();

  // Cabecera
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(data.fecha);
  display.print(' ');
  display.println(data.hora);

  // Divisiones
  display.drawRect(0, 10, SCREEN_WIDTH, SCREEN_HEIGHT - 10, WHITE);
  display.drawLine(0, 10 + 18, SCREEN_WIDTH, 10 + 18, WHITE);
  display.drawLine(0, 10 + 36, SCREEN_WIDTH, 10 + 36, WHITE);

  // Valores IMU
  display.setCursor(3, 15);
  display.print(data.accX, 2); display.print(' ');
  display.print(data.accY, 2); display.print(' ');
  display.print(data.accZ, 2);

  display.setCursor(3, 32);
  display.print(data.gyroX, 2); display.print(' ');
  display.print(data.gyroY, 2); display.print(' ');
  display.print(data.gyroZ, 2);

  display.setCursor(3, 50);
  display.print(data.magX, 2); display.print(' ');
  display.print(data.magY, 2); display.print(' ');
  display.print(data.magZ, 2);

  display.display();

  tone(BUZZER_PIN, 1000);  // Tocar tono de 1000 Hz
  delay(100);              // Esperar 0.5 segundos
  noTone(BUZZER_PIN);      // Detener sonido
  delay(100);              // Esperar otros 0.5 segundos

}


void mostrarSonrisa(){
  display.clearDisplay();
  display.drawBitmap(0,0,logo,128,64,WHITE);
  display.display();
}

void playInitialMelody(){
    tone(BUZZER_PIN, 262, 200);  // DO
    delay(220);
    tone(BUZZER_PIN, 330, 200);  // MI
    delay(220);
    tone(BUZZER_PIN, 392, 200);  // SOL
    delay(220);
    tone(BUZZER_PIN, 523, 350);  // DO alto
    delay(400);
    noTone(BUZZER_PIN);
}

void playMelody() {
  for (int i = 0; i < melodyLength; i++) {
    tone(BUZZER_PIN, melody[i], noteDurations[i]);
    delay(noteDurations[i] + 50);  // un pequeño espacio entre notas
    noTone(BUZZER_PIN);
  }
}
void playHighMelody() {
  for (int i = 0; i < melodyHighLength; i++) {
    tone(BUZZER_PIN, melodyHigh[i], noteDurationsHigh[i]);
    delay(noteDurationsHigh[i] + 20);  // breve pausa entre notas
    noTone(BUZZER_PIN);
  }
}
void playNokia(){
    int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melodynokia[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    
    //stop the tone playing:
    noTone(BUZZER_PIN);
}
}
void R2D2_Beep() {
for (int freq = 1500; freq <= 3600; freq += 40) {
    tone(BUZZER_PIN, freq, 15);
    delay(8);
  }

  // pequeña pausa
  delay(90);

  // --- Segundo barrido ascendente más agudo (weee!)
  for (int freq = 2200; freq <= 4200; freq += 60) {
    tone(BUZZER_PIN, freq, 10);
    delay(8);
  }

  // breve pausa entre silbido y caída
  delay(110);

  // --- Barrido descendente rápido (eep!)
  for (int freq = 4200; freq >= 1800; freq -= 80) {
    tone(BUZZER_PIN, freq, 10);
    delay(10);
  }

  noTone(BUZZER_PIN);
}


void ImperialMarchfun(){
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = ImperialMarch[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(BUZZER_PIN, ImperialMarch[thisNote], noteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);
    
    // stop the waveform generation before the next note.
    noTone(BUZZER_PIN);
  }
}
void millisBoton(){
  currentState = digitalRead(PIN_BOTON);

  if (currentState != prevState) {
    delay(debounce);
      // update status in case of bounce
    currentState = digitalRead(PIN_BOTON);
    if (currentState == PRESSED) {
        //Serial.println("Press");

         // a new press event occured
         // record when button went down
        counter = millis();
  }
  if (currentState == NOT_PRESSED) {
         // but no longer pressed, how long was it down?
        //Serial.println("Not press");
        
          unsigned long currentMillis = millis();
          Serial.print("Tiempo presionado (ms): ");
          Serial.println(currentMillis - counter);  

          //if ((currentMillis - button.counter >= shortPress) && !(currentMillis - button.counter >= longPress)) {
          if ((currentMillis - counter >= shortPress) && !(currentMillis - counter >= longPress)) {
          // short press detected.
          estado = EJECUTANDO;
          flagShort = true;
      }
          else if ((currentMillis - counter >= longPress)) {
          // the long press was detected
          estado = EJECUTANDO;
          flagLong = true;
      }
   }
  }
   // used to detect when state changes
   prevState = currentState;
}
void PushShort(){
  Serial.println(">> Botón corto presionado. ");
  playMelody();
  Rutina_A();
  Serial.println("Seleccione un modo:");
  Serial.println("[1] Axis Tests");
  Serial.println("[2] Walking On Sunshine");
  Serial.println("[9] Para cambiar modo IMU");
  Serial.println("Presione brevemente el botón para Axis_Test");
  Serial.println("Presione por 3 segundos el botón para Walking On Sunshine");
  Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
    
}
void PushLong(){
  Serial.println(">> Botón largo presionado.");
  playHighMelody();
  Rutina_B();
  Serial.println("Seleccione un modo:");
  Serial.println("[1] Axis Tests");
  Serial.println("[2] Walking On Sunshine");
  Serial.println("[9] Para cambiar modo IMU");
  Serial.println("Presione brevemente el botón para Axis_Test");
  Serial.println("Presione por 3 segundos el botón para Walking On Sunshine");
  Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
    
}
