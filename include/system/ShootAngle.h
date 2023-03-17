#ifndef SHOOTANGLE_H
#define SHOOTANGLE_H
#include <iostream>
#include <array>
#include "hard/Motor.h"
namespace shootangle
{
    class ShootAngle
    {
    public:
        // デフォルトコンストラクタ.
        ShootAngle(mymotor::MyMotor<1> &mt) : mt_(mt), stopFlug_(1) {}
        ShootAngle() = delete;

        // デストラクタ.
        ~ShootAngle() {}

        void selectAngle(const bool up, const bool down)
        {
            if (up)
            {
                mt_.setPwm(defaultPwm_ * stopFlug_, 0);
            }
            else if (down)
            {
                mt_.setPwm(defaultPwm_ * -1 * stopFlug_, 0);
            }
            else
            {
                mt_.setPwm(0, 0);
            }
        }

        const int16_t getAnglePwm() const
        {
            return mt_.getPwm(0);
        }

        void stopAngle(const bool data)
        {
            stopFlug_ = data ? 0 : 1;
            mt_.setPwm(0, 0);
        }

    private:
        // コピーコンストラクタ.

        mymotor::MyMotor<1> &mt_;
        bool stopFlug_;
        static constexpr int16_t defaultPwm_ = 3000;
    };
}
#endif