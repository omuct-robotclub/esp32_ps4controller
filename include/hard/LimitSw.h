#ifndef LIMITSW_H
#define LIMITSW_H

#include <iostream>
#include <array>
namespace limitsw
{
    template <std::size_t N>
    class LimitSw
    {
    public:
        // デフォルトコンストラクタ.
        LimitSw() : datas_{0}
        {
            static_assert(0 < N && N <= 5, "pwm is must up to 5 lengths");
        }

        // デストラクタ.
        ~LimitSw() {}

        // アクセッサ.

        void setSwData(const uint8_t datas)
        {
            for (uint8_t i = 0; i < N; i++)
            {
                datas_[i] = (datas >> i) & 0x1;
            }
        }

        const bool getSwData(uint8_t len) const
        {
            return datas_[len];
        }

        const std::array<bool, N> getSwDatas() const
        {
            return datas_;
        }

        // virtual const uint8_t getId() const = 0;

    private:
        // コピーコンストラクタ.
        LimitSw(const LimitSw &other) {}
        std::array<bool, N> datas_;
    };
}
#endif