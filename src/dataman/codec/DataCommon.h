/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef DATAMAN_CODEC_DATACOMMON_H_
#define DATAMAN_CODEC_DATACOMMON_H_

#include <type_traits>
#include "base/Logging.h"

namespace nebula {
namespace dataman {
namespace codec {

enum class ResultType {
    SUCCEEDED = 0,
    E_NAME_NOT_FOUND = -1,
    E_INDEX_OUT_OF_RANGE = -2,
    E_INCOMPATIBLE_TYPE = -3,
    E_VALUE_OUT_OF_RANGE = -4,
    E_DATA_INVALID = -5,
};

template<typename IntType>
typename std::enable_if<
    std::is_integral<
        typename std::remove_cv<
            typename std::remove_reference<IntType>::type
        >::type
    >::value,
    bool
>::type
intToBool(IntType iVal) {
    return iVal != 0;
}

static int32_t decodeVarint(const int8_t* begin, size_t len, uint64_t& val) {
    const int8_t* end = begin + len;
    const int8_t* p = begin;
    int shift = 0;
    while (p != end && *p < 0) {
        val |= static_cast<uint64_t>(*p++ & 0x7f) << shift;
        shift += 7;
    }
    if (p == end) {
        return -1;
    }
    val |= static_cast<uint64_t>(*p++) << shift;
    return p - begin;
}

static size_t encodeVarint(uint64_t val, uint8_t* buf) {
    uint8_t* p = buf;
    while (val >= 128) {
        *p++ = 0x80 | (val & 0x7f);
        val >>= 7;
    }
    *p++ = uint8_t(val);
    return size_t(p - buf);
}

}  // namespace codec
}  // namespace dataman
}  // namespace nebula

#endif  // DATAMAN_CODEC_DATACOMMON_H_

