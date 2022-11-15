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
#include "frameworks/native/memory_manager.h"
#include "common/hdi_nnrt_test_utils.h"
#include "common/hdi_nnrt_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::NeuralNetworkRuntime;
using namespace OHOS::NeuralNetworkRuntime::Test;

namespace {

class ModelRunTest : public HDINNRtTest {};

void AddModelTest(OHOS::sptr<V1_0::INnrtDevice> &device_, V1_0::ModelConfig &modelConfig, bool isDynamic)
{
    OH_NNModel *model = nullptr;
    if (isDynamic) {
        HDICommon::BuildAddGraphDynamic(&model);
    } else {
        HDICommon::BuildAddGraph(&model);
    }
    ASSERT_NE(model, nullptr);
    // conver model from OH_NNModel to V1_0:Model
    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // prepare model
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));

    std::vector<V1_0::IOTensor> inputs;
    std::vector<V1_0::IOTensor> outputs;
    std::vector<std::vector<int32_t>> outputsDims;
    std::vector<bool> isOutputBufferEnough;

    std::vector<void* > mapedMemorys;
    // set inputs
    std::vector<float> inputValue = {ADD_VALUE_1, ADD_VALUE_2};
    for (uint32_t i = 0; i < inputValue.size(); i++) {
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
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel->Run(inputs, outputs, outputsDims, isOutputBufferEnough));

    // map memory to get output buffer
    auto memAddress = HDICommon::MapMemory(outputs[0].data.fd, ADDEND_BUFFER_LENGTH);
    mapedMemorys.emplace_back(memAddress);

    auto buffer = (float *)memAddress;
    std::vector<float> expectValue(ADDEND_DATA_SIZE, ADD_VALUE_RESULT);
    std::vector<float> outputValue(buffer, buffer + ADDEND_DATA_SIZE);
    PrintTensor(buffer, ADDEND_DATA_SIZE);
    // check output
    EXPECT_TRUE(CheckExpectOutput(outputValue, expectValue)) << "output value check failed.";

    // release
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
    HDICommon::ReleaseBufferOfTensors(device_, inputs);
    HDICommon::ReleaseBufferOfTensors(device_, outputs);
    HDICommon::UnmapAllMemory(mapedMemorys);
}

} // namespace

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_Run_0200
 * @tc.name   : 定长模型端到端推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_Run_0200, Function | MediumTest | Level1)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_Run_0300
 * @tc.name   : 定长模型端到端推理-fp16
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_Run_0300, Function | MediumTest | Level2)
{
    bool isFloat16Supported = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsFloat16PrecisionSupported(isFloat16Supported));
    if (!isFloat16Supported) {
        GTEST_SKIP() << "Float16 precision is not supported.";
    }

    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = true, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_Run_0400
 * @tc.name   : 变长模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_Run_0400, Function | MediumTest | Level2)
{
    bool isDynamicInputSupported = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsDynamicInputSupported(isDynamicInputSupported));
    if (!isDynamicInputSupported) {
        GTEST_SKIP() << "Dynamic input is not supported.";
    }

    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = true, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
    AddModelTest(device_, modelConfig, true);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_Run_0500
 * @tc.name   : 模型推理，inputs为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_Run_0500, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // model config
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
    // prepared model
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));

    std::vector<V1_0::IOTensor> inputs;
    std::vector<V1_0::IOTensor> outputs;
    std::vector<std::vector<int32_t>> outputsDims;
    std::vector<bool> isOutputBufferEnough;
    vector<void* > mapedMemorys;

    // only set outputs
    auto outputTensor = HDICommon::CreateIOTensor(device_);
    outputs.emplace_back(outputTensor);
    // model run, retcode less than HDF_SUCCESS
    EXPECT_GT(HDF_SUCCESS, iPreparedModel->Run(inputs, outputs, outputsDims, isOutputBufferEnough));

    // release
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
    HDICommon::ReleaseBufferOfTensors(device_, outputs);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_Run_0600
 * @tc.name   : 模型推理，outputs为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_Run_0600, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // model config
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
    // prepared model
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));

    std::vector<V1_0::IOTensor> inputs;
    std::vector<V1_0::IOTensor> outputs;
    std::vector<std::vector<int32_t>> outputsDims;
    std::vector<bool> isOutputBufferEnough;
    vector<void* > mapedMemorys;

    // only set inputs
    std::vector<float> inputValue = {ADD_VALUE_1, ADD_VALUE_2};
    for (uint32_t i = 0; i < inputValue.size(); i++) {
        std::vector<float> data(ADDEND_DATA_SIZE, inputValue[i]);

        auto tensor = HDICommon::CreateIOTensor(device_);
        auto memAddress = HDICommon::MapMemory(tensor.data.fd, ADDEND_BUFFER_LENGTH);
        mapedMemorys.emplace_back(memAddress);
        // set input data
        HDICommon::SetData((float*)memAddress, ADDEND_BUFFER_LENGTH, (float*)data.data());
        inputs.emplace_back(tensor);
    }
    // model run
    EXPECT_GT(HDF_SUCCESS, iPreparedModel->Run(inputs, outputs, outputsDims, isOutputBufferEnough));

    // release
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
    HDICommon::ReleaseBufferOfTensors(device_, inputs);
    HDICommon::UnmapAllMemory(mapedMemorys);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0100
 * @tc.name   : 性能模式设置为PERFORMANCE_NONE，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0100, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_NONE, .priority = V1_0::PRIORITY_MEDIUM};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0200
 * @tc.name   : 性能模式设置为PERFORMANCE_LOW，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0200, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_LOW, .priority = V1_0::PRIORITY_MEDIUM};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0300
 * @tc.name   : 性能模式设置为PERFORMANCE_MEDIUM，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0300, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_MEDIUM, .priority = V1_0::PRIORITY_MEDIUM};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0400
 * @tc.name   : 性能模式设置为PERFORMANCE_HIGH，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0400, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_HIGH, .priority = V1_0::PRIORITY_HIGH};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0500
 * @tc.name   : 性能模式设置为PERFORMANCE_EXTREME，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0500, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_LOW};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0600
 * @tc.name   : 优先级设置为PRIORITY_NONE，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0600, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_NONE};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0700
 * @tc.name   : 优先级设置为PRIORITY_LOW，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0700, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_HIGH, .priority = V1_0::PRIORITY_LOW};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0800
 * @tc.name   : 优先级设置为PRIORITY_MEDIUM，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0800, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_MEDIUM};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0900
 * @tc.name   : 优先级设置为PRIORITY_HIGH，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_0900, Function | MediumTest | Level2)
{
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
    AddModelTest(device_, modelConfig, false);
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_1000
 * @tc.name   : 加载模型缓存，模型推理
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(ModelRunTest, SUB_AI_NNRt_Func_South_Model_Invoke_CombRun_1000, Function | MediumTest | Level1)
{
    bool isModelCacheSupported = false;
    EXPECT_EQ(HDF_SUCCESS, device_->IsModelCacheSupported(isModelCacheSupported));
    if (!isModelCacheSupported) {
        GTEST_SKIP() << "Model cache is not supported.";
    }

    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // model config
    V1_0::ModelConfig modelConfig = {
        .enableFloat16 = false, .mode = V1_0::PERFORMANCE_HIGH, .priority = V1_0::PRIORITY_HIGH};
    // prepared model
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModel(*iModel, modelConfig, iPreparedModel));
    // export model cache
    std::vector<V1_0::SharedBuffer> modelCache;
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel->ExportModelCache(modelCache));
    // prepared model from cache
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel1;
    EXPECT_EQ(HDF_SUCCESS, device_->PrepareModelFromModelCache(modelCache, modelConfig, iPreparedModel1));

    std::vector<V1_0::IOTensor> inputs;
    std::vector<V1_0::IOTensor> outputs;
    std::vector<std::vector<int32_t>> outputsDims;
    std::vector<bool> isOutputBufferEnough;
    vector<void* > mapedMemorys;

    // set inputs
    std::vector<float> inputValue = {ADD_VALUE_1, ADD_VALUE_2};
    for (uint32_t i = 0; i < inputValue.size(); i++) {
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
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel1->Run(inputs, outputs, outputsDims, isOutputBufferEnough));

    // map memory to get output buffer
    auto memAddress = HDICommon::MapMemory(outputs[0].data.fd, ADDEND_BUFFER_LENGTH);
    mapedMemorys.emplace_back(memAddress);

    auto buffer = (float *)memAddress;
    std::vector<float> expectValue(ADDEND_DATA_SIZE, ADD_VALUE_RESULT);
    std::vector<float> outputValue(buffer, buffer + ADDEND_DATA_SIZE);
    // check output
    EXPECT_TRUE(CheckExpectOutput(outputValue, expectValue)) << "output value check failed.";

    // release
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
    HDICommon::ReleaseBufferOfTensors(device_, inputs);
    HDICommon::ReleaseBufferOfTensors(device_, outputs);
    HDICommon::UnmapAllMemory(mapedMemorys);
}
