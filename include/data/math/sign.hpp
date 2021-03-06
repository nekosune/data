// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_SIGN
#define DATA_MATH_SIGN

#include <data/types.hpp>

namespace data::math {
            
    enum sign : int8_t { zero = 0 , positive = 1 , negative = -1 };

}

inline data::math::sign operator*(data::math::sign a, data::math::sign b) {
    return a == data::math::zero || b == data::math::zero ? data::math::zero : 
        a == b ? data::math::positive : data::math::negative;
}

#endif
