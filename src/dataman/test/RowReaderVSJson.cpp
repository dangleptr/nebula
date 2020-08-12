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
#include <folly/container/F14Map.h>

using nebula::SchemaWriter;
using nebula::RowWriter;
using nebula::RowReader;

auto schemaMix = std::make_shared<SchemaWriter>();
static std::string dataMix;             // NOLINT
static std::string jsonVal;             // NOLINT
static std::string mapVal;              // NOLINT

void prepareSchemaAndData() {
    schemaMix->appendCol("col01", nebula::cpp2::SupportedType::BOOL)
              .appendCol("col02", nebula::cpp2::SupportedType::INT)
              .appendCol("col03", nebula::cpp2::SupportedType::STRING)
              .appendCol("col04", nebula::cpp2::SupportedType::FLOAT)
              .appendCol("col05", nebula::cpp2::SupportedType::DOUBLE)
              .appendCol("col06", nebula::cpp2::SupportedType::BOOL)
              .appendCol("col07", nebula::cpp2::SupportedType::INT)
              .appendCol("col08", nebula::cpp2::SupportedType::STRING)
              .appendCol("col09", nebula::cpp2::SupportedType::FLOAT)
              .appendCol("col10", nebula::cpp2::SupportedType::DOUBLE)
              .appendCol("col11", nebula::cpp2::SupportedType::BOOL)
              .appendCol("col12", nebula::cpp2::SupportedType::INT)
              .appendCol("col13", nebula::cpp2::SupportedType::STRING)
              .appendCol("col14", nebula::cpp2::SupportedType::FLOAT)
              .appendCol("col15", nebula::cpp2::SupportedType::DOUBLE);

    RowWriter wMix(schemaMix);
    wMix << true << 123 << "Hello" << 1.23 << 3.1415926;
    wMix << true << 123 << "Hello" << 1.23 << 3.1415926;
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

enum class VType : uint8_t {
    STR = 0,
    INT64 = 1,
    DOUBLE = 2,
    FLOAT = 3,
    BOOL = 4,
};

void prepareMapStr() {
    mapVal.reserve(1024);
    int32_t num = 15;
    mapVal.append(reinterpret_cast<char*>(&num), sizeof(num));


    bool bVal = true;
    int64_t iVal = 123;
    std::string sVal = "Hello";
    float fVal = 1.23;
    double dVal = 3.1415926;

    for (int i = 0; i < 3; i++) {
    {
        std::string col = folly::stringPrintf("col%02d", i * 5 + 1);
        int8_t colSize = col.size();
        mapVal.append(reinterpret_cast<char*>(&colSize), sizeof(colSize));
        mapVal.append(col.data(), col.size());

        VType type = VType::BOOL;
        mapVal.append(reinterpret_cast<char*>(&type), sizeof(type));
        mapVal.append(reinterpret_cast<char*>(&bVal), sizeof(bVal));
    }

    {
        std::string col = folly::stringPrintf("col%02d", i * 5 + 2);
        int8_t colSize = col.size();
        mapVal.append(reinterpret_cast<char*>(&colSize), sizeof(colSize));
        mapVal.append(col.data(), col.size());

        VType type = VType::INT64;
        mapVal.append(reinterpret_cast<char*>(&type), sizeof(type));
        mapVal.append(reinterpret_cast<char*>(&iVal), sizeof(iVal));
    }

    {
        std::string col = folly::stringPrintf("col%02d", i * 5 + 3);
        int8_t colSize = col.size();
        mapVal.append(reinterpret_cast<char*>(&colSize), sizeof(colSize));
        mapVal.append(col.data(), col.size());

        VType type = VType::STR;
        mapVal.append(reinterpret_cast<char*>(&type), sizeof(type));
        int32_t size = sVal.size();
        mapVal.append(reinterpret_cast<char*>(&size), sizeof(size));
        mapVal.append(sVal.data(), sVal.size());
    }

    {
        std::string col = folly::stringPrintf("col%02d", i * 5 + 4);
        int8_t colSize = col.size();
        mapVal.append(reinterpret_cast<char*>(&colSize), sizeof(colSize));
        mapVal.append(col.data(), col.size());

        VType type = VType::FLOAT;
        mapVal.append(reinterpret_cast<char*>(&type), sizeof(type));
        mapVal.append(reinterpret_cast<char*>(&fVal), sizeof(fVal));
    }

    {
        std::string col = folly::stringPrintf("col%02d", i * 5 + 5);
        int8_t colSize = col.size();
        mapVal.append(reinterpret_cast<char*>(&colSize), sizeof(colSize));
        mapVal.append(col.data(), col.size());

        VType type = VType::DOUBLE;
        mapVal.append(reinterpret_cast<char*>(&type), sizeof(type));
        mapVal.append(reinterpret_cast<char*>(&dVal), sizeof(dVal));
    }
    }
}

namespace std {

template <>
struct hash<folly::StringPiece> {
    size_t operator()(const folly::StringPiece& r) const {
        return folly::hash::SpookyHashV2::Hash64(r.data(), r.size(), 0);
    }
};

}

using Map = folly::F14FastMap<folly::StringPiece, folly::StringPiece>;

void decodeMapStr(Map& data, const std::string& encodedStr) {
    data.clear();
    int32_t offset = 0;
    const char* str = encodedStr.data();

    int32_t num = *reinterpret_cast<const int32_t*>(str);
    offset += sizeof(int32_t);

    for (int32_t i = 0; i < num; i++) {
        int8_t len = *reinterpret_cast<const int8_t*>(str + offset);
        offset += sizeof(int8_t);

        auto key = folly::StringPiece(str + offset, len);
        offset += len;

        VType type = *reinterpret_cast<const VType*>(str + offset);
        switch (type) {
            case VType::BOOL: {
                auto val = folly::StringPiece(str + offset, sizeof(VType) + sizeof(bool));
                offset += sizeof(VType) + sizeof(bool);
                data.emplace(key, val);
                break;
            }
            case VType::INT64: {
                auto val = folly::StringPiece(str + offset, sizeof(VType) + sizeof(int64_t));
                offset += sizeof(VType) + sizeof(int64_t);
                data.emplace(key, val);
                break;
            }
            case VType::FLOAT: {
                auto val = folly::StringPiece(str + offset, sizeof(VType) + sizeof(float));
                offset += sizeof(VType) + sizeof(float);
                data.emplace(key, val);
                break;
            }
            case VType::DOUBLE: {
                auto val = folly::StringPiece(str + offset, sizeof(VType) + sizeof(double));
                offset += sizeof(VType) + sizeof(double);
                data.emplace(key, val);
                break;
            }
            case VType::STR: {
                int32_t size = *reinterpret_cast<const int32_t*>(str + offset + sizeof(VType));
                auto val = folly::StringPiece(str + offset, sizeof(VType) + sizeof(int32_t) + size);
                offset += sizeof(VType) + sizeof(int32_t) + size;
                data.emplace(key, val);
                break;
            }
        }
    }
}

void readMix(int32_t iters) {
    for (int i = 0; i < iters; i++) {
        auto reader = RowReader::getRowReader(dataMix, schemaMix);
        /*
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
        */
        double dVal;
        reader->getDouble("col15", dVal);
        folly::doNotOptimizeAway(dVal);
    }
}

BENCHMARK(read_mix, iters) {
    readMix(iters);
}

/*
BENCHMARK_RELATIVE(JsonTest, iters) {
    for (decltype(iters) i = 0; i < iters; i++) {
        auto json = folly::parseJson(jsonVal);
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
    }
}
*/

using Value = boost::variant<int64_t, double, float, bool, folly::StringPiece>;

Value getValue(const folly::StringPiece& value) {
    const char* str = value.data();
    int32_t offset = 0;
    VType type = *reinterpret_cast<const VType*>(str + offset);
    offset += sizeof(VType);
    VLOG(1) << "Value size " << value.size() << ", type " << static_cast<int32_t>(type);
    switch (type) {
        case VType::BOOL: {
            return *reinterpret_cast<const bool*>(str + offset);
        }
        case VType::INT64: {
            return *reinterpret_cast<const int64_t*>(str + offset);
        }
        case VType::FLOAT: {
            return *reinterpret_cast<const float*>(str + offset);
        }
        case VType::DOUBLE: {
            return *reinterpret_cast<const double*>(str + offset);
        }
        case VType::STR: {
            int32_t size = *reinterpret_cast<const int32_t*>(str + offset);
            offset += sizeof(int32_t);
            return folly::StringPiece(str + offset, size);
        }
    }
    return 0L;
}


BENCHMARK_RELATIVE(MapTest, iters) {
    Map data(16);
    for (decltype(iters) i = 0; i < iters; i++) {
        decodeMapStr(data, mapVal);
        /*
        auto bVal = getValue(data["col01"]);
        folly::doNotOptimizeAway(bVal);

        auto iVal = getValue(data["col02"]);
        folly::doNotOptimizeAway(iVal);

        auto sVal = getValue(data["col03"]);
        folly::doNotOptimizeAway(sVal);

        auto fVal = getValue(data["col04"]);
        folly::doNotOptimizeAway(fVal);
        */
        auto dVal = getValue(data["col15"]);
        folly::doNotOptimizeAway(dVal);
    }
}

int main(int argc, char** argv) {
    folly::init(&argc, &argv, true);
    prepareSchemaAndData();
    prepareJson();
    prepareMapStr();
    folly::runBenchmarks();
    return 0;
}


