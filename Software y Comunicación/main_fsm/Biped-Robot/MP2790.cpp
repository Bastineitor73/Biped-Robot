/*
 * @file MP2790.cpp
 * @brief Source file for the MP2790 library using a full-featured external descriptor pattern.
 *
 * @author Alonso Garrido Diaz, LCDA UDEC (Refactored by Gemini)
 * @date 2025-07-18
 * @license MIT License
 */

#include "MP2790.h"

// ============================================================================
// MP2790 Class Implementation
// ============================================================================

MP2790::MP2790(uint8_t address, uint8_t cells, void* wireObj)
    : _devAddr(address), _nCells(cells), _wireObj(wireObj) {}

uint16_t MP2790::readAdd(RegDes reg) {

    // Check if the register is writable-only
    if (reg.access == RWAccess::WRITE_ONLY) {
        if (Serial) {
            Serial.println("Warning: Attempted to read from a write-only register.");
        }
        return 0; // Return 0 or some other error indicator
    }
    uint16_t data = 0;
    if(I2Cdev::readBitsW(_devAddr, reg.addr, reg.start, reg.len, &data, I2Cdev::readTimeout, _wireObj) < 0) {
        if (Serial) {
            Serial.print("Failed to read register 0x");
            Serial.print(reg.addr, HEX);
            Serial.println(". Returning 0.");
        }
        return 0; // Return 0 on failure
    }
    return data;
}

uint16_t MP2790::readAdd(uint8_t regNum) {
    // Check if the register is writable-only
    // if (reg.access == RWAccess::WRITE_ONLY) {
    //     if (Serial) {
    //         Serial.println("Warning: Attempted to read from a write-only register.");
    //     }
    //     return 0; // Return 0 or some other error indicator
    // }
    uint16_t data = 0;

    if(I2Cdev::readWord(_devAddr, regNum, &data, I2Cdev::readTimeout, _wireObj) < 0) {
        if (Serial) {
            Serial.print("Failed to read register 0x");
            Serial.print(regNum, HEX);
            Serial.println(". Returning 0.");
        }
        return 0; // Return 0 on failure
    }
    return data;
}


bool MP2790::writeAdd(RegDes reg, uint16_t value) {
    // Check if the register is readable-only
    if (reg.access == RWAccess::READ_ONLY) {
        if (Serial) {
            Serial.println("Warning: Attempted to write to a read-only register. Operation cancelled.");
        }
        return false; // Do not proceed with the write
    }
    return I2Cdev::writeBitsW(_devAddr, reg.addr, reg.start, reg.len, value, _wireObj);
}



bool MP2790::writeAdd(uint8_t regNum, uint16_t value) {
    // Check if the register is readable-only
    // if (reg.access == RWAccess::READ_ONLY) {
    //     if (Serial) {
    //         Serial.println("Warning: Attempted to write to a read-only register. Operation cancelled.");
    //     }
    //     return; // Do not proceed with the write
    // }
    delay(20);

    return I2Cdev::writeWord(_devAddr, regNum, value, _wireObj);
    // I2Cdev::writeBitsW(_devAddr, reg.addr, reg.start, reg.len, value, _wireObj);
}

bool MP2790::testConnection() {
    uint16_t deviceId = this->readAdd(MP2790_Reg::DEVICE_ADD);
    if (Serial) {
        Serial.print("Probing device. Expecting ID 0x");
        Serial.print(_devAddr, HEX);
        Serial.print(", found ID: 0x");
        Serial.println(deviceId, HEX);
    }
    return (deviceId == _devAddr);
}

void MP2790::setAddress(uint8_t newAddress) {
    if (Serial) {
        Serial.print("Sending command to change address from 0x");
        Serial.print(_devAddr, HEX);
        Serial.print(" to 0x");
        Serial.println(newAddress, HEX);
    }

    bool status = writeAdd(MP2790_Reg::DEVICE_ADD, newAddress);
    if (!status) {
        if (Serial) {
            Serial.println("Failed to set new address.");
        }
        return;
    }
    
    this->_devAddr = newAddress;
    if (Serial) {
        Serial.println("Internal device address updated.");
    }
}

uint8_t MP2790::getAddress() {
    return _devAddr;
}

bool MP2790::storeNVM() {
    this->writeAdd(MP2790_Reg::STORE_NVM_CMD, 0x01);
    for(int i = 0; i < 5; i++){
        delay(100);
        if (this->readAdd(MP2790_Reg::STORE_IN_PROGRESS) == 0) {
            if (Serial) Serial.println("NVM Store Successful!");
            return true;
        }
        if (Serial) Serial.println("NVM store in progress...");
    }
    if (Serial) Serial.println("NVM Store Failed!");
    return false;
}

void MP2790::storeAccessCode() {
    this->writeAdd(MP2790_Reg::CELL_CMD_ACCESS_CODE, 0xA5B6);
}

uint8_t MP2790::powerStatus() {
    uint8_t status = this->readAdd(MP2790_Reg::PWR_STATE);
    // Serial.print("Power status: ");
    // switch (status) {
    //     case 0b00001:
    //         Serial.println("Safe");
    //         break;
    //     case 0b00010:
    //         Serial.println("Standby");
    //         break;
    //     case 0b00100:
    //         Serial.println("Normal B");
    //         break;
    //     case 0b01000:
    //         Serial.println("Normal C");
    //         break;
    //     case 0b10000:
    //         Serial.println("Normal A");
    //         break;
    //     default:
    //         Serial.println("Unknown status");
    //         break;
    // }
    return status;
}

void MP2790::currentDirection() {
    uint8_t status = this->readAdd(MP2790_Reg::PACK_CURRENT_STATUS);
    Serial.print("Current direction: ");
    switch (status)
    {
    case 0x01:
        Serial.println("Discharging");
        break;
    case 0x02:
        Serial.println("Standby");
        break;
    case 0x03:
        Serial.println("Charging");
        break;
    default:    
        Serial.println("Invalid");
        break;
    }
}

bool MP2790::getInt(bool *intFlags) {
    bool intStatus = false;
    uint32_t tempFlags = 0;
    uint16_t int0 =  this->readAdd(RD_INT0);
    uint16_t int1 =  this->readAdd(RD_INT1);
    tempFlags = (uint32_t)int0 | (uint32_t)int1 << 16;

    // Serial.print("Interrupts_read_raw");
    // Serial.println(int0, BIN);
    // Serial.println(int1, BIN);
    // Serial.println(tempFlags, BIN);

    for (byte i = 0; i < 32; i++) {
        intFlags[i] = (tempFlags >> i) & 0x01;
        if (intFlags[i]) {
            intStatus = true;
            Serial.print("Interrupt Flag ");
            Serial.print(i);
            Serial.print(" is set. ");
            Serial.println(InterruptTable[i].name);
        }
    }
    return intStatus;
}

void MP2790::clearInt(uint8_t intNum) {

    if (intNum < 16){
        RegDes reg = {INT0_CLR, intNum, 1, RWAccess::READ_WRITE};
        MP2790::writeAdd(reg, 1);
    }
    else{
        RegDes reg = {INT1_CLR, intNum-16, 1};
        MP2790::writeAdd(reg, 1);
    }

    if (Serial) {
        Serial.print("Cleared interrupt ");
        Serial.println(intNum);
    }
    return;
}

// void returnInterrupt(uint8_t intNum) {
//     if (intNum > 0 && intNum < 32) {
//         switch (intNum)
//         {
//         case 0:
//             Serial.println("Interrupt 0: Cell Over Voltage");
//             break;
//         case 1:
//             Serial.println("Interrupt 1: Cell Under Voltage");
//             break;
//         case 2:
//             Serial.println("Interrupt 2: Cell Mismatch");
//             break;
//         case 3:
//             Serial.println("Interrupt 3: Cell Dead");
//             break;
//         case 4:
//             Serial.println("Interrupt 4: Cell Open Wire");
//             break;
//         case 5:
//             Serial.println("Interrupt 5: Cell Over Temperature");
//             break;
//         case 6:
//             Serial.println("Interrupt 6: Cell Under Temperature");
//             break;
//         case 7: 
//             Serial.println("Interrupt 7: Cell Hot Cold DSG");
//             break;  
//         case 8:
//             Serial.println("Interrupt 8: Cell Hot Cold CHG");
//             break;
//         case 9:
            
//             Serial.println("Interrupt 9: Cell Measurement");
//             break;
//         case 10:
//             Serial.println("Interrupt 10: Cell NTC Measurement");
//             break;
        
//         default:
//             break;
//         }
//     }

// }

/**
 * @brief Checks the status of all cell under-voltage flags.
 * @param cellUVFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has a UV fault, false otherwise.
 */
bool MP2790::getCellOVUV(bool* cellUVFlags, bool* cellOVFlags) {
    // Initialize the flags to false
    bool anyCellOVUV = false;
    // Read the entire 16-bit register containing all UV flags.
    uint16_t tempFlags = this->readAdd(RD_CELL_UV);
    // Extract the flag for each cell from the register's bits.
    for (int i = 0; i < this->_nCells; i++) {
        cellUVFlags[i] = (tempFlags >> i) & 0x01;
        if (cellUVFlags[i]) {
            anyCellOVUV = true;
        }
    }
    // Read the entire 16-bit register containing all OV flags.
    tempFlags = this->readAdd(RD_CELL_OV);
    for (int i = 0; i < this->_nCells; i++) {
        cellOVFlags[i] = (tempFlags >> i) & 0x01;
        if (cellOVFlags[i]) {
            anyCellOVUV = true;
        }
    }
    return anyCellOVUV;
}

/**
 * @brief Checks the status of all cell mismatch flags.
 * @param cellMismatchFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has a mismatch fault, false otherwise.
 */
bool MP2790::getCellAlert(bool* cellMismatchFlags, bool* cellDeadFlags, bool* cellOpenWireFlags) {

    //Cell Mismatch
    bool anyCellMismatch = false;
    uint16_t tempFlags = this->readAdd(RD_CELL_MSMT);
    for (int i = 0; i < this->_nCells; i++) {
        cellMismatchFlags[i] = (tempFlags >> i) & 0x01;
        if (cellMismatchFlags[i]) {
            anyCellMismatch = true;
        }
    }
    //Cell Dead
    bool anyCellDead = false;
    tempFlags = this->readAdd(RD_CELL_DEAD);
    for (int i = 0; i < this->_nCells; i++) {
        cellDeadFlags[i] = (tempFlags >> i) & 0x01;
        if (cellDeadFlags[i]) {
            anyCellDead = true;
        }
    }

    //Cell Open wire
    bool anyCellOpenWire = false;
    tempFlags = this->readAdd(RD_OPENH);
    for (int i = 0; i < this->_nCells; i++) {
        cellOpenWireFlags[i] = (tempFlags >> i) & 0x01;
        if (cellOpenWireFlags[i]) {
            anyCellOpenWire = true;
        }
    }

    bool anyCellAlert = anyCellMismatch || anyCellDead || anyCellOpenWire;
    return anyCellAlert;
}

/**
 * @brief Gets the low-resolution readings for all four NTCs.
 * @param ntcValues A float array to be filled with the NTC values as a percentage of V3V3.
 */
void MP2790::getNTCreadings(uint16_t* ntcValues) {
    // Create an array of descriptors for easy iteration.
    const RegDes ntc_regs[] = {
        MP2790_Reg::NTC1_VALUE,
        MP2790_Reg::NTC2_VALUE,
        MP2790_Reg::NTC3_VALUE,
        MP2790_Reg::NTC4_VALUE
    };

    for (int i = 0; i < 4; i++) {
        uint16_t rawValue = this->readAdd(ntc_regs[i]);
        // Convert the raw 10-bit value to a percentage of the reference voltage.
        ntcValues[i] = (uint16_t)(rawValue * 100.0 / 1023.0);
    }
}

/**
 * @brief Checks the hot/cold status for all NTCs during discharge.
 * @param ntcHotColdDSGFlags A boolean array to be filled with the status.
 * @return True if any NTC has a fault, false otherwise.
 */
bool MP2790::getNTCHotCold(bool* ntcHotColdDSGFlags, bool* ntcHotColdCHGFlags) {
    bool ntcHotColdStatus = false;

    // DSG read of the 4-bit field containing all flags at once.
    constexpr RegDes all_ntc_dsg_flags = {RD_V_NTC4_LR, 12, 4, RWAccess::READ_ONLY, NVMType::VOLATILE};
    uint16_t tempFlags = this->readAdd(all_ntc_dsg_flags);
    for (int i = 0; i < 4; i++) {
        ntcHotColdDSGFlags[i] = (tempFlags >> i) & 0x01;
        if (ntcHotColdDSGFlags[i]) {
            ntcHotColdStatus = true;
        }
    }
    
    // CHG read of the 4-bit field containing all flags at once.
    constexpr RegDes all_ntc_chg_flags = {RD_V_NTC3_LR, 12, 4, RWAccess::READ_ONLY, NVMType::VOLATILE};
    tempFlags = this->readAdd(all_ntc_chg_flags);
    for (int i = 0; i < 4; i++) {
        ntcHotColdCHGFlags[i] = (tempFlags >> i) & 0x01;
        if (ntcHotColdCHGFlags[i]) {
            ntcHotColdStatus = true;
        }
    }

    return ntcHotColdStatus;

}

/**
 * @brief Gets the die temperature.
 * @return The die temperature in degrees Celsius.
 */
uint16_t MP2790::getDieTemperature() {
    float rawValue = this->readAdd(MP2790_Reg::DIE_TEMP_V);
    // Apply the conversion formula from the datasheet.
    // return (uint16_t)(rawValue * 0.474f - 269.12f);
    return (uint16_t)(rawValue);
}

/**
 * @brief Gets the low-resolution readings of the internal ADC channels.
 * @param adcValues A float array to be filled with the voltage readings.
 */
void MP2790::getADCReadings(uint16_t* adcValues) {
    const RegDes adc_regs[] = {
        MP2790_Reg::VDD_VALUE,
        MP2790_Reg::V3V3_VALUE,
        MP2790_Reg::REGIN_VALUE,
        MP2790_Reg::SELF_TEST_VALUE
    };

    for (int i = 0; i < 4; i++) {
        uint16_t rawValue = this->readAdd(adc_regs[i]);
        // Apply different scaling factors based on the channel.
        if (i == 1 || i == 2) { // V3V3 and REGIN
            adcValues[i] = (uint16_t)(rawValue * 6.4453f);// / 1000.0f);
        } else { // VDD and SELF_TEST
            adcValues[i] = (uint16_t)(rawValue * 3.2227f);// / 1000.0f);
        }
    }
}

/**
 * @brief Gets the high-resolution reading of the PACKP voltage.
 * @return The voltage in millivolts (mV).
 */
uint16_t MP2790::getPackPReadings() {
    uint16_t rawValue = this->readAdd(MP2790_Reg::VPACK_V);
    return (uint16_t)(rawValue * 80000.0f / 32768.0f);
}

/**
 * @brief Gets the high-resolution reading of the VTOP voltage.
 * @return The voltage in millivolts (mV).
 */
uint16_t MP2790::getVTopReadings() {
    uint16_t rawValue = this->readAdd(MP2790_Reg::VTOP_V);
    return (uint16_t)(rawValue * 80000.0f / 32768.0f);
}

/**
 * @brief Gets the high-resolution reading of the ITop current.
 * @return The current in milliamps (mA).
 */
int16_t MP2790::getITopReadings() {
    int16_t rawValue = (int16_t)this->readAdd(MP2790_Reg::VTOP_I); // Cast to signed
    // Convert from raw ADC value to mA using the sense resistor value.
    return (int16_t)(rawValue * 100.0f / (32768.0f * MP2790_R_SENSE));
}

/**
 * @brief Gets high-resolution voltage and current for all cells.
 * @param cellHRVoltage A float array to be filled with cell voltages (mV).
 * @param cellHRCurrent A float array to be filled with cell currents (mA).
 */
void MP2790::getHRCellReadings(uint16_t* cellHRVoltage, int16_t* cellHRCurrent) {
    constexpr RegDes v_regs[] = {
        MP2790_Reg::CELL_1_V, MP2790_Reg::CELL_2_V,
        MP2790_Reg::CELL_3_V, MP2790_Reg::CELL_4_V,
        MP2790_Reg::CELL_5_V, MP2790_Reg::CELL_6_V,
        MP2790_Reg::CELL_7_V, MP2790_Reg::CELL_8_V,
        MP2790_Reg::CELL_9_V, MP2790_Reg::CELL_10_V
    };
    constexpr RegDes i_regs[] = {
        MP2790_Reg::CELL_1_I_SYNC, MP2790_Reg::CELL_2_I_SYNC,
        MP2790_Reg::CELL_3_I_SYNC, MP2790_Reg::CELL_4_I_SYNC,
        MP2790_Reg::CELL_5_I_SYNC, MP2790_Reg::CELL_6_I_SYNC,
        MP2790_Reg::CELL_7_I_SYNC, MP2790_Reg::CELL_8_I_SYNC,
        MP2790_Reg::CELL_9_I_SYNC, MP2790_Reg::CELL_10_I_SYNC
    };

    for (int i = 0; i < this->_nCells; i++) {
        uint16_t rawVoltage = this->readAdd(v_regs[i]);
        cellHRVoltage[i] = (uint16_t)(rawVoltage * 5000.0f / 32768.0f);

        int16_t rawCurrent = (int16_t)this->readAdd(i_regs[i]);
        cellHRCurrent[i] = (int16_t)(rawCurrent * 100.0f / (32768.0f * MP2790_R_SENSE));
    }
}

/**
 * @brief Gets high-resolution readings for all NTCs.
 * @param ntcHRValue A float array to be filled with NTC voltages (mV).
 */
void MP2790::getHRNTCReadings(uint16_t* ntcHRValue) {
    constexpr RegDes ntc_hr_regs[] = {
        MP2790_Reg::NTC1_HIRES_V,
        MP2790_Reg::NTC2_HIRES_V,
        MP2790_Reg::NTC3_HIRES_V,
        MP2790_Reg::NTC4_HIRES_V
    };
    for (int i = 0; i < 4; i++) {
        uint16_t rawValue = this->readAdd(ntc_hr_regs[i]);
        ntcHRValue[i] = (uint16_t)(rawValue * 3300.0f / 32768.0f); // Assuming 3.3V reference
    }
}

/**
 * @brief Gets high-resolution readings for all GPIOs.
 * @param gpioHRValue A float array to be filled with GPIO voltages (mV).
 */
void MP2790::getHRGPIOReadings(uint16_t* gpioHRValue) {
    constexpr RegDes gpio_hr_regs[] = {
        MP2790_Reg::GPIO1_VOLTAGE,
        MP2790_Reg::GPIO2_VOLTAGE,
        MP2790_Reg::GPIO3_VOLTAGE
    };
    for (int i = 0; i < 3; i++) {
        uint16_t rawValue = this->readAdd(gpio_hr_regs[i]);
        gpioHRValue[i] = (uint16_t)(rawValue * 3300.0f / 32768.0f); // Assuming 3.3V reference
    }
}

/**
 * @brief Gets the high-resolution die temperature.
 * @return The die temperature in degrees Celsius.
 */
uint16_t MP2790::getHRDieTemperature() {
    float rawValue = this->readAdd(MP2790_Reg::DIE_T_VOLTAGE);
    // return (uint16_t)(rawValue * 0.014181f - 269.12f);
    return (uint16_t)(rawValue);
}

/**
 * @brief Gets high-resolution readings of the internal ADC channels.
 * @param adcHRValues A float array to be filled with the voltage readings (mV).
 */
void MP2790::getHRADCReadings(uint16_t* adcHRValues) {
    const RegDes adc_hr_regs[] = {
        MP2790_Reg::VDD_VOLTAGE,
        MP2790_Reg::V3V3_VOLTAGE,
        MP2790_Reg::REGIN_VOLTAGE
    };
    for (int i = 0; i < 3; i++) {
        uint16_t rawValue = this->readAdd(adc_hr_regs[i]);
        if (i == 0) { // VDD (V1P8)
            adcHRValues[i] = (uint16_t)(rawValue * 3300.0f / 32768.0f);
        } else { // V3V3 and REGIN
            adcHRValues[i] = (uint16_t)(rawValue * 6600.0f / 32768.0f);
        }
    }
}

// bool MP2790::clearDeadCell(uint8_t cellNum) {
//     if (cellNum < 1 || cellNum > this->_nCells) {
//         if (Serial) {
//             Serial.print("Invalid cell number: ");
//             Serial.println(cellNum);
//         }
//         return false;
//     }
//     uint16_t command = 0x0001 << (cellNum - 1);
//     return this->writeAdd(MP2790_Reg::CELL_DEAD_DET_CLEAR, command);
// }