#ifndef MYTIMER_H
#define MYTIMER_H

#include <iostream>
#include <array>
#include <thread>
#include <chrono>

namespace mytimer
{

    class MyTimer
    {
    public:
        // 引数を1つとるコンストラクタ.
        MyTimer() : start_(std::chrono::system_clock::now()), end_(std::chrono::system_clock::now()), checkReset_(1), oldTime_(0) {}

        // デストラクタ.
        ~MyTimer() {}

        bool elapsedClock(double time)
        {
            if (checkReset_ || oldTime_ != time)
            {
                start_ = std::chrono::system_clock::now();
                end_ = std::chrono::system_clock::now();
                checkReset_ = checkReset_ ? 0 : 1;
            }
            oldTime_ = time;
            end_ = std::chrono::system_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_).count() > time)
            {
                checkReset_ = 1;
                return 1;
            }
            return 0;
        }

        void reset()
        {
            start_ = std::chrono::system_clock::now();
            end_ = std::chrono::system_clock::now();
            checkReset_ = 1;
        }

    private:
        // コピーコンストラクタ.
        MyTimer(const MyTimer &other) = delete;
        std::chrono::system_clock::time_point start_, end_;
        bool checkReset_;
        double oldTime_;
    };
}
#endif