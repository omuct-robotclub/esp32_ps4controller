#ifndef MY_CANSEND_H
#define MY_CANSEND_H

#include <iostream>
#include <array>
#include <thread>
#include <chrono>
#include <CAN.h>

#include "MyCanInit.h"

namespace mycansend
{
    class MyCanSend : public mycaninit::MyCanInit
    {
    public:
        // MyCanSend(const int pinrx, const int pinlx, const uint32_t ratetime = 1e6) : mycaninit::MyCanInit(pinrx, pinlx, ratetime) { std::cout << "MyCanSenddone" << std::endl; }
        using mycaninit::MyCanInit::MyCanInit;

        ~MyCanSend(){};

        /**
         * @brief CANでdataを送る関数
         * @param ((&Datas)[N]) 任意のデータを最大４長まで(型:int16_t)
         * @param (id = 0x02) CANid(型:uint8_t)
         */
        template <std::size_t N>
        void DataSend(const std::array<int16_t, N> Datas, const uint8_t id = 0x02)
        {
            // 最大uint8_t×8しか送信できないためint16_t×4より要素数が大きい場合エラーを吐く
            static_assert(0 < N && N <= 4, "pwm is must up to 4 lengths");
            const std::size_t CanDataLen_ = N * 2;
            int16_t SendDatas_[N] = {0};
            for (int i = 0; i < N; i++)
            {
                SendDatas_[i] = Datas[i];
            }
            uint8_t *CanData_ = reinterpret_cast<uint8_t *>(SendDatas_);

            // std::cout << "Sending packet ... " << std::endl;

            CAN.beginPacket(id);
            CAN.write(CanData_, CanDataLen_);
            CAN.endPacket();

            // std::cout << "done" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
/**
         * @brief CANでdataを送る関数
         * @param ((&Datas)[N]) 任意のデータを最大４長まで(型:int16_t)
         * @param (id = 0x02) CANid(型:uint8_t)
         */
        template <std::size_t N>
        void DataSend(const std::array<uint8_t, N> Datas, const uint8_t id = 0x02)
        {
            // 最大uint8_t×8しか送信できないためint16_t×4より要素数が大きい場合エラーを吐く
            static_assert(0 < N && N <= 8, "pwm is must up to 4 lengths");
            const std::size_t CanDataLen_ = N;
            uint8_t SendDatas_[N] = {0};
            for (int i = 0; i < N; i++)
            {
                SendDatas_[i] = Datas[i];
            }
            uint8_t *CanData_ = reinterpret_cast<uint8_t *>(SendDatas_);

            // std::cout << "Sending packet ... " << std::endl;

            CAN.beginPacket(id);
            CAN.write(CanData_, CanDataLen_);
            CAN.endPacket();

            // std::cout << "done" << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }

    private:
    };
}
#endif