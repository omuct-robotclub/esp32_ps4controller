#ifndef MYPS4_H
#define MYPS4_H
#include <iostream>
#include <array>
#include <PS4Controller.h>

namespace myps4
{
    enum
    {
        Right = 0,
        Down,
        Up,
        Left,
        Square,
        Cross,
        Circle,
        Triangle,
        UpRight,
        DownRight,
        UpLeft,
        DownLeft,
        L1,
        R1,
        Share,
        Options,
        L3,
        R3,
        PSButton,
        TouchPad,
        L2B,
        R2B
    } buttun_state;
    enum
    {
        L2C = 0,
        R2C,
        LStickX,
        LStickY,
        RStickX,
        RStickY
    } coordinate_state;
    class MyPS4
    {
    public:
        // デフォルトコンストラクタ.
        MyPS4() {}

        // デストラクタ.
        ~MyPS4() {}

        void init()
        {
            PS4.begin("cc:db:a7:49:57:a2");
            std::cout << "MyPS4done" << std::endl;
        }

        void DataRead()
        {
            if (PS4.isConnected())
            {
                button_[Right] = PS4.Right();
                button_[Down] = PS4.Down();
                button_[Up] = PS4.Up();
                button_[Left] = PS4.Left();

                button_[Square] = PS4.Square();
                button_[Cross] = PS4.Cross();
                button_[Circle] = PS4.Circle();
                button_[Triangle] = PS4.Triangle();

                button_[UpRight] = PS4.UpRight();
                button_[DownRight] = PS4.DownRight();
                button_[UpLeft] = PS4.UpLeft();
                button_[DownLeft] = PS4.DownLeft();

                button_[L1] = PS4.L1();
                button_[R1] = PS4.R1();

                button_[Share] = PS4.Share();
                button_[Options] = PS4.Options();
                button_[L3] = PS4.L3();
                button_[R3] = PS4.R3();

                button_[PSButton] = PS4.PSButton();
                button_[TouchPad] = PS4.Touchpad();

                button_[L2B] = PS4.L2();
                coordinate_[L2C] = button_[L2B] ? PS4.L2Value() : 0;
                button_[R2B] = PS4.R2();
                coordinate_[R2C] = button_[R2B] ? PS4.R2Value() : 0;

                coordinate_[LStickX] = PS4.LStickX();
                coordinate_[LStickY] = PS4.LStickY();
                coordinate_[RStickX] = PS4.RStickX();
                coordinate_[RStickY] = PS4.RStickY();
            }
        }

        const bool getButton(const uint8_t len) const
        {
            return button_[len];
        }

        const int8_t getCoordinate(const uint8_t len) const
        {
            return coordinate_[len];
        }

    private:
        // コピーコンストラクタ.
        MyPS4(const MyPS4 &other) {}
        std::array<bool, 22> button_;
        std::array<uint8_t, 6> coordinate_;
    };
}
#endif