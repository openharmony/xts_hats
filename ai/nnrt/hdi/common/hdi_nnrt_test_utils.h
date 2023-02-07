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

#ifndef HDI_NNRT_TEST_UTILS_H
#define HDI_NNRT_TEST_UTILS_H

#include <string>
#include <vector>
#include <v1_0/innrt_device.h>
#include <v1_0/nnrt_types.h>

#include "gtest/gtest.h"
#include "mindir_lite_graph.h"

#include "interfaces/kits/c/neural_network_runtime_type.h"

namespace V1_0 = OHOS::HDI::Nnrt::V1_0;

namespace OHOS::NeuralNetworkRuntime::Test {
// invaild file discription
const int NNRT_INVALID_FD = -1;
const uint32_t ADDEND_DATA_SIZE = 12;
const uint32_t ADDEND_BUFFER_LENGTH = ADDEND_DATA_SIZE * sizeof(float);
const std::vector<int32_t> TENSOR_DIMS = {3, 2, 2};
const float ADD_VALUE_1 = 1;
const float ADD_VALUE_2 = 2;
const float ADD_VALUE_RESULT = 3;
const size_t PRINT_NUM = 10;

class HDICommon {
public:
    static void BuildAddGraph(OH_NNModel **model);
    static void BuildAddGraphDynamic(OH_NNModel **model);
    static OH_NN_ReturnCode ConvertModel(OHOS::sptr<V1_0::INnrtDevice> device_, OH_NNModel *model,
                                         V1_0::SharedBuffer &tensorBuffer, V1_0::Model **iModel);
    static V1_0::IOTensor CreateIOTensor(OHOS::sptr<V1_0::INnrtDevice> &device);
    static V1_0::IOTensor CreateInputIOTensor(OHOS::sptr<V1_0::INnrtDevice> &device, size_t length, float* data);
    static V1_0::IOTensor CreateOutputIOTensor(OHOS::sptr<V1_0::INnrtDevice> &device, size_t length);
    static void* MapMemory(int fd, size_t length);
    static void UnmapMemory(float* buffer);
    static void UnmapAllMemory(std::vector<void* > &buffers);
    static void SetData(float* buffer, size_t length, float* data);
    static void ReleaseBufferOfTensors(OHOS::sptr<V1_0::INnrtDevice> &device, std::vector<V1_0::IOTensor> &tensors);
};

bool CheckExpectOutput(const std::vector<float> &output, const std::vector<float> &expect);
void PrintTensor(const float *buffer, size_t length);
} // namespace OHOS::NeuralNetworkRuntime::Test

#endif