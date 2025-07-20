#include <Arduino.h>
#include <ODriveTeensyCAN.h>
//#include <Keyboard.h>

ODriveTeensyCAN odriveCAN(250000);  // Configuración CAN

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

MotorConfig motores[] = {
  {0, 0.0, 3.5, 0.0, 0, 4000, 0.0, 0.0},        // Motor con Node ID 0
  {1, 0.0, -3, 0.0, 0, 4000, 0.0, 0.0},      // Motor con Node ID 1
  {2, 0.0, -0.8, 0.0, 0, 4000, 0.0, 0.0},       // Motor con Node ID 2
  {3, 0.0, 0.7, 0.0, 0, 4000, 0.0, 0.0},         // Motor con Node ID 3
  {4, 0.0, -0.15, 0, 0, 4000, 0,0},
  {5, 0.0, 0.2, 0, 0, 4000, 0,0}     
};

const int num_motores = sizeof(motores) / sizeof(MotorConfig);
const int espera = 900;

// Variables de control de estado
enum Estado { MENU, EJECUTANDO, DETENIENDO };
Estado estado = MENU;
char rutina_seleccionada = 0;
bool detener_rutina = false;



void setup() {
  Serial.begin(115200);
  while (!Serial);

  bool ejesOk[num_motores] = {false};
  bool errorMostrado[num_motores] = {false};

  while (true) {
    int motoresPendientes = 0;

    for (int i = 0; i < num_motores; i++) {
      if (ejesOk[i]) continue;

      odriveCAN.RunState(motores[i].node_id, 8); // Pedir closed-loop

      // Espera hasta 1200 ms para recibir heartbeat correcto
      uint32_t axisError = 0;
      uint8_t estadoActual = 0;
      bool ok = false;
      unsigned long start = millis();
      while (millis() - start < 1200) {
        CAN_message_t msg;
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
    delay(800); // Espera antes de reintentar (silenciosamente)
  }

  Serial.println("Sistema iniciado.");
  Serial.println("Seleccione un modo:");
  Serial.println("[1] Axis Tests");
  Serial.println("[2] Walking On Sunshine");
  Serial.println("[3] Modo Manual");
  Serial.println("Para detener una rutina en ejecución, presione [s] y Enter.");
}


void loop() {
  //leerDatosCAN();
  leerErroresCAN();
  if (estado == MENU) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '1') {
        rutina_seleccionada = '1';
        estado = EJECUTANDO;
      } else if (c == '2') {
        rutina_seleccionada = '2';
        estado = EJECUTANDO;
      } else if (c == '3'){
        rutina_seleccionada = '3';
        estado = EJECUTANDO;
      }
    }
  }
  else if (estado == EJECUTANDO) {
    detener_rutina = false;
    if (rutina_seleccionada == '1') {
      Rutina_A();
    } 
    else if (rutina_seleccionada == '2') {
      Rutina_B();
    }
    else if (rutina_seleccionada == '3') {
      manualControl();
    }

    // Al terminar, volver a posiciones originales
    Serial.println(">> Volviendo a posición original...");
    moverMotoresAPosicionInicial();
    estado = MENU;
    rutina_seleccionada = 0;
    Serial.println(" ");
    Serial.println("Seleccione un modo:");
    Serial.println("[1] Axis Tests");
    Serial.println("[2] Walking On Sunshine");
    Serial.println("[3] Modo Manual");
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
  for (int contador = 0; contador < 10; contador++){
    //Mover Pierna Izquierda
    odriveCAN.SetPosition(motores[0].node_id, motores[0].pos_b);  // Posición objetivo
    esperarConInterrupcion(100);
    odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_b);  // Posición objetivo
    esperarConInterrupcion(800);
    odriveCAN.SetPosition(motores[0].node_id, 1);  // Regreso a inicial
    esperarConInterrupcion(20);
    odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_a);  // Regreso a inicial

    esperarConInterrupcion(10);

    //Mover Pierna Derecha
    odriveCAN.SetPosition(motores[1].node_id, motores[1].pos_b);  // Posición objetivo
    esperarConInterrupcion(100);
    odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_b);  // Posición objetivo
    esperarConInterrupcion(800);
    odriveCAN.SetPosition(motores[1].node_id, -1);  // Regreso a inicial
    esperarConInterrupcion(20);
    odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_a);  // Regreso a inicial
  }
  esperarConInterrupcion(espera);
  //Serial.println(">> Modo completado");
}


//------------------EN REVISIÓN----------------
//------------------NO USAR--------------------
void manualControl() {
  Serial.println(">> Ejecutando Modo Manual...");
  Serial.println("Presione teclas (q,a,w,s,e,d,r,f) para mover motores. 'm' para salir.");
  
  // Configuración especial para lectura inmediata
  uint32_t lastKeyTime = 0;
  const uint16_t keyDebounceTime = 100; // ms
  
  while (estado == EJECUTANDO) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      
      // Limpiar buffer para evitar acumulación
      while(Serial.available() > 0) {
        Serial.read();
        delay(1);
      }
      
      // Solo procesar si ha pasado el tiempo de debounce
      if (millis() - lastKeyTime > keyDebounceTime) {
        lastKeyTime = millis();
        
        if (c == 'm') {
          Serial.println("Saliendo al menú...");
          estado = MENU;
          return;
        }

        switch (c) {
          case 'q':
            odriveCAN.SetPosition(motores[0].node_id, motores[0].pos_a + 0.05);
            Serial.println("Motor 0 +0.05");
            break;
          case 'a':
            odriveCAN.SetPosition(motores[0].node_id, motores[0].pos_a - 0.05);
            Serial.println("Motor 0 -0.05");
            break;
          case 'w':
            odriveCAN.SetPosition(motores[1].node_id, motores[1].pos_a - 0.05);
            Serial.println("Motor 1 -0.05");
            break;
          case 's':
            odriveCAN.SetPosition(motores[1].node_id, motores[1].pos_a + 0.05);
            Serial.println("Motor 1 +0.05");
            break;
          case 'e':
            odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_a - 0.01);
            Serial.println("Motor 2 -0.01");
            break;
          case 'd':
            odriveCAN.SetPosition(motores[2].node_id, motores[2].pos_a + 0.01);
            Serial.println("Motor 2 +0.01");
            break;
          case 'r':
            odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_a + 0.01);
            Serial.println("Motor 3 +0.01");
            break;
          case 'f':
            odriveCAN.SetPosition(motores[3].node_id, motores[3].pos_a - 0.01);
            Serial.println("Motor 3 -0.01");
            break;
          default:
            break;
        }
      }
    }
    delay(1); // Pequeña pausa para no saturar
  }
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
  for (int i = 0; i < num_motores; i++) {
    odriveCAN.SetPosition(motores[i].node_id, motores[i].pos_a);
    delay(100);  // espera a que todos lleguen, ajusta según necesidad
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

    // Pedir explicitamente el heartbeat a ese axis
    // (En ODrive CANSimple, basta con escuchar el heartbeat que se envía cíclicamente cada 100ms aprox)
    // Si quieres leer solo un mensaje por cada axis, puedes hacer esto:
    bool encontrado = false;
    unsigned long t0 = millis();
    while (millis() - t0 < 300) { // Esperar hasta 300 ms por mensaje
      CAN_message_t msg;
      if (odriveCAN.ReadMsg(msg)) {
        uint8_t id_msg = (msg.id >> 5);
        uint8_t cmd = (msg.id & 0x1F);
        if (id_msg == node_id && cmd == ODriveTeensyCAN::CMD_ID_ODRIVE_HEARTBEAT_MESSAGE) {
          HeartbeatMsg_t hb;
          odriveCAN.Heartbeat(hb, msg);
          Serial.print("Heartbeat Axis ");
          Serial.print(node_id);
          Serial.print(": axisError=0x"); Serial.print(hb.axisError, HEX);
          Serial.print(", state="); Serial.print(hb.currentState);
          Serial.print(", motorFlag="); Serial.print(hb.motorFlag);
          Serial.print(", encoderFlag="); Serial.println(hb.encoderFlag);
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


