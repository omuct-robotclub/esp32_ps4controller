#ifndef SAFEPWM_H
#define SAFEPWM_H
#include <iostream>
namespace safepwm
{
    class SafePwm
    {
    public:
        // デフォルトコンストラクタ.
        SafePwm() {}

        // デストラクタ.
        ~SafePwm() {}

        static constexpr bool checkPwm(const int16_t pwm)
        {
            return minPwm_ < pwm && pwm < maxPwm_ ? 1 : 0;
        }

        static constexpr int16_t keepPwm(const int16_t pwm)
        {
            return minPwm_ > pwm ? minPwm_ : pwm > maxPwm_ ? maxPwm_
                                                           : pwm;
        }

    private:
        // コピーコンストラクタ.
        SafePwm(const SafePwm &other)
        {
        }
        static constexpr int16_t minPwm_ = -20000;
        static constexpr int16_t maxPwm_ = 20000;
    };
}
#endif