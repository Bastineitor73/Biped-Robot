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

uint16_t MP2790::readReg(RegDes reg) {
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

bool MP2790::writeReg(RegDes reg, uint16_t value) {
    // Check if the register is readable-only
    if (reg.access == RWAccess::READ_ONLY) {
        if (Serial) {
            Serial.println("Warning: Attempted to write to a read-only register. Operation cancelled.");
        }
        return false; // Do not proceed with the write
    }
    return I2Cdev::writeBitsW(_devAddr, reg.addr, reg.start, reg.len, value, _wireObj);
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
    uint16_t deviceId = this->readReg(MP2790_Reg::DEVICE_ADD);
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

    bool status = writeReg(MP2790_Reg::DEVICE_ADD, newAddress);
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
    this->writeReg(MP2790_Reg::STORE_NVM_CMD, 0x01);
    for(int i = 0; i < 5; i++){
        delay(100);
        if (this->readReg(MP2790_Reg::STORE_IN_PROGRESS) == 0) {
            if (Serial) Serial.println("NVM Store Successful!");
            return true;
        }
        if (Serial) Serial.println("NVM store in progress...");
    }
    if (Serial) Serial.println("NVM Store Failed!");
    return false;
}

void MP2790::storeAccessCode() {
    this->writeReg(MP2790_Reg::CELL_CMD_ACCESS_CODE, 0xA5B6);
}

bool MP2790::getInterrupts(bool* intFlags) {
    bool intStatus = false;
    uint32_t tempFlags;;
    tempFlags = this->readAdd(RD_INT0);
    tempFlags |= this->readAdd(RD_INT1)<<8;
    for (uint8_t i = 0; i < 32; i++) {
        intFlags[i] = (tempFlags >> 1) & 0x01;
        if (intFlags[i]) {
            intStatus = true;
        }
    }
    return intStatus;
}

/**
 * @brief Checks the status of all cell under-voltage flags.
 * @param cellUVFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has a UV fault, false otherwise.
 */
bool MP2790::getCellUVStatus(bool* cellUVFlags) {
    bool anyCellUV = false;
    // Read the entire 16-bit register containing all UV flags.
    uint16_t tempFlags = this->readAdd(RD_CELL_UV);

    // Extract the flag for each cell from the register's bits.
    for (int i = 0; i < this->_nCells; i++) {
        cellUVFlags[i] = (tempFlags >> i) & 0x01;
        if (cellUVFlags[i]) {
            anyCellUV = true;
        }
    }
    return anyCellUV;
}

/**
 * @brief Checks the status of all cell over-voltage flags.
 * @param cellOVFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has an OV fault, false otherwise.
 */
bool MP2790::getCellOVStatus(bool* cellOVFlags) {
    bool anyCellOV = false;
    uint16_t tempFlags = this->readAdd(RD_CELL_OV);
    for (int i = 0; i < this->_nCells; i++) {
        cellOVFlags[i] = (tempFlags >> i) & 0x01;
        if (cellOVFlags[i]) {
            anyCellOV = true;
        }
    }
    return anyCellOV;
}

/**
 * @brief Checks the status of all cell mismatch flags.
 * @param cellMismatchFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has a mismatch fault, false otherwise.
 */
bool MP2790::getCellMismatchStatus(bool* cellMismatchFlags) {
    bool anyCellMismatch = false;
    uint16_t tempFlags = this->readAdd(RD_CELL_MSMT);
    for (int i = 0; i < this->_nCells; i++) {
        cellMismatchFlags[i] = (tempFlags >> i) & 0x01;
        if (cellMismatchFlags[i]) {
            anyCellMismatch = true;
        }
    }
    return anyCellMismatch;
}

/**
 * @brief Checks the status of all dead cell flags.
 * @param cellDeadFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has a dead fault, false otherwise.
 */
bool MP2790::getCellDeadStatus(bool* cellDeadFlags) {
    bool anyCellDead = false;
    uint16_t tempFlags = this->readAdd(RD_CELL_DEAD);
    for (int i = 0; i < this->_nCells; i++) {
        cellDeadFlags[i] = (tempFlags >> i) & 0x01;
        if (cellDeadFlags[i]) {
            anyCellDead = true;
        }
    }
    return anyCellDead;
}

/**
 * @brief Checks the status of all cell open-wire flags.
 * @param cellOpenWireFlags A boolean array to be filled with the status for each cell.
 * @return True if any cell has an open-wire fault, false otherwise.
 */
bool MP2790::getCellOpenWireStatus(bool* cellOpenWireFlags) {
    bool anyCellOpenWire = false;
    uint16_t tempFlags = this->readAdd(RD_OPENH);
    for (int i = 0; i < this->_nCells; i++) {
        cellOpenWireFlags[i] = (tempFlags >> i) & 0x01;
        if (cellOpenWireFlags[i]) {
            anyCellOpenWire = true;
        }
    }
    return anyCellOpenWire;
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
        uint16_t rawValue = this->readReg(ntc_regs[i]);
        // Convert the raw 10-bit value to a percentage of the reference voltage.
        ntcValues[i] = (uint16_t)(rawValue * 100.0 / 1023.0);
    }
}

/**
 * @brief Checks the hot/cold status for all NTCs during discharge.
 * @param ntcHotColdDSGFlags A boolean array to be filled with the status.
 * @return True if any NTC has a fault, false otherwise.
 */
bool MP2790::getNTCHotColdDSGStatus(bool* ntcHotColdDSGFlags) {
    bool ntcHotColdDSGStatus = false;
    // This descriptor reads the 4-bit field containing all flags at once.
    constexpr RegDes all_ntc_dsg_flags = {RD_V_NTC4_LR, 12, 4, RWAccess::READ_ONLY, NVMType::VOLATILE};
    uint16_t tempFlags = this->readReg(all_ntc_dsg_flags);
    for (int i = 0; i < 4; i++) {
        ntcHotColdDSGFlags[i] = (tempFlags >> i) & 0x01;
        if (ntcHotColdDSGFlags[i]) {
            ntcHotColdDSGStatus = true;
        }
    }
    return ntcHotColdDSGStatus;
}

/**
 * @brief Checks the hot/cold status for all NTCs during charge.
 * @param ntcHotColdCHGFlags A boolean array to be filled with the status.
 * @return True if any NTC has a fault, false otherwise.
 */
bool MP2790::getNTCHotColdCHGStatus(bool* ntcHotColdCHGFlags) {
    bool ntcHotColdCHGStatus = false;
    constexpr RegDes all_ntc_chg_flags = {RD_V_NTC3_LR, 12, 4, RWAccess::READ_ONLY, NVMType::VOLATILE};
    uint16_t tempFlags = this->readReg(all_ntc_chg_flags);
    for (int i = 0; i < 4; i++) {
        ntcHotColdCHGFlags[i] = (tempFlags >> i) & 0x01;
        if (ntcHotColdCHGFlags[i]) {
            ntcHotColdCHGStatus = true;
        }
    }
    return ntcHotColdCHGStatus;
}

/**
 * @brief Gets the die temperature.
 * @return The die temperature in degrees Celsius.
 */
uint16_t MP2790::getDieTemperature() {
    float rawValue = this->readReg(MP2790_Reg::DIE_TEMP_V);
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
        uint16_t rawValue = this->readReg(adc_regs[i]);
        // Apply different scaling factors based on the channel.
        if (i == 1 || i == 2) { // V3V3 and REGIN
            adcValues[i] = (uint16_t)(rawValue);// * 6.4453f);// / 1000.0f);
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
    uint16_t rawValue = this->readReg(MP2790_Reg::VPACK_V);
    return (uint16_t)(rawValue * 80000.0f / 32768.0f);
}

/**
 * @brief Gets the high-resolution reading of the VTOP voltage.
 * @return The voltage in millivolts (mV).
 */
uint16_t MP2790::getVTopReadings() {
    uint16_t rawValue = this->readReg(MP2790_Reg::VTOP_V);
    return (uint16_t)(rawValue * 80000.0f / 32768.0f);
}

/**
 * @brief Gets the high-resolution reading of the ITop current.
 * @return The current in milliamps (mA).
 */
int16_t MP2790::getITopReadings() {
    int16_t rawValue = (int16_t)this->readReg(MP2790_Reg::VTOP_I); // Cast to signed
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
        uint16_t rawVoltage = this->readReg(v_regs[i]);
        cellHRVoltage[i] = (uint16_t)(rawVoltage * 5000.0f / 32768.0f);

        int16_t rawCurrent = (int16_t)this->readReg(i_regs[i]);
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
        uint16_t rawValue = this->readReg(ntc_hr_regs[i]);
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
        uint16_t rawValue = this->readReg(gpio_hr_regs[i]);
        gpioHRValue[i] = (uint16_t)(rawValue * 3300.0f / 32768.0f); // Assuming 3.3V reference
    }
}

/**
 * @brief Gets the high-resolution die temperature.
 * @return The die temperature in degrees Celsius.
 */
uint16_t MP2790::getHRDieTemperature() {
    float rawValue = this->readReg(MP2790_Reg::DIE_T_VOLTAGE);
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
        uint16_t rawValue = this->readReg(adc_hr_regs[i]);
        if (i == 0) { // VDD (V1P8)
            adcHRValues[i] = (uint16_t)(rawValue * 3300.0f / 32768.0f);
        } else { // V3V3 and REGIN
            adcHRValues[i] = (uint16_t)(rawValue * 6600.0f / 32768.0f);
        }
    }
}
