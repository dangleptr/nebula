/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */
#include <climits>
#include <glog/logging.h>
#include "NebulaStorageClient.h"

DEFINE_string(meta_addrs, "", "meta server address");
DEFINE_string(space, "", "default space");
DEFINE_string(edge_name, "", "default edge name");
DEFINE_int64(vertex_id, 0, "default vertex id");

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::SetStderrLogging(google::INFO);
    nebula::NebulaStorageClient::bootstrap(&argc, &argv);
    nebula::NebulaStorageClient client(FLAGS_meta_addrs);

    if (!client.init(FLAGS_space)) {
        LOG(ERROR) << "Init client failed!";
        return -1;
    }
    LOG(INFO) << "Init storage client successfully";
    auto ret = client.getNeighbors({FLAGS_vertex_id},
                                   FLAGS_edge_name,
                                   INT_MAX,
                                   [] (nebula::Response&& resp) {
                    auto totalEdges = 0;
                    for (auto& vd : resp.vertices_) {
                        totalEdges += vd.dstIds_.size();
                    }
                    LOG(INFO) << "Total edges: " << totalEdges;
               }, [](nebula::ResultCode errCode) {
                    LOG(ERROR) << "ErrorCode:" << static_cast<int32_t>(errCode);
               });
    LOG(INFO) << "result " << static_cast<int32_t>(ret);
    return 0;
}
