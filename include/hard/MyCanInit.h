#ifndef MY_CANINIT_H
#define MY_CANINIT_H

#include <iostream>
#include <CAN.h>

namespace mycaninit
{
    class MyCanInit
    {
    public:
        MyCanInit(){}
        /**
         * @brief MyCanInitのコンストラクタ(2変数)。CANの初期化を行う。
         * @param (pinlx) LXpin(型:uint8_t)
         * @param (pinrx) RXpin(型:uint8_t)
         * @param (ratetime) CANのレート時間[初期値1e6](型:uint32_t)
         */
        void init(const int pinrx, const int pinlx, const uint32_t ratetime = 1e6) // init関数を作る
        {
            CAN.setPins(pinrx, pinlx);
            if (!CAN.begin(ratetime))
            {
                while (1)
                {
                    std::cout << "Starting CAN failed!" << std::endl;
                }
            }
            std::cout << "MyCanInitdone" << std::endl;
        }

        /**
         * @brief MyCanInitのコンストラクタ(配列参照)。CANの初期化を行う。
         * @param ((&pin)[2]) LXpinとRXpin(型:uint8_t)
         * @param (ratetime) CANのレート時間[初期値500E3](型:uint32_t)
         */
        void init(const uint8_t (&pin)[2], const uint32_t ratetime = 1e6)
        {
            CAN.setPins(pin[0], pin[1]);
            if (!CAN.begin(ratetime))
            {
                while (1)
                {
                    std::cout << "Starting CAN failed!" << std::endl;
                }
            }
        }

        virtual ~MyCanInit(){};

    private:
    };
}
#endif