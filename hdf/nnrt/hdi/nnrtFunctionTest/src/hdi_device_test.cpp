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

class DeviceTest : public HDINNRtTest {};

} // namespace

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceInfo_0100
 * @tc.desc: 获取设备名称
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceInfo_0100, Function | MediumTest | Level2)
{
    std::string deviceName;
    auto hdiRet = device_->GetDeviceName(deviceName);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
    std::cout << "deviceName:" << deviceName << std::endl;
    ASSERT_TRUE(!deviceName.empty());
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceInfo_0200
 * @tc.desc: 获取芯片供应商名称
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceInfo_0200, Function | MediumTest | Level2)
{
    std::string vendorName;
    auto hdiRet = device_->GetVendorName(vendorName);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
    std::cout << "vendorName:" << vendorName << std::endl;
    ASSERT_TRUE(!vendorName.empty());
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceInfo_0300
 * @tc.desc: 获取设备类型
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceInfo_0300, Function | MediumTest | Level1)
{
    V1_0::DeviceType deviceType;
    auto hdiRet = device_->GetDeviceType(deviceType);
    ASSERT_EQ(HDF_SUCCESS, hdiRet);

    ASSERT_TRUE(deviceType == V1_0::DeviceType::OTHER || deviceType == V1_0::DeviceType::GPU ||
                deviceType == V1_0::DeviceType::CPU || deviceType == V1_0::DeviceType::ACCELERATOR)
        << deviceType;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceInfo_0400
 * @tc.desc: 获取设备状态
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceInfo_0400, Function | MediumTest | Level1)
{
    V1_0::DeviceStatus deviceStatus;
    auto hdiRet = device_->GetDeviceStatus(deviceStatus);
    ASSERT_EQ(HDF_SUCCESS, hdiRet);
    ASSERT_TRUE(deviceStatus == V1_0::DeviceStatus::AVAILABLE || deviceStatus == V1_0::DeviceStatus::BUSY ||
                deviceStatus == V1_0::DeviceStatus::OFFLINE || deviceStatus == V1_0::DeviceStatus::UNKNOWN)
        << deviceStatus;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceConfig_0100
 * @tc.desc: 查询设备是否支持以Float16精度运算Float32的模型
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceConfig_0100, Function | MediumTest | Level2)
{
    bool isSupportedFp16;
    auto hdiRet = device_->IsFloat16PrecisionSupported(isSupportedFp16);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceConfig_0200
 * @tc.desc: 查询设备是否支持性能偏好设置
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceConfig_0200, Function | MediumTest | Level2)
{
    bool isSupportedPerformance;
    auto hdiRet = device_->IsPerformanceModeSupported(isSupportedPerformance);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceConfig_0300
 * @tc.desc: 查询设备是否支持任务优先级设置
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceConfig_0300, Function | MediumTest | Level2)
{
    bool isSupportedPriority;
    auto hdiRet = device_->IsPrioritySupported(isSupportedPriority);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceConfig_0400
 * @tc.desc: 查询设备是否支持变尺寸输入
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceConfig_0400, Function | MediumTest | Level2)
{
    bool isSupportedDynamicInput;
    auto hdiRet = device_->IsDynamicInputSupported(isSupportedDynamicInput);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_DeviceConfig_0500
 * @tc.desc: 查询是否支持模型缓存功能
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_DeviceConfig_0500, Function | MediumTest | Level2)
{
    bool isSupportedCache;
    auto hdiRet = device_->IsModelCacheSupported(isSupportedCache);
    ASSERT_EQ(HDF_SUCCESS, hdiRet) << hdiRet;
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Device_ModelSupport_0100
 * @tc.name   : 模型算子支持查询，nodes为空
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_ModelSupport_0100, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set nodes to empty
    iModel->nodes = {};
    std::vector<bool> supportedOperations;
    EXPECT_EQ(HDF_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    EXPECT_TRUE(supportedOperations.empty());

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Device_ModelSupport_0200
 * @tc.name   : 模型算子支持查询，nodes中存在NodeType为-1
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_ModelSupport_0200, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));
    // set node[0] to
    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V1_0::NodeType>(-1);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(HDF_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.number : SUB_AI_NNRt_Func_South_Device_ModelSupport_0300
 * @tc.name   : 模型算子支持查询，nodes中存在NodeType为10000
 * @tc.desc   : [C- SOFTWARE -0200]
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_ModelSupport_0300, Function | MediumTest | Level3)
{
    OH_NNModel *model = nullptr;
    HDICommon::BuildAddGraph(&model);
    ASSERT_NE(model, nullptr);

    V1_0::Model *iModel = nullptr;
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    ASSERT_EQ(OH_NN_SUCCESS, HDICommon::ConvertModel(device_, model, tensorBuffer, &iModel));

    for (auto &node : iModel->nodes) {
        node.nodeType = static_cast<V1_0::NodeType>(100000);
    }

    std::vector<bool> supportedOperations;
    EXPECT_EQ(HDF_SUCCESS, device_->GetSupportedOperation(*iModel, supportedOperations));
    for (uint32_t i = 0; i < supportedOperations.size(); i++) {
        EXPECT_EQ(false, supportedOperations[i]);
    }

    // release
    mindspore::lite::MindIR_Model_Destroy(&iModel);
    if (tensorBuffer.fd != -1) {
        EXPECT_EQ(HDF_SUCCESS, device_->ReleaseBuffer(tensorBuffer));
    }
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_Memory_0100
 * @tc.desc: 创建共享内存，length为1
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_Memory_0100, Function | MediumTest | Level1)
{
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    size_t tensorSize = 1;
    auto hdiRet = device_->AllocateBuffer(tensorSize, tensorBuffer);
    EXPECT_TRUE(hdiRet == HDF_SUCCESS) << hdiRet;
    EXPECT_TRUE(tensorBuffer.fd != NNRT_INVALID_FD && tensorBuffer.bufferSize == tensorSize)
        << tensorBuffer.fd << tensorBuffer.bufferSize;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_Memory_0200
 * @tc.desc: 创建共享内存，length为0
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_Memory_0200, Function | MediumTest | Level3)
{
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    size_t tensorSize = 0;
    auto hdiRet = device_->AllocateBuffer(tensorSize, tensorBuffer);
    EXPECT_TRUE(hdiRet != HDF_SUCCESS) << hdiRet;
    EXPECT_TRUE(tensorBuffer.fd == NNRT_INVALID_FD) << tensorBuffer.fd;
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_Memory_0300
 * @tc.desc: 销毁共享内存，buffer为空
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_Memory_0300, Function | MediumTest | Level2)
{
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    auto hdiRet = device_->ReleaseBuffer(tensorBuffer);
    EXPECT_TRUE(hdiRet != HDF_SUCCESS);
}

/**
 * @tc.name: SUB_AI_NNRt_Func_South_Device_Memory_0400
 * @tc.desc: 销毁共享内存，buffer释放成功
 * @tc.type: FUNC
 */
HWTEST_F(DeviceTest, SUB_AI_NNRt_Func_South_Device_Memory_0400, Function | MediumTest | Level1)
{
    V1_0::SharedBuffer tensorBuffer{NNRT_INVALID_FD, 0, 0, 0};
    size_t tensorSize = 224;
    auto hdiRet = device_->AllocateBuffer(tensorSize, tensorBuffer);
    EXPECT_TRUE(hdiRet == HDF_SUCCESS && tensorBuffer.fd != NNRT_INVALID_FD && tensorBuffer.bufferSize == tensorSize)
        << tensorBuffer.fd << tensorBuffer.bufferSize;
    hdiRet = device_->ReleaseBuffer(tensorBuffer);
    EXPECT_TRUE(hdiRet == HDF_SUCCESS);
}