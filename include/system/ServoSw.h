
#ifndef SERVOSW_H
#define SERVOSW_H
#include <iostream>
#include <array>
#include "hard/Servo.h"
namespace serbosw
{
    class ServoSw
    {
    public:
        // デフォルトコンストラクタ.
        ServoSw(myservo::MyServo &servo, uint8_t maxRatio, uint8_t minRatio) : servo_(servo), stopFlug_(0), maxRatio_(maxRatio), minRatio_(minRatio) {}
        ServoSw() = delete;
        // デストラクタ.
        ~ServoSw() {}
        void selectMoveRatio(const uint8_t ratio, bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            selectMovePwm(ratio, sw);
        }

        void selectMovePwm(const uint8_t ratio, bool (&sw)[2])
        {
            if (sw[0])
            {
                maxRatio_ = ratio;
            }
            else if (sw[1])
            {
                minRatio_ = ratio;
            }
            nowRatio_ = ratio < maxRatio_ ? ratio : maxRatio_;
            nowRatio_ = ratio > minRatio_ ? ratio : minRatio_;
            if (!stopFlug_)
            {
                servo_.setRatio(nowRatio_);
            }
        }

        const int16_t getMovePwm() const
        {
            return servo_.getRatio();
        }

        void stop(const uint8_t ratio)
        {
            stopFlug_ = 1;
            servo_.setRatio(ratio);
        }

        void move()
        {
            stopFlug_ = 0;
        }

    private:
        // コピーコンストラクタ.
        myservo::MyServo &servo_;
        bool stopFlug_;
        uint8_t maxRatio_;
        uint8_t minRatio_;
        uint8_t nowRatio_;
        static constexpr int16_t defaultPwm_ = 3000;
    };
}
#endif