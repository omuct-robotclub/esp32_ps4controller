#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "Motor.h"
#include <iostream>
#include <array>
namespace motordriver
{

    class MotorDriver
    {
    public:
        MotorDriver() = delete;
        // 引数を1つとるコンストラクタ.
        explicit MotorDriver(const uint8_t canid) : canid_(canid), motorid_{0}, pwm_{0} { std::cout << "MyMotorDriverdone" << std::endl; }

        // デストラクタ.
        ~MotorDriver() {}

        // アクセッサ.
        template <std::size_t N>
        void setMotorparameter(const std::array<int16_t, N> mt)
        {
            static_assert(0 < N && N <= 4, "pwm is must up to 4 lengths");
            for (uint8_t i = 0; i < N; i++)
            {
                pwm_[i] = mt[i];
                motorid_[i] = i;
            }
            for (uint8_t i = N; i < 4; i++)
            {
                pwm_[i] = 0;
                motorid_[i] = i;
            }
        }

        const uint8_t getCanId() const
        {
            return canid_;
        }

        const uint8_t getMotorId(const uint8_t len) const
        {
            return motorid_[len];
        }
        const std::array<uint8_t, 4> getMotorIds() const
        {
            return motorid_;
        }

        const int16_t getPwm(const int16_t len) const
        {
            return pwm_[len];
        }
        const std::array<int16_t, 4> getPwms() const
        {
            return pwm_;
        }

    private:
        // コピーコンストラクタ.
        MotorDriver(const MotorDriver &other) {}
        uint8_t canid_;
        // uint8_t motorid_[4];
        // int16_t pwm_[4];
        std::array<uint8_t, 4> motorid_;
        std::array<int16_t, 4> pwm_;
    };
}
#endif