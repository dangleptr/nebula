/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "storage/client/NebulaStorageClient.h"
#include "storage/client/NebulaStorageClientImpl.h"

namespace nebula {

// static
void NebulaStorageClient::bootstrap(int* argc, char*** argv) {
    folly::init(argc, argv);
}

NebulaStorageClient::NebulaStorageClient(const std::string& metaAddr)
    : metaAddr_(metaAddr) {
}

NebulaStorageClient::~NebulaStorageClient() {
    if (client_ != nullptr) {
        delete client_;
    }
}

bool NebulaStorageClient::init(const std::string& spaceName, int32_t ioHandlers) {
    client_ = new NebulaStorageClientImpl(metaAddr_);
    return client_->init(spaceName, ioHandlers);
}

ResultCode NebulaStorageClient::getNeighbors(const std::vector<VertexID>& srcIds,
                                             const std::string& edgeName,
                                             int32_t edgesLimit,
                                             OnSucceeded onSuc,
                                             OnError onErr) {
    return client_->getNeighbors(srcIds,
                                 edgeName,
                                 edgesLimit,
                                 std::move(onSuc),
                                 std::move(onErr));
}

}   // namespace nebula


