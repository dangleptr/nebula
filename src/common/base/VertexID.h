/* Copyright (c) 2018 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef COMMON_BASE_VERTEXID_H_
#define COMMON_BASE_VERTEXID_H_

#include <cstdint>

namespace nebula {

struct VertexID {
    int64_t first = 0L;
    int64_t second = 0L;

    VertexID() = default;
    VertexID(const VertexID&) = default;
    VertexID(VertexID&&) noexcept = default;
    VertexID& operator=(const VertexID&) = default;
    VertexID& operator=(VertexID&&) noexcept = default;

    explicit VertexID(int64_t v) {
        first = v;
    }

    VertexID& operator=(int64_t v) {
        first = v;
        return *this;
    }

    bool operator==(const VertexID& that) const {
        return first == that.first && second == that.second;
    }

    bool operator<(const VertexID& that) const {
        return first != that.first ? first < that.first : second < that.second;
    }

    void clear() {
        first = 0L;
        second = 0L;
    }
};
}  // namespace nebula


namespace std {

inline std::ostream& operator <<(std::ostream& os, const nebula::VertexID& vid) {
    os << vid.first;
    if (vid.second != 0) {
        os << "_" << vid.second;
    }
    return os;
}

template<>
struct hash<nebula::VertexID> {
    std::size_t operator() (const nebula::VertexID& vid) const {
        return std::hash<std::pair<int64_t, int64_t>>()(std::make_pair(vid.first, vid.second));
    }
};

}  // namespace std

#endif  // COMMON_BASE_VERTEXID_H_

