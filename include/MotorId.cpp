// #include "MotorId.h"
// #include <iostream>
// namespace motorid
// {

//     // デストラクタ.
//     MotorId::~MotorId()
//     {
//     }

//     MotorId *MotorId::clone() const
//     {
//         return 0;
//     }

//     bool MotorId::equals(const Motor &other) const
//     {
//         return (this == &other);
//     }

//     // アクセッサ.
//     const uint8_t MotorId::getId() const
//     {
//         return id_;
//     }

//     // 等価演算子.
//     bool MotorId::operator==(const MotorId &other) const
//     {
//         if (id_ == other.id_)
//         {
//             return true;
//         }
//         return false;
//     }

//     bool MotorId::operator!=(const MotorId &other) const
//     {
//         return !(*this == other);
//     }

//     // 出力演算子.
//     std::ostream &operator<<(std::ostream &lhs, const MotorId &rhs)
//     {
//         const uint8_t id = rhs.getId();
//         lhs << id;
//         return lhs;
//     }
// }