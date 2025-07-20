# ODriveTeensyCAN

Library for interfacing ODrive with Teensy 3/4 over CANbus. 

Requires FlexCAN_T4 (https://github.com/tonton81/FlexCAN_T4) and ODrive firmware version 0.5.4 or later.

You can review the ODrive CAN protocol, called CAN Simple, here: https://newdocs.odriverobotics.com/v/latest/can-protocol.html


## Getting Started

Your sketch needs to include both FlexCAN_T4 and ODriveTeensyCAN libraries. Be sure to include FlexCAN_T4 *before* ODriveTeensyCAN.
```
#include <FlexCAN_T4.h>
#include <ODriveTeensyCAN.h>
```
Create ODriveTeensyCAN object with `ODriveTeensyCAN odriveCAN();` where `odriveCAN` is any name you want to call your object. You can optionally change the CAN baud rate by passing a parameter to the constructor `ODriveTeensyCAN odriveCAN(500000);`. The default baud rate is 250000.

### Reading Messages

You should keep an eye on the CAN bus to look for incoming messages. A good technique for this is something like the following:
```
CAN_message_t inMsg;

if(odriveCAN.ReadMsg(inMsg)) {
	handleCANMsg(inMsg);
}
```
Create a variable of type `CAN_message_t` and use `ReadMsg()` inside an if statement to check for incoming messages. Pass the incoming message to a function that will handle the id and data.

The library defines structs that help contain the data received from CAN messages. You can use them to easily store data from messages that send multiple values in a single message like encoder estimates message. First, create new variables of type `HeartbeatMsg_t`, `EncoderEstimatesMsg_t`, `EncoderCountsMsg_t`, `SensorlessEstimatesMsg_t` or `IqMsg_t`. Then use the handler function to store data in the struct.
```
HeartbeatMsg_t returnVals;
IqMsg_t iqVals;

void handleCANMsg(CAN_message_t inMsg) {
	uint32_t axis_id = inMsg.id >> 6;
    uint8_t cmd_id = inMsg.id & 0x01F;
    
    switch(cmd_id) {
      case (ODriveTeensyCAN::CMD_ID_ODRIVE_HEARTBEAT_MESSAGE):
        odriveCAN.Heartbeat(returnVals, inMsg);
        break;
      case (ODriveTeensyCAN::CMD_ID_GET_IQ):
        odriveCAN.GetIqResponse(iqVals, inMsg);
        break;
      default:
        break;
    }
}
```
To store data from messages that send only one value, simply define a variable of the correct type and treat the CAN message like any other function that returns a value
```
float adcVoltage;
float vbusVoltage;

	  ...
      case (ODriveTeensyCAN::CMD_ID_GET_ADC_VOLTAGE):
        adcVoltage = odriveCAN.GetADCVoltageResponse(inMsg);
        break;
      case (ODriveTeensyCAN::CMD_ID_GET_VBUS_VOLTAGE):
        vbusVoltage = odriveCAN.GetVbusVoltageResponse(inMsg);
        break;
	  ...
```
		
### Sending Messages

Sending messages with the library works the same as calling any other function with the required arguments. For example, to set a new node id you can you call the function `odriveCAN.SetAxisNodeId(0, 1)` where 0 is the axis id and 1 is the desired node id.