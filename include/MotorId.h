// #ifndef MOTOR_ID_H
// #define MOTOR_ID_H
// #include "Motor.h"
// #include <iostream>
// namespace motorid
// {
//     template <uint8_t N>
//     class MotorId : public motorbase::Motor<N>
//     {
//     public:
//         // デフォルトコンストラクタ.
//         MotorId()
//         {
//             for (uint8_t i; i < N; i++)
//             {
//                 id_[N] = i;
//             }
//         }
//         // デストラクタ.
//         virtual ~MotorId(){};

//         // オブジェクトを複製する.
//         virtual MotorId *clone() const
//         {
//             return 0;
//         }

//         // 等価なオブジェクトか?
//         virtual bool equals(const motorbase::Motor<N> &other) const
//         {
//             return (this == &other);
//         }

//         // 等価演算子.
//         bool operator==(const MotorId &other) const
//         {
//             if (id_ == other.id_)
//             {
//                 return true;
//             }
//             return false;
//         }
//         bool operator!=(const MotorId &other) const
//         {
//             return !(*this == other);
//         }

//     protected:
//         // コピーコンストラクタ.
//         MotorId(const MotorId &other){};

//     private:
//         uint8_t id_[N];
//     };
// }
// #endif