// #include <Arduino.h>
// #include <PS4BT.h>
// #include "MyCan.h"
// USB Usb;
// BTD Btd(&Usb);
// PS4BT PS4(&Btd, PAIR);

// // 移動             Lスティック
// // 旋回　           ZL、ZR
// // 低速旋回　       Rスティック
// // 射角変更　       Rスティック
// // 移動速度アップ　 PS4.getButtonClick(L3)
// // ハンド上下
// // ハンド前後
// // ハンド開閉
// // 発射モーターオン PS4.getButtonClick(R3)
// // 発射            PS4.getButtonPress(R1)
// // 単発発射
// // 回収           PS4.getButtonClick(LEFT)
// // 弾倉付け替え

// int v1 = 0;
// int v2 = 0;
// int v3 = 0;
// int v4 = 0;

// int fire = 0;
// int count = 0;

// int times = 50;

// int old = 1;
// int old1 = 1;
// int now = 0; // 前方リミットスイッチの現在の値
// int now1 = 1;

// unsigned long oldtime = 0;

// double turn = 0;

// int Moter = 1;
// int power = 0;

// int shoot = 0;

// int kaisyu = 0;

// int rad = 0;

// int LS = 22;
// int LS1 = 24;

// bool printAngle, printTouch;
// uint8_t oldL2Value, oldR2Value;

// void PWM_transmit_via_CAN(int16_t pwm1, int16_t pwm2, int16_t pwm3, int16_t pwm4, uint8_t can_id)
// { // CAN通信
//   int16_t pwms[4] = {pwm1, pwm2, pwm3, pwm4};
//   uint8_t pwm_highbyte[4], pwm_lowbyte[4], can_data[8], calc_checksum;
//   static const uint8_t can_frame_prefix = 0xff,
//                        can_frame_verID = 0xfe;
//   for (uint8_t i = 0; i < 4; i++)
//   {
//     if (abs(pwms[i]) > 30000)
//       pwms[i] = (pwms[i] > 0) ? 30000 : -30000;
//     pwm_highbyte[i] = uint8_t(int16_t(pwms[i] >> 8) & 0xff);
//     pwm_lowbyte[i] = uint8_t(pwms[i] & 0xff);
//     can_data[i * 2] = pwm_lowbyte[i];
//     can_data[(i * 2) + 1] = pwm_highbyte[i];
//   }
//   calc_checksum = 255 - (uint16_t(can_id + can_data[0] + can_data[1] + can_data[2] + can_data[3] + can_data[4] + can_data[5] + can_data[6] + can_data[7]) % 256);

//   Serial1.write(can_frame_prefix);
//   Serial1.write(can_frame_verID);
//   Serial1.write(can_id);
//   Serial1.write(can_data[0]);
//   Serial1.write(can_data[1]);
//   Serial1.write(can_data[2]);
//   Serial1.write(can_data[3]);
//   Serial1.write(can_data[4]);
//   Serial1.write(can_data[5]);
//   Serial1.write(can_data[6]);
//   Serial1.write(can_data[7]);
//   Serial1.write(calc_checksum);
// }

// void setup()
// {
//   Serial.begin(115200);
//   Serial1.begin(115200);

// #if !defined(__MIPSEL__) //_MIPSEL_定義済み
//   while (!Serial)
//     ; // LeonardoやTeensyなど、USB CDCシリアル接続を内蔵したボードで使用される。
// #endif
//   if (Usb.Init() == -1)
//   {
//     Serial.print(F("\r\nOSC did not start"));
//     while (1)
//       ; // 停止
//   }
//   Serial.print(F("\r\nPS4 Bluetooth Library Started"));
//   pinMode(LS, INPUT_PULLUP);
//   pinMode(LS1, INPUT_PULLUP);
// }

// void loop()
// {
//   // 左スティックのX,Y数値をhypotにかけた値の変数
//   float hypot_S = 0;
//   // 左スティックのX数値[-128~127]
//   double RHY = (128 - PS4.getAnalogHat(RightHatY));
//   // 左スティックのY数値[-128~127]
//   double RHX = (PS4.getAnalogHat(RightHatX) - 128);

//   double LHX = (PS4.getAnalogHat(LeftHatX) - 128);

//   double LHY = (128 - PS4.getAnalogHat(LeftHatY));

//   double ZRL = ((PS4.getAnalogButton(R2)) - (PS4.getAnalogButton(L2))) / 4;

//   Usb.Task();

//   if (PS4.connected())
//   {

//     // 移動速度の管理
//     if (PS4.getButtonClick(L3))
//     {
//       if (times == 50)
//       {
//         times = 150;
//       }
//       else
//       {
//         times = 50;
//       }
//     }

//     if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117)
//     {
//       turn = LHX;
//     }
//     else if (ZRL < -15 || ZRL > 15)
//     {
//       turn = ZRL;
//     }
//     else
//     {
//       turn = 0;
//     }

//     // 移動
//     if (PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117 || turn > 15 || turn < -15)
//     {
//       v1 = RHX * -sin(PI / 4) + RHY * cos(PI / 4) - turn;
//       v2 = RHX * -sin(PI / 4) - RHY * cos(PI / 4) - turn;
//       v3 = RHX * sin(PI / 4) - RHY * cos(PI / 4) - turn;
//       v4 = RHX * sin(PI / 4) + RHY * cos(PI / 4) - turn;
//     }
//     else
//     {
//       v1 = 0;
//       v2 = 0;
//       v3 = 0;
//       v4 = 0;
//     }
//     PWM_transmit_via_CAN(v1 * times, v2 * times, v3 * times, v4 * times, 2);

//     // 射角変更
//     if (PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117)
//     {
//       rad = LHY * 150;
//     }
//     else
//     {
//       rad = 0;
//     }

//     // 回収機構のオンオフ
//     if (PS4.getButtonClick(LEFT))
//     {
//       if (kaisyu == 0)
//       {
//         kaisyu = -13000;
//       }
//       else
//       {
//         kaisyu = 0;
//       }
//     }

//     // 発射用モーターのオンオフ
//     if (PS4.getButtonClick(R3))
//     {
//       if (shoot == 0)
//       {
//         shoot = 1;
//       }
//       else
//       {
//         shoot = 0;
//       }
//     }
//     switch (shoot)
//     {
//     case 1:
//       if (oldtime + 100 < millis() && power < 11000)
//       {
//         power = power + 500;
//         Serial.println(power);
//         oldtime = millis();
//       }
//       break;

//     case 0:
//       power = 0;
//       oldtime = 0;
//       break;

//     default:
//       shoot = 0;
//     }

//     // 出力の確認用コード、普段は不使用
//     //    if (PS4.getButtonClick(LEFT)) {
//     //      power = power + 500;
//     //      Serial.println(power);
//     //    }
//     //  } else {
//     //    power = 0;
//     //  }

//     // ディスクの押し出し
//     if (PS4.getButtonPress(R1))
//     {
//       if (now == 0)
//       {
//         Moter = -1500;
//         Serial.println("back");
//       }
//       if (now1 == 0)
//       {
//         Moter = 6000;
//       }
//       now = digitalRead(LS);
//       now1 = digitalRead(LS1);
//     }
//     else
//     {
//       Moter = 0;
//       now = 0;
//     }

//     PWM_transmit_via_CAN(power, kaisyu, Moter, rad, 4);
//   }
// }
#include <iostream>
#include <Arduino.h>
#include "Motor.h"
#include "MyCan.h"
#include "MotorDriver.h"
mycan::MyCan can(4, 5);
motordriver::MotorDriver md[3]={motordriver::MotorDriver(1),motordriver::MotorDriver(2),motordriver::MotorDriver(3)};
motorbase::Motor<4> mb;
// motorbase::Motor *mbb = {{{motorid::MotorId(0)}, {motorid::MotorId(1)}, {motorid::MotorId(2)}, {motorid::MotorId(3)}}};
void setup()
{
  Serial.begin(115200);
}
void loop()
{
  for (int i = 0; i < 4; i++)
  {
    // Serial.println(mbb[i].getId());
    // mbb[i].setPwm(); // なぞ
    md[0].setMotorparameter(mb);
    // std::cout << mbb[i].getId() << std::endl;
  }
}
