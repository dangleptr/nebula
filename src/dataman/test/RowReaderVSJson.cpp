/* Copyright (c) 2018 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "base/Base.h"
#include <folly/Benchmark.h>
#include "dataman/SchemaWriter.h"
#include "dataman/RowWriter.h"
#include "dataman/RowReader.h"
#include <folly/dynamic.h>

using nebula::SchemaWriter;
using nebula::RowWriter;
using nebula::RowReader;

auto schemaMix = std::make_shared<SchemaWriter>();
static std::string dataMix;             // NOLINT
static std::string jsonVal;             // NOLINT

void prepareSchemaAndData() {
    schemaMix->appendCol("col01", nebula::cpp2::SupportedType::BOOL)
              .appendCol("col02", nebula::cpp2::SupportedType::INT)
              .appendCol("col03", nebula::cpp2::SupportedType::STRING)
              .appendCol("col04", nebula::cpp2::SupportedType::FLOAT)
              .appendCol("col05", nebula::cpp2::SupportedType::DOUBLE);

    RowWriter wMix(schemaMix);
    wMix << true << 123 << "Hello" << 1.23 << 3.1415926;
    dataMix = wMix.encode();
}

void prepareJson() {
    folly::dynamic d = folly::dynamic::object;
    d["col01"] = true;
    d["col02"] = 123;
    d["col03"] = "Hello";
    d["col04"] = 1.23;
    d["col05"] = 3.1415926;
    jsonVal = folly::toJson(d);
}

void readMix(int32_t iters) {
    for (int i = 0; i < iters; i++) {
        auto reader = RowReader::getRowReader(dataMix, schemaMix);
        folly::StringPiece sVal;
        reader->getString("col03", sVal);
        folly::doNotOptimizeAway(sVal);

#if 0
        bool bVal;
        reader->getBool("col01", bVal);
        folly::doNotOptimizeAway(bVal);

        int64_t iVal;
        reader->getInt("col02", iVal);
        folly::doNotOptimizeAway(iVal);

        folly::StringPiece sVal;
        reader->getString("col03", sVal);
        folly::doNotOptimizeAway(sVal);

        float fVal;
        reader->getFloat("col04", fVal);
        folly::doNotOptimizeAway(fVal);

        double dVal;
        reader->getDouble("col05", dVal);
        folly::doNotOptimizeAway(dVal);
#endif
    }
}

BENCHMARK(read_mix, iters) {
    readMix(iters);
}

BENCHMARK_RELATIVE(JsonTest, iters) {
    for (decltype(iters) i = 0; i < iters; i++) {
        auto json = folly::parseJson(jsonVal);
        const auto& sVal = json["col03"].getString();
        folly::doNotOptimizeAway(sVal);
#if 0
        auto bVal = json["col01"].getBool();
        folly::doNotOptimizeAway(bVal);

        auto iVal = json["col02"].getInt();
        folly::doNotOptimizeAway(iVal);

        const auto& sVal = json["col03"].getString();
        folly::doNotOptimizeAway(sVal);

        auto fVal = json["col04"].getDouble();
        folly::doNotOptimizeAway(fVal);

        auto dVal = json["col05"].getDouble();
        folly::doNotOptimizeAway(dVal);
#endif
    }
}


int main(int argc, char** argv) {
    folly::init(&argc, &argv, true);
    prepareSchemaAndData();
    prepareJson();
    folly::runBenchmarks();
    return 0;
}


