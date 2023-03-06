// #include "Motor.h"
// #include <iostream>
// namespace motorbase
// {
//     // void MotorBase::setpwm(int16_t pwm)
//     // {
//     //     MotorBase::pwm_ = pwm;
//     // }

//     // int16_t MotorBase::getpwm()
//     // {
//     //     return MotorBase::pwm_;
//     // }
//     // デフォルトコンストラクタ.
//     Motor::Motor()
//         : pwm_(0)
//     {
//     }

//     // コピーコンストラクタ.
//     Motor::Motor(const Motor &other)
//         : pwm_(0)
//     {
//     }

//     // デストラクタ.
//     Motor::~Motor()
//     {
//     }

//     Motor *Motor::clone() const
//     {
//         return 0;
//     }

//     bool Motor::equals(const Motor &other) const
//     {
//         return (this == &other);
//     }

//     // アクセッサ.
//     const int16_t Motor::getPwm() const
//     {
//         return pwm_;
//     }

//     void Motor::setPwm(const int16_t pwm)
//     {
//         if (pwm_ == pwm)
//             return;

//         pwm_ = pwm;
//     }

//     // 等価演算子.
//     bool operator==(const Motor &lhs, const Motor &rhs)
//     {
//         return lhs.equals(rhs);
//     }

//     bool operator!=(const Motor &lhs, const Motor &rhs)
//     {
//         return !(lhs.equals(rhs));
//     }

//     // 出力演算子.
//     std::ostream &operator<<(std::ostream &lhs, const Motor &rhs)
//     {
//         const int16_t pwm = rhs.getPwm();
//         lhs << pwm;
//         return lhs;
//     }

//     std::ostream &operator<<(std::ostream &lhs, const Motor *rhs)
//     {
//         if (rhs == 0)
//         {
//             lhs << "(null)";
//         }
//         else
//         {
//             lhs << rhs->getPwm();
//         }
//         return lhs;
//     }
// }