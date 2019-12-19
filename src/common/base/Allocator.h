/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef COMMON_BASE_ALLOCATOR_H_
#define COMMON_BASE_ALLOCATOR_H_


namespace nebula {

/**
 * It will reduce lots of malloc/free operations if you have many stl containers such as vector.
 **/
template<class T>
class SimpleAlloc {
public:
    typedef T value_type;

    SimpleAlloc(value_type* value, size_t capacity) {
        value_ = value;
        capacity_ = capacity;
    }

    ~SimpleAlloc() {
    }

    value_type* allocate(size_t n) {
        if (index_ + n > capacity_) {
            throw std::bad_alloc();
        }
        auto* q = &value_[index_];
        index_ += n;
        return q;
    }

    void deallocate(value_type*, size_t n) {
        if (index_ - n < 0) {
            LOG(FATAL) << "Impossible! index_ " << index_
                       << ", size " << n;
        }
        index_ -= n;
    }

private:
    value_type* value_ = nullptr;
    size_t capacity_ = 0;
    size_t index_ = 0;
 };



}  // namespace nebula
#endif  // COMMON_BASE_ALLOCATOR_H_

