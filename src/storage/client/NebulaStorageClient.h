/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef NEBULA_STORAGE_CLIENT_STORAGECLIENT_H_
#define NEBULA_STORAGE_CLIENT_STORAGECLIENT_H_

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace nebula {

using VertexID = int64_t;
using EdgeType = int32_t;

struct VertexData {
    VertexID srcId_;
    std::vector<VertexID> dstIds_;
};

struct Response {
    std::vector<VertexData> vertices_;
};

enum ResultCode {
    SUCCEEDED,
    ERR_EDGE_TYPE_NOT_EXIST,
    ERR_UNKNOWN,
};

using OnSucceeded = std::function<void(Response&&)>;
using OnError = std::function<void(ResultCode)>;

class NebulaStorageClientImpl;

class NebulaStorageClient {
public:
    static void bootstrap(int* argc, char*** argv);

    explicit NebulaStorageClient(const std::string& metaAddr);

    ~NebulaStorageClient();

    bool init(const std::string& spaceName, int32_t ioHandlers = 10);

    ResultCode getNeighbors(const std::vector<VertexID>& srcIds,
                            const std::string& edgeName,
                            int32_t edgesLimit,
                            OnSucceeded onSuc,
                            OnError onErr);


private:
    std::string metaAddr_;
    NebulaStorageClientImpl* client_ = nullptr;
};

}   // namespace nebula


#endif  // NEBULA_STORAGE_CLIENT_STORAGECLIENT_H_
