
#ifndef LACKSW_H
#define LACKSW_H
#include <iostream>
#include <array>
#include "SafePwm.h"
namespace lacksw
{
    class LackSw
    {
    public:
        // デフォルトコンストラクタ.
        LackSw(mymotor::MyMotor<1> &mt) : mt_(mt), stopFlug_(1), resetWaitPwm_(0) {}
        LackSw() = delete;
        // デストラクタ.
        ~LackSw() {}
        void selectMovePwm(const bool up, const bool down, bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            selectMovePwm(up, down, sw);
        }

        void selectMovePwm(const uint8_t data, bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            selectMovePwm(data, sw);
        }

        void selectMovePwm(const bool up, const bool down, bool (&sw)[2])
        {
            if (up)
            {
                mt_.setPwm(defaultPwm_ * stopFlug_ * !sw[0], 0);
            }
            else if (down)
            {
                mt_.setPwm(defaultPwm_ * -1 * stopFlug_ * !sw[1], 0);
            }
            else
            {
                mt_.setPwm(resetWaitPwm_, 0);
            }
        }
        void selectMovePwm(const uint8_t data, bool (&sw)[2])
        {
            switch (data)
            {
            case 1:
            {
                mt_.setPwm(defaultPwm_ * stopFlug_ * !sw[0], 0);
                break;
            }
            case 2:
            {
                mt_.setPwm(defaultPwm_ * -1 * stopFlug_ * !sw[1], 0);
                break;
            }
            default:
                mt_.setPwm(resetWaitPwm_, 0);
                break;
            }
        }

        void selectOwmMovePwm(const uint8_t data, const int16_t pwm, bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            selectOwmMovePwm(data, pwm, sw);
        }
        void selectOwmMovePwm(const uint8_t data, const int16_t pwm, bool (&sw)[2])
        {
            switch (data)
            {
            case 1:
            {
                mt_.setPwm(pwm * stopFlug_ * !sw[0], 0);
                break;
            }
            case 2:
            {
                mt_.setPwm(pwm * -1 * stopFlug_ * !sw[1], 0);
                break;
            }
            default:
                mt_.setPwm(resetWaitPwm_, 0);
                break;
            }
        }

        void selectOwmMovePwm(const uint8_t data, const int16_t uppwm, const int16_t downpwm, bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            selectOwmMovePwm(data, uppwm, downpwm, sw);
        }
        void selectOwmMovePwm(const uint8_t data, const int16_t uppwm, const int16_t downpwm, bool (&sw)[2])
        {
            switch (data)
            {
            case 1:
            {
                mt_.setPwm(uppwm * stopFlug_ * !sw[0], 0);
                break;
            }
            case 2:
            {
                mt_.setPwm(downpwm * -1 * stopFlug_ * !sw[1], 0);
                break;
            }
            default:
                mt_.setPwm(resetWaitPwm_, 0);
                break;
            }
        }

        void selectOwmMovePwm(const bool up, const bool down, const int16_t pwm, bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            selectOwmMovePwm(up, down, pwm, sw);
        }
        void selectOwmMovePwm(const bool up, const bool down, const int16_t pwm, bool (&sw)[2])
        {
            if (up)
            {
                mt_.setPwm(pwm * stopFlug_ * !sw[0], 0);
            }
            else if (down)
            {
                mt_.setPwm(pwm * -1 * stopFlug_ * !sw[1], 0);
            }
            else
            {
                mt_.setPwm(resetWaitPwm_, 0);
            }
        }

        const int16_t getMovePwm() const
        {
            return mt_.getPwm(0);
        }

        void stop(const bool data)
        {
            stopFlug_ = data ? 0 : 1;
            mt_.getPwm(0);
        }

        void resetWaitPwm(const int16_t pwm)
        {
            if (resetWaitPwm_ == pwm)
                return;
            resetWaitPwm_ = safe_.keepPwm(pwm);
        }

    private:
        safepwm::SafePwm safe_;
        // コピーコンストラクタ.
        mymotor::MyMotor<1> &mt_;
        bool stopFlug_;
        static constexpr int16_t defaultPwm_ = 3000;
        int16_t resetWaitPwm_;
    };
}
#endif