#ifndef NOMALROTATION_H
#define NOMALROTATION_H
#include <iostream>
#include "SafePwm.h"
namespace normalrotation
{
    class NormalRotation
    {
    public:
        // デフォルトコンストラクタ.
        NormalRotation(mymotor::MyMotor<1> &mt) : mt_(mt), stopFlug_(1) {}
        NormalRotation() = delete;

        // デストラクタ.
        ~NormalRotation() {}

        void setRotationPwm(const bool data)
        {
            mt_.setPwm(data ? defaultPwm_ * stopFlug_ : 0, 0);
        }
        void setOwnRotationPwm(const int16_t pwm)
        {
            mt_.setPwm(pwm * stopFlug_, 0);
        }

        const int16_t getRotationPwm() const
        {
            return mt_.getPwm(0);
        }

        void stop(const bool data)
        {
            stopFlug_ = data ? 0 : 1;
            mt_.setPwm(0, 0);
        }

    private:
        // コピーコンストラクタ.
        mymotor::MyMotor<1> &mt_;
        bool stopFlug_;
        static constexpr int16_t defaultPwm_ = 8000;
    };
}
#endif