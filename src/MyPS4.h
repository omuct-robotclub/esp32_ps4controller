#ifndef MYPS4_H
#define MYPS4_H
#include <iostream>
#include <array>
#include <PS4Controller.h>
namespace myps4
{
    enum buttun_state
    {
        Up = 0,
        Down,
        Right,
        Left,
        L3,
        R3,
        Cross,
        Circle,
        Triangle,
        Square,
        L1,
        R1,
        L2,
        R2
    };
    enum coordinate_state
    {
        LStickX = 0,
        LStickY,
        RStickX,
        RStickY
    };

    class MyPS4
    {
    public:
        MyPS4() {}
        // デストラクタ.
        ~MyPS4() {}

        void init()
        {
            PS4.begin("4c:75:25:92:34:ba");
            // PS4.begin("4c:75:25:92:20:9e");
            std::cout << "MyPS4done" << std::endl;
        }

        const bool getButton(const buttun_state &len) noexcept
        {
            // Serial.println(len);
            if (PS4.isConnected())
            {
                switch (len)
                {
                case buttun_state::Up:
                    // Serial.println("up");
                    return PS4.Up();
                    break;
                case buttun_state::Down:
                    // Serial.println("down");
                    return PS4.Down();
                    break;
                case buttun_state::Right:
                    // Serial.println("right");
                    return PS4.Right();
                    break;
                case buttun_state::Left:
                    // Serial.println("left");
                    return PS4.Left();
                    break;
                case buttun_state::L3:
                    // Serial.println("l3");
                    return PS4.L3();
                    break;
                case buttun_state::R3:
                    // Serial.println("R3");
                    return PS4.R3();
                    break;
                case buttun_state::Cross:
                    // Serial.println("Cross");
                    return PS4.Cross();
                    break;
                case buttun_state::Circle:
                    // Serial.println("Circle");
                    return PS4.Circle();
                    break;
                case buttun_state::Triangle:
                    // Serial.println("Triangle");
                    return PS4.Triangle();
                    break;
                case buttun_state::Square:
                    // Serial.println("square");
                    return PS4.Square();
                    break;
                case buttun_state::L1:
                    // Serial.println("l1");
                    return PS4.L1();
                    break;
                case buttun_state::R1:
                    // Serial.println("r1");
                    return PS4.R1();
                    break;
                case buttun_state::L2:
                    // Serial.println("l2");
                    if (PS4.L2Value() > 200)
                    {
                        return 1;
                    }
                    return 0;
                    // return PS4.L2();
                    break;
                case buttun_state::R2:
                    // Serial.println("r2");
                    if (PS4.R2Value() > 200)
                    {
                        return 1;
                    }
                    return 0;
                    // return PS4.R2();
                    break;
                default:
                    // Serial.println("no");
                    return 0;
                    break;
                }
            }
            else
            {
                return 0;
            }
        }

        const int8_t getCoordinate(const coordinate_state &len) noexcept
        {
            if (PS4.isConnected())
            {
                switch (len)
                {
                case coordinate_state::LStickX:
                    return PS4.LStickX();
                    break;
                case coordinate_state::LStickY:
                    return PS4.LStickY();
                    break;
                case coordinate_state::RStickX:
                    return PS4.RStickX();
                    break;
                case coordinate_state::RStickY:
                    return PS4.RStickY();
                    break;
                default:
                    return 0;
                    break;
                }
            }
            else
            {
                return 0;
            }
        }

    private:
        // コピーコンストラクタ.
        MyPS4(const MyPS4 &other) {}
    };
}
#endif