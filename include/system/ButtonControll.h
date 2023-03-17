#ifndef BOTTONCONTROLL_H
#define BOTTONCONTROLL_H
#include <iostream>
#include <vector>
#include <system/MyPS4.h>
namespace buttoncontroll
{
    class ButtonControll
    {
    public:
        // デフォルトコンストラクタ.
        ButtonControll(myps4::MyPS4 &ps) : ps_(ps) {}
        ButtonControll() = delete;

        // デストラクタ.
        ~ButtonControll() {}

        const uint8_t Click(const uint8_t state, const uint8_t maxCount = 2)
        {
            std::size_t i = 0;
            for (i = 0; i < clickState_.size(); i++)
            {
                if (clickState_[i].state_ == state)
                    break;
            }

            if (clickState_.size() == i)
            {
                clickState_.push_back({0});
                clickState_[i].state_ = state;
            }

            bool nowButtonState_ = ps_.getButton(clickState_[i].state_);
            if (nowButtonState_ && !clickState_[i].checkClick_)
            {
                clickState_[i].click_++;
                clickState_[i].click_ = clickState_[i].click_ > maxCount ? 0 : clickState_[i].click_;
                clickState_[i].checkClick_ = 1;
            }
            else if (!nowButtonState_)
            {
                clickState_[i].checkClick_ = 0;
            }
            return clickState_[i].click_;
        }

    private:
        myps4::MyPS4 &ps_;
        struct ClickState
        {
            uint8_t click_;
            uint8_t state_;
            bool checkClick_;
        };
        std::vector<ClickState> clickState_;
    };
}
#endif