#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "Motor.h"
#include <iostream>
namespace motordriver
{

    class MotorDriver
    {
    public:
        // 引数を1つとるコンストラクタ.
        explicit MotorDriver(uint8_t canid) : canid_(0){};

        // デストラクタ.
        ~MotorDriver(){};

        // アクセッサ.
        const uint8_t getCanId() const
        {
            return canid_;
        }
        template <uint8_t N>
        void setMotorparameter(motorbase::Motor<N> &mt)
        {
            static_assert(0 < N && N <= 4, "pwm is must up to 4 lengths");
            for (uint8_t i = 0; i < N; i++)
            {
                pwm_[i] = mt.getPwm(i);
            }
            for (uint8_t i = N; i < 4; i++)
            {
                pwm_[i] = mt.getPwm(0);
            }
        }

        // 等価演算子.
        // bool operator==(const MotorDriver &other) const;
        // bool operator!=(const MotorDriver &other) const;

    private:
        // デフォルトコンストラクタ.
        MotorDriver();
        // コピーコンストラクタ.
        MotorDriver(const MotorDriver &other);
        // 代入演算子.
        MotorDriver &operator=(const MotorDriver &other);
        uint8_t canid_;
        int16_t pwm_[4];
    };
}
#endif