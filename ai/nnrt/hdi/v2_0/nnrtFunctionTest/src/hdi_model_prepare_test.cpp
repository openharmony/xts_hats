/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
#include <v2_0/nnrt_types.h>
#include <v2_0/innrt_device.h>
#include <v2_0/iprepared_model.h>

#include "gtest/gtest.h"
#include "mindir.h"
#include "mindir_lite_graph.h"

#include "interfaces/kits/c/neural_network_runtime/neural_network_runtime.h"
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
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V2_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V2_0::PERFORMANCE_NONE;
    config.priority = V2_0::PRIORITY_NONE;
    // prepared model
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->PrepareModel(*iModel, config, iPreparedModel));

    // export model cache
    std::vector<V2_0::SharedBuffer> modelCache;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModel->ExportModelCache(modelCache));

    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    V2_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V2_0::PERFORMANCE_NONE;
    config.priority = V2_0::PRIORITY_NONE;

    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    std::vector<V2_0::SharedBuffer> modelCache;
    // prepared model with empty model cache
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_MODEL_CACHE,
        device_->PrepareModelFromModelCache(modelCache, config, iPreparedModel));
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0200
 * @tc.name   : 加载模型缓存，modelCache不匹配
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_PreparedModelFromCache_0200, Function | MediumTest | Level3)
{
    bool isSupportedCache = false;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V2_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V2_0::PERFORMANCE_NONE;
    config.priority = V2_0::PRIORITY_NONE;
    // export model cache
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->PrepareModel(*iModel, config, iPreparedModel));

    std::vector<V2_0::SharedBuffer> modelCache;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModel->ExportModelCache(modelCache));

    // prepare model from invalid model cache
    OHOS::HDI::Nnrt::V2_0::SharedBuffer invalidBuffer{NNRT_INVALID_FD, 0, 0, 0};
    modelCache.emplace_back(invalidBuffer);
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel1;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_MODEL_CACHE,
        device_->PrepareModelFromModelCache(modelCache, config, iPreparedModel1));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->IsModelCacheSupported(isSupportedCache));
    if (!isSupportedCache) {
        GTEST_SKIP() << "Export cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V2_0::ModelConfig config;
    config.enableFloat16 = false;
    config.mode = V2_0::PERFORMANCE_NONE;
    config.priority = V2_0::PRIORITY_NONE;
    // export model cache
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->PrepareModel(*iModel, config, iPreparedModel));

    std::vector<V2_0::SharedBuffer> modelCache;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModel->ExportModelCache(modelCache));

    // prepare model from invalid model cache
    modelCache.resize(size_t(modelCache.size() * 0.9));
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel1;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_MODEL_CACHE,
        device_->PrepareModelFromModelCache(modelCache, config, iPreparedModel1));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set inputIndex to empty
    iModel->inputIndex = {};
    // prepare model with empty inputIndex
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_INPUT, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set outputIndex to empty
    iModel->outputIndex = {};
    // prepare model with empty outputIndex
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_OUTPUT, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set nodes to empty
    iModel->nodes = {};
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    // prepare model with empty nodes
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_NODE, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set Model.allTensors empty
    iModel->allTensors = {};
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_TENSOR, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set invalid Tensor.DataType
    auto &Tensor = iModel->allTensors[0];
    Tensor.dataType = static_cast<V2_0::DataType>(100000);
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    // prepare model
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_DATATYPE,
        device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set invalid Tensor.Format
    auto &Tensor = iModel->allTensors[0];
    Tensor.format = static_cast<V2_0::Format>(100000);
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_FORMAT, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set empty Model.subGraph
    iModel->subGraph = {};
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_MODEL, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set wrong input of subGraph
    auto &subGraph = iModel->subGraph[0];
    subGraph.inputIndices = {0, 1, 3};
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_NONE, V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_MODEL, device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V2_0::ModelConfig modelConfig{true, static_cast<V2_0::PerformanceMode>(V2_0::PERFORMANCE_NONE - 1),
                                  V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_PERFORMANCE_MODE,
        device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    EXPECT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V2_0::ModelConfig modelConfig{true, static_cast<V2_0::PerformanceMode>(V2_0::PERFORMANCE_EXTREME + 1),
                                  V2_0::PRIORITY_NONE};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_PERFORMANCE_MODE,
        device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    EXPECT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_EXTREME,
                                  static_cast<V2_0::Priority>(V2_0::PRIORITY_NONE - 1)};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_PRIORITY,
        device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
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

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    V2_0::ModelConfig modelConfig{true, V2_0::PERFORMANCE_EXTREME,
                                  static_cast<V2_0::Priority>(V2_0::PRIORITY_HIGH + 1)};
    V2_0::sptr<V2_0::IPreparedModel> preparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_PRIORITY,
        device_->PrepareModel(*iModel, modelConfig, preparedModel));

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_GetInputDimRanges_0100
 * @tc.name   : 返回动态输入范围；定长模型已编译，获取输入范围
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_GetInputDimRanges_0100, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    // conver model from OH_NNModel to V2_0:Model
    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V2_0::ModelConfig modelConfig;
    modelConfig.enableFloat16 = false;
    modelConfig.mode = V2_0::PERFORMANCE_NONE;
    modelConfig.priority = V2_0::PRIORITY_NONE;

    // prepared model
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));

    std::vector<std::vector<uint32_t>> minInputsDim;
    std::vector<std::vector<uint32_t>> maxInputsDim;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModel->GetInputDimRanges(minInputsDim, maxInputsDim));

    printf("the value of minInputsDim : ");
    for (size_t i = 0; i < minInputsDim.size(); ++i) {
        for (size_t j = 0; j < minInputsDim.size(); ++j) {
            printf("%u ", minInputsDim[i][j]);
        }
    }
    printf("\n");

    printf("the value of maxInputsDim : ");
    for (size_t i = 0; i < maxInputsDim.size(); ++i) {
        for (size_t j = 0; j < maxInputsDim.size(); ++j) {
            printf("%u ", maxInputsDim[i][j]);
        }
    }
    printf("\n");

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_GetInputDimRanges_0200
 * @tc.name   : 返回动态输入范围；变长模型已编译，获取输入范围
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_Model_GetInputDimRanges_0200, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraphDynamic(&model);
    ASSERT_NE(model, nullptr);

    // conver model from OH_NNModel to V2_0:Model
    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V2_0::ModelConfig modelConfig;
    modelConfig.enableFloat16 = false;
    modelConfig.mode = V2_0::PERFORMANCE_NONE;
    modelConfig.priority = V2_0::PRIORITY_NONE;

    // prepare model
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));

    std::vector<std::vector<uint32_t>> minInputsDim;
    std::vector<std::vector<uint32_t>> maxInputsDim;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModel->GetInputDimRanges(minInputsDim, maxInputsDim));

    printf("the value of minInputsDim : ");
    for (size_t i = 0; i < minInputsDim.size(); ++i) {
        for (size_t j = 0; j < minInputsDim.size(); ++j) {
            printf("%u ", minInputsDim[i][j]);
        }
    }
    printf("\n");

    printf("the value of maxInputsDim : ");
    for (size_t i = 0; i < maxInputsDim.size(); ++i) {
        for (size_t j = 0; j < maxInputsDim.size(); ++j) {
            printf("%u ", maxInputsDim[i][j]);
        }
    }
    printf("\n");

    // release
    OHOS::NeuralNetworkRuntime::V2::HDIModel_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_PreparedOfflineModel_0100, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    // conver model from OH_NNModel to V2_0:Model
    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    V2_0::ModelConfig modelConfig;
    modelConfig.enableFloat16 = false;
    modelConfig.mode = V2_0::PERFORMANCE_HIGH;
    modelConfig.priority = V2_0::PRIORITY_HIGH;

    // prepare model
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));

    // export model cache
    std::vector<V2_0::SharedBuffer> modelCache;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModel->ExportModelCache(modelCache));

    // prepared model from offline model
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModelFromOffline;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS,
        device_->PrepareOfflineModel(modelCache, modelConfig, iPreparedModelFromOffline));

    std::vector<V2_0::IOTensor> inputs;
    std::vector<V2_0::IOTensor> outputs;
    std::vector<std::vector<int32_t>> outputsDims;
    std::vector<void*> mapedMemorys;

    // set inputs
    std::vector<float> inputValue = {ADD_VALUE_1, ADD_VALUE_2};
    for (uint32_t i = 0; i < inputValue.size(); ++i) {
        std::vector<float> data(ADDEND_DATA_SIZE, inputValue[i]);
        auto tensor = HDICommon::CreateIOTensor(device_);
        auto memAddress = HDICommon::MapMemory(tensor.data.fd, ADDEND_BUFFER_LENGTH);
        mapedMemorys.emplace_back(memAddress);
        // set input data
        HDICommon::SetData((float*)memAddress, ADDEND_BUFFER_LENGTH, (float*)data.data());
        inputs.emplace_back(tensor);
    }
    // set outputs
    auto outputTensor = HDICommon::CreateIOTensor(device_);
    outputs.emplace_back(outputTensor);
    // model run
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, iPreparedModelFromOffline->Run(inputs, outputs, outputsDims));

    // map memory to get output buffer
    auto memAddress = HDICommon:: MapMemory(outputs[0].data.fd, ADDEND_BUFFER_LENGTH);
    mapedMemorys.emplace_back(memAddress);

    auto buffer = (float*)memAddress;
    std::vector<float> expectValue(ADDEND_DATA_SIZE, ADD_VALUE_RESULT);
    std::vector<float> outputValue(buffer, buffer + ADDEND_DATA_SIZE);
    // check output
    EXPECT_TRUE(CheckExpectOutput(outputValue, expectValue)) << "output value check failed.";

    // release
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
    HDICommon::ReleaseBufferOfTensors(device_, inputs);
    HDICommon::ReleaseBufferOfTensors(device_, outputs);
    HDICommon::UnmapAllMemory(mapedMemorys);
}

HWTEST_F(ModelPrepareTest, SUB_AI_NNRt_Func_South_PreparedOfflineModel_0200, Function | MediumTest | Level3)
{
    V2_0::ModelConfig modelConfig;
    modelConfig.enableFloat16 = false;
    modelConfig.mode = V2_0::PERFORMANCE_HIGH;
    modelConfig.priority = V2_0::PRIORITY_HIGH;

    // prepared model from offline model
    std::vector<V2_0::SharedBuffer> modelCache;
    OHOS::sptr<V2_0::IPreparedModel> iPreparedModelFromOffline;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_INVALID_MODEL,
        device_->PrepareOfflineModel(modelCache, modelConfig, iPreparedModelFromOffline));
}