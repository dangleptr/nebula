/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "storage/client/NebulaStorageClientImpl.h"

namespace nebula {

bool NebulaStorageClientImpl::init(const std::string& spaceName, int ioHandlers) {
    ioExecutor_ = std::make_shared<folly::IOThreadPoolExecutor>(ioHandlers);
    auto addrsRet
        = network::NetworkUtils::toHosts(metaAddr_);
    if (!addrsRet.ok()) {
        LOG(ERROR) << "Init failed, get address failed, status " << addrsRet.status();
        return false;
    }
    auto addrs = std::move(addrsRet).value();

    metaClient_ = std::make_unique<meta::MetaClient>(ioExecutor_,
                                                     std::move(addrs),
                                                     HostAddr(0, 0),
                                                     0,
                                                     false,
                                                     "",
                                                     true);
    // load data try 3 time
    bool loadDataOk = metaClient_->waitForMetadReady(3);
    if (!loadDataOk) {
        LOG(ERROR) << "Failed to synchronously wait for meta service ready";
        return false;
    }
    {
        auto ret = metaClient_->getSpaceIdByNameFromCache(spaceName);
        if (!ret.ok()) {
            LOG(ERROR) << "Failed get space id for " << spaceName;
            return false;
        }
        spaceId_ = ret.value();
    }
    {
        auto allRet = metaClient_->getAllEdgeFromCache(spaceId_);
        if (!allRet.ok()) {
            LOG(ERROR) << "Failed get all edges!";
            return false;
        }
        auto edgeNames = std::move(allRet).value();
        for (auto& ename : edgeNames) {
            auto typeRet = metaClient_->getEdgeTypeByNameFromCache(spaceId_, ename);
            if (!typeRet.ok()) {
                LOG(ERROR) << "Failed get edge type for " << ename;
                return false;
            }
            edgeMaps_.emplace(ename, typeRet.value());
        }
    }
    storageClient_ = std::make_unique<storage::StorageClient>(ioExecutor_,
                                                              metaClient_.get(),
                                                              nullptr);
    return true;
}

ResultCode NebulaStorageClientImpl::getNeighbors(const std::vector<VertexID>& srcIds,
                                                 const std::string& edgeName,
                                                 int32_t edgesLimit,
                                                 OnSucceeded onSuc,
                                                 OnError onErr) {
    auto it = edgeMaps_.find(edgeName);
    if (it == edgeMaps_.end()) {
        return ResultCode::ERR_EDGE_TYPE_NOT_EXIST;
    }
    auto type = it->second;
    storage::cpp2::PropDef pd;
    pd.owner = storage::cpp2::PropOwner::EDGE;
    pd.name = "_dst";
    pd.id.set_edge_type(type);
    storageClient_->getNeighbors(spaceId_, srcIds, {type}, "", {pd}, nullptr, edgesLimit)
        .via(ioExecutor_.get())
        .thenValue([this, onSuc = std::move(onSuc)](auto&& rpcResp) mutable {
        auto& all = rpcResp.responses();
        auto& hostLatency = rpcResp.hostLatency();
        int32_t totalVertices = 0;
        for (size_t i = 0; i < hostLatency.size(); i++) {
            totalVertices += rpcResp.responses()[i].get_vertices()->size();
            VLOG(1) << std::get<0>(hostLatency[i])
                    << ", time cost " << std::get<1>(hostLatency[i])
                    << "us / " << std::get<2>(hostLatency[i])
                    << "us, total results " << rpcResp.responses()[i].get_vertices()->size();
        }
        Response result;
        result.vertices_.reserve(totalVertices);
        for (auto &resp : all) {
            if (resp.get_vertices() == nullptr) {
                continue;
            }
            for (auto& vdata : resp.vertices) {
                result.vertices_.emplace_back();
                auto& vd = result.vertices_.back();
                vd.srcId_ = vdata.get_vertex_id();
                DCHECK(vdata.__isset.edge_data);
                for (auto& edata : vdata.edge_data) {
                    vd.dstIds_.reserve(edata.edges.size());
                    for (auto& edge : edata.edges) {
                        vd.dstIds_.emplace_back(edge.get_dst());
                    }  // for edges
                }  // for edata
            }   // for `vdata'
        }   // for `resp'
        onSuc(std::move(result));
     }).thenError([this, onErr = std::move(onErr)](auto&& e) {
        LOG(ERROR) << e.what().c_str();
        onErr(ResultCode::ERR_UNKNOWN);
     });
    return ResultCode::SUCCEEDED;
}

}   // namespace nebula


