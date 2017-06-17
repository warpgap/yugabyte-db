//--------------------------------------------------------------------------------------------------
// Copyright (c) YugaByte, Inc.
//
// Parameters for executing a SQL statement.
//--------------------------------------------------------------------------------------------------

#ifndef YB_SQL_UTIL_STATEMENT_PARAMS_H_
#define YB_SQL_UTIL_STATEMENT_PARAMS_H_

#include <boost/thread/shared_mutex.hpp>

#include "yb/common/yql_protocol.pb.h"
#include "yb/common/yql_value.h"

namespace yb {
namespace sql {

// This class represents the parameters for executing a SQL statement.
class StatementParameters {
 public:
  // Public types.
  typedef std::unique_ptr<StatementParameters> UniPtr;
  typedef std::unique_ptr<const StatementParameters> UniPtrConst;

  // Constructors
  StatementParameters();
  StatementParameters(const StatementParameters& other);
  virtual ~StatementParameters();

  // Accessor functions for page_size.
  uint64_t page_size() const { return page_size_; }
  void set_page_size(const uint64_t page_size) { page_size_ = page_size; }

  // Set paging state.
  CHECKED_STATUS set_paging_state(const std::string& paging_state) {
    // For performance, create YQLPagingStatePB by demand only when setting paging state because
    // only SELECT statements continuing from a previous page carry a paging state.
    if (paging_state_ == nullptr) {
      paging_state_.reset(new YQLPagingStatePB());
    }
    return paging_state_->ParseFromString(paging_state) ?
        Status::OK() : STATUS(Corruption, "invalid paging state");
  }

  // Accessor functions for paging state fields.
  const std::string& table_id() const { return paging_state().table_id(); }

  const std::string& next_partition_key() const { return paging_state().next_partition_key(); }

  const std::string& next_row_key() const { return paging_state().next_row_key(); }

  int64_t total_num_rows_read() const { return paging_state().total_num_rows_read(); }

  // Retrieve a bind variable for the execution of the statement. To be overridden by subclasses
  // to return actual bind variables.
  virtual CHECKED_STATUS GetBindVariable(const std::string& name,
                                         int64_t pos,
                                         const std::shared_ptr<YQLType>& type,
                                         YQLValue* value) const {
    return STATUS(RuntimeError, "no bind variable available");
  }

 private:
  const YQLPagingStatePB& paging_state() const {
    return paging_state_ != nullptr ? *paging_state_ : YQLPagingStatePB::default_instance();
  }

  // Limit of the number of rows to return set as page size.
  uint64_t page_size_;

  // Paging State.
  std::unique_ptr<YQLPagingStatePB> paging_state_;
};

} // namespace sql
} // namespace yb

#endif  // YB_SQL_UTIL_STATEMENT_PARAMS_H_
