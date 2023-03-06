#ifndef MOTOR_BASE_H
#define MOTOR_BASE_H
#include <iostream>
namespace motorbase
{
    template <uint8_t N>
    class Motor
    {
    public:
        // デフォルトコンストラクタ.
        Motor() : pwm_{0} {}

        // デストラクタ.
        virtual ~Motor() {}

        // オブジェクトを複製する.
        virtual Motor *clone() const
        {
            return 0;
        }

        // 等価なオブジェクトか?
        virtual bool equals(const Motor &other) const
        {
            return (this == &other);
        }

        // アクセッサ.
        const int16_t getPwm(uint8_t len) const
        {
            return pwm_[len];
        }
        void setPwm(const int16_t pwm, uint8_t len)
        {
            if (pwm_[len] == pwm)
                return;

            pwm_[len] = pwm;
        }

        // virtual const uint8_t getId() const = 0;

    protected:
        // コピーコンストラクタ.
        Motor(const Motor &other);

    private:
        // 代入演算子.
        Motor &operator=(const Motor &other);
        int16_t pwm_[N];
    };
}
#endif