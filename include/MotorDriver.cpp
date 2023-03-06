#include "MotorDriver.h"
#include <iostream>
namespace motordriver
{

    const uint8_t MotorDriver::getCanId() const
    {
        return MotorDriver::canid_;
    }
}