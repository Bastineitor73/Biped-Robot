#include <ODriveTeensyCAN.h>

/*En el Teensy 4.1, típicamente:
  CAN0 usa los pines:
  Pin 22 → CAN TX
  Pin 23 → CAN RX*/

// Configuración CAN
ODriveTeensyCAN odriveCAN(250000); // Baud rate estándar (250kbps)

// Variables para datos
float current = 0.0;
float position = 0.0;
uint8_t axis_id = 0; // Node ID del ODrive (ajustar en ODrive Tool)

void setup() {
  Serial.begin(115200);
  // Inicializar ODrive en modo Closed-Loop Control
  odriveCAN.RunState(axis_id, 8); // 8 = AXIS_STATE_CLOSED_LOOP_CONTROL
  delay(1000); // Esperar inicialización
}

void loop() {
  readMotorData();        // Leer corriente y posición (actualización cada 100ms)
  odriveCAN.SetPosition(axis_id, deg2pos_Normal(90)); // Mover a 90 grados
  delay(1000); 
  odriveCAN.SetPosition(axis_id, deg2pos_Normal(0)); // Mover a 0 grados
  delay(1000); 
  odriveCAN.SetPosition(axis_id, deg2pos_Normal(90)); // Mover a 90 grados
}

//Se ingresa el valor en grados y se devuelve la posición en "counts"
float deg2pos_Normal(float degrees) {
  float position = degrees / 360.0;
  return position;
}

// Función para convertir grados a posición con reductor 9:1
float deg2pos_Gearbox(float degrees) {
  float position = (degrees / 360.0) * 9.0;
  return position;
}

// Función para leer datos del motor
void readMotorData() {
  CAN_message_t msg;    //declara una estructura vacía para guardar el mensaje CAN
  
  // Solicitar corriente (CMD_ID_GET_IQ)
  odriveCAN.GetIq(axis_id);                 //Le solicita a Odrive obtener un dato por CAN
  if (odriveCAN.ReadMsg(msg)) {             //Si el Odrive ya respondió y existe un mensaje, entones es True y lo guarda en "msg"
    current = odriveCAN.GetIqResponse(msg);      //Se extrae el dato y se guarda en "current"
    Serial.print("Corriente: "); 
    Serial.print(current); 
    Serial.println(" A");
  }

  // Solicitar posición (CMD_ID_GET_ENCODER_ESTIMATES)
  odriveCAN.GetEncoderEstimates(axis_id);
  if (odriveCAN.ReadMsg(msg)) {
    EncoderEstimatesMsg_t encoderData;
    odriveCAN.EncoderEstimates(encoderData, msg);
    position = encoderData.pos_estimate;  //Posición en "counts"
    Serial.print("Posición en Grados: "); 
    Serial.print(position * 360.0); 
    Serial.println("°");
  }
  delay(100);
}