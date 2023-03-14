// Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//

#pragma once

#include <memory>
#include <vector>

#include "yb/client/client_fwd.h"

#include "yb/gutil/macros.h"

#include "yb/rpc/rpc_fwd.h"

#include "yb/tserver/pg_client.fwd.h"
#include "yb/tserver/pg_client_session.h"
#include "yb/tserver/tserver_fwd.h"

#include "yb/util/ref_cnt_buffer.h"

#include "yb/util/monotime.h"

namespace yb {

class MetricEntity;

namespace tserver {

class PgResponseCache {
 public:
  explicit PgResponseCache(MetricEntity* metric_entity);
  ~PgResponseCache();

  struct Response {
    Response(
        bool is_ok_status_, PgPerformResponsePB response_, std::vector<RefCntSlice> rows_data_)
        : is_ok_status(is_ok_status_),
          response(std::move(response_)),
          rows_data(std::move(rows_data_)) {
      DCHECK_EQ(response.responses_size(), rows_data.size());
    }

    bool is_ok_status;
    PgPerformResponsePB response;
    std::vector<RefCntSlice> rows_data;
  };

  using Setter = std::function<void(Response&&)>;

  Result<Setter> Get(
      PgPerformOptionsPB::CachingInfoPB* cache_info,
      PgPerformResponsePB* response, rpc::RpcContext* context);

 private:
  class Impl;

  std::unique_ptr<Impl> impl_;

  DISALLOW_COPY_AND_ASSIGN(PgResponseCache);
};

}  // namespace tserver
}  // namespace yb
