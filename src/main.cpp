#include <iostream>
#include <Arduino.h>
#include <CAN.h>
#include <array>
#include <PS4Controller.h>
#include <chrono>
#include <thread>
#include "MyPS4.h"
#include "MyTimer.h"
#include "protocol.hpp"

static constexpr int8_t canReciveId = 9;

myps4::MyPS4 ps;
mytimer::MyTimer canTimer[4];
mytimer::MyTimer collectRogerTimer;

void CanSend(Command &cmd)
{
  uint8_t CanDatas[8];
  memcpy(CanDatas, &cmd, sizeof(cmd));
  CAN.beginPacket(cmd.ID);
  CAN.write(CanDatas, 8);
  CAN.endPacket();
}

void setup()
{
  Serial.begin(115200);
  CAN.setPins(G19, G22);
  if (!CAN.begin(1e6))
  {
    while (1)
    {
      std::cout << "Starting CAN failed!" << std::endl;
    }
  }
  std::cout << "MyCanInitdone" << std::endl;
  ps.init();
}
void loop()
{

  // omuni
  std::array<int16_t, 4> omuni_pwm = {0};
  int8_t LeftStickX = ps.getCoordinate(myps4::LStickX);
  int8_t LeftStickY = ps.getCoordinate(myps4::LStickY);
  int8_t RightStickX = ps.getCoordinate(myps4::RStickX);

  if (std::abs(LeftStickX) < 30)
  {
    LeftStickX = 0;
  }
  if (std::abs(LeftStickY) < 30)
  {
    LeftStickY = 0;
  }
  if (std::abs(RightStickX) < 70)
  {
    RightStickX = 0;
  }
  static constexpr int16_t TargetVelocity = 2 * 1000;
  // TargetVelocity = TargetVelocity ;
  Command cmd = Command{.tag = Command::Tag::SET_TARGET_VELOCITY, .set_target_velocity = Command::SetTargetVelocity{(int16_t)((TargetVelocity * LeftStickY * -1) / 127), (int16_t)((TargetVelocity * LeftStickX) / 127), (int16_t)((3000 * RightStickX* -1) / 127)}};

  if (canTimer[0].elapsedClock(20))
  {
    CanSend(cmd);
    // Serial.println("\tok");
  }

  int8_t donState = 0;
  int16_t rogerState = 0;
  int16_t collectAngleState = 0;
  static bool upButtonState = 0;
  static int8_t upSendState = 0;
  static int8_t upSendCount = 0;
  static bool downButtonState = 0;
  static int8_t downSendState = 0;
  static int8_t downSendCount = 0;
  if ((ps.getButton(myps4::Triangle) && ps.getButton(myps4::R2)) > downButtonState || downSendState)
  {
    donState = -1;
    rogerState = 0;
    collectAngleState = 0;
    if (!downSendState)
    {
      downSendCount = 0;
      downSendState++;
      canTimer[3].reset();
    }
    if (canTimer[3].elapsedClock(10) && downSendState == 1)
    {
      cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
      CanSend(cmd);
      downSendState++;
    }
    else if (canTimer[3].elapsedClock(10) && downSendState == 2)
    {
      cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
      CanSend(cmd);
      downSendState++;
    }
    else if (canTimer[3].elapsedClock(10) && downSendState == 3)
    {
      cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
      CanSend(cmd);
      downSendState = ++downSendCount > 3 ? 0 : 1;
    }
  }
  else if (ps.getButton(myps4::Triangle) > upButtonState || upSendState)
  {
    donState = 1;
    rogerState = 1400;
    collectAngleState = 1000 * M_PI / 2;
    if (!upSendState)
    {
      upSendCount = 0;
      upSendState++;
      canTimer[3].reset();
    }
    if (canTimer[3].elapsedClock(10) && upSendState == 1)
    {
      cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
      CanSend(cmd);
      upSendState++;
    }
    else if (canTimer[3].elapsedClock(10) && upSendState == 2)
    {
      cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
      CanSend(cmd);
      upSendState++;
    }
    else if (canTimer[3].elapsedClock(10) && upSendState == 3)
    {
      cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
      CanSend(cmd);
      upSendState = ++upSendCount > 3 ? 0 : 1;
    }
  }
  upButtonState = ps.getButton(myps4::Triangle);
  downButtonState = ps.getButton(myps4::Triangle) & ps.getButton(myps4::R2);

  static int16_t collectRogerState = 0;
  if (ps.getButton(myps4::Square) && ps.getButton(myps4::R2))
  {
    if (collectRogerTimer.elapsedClock(50))
    {
      collectRogerState -= 10;
    }
    collectRogerState = std::clamp<int16_t>(collectRogerState, 0, 2000);
    cmd = Command{.tag = Command::Tag::SET_ARM_LENGTH, .set_arm_length = Command::SetArmLength{collectRogerState}};

    if (canTimer[1].elapsedClock(30))
    {
      CanSend(cmd);
    }
  }
  else if (ps.getButton(myps4::Square))
  {
    if (collectRogerTimer.elapsedClock(50))
    {
      collectRogerState += 10;
    }
    collectRogerState = std::clamp<int16_t>(collectRogerState, 0, 2000);
    cmd = Command{.tag = Command::Tag::SET_ARM_LENGTH, .set_arm_length = Command::SetArmLength{collectRogerState}};
    if (canTimer[1].elapsedClock(30))
    {
      CanSend(cmd);
    }
  }
  else
  {
    collectRogerTimer.reset();
    canTimer[1].reset();
  }

  if (ps.getButton(myps4::Circle) && ps.getButton(myps4::R2))
  {
    collectAngleState = 1000 * M_PI / 4;
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    if (canTimer[2].elapsedClock(30))
    {
      CanSend(cmd);
    }
  }
  else if (ps.getButton(myps4::Circle))
  {
    collectAngleState = 1000 * M_PI / 2;
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    if (canTimer[2].elapsedClock(30))
    {
      CanSend(cmd);
    }
  }
  else
  {
    canTimer[2].reset();
  }

  static bool collectorState = 0;
  static bool collectorButtonState = 0;
  if (ps.getButton(myps4::Cross) > collectorButtonState)
  {
    collectorState = !collectorState;
    cmd = Command{.tag = Command::Tag::SET_COLLECTOR_CMD, .set_collector_cmd = Command::SetCollectorCmd{collectorState}};
    CanSend(cmd);
  }
  collectorButtonState = ps.getButton(myps4::Cross);

  static int16_t largeWheelState = 0;
  static bool largeWheelButtonState = 0;

  if (ps.getButton(myps4::R3) > largeWheelButtonState)
  {
    largeWheelState = largeWheelState ? 0 : 12000;
    cmd = Command{.tag = Command::Tag::SET_LARGE_WHEEL_CMD, .set_large_wheel_cmd = Command::SetLargeWheelCmd{largeWheelState}};
    CanSend(cmd);
  }
  largeWheelButtonState = ps.getButton(myps4::R3);

  static bool unwindButtonState = 0;

  if (ps.getButton(myps4::L3) > unwindButtonState)
  {
    cmd = Command{.tag = Command::Tag::UNWIND_STEER};
    CanSend(cmd);
  }
  unwindButtonState = ps.getButton(myps4::L3);
}
