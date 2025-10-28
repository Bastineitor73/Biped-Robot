/*
 * @file MP2790.h
 * @brief Header file for the MP2790 library using a full-featured external descriptor pattern.
 *
 * This design completely decouples the register definitions from the main
 * device class. Each descriptor contains all properties of a register,
 * including access rights and NVM type.
 *
 * @author Alonso Garrido Diaz, LCDA UDEC (Refactored by Gemini)
 * @date 2025-07-18
 * @license MIT License
 */

#ifndef MP2790_HPP
#define MP2790_HPP

#include <cstdint>
#include "I2Cdev.h"
#include <Arduino.h>

// Main definitions
#define MP27XX_DEFAULT_ADDRESS  0x01
#define MP27XX_DEFAULT_CELLS    10
#define MP2790_R_SENSE          1.5
#define ADC_CONVERSION_TIME     20
#define ADC_SCAN_TIMEOUT        2000


// ============================================================================
// Parent Register Addresses
// ============================================================================
#define CELLS_CTRL                  0x00
#define PWR_STATUS                  0x01
#define STB_STATUS                  0x02
#define LOAD_CHARGER_STATUS         0x03
#define ACT_CFG                     0x05
#define STB_CFG                     0x06
#define SAFE_CFG                    0x07
#define RGL_CFG                     0x08
#define LOAD_CHARGER_CFG            0x09
#define GPIO_STATUS                 0x0A
#define GPIO_OUT                    0x0B
#define GPIO_CFG                    0x0C
#define PINS_CFG                    0x0D
#define WDT_STATUS                  0x0E
#define WDT_RST_X                   0x0F
#define WDT_CFG_X                   0x10
#define FET_STATUS                  0x11
#define FET_CTRL                    0x12
#define FET_MODE                    0x13
#define FET_CFG_X                   0x14
#define RD_INT0                     0x15
#define RD_INT1                     0x16
#define INT0_CLR                    0x17
#define INT1_CLR                    0x18
#define INT0_EN                     0x19
#define INT1_EN                     0x1A
#define INT_TYPE0                   0x1B
#define INT_TYPE1                   0x1C
#define INT_TYPE2                   0x1D
#define MASK_INT0                   0x1E
#define MASK_INT1                   0x1F
#define OC_STATUS                   0x20
#define OCFT_CTRL                   0x23
#define DSGOC_LIM                   0x24
#define DSGOC_DEG                   0x25
#define CHGOC_DEG                   0x26
#define SC_STATUS                   0x27
#define SCFT_CTRL                   0x2A
#define DSGSC_CFG                   0x2B
#define CHGSC_CFG                   0x2C
#define RD_CELL_UV                  0x2D
#define RD_CELL_OV                  0x2E
#define RD_CELL_MSMT                0x2F
#define RD_CELL_DEAD                0x30
#define CELL_MSMT_STS               0x33
#define PACKFT_CTRL                 0x34
#define CELLFT_CTRL                 0x35
#define CELL_HYST                   0x36
#define PACK_UV_OV                  0x37
#define CELL_UV_X                   0x38
#define CELL_OV_X                   0x39
#define PACK_UV                     0x3A
#define PACK_OV                     0x3B
#define CELL_DEAD_THR               0x3C
#define CELL_MSMT               	0x3D
#define RD_NTC_DIE                  0x3E
#define RD_V_NTC4_LR                0x3F
#define RD_V_NTC3_LR                0x40
#define RD_V_NTC2_LR                0x41
#define RD_V_NTC1_LR                0x42
#define RD_T_DIE                    0x43
#define NTC_CLR                     0x44
#define DIE_CFG                     0x46
#define NTC_CFG                     0x47
#define NTCC_OTHR_DSG               0x48
#define NTCC_UTHR_DSG               0x49
#define NTCC_OTHR_CHG               0x4A
#define NTCC_UTHR_CHG               0x4B
#define NTCM_OTHR                   0x4C
#define DIE_OT                      0x4D
#define SELF_STS                    0x4E
#define RD_VA1P8                    0x4F
#define RD_VA3P3                    0x50
#define RD_VA5                      0x51
#define RD_VASELF                   0x52
#define RD_OPENH                    0x53
#define SFT_GO                      0x55
#define SELF_CFG                    0x56
#define OPEN_CFG                    0x57
#define REGIN_UV                    0x58
#define V3P3_UV                     0x59
#define VDD_UV                      0x5A
#define SELF_THR                    0x5B
#define FT_STS1                     0x5D
#define FT_STS2                     0x5E
#define FT_CLR                      0x5F
#define FT_REC                      0x60
#define FT0_CFG                     0x61
#define FT1_CFG                     0x62
#define RD_CCIRQL                   0x65
#define RD_CCIRQH                   0x66
#define RD_CCACCQL                  0x67
#define RD_CCACCQH                  0x68
#define RD_VPACKP                   0x69
#define RD_VTOP                     0x6A
#define RD_ITOP                     0x6B
#define RD_VCELL1                   0x6C
#define RD_ICELL1                   0x6D
#define RD_VCELL2                   0x6E
#define RD_ICELL2                   0x6F
#define RD_VCELL3                   0x70
#define RD_ICELL3                   0x71
#define RD_VCELL4                   0x72
#define RD_ICELL4                   0x73
#define RD_VCELL5                   0x74
#define RD_ICELL5                   0x75
#define RD_VCELL6                   0x76
#define RD_ICELL6                   0x77
#define RD_VCELL7                   0x78
#define RD_ICELL7                   0x79
#define RD_VCELL8                   0x7A
#define RD_ICELL8                   0x7B
#define RD_VCELL9                   0x7C
#define RD_ICELL9                   0x7D
#define RD_VCELL10                  0x7E
#define RD_ICELL10                  0x7F
#define RD_VNTC4                    0x8C
#define RD_VNTC3                    0x8D
#define RD_VNTC2                    0x8E
#define RD_VNTC1                    0x8F
#define RD_VGPIO3                   0x90
#define RD_VGPIO2                   0x91
#define RD_VGPIO1                   0x92
#define RD_TDIE                     0x93
#define RD_V1P8                     0x94
#define RD_V3P3                     0x95
#define RD_V5                       0x96
#define CC_STS                      0x97
#define ADC_STS                     0x98
#define ADC_CTRL                    0x99
#define CC_CFG                      0x9A
#define TRIMG_IPCB                  0x9B
#define HR_SCAN0                    0x9C
#define HR_SCAN1                    0x9D
#define HR_SCAN2                    0x9E
#define SILC_INFO1                  0xA0
#define COMM_CFG                    0xA3
#define BAL_STS                     0xA4
#define BAL_LIST                    0xA5
#define BAL_CTRL                    0xA6
#define BAL_CFG                     0xA7
#define BAL_THR                     0xA8
#define MEM_STATUS                  0xB4
#define OTP_STORE_CMD               0xB8
#define STORE_CMD_ACCESS_CODE_X     0xB9

// Enum for Read/Write access permissions
enum class RWAccess {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE
};

// Enum for Non-Volatile Memory type
enum class NVMType {
    VOLATILE,
    MTP, // Multi-Time Programmable
    OTP  // One-Time Programmable
};

/**
 * @brief A lightweight, compile-time descriptor for a device register.
 * Contains all static properties of a register.
 */
struct RegDes {
    const uint8_t addr;
    const uint8_t start;
    const uint8_t len;
    const RWAccess access;
    const NVMType nvm;
};

/**
 * @brief Namespace for all MP2790 register descriptors.
 */
namespace MP2790_Reg {
    // Format: { Parent Address, Start Bit, length, RW Access, NVM Type }
    constexpr RegDes CELL_S_CTRL                 = {CELLS_CTRL,0,4,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes PACK_CURRENT_STATUS         = {PWR_STATUS,7,3,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes PWR_STATE                   = {PWR_STATUS,0,5,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes PACKP_COMP_STS              = {STB_STATUS,8,3,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DSG_PFET_SYNC               = {STB_STATUS,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes STBY_STATE                  = {STB_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHG_IN                      = {LOAD_CHARGER_STATUS,15,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes LD_IN                       = {LOAD_CHARGER_STATUS,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHGDET_FAIL                 = {LOAD_CHARGER_STATUS,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes LDDET_FAIL                  = {LOAD_CHARGER_STATUS,12,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHGDET_ENGAGED              = {LOAD_CHARGER_STATUS,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes LDDET_ENGAGED               = {LOAD_CHARGER_STATUS,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHG_SETTLING                = {LOAD_CHARGER_STATUS,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes LD_SETTLING                 = {LOAD_CHARGER_STATUS,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHCONN_EN                   = {LOAD_CHARGER_STATUS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes LDDET_EN                    = {LOAD_CHARGER_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes FT_STATE_SEL                = {ACT_CFG,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes ACTIVE_CTRL                 = {ACT_CFG,3,2,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes FET_CFG                     = {ACT_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes FET_SRC                     = {ACT_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STBY_PFET_EN                = {STB_CFG,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STBY_MONITOR_CFG            = {STB_CFG,4,2,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STBY_HYS                    = {STB_CFG,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STBY_CUR_TH                 = {STB_CFG,1,2,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STBY_STATE_EN               = {STB_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NORM_MONITOR_CFG            = {SAFE_CFG,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SAFE_SCOC_EN                = {SAFE_CFG,1,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes PROTECT_IN_SAFE_CFG         = {SAFE_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes V3P3_SHOFF_STS              = {RGL_CFG,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3D3_SHDN_EN                = {RGL_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes PACKP_CMP_EN                = {LOAD_CHARGER_CFG,15,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes T_PLUGIN_PRECHARGE_MAX      = {LOAD_CHARGER_CFG,8,3,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CH_CONN_P_SBY               = {LOAD_CHARGER_CFG,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes LD_DET_DONE_STS             = {LOAD_CHARGER_CFG,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHG_DET_DONE_STS            = {LOAD_CHARGER_CFG,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes LD_PLUGIN_DET_EN            = {LOAD_CHARGER_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CH_PLUGIN_DET_EN            = {LOAD_CHARGER_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CHG_LD_CLR                  = {LOAD_CHARGER_CFG,0,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO1HV                     = {GPIO_STATUS,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO3                       = {GPIO_STATUS,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO2                       = {GPIO_STATUS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO1                       = {GPIO_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO1HV_HZ                  = {GPIO_OUT,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1HV_O                   = {GPIO_OUT,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO3_O                     = {GPIO_OUT,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO2_O                     = {GPIO_OUT,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1_O                     = {GPIO_OUT,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO3_PUP                   = {GPIO_CFG,10,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO3_TYPE                  = {GPIO_CFG,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO3_10                    = {GPIO_CFG,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO2_PUP                   = {GPIO_CFG,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO2_TYPE                  = {GPIO_CFG,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO2_10                    = {GPIO_CFG,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1_PUP                   = {GPIO_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1_TYPE                  = {GPIO_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1_10                    = {GPIO_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1_HV_CFG                = {PINS_CFG,10,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO_LV_CFG                 = {PINS_CFG,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes WDT_RST_EN                  = {PINS_CFG,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes WDT_RPT                     = {PINS_CFG,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes ALERT_POL                   = {PINS_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes WDT_BITE                    = {WDT_STATUS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes WDT_BARKED                  = {WDT_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes WDT_RST                     = {WDT_RST_X,0,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes WDT_BITE_CFG                = {WDT_CFG_X,9,7,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes WDT_BARK_CFG                = {WDT_CFG_X,2,7,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes WDT_COM_CTRL                = {WDT_CFG_X,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CP_STS                      = {FET_STATUS,11,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SBYDSG_DRV_TRANS            = {FET_STATUS,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DSG_DRV_TRANS               = {FET_STATUS,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHG_DRV_TRANS               = {FET_STATUS,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes FET_TIMEOUT                 = {FET_STATUS,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SBYDSG_DRV                  = {FET_STATUS,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DSG_DRV                     = {FET_STATUS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CHG_DRV                     = {FET_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes P_FET_MAN_CTRL              = {FET_CTRL,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes P_FET_MANUAL                = {FET_MODE,12,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CHG_SOFTON_OC_LIM           = {FET_MODE,11,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes TURNON_TIMEOUT_FAULT        = {FET_MODE,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CHG_TURNON_TIMER            = {FET_MODE,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes TURNON_TIMEOUT              = {FET_MODE,5,3,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CHG_SOFTON_EN               = {FET_MODE,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DSGFET_ON_RUN_SC_DET_EN     = {FET_MODE,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DSG_SOFTON_SBY_EN           = {FET_MODE,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DSG_SOFTON_EN               = {FET_MODE,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes FET_DRV_LVL                 = {FET_CFG_X,12,3,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CHG_SOFTON_PUP              = {FET_CFG_X,9,3,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes RAMP_UP_SC_GF               = {FET_CFG_X,6,3,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STBY_SC_CUR_TH              = {FET_CFG_X,4,2,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DSG_SOFTON_DV               = {FET_CFG_X,0,3,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes PACK_CURRENT_INT_STS        = {RD_INT0,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CONN_DET_INT_STS            = {RD_INT0,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_ACC_INT_STS              = {RD_INT0,12,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VSCAN_DONE_INT_STS          = {RD_INT0,11,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes AFE_MODE_CHANGE_INT_STS     = {RD_INT0,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes RECOVERED_INT_STS           = {RD_INT0,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes WDT_INT_STS                 = {RD_INT0,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_CHG_INT_STS             = {RD_INT0,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_DSG_INT_STS             = {RD_INT0,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SHORT_CURR_INT_STS          = {RD_INT0,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OVER_CURR_INT_STS           = {RD_INT0,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_INT_STS             = {RD_INT0,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_INT_STS             = {RD_INT0,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_INT_STS             = {RD_INT0,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_INT_STS             = {RD_INT0,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes FET_DRIVER_INT_STS          = {RD_INT1,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes PACKP_V_INT_STS             = {RD_INT1,12,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes BAL_DONE_INT_STS            = {RD_INT1,11,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_INT_STS           = {RD_INT1,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes FSM_ERROR_INT_STS           = {RD_INT1,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_HOT_INT_STS        = {RD_INT1,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_INT_STS            = {RD_INT1,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MISMATCH_INT_STS       = {RD_INT1,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_INT_STS           = {RD_INT1,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_INT_STS           = {RD_INT1,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VDD_INT_STS                 = {RD_INT1,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_INT_STS                = {RD_INT1,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes REGIN_INT_STS               = {RD_INT1,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OTP_CRC_EVENT_INT_STS       = {RD_INT1,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes PACK_CURRENT_INT_CLEAR      = {INT0_CLR,14,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CONN_DET_INT_CLEAR          = {INT0_CLR,13,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_ACC_STS_INT_CLEAR        = {INT0_CLR,12,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes VSCAN_DONE_INT_CLEAR        = {INT0_CLR,11,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes AFE_MODE_CHANGE_INT_CLEAR   = {INT0_CLR,10,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes RECOVERED_INT_CLEAR         = {INT0_CLR,9,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes WDT_INT_CLEAR               = {INT0_CLR,8,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_CHG_INT_CLEAR           = {INT0_CLR,7,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_DSG_INT_CLEAR           = {INT0_CLR,6,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes SHORT_CURR_INT_CLEAR        = {INT0_CLR,5,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OVER_CURR_INT_CLEAR         = {INT0_CLR,4,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_INT_CLEAR           = {INT0_CLR,3,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_INT_CLEAR           = {INT0_CLR,2,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_INT_CLEAR           = {INT0_CLR,1,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_INT_CLEAR           = {INT0_CLR,0,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes FET_DRIVER_INT_CLEAR        = {INT1_CLR,13,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes PACKP_V_INT_CLEAR           = {INT1_CLR,12,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes BAL_DONE_INT_CLEAR          = {INT1_CLR,11,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_INT_CLEAR         = {INT1_CLR,10,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes FSM_ERROR_INT_CLEAR         = {INT1_CLR,9,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_TEMP_INT_CLEAR     = {INT1_CLR,8,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MISMATCH_INT_CLEAR     = {INT1_CLR,6,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_INT_CLEAR         = {INT1_CLR,5,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_INT_CLEAR         = {INT1_CLR,4,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes VDD_INT_CLEAR               = {INT1_CLR,3,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_INT_CLEAR              = {INT1_CLR,2,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes REGIN_INT_CLEAR             = {INT1_CLR,1,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OTP_CRC_EVENT_INT_CLEAR     = {INT1_CLR,0,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes INT_ALERT_CTRL              = {INT0_EN,15,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes PACK_CURRENT_INT_EN         = {INT0_EN,14,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CONN_DET_INT_EN             = {INT0_EN,13,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CC_ACC_INT_EN               = {INT0_EN,12,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VSCAN_DONE_INT_EN           = {INT0_EN,11,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes AFE_MODE_CHANGE_INT_EN      = {INT0_EN,10,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes RECOVERED_INT_EN            = {INT0_EN,9,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes WDT_INT_EN                  = {INT0_EN,8,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_CHG_INT_EN              = {INT0_EN,7,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_DSG_INT_EN              = {INT0_EN,6,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SHORT_CURR_INT_EN           = {INT0_EN,5,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OVER_CURR_INT_EN            = {INT0_EN,4,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_INT_EN              = {INT0_EN,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_INT_EN              = {INT0_EN,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_INT_EN              = {INT0_EN,1,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_INT_EN              = {INT0_EN,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes FET_DRIVER_INT_EN           = {INT1_EN,13,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes PACKP_V_INT_EN              = {INT1_EN,12,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes BAL_DONE_INT_EN             = {INT1_EN,11,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_INT_EN            = {INT1_EN,10,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes FSM_ERROR_INT_EN            = {INT1_EN,9,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_TEMP_INT_EN        = {INT1_EN,8,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_INT_EN             = {INT1_EN,7,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_MISMATCH_INT_EN        = {INT1_EN,6,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_INT_EN            = {INT1_EN,5,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_INT_EN            = {INT1_EN,4,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VDD_INT_EN                  = {INT1_EN,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes V3V3_INT_EN                 = {INT1_EN,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes REGIN_INT_EN                = {INT1_EN,1,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OTP_CRC_EVENT_INT_EN        = {INT1_EN,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_CHG_INT_TYPE            = {INT_TYPE0,6,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_DSG_INT_TYPE            = {INT_TYPE0,4,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_INT_TYPE            = {INT_TYPE0,2,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_INT_TYPE            = {INT_TYPE0,0,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VDD_INT_TYPE                = {INT_TYPE1,12,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes V3V3_INT_TYPE               = {INT_TYPE1,10,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes REGIN_INT_TYPE              = {INT_TYPE1,8,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_INT_TYPE          = {INT_TYPE2,10,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_TEMP_INT_TYPE      = {INT_TYPE2,6,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_INT_TYPE           = {INT_TYPE2,4,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_MISMATCH_INT_TYPE      = {INT_TYPE2,2,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_INT_TYPE          = {INT_TYPE2,0,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_CHG_MASK           = {MASK_INT0,7,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_DSG_MASK           = {MASK_INT0,6,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SC_MASK                     = {MASK_INT0,5,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OVER_CURR_MASK              = {MASK_INT0,4,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_MASK                = {MASK_INT0,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_MASK                = {MASK_INT0,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_MASK                = {MASK_INT0,1,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_MASK                = {MASK_INT0,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_HOT_MASK           = {MASK_INT1,8,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_DIG_MASK           = {MASK_INT1,7,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_MASK              = {MASK_INT1,6,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes VDD_MASK                    = {MASK_INT1,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes V3V3_MASK                   = {MASK_INT1,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes REGIN_MASK                  = {MASK_INT1,1,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_MASK              = {MASK_INT1,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_RT_STS               = {OC_STATUS,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC2_DCHG_RT_STS             = {OC_STATUS,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC1_DCHG_RT_STS             = {OC_STATUS,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_STS                  = {OC_STATUS,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC2_DCHG_STS                = {OC_STATUS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC1_DCHG_STS                = {OC_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_INT_TYPE             = {OCFT_CTRL,14,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OC_DCHG_INT_TYPE            = {OCFT_CTRL,12,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_FAULT_EN             = {OCFT_CTRL,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OC2_DCHG_FAULT_EN           = {OCFT_CTRL,7,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OC1_DCHG_FAULT_EN           = {OCFT_CTRL,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OC_CHG_INT_EN               = {OCFT_CTRL,5,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OC2_DCHG_INT_EN             = {OCFT_CTRL,4,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OC1_DCHG_INT_EN             = {OCFT_CTRL,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_EN_CTRL              = {OCFT_CTRL,2,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC2_DCHG_EN_CTRL            = {OCFT_CTRL,1,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC1_DCHG_EN_CTRL            = {OCFT_CTRL,0,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC2_DCHG_RNG                = {DSGOC_LIM,13,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC2_DCHG_LIM                = {DSGOC_LIM,8,5,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC1_DCHG_RNG                = {DSGOC_LIM,5,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC1_DCHG_LIM                = {DSGOC_LIM,0,5,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC2_DCHG_DGL_RNG            = {DSGOC_DEG,14,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC2_DCHG_DGL                = {DSGOC_DEG,8,6,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC1_DCHG_DGL_RNG            = {DSGOC_DEG,6,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC1_DCHG_DGL                = {DSGOC_DEG,0,6,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC_CHG_DGL_RNG              = {CHGOC_DEG,14,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC_CHG_DG                   = {CHGOC_DEG,8,6,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC_CHG_RNG                  = {CHGOC_DEG,5,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes OC_CHG_LIM                  = {CHGOC_DEG,0,5,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_CHG_RT_STS               = {SC_STATUS,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_DCHG_RT_STS              = {SC_STATUS,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_CHG_STS                  = {SC_STATUS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_DCHG_STS                 = {SC_STATUS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_CHG_FAULT_EN             = {SCFT_CTRL,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SC_DCHG_FAULT_EN            = {SCFT_CTRL,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SC_CHG_INT_EN               = {SCFT_CTRL,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SC_DCHG_INT_EN              = {SCFT_CTRL,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes SC_CHG_EN_CTRL              = {SCFT_CTRL,1,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_DCHG_EN_CTRL             = {SCFT_CTRL,0,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_DCHG_DG                  = {DSGSC_CFG,8,7,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_DCHG_RNG                 = {DSGSC_CFG,5,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_DCHG_LIM                 = {DSGSC_CFG,0,5,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_CHG_DG                   = {CHGSC_CFG,8,7,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_CHG_RNG                  = {CHGSC_CFG,5,1,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SC_CHG_LIM                  = {CHGSC_CFG,0,5,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes CELL_10_UV_STS              = {RD_CELL_UV,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_UV_STS               = {RD_CELL_UV,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_UV_STS               = {RD_CELL_UV,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_UV_STS               = {RD_CELL_UV,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_UV_STS               = {RD_CELL_UV,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_UV_STS               = {RD_CELL_UV,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_UV_STS               = {RD_CELL_UV,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_UV_STS               = {RD_CELL_UV,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_UV_STS               = {RD_CELL_UV,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_UV_STS               = {RD_CELL_UV,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_OV_STS              = {RD_CELL_OV,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_OV_STS               = {RD_CELL_OV,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_OV_STS               = {RD_CELL_OV,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_OV_STS               = {RD_CELL_OV,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_OV_STS               = {RD_CELL_OV,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_OV_STS               = {RD_CELL_OV,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_OV_STS               = {RD_CELL_OV,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_OV_STS               = {RD_CELL_OV,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_OV_STS               = {RD_CELL_OV,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_OV_STS               = {RD_CELL_OV,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_MSMT_STS            = {RD_CELL_MSMT,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_MSMT_STS             = {RD_CELL_MSMT,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_MSMT_STS             = {RD_CELL_MSMT,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_MSMT_STS             = {RD_CELL_MSMT,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_MSMT_STS             = {RD_CELL_MSMT,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_MSMT_STS             = {RD_CELL_MSMT,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_MSMT_STS             = {RD_CELL_MSMT,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_MSMT_STS             = {RD_CELL_MSMT,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_MSMT_STS             = {RD_CELL_MSMT,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_MSMT_STS             = {RD_CELL_MSMT,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_DEAD_STS            = {RD_CELL_DEAD,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_DEAD_STS             = {RD_CELL_DEAD,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_DEAD_STS             = {RD_CELL_DEAD,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_DEAD_STS             = {RD_CELL_DEAD,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_DEAD_STS             = {RD_CELL_DEAD,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_DEAD_STS             = {RD_CELL_DEAD,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_DEAD_STS             = {RD_CELL_DEAD,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_DEAD_STS             = {RD_CELL_DEAD,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_DEAD_STS             = {RD_CELL_DEAD,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_DEAD_STS             = {RD_CELL_DEAD,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_RT_STS            = {CELL_MSMT_STS,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_LOWER             = {CELL_MSMT_STS,10,4,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_DELTA             = {CELL_MSMT_STS,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_DET_CLEAR         = {PACKFT_CTRL,15,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_LOG_STS           = {PACKFT_CTRL,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_FAULT_EN          = {PACKFT_CTRL,12,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_MSMT_EN                = {PACKFT_CTRL,11,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_DEAD_STS_SEL           = {PACKFT_CTRL,10,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_FAULT_EN          = {PACKFT_CTRL,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_DEAD_EN                = {PACKFT_CTRL,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_OV_FAULT_EN_CTRL       = {PACKFT_CTRL,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_OV_EN_CTRL             = {PACKFT_CTRL,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_UV_FAULT_EN_CTRL       = {PACKFT_CTRL,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_UV_EN_CTRL             = {PACKFT_CTRL,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV_STS_SEL             = {CELLFT_CTRL,12,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_STS_SEL             = {CELLFT_CTRL,11,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_INT_TYPE            = {CELLFT_CTRL,9,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_INT_TYPE            = {CELLFT_CTRL,7,2,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_FAULT_EN            = {CELLFT_CTRL,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV_EN_CTRL             = {CELLFT_CTRL,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_FAULT_EN            = {CELLFT_CTRL,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_EN_CTRL             = {CELLFT_CTRL,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SYNC_RT_STATUS              = {CELLFT_CTRL,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_HYST                = {CELL_HYST,8,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_HYST                = {CELL_HYST,4,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_OV_HYST                = {PACK_UV_OV,10,6,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes VTOP_OV_RT_STS              = {PACK_UV_OV,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_HYST                = {PACK_UV_OV,2,6,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes VTOP_UV_RT_STS              = {PACK_UV_OV,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_DG                  = {CELL_UV_X,8,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV                     = {CELL_UV_X,0,8,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes CELL_OV_DG                  = {CELL_OV_X,8,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV                     = {CELL_OV_X,0,8,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes VTOP_UV_DG                  = {PACK_UV,12,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_UV_LIMIT               = {PACK_UV,0,12,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes VTOP_OV_DG                  = {PACK_OV,12,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_OV_LIMIT               = {PACK_OV,0,12,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes CELL_DEAD_DGL_N             = {CELL_DEAD_THR,7,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_DEAD_LIMIT             = {CELL_DEAD_THR,0,7,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes CELL_MSMT_DGL_N             = {CELL_MSMT,5,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes MSMT_TH                     = {CELL_MSMT,0,5,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DIE_TEMP_DIG_STS            = {RD_NTC_DIE,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_DIG_RT_STS         = {RD_NTC_DIE,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_PCB_MNTR_HOT_STS       = {RD_NTC_DIE,11,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_PCB_MNTR_HOT_STS       = {RD_NTC_DIE,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_PCB_MNTR_HOT_STS       = {RD_NTC_DIE,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_PCB_MNTR_HOT_STS       = {RD_NTC_DIE,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_CELL_DSG_STS           = {RD_NTC_DIE,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_CELL_DSG_STS           = {RD_NTC_DIE,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_CELL_DSG_STS           = {RD_NTC_DIE,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_CELL_DSG_STS           = {RD_NTC_DIE,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_CELL_CHG_STS           = {RD_NTC_DIE,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_CELL_CHG_STS           = {RD_NTC_DIE,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_CELL_CHG_STS           = {RD_NTC_DIE,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_CELL_CHG_STS           = {RD_NTC_DIE,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_CELL_DSG_RT_STS        = {RD_V_NTC4_LR,15,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_CELL_DSG_RT_STS        = {RD_V_NTC4_LR,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_CELL_DSG_RT_STS        = {RD_V_NTC4_LR,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_CELL_DSG_RT_STS        = {RD_V_NTC4_LR,12,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_VALUE                  = {RD_V_NTC4_LR,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_CELL_CHG_RT_STS        = {RD_V_NTC3_LR,15,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_CELL_CHG_RT_STS        = {RD_V_NTC3_LR,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_CELL_CHG_RT_STS        = {RD_V_NTC3_LR,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_CELL_CHG_RT_STS        = {RD_V_NTC3_LR,12,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_VALUE                  = {RD_V_NTC3_LR,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_VALUE                  = {RD_V_NTC2_LR,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_VALUE                  = {RD_V_NTC1_LR,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_V                  = {RD_T_DIE,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_DIG_CLEAR          = {NTC_CLR,14,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_STS_SEL            = {NTC_CLR,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_DIG_FAULT_EN       = {DIE_CFG,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DIE_TEMP_DIG_EN             = {DIE_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes PCB_MNTR_FAULT_EN           = {NTC_CFG,15,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_DSG_FAULT_EN       = {NTC_CFG,14,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_CHG_FAULT_EN       = {NTC_CFG,13,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTCB_DYNAMIC_ON             = {NTC_CFG,10,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC4_PD_EN                  = {NTC_CFG,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC3_PD_EN                  = {NTC_CFG,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC4_TYPE_SEL               = {NTC_CFG,7,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC4_EN                     = {NTC_CFG,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC3_TYPE_SEL               = {NTC_CFG,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC3_EN                     = {NTC_CFG,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC2_TYPE_SEL               = {NTC_CFG,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC2_EN                     = {NTC_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC1_TYPE_SEL               = {NTC_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC1_EN                     = {NTC_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_HOT_DSG            = {NTCC_OTHR_DSG,0,10,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes NTC_CELL_COLD_DSG           = {NTCC_UTHR_DSG,0,10,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes NTC_CELL_HOT_CHG            = {NTCC_OTHR_CHG,0,10,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes NTC_CELL_HYST               = {NTCC_UTHR_CHG,11,5,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_COLD_CHG           = {NTCC_UTHR_CHG,0,10,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes PCB_MNTR_HYST               = {NTCM_OTHR,11,5,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes PCB_MNTR_HOT                = {NTCM_OTHR,0,10,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DIE_TEMP_HYST               = {DIE_OT,11,5,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes DIE_TEMP_HOT                = {DIE_OT,0,10,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NVM_CRC_OUTCOME             = {SELF_STS,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NVMCHK_DONE_STS             = {SELF_STS,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VDD_STS                     = {SELF_STS,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_STS                    = {SELF_STS,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes REGIN_STS                   = {SELF_STS,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_STS_OV            = {SELF_STS,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_STS_UV            = {SELF_STS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VDD_VALUE                   = {RD_VA1P8,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_VALUE                  = {RD_VA3P3,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes REGIN_VALUE                 = {RD_VA5,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_VALUE             = {RD_VASELF,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_OPW_STS             = {RD_OPENH,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_OPW_STS              = {RD_OPENH,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_OPW_STS              = {RD_OPENH,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_OPW_STS              = {RD_OPENH,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_OPW_STS              = {RD_OPENH,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_OPW_STS              = {RD_OPENH,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_OPW_STS              = {RD_OPENH,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_OPW_STS              = {RD_OPENH,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_OPW_STS              = {RD_OPENH,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_OPW_STS              = {RD_OPENH,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_0_OPW_STS              = {RD_OPENH,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_ERR_STS           = {SFT_GO,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_DONE_STS          = {SFT_GO,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_GO                = {SFT_GO,8,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NVM_CRC_DO                  = {SFT_GO,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NVM_FAULT_EN                = {SELF_CFG,15,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes V3V3_VDD_FAULT_EN           = {SELF_CFG,14,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OPEN_WIRE_PON               = {SELF_CFG,10,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OPEN_WIRE_FAULT_EN          = {SELF_CFG,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NVM_CRC_EN                  = {SELF_CFG,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes ADC_SELF_TEST_EN            = {SELF_CFG,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VDD_EN                      = {SELF_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes V3V3_EN                     = {SELF_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes REGIN_EN                    = {SELF_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OPW_CHECK_TH                = {OPEN_CFG,8,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes OPW_CHECK_LEN               = {OPEN_CFG,0,4,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes REGIN_UV_LIMIT              = {REGIN_UV,0,9,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_UV_LIMIT               = {V3P3_UV,0,8,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VDD_UV_LIMIT                = {VDD_UV,0,8,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_OV_LIMIT          = {SELF_THR,8,8,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SELF_TEST_UV_LIMIT          = {SELF_THR,0,8,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_FAULT_STS          = {FT_STS1,15,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_CHG_FAULT_STS            = {FT_STS1,14,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_DCHG_FAULT_STS           = {FT_STS1,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_FAULT_STS            = {FT_STS1,12,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC2_DCHG_FAULT_STS          = {FT_STS1,11,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC1_DCHG_FAULT_STS          = {FT_STS1,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_DSG_FAULT_STS      = {FT_STS1,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_CHG_FAULT_STS      = {FT_STS1,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_FAULT_STS          = {FT_STS1,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_FAULT_STS           = {FT_STS1,6,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_FAULT_STS           = {FT_STS1,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_FAULT_STS         = {FT_STS1,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_FAULT_STS         = {FT_STS1,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_FAULT_STS         = {FT_STS1,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_FAULT_STS           = {FT_STS1,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_FAULT_STS           = {FT_STS1,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes RMVL_BUSY                   = {FT_STS2,13,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DRIVER_FAULT_STS            = {FT_STS2,3,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VDD_FAULT_STS               = {FT_STS2,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_FAULT_STS              = {FT_STS2,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes OTP_CRC_FAULT_STS           = {FT_STS2,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_FAULT_CLR          = {FT_CLR,15,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_CHG_FAULT_CLR            = {FT_CLR,14,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes SC_DCHG_FAULT_CLR           = {FT_CLR,13,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC_CHG_FAULT_CLR            = {FT_CLR,12,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC2_DCHG_FAULT_CLR          = {FT_CLR,11,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OC1_DCHG_FAULT_CLR          = {FT_CLR,10,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_DSG_FAULT_CLR      = {FT_CLR,9,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_CHG_FAULT_CLR      = {FT_CLR,8,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes PCB_MNTR_FAULT_CLR          = {FT_CLR,7,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_OV_FAULT_CLR           = {FT_CLR,6,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_UV_FAULT_CLR           = {FT_CLR,5,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OPEN_WIRE_FAULT_CLR         = {FT_CLR,4,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_MSMT_FAULT_CLR         = {FT_CLR,3,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_DEAD_FAULT_CLR         = {FT_CLR,2,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_OV_FAULT_CLR           = {FT_CLR,1,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_UV_FAULT_CLR           = {FT_CLR,0,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_VDD_FAULT_CLR          = {FT_REC,15,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes OTP_CRC_CLR                 = {FT_REC,14,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes DRIVER_FAULT_CLR            = {FT_REC,13,1,RWAccess::WRITE_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_TEMP_FAULT_REC          = {FT_REC,12,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes PCB_MNTR_REC                = {FT_REC,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CHG_HYS_MODE            = {FT_REC,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_CHG_REC            = {FT_REC,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_DSG_REC            = {FT_REC,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV_REC                 = {FT0_CFG,11,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV_FAULT_ACTION        = {FT0_CFG,10,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV_LOGIC_SEL           = {FT0_CFG,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_OV_CHG_REC_MODE        = {FT0_CFG,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_REC                 = {FT0_CFG,7,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_FAULT_ACTION        = {FT0_CFG,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_LOGIC_SEL           = {FT0_CFG,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_UV_DSG_REC_MODE        = {FT0_CFG,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC_CELL_CHG_REC_MODE       = {FT0_CFG,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_STBY_MODE          = {FT0_CFG,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes NTC_CELL_CHG_ACTION         = {FT0_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCOC_DET_TIME               = {FT1_CFG,2,2,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes SCOC_PUP                    = {FT1_CFG,0,2,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes CC_ACC_LSBS                 = {RD_CCIRQL,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_ACC_MSBS                 = {RD_CCIRQH,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_RT_ACC_LSBS              = {RD_CCACCQL,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_RT_ACC_MSBS              = {RD_CCACCQH,0,10,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VPACK_V                     = {RD_VPACKP,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_V                      = {RD_VTOP,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VTOP_I                      = {RD_ITOP,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_V                    = {RD_VCELL1,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_1_I_SYNC               = {RD_ICELL1,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_V                    = {RD_VCELL2,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_2_I_SYNC               = {RD_ICELL2,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_V                    = {RD_VCELL3,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_3_I_SYNC               = {RD_ICELL3,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_V                    = {RD_VCELL4,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_4_I_SYNC               = {RD_ICELL4,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_V                    = {RD_VCELL5,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_5_I_SYNC               = {RD_ICELL5,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_V                    = {RD_VCELL6,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_6_I_SYNC               = {RD_ICELL6,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_V                    = {RD_VCELL7,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_7_I_SYNC               = {RD_ICELL7,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_V                    = {RD_VCELL8,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_8_I_SYNC               = {RD_ICELL8,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_V                    = {RD_VCELL9,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_9_I_SYNC               = {RD_ICELL9,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_V                   = {RD_VCELL10,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_I_SYNC              = {RD_ICELL10,0,16,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC4_HIRES_V                = {RD_VNTC4,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC3_HIRES_V                = {RD_VNTC3,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC2_HIRES_V                = {RD_VNTC2,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes NTC1_HIRES_V                = {RD_VNTC1,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO3_VOLTAGE               = {RD_VGPIO3,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO2_VOLTAGE               = {RD_VGPIO2,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes GPIO1_VOLTAGE               = {RD_VGPIO1,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes DIE_T_VOLTAGE               = {RD_TDIE,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VDD_VOLTAGE                 = {RD_V1P8,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes V3V3_VOLTAGE                = {RD_V3P3,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes REGIN_VOLTAGE               = {RD_V5,0,15,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_STATUS                   = {CC_STS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VADC_FSM                    = {ADC_STS,11,2,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes VADC_SCHEDULER              = {ADC_STS,8,2,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SCAN_ERROR_STS              = {ADC_CTRL,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes SCAN_DONE_STS               = {ADC_CTRL,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes ADC_SCAN_GO                 = {ADC_CTRL,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CC_B2B_ACC_CTRL             = {CC_CFG,14,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CC_INT_CNT                  = {CC_CFG,8,6,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CC_ERROR_STS                = {CC_CFG,5,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_DONE                     = {CC_CFG,4,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CC_PWR_SAVE                 = {CC_CFG,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CC_EN_SAFE                  = {CC_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CC_EN                       = {CC_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes I_PCB_GAIN_VALUE            = {TRIMG_IPCB,0,10,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes CELL_1K_COMP                = {HR_SCAN0,11,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes VTOP_SYNC_EN                = {HR_SCAN0,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_SYNC_EN                = {HR_SCAN0,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_LDOS_EN                = {HR_SCAN0,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_DIE_T                  = {HR_SCAN0,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_NTCS_EN                = {HR_SCAN0,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_GPIO_EN                = {HR_SCAN0,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_PACKP_EN               = {HR_SCAN0,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_VTOP_EN                = {HR_SCAN0,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes SCAN_VCELLS_EN              = {HR_SCAN0,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_10_VI_READ_EN          = {HR_SCAN1,9,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_9_VI_READ_EN           = {HR_SCAN1,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_8_VI_READ_EN           = {HR_SCAN1,7,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_7_VI_READ_EN           = {HR_SCAN1,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_6_VI_READ_EN           = {HR_SCAN1,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_5_VI_READ_EN           = {HR_SCAN1,4,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_4_VI_READ_EN           = {HR_SCAN1,3,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_3_VI_READ_EN           = {HR_SCAN1,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_2_VI_READ_EN           = {HR_SCAN1,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes CELL_1_VI_READ_EN           = {HR_SCAN1,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC4_READ_EN                = {HR_SCAN2,8,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC3_READ_EN                = {HR_SCAN2,7,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC2_READ_EN                = {HR_SCAN2,6,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes NTC1_READ_EN                = {HR_SCAN2,5,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO3_READ_EN               = {HR_SCAN2,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO2_READ_EN               = {HR_SCAN2,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes GPIO1_READ_EN               = {HR_SCAN2,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes XFR_NUM_RD_WORDS            = {SILC_INFO1,0,6,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes DEVICE_ADD                  = {COMM_CFG,8,7,RWAccess::READ_WRITE,NVMType::MTP};
    constexpr RegDes USE_COMM_CRC                = {COMM_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes AUTO_BAL_SKIPPED_HOT        = {BAL_STS,10,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes AUTO_BAL_SKIPPED_DISCHARGE  = {BAL_STS,9,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes AUTO_BAL_SKIPPED_STANDBY    = {BAL_STS,8,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes AUTO_BAL_SKIPPED_CHARGE     = {BAL_STS,7,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes AUTO_BALANCING_COUNT_STS    = {BAL_STS,1,6,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes BALANCING_ACTIVE            = {BAL_STS,0,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes CELL_10_TO_BALANCE          = {BAL_LIST,9,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_9_TO_BALANCE           = {BAL_LIST,8,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_8_TO_BALANCE           = {BAL_LIST,7,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_7_TO_BALANCE           = {BAL_LIST,6,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_6_TO_BALANCE           = {BAL_LIST,5,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_5_TO_BALANCE           = {BAL_LIST,4,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_4_TO_BALANCE           = {BAL_LIST,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_3_TO_BALANCE           = {BAL_LIST,2,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_2_TO_BALANCE           = {BAL_LIST,1,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_1_TO_BALANCE           = {BAL_LIST,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes BAL_ERROR_STS               = {BAL_CTRL,2,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes BAL_DONE_STS                = {BAL_CTRL,1,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes BALANCE_GO                  = {BAL_CTRL,0,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes BAL_REPETITION              = {BAL_CFG,3,5,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes AUTO_BAL_ALWAYS             = {BAL_CFG,2,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes BALANCE_MODE_REG            = {BAL_CFG,1,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes BALANCE_MODE_CTRL           = {BAL_CFG,0,1,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes BAL_MSM_TH                  = {BAL_THR,10,3,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes STOP_ON_HOT                 = {BAL_THR,9,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes ABAL_ON_STBY                = {BAL_THR,7,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes ABAL_ON_CHARGE              = {BAL_THR,6,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_BAL_MIN                = {BAL_THR,0,6,RWAccess::READ_WRITE,NVMType::OTP};
    constexpr RegDes MTP_STATUS                  = {MEM_STATUS,3,3,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes STORE_IN_PROGRESS           = {OTP_STORE_CMD,15,1,RWAccess::READ_ONLY,NVMType::VOLATILE};
    constexpr RegDes STORE_NVM_CMD               = {OTP_STORE_CMD,3,1,RWAccess::READ_WRITE,NVMType::VOLATILE};
    constexpr RegDes CELL_CMD_ACCESS_CODE        = {STORE_CMD_ACCESS_CODE_X,0,16,RWAccess::READ_WRITE,NVMType::VOLATILE};
}



// Estructura: si nRegs==0 o regs==nullptr, no hay llamadas a mp2790()
struct Interrupt {
  const char* name;
  const RegDes* regs;   // array de MP2790::<...> o nullptr
  uint8_t nVals;     // número de registros en 'regs'
};

// ===== Table 11-12: Main Interrupts (RD_INT0-RD_INT1) =====
// Conjuntos de registros relacionados (columna "Related Status Register(s)")
static const RegDes RD_CELL_OV_REGS      = { RD_CELL_OV         ,0  ,10};
static const RegDes RD_CELL_UV_REGS      = { RD_CELL_UV         ,0  ,10};
static const RegDes OC_REGS              = { OC_STATUS          ,0  ,3 };
static const RegDes SC_REGS              = { SC_STATUS          ,0  ,2 };
static const RegDes NTC_DSG_REGS         = { RD_NTC_DIE         ,0  ,4 };
static const RegDes NTC_CHG_REGS         = { RD_NTC_DIE         ,4  ,4 };
static const RegDes WDT_REGS             = { WDT_STATUS         ,0  ,2 };
static const RegDes CONN_DET_REGS        = { LOAD_CHARGER_STATUS,12 ,4 };
static const RegDes PACK_CURRENT_REGS    = { PWR_STATUS         ,7  ,3 };
static const RegDes OW_REGS              = { RD_OPENH           ,0  ,11 };
static const RegDes DEAD_REGS            = { RD_CELL_DEAD       ,0  ,10 };
static const RegDes MISMATCH_REG         = { RD_CELL_MSMT       ,0  ,10 };
static const RegDes NTC_OT_REG           = { RD_NTC_DIE         ,8  ,4 };
static const RegDes SELF_REG             = { SELF_STS           ,0  ,2 };

static const Interrupt InterruptTable[] = {

  { "00_Cell OV",              &RD_CELL_OV_REGS        ,RD_CELL_OV_REGS.len }, //#cell in OV
  { "01_Cell UV",              &RD_CELL_UV_REGS        ,RD_CELL_UV_REGS.len},  //#cell in UV
  { "02_Stack TOP OV",         nullptr                 ,0 },
  { "03_Stack TOP UV",         nullptr                 ,0 },
  { "04_OC",                   &OC_REGS                ,OC_REGS.len },         //OC DSG1, OC_DSG2, OC CHG
  { "05_SC",                   &SC_REGS                ,SC_REGS.len },         //SC DSG, SC CHG
  { "06_NTC DSG",              &NTC_DSG_REGS           ,NTC_DSG_REGS.len },    //NTC 1-4
  { "07_NTC CHG",              &NTC_CHG_REGS           ,NTC_CHG_REGS.len },    //NTC 1-4
  { "08_WDT Barked",           &WDT_REGS               ,WDT_REGS.len },        //Bark, Bite
  { "09_Fault recovered",      nullptr                 ,0 },
  { "10_AFE mode change",      &MP2790_Reg::PWR_STATE  ,1 },                   //0xi & {nA, nC, nB SBY, Safe}
  { "11_Scan complete",        nullptr                 ,0 },
  { "12_CC done",              nullptr                 ,0 },
  { "13_Plug-in detection",    &CONN_DET_REGS          ,4 },                   //LDDET_FAIL, CHGDET_FAIL, LD_IN, CHG_IN }
  { "14_Pack current",         &PACK_CURRENT_REGS      ,1 },                   //0xi & {Charging, Standby, Discharging}
  { "15_Nothing - Reserved"    ,nullptr                ,0 },                   //Reserved value, not meaningful
  { "16_CRC check done/OTP not match"  ,nullptr        ,0 },
  { "17_REGIN UV",             nullptr                 ,0 },
  { "18_3V3 UV",               nullptr                 ,0 },
  { "19_VDD UV",               nullptr                 ,0 },
  { "20_Open wire",            &OW_REGS                ,OW_REGS.len },         //C# with OW //RD_OPENL not found
  { "21_Dead cell",            &DEAD_REGS              ,DEAD_REGS.len },       //#cell dead
  { "22_Cell Mismatch",        &MISMATCH_REG           ,MISMATCH_REG.len },    //#cell mismatch
  { "23_Die Over Temperature", nullptr                 ,0 },
  { "24_NTC in PCB mode Over Temperature",&NTC_OT_REG  ,4 },                   //NTC 1-4
  { "25_Scheduler error",      nullptr                 ,0 },
  { "26_Self test failed",     &SELF_REG               ,2 },                   //Self test UV, Self test OV
  { "27_Balancing complete",   nullptr                 ,0 },                   //Check!!!
  { "28_PackP comparator changed", nullptr             ,1 },                   //0-4: {
  //Not available, VpackP<Vtop-1, Vtop+160mV>VpackP>Vtop-1, PackP comp invalid, VpackP>Vtop+208mv}
  { "29_Fet driver timeout",   &MP2790_Reg::PWR_STATE  ,1 },                   
  // 0: OC during soft start or SC before DSG turn-on //1: FET timeout
};
static const size_t M_InterruptTable = sizeof(InterruptTable)/sizeof(InterruptTable[0]);

static const Interrupt FaultTable[] = {
    {"00_Cell UV"                      ,nullptr                ,0 },
    {"01_Cell OV"                      ,nullptr                ,0 },
    {"02_Cell Dead"                    ,nullptr                ,0 },
    {"03_Cell Mismatch"                ,nullptr                ,0 },
    {"04_Open Wire"                    ,nullptr                ,0 },
    {"05_Vtop UV"                      ,nullptr                ,0 },
    {"06_Vtop OV"                      ,nullptr                ,0 },
    {"07_PCB NTC"                      ,nullptr                ,0 },
    {"08_Cell CHG NTC"                 ,nullptr                ,0 },
    {"09_Cell DSG NTC"                 ,nullptr                ,0 },
    {"10_OC1 DSG"                      ,nullptr                ,0 },
    {"11_OC2 DSG"                      ,nullptr                ,0 },
    {"12_OC CHG"                       ,nullptr                ,0 },
    {"13_SC DSG"                       ,nullptr                ,0 },
    {"14_SC CHG"                       ,nullptr                ,0 },
    {"15_DIE temp"                     ,nullptr                ,0 },
    {"16_OTP CRC"                      ,nullptr                ,0 },
    {"17_3V3 UV"                       ,nullptr                ,0 },
    {"18_VDD UV"                       ,nullptr                ,0 },
    {"19_Driver turn on"               ,nullptr                ,0 },
};


/**
 * @brief Main MP2790 device driver class.
 */
class MP2790 {
public:
    MP2790(uint8_t address = MP27XX_DEFAULT_ADDRESS, uint8_t cells = MP27XX_DEFAULT_CELLS, void *wireObj = 0);

    bool testConnection();
    void setAddress(uint8_t newAddress);
    uint8_t getAddress();
    
    uint16_t readAdd(RegDes reg);    
    uint16_t readAdd(uint8_t regNum);
    bool writeAdd(RegDes reg, uint16_t value);
    bool writeAdd(uint8_t regNum, uint16_t value);
    // uint16_t readAdd(uint8_t regNum);
    // bool writeAdd(uint8_t regNum, uint16_t value);
    
    bool storeNVM();
    void storeAccessCode();
    uint8_t powerStatus();
    void currentDirection();

    //Register methods
        //Main register
    void getFault();
    void clearFT(uint8_t ftNum); 
    bool getInt(bool* intFlags);
    void clearInt(uint8_t intNum); 

    bool getCellOVUV(bool* cellOVflags, bool* cellUVflags);
    // bool getCellAlert(bool* cellAlertNum, uint8_t* cellAlertType);
    bool getCellAlert(bool* cellMismatchFlags, bool* cellDeadFlags, bool* cellOpenWireFlags);
    void getNTCreadings(uint16_t* ntcValues);               //For NTC 1-4
    bool getNTCHotCold(bool* ntcHotColdDSGFlags, bool* ntcHotColdCHGFlags);
    uint16_t getDieTemperature();                           //Die temperature
    void getADCReadings(uint16_t* adcValues);               //ADC readings
    //HR ADC readings
    bool initHR();

    uint16_t getPackPReadings();
    uint16_t getVTopReadings();
    int16_t getITopReadings();
    void getHRCellReadings(uint16_t* cellHRVoltage, int16_t* cellHRCurrent);
    void getHRNTCReadings(uint16_t* ntcHRValue);
    void getHRGPIOReadings(uint16_t* gpioHRValue);
    uint16_t getHRDieTemperature();
    void getHRADCReadings(uint16_t* adcHRValues);

    bool _scanInProgress = false;
    uint32_t _scanStartTime;

private:
    uint8_t _devAddr;
    void* _wireObj;
    uint8_t _nCells;
};

#endif // MP2790_HPP