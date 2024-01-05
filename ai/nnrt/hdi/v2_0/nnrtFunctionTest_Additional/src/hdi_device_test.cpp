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

#include <v2_0/innrt_device.h>
#include <v2_0/iprepared_model.h>
#include <v2_0/nnrt_types.h>
#include <vector>

#include "mindir.h"
#include "mindir_lite_graph.h"
#include "gtest/gtest.h"

#include "common/hdi_nnrt_test.h"
#include "common/hdi_nnrt_test_utils.h"
#include "interfaces/kits/c/neural_network_runtime/neural_network_runtime.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::NeuralNetworkRuntime;
using namespace OHOS::NeuralNetworkRuntime::Test;

namespace {

class DeviceTestAdditional : public HDINNRtTest {};

}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_GetDeviceNameV2_0200
 * @tc.name: testNnrtGetDeviceNameV2_001
 * @tc.desc: Call function V2 GetDeviceName, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetDeviceNameV2_001, Function | MediumTest | Level1)
{
    std::string deviceName = "abc";
    auto hdiRet = 0;

    for (int i = 0; i < 100; i++) {
        hdiRet = device_->GetDeviceName(deviceName);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
        std::cout << "deviceName:" << deviceName << std::endl;
        ASSERT_TRUE(!deviceName.empty());
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetDeviceNameV2_0300
 * @tc.name: testNnrtGetDeviceNameV2_002
 * @tc.desc: Call function V2 GetDeviceName, deviceName is nullptr
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetDeviceNameV2_002, Function | MediumTest | Level2)
{
    std::string deviceName = nullptr;
    auto hdiRet = device_->GetDeviceName(deviceName);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_NULL_PTR, hdiRet) << hdiRet;
    std::cout << "deviceName:" << deviceName << std::endl;
    ASSERT_TRUE(!deviceName.empty());
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_GetVendorNameV2_0200
 * @tc.name: testNnrtGetVendorNameV2_001
 * @tc.desc: Call function V2 GetVendorName, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetVendorNameV2_001, Function | MediumTest | Level1)
{
    std::string vendorName = "abc";
    auto hdiRet = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->GetVendorName(vendorName);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
        std::cout << "vendorName:" << vendorName << std::endl;
        ASSERT_TRUE(!vendorName.empty());
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetVendorNameV2_0300
 * @tc.name: testNnrtGetVendorNameV2_002
 * @tc.desc: Call function V2 GetVendorName, vendorName is nullptr
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetVendorNameV2_002, Function | MediumTest | Level2)
{
    std::string vendorName = nullptr;
    auto hdiRet = device_->GetVendorName(vendorName);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_NULL_PTR, hdiRet) << hdiRet;
    std::cout << "vendorName:" << vendorName << std::endl;
    ASSERT_TRUE(!vendorName.empty());
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_GetDeviceTypeV2_0200
 * @tc.name: testNnrtGetDeviceTypeV2_001
 * @tc.desc: Call function V2 GetDeviceType, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetDeviceTypeV2_001, Function | MediumTest | Level1)
{
    V2_0::DeviceType deviceType = V2_0::DeviceType::CPU;
    auto hdiRet = 0;

    for (int i = 0; i < 100; i++) {
        hdiRet = device_->GetDeviceType(deviceType);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet);

        ASSERT_TRUE(deviceType == V2_0::DeviceType::OTHER || deviceType == V2_0::DeviceType::GPU ||
                    deviceType == V2_0::DeviceType::CPU || deviceType == V2_0::DeviceType::ACCELERATOR)
            << deviceType;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_GetDeviceStatusV2_0200
 * @tc.name: testNnrtGetDeviceStatusV2_001
 * @tc.desc: Call function V2 GetDeviceStatus, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetDeviceStatusV2_001, Function | MediumTest | Level1)
{
    V2_0::DeviceStatus deviceStatus = V2_0::DeviceStatus::OFFLINE;
    auto hdiRet = 0;

    for (int i = 0; i < 100; i++) {
        hdiRet = device_->GetDeviceStatus(deviceStatus);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet);
        ASSERT_TRUE(deviceStatus == V2_0::DeviceStatus::AVAILABLE || deviceStatus == V2_0::DeviceStatus::BUSY ||
                    deviceStatus == V2_0::DeviceStatus::OFFLINE || deviceStatus == V2_0::DeviceStatus::UNKNOWN)
            << deviceStatus;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_IsFloat16PrecisionSupportedV2_0200
 * @tc.name: testNnrtIsFloat16PrecisionSupportedV2_001
 * @tc.desc: Call function V2 IsFloat16PrecisionSupported, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsFloat16PrecisionSupportedV2_001, Function | MediumTest | Level1)
{
    bool isSupportedFp16 = true;
    auto hdiRet = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->IsFloat16PrecisionSupported(isSupportedFp16);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_IsFloat16PrecisionSupportedV2_0300
 * @tc.name: testNnrtIsFloat16PrecisionSupportedV2_002
 * @tc.desc: Call function V2 IsFloat16PrecisionSupported, isSupportedFp16 is false
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsFloat16PrecisionSupportedV2_002, Function | MediumTest | Level1)
{
    bool isSupportedFp16 = false;
    auto hdiRet = device_->IsFloat16PrecisionSupported(isSupportedFp16);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_IsPerformanceModeSupportedV2_0200
 * @tc.name: testNnrtIsPerformanceModeSupportedV2_001
 * @tc.desc: Call function V2 IsPerformanceModeSupported, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsPerformanceModeSupportedV2_001, Function | MediumTest | Level1)
{
    bool isSupportedPerformance = true;
    auto hdiRet = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->IsPerformanceModeSupported(isSupportedPerformance);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_IsPerformanceModeSupportedV2_0300
 * @tc.name: testNnrtIsPerformanceModeSupportedV2_002
 * @tc.desc: Call function V2 IsPerformanceModeSupported, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsPerformanceModeSupportedV2_002, Function | MediumTest | Level1)
{
    bool isSupportedPerformance = false;
    auto hdiRet = device_->IsPerformanceModeSupported(isSupportedPerformance);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_IsPrioritySupportedV2_0200
 * @tc.name: testNnrtIsPrioritySupportedV2_001
 * @tc.desc: Call function V2 IsPrioritySupported, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsPrioritySupportedV2_001, Function | MediumTest | Level1)
{
    bool isSupportedPriority = true;
    auto hdiRet = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->IsPrioritySupported(isSupportedPriority);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_IsPrioritySupportedV2_0300
 * @tc.name: testNnrtIsPrioritySupportedV2_002
 * @tc.desc: Call function V2 IsPrioritySupported, isSupportedPriority is false
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsPrioritySupportedV2_002, Function | MediumTest | Level1)
{
    bool isSupportedPriority = false;
    auto hdiRet = device_->IsPrioritySupported(isSupportedPriority);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_IsDynamicInputSupportedV2_0200
 * @tc.name: testNnrtIsDynamicInputSupportedV2_001
 * @tc.desc: Call function V2 IsDynamicInputSupported, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsDynamicInputSupportedV2_001, Function | MediumTest | Level1)
{
    bool isSupportedDynamicInput = true;
    auto hdiRet = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->IsDynamicInputSupported(isSupportedDynamicInput);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_IsDynamicInputSupportedV2_0300
 * @tc.name: testNnrtIsDynamicInputSupportedV2_002
 * @tc.desc: Call function V2 IsDynamicInputSupported, isSupportedDynamicInput is false
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsDynamicInputSupportedV2_002, Function | MediumTest | Level1)
{
    bool isSupportedDynamicInput = false;
    auto hdiRet = device_->IsDynamicInputSupported(isSupportedDynamicInput);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_IsModelCacheSupportedV2_0200
 * @tc.name: testNnrtIsModelCacheSupportedV2_001
 * @tc.desc: Call function V2 IsModelCacheSupported, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsModelCacheSupportedV2_001, Function | MediumTest | Level1)
{
    bool isSupportedCache = true;
    auto hdiRet = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->IsModelCacheSupported(isSupportedCache);
        ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_IsModelCacheSupportedV2_0300
 * @tc.name: testNnrtIsModelCacheSupportedV2_002
 * @tc.desc: Call function V2 IsModelCacheSupported, isSupportedCache is false
 */
HWTEST_F(DeviceTestAdditional, testNnrtIsModelCacheSupportedV2_002, Function | MediumTest | Level1)
{
    bool isSupportedCache = false;
    auto hdiRet = device_->IsModelCacheSupported(isSupportedCache);
    ASSERT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_0400
 * @tc.name: testNnrtGetSupportedOperationV2_001
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_NONE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_001, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_NONE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_0500
 * @tc.name: testNnrtGetSupportedOperationV2_002
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_ACTIVATION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_002, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_ACTIVATION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_0600
 * @tc.name: testNnrtGetSupportedOperationV2_003
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_ADD_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_003, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_ADD_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_0700
 * @tc.name: testNnrtGetSupportedOperationV2_004
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_ARGMAX_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_004, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_ARGMAX_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_0800
 * @tc.name: testNnrtGetSupportedOperationV2_005
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_AVG_POOL_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_005, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_AVG_POOL_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_0900
 * @tc.name: testNnrtGetSupportedOperationV2_006
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_BATCH_TO_SPACE_ND
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_006, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_BATCH_TO_SPACE_ND);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1000
 * @tc.name: testNnrtGetSupportedOperationV2_007
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_BIAS_ADD
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_007, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_BIAS_ADD);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1100
 * @tc.name: testNnrtGetSupportedOperationV2_008
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_CAST
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_008, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_CAST);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1200
 * @tc.name: testNnrtGetSupportedOperationV2_009
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_CONCAT
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_009, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_CONCAT);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1300
 * @tc.name: testNnrtGetSupportedOperationV2_010
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_CONV2D_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_010, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_CONV2D_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1400
 * @tc.name: testNnrtGetSupportedOperationV2_011
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_CONV2D_TRANSPOSE_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_011, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_CONV2D_TRANSPOSE_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1500
 * @tc.name: testNnrtGetSupportedOperationV2_012
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_DIV_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_012, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_DIV_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1600
 * @tc.name: testNnrtGetSupportedOperationV2_013
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_ELTWISE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_013, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_ELTWISE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1700
 * @tc.name: testNnrtGetSupportedOperationV2_014
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_EXPAND_DIMS
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_014, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_EXPAND_DIMS);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1800
 * @tc.name: testNnrtGetSupportedOperationV2_015
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_FILL
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_015, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_FILL);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_1900
 * @tc.name: testNnrtGetSupportedOperationV2_016
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_FULL_CONNECTION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_016, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_FULL_CONNECTION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2000
 * @tc.name: testNnrtGetSupportedOperationV2_017
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_FUSED_BATCH_NORM
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_017, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_FUSED_BATCH_NORM);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2100
 * @tc.name: testNnrtGetSupportedOperationV2_018
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_GATHER
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_018, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_GATHER);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2200
 * @tc.name: testNnrtGetSupportedOperationV2_019
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_LAYER_NORM_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_019, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_LAYER_NORM_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2300
 * @tc.name: testNnrtGetSupportedOperationV2_020
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_LESS_EQUAL
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_020, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_LESS_EQUAL);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2400
 * @tc.name: testNnrtGetSupportedOperationV2_021
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_MATMUL_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_021, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_MATMUL_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2500
 * @tc.name: testNnrtGetSupportedOperationV2_022
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_MAXIMUM
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_022, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_MAXIMUM);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2600
 * @tc.name: testNnrtGetSupportedOperationV2_023
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_MAX_POOL_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_023, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_MAX_POOL_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2700
 * @tc.name: testNnrtGetSupportedOperationV2_024
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_MUL_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_024, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_MUL_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2800
 * @tc.name: testNnrtGetSupportedOperationV2_025
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_ONE_HOT
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_025, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_ONE_HOT);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_2900
 * @tc.name: testNnrtGetSupportedOperationV2_026
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_PAD_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_026, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_PAD_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3000
 * @tc.name: testNnrtGetSupportedOperationV2_027
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_POW_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_027, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_POW_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3100
 * @tc.name: testNnrtGetSupportedOperationV2_028
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_PRELU_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_028, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_PRELU_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3200
 * @tc.name: testNnrtGetSupportedOperationV2_029
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_QUANT_DTYPE_CAST
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_029, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_QUANT_DTYPE_CAST);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3300
 * @tc.name: testNnrtGetSupportedOperationV2_030
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_REDUCE_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_030, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_REDUCE_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3400
 * @tc.name: testNnrtGetSupportedOperationV2_031
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_RESHAPE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_031, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_RESHAPE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3500
 * @tc.name: testNnrtGetSupportedOperationV2_032
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_RESIZE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_032, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_RESIZE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3600
 * @tc.name: testNnrtGetSupportedOperationV2_033
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_RSQRT
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_033, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_RSQRT);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3700
 * @tc.name: testNnrtGetSupportedOperationV2_034
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SCALE_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_034, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SCALE_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3800
 * @tc.name: testNnrtGetSupportedOperationV2_035
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SHAPE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_035, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SHAPE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_3900
 * @tc.name: testNnrtGetSupportedOperationV2_036
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SLICE_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_036, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SLICE_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4000
 * @tc.name: testNnrtGetSupportedOperationV2_037
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SOFTMAX
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_037, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SOFTMAX);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4100
 * @tc.name: testNnrtGetSupportedOperationV2_038
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SPACE_TO_BATCH_ND
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_038, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SPACE_TO_BATCH_ND);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4200
 * @tc.name: testNnrtGetSupportedOperationV2_039
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SPLIT
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_039, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SPLIT);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4300
 * @tc.name: testNnrtGetSupportedOperationV2_040
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SQRT
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_040, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SQRT);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4400
 * @tc.name: testNnrtGetSupportedOperationV2_041
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SQUEEZE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_041, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SQUEEZE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4500
 * @tc.name: testNnrtGetSupportedOperationV2_042
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SQUARED_DIFFERENCE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_042, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SQUARED_DIFFERENCE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4600
 * @tc.name: testNnrtGetSupportedOperationV2_043
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_STACK
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_043, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_STACK);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4700
 * @tc.name: testNnrtGetSupportedOperationV2_044
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_STRIDED_SLICE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_044, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_STRIDED_SLICE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4800
 * @tc.name: testNnrtGetSupportedOperationV2_045
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_SUB_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_045, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_SUB_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_4900
 * @tc.name: testNnrtGetSupportedOperationV2_046
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_TILE_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_046, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_TILE_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5000
 * @tc.name: testNnrtGetSupportedOperationV2_047
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_TOPK_FUSION
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_047, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_TOPK_FUSION);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5100
 * @tc.name: testNnrtGetSupportedOperationV2_048
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_TRANSPOSE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_048, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_TRANSPOSE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5200
 * @tc.name: testNnrtGetSupportedOperationV2_049
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeType is NODE_TYPE_UNSQUEEZE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_049, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V2_0::NodeType>(mindspore::lite::NODE_TYPE_UNSQUEEZE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5300
 * @tc.name: testNnrtGetSupportedOperationV2_050
 * @tc.desc: Call function V2 GetSupportedOperation, node.quantType is QUANT_TYPE_NONE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_050, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.quantType = static_cast<V2_0::QuantType>(mindspore::lite::QUANT_TYPE_NONE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5400
 * @tc.name: testNnrtGetSupportedOperationV2_051
 * @tc.desc: Call function V2 GetSupportedOperation, node.quantType is QUANT_TYPE_ALL
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_051, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.quantType = static_cast<V2_0::QuantType>(mindspore::lite::QUANT_TYPE_ALL);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5500
 * @tc.name: testNnrtGetSupportedOperationV2_052
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_UNKNOWN
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_052, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_UNKNOWN);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5600
 * @tc.name: testNnrtGetSupportedOperationV2_053
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_BOOL
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_053, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_BOOL);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5700
 * @tc.name: testNnrtGetSupportedOperationV2_054
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_INT8
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_054, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_INT8);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5800
 * @tc.name: testNnrtGetSupportedOperationV2_055
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_INT16
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_055, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_INT16);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_5900
 * @tc.name: testNnrtGetSupportedOperationV2_056
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_INT32
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_056, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_INT32);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6000
 * @tc.name: testNnrtGetSupportedOperationV2_057
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_INT64
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_057, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_INT64);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6100
 * @tc.name: testNnrtGetSupportedOperationV2_058
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_UINT8
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_058, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_UINT8);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6200
 * @tc.name: testNnrtGetSupportedOperationV2_059
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_UINT16
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_059, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_UINT16);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6300
 * @tc.name: testNnrtGetSupportedOperationV2_060
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_UINT32
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_060, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_UINT32);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6400
 * @tc.name: testNnrtGetSupportedOperationV2_061
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_UINT64
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_061, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_UINT64);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6500
 * @tc.name: testNnrtGetSupportedOperationV2_062
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_FLOAT16
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_062, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_FLOAT16);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6600
 * @tc.name: testNnrtGetSupportedOperationV2_063
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_FLOAT32
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_063, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_FLOAT32);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6700
 * @tc.name: testNnrtGetSupportedOperationV2_064
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is DATA_TYPE_FLOAT64
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_064, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(mindspore::lite::DATA_TYPE_FLOAT64);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6800
 * @tc.name: testNnrtGetSupportedOperationV2_065
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.format is FORMAT_NONE
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_065, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.format = static_cast<V2_0::Format>(OHOS::HDI::Nnrt::V2_0::FORMAT_NONE);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_6900
 * @tc.name: testNnrtGetSupportedOperationV2_066
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.format is FORMAT_NCHW
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_066, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.format = static_cast<V2_0::Format>(mindspore::lite::FORMAT_NCHW);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7000
 * @tc.name: testNnrtGetSupportedOperationV2_067
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.format is FORMAT_NHWC
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_067, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.format = static_cast<V2_0::Format>(mindspore::lite::FORMAT_NHWC);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7100
 * @tc.name: testNnrtGetSupportedOperationV2_068
 * @tc.desc: Call function V2 GetSupportedOperation, allTensors is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_068, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    iModel->allTensors = {};

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7200
 * @tc.name: testNnrtGetSupportedOperationV2_069
 * @tc.desc: Call function V2 GetSupportedOperation, subGraph is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_069, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    iModel->subGraph = {};

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7300
 * @tc.name: testNnrtGetSupportedOperationV2_070
 * @tc.desc: Call function V2 GetSupportedOperation, inputIndex is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_070, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    iModel->inputIndex = {};

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7400
 * @tc.name: testNnrtGetSupportedOperationV2_071
 * @tc.desc: Call function V2 GetSupportedOperation, outputIndex is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_071, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    iModel->outputIndex = {};
    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7500
 * @tc.name: testNnrtGetSupportedOperationV2_072
 * @tc.desc: Call function V2 GetSupportedOperation, node.quantType is 10000
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_072, Function | MediumTest | Level1)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.quantType = static_cast<V2_0::QuantType>(100000);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7600
 * @tc.name: testNnrtGetSupportedOperationV2_073
 * @tc.desc: Call function V2 GetSupportedOperation, node.quantType is -1
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_073, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.quantType = static_cast<V2_0::QuantType>(-1);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7700
 * @tc.name: testNnrtGetSupportedOperationV2_074
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is -1
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_074, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(-1);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7800
 * @tc.name: testNnrtGetSupportedOperationV2_075
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dataType is 10000
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_075, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dataType = static_cast<V2_0::DataType>(10000);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_7900
 * @tc.name: testNnrtGetSupportedOperationV2_076
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.format is -1
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_076, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.format = static_cast<V2_0::Format>(-1);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8000
 * @tc.name: testNnrtGetSupportedOperationV2_077
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.format is 10000
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_077, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.format = static_cast<V2_0::Format>(10000);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(true, supportedOperations[i]);
    }

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8100
 * @tc.name: testNnrtGetSupportedOperationV2_078
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.dims is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_078, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.dims = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8200
 * @tc.name: testNnrtGetSupportedOperationV2_079
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.data is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_079, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.data = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8300
 * @tc.name: testNnrtGetSupportedOperationV2_080
 * @tc.desc: Call function V2 GetSupportedOperation, tensor.quantParams is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_080, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &tensor : iModel->allTensors) {
        tensor.quantParams = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8400
 * @tc.name: testNnrtGetSupportedOperationV2_081
 * @tc.desc: Call function V2 GetSupportedOperation, node.nodeAttr is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_081, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeAttr = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8500
 * @tc.name: testNnrtGetSupportedOperationV2_082
 * @tc.desc: Call function V2 GetSupportedOperation, node.inputIndex is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_082, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.inputIndex = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8600
 * @tc.name: testNnrtGetSupportedOperationV2_083
 * @tc.desc: Call function V2 GetSupportedOperation, node.outputIndex is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_083, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.outputIndex = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8700
 * @tc.name: testNnrtGetSupportedOperationV2_084
 * @tc.desc: Call function V2 GetSupportedOperation, subgraph.inputIndices is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_084, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &subgraph : iModel->subGraph) {
        subgraph.inputIndices = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8800
 * @tc.name: testNnrtGetSupportedOperationV2_085
 * @tc.desc: Call function V2 GetSupportedOperation, subgraph.outputIndices is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_085, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &subgraph : iModel->subGraph) {
        subgraph.outputIndices = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_GetSupportedOperationV2_8900
 * @tc.name: testNnrtGetSupportedOperationV2_086
 * @tc.desc: Call function V2 GetSupportedOperation, subgraph.nodeIndices is empty
 */
HWTEST_F(DeviceTestAdditional, testNnrtGetSupportedOperationV2_086, Function | MediumTest | Level2)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V2_0::Model *iModel = nullptr;
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &subgraph : iModel->subGraph) {
        subgraph.nodeIndices = {};
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));

    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number: SUB_AI_Nnrt_Func_HDI_Device_AllocateBufferV2_0300
 * @tc.name: testNnrtAllocateBufferV2_001
 * @tc.desc: Call function V2 AllocateBuffer, tensorSize is -1
 */
HWTEST_F(DeviceTestAdditional, testNnrtAllocateBufferV2_001, Function | MediumTest | Level2)
{
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    size_t tensorSize = -1;
    auto hdiRet = device_->AllocateBuffer(tensorSize, tensorBuffer);
    EXPECT_EQ(V2_0::NNRT_ReturnCode::NNRT_OUT_OF_MEMORY, hdiRet);
    EXPECT_FALSE(tensorBuffer.fd != NNRT_INVALID_FD && tensorBuffer.bufferSize == tensorSize)
        << tensorBuffer.fd << tensorBuffer.bufferSize;

    auto hdiRet1 = device_->ReleaseBuffer(tensorBuffer);
    EXPECT_FALSE(hdiRet1 == V2_0::NNRT_ReturnCode::NNRT_SUCCESS);
}

/**
 * @tc.number: SUB_AI_Nnrt_Stability_HDI_Device_ReleaseBufferV2_0300
 * @tc.name: testNnrtReleaseBufferV2_001
 * @tc.desc: Call function V2 ReleaseBuffer, stability test
 */
HWTEST_F(DeviceTestAdditional, testNnrtReleaseBufferV2_001, Function | MediumTest | Level1)
{
    V2_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    size_t tensorSize = 1;
    auto hdiRet = 0;
    auto hdiRet1 = 0;
    for (int i = 0; i < 100; i++) {
        hdiRet = device_->AllocateBuffer(tensorSize, tensorBuffer);
        EXPECT_TRUE(hdiRet == V2_0::NNRT_ReturnCode::NNRT_SUCCESS) << hdiRet;
        EXPECT_TRUE(tensorBuffer.fd != NNRT_INVALID_FD && tensorBuffer.bufferSize == tensorSize)
            << tensorBuffer.fd << tensorBuffer.bufferSize;

        hdiRet1 = device_->ReleaseBuffer(tensorBuffer);
        EXPECT_TRUE(hdiRet1 == V2_0::NNRT_ReturnCode::NNRT_SUCCESS);
    }
}