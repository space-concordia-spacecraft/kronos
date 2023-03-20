#pragma once
#include "ks_error_codes.h"
#include "ks_i2c.h"

/**
 * This implementation is based on cubewheel
 */

#define OT_RWD_RESET 85

// REGISTERS
#define OT_RWD_REG_RESET					(uint8_t) 1
#define OT_RWD_REG_SET_REF_SPEED			(uint8_t) 2
#define OT_RWD_REG_SET_CMD_TORQUE			(uint8_t) 3
#define OT_RWD_REG_SET_MOTOR_STATE			(uint8_t) 7
#define OT_RWD_REG_SET_ENCODER_STATE		(uint8_t) 8
#define OT_RWD_REG_SET_HALL_STATE			(uint8_t) 9
#define OT_RWD_REG_SET_MOTOR_MODE			(uint8_t) 10
#define OT_RWD_REG_SET_BCKP_MODE			(uint8_t) 12
#define OT_RWD_REG_CLEAR_ERRORS				(uint8_t) 20

#define OT_RWD_REG_ID						(uint8_t) 128
#define OT_RWD_REG_ID_EXTENDED				(uint8_t) 129
#define OT_RWD_REG_WHEEL_STATUS				(uint8_t) 130
#define OT_RWD_REG_WHEEL_SPEED				(uint8_t) 133
#define OT_RWD_REG_WHEEL_REFERENCE			(uint8_t) 134
#define OT_RWD_REG_WHEEL_CURRENT			(uint8_t) 135
#define OT_RWD_REG_WHEEL_DATA				(uint8_t) 137
#define OT_RWD_REG_WHEEL_DATA_ADDITIONAL	(uint8_t) 138
#define OT_RWD_REG_PWM_GAIN					(uint8_t) 139
#define OT_RWD_REG_MAIN_GAIN				(uint8_t) 140
#define OT_RWD_REG_BACKUP_GAIN				(uint8_t) 141
#define OT_RWD_REG_FLAGS					(uint8_t) 145

using namespace kronos;

namespace kronos {
    struct ReactionWheelId{
        uint8_t NodeType;
        uint8_t InterfaceVersion;
        uint8_t FirmwareVersionMajor;
        uint8_t FirmwareVersionMinor;
        uint16_t RuntimeSeconds;
        uint16_t RuntimeMillis;
    }__attribute__((packed));

    struct ReactionWheelIdExtended {
        uint16_t SerialNumber;
        uint8_t I2CAddress;
        uint8_t CANAddress;
    }__attribute__((packed));

    struct ReactionWheelStatus {
        uint16_t RuntimeSeconds;
        uint16_t RuntimeMillis;
        uint16_t Reserved;
        uint8_t MotorControlMode;
        bool BackupModeState;
        bool MotorSwitchState;
        bool HallSensorState;
        bool EncoderSwitchState;
        bool ErrorFlag;
    }__attribute__((packed));

    struct ReactionWheelData {
        uint16_t WheelSpeed;
        uint16_t WheelReference;
        uint16_t WheelCurrent;
    }__attribute__((packed));

    struct ReactionWheelDataAdditional {
        uint16_t WheelDuty;
        uint16_t WheelSpeedBackup;
    }__attribute__((packed));

    struct ReactionWheelPwmGain {
        uint16_t MainGain;
        uint8_t MainGainMultiplier;
    }__attribute__((packed));

    struct ReactionWheelGainTelemetry {
        uint16_t IntegratorGain;
        uint8_t IntegratorGainMultiplier;
        uint16_t FeedbackGain;
        uint8_t FeedbackGainMultiplier;
    }__attribute__((packed));

    class ReactionWheel {
    public:
        void SetSpeed();
    private:
        uint8_t m_Address;
        KsI2c* m_Driver;

        template<class T>// pointers are fun
        KsResultType ReadRegister(uint8_t reg, T& buff) {
            m_Driver->SetSlaveAddr(m_Address, I2C_M_SEVEN);
            m_Driver->Read(reg, (uint8_t *) &buff, sizeof(T)); // TODO: add register method
            return ks_success;
        }

        template<class T>
        KsResultType WriteRegister(uint8_t reg, const T& buff) {
            m_Driver->SetSlaveAddr(m_Address, I2C_M_SEVEN);
            m_Driver->Write(reg, (uint8_t*) &buff, sizeof(T));
            return ks_success;
        }
    };
}
