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
mytimer::MyTimer canTimer;
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
  int16_t roll = RightStickX;
  static int16_t TargetVelocity = 1;
  TargetVelocity = TargetVelocity * 1000;

  Command cmd = Command{.tag = Command::Tag::SET_TARGET_VELOCITY, .set_target_velocity = Command::SetTargetVelocity{(int16_t)(TargetVelocity * LeftStickX / 127), (int16_t)(TargetVelocity * LeftStickY / 127), (int16_t)(TargetVelocity * RightStickX / 127)}};
  CanSend(cmd);

  int8_t donState = 0;
  int16_t rogerState = 0;
  static int16_t collectAngleState = 0;
  static int8_t upSendState = 0;
  static int8_t downSendState = 0;

  if (ps.getButton(myps4::Triangle) && ps.getButton(myps4::R2) || upSendState)
  {
    donState = -1;
    cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
    CanSend(cmd);
    rogerState = 7000;
    cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
    CanSend(cmd);
    collectAngleState = 0;
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    CanSend(cmd);
    upSendState++;
    if (upSendState > 10)
    {
      upSendState = 0;
    }
  }
  else if (ps.getButton(myps4::Triangle) || downSendState)
  {
    donState = 1;
    cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
    CanSend(cmd);
    rogerState = -7000;
    cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
    CanSend(cmd);
    collectAngleState = 1000 * M_PI / 2;
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    CanSend(cmd);
    downSendState++;
    if (downSendState > 10)
    {
      downSendState = 0;
    }
  }

  static bool upState = 0;
  static bool upButtonState = 0;
  static int8_t upSendState = 0;
  static bool downState = 0;
  static bool downButtonState = 0;
  static int8_t downSendState = 0;
  if ((ps.getButton(myps4::Triangle) && ps.getButton(myps4::R2)) > downButtonState || downSendState)
  {
    if (!downSendState)
    {
      donState = -1;
      rogerState = -7000;
      collectAngleState = 0;
      downSendState++;
    }
    else if (downSendState > 10)
    {
      downSendState = 0;
    }
    else
    {
      downSendState++;
    }
    cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
    CanSend(cmd);
    cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
    CanSend(cmd);
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    CanSend(cmd);
  }
  else if (ps.getButton(myps4::Triangle) > upButtonState || upSendState)
  {
    if (!upSendState)
    {
      donState = 1;
      rogerState = 7000;
      collectAngleState = 1000 * M_PI / 2;
      upSendState++;
    }
    else if (upSendState > 10)
    {
      upSendState = 0;
    }
    else
    {
      upSendState++;
    }
    cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
    CanSend(cmd);
    cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
    CanSend(cmd);
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    CanSend(cmd);
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

    CanSend(cmd);
  }
  else if (ps.getButton(myps4::Square))
  {
    if (collectRogerTimer.elapsedClock(50))
    {
      collectRogerState += 10;
    }
    collectRogerState = std::clamp<int16_t>(collectRogerState, 0, 2000);
    cmd = Command{.tag = Command::Tag::SET_ARM_LENGTH, .set_arm_length = Command::SetArmLength{collectRogerState}};

    CanSend(cmd);
  }
  else
  {
    collectRogerTimer.reset();
  }

  if (ps.getButton(myps4::Circle) && ps.getButton(myps4::R2))
  {
    collectAngleState = 1000 * M_PI / 4;
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    CanSend(cmd);
  }
  else if (ps.getButton(myps4::Circle))
  {
    collectAngleState = 1000 * M_PI / 2;
    cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
    CanSend(cmd);
  }

  // Command cmd = Command{.tag = Command::Tag::SET_TARGET_VELOCITY, .set_target_velocity = Command::SetTargetVelocity{(int16_t)(TargetVelocity * LeftStickX / 127), (int16_t)(TargetVelocity * LeftStickY / 127), (int16_t)(TargetVelocity * RightStickX / 127)}};
  // cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
  // cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
  // cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
  // cmd = Command{.tag = Command::Tag::SET_ARM_LENGTH, .set_arm_length = Command::SetArmLength{collectRogerState}};

  static bool collectorState = 0;
  static bool collectorButtonState = 0;
  static int8_t collectorSendState = 0;
  if (ps.getButton(myps4::Cross) > collectorButtonState || collectorSendState)
  {
    if (!collectorSendState)
    {
      collectorState = !collectorState;
      collectorSendState++;
    }
    else if (collectorSendState > 10)
    {
      collectorSendState = 0;
    }
    else
    {
      collectorSendState++;
    }
    cmd = Command{.tag = Command::Tag::SET_COLLECTOR_CMD, .set_collector_cmd = Command::SetCollectorCmd{collectorState}};
    CanSend(cmd);
  }
  collectorButtonState = ps.getButton(myps4::Cross);
  // Command cmd = Command{.tag = Command::Tag::SET_TARGET_VELOCITY, .set_target_velocity = Command::SetTargetVelocity{(int16_t)(TargetVelocity * LeftStickX / 127), (int16_t)(TargetVelocity * LeftStickY / 127), (int16_t)(TargetVelocity * RightStickX / 127)}};
  // cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
  // cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
  // cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
  // cmd = Command{.tag = Command::Tag::SET_ARM_LENGTH, .set_arm_length = Command::SetArmLength{collectRogerState}};
  // cmd = Command{.tag = Command::Tag::SET_COLLECTOR_CMD, .set_collector_cmd = Command::SetCollectorCmd{collectorState}};

  static int16_t largeWheelState = 0;
  static bool largeWheelButtonState = 0;
  static int8_t largeWheelSendState = 0;

  if (ps.getButton(myps4::R3) > largeWheelButtonState || largeWheelSendState)
  {
    if (!largeWheelSendState)
    {
      largeWheelState = largeWheelState ? 0 : 8000;
      largeWheelSendState++;
    }
    else if (largeWheelSendState > 10)
    {
      largeWheelSendState = 0;
    }
    else
    {
      largeWheelSendState++;
    }
    cmd = Command{.tag = Command::Tag::SET_LARGE_WHEEL_CMD, .set_large_wheel_cmd = Command::SetLargeWheelCmd{largeWheelState}};
    CanSend(cmd);
  }
  largeWheelButtonState = ps.getButton(myps4::R3);
  // if(canTimer.elapsedClock(500)){
  //   cmd = Command{.tag = Command::Tag::SET_COLLECTOR_CMD, .set_collector_cmd = Command::SetCollectorCmd{collectorState}};
  //   CanSend(cmd);
  //   cmd = Command{.tag = Command::Tag::SET_LARGE_WHEEL_CMD, .set_large_wheel_cmd = Command::SetLargeWheelCmd{largeWheelState}};
  //   CanSend(cmd);
  // }
  // Command cmd = Command{.tag = Command::Tag::SET_TARGET_VELOCITY, .set_target_velocity = Command::SetTargetVelocity{(int16_t)(TargetVelocity * LeftStickX / 127), (int16_t)(TargetVelocity * LeftStickY / 127), (int16_t)(TargetVelocity * RightStickX / 127)}};
  // cmd = Command{.tag = Command::Tag::SET_DONFAN_CMD, .set_donfan_cmd = Command::SetDonfanCmd{donState}};
  // cmd = Command{.tag = Command::Tag::SET_EXPANDER_CMD, .set_expander_cmd = Command::SetExpanderCmd{rogerState}};
  // cmd = Command{.tag = Command::Tag::SET_ARM_ANGLE, .set_arm_angle = Command::SetArmAngle{collectAngleState}};
  // cmd = Command{.tag = Command::Tag::SET_ARM_LENGTH, .set_arm_length = Command::SetArmLength{collectRogerState}};
  // cmd = Command{.tag = Command::Tag::SET_COLLECTOR_CMD, .set_collector_cmd = Command::SetCollectorCmd{collectorState}};
  // cmd = Command{.tag = Command::Tag::SET_LARGE_WHEEL_CMD, .set_large_wheel_cmd = Command::SetLargeWheelCmd{largeWheelState}};

  // for (auto i : CanDatas)
  // {
  //   Serial.print(+i);
  //   Serial.print("\t");
  // }
  // Serial.println();
}
