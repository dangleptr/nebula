/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef NEBULA_STORAGE_CLIENT_STORAGECLIENTIMPL_H_
#define NEBULA_STORAGE_CLIENT_STORAGECLIENTIMPL_H_

#include "storage/client/NebulaStorageClient.h"
#include "storage/client/StorageClient.h"

namespace nebula {

class NebulaStorageClientImpl {
public:
    explicit NebulaStorageClientImpl(const std::string& metaAddr)
        : metaAddr_(metaAddr) {
    }

    bool init(const std::string& spaceName, int32_t ioHandlers);

    ResultCode getNeighbors(const std::vector<VertexID>& srcIds,
                            const std::string& edgeName,
                            OnSucceeded onSuc,
                            OnError onErr);

private:
    std::string metaAddr_;
    std::shared_ptr<folly::IOThreadPoolExecutor> ioExecutor_;
    std::unique_ptr<meta::MetaClient> metaClient_;
    std::unique_ptr<storage::StorageClient> storageClient_;
    std::unordered_map<std::string, EdgeType> edgeMaps_;
    int32_t spaceId_ = 0;
};

}   // namespace nebula


#endif  // NEBULA_STORAGE_CLIENT_STORAGECLIENTIMPL_H_
