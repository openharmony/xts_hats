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

#include <sstream>
#include <algorithm>
#include <v1_0/nnrt_types.h>

#include "securec.h"
#include "interfaces/kits/c/neural_network_runtime.h"
#include "frameworks/native/inner_model.h"
#include "frameworks/native/memory_manager.h"
#include "hdi_nnrt_test_utils.h"

namespace OHOS::NeuralNetworkRuntime::Test {

void HDICommon::BuildAddGraph(OH_NNModel **model)
{
    printf("[NNRtTest] BuildAddGraph start.\n");
    // create OH_NNModel
    *model = OH_NNModel_Construct();
    ASSERT_NE(nullptr, model);
    // add inputs of Add operation
    int32_t dimensions[3]{3, 2, 2};
    OH_NN_Tensor firstAddend{OH_NN_FLOAT32, 3, dimensions, nullptr, OH_NN_TENSOR};
    OH_NN_Tensor secondAddend{OH_NN_FLOAT32, 3, dimensions, nullptr, OH_NN_TENSOR};
    uint32_t inputIndicesValue[2]{0, 1};
    OH_NN_UInt32Array inputIndices{inputIndicesValue, 2};
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &firstAddend));
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &secondAddend));

    // Add activation type and set value
    OH_NN_Tensor activationType{OH_NN_INT8, 0, nullptr, nullptr, OH_NN_ADD_ACTIVATIONTYPE};
    int8_t activationValue{0};
    uint32_t paramIndicesValue{2};
    OH_NN_UInt32Array paramIndices{&paramIndicesValue, 1};
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &activationType));
    int opCnt = 2;
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_SetTensorData(*model, opCnt, (void *)&activationValue, sizeof(int8_t)));

    // add output of Add operation
    OH_NN_Tensor output{OH_NN_FLOAT32, 3, dimensions, nullptr, OH_NN_TENSOR};
    uint32_t outputIndicesValue{3};
    OH_NN_UInt32Array outputIndices{&outputIndicesValue, 1};
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &output));

    // add Add operation to model
    ASSERT_EQ(OH_NN_SUCCESS,
              OH_NNModel_AddOperation(*model, OH_NN_OPS_ADD, &paramIndices, &inputIndices, &outputIndices));
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_SpecifyInputsAndOutputs(*model, &inputIndices, &outputIndices));
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_Finish(*model));

    printf("[NNRtTest] BuildAddGraph done.\n");
}

void HDICommon::BuildAddGraphDynamic(OH_NNModel **model)
{
    printf("[NNRtTest] BuildAddGraphDynamic start.\n");
    // create OH_NNModel
    *model = OH_NNModel_Construct();
    ASSERT_NE(nullptr, *model);
    // add inputs of Add operation
    int32_t dimensions[3]{-1, -1, -1};
    OH_NN_Tensor firstAddend{OH_NN_FLOAT32, 3, dimensions, nullptr, OH_NN_TENSOR};
    OH_NN_Tensor secondAddend{OH_NN_FLOAT32, 3, dimensions, nullptr, OH_NN_TENSOR};
    uint32_t inputIndicesValue[2]{0, 1};
    OH_NN_UInt32Array inputIndices{inputIndicesValue, 2};
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &firstAddend));
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &secondAddend));

    // Add activation type and set value
    OH_NN_Tensor activationType{OH_NN_INT8, 0, nullptr, nullptr, OH_NN_ADD_ACTIVATIONTYPE};
    int8_t activationValue{OH_NN_FUSED_NONE};
    uint32_t paramIndicesValue{2};
    OH_NN_UInt32Array paramIndices{&paramIndicesValue, 1};
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &activationType));
    int opCnt = 2;
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_SetTensorData(*model, opCnt, (void *)&activationValue, sizeof(int8_t)));

    // add output of Add operation
    OH_NN_Tensor output{OH_NN_FLOAT32, 3, dimensions, nullptr, OH_NN_TENSOR};
    uint32_t outputIndicesValue{3};
    OH_NN_UInt32Array outputIndices{&outputIndicesValue, 1};
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_AddTensor(*model, &output));

    // add Add operation to model
    ASSERT_EQ(OH_NN_SUCCESS,
              OH_NNModel_AddOperation(*model, OH_NN_OPS_ADD, &paramIndices, &inputIndices, &outputIndices));
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_SpecifyInputsAndOutputs(*model, &inputIndices, &outputIndices));
    ASSERT_EQ(OH_NN_SUCCESS, OH_NNModel_Finish(*model));

    printf("[NNRtTest] BuildAddGraphDynamic done\n");
}

OH_NN_ReturnCode HDICommon::ConvertModel(OHOS::sptr<V1_0::INnrtDevice> device_, OH_NNModel *model,
                                         V1_0::SharedBuffer &tensorBuffer, V1_0::Model **iModel)
{
    printf("[NNRtTest] [ConvertModel] convert OH_NNModel to V1_0::Model\n");
    auto *innerModel = reinterpret_cast<InnerModel *>(model);
    std::shared_ptr<mindspore::lite::LiteGraph> m_liteGraph = innerModel->GetLiteGraphs();
    if (m_liteGraph == nullptr) {
        printf("[NNRtTest] Model is nullptr, cannot query supported operation.");
        return OH_NN_NULL_PTR;
    }

    size_t tensorSize = mindspore::lite::MindIR_LiteGraph_GetConstTensorSize(m_liteGraph.get());
    std::cout << "[ConvertModel] const tensorsize:" << tensorSize << std::endl;
    int32_t hdiRet{0};
    if (tensorSize > 0) {
        hdiRet = device_->AllocateBuffer(tensorSize, tensorBuffer);
        if (hdiRet != HDF_SUCCESS || tensorBuffer.fd == NNRT_INVALID_FD) {
            printf("[NNRtTest] [ConvertModel] allocate tensor buffer failed after get const tensor size,"\
                "ret:%d\n", hdiRet);
            return OH_NN_FAILED;
        }
    }
    *iModel = mindspore::lite::MindIR_LiteGraph_To_Model(m_liteGraph.get(), tensorBuffer);
    if (iModel == nullptr) {
        printf("[NNRtTest] Parse litegraph to hdi model failed.\n");
        device_->ReleaseBuffer(tensorBuffer);
        return OH_NN_FAILED;
    }
    // release model
    OH_NNModel_Destroy(&model);
    model = nullptr;
    printf("[NNRtTest] [ConvertModel] convert model done\n");
    return OH_NN_SUCCESS;
}

V1_0::IOTensor HDICommon::CreateIOTensor(OHOS::sptr<V1_0::INnrtDevice> &device)
{
    V1_0::SharedBuffer buffer{NNRT_INVALID_FD, 0, 0, 0};
    int ret = device->AllocateBuffer(ADDEND_BUFFER_LENGTH, buffer);
    if (ret != HDF_SUCCESS || buffer.fd == NNRT_INVALID_FD) {
        printf("[NNRtTest] [CreateIOTensor] allocate buffer error. ret: %d, fd: %d\n", ret, buffer.fd);
    }
    V1_0::IOTensor tensor{.name = "tensor",
                          .dataType = V1_0::DATA_TYPE_FLOAT32,
                          .dimensions = TENSOR_DIMS,
                          .format = V1_0::FORMAT_NHWC,
                          .data = buffer};
    return tensor;
}

V1_0::IOTensor HDICommon::CreateInputIOTensor(OHOS::sptr<V1_0::INnrtDevice> &device, size_t length, float* data)
{
    if (length == 0) {
        std::cout << "The length param is invalid, length=0" << std::endl;
    }

    V1_0::SharedBuffer buffer{NNRT_INVALID_FD, 0, 0, 0};
    auto ret = device->AllocateBuffer(length, buffer);

    if (ret != HDF_SUCCESS || buffer.fd == NNRT_INVALID_FD) {
        printf("[NNRtTest] [CreateInputIOTensor] allocate buffer error. ret: %d, fd: %d\n", ret, buffer.fd);
    }

    auto memManager = MemoryManager::GetInstance();
    auto memAddress = memManager->MapMemory(buffer.fd, length);
    if (memAddress == nullptr) {
        printf("[NNRtTest] [CreateInputIOTensor] map fd to address failed.\n");
    }
    memcpy_s(memAddress, length, data, length);

    V1_0::IOTensor tensor{.name = "tensor",
                          .dataType = V1_0::DATA_TYPE_FLOAT32,
                          .dimensions = {3, 2, 2},
                          .format = V1_0::FORMAT_NHWC,
                          .data = buffer};
    return tensor;
}

V1_0::IOTensor HDICommon::CreateOutputIOTensor(OHOS::sptr<V1_0::INnrtDevice> &device, size_t length)
{
    if (length == 0) {
        printf("[NNRtTest] The length param is invalid, length=0");
    }

    V1_0::SharedBuffer buffer{NNRT_INVALID_FD, 0, 0, 0};
    int ret = device->AllocateBuffer(length, buffer);

    if (ret != HDF_SUCCESS || buffer.fd == NNRT_INVALID_FD) {
        printf("[NNRtTest] Allocate buffer error. ErrorCode: %d, fd: %d", ret, buffer.fd);
    }

    V1_0::IOTensor tensor{.name = "tensor",
                          .dataType = V1_0::DATA_TYPE_FLOAT32,
                          .dimensions = {3, 2, 2},
                          .format = V1_0::FORMAT_NHWC,
                          .data = buffer};
    return tensor;
}

void* HDICommon::MapMemory(int fd, size_t length)
{
    auto memManager = MemoryManager::GetInstance();
    auto memAddress = memManager->MapMemory(fd, length);
    if (memAddress == nullptr) {
        printf("[NNRtTest] Map fd to address failed.");
        return nullptr;
    }
    return memAddress;
}

void HDICommon::UnmapMemory(float* buffer)
{
    auto memManager = MemoryManager::GetInstance();
    auto ret = memManager->UnMapMemory(buffer);
    if (ret != OH_NN_SUCCESS) {
        printf("[NNRtTest] [UnmapMemory] unmap memory failed. ret:%d.\n", ret);
    }
}

void HDICommon::SetData(float* buffer, size_t length, float* data)
{
    if (buffer == nullptr || data == nullptr) {
        printf("[NNRtTest] [SetData] buffer or data is nullprt\n");
        return;
    }
    int ret = memcpy_s(buffer, length, data, length);
    if (ret != 0) {
        printf("[NNRtTest] [SetData] set data failed, error code: %d\n", ret);
    }
}

void HDICommon::ReleaseBufferOfTensors(OHOS::sptr<V1_0::INnrtDevice> &device, std::vector<V1_0::IOTensor> &tensors)
{
    if (device == nullptr) {
        printf("[NNRtTest] [ReleaseBufferOfTensors] device is nullptr.\n");
        return;
    }

    for (auto &tensor : tensors) {
        auto ret = device->ReleaseBuffer(tensor.data);
        if (ret != HDF_SUCCESS) {
            printf("[NNRtTest] [ReleaseBufferOfTensors] release buffer failed, fd:%d ret:%d.\n", tensor.data.fd, ret);
        }
    }
}

void HDICommon::UnmapAllMemory(std::vector<void* > &buffers)
{
    auto memoryMenager = MemoryManager::GetInstance();
    for (auto buffer : buffers) {
        auto ret = memoryMenager->UnMapMemory(buffer);
        if (ret != OH_NN_SUCCESS) {
            printf("[NNRtTest] [UnmapAllMemory] release buffer failed, ret:%d.\n", ret);
        }
    }
}

bool CheckExpectOutput(const std::vector<float> &output, const std::vector<float> &expect)
{
    if (output.empty() || expect.empty()) {
        printf("[NNRtTest] [CheckExpectOutput] output or expect is empty.\n");
        return false;
    }
    int outputSize = output.size();
    int expectSize = expect.size();
    if (outputSize != expectSize) {
        printf("[NNRtTest] [CheckExpectOutput] output size not match: expect:%d, actual:%d\n", outputSize, expectSize);
        return false;
    }
    for (int i = 0; i < outputSize; i++) {
        if (std::abs(float(output[i]) - float(expect[i])) > 1e-8) {
            printf("[NNRtTest] [CheckExpectOutput] output %d not match: expect:%f, actual:%f\n", i, float(expect[i]),
                   float(output[i]));
            return false;
        }
    }
    return true;
}

void PrintTensor(const float *buffer, size_t length)
{
    std::stringstream ss;
    size_t printNum = std::min(length, PRINT_NUM);
    for (size_t i = 0; i < printNum; i++) {
        ss << std::to_string(buffer[i]) << " ";
    }
    printf("[NNRtTest] [data] %s\n", ss.str().c_str());
}

} // namespace OHOS::NeuralNetworkRuntime::Test