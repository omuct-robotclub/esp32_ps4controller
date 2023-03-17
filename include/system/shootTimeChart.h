#ifndef SHOOTTIMECHART_H
#define SHOOTTIMECHART_H

#include <iostream>
#include <array>
#include "hard/Servo.h"
#include "hard/LimitSw.h"
#include "LackSw.h"
#include "MyTimer.h"

namespace shoottimechart
{
    enum
    {
        stay,
        doorOpen,
        lackPush,
        lackPull,
        doorClose,
        diskLockOpen,
    } shootState_;
    class ShootTimeChart
    {
    public:
        ShootTimeChart() = delete;
        // 引数を1つとるコンストラクタ.
        ShootTimeChart(lacksw::LackSw &push, myservo::MyServo &door, myservo::MyServo &disklock) : push_(push), door_(door), diskLock_(disklock) {}

        // デストラクタ.
        ~ShootTimeChart() {}

        void Stay()
        {
            shootState_ = stay;
            std::cout << "staydone" << std::endl;
        }

        // アクセッサ.
        void DoorOpen()
        {
            shootState_ = doorOpen;
            std::cout << "doorOpendone" << std::endl;
        }

        void LackPush()
        {
            shootState_ = lackPush;
            std::cout << "lackPushdone" << std::endl;
        }

        void DiskLockOpen()
        {
            shootState_ = diskLockOpen;
            std::cout << "diskLockOpendone" << std::endl;
        }

        void LackPull()
        {
            shootState_ = lackPull;
            std::cout << "lackPulldone" << std::endl;
        }

        void DoorClose()
        {
            shootState_ = doorClose;
            std::cout << "doorCloesedone" << std::endl;
        }

        const uint8_t nowState() const
        {
            return shootState_;
        }

        void task(bool sw0, bool sw1)
        {
            bool sw[2] = {sw0, sw1};
            task(sw);
        }

        void task(bool (&sw)[2])
        {
            bool timeFlag_ = 0;
            if (shootState_ != stay)
            {
                timeFlag_ = timer_.elapsedClock(3000);
            }
            else
            {
                timer_.reset();
            }
            if (timeFlag_ && shootState_ == doorOpen)
            {
                LackPush();
            }
            else if (timeFlag_ && shootState_ == lackPush)
            {
                LackPull();
            }
            else if (timeFlag_ && shootState_ == lackPull)
            {
                DoorClose();
            }
            else if (timeFlag_ && shootState_ == doorClose)
            {
                DiskLockOpen();
            }

            switch (shootState_)
            {
            case stay:
                door_.setRatio(degree140_);
                diskLock_.setRatio(degree50_);
                push_.selectOwmMovePwm(lackPull_, defaltPwm_, sw);
                break;
            case doorOpen:
                door_.setRatio(degree55_);
                diskLock_.setRatio(degree0_);
                push_.selectOwmMovePwm(lackPush_, defaltPwm_, sw);
                // Serial.println(push_.getMovePwm());
                break;
            case lackPush:
                door_.setRatio(degree55_);
                diskLock_.setRatio(degree0_);
                push_.selectOwmMovePwm(lackPush_, defaltPwm_, sw);
                break;
            case lackPull:
                door_.setRatio(degree55_);
                diskLock_.setRatio(degree0_);
                push_.selectOwmMovePwm(lackPull_, defaltPwm_, sw);
                break;
            case doorClose:
                door_.setRatio(degree140_);
                diskLock_.setRatio(degree0_);
                push_.selectOwmMovePwm(lackPull_, defaltPwm_, sw);
                // Serial.println(push_.getMovePwm());
                break;
            case diskLockOpen:
                door_.setRatio(degree140_);
                diskLock_.setRatio(degree50_);
                push_.selectOwmMovePwm(lackPull_, defaltPwm_, sw);
                break;
            }
        }

    private:
        // コピーコンストラクタ.
        ShootTimeChart(const ShootTimeChart &other) = delete;
        lacksw::LackSw &push_;
        myservo::MyServo &door_;
        myservo::MyServo &diskLock_;
        mytimer::MyTimer timer_;
        static constexpr uint16_t defaltPwm_ = 12000;
        static constexpr uint8_t lackPush_ = 1;
        static constexpr uint8_t lackPull_ = 2;
        static constexpr uint8_t degree0_ = 0;
        static constexpr uint8_t degree55_ = 81;
        static constexpr uint8_t degree50_ = 71;
        static constexpr uint8_t degree140_ = 198;
        static constexpr uint8_t degree180_ = 255;
    };
}
#endif