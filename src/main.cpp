#include <iostream>
#include <Arduino.h>
#include <array>
#include "hard/LimitSw.h"
#include "hard/Motor.h"
#include "hard/MotorDriver.h"
#include "hard/MyCanInit.h"
#include "hard/MyCanRecive.h"
#include "hard/MyCanSend.h"
#include "hard/Servo.h"
#include "system/LackSw.h"
#include "system/mecunum.h"
#include "system/MyPS4.h"
#include "system/MyTimer.h"
#include "system/NormalRotation.h"
#include "system/SafePwm.h"
#include "system/ShootAngle.h"
#include "system/ButtonControll.h"
#include "system/shootTimeChart.h"
static constexpr uint8_t sensorBoardId1 = 7;
static constexpr uint8_t sensorBoardId2 = 9;
mycansend::MyCanSend canS;
mycanrecive::MyCanRecive canR; // port指定はしっかりと
mytimer::MyTimer tm;

motordriver::MotorDriver md[3] = {motordriver::MotorDriver(1), motordriver::MotorDriver(2), motordriver::MotorDriver(3)};
// 足回り
mymotor::MyMotor<4> mecunumMotor;
mecunum::Mecunum mecu(mecunumMotor);
// 発射
mymotor::MyMotor<1> shootRollerMotor;
normalrotation::NormalRotation shootR(shootRollerMotor);

mymotor::MyMotor<1> shootAngleMotor;
// shootangle::ShootAngle shootA(shootAngleMotor);
lacksw::LackSw shootA(shootAngleMotor);

mymotor::MyMotor<1> diskPushMotor;
lacksw::LackSw diskP(diskPushMotor);
// リロード
myservo::MyServo lockControllServo;

myservo::MyServo doorControllServo;

mymotor::MyMotor<1> diskTowerPushMotor;
lacksw::LackSw diskT(diskTowerPushMotor);

shoottimechart::ShootTimeChart sc(diskT, doorControllServo, lockControllServo);
// 回収
mymotor::MyMotor<1> rollerRotation;
lacksw::LackSw rollerR(rollerRotation);

myservo::MyServo pushU;
// 旗アーム
mymotor::MyMotor<1> upDownMotor;
lacksw::LackSw upD(upDownMotor);

mymotor::MyMotor<1> frontBackMotor;
lacksw::LackSw frontB(frontBackMotor);

mymotor::MyMotor<1> handControllMotor;
lacksw::LackSw handC(handControllMotor);

// ps4
myps4::MyPS4 ps;

// Clickモード
buttoncontroll::ButtonControll bc(ps);

// Limitsw
limitsw::LimitSw<5> LimitSw1;
limitsw::LimitSw<5> LimitSw2;
void setup()
{
  Serial.begin(115200);
  ps.init();
  canS.init(4, 5);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  // canR.init(4, 5);
}
void loop()
{
  ps.DataRead();
  canR.DataRead(sensorBoardId1);
  LimitSw1.setSwData(canR.getData(sensorBoardId1, 5));
  canR.DataRead(sensorBoardId2); // データ取得の確認
  LimitSw2.setSwData(canR.getData(sensorBoardId2, 5));

  bool LimitSw3[2] = {digitalRead(15), digitalRead(16)};

  // ashimawari
  if (ps.getButton(myps4::R1))
  {
    mecu.upPwm(80); // ここで倍率を上げる
  }
  else
  {
    mecu.upPwm(40); // 基本４０倍
  }
  mecu.setVector(ps.getCoordinate(myps4::RStickX), ps.getCoordinate(myps4::RStickY));
  if (ps.getCoordinate(myps4::LStickX))
  {
    mecu.selectRotation(ps.getCoordinate(myps4::LStickX));
  }
  md[1].setMotorparameter<4>(mecunumMotor.getPwms());

  // shoot

  shootR.setRotationPwm(ps.getButton(myps4::Circle));
  shootA.selectOwmMovePwm(ps.getButton(myps4::Up), ps.getButton(myps4::Down), 20000, 0, LimitSw1.getSwData(2));
  uint8_t TriangleClick = bc.Click(myps4::Triangle, 1);
  static uint8_t oldTriangleClick = TriangleClick;
  static uint8_t TriangleState = 0;
  if (oldTriangleClick != TriangleClick || TriangleState)
  {
    if (TriangleState == 1)
    {
      diskP.selectOwmMovePwm(TriangleState, 4000, 1300, LimitSw1.getSwData(1), LimitSw1.getSwData(0));
      TriangleState = LimitSw1.getSwData(1) ? 2 : TriangleState;
    }
    else if (TriangleState == 2)
    {
      diskP.selectOwmMovePwm(TriangleState, 4000, 1300, LimitSw1.getSwData(1), LimitSw1.getSwData(0));
      TriangleState = LimitSw1.getSwData(0) ? 0 : TriangleState;
    }
    else
    {
      TriangleState = 1;
    }
  }
  else
  {
    TriangleState = 0;
    diskP.stop(0);
  }
  oldTriangleClick = TriangleClick;

  // reload
  uint8_t L1Click = bc.Click(myps4::L1, 1);
  static uint8_t oldL1Click = L1Click;
  if (oldL1Click != L1Click && sc.nowState() == shoottimechart::stay)
  {
    sc.DoorOpen();
  }
  else if (oldL1Click != L1Click && sc.nowState() == shoottimechart::diskLockOpen)
  {
    sc.Stay();
  }
  sc.task(LimitSw1.getSwData(3), LimitSw1.getSwData(4));
  oldL1Click = L1Click;

  std::array<int16_t, 4> packedpwm1 = {shootR.getRotationPwm(), shootA.getMovePwm(), diskP.getMovePwm(), diskT.getMovePwm()};
  // std::array<int16_t, 4> packedpwm1 = {shootR.getRotationPwm(), shootA.getMovePwm(), diskT.getMovePwm(), diskT.getMovePwm()};
  md[2].setMotorparameter<4>(packedpwm1);
  std::array<uint8_t, 8> servoData = {0, 0, 0, 0, 0, lockControllServo.getRatio(), 0, doorControllServo.getRatio()};
  // std::array<int16_t, 4> servoData = {0, 220, 255, 0};
  // Serial.println(lockControllServo.getRatio());

  // collect
  rollerR.selectMovePwm(ps.getButton(myps4::L2B), LimitSw3[1], 0); // 回転方向の確認

  if (ps.getButton(myps4::R2B) && LimitSw3[1])
  {
    pushU.setRatio(0);
  }
  else
  {
    pushU.setRatio(64); // 45度
  }

  // // flag
  upD.selectOwmMovePwm(ps.getButton(myps4::Left), ps.getButton(myps4::Right), 6000, LimitSw2.getSwData(1), LimitSw2.getSwData(0)); // 回転方向の確認

  uint8_t CrossClick = bc.Click(myps4::Cross, 2);
  static bool upLimit = 0;
  static bool downLimit = 0;
  static bool limitCheck = 0;
  static bool setupCheck = 0;
  static uint8_t limitCounter = 0;

  if (!setupCheck && LimitSw2.getSwData(2))
  {
    frontB.resetWaitPwm(-8000);
    setupCheck = limitCounter == 2 ? 1 : 0;
    if (!limitCheck)
    {
      limitCounter++;
      limitCheck = 1;
    }
  }
  else if (!setupCheck)
  {
    frontB.resetWaitPwm(-8000);
    limitCheck = 0;
  }
  else if (LimitSw2.getSwData(2))
  {
    frontB.resetWaitPwm(0);
    if ((CrossClick == 1 || !CrossClick) && limitCounter == 2)
    {
      upLimit = 1;
      downLimit = 0;
    }
    else if (CrossClick == 1 && !limitCheck)
    {
      upLimit = 0;
      downLimit = 0;
      limitCounter++;
      limitCounter = limitCounter > 2 ? 2 : limitCounter;
      limitCheck = 1;
    }
    else if ((CrossClick == 2 || !CrossClick) && limitCounter == 1)
    {
      upLimit = 0;
      downLimit = 1;
    }
    else if (CrossClick == 2 && !limitCheck)
    {
      upLimit = 0;
      downLimit = 0;
      limitCounter--;
      limitCheck = 1;
    }
  }
  else
  {
    upLimit = 0;
    downLimit = 0;
    limitCheck = 0;
    frontB.resetWaitPwm(0);
  }
  frontB.selectOwmMovePwm(CrossClick, -8000, upLimit, downLimit); // 回転方向の確認
  // Serial.println(limitCounter);

  // frontB.selectMovePwm(CrossClick, LimitSw2.getSwData(2), LimitSw2.getSwData(3)); // 回転方向の確認

  uint8_t SquareClick = bc.Click(myps4::Square, 2);
  handC.selectOwmMovePwm(SquareClick, -6000, LimitSw2.getSwData(3), LimitSw2.getSwData(4)); // 回転方向の確認

  std::array<int16_t, 4> packedpwm2 = {upD.getMovePwm(), frontB.getMovePwm(), handC.getMovePwm(), rollerR.getMovePwm()};
  md[0].setMotorparameter<4>(packedpwm2);

  if (tm.elapsedClock(180))
  {
    // canS.DataSend<4>(md[0].getPwms(), md[0].getCanId());
    // canS.DataSend<4>(md[1].getPwms(), md[1].getCanId());
    canS.DataSend<4>(md[2].getPwms(), md[2].getCanId());
    canS.DataSend<8>(servoData, 144);
  }
  // canS.DataSend<4>(servodatta, md[1].getCanId());

  // std::array<int16_t, 4>
  //     aa = md[0].getPwms();
  // for (int i = 0; i < 4; i++)
  // {
  //   Serial.print(aa[i]);
  //   Serial.print("\t");
  // }
  // Serial.print("\n");
  // std::array<bool, 5>
  //     aa = LimitSw1.getSwDatas();
  // // aa = {1, 0, 0, 1, 0};
  // for (int i = 0; i < 5; i++)
  // {
  //   Serial.print(aa[i]);
  //   Serial.print("\t");
  // }
  // Serial.print(canR.getData(sensorBoardId1, 5), BIN);
  // Serial.print("\n");
  // int8_t x = ps.getCoordinate(myps4::LStickX);
  // int8_t y = ps.getCoordinate(myps4::LStickY);
  // Serial.print(x);
  // Serial.print("\t");
  // Serial.print(y);
  // Serial.print("\t");
  // int8_t x_ = std::abs(x) > 30 ? 0 : x;
  // int8_t y_ = std::abs(y) > 30 ? 0 : y;
  // Serial.print(x_);
  // Serial.print("\t");
  // Serial.print(y_);
  // Serial.print("\t");
  // Serial.print("\n");
  // Serial.println("a");
}
