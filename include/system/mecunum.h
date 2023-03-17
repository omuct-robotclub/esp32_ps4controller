#ifndef MECUNUM_H
#define MECUNUM_H
#include <iostream>
#include <array>
#include <cmath>
#include "hard/Motor.h"
namespace mecunum
{
    class Mecunum
    {
    public:
        // デフォルトコンストラクタ.
        Mecunum(mymotor::MyMotor<4> &mt) : mt_(mt), magnification_(40) {}
        Mecunum() = delete;
        // デストラクタ.
        ~Mecunum() {}

        void setVector(const int8_t x, const int8_t y)
        {
            int8_t x_ = std::abs(x) < 30 ? 0 : x;
            int8_t y_ = std::abs(y) < 30 ? 0 : y;
            mt_.setPwms(getAngle(x_, y_));
        }
        void selectRotation(const int8_t data)
        {
            int8_t check_ = 0;
            if (std ::abs((double)data) > 30)
            {
                check_ = data > 0 ? -1 : 1;
            }
            mt_.addPwms(getRotation(check_));
        }

        const std::array<int16_t, 4> getAngles() const
        {
            return mt_.getPwms();
        }

        void upPwm(uint8_t magnification)
        {
            magnification_ = magnification < 40 ? 40 : magnification;
        }

    private:
        mymotor::MyMotor<4> &mt_;
        // コピーコンストラクタ.
        // Mecunum(const Mecunum &other) {}
        std::array<int16_t, 4> getAngle(const int8_t x, const int8_t y)
        {
            std::array<int16_t, 4> defaultAngle_;
            defaultAngle_[0] = (x * rootover2_ * -1 + y * rootover2_) * magnification_;
            defaultAngle_[1] = (x * rootover2_ * -1 + y * rootover2_ * -1) * magnification_;
            defaultAngle_[2] = (x * rootover2_ + y * rootover2_ * -1) * magnification_;
            defaultAngle_[3] = (x * rootover2_ + y * rootover2_) * magnification_;
            return defaultAngle_;
        }

        std::array<int16_t, 4> getRotation(const int8_t rotationAngle)
        {
            std::array<int16_t, 4> defaultAngle_;
            for (int i = 0; i < 4; i++)
            {
                defaultAngle_[i] = rotationMagnification_ * rotationAngle * magnification_;
            }
            return defaultAngle_;
        }

        static constexpr float rootover2_ = 0.707107;
        static constexpr float robotSize[2] = {1, 1};
        static constexpr uint16_t rotationMagnification_ = 100;
        uint8_t magnification_;
    };
}
#endif