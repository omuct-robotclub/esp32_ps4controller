#ifndef MY_CAN_H
#define MY_CAN_H

#include <iostream>
#include <thread>
#include <chrono>
#include <CAN.h>

namespace mycan
{
    class MyCan
    {
    public:
        /**
         * @brief MyCanのコンストラクタ(2変数)。CANの初期化を行う。
         * @param (pinlx) LXpin(型:uint8_t)
         * @param (pinrx) RXpin(型:uint8_t)
         * @param (ratetime) CANのレート時間[初期値1e6](型:uint32_t)
         */
        MyCan(const uint8_t pinrx, const uint8_t pinlx, const uint32_t ratetime = 1e6)
        {
            CAN.setPins(pinrx, pinlx);
            if (!CAN.begin(ratetime))
            {
                while (1)
                {
                    std::cout << "Starting CAN failed!" << std::endl;
                }
            }
        }

        /**
         * @brief MyCanのコンストラクタ(配列参照)。CANの初期化を行う。
         * @param ((&pin)[2]) LXpinとRXpin(型:uint8_t)
         * @param (ratetime) CANのレート時間[初期値500E3](型:uint32_t)
         */
        MyCan(const uint8_t (&pin)[2], const uint32_t ratetime = 1e6)
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
        ~MyCan(){};

        /**
         * @brief CANでdataを送る関数
         * @param ((&Datas)[N]) 任意のデータを最大４長まで(型:int16_t)
         * @param (id = 0x02) CANid(型:uint8_t)
         */
        template <std::size_t N>
        void DataSend(int16_t (&Datas)[N], const uint8_t id = 0x02)
        {
            // 最大uint8_t×8しか送信できないためint16_t×4より要素数が大きい場合エラーを吐く
            static_assert(0 < N && N <= 4, "pwm is must up to 4 lengths");

            const std::size_t CanDataLen_ = N * 2;
            uint8_t *CanData_ = reinterpret_cast<uint8_t *>(Datas);

            std::cout << "Sending packet ... " << std::endl;

            CAN.beginPacket(id);
            CAN.write(CanData_, CanDataLen_);
            CAN.endPacket();

            std::cout << "done" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }

    private:
    };
}
#endif