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
#include <optional>
#include <string>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/version.hpp>

#include "yb/client/client_fwd.h"

#include "yb/common/pg_types.h"
#include "yb/common/read_hybrid_time.h"
#include "yb/common/transaction.h"

#include "yb/master/master_fwd.h"

#include "yb/rpc/rpc_fwd.h"

#include "yb/tserver/tserver_fwd.h"
#include "yb/tserver/tserver_util_fwd.h"
#include "yb/tserver/pg_client.fwd.h"

#include "yb/util/enums.h"
#include "yb/util/monotime.h"

#include "yb/yql/pggate/pg_gate_fwd.h"
#include "yb/yql/pggate/ybc_pg_typedefs.h"

namespace yb {
namespace pggate {

YB_DEFINE_ENUM(
  DdlType,
  // Not a DDL operation.
  ((NonDdl, 0))
  // DDL operation that does not modify the DocDB schema protobufs.
  ((DdlWithoutDocdbSchemaChanges, 1))
  // DDL operation that modifies the DocDB schema protobufs.
  ((DdlWithDocdbSchemaChanges, 2))
);

#define YB_PG_CLIENT_SIMPLE_METHODS \
    (AlterDatabase)(AlterTable)(CreateDatabase)(CreateTable)(CreateTablegroup) \
    (DropDatabase)(DropTablegroup)(TruncateTable)

struct PerformResult {
  Status status;
  ReadHybridTime catalog_read_time;
  rpc::CallResponsePtr response;
  HybridTime used_in_txn_limit;

  std::string ToString() const {
    return YB_STRUCT_TO_STRING(status, catalog_read_time, used_in_txn_limit);
  }
};

using PerformCallback = std::function<void(const PerformResult&)>;

class PgClient {
 public:
  PgClient();
  ~PgClient();

  Status Start(rpc::ProxyCache* proxy_cache,
               rpc::Scheduler* scheduler,
               const tserver::TServerSharedObject& tserver_shared_object);
  void Shutdown();

  void SetTimeout(MonoDelta timeout);

  Result<PgTableDescPtr> OpenTable(
      const PgObjectId& table_id, bool reopen, CoarseTimePoint invalidate_cache_time);

  Result<client::VersionedTablePartitionList> GetTablePartitionList(const PgObjectId& table_id);

  Status FinishTransaction(Commit commit, DdlType ddl_type);

  Result<master::GetNamespaceInfoResponsePB> GetDatabaseInfo(PgOid oid);

  Result<std::pair<PgOid, PgOid>> ReserveOids(PgOid database_oid, PgOid next_oid, uint32_t count);

  Result<bool> IsInitDbDone();

  Result<uint64_t> GetCatalogMasterVersion();

  Status CreateSequencesDataTable();

  Result<client::YBTableName> DropTable(
      tserver::PgDropTableRequestPB* req, CoarseTimePoint deadline);

  Result<int> WaitForBackendsCatalogVersion(
      tserver::PgWaitForBackendsCatalogVersionRequestPB* req, CoarseTimePoint deadline);
  Status BackfillIndex(tserver::PgBackfillIndexRequestPB* req, CoarseTimePoint deadline);

  Status GetIndexBackfillProgress(const std::vector<PgObjectId>& index_ids,
                                  uint64_t** backfill_statuses);

  Result<yb::tserver::PgGetLockStatusResponsePB> GetLockStatusData(
      const std::string& table_id, const std::string& transaction_id);

  Result<int32> TabletServerCount(bool primary_only);

  Result<client::TabletServersInfo> ListLiveTabletServers(bool primary_only);

  Status SetActiveSubTransaction(
      SubTransactionId id, tserver::PgPerformOptionsPB* options);
  Status RollbackToSubTransaction(SubTransactionId id, tserver::PgPerformOptionsPB* options);

  Status ValidatePlacement(const tserver::PgValidatePlacementRequestPB* req);

  Result<client::TableSizeInfo> GetTableDiskSize(const PgObjectId& table_oid);

  Status InsertSequenceTuple(int64_t db_oid,
                             int64_t seq_oid,
                             uint64_t ysql_catalog_version,
                             bool is_db_catalog_version_mode,
                             int64_t last_val,
                             bool is_called);

  Result<bool> UpdateSequenceTuple(int64_t db_oid,
                                   int64_t seq_oid,
                                   uint64_t ysql_catalog_version,
                                   bool is_db_catalog_version_mode,
                                   int64_t last_val,
                                   bool is_called,
                                   std::optional<int64_t> expected_last_val,
                                   std::optional<bool> expected_is_called);

  Result<std::pair<int64_t, int64_t>> FetchSequenceTuple(int64_t db_oid,
                                                         int64_t seq_oid,
                                                         uint64_t ysql_catalog_version,
                                                         bool is_db_catalog_version_mode,
                                                         uint32_t fetch_count,
                                                         int64_t inc_by,
                                                         int64_t min_value,
                                                         int64_t max_value,
                                                         bool cycle);

  Result<std::pair<int64_t, bool>> ReadSequenceTuple(int64_t db_oid,
                                                     int64_t seq_oid,
                                                     uint64_t ysql_catalog_version,
                                                     bool is_db_catalog_version_mode);

  Status DeleteSequenceTuple(int64_t db_oid, int64_t seq_oid);

  Status DeleteDBSequences(int64_t db_oid);

  void PerformAsync(
      tserver::PgPerformOptionsPB* options,
      PgsqlOps* operations,
      const PerformCallback& callback);

  Result<bool> CheckIfPitrActive();

  Result<bool> IsObjectPartOfXRepl(const PgObjectId& table_id);

  Result<tserver::PgGetTserverCatalogVersionInfoResponsePB> GetTserverCatalogVersionInfo(
      bool size_only, uint32_t db_oid);

#define YB_PG_CLIENT_SIMPLE_METHOD_DECLARE(r, data, method) \
  Status method(                             \
      tserver::BOOST_PP_CAT(BOOST_PP_CAT(Pg, method), RequestPB)* req, \
      CoarseTimePoint deadline);

  BOOST_PP_SEQ_FOR_EACH(YB_PG_CLIENT_SIMPLE_METHOD_DECLARE, ~, YB_PG_CLIENT_SIMPLE_METHODS);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace pggate
}  // namespace yb
