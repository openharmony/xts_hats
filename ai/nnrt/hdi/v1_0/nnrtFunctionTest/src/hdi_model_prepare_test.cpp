/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>
#include <v1_0/nnrt_types.h>
#include <v1_0/innrt_device.h>
#include <v1_0/iprepared_model.h>

#include "gtest/gtest.h"
#include "mindir.h"
#include "mindir_lite_graph.h"

#include "interfaces/kits/c/neural_network_runtime.h"
#include "common/hdi_nnrt_test_utils.h"
#include "common/hdi_nnrt_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::NeuralNetworkRuntime;
using namespace OHOS::NeuralNetworkRuntime::Test;

namespace {

class ModelPrepareTest : public HDINNRtTest {};

} // namespace

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_ExportModelCache_0100
 * @tc.name   : 模型已编译，导出cache
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_ExportModelCache_0100, Function | MediumTest | Level1)
{
    bool isSupportedCache = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V1_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V1_0::PERFORMANCE_NONE;
    config.priority = V1_0::PRIORITY_NONE;
    // prepared model
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, config, iPreparedModel));
    // export model cache
    std::vector<V1_0::SharedBuffer> modelCache;
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel->ExportModelCache(modelCache));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0100
 * @tc.name   : 加载模型缓存，modelCache为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0100, Function | MediumTest | Level3)
{
    bool isSupportedCache = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    V1_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V1_0::PERFORMANCE_NONE;
    config.priority = V1_0::PRIORITY_NONE;

    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    std::vector<V1_0::SharedBuffer> modelCache;
    // prepared model with empty model cache
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModelFromModelCache(modelCache, config, iPreparedModel));
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0200
 * @tc.name   : 加载模型缓存，modelCache不匹配
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0200, Function | MediumTest | Level3)
{
    bool isSupportedCache = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V1_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V1_0::PERFORMANCE_NONE;
    config.priority = V1_0::PRIORITY_NONE;
    // export model cache
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, config, iPreparedModel));
    std::vector<V1_0::SharedBuffer> modelCache;
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel->ExportModelCache(modelCache));

    // prepare model from invalid model cache
    OHOS::HDI::Nnrt::V1_0::SharedBuffer invalidBuffer{NNRT_INVALID_FD, 0, 0, 0};
    modelCache.emplace_back(invalidBuffer);
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel1;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModelFromModelCache(modelCache, config, iPreparedModel1));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0300
 * @tc.name   : 加载模型缓存，modelCache不完整
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0300, Function | MediumTest | Level3)
{
    bool isSupportedCache = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V1_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V1_0::PERFORMANCE_NONE;
    config.priority = V1_0::PRIORITY_NONE;
    // export model cache
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, config, iPreparedModel));
    std::vector<V1_0::SharedBuffer> modelCache;
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel->ExportModelCache(modelCache));
    // prepare model from invalid model cache
    modelCache.resize(size_t(modelCache.size() * 0.9));
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel1;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModelFromModelCache(modelCache, config, iPreparedModel1));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0100
 * @tc.name   : 编译模型，model中inputIndex为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0100, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set inputIndex to empty
    iModel->inputIndex = {};
    // prepare model with empty inputIndex
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}
/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0200
 * @tc.name   : 编译模型，model中outputIndex为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0200, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set outputIndex to empty
    iModel->outputIndex = {};
    // prepare model with empty outputIndex
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0300
 * @tc.name   : 编译模型，model中nodes为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0300, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set nodes to empty
    iModel->nodes = {};
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    // prepare model with empty nodes
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0400
 * @tc.name   : 编译模型，model中allTensors为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0400, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set Model.allTensors empty
    iModel->allTensors = {};
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0500
 * @tc.name   : 编译模型，Tensor的DataTyp为100000
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0500, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set invalid Tensor.DataType
    auto &Tensor = iModel->allTensors[0];
    Tensor.dataType = static_cast<V1_0::DataType>(100000);
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    // prepare model
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0600
 * @tc.name   : 编译模型，Tensor的Format值为100000
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0600, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set invalid Tensor.Format
    auto &Tensor = iModel->allTensors[0];
    Tensor.format = static_cast<V1_0::Format>(100000);
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0700
 * @tc.name   : 编译模型，model中subGraph为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0700, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set empty Model.subGraph
    iModel->subGraph = {};
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0800
 * @tc.name   : 编译模型，model中subGraph输入输出错误
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0800, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set wrong input of subGraph
    auto &subGraph = iModel->subGraph[0];
    subGraph.inputIndices = {0, 1, 3};
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_NONE, V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_FAILURE, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_0900
 * @tc.name   : 编译模型，config中mode为PERFORMANCE_NONE-1
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_0900, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V1_0::ModelConfig modelConfig{true, static_cast<V1_0::PerformanceMode>(V1_0::PERFORMANCE_NONE - 1),
                                  V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_1000
 * @tc.name   : 编译模型，config中mode为PERFORMANCE_EXTREME+1
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_1000, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    EXPECT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V1_0::ModelConfig modelConfig{true, static_cast<V1_0::PerformanceMode>(V1_0::PERFORMANCE_EXTREME + 1),
                                  V1_0::PRIORITY_NONE};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_1100
 * @tc.name   : 编译模型，config中priority为PRIORITY_NONE-1
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_1100, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    EXPECT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_EXTREME,
                                  static_cast<V1_0::Priority>(V1_0::PRIORITY_NONE - 1)};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModel_1200
 * @tc.name   : 编译模型，config中priority为PRIORITY_HIGH+1
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModel_1200, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V1_0::ModelConfig modelConfig{true, V1_0::PERFORMANCE_EXTREME,
                                  static_cast<V1_0::Priority>(V1_0::PRIORITY_HIGH + 1)};
    V1_0::sptr<V1_0::IPreparedModel> preparedModel;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}