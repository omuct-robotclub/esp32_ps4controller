#ifndef MYMOTOR_H
#define MYMOTOR_H
#include <iostream>
#include <array>
#include <system/SafePwm.h>
namespace mymotor
{
    template <uint8_t N>
    class MyMotor
    {
    public:
        // デフォルトコンストラクタ.
        MyMotor() : pwm_{0} { std::cout << "MyMotordone" << std::endl; }

        // デストラクタ.
        ~MyMotor() {}

        // アクセッサ.

        void setPwm(const int16_t pwm, uint8_t len)
        {
            if (pwm_[len] == pwm)
                return;
            pwm_[len] = safe_.keepPwm(pwm);
        }
        void setPwms(const std::array<int16_t, N> &pwm)
        {
            for (std::size_t i = 0; i < N; i++)
            {
                pwm_[i] = safe_.keepPwm(pwm[i]);
            }
        }

        const int16_t getPwm(const uint8_t len) const
        {
            return pwm_[len];
        }
        const std::array<int16_t, N> getPwms() const
        {
            return pwm_;
        }

        const void addPwm(const int16_t pwm, uint8_t len)
        {
            pwm_[len] += pwm;
            pwm_[len] = safe_.keepPwm(pwm_[len]);
        }

        void addPwms(const std::array<int16_t, N> pwms)
        {
            for (uint8_t i = 0; i < N; i++)
            {
                pwm_[i] += pwms[i];
                pwm_[i] = safe_.keepPwm(pwm_[i]);
            }
        }

    protected:
        // コピーコンストラクタ.
        MyMotor(const MyMotor &other) {}
        safepwm::SafePwm safe_;

    private:
        std::array<int16_t, N> pwm_;
    };
}
#endif