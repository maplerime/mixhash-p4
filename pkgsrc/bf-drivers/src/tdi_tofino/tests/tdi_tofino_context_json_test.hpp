/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2021 Intel Corporation
 *  All Rights Reserved.
 *
 *  This software and the related documents are Intel copyrighted materials,
 *  and your use of them is governed by the express license under which they
 *  were provided to you ("License"). Unless the License provides otherwise,
 *  you may not use, modify, copy, publish, distribute, disclose or transmit
 *  this software or the related documents without Intel's prior written
 *  permission.
 *
 *  This software and the related documents are provided as is, with no express
 *  or implied warranties, other than those that are expressly stated in the
 *  License.
 ******************************************************************************/

#ifndef _TDI_TOFINO_CONTEXT_JSON_TEST_HPP
#define _TDI_TOFINO_CONTEXT_JSON_TEST_HPP

#include <stdio.h>
#include <assert.h>
#include <sched.h>
#include <string.h>

#include <memory>
#include <map>
#include <tuple>

#include <tdi/common/tdi_defs.h>
#include <../tdi_common/tdi_tofino_info.hpp>
#include <tdi/common/tdi_info.hpp>
#include <tdi/common/tdi_learn.hpp>
#include <tdi/common/tdi_table.hpp>
#include <tdi/common/tdi_table_data.hpp>
#include <tdi/common/tdi_table_key.hpp>

#include <tdi_tofino/tdi_tofino_defs.h>

#include <tdi/common/tdi_json_parser/tdi_cjson.hpp>
#include <../tdi_common/tdi_context_info.hpp>

#include "tdi_pipe_mgr_intf_mock.hpp"

#include "../tdi_common/tdi_pipe_mgr_intf.hpp"

namespace tdi {
namespace tna {
namespace tofino {
namespace tofino_test {

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::WithArgs;

namespace {

std::string getTestJsonFileContent(const std::string &filename,
                                   const std::string &program_name) {
  std::string temp_file_name =
      std::string(JSONDIR) + "/" + program_name + "/" + filename;
  std::ifstream file(temp_file_name);
  if (file.fail()) {
    LOG_CRIT("Unable to find Json File %s", temp_file_name.c_str());
    return "";
  }
  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  return content;
}

template <class T>
void setupInternal(T *obj) {
  auto t = obj->GetParam();
  std::string tdi_json_filename = std::get<0>(t);
  std::vector<std::string> &context_json_filenames = std::get<1>(t);
  std::vector<std::vector<bf_dev_pipe_t>> &pipe_scope_vec = std::get<2>(t);
  std::vector<std::string> &pipeline_profile_vec = std::get<3>(t);
  std::string program_name = obj->program_name;

  std::string tdi_json_filePath =
      std::string(JSONDIR) + "/" + program_name + "/" + tdi_json_filename;

  std::string dummy_binary_path = "dummy_binary_file_path";
  std::vector<tdi::P4Pipeline> p4_pipelines;
  for (uint32_t i = 0; i < context_json_filenames.size(); i++) {
    std::string context_json_filePath =
        std::string(JSONDIR) + std::string("/") + program_name +
        std::string("/") + context_json_filenames[i];
    p4_pipelines.emplace_back(pipeline_profile_vec[i],
                              context_json_filePath,
                              dummy_binary_path,
                              pipe_scope_vec[i]);
  }

  obj->tdi_info = nullptr;
  std::vector<std::string> tdi_json_file_path_vect;
  tdi_json_file_path_vect.push_back(tdi_json_filePath);

  auto *pipe_mgr_obj = MockIPipeMgrIntf::getInstance();

  // Set the action on call to pipeMgrTblHdlPipeMaskGet
  ON_CALL(*pipe_mgr_obj, pipeMgrTblHdlPipeMaskGet(_, _, _, _))
      .WillByDefault(WithArgs<3>(Invoke([](uint32_t *mask) {
        *mask = 0;
        return PIPE_SUCCESS;
      })));

  ProgramConfig program_config(
      obj->program_name, tdi_json_file_path_vect, p4_pipelines);

  // Parse tdi.json
  auto tdi_info_mapper = std::unique_ptr<tdi::TdiInfoMapper>(
      new tdi::tna::tofino::TdiInfoMapper());
  auto table_factory =
      std::unique_ptr<tdi::TableFactory>(new tdi::tna::tofino::TableFactory());

  auto tdi_info_parser = std::unique_ptr<TdiInfoParser>(
      new TdiInfoParser(std::move(tdi_info_mapper)));
  auto status =
      tdi_info_parser->parseTdiInfo(program_config.tdi_info_file_paths_);
  ASSERT_EQ(status, TDI_SUCCESS)
      << "Failed to parse file : " << program_config.tdi_info_file_paths_[0];

  obj->tdi_info = std::move(tdi::TdiInfo::makeTdiInfo(
      std::move(tdi_info_parser), table_factory.get()));

  ASSERT_NE(obj->tdi_info, nullptr) << "Failed to open json files";

  // Parse context json
  auto context_status =
      parseContextJson(obj->tdi_info.get(), 0, program_config);
  if (context_status) {
    LOG_ERROR("%s:%d Failed to parse context.json", __func__, __LINE__);
  }
}
}  // Anonymous namespace

class ContextInfoTest : public ::testing::TestWithParam<
                            std::tuple<std::string,
                                       std::vector<std::string>,
                                       std::vector<std::vector<bf_dev_pipe_t>>,
                                       std::vector<std::string>,
                                       std::string>> {
 public:
  ContextInfoTest() : tdi_info(nullptr){};
  virtual void SetUp() {
    auto t = this->GetParam();
    program_name = std::get<4>(t);
    setupInternal<ContextInfoTest>(this);
    auto root = Cjson::createCjsonFromFile(
        getTestJsonFileContent(std::get<0>(t), program_name));
    // Now set up cjson for ground truth
    tdi_root_cjson = std::unique_ptr<Cjson>(new Cjson(root));
  }
  virtual void TearDown() {
    tdi_info.reset(nullptr);
    tdi_root_cjson.reset(nullptr);
    std::unique_ptr<IPipeMgrIntf> &unique_ptr_mock =
        MockIPipeMgrIntf::getInstanceToUniquePtr();
    unique_ptr_mock.reset();
  }
  std::unique_ptr<const TdiInfo> tdi_info;
  // TdiInfo *tdi_info;
  std::unique_ptr<Cjson> tdi_root_cjson;
  std::string program_name;
};

class TnaExactMatchTest : public ContextInfoTest {};
class TnaActionProfileTest : public ContextInfoTest {};

INSTANTIATE_TEST_CASE_P(TofinoContextJsonTest,
                        TnaExactMatchTest,
                        ::testing::Values(std::make_tuple(
                            "bf-rt.json",
                            std::vector<std::string>{"pipe/context.json"},
                            std::vector<std::vector<bf_dev_pipe_t>>{
                                {0, 1, 2, 3}},
                            std::vector<std::string>{"pipe"},
                            "tna_exact_match")));

INSTANTIATE_TEST_CASE_P(TofinoContextJsonTest,
                        TnaActionProfileTest,
                        ::testing::Values(std::make_tuple(
                            "bf-rt.json",
                            std::vector<std::string>{"pipe/context.json"},
                            std::vector<std::vector<bf_dev_pipe_t>>{
                                {0, 1, 2, 3}},
                            std::vector<std::string>{"pipe"},
                            "tna_action_profile")));

}  // namespace tofino_test
}  // namespace tofino
}  // namespace tna
}  // namespace tdi

#endif
