/*
 * Copyright (c) 2022-2022 Huawei Device Co., Ltd.
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

#include <sstream>
#include <vector>
#include <thread>
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

// number of thread to create
const int THREAD_NUM = 3;

// number of times to run
const int STRESS_COUNT = 100000;

// number of print frequency
const int PRINT_FREQ = 500;

class StabilityTest : public HDINNRtTest {};

void PrepareModelTest(OHOS::sptr<V1_0::INnrtDevice> device, V1_0::Model *iModel)
{
    OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
    V1_0::ModelConfig config;
    EXPECT_EQ(HDF_SUCCESS, device->PrepareModel(*iModel, config, iPreparedModel));
}

void RunModelTest(OHOS::sptr<V1_0::INnrtDevice> device, OHOS::sptr<V1_0::IPreparedModel> iPreparedModel)
{
    std::vector<V1_0::IOTensor> inputs;
    std::vector<V1_0::IOTensor> outputs;
    std::vector<std::vector<int32_t>> outputsDims;
    std::vector<bool> isOutputBufferEnough;
    std::vector<void* > mapedMemorys;

    // set inputs
    std::vector<float> inputValue = {ADD_VALUE_1, ADD_VALUE_2};
    for (uint32_t i = 0; i < inputValue.size(); i++) {
        std::vector<float> data(ADDEND_DATA_SIZE, inputValue[i]);

        auto tensor = HDICommon::CreateIOTensor(device);
        auto memAddress = HDICommon::MapMemory(tensor.data.fd, ADDEND_BUFFER_LENGTH);
        mapedMemorys.emplace_back(memAddress);
        // set input data
        HDICommon::SetData((float*)memAddress, ADDEND_BUFFER_LENGTH, (float*)data.data());
        inputs.emplace_back(tensor);
    }
    // set outputs
    auto outputTensor = HDICommon::CreateIOTensor(device);
    outputs.emplace_back(outputTensor);
    // model run
    EXPECT_EQ(HDF_SUCCESS, iPreparedModel->Run(inputs, outputs, outputsDims, isOutputBufferEnough));

    // map memory to get output buffer
    auto memAddress = HDICommon::MapMemory(outputs[0].data.fd, ADDEND_BUFFER_LENGTH);
    mapedMemorys.emplace_back(memAddress);

    auto buffer = (float *)memAddress;
    std::vector<float> expectValue(ADDEND_DATA_SIZE, ADD_VALUE_RESULT);
    std::vector<float> outputValue(buffer, buffer + ADDEND_DATA_SIZE);
    // check output
    EXPECT_TRUE(CheckExpectOutput(outputValue, expectValue)) << "output value check failed.";
}

} // namespace

/**
 * @tc.number : SUB_AI_NNRt_Reliability_South_Stress_0100
 * @tc.name   : 多线程并发模型编译，编译成功
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(StabilityTest, SUB_AI_NNRt_Reliability_South_Stress_0100, Reliability | MediumTest | Level2)
{
    OHOS::sptr<V1_0::INnrtDevice> device = V1_0::INnrtDevice::Get();
    std::vector<V1_0::Model *> iModels;
    std::vector<OHOS::sptr<V1_0::IPreparedModel>> iPreparedModels;
    std::vector<V1_0::SharedBuffer> tensorBuffers;
    for (int i = 0; i < THREAD_NUM; i++) {
        // build graph with NNModel
        OH_NNModel *model = nullptr;
        HDICommon::BuildAddGraph(&model);
        // convert NNModel to V1_0::Model
        V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
        V1_0::Model *iModel = nullptr;
        auto retConvert = HDICommon::ConvertModel(device, model, tensorBuffer, &iModel);
        EXPECT_EQ(OH_NN_SUCCESS, retConvert) << "ConvertModel failed";
        if (retConvert != OH_NN_SUCCESS) {
            break;
        }
        iModels.emplace_back(iModel);
        tensorBuffers.emplace_back(tensorBuffer);
    }
    for (int i = 0; i < STRESS_COUNT; i++) {
        // create threads to prepare model
        std::vector<std::thread> threads;
        for (auto &iModel : iModels) {
            threads.emplace_back(std::thread(PrepareModelTest, device, iModel));
        }
        // wait for thread finish
        for (auto &th : threads) {
            th.join();
        }
        if (i % PRINT_FREQ == 0) {
            printf("[NnrtTest] SUB_AI_NNRt_Reliability_South_Stress_0100 times: %d/%d\n", i, STRESS_COUNT);
        }
    }
    for (int i=0; i< iModels.size(); i++) {
        mindspore::lite::MindIR_Model_Destroy(&iModels[i]);
        if (tensorBuffers[i].fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffers[i]));
        }
    }
}

/**
 * @tc.number : SUB_AI_NNR_Reliability_South_Stress_0200
 * @tc.name   : 模型推理多线程并发长稳测试
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(StabilityTest, SUB_AI_NNR_Reliability_South_Stress_0200, Reliability | MediumTest | Level2)
{
    OHOS::sptr<V1_0::INnrtDevice> device = V1_0::INnrtDevice::Get();

    std::vector<V1_0::Model *> iModels;
    std::vector<OHOS::sptr<V1_0::IPreparedModel>> iPreparedModels;
    std::vector<V1_0::SharedBuffer> tensorBuffers;
    for (int i = 0; i < THREAD_NUM; i++) {
        // build graph with NNModel
        OH_NNModel *model = nullptr;
        HDICommon::BuildAddGraph(&model);
        // convert NNModel to V1_0::Model
        V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
        V1_0::Model *iModel = nullptr;
        auto retConvert = HDICommon::ConvertModel(device, model, tensorBuffer, &iModel);
        EXPECT_EQ(OH_NN_SUCCESS, retConvert) << "ConvertModel failed";
        if (retConvert != OH_NN_SUCCESS) {
            break;
        }
        iModels.emplace_back(iModel);
        tensorBuffers.emplace_back(tensorBuffer);
        // prepare model
        OHOS::sptr<V1_0::IPreparedModel> iPreparedModel;
        V1_0::ModelConfig config = {
            .enableFloat16 = false, .mode = V1_0::PERFORMANCE_EXTREME, .priority = V1_0::PRIORITY_HIGH};
        auto retPrepare = device->PrepareModel(*iModel, config, iPreparedModel);
        EXPECT_EQ(HDF_SUCCESS, retPrepare) << "PrepareModel failed";
        if (retPrepare != HDF_SUCCESS) {
            break;
        }
        iPreparedModels.emplace_back(iPreparedModel);
    }
    for (int i = 0; i < STRESS_COUNT; i++) {
        // create threads to run model
        std::vector<std::thread> threads;
        for (auto &iPreparedModel : iPreparedModels) {
            threads.emplace_back(std::thread(RunModelTest, device, iPreparedModel));
        }
        // wait for thread finish
        for (auto &th : threads) {
            th.join();
        }
        if (i % PRINT_FREQ == 0) {
            printf("[NnrtTest] SUB_AI_NNRt_Reliability_South_Stress_0200 times: %d/%d\n", i, STRESS_COUNT);
        }
    }
    for (size_t i=0; i< iModels.size(); i++) {
        mindspore::lite::MindIR_Model_Destroy(&iModels[i]);
        if (tensorBuffers[i].fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffers[i]));
        }
    }
}