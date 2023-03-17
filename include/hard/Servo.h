#ifndef MYSERVO_H
#define MYSERVO_H
#include <iostream>
namespace myservo
{
    class MyServo
    {
    public:
        MyServo() : ratio_(0) {}
        ~MyServo(){};

        void setRatio(const uint8_t ratio)
        {
            ratio_ = ratio;
        }

        const uint8_t getRatio() const
        {
            return ratio_;
        }

        const float getEstimatedDeg()
        {
            return ratio_ * ByteToDeg_;
        }

    private:
        uint8_t ratio_;
        static constexpr float ByteToDeg_ = 0.70588235;
    };
}
#endif