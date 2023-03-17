#ifndef MY_CANRECIVE_H
#define MY_CANRECIVE_H

#include <iostream>
#include <Arduino.h>
#include <array>
#include <vector>
#include <CAN.h>

#include "MyCanInit.h"

namespace mycanrecive
{
    class MyCanRecive : public mycaninit::MyCanInit
    {
    public:
        // MyCanRecive(const int pinrx, const int pinlx, const uint32_t ratetime = 1e6) : mycaninit::MyCanInit(pinrx, pinlx, ratetime) { std::cout << "MyCanRecivedone" << std::endl; }
        using mycaninit::MyCanInit::MyCanInit;
        ~MyCanRecive() {}
        void DataRead(const uint8_t id)
        {
            std::size_t i = 0;
            if (package_.empty())
            {
                package_.push_back({0});
                package_[i].canid_ = id;
            }
            else
            {
                for (i = 0; i < package_.size(); i++)
                {
                    if (package_[i].canid_ == id)
                        break;
                }

                if (package_.size() == i)
                {
                    package_.push_back({0});
                    package_[i].canid_ = id;
                }
            }
            int parsePacket_ = CAN.parsePacket();
            // std::cout << package_[i].canid_  << std::endl;
            // Serial.println(CAN.packetId());

            if (!CAN.packetRtr() && parsePacket_ > 0 && package_[i].canid_ == CAN.packetId())
            {
                for (uint8_t n = 0; n < 8; n++)
                {
                    package_[i].Datas_[n] = CAN.read();
                }
            }
        }

        const std::array<uint8_t, 8> getDatas(const uint8_t id)
        {
            assert(!package_.empty());
            std::size_t i = 0;
            for (i = 0; i < package_.size(); i++)
            {
                if (package_[i].canid_ == id)
                    break;
            }

            assert(!(package_.size() == i));
            return package_[i].Datas_;
        }

        const uint8_t getData(const uint8_t id, const uint8_t len)
        {
            assert(!package_.empty());
            std::size_t i = 0;
            for (i = 0; i < package_.size(); i++)
            {
                if (package_[i].canid_ == id)
                    break;
            }

            assert(!(package_.size() == i));
            return package_[i].Datas_[len];
        }

    private:
        struct ReadDatas
        {
            uint8_t canid_;
            std::array<uint8_t, 8> Datas_;
        };
        std::vector<ReadDatas> package_;
    };

}
#endif