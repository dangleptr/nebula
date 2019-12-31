/* Copyright (c) 2019 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "storage/query/QueryBaseProcessor.h"

DEFINE_int32(max_handlers_per_req, 10, "The max handlers used to handle one request");
DEFINE_int32(min_vertices_per_bucket, 3, "The min vertices number in one bucket");
DEFINE_int32(max_edge_returned_per_vertex, INT_MAX, "Max edge number returnred searching vertex");
DEFINE_bool(enable_vertex_cache, true, "Enable vertex cache");
DEFINE_int32(slow_get_bound_ms, 15, "Default slow get bound threshhold");
DEFINE_int32(reserved_edges_one_vertex, 1024, "reserve edges for one vertex");
DEFINE_int32(reserved_vertices, 512, "reserve vertices for one request");
DEFINE_bool(fast_path, true, "Enable fast path for structure");

namespace nebula {
namespace storage {

}  // namespace storage
}  // namespace nebula
