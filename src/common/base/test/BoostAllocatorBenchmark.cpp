/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */
#include "base/Logging.h"
#include <folly/init/Init.h>
#include <folly/Benchmark.h>
#include <algorithm>
#include <vector>
#include <boost/pool/pool_alloc.hpp>
#include <folly/FBVector.h>
#include "base/Allocator.h"

BENCHMARK(Test1_without_allocator) {
    for (size_t i = 0; i < 1000 * 1000; i++) {
        std::vector<int64_t> a;
        a.emplace_back(0);
        folly::doNotOptimizeAway(a);
    }
}

BENCHMARK_RELATIVE(Test1_fbvector_allocator) {
    for (size_t i = 0; i < 1000 * 1000; i++) {
        folly::fbvector<int64_t> a;
        a.emplace_back(0);
        folly::doNotOptimizeAway(a);
    }
}

BENCHMARK_RELATIVE(Test1_with_allocator) {
    int64_t tmp;
    nebula::SimpleAlloc<int64_t> myAlloc(&tmp, 1);
    for (size_t i = 0; i < 1000 * 1000; i++) {
        std::vector<int64_t, nebula::SimpleAlloc<int64_t>> a(myAlloc);
        a.emplace_back(0);
        folly::doNotOptimizeAway(a);
    }
}

BENCHMARK_RELATIVE(Test1_with_boostallocator) {
    typedef boost::pool_allocator<int64_t,
                              boost::default_user_allocator_new_delete,
                              boost::details::pool::default_mutex,
                              64,
                              128> Allocator;
    Allocator alloc;

    for (size_t i = 0; i < 1000 * 1000; i++) {
        std::vector<int64_t, Allocator> a(alloc);
        a.emplace_back(0L);
        folly::doNotOptimizeAway(a);
    }
    boost::singleton_pool<boost::pool_allocator_tag, sizeof(int64_t)>::purge_memory();
}

int main(int argc, char** argv) {
    folly::init(&argc, &argv, true);
    google::SetStderrLogging(google::INFO);
    folly::runBenchmarks();
    return 0;
}


/*
Intel(R) Xeon(R) CPU E5-2690 v2 @ 3.00GHz*

-O2
============================================================================
BoostAllocatorBenchmark.cpprelative                        time/iter  iters/s
 ============================================================================
Test1_without_allocator                                    273.86ms     3.65
Test1_with_allocator                             150.20%   182.34ms     5.48
Test1_with_boostallocator                         74.30%   368.56ms     2.71
============================================================================
*/

