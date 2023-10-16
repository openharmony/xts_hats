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

#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include <hdf_base.h>
#include <hdf_log.h>
#include "osal_time.h"
#include "v1_0/iinput_interfaces.h"
#include "input_type.h"
#include "input_callback_impl.h"

using namespace OHOS::HDI::Input::V1_0;
using namespace testing::ext;

namespace {
    sptr<IInputInterfaces>  g_inputInterfaces = nullptr;
    sptr<IInputCallback> g_callback = nullptr;
    sptr<IInputCallback> g_hotplugCb = nullptr;

    constexpr int32_t INIT_DEFAULT_VALUE = 255;
    constexpr int32_t KEEP_ALIVE_TIME_MS = 3000;
    constexpr int32_t TOUCH_INDEX = 1;
    constexpr int32_t INVALID_INDEX = 5;
    constexpr int32_t MAX_DEVICES = 32;
    constexpr int32_t TEST_RESULT_LEN = 32;
}

class HdfInputHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfInputHdiTest::SetUpTestCase()
{
    g_inputInterfaces = IInputInterfaces::Get(true);
    if (g_inputInterfaces != nullptr) {
        g_callback = new InputCallbackImpl(g_inputInterfaces, nullptr);
        g_hotplugCb = new InputCallbackImpl(g_inputInterfaces, g_callback);
    }
}

void HdfInputHdiTest::TearDownTestCase()
{
}

void HdfInputHdiTest::SetUp()
{
}

void HdfInputHdiTest::TearDown()
{
}

static void OpenOnlineDev(std::vector<DevDesc> sta)
{
    int32_t ret = g_inputInterfaces->ScanInputDevice(sta);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: scan device failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    for (int32_t i = 0; i < MAX_DEVICES; i++) {
        if (sta[i].devIndex == 0) {
            break;
        }
        ret = g_inputInterfaces->OpenInputDevice(sta[i].devIndex);
        if (ret != INPUT_SUCCESS) {
            HDF_LOGE("%s: open device[%d] failed, ret %d", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);

        ret  = g_inputInterfaces->RegisterReportCallback(sta[i].devIndex, g_callback);
        if (ret != INPUT_SUCCESS) {
            HDF_LOGE("%s: register callback failed for device[%d], ret %d", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);
    }
}

static void CloseOnlineDev(std::vector<DevDesc> sta)
{
    int32_t ret = g_inputInterfaces->ScanInputDevice(sta);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: scan device failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    for (int32_t i = 0; i < MAX_DEVICES; i++) {
        if (sta[i].devIndex == 0) {
            break;
        }
        ret = g_inputInterfaces->UnregisterReportCallback(sta[i].devIndex);
        HDF_LOGE("%{public}s: index = %{public}d", __func__, i);
        if (ret != INPUT_SUCCESS) {
            HDF_LOGE("%s: register callback failed for device[%d], ret %d", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);

        ret = g_inputInterfaces->CloseInputDevice(sta[i].devIndex);
        if (ret != INPUT_SUCCESS) {
            HDF_LOGE("%s: close device[%d] failed, ret %d", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);
    }
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0900
  * @tc.name: GetInputClient001
  * @tc.desc: Get a client and check whether the client is empty.
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0900, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_inputInterfaces);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3100
  * @tc.name: ScanInputDevice001
  * @tc.desc: scan input device test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3100, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    std::vector<DevDesc> sta;

    HDF_LOGI("%s: [Hdi-Input] ScanInputDevice001 enter", __func__);
    int32_t ret;

    ret  = g_inputInterfaces->ScanInputDevice(sta);
    if (ret == INPUT_SUCCESS) {
        HDF_LOGE("%s: %d, %d, %d, %d", __func__, sta[0].devType, sta[0].devIndex, sta[1].devType, sta[1].devIndex);
    }

    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2000
  * @tc.name: OpenInputDev001
  * @tc.desc: open input device test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2000, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [Hdi-Input] OpenInputDevice001 enter", __func__);

    int32_t ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: open device failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2100
  * @tc.name: OpenInputDev002
  * @tc.desc: open input device test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2100, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [Hdi-Input] OpenInputDevice002 enter", __func__);

    /* Device "5" is used for testing nonexistent device node */
    int32_t ret = g_inputInterfaces->OpenInputDevice(INVALID_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: device %d does not exist, can't open it, ret %d", __func__, INVALID_INDEX, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0100
  * @tc.name: CloseInputDevice001
  * @tc.desc: close input device test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0100, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] CloseInputDevice001 enter", __func__);

    int32_t ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: close device failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0200
  * @tc.name: CloseInputDevice002
  * @tc.desc: close input device test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0200, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] CloseInputDevice002 enter", __func__);

    /* Device "5" is used for testing nonexistent device node */
    int32_t ret = g_inputInterfaces->CloseInputDevice(INVALID_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: device %d doesn't exist, can't close it, ret %d", __func__, INVALID_INDEX, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1000
  * @tc.name: GetInputDevice001
  * @tc.desc: get input device info test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1000, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetInputDevice001 enter", __func__);
    struct DeviceInfo dev;

    int32_t ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: open device failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    ret = g_inputInterfaces->GetInputDevice(TOUCH_INDEX, dev);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: devindex = %u, devType = %u", __func__, dev.devIndex, dev.devType);
    HDF_LOGI("%s: chipInfo = %s, vendorName = %s, chipName = %s",
        __func__, dev.chipInfo.c_str(), dev.vendorName.c_str(), dev.chipName.c_str());
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1100
  * @tc.name: GetInputDevice002
  * @tc.desc: get input device info test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1100, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetInputDevice002 enter", __func__);
    struct DeviceInfo dev;

    int32_t ret = g_inputInterfaces->GetInputDevice(INVALID_INDEX, dev);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device failed, ret %d", __func__, ret);
    }
    ASSERT_NE(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: devindex = %u, devType = %u", __func__, dev.devIndex, dev.devType);
    HDF_LOGI("%s: chipInfo = %s, vendorName = %s, chipName = %s",
        __func__, dev.chipInfo.c_str(), dev.vendorName.c_str(), dev.chipName.c_str());
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1400
  * @tc.name: GetInputDeviceList001
  * @tc.desc: get input device list info test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1400, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetInputDeviceList001 enter", __func__);
    int32_t ret;
    uint32_t num = 0;
    std::vector<DeviceInfo> dev;

    ret = g_inputInterfaces->GetInputDeviceList(num, dev, MAX_INPUT_DEV_NUM);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device list failed, ret %d", __func__, ret);
    }
    ret = num <= MAX_INPUT_DEV_NUM ? HDF_SUCCESS : HDF_FAILURE;  /* num <= MAX_INPUT_DEV_NUM return true */
    ASSERT_EQ(ret, HDF_SUCCESS);


    for (uint32_t i = 0; i < num; i++) {
        HDF_LOGI("%s: num = %u, device[%u]'s info is:", __func__, num, i);
        HDF_LOGI("%s: index = %u, devType = %u", __func__, dev[i].devIndex, dev[i].devType);
        HDF_LOGI("%s: chipInfo = %s, vendorName = %s, chipName = %s",
            __func__, dev[i].chipInfo.c_str(), dev[i].vendorName.c_str(), dev[i].chipName.c_str());
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0700
  * @tc.name: GetDeviceType001
  * @tc.desc: get input device type test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0700, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetDeviceType001 enter", __func__);
    int32_t ret;
    uint32_t devType = INIT_DEFAULT_VALUE;

    ret = g_inputInterfaces->GetDeviceType(TOUCH_INDEX, devType);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's type failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's type is %u", __func__, devType);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0800
  * @tc.name: GetDeviceType002
  * @tc.desc: get input device type test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0800, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetDeviceType002 enter", __func__);
    int32_t ret;
    uint32_t devType = INIT_DEFAULT_VALUE;

    ret = g_inputInterfaces->GetDeviceType(INVALID_INDEX, devType);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's type failed, ret %d", __func__, ret);
    }
    ASSERT_NE(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's type is %u", __func__, devType);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0300
  * @tc.name: GetChipInfo001
  * @tc.desc: get input device chip info test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0300, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetChipInfo001 enter", __func__);
    int32_t ret;
    std::string chipInfo;

    ret = g_inputInterfaces->GetChipInfo(TOUCH_INDEX, chipInfo);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's chip info failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's chip info is %s", __func__, chipInfo.c_str());
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0400
  * @tc.name: GetChipInfo002
  * @tc.desc: get input device chip info test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0400, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetChipInfo002 enter", __func__);
    int32_t ret;
    std::string chipInfo;

    ret = g_inputInterfaces->GetChipInfo(INVALID_INDEX, chipInfo);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's chip info failed, ret %d", __func__, ret);
    }
    ASSERT_NE(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's chip info is %s", __func__, chipInfo.c_str());
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1200
  * @tc.name: GetInputDevice003
  * @tc.desc: get input device chip info test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1200, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }

    HDF_LOGI("%s: [hdi-input] GetInputDevice003 enter", __func__);
    struct DeviceInfo dev;

    int32_t ret = g_inputInterfaces->GetInputDevice(TOUCH_INDEX, dev);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: After fill the info, new device's info is:", __func__);
    HDF_LOGI("%s: new devIndex = %u, devType = %u", __func__, dev.devIndex, dev.devType);
    HDF_LOGI("%s: new chipInfo = %s, vendorName = %s, chipName = %s",
        __func__, dev.chipInfo.c_str(), dev.vendorName.c_str(), dev.chipName.c_str());
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1300
  * @tc.name: GetInputDevice004
  * @tc.desc: get input device chip info test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1300, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }

    HDF_LOGI("%s: [hdi-input] GetInputDevice004 enter", __func__);
    struct DeviceInfo dev;

    int32_t ret = g_inputInterfaces->GetInputDevice(INVALID_INDEX, dev);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device failed, ret %d", __func__, ret);
    }

    HDF_LOGI("%s: After fill the info, new device's info is:", __func__);
    HDF_LOGI("%s: new devIndex = %u, devType = %u", __func__, dev.devIndex, dev.devType);
    HDF_LOGI("%s: new chipInfo = %s, vendorName = %s, chipName = %s",
        __func__, dev.chipInfo.c_str(), dev.vendorName.c_str(), dev.chipName.c_str());
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2200
  * @tc.name: RegisterCallback001
  * @tc.desc: register input device report test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2200, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RegisterCallback001 enter", __func__);

    /* Device "5" is used for testing nonexistent device node */
    int32_t ret  = g_inputInterfaces->RegisterReportCallback(INVALID_INDEX, g_callback);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: device %d dose not exist, can't register callback to it, ret %d", __func__, INVALID_INDEX, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3400
  * @tc.name: SetPowerStatus001
  * @tc.desc: set device power status test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3400, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetPowerStatus001 enter", __func__);
    int32_t ret;
    uint32_t setStatus = INPUT_LOW_POWER;

    ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device's power status failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3500
  * @tc.name: SetPowerStatus002
  * @tc.desc: set device power status test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3500, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetPowerStatus002 enter", __func__);
    int32_t ret;
    uint32_t setStatus = INPUT_SUSPEND;

    ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device's power status failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3600
  * @tc.name: SetPowerStatus003
  * @tc.desc: set device power status test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3600, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetPowerStatus003 enter", __func__);
    int32_t ret;
    uint32_t setStatus = INPUT_RESUME;

    ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device's power status failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3700
  * @tc.name: SetPowerStatus004
  * @tc.desc: set device power status test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3700, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetPowerStatus004 enter", __func__);
    int32_t ret;
    uint32_t setStatus = INPUT_LOW_POWER;
    /* Device "5" is used for testing nonexistent device node */
    ret = g_inputInterfaces->SetPowerStatus(INVALID_INDEX, setStatus);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device %d's power status failed, ret %d", __func__, INVALID_INDEX, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1500
  * @tc.name: GetPowerStatus001
  * @tc.desc: get device power status test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1500, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetPowerStatus001 enter", __func__);
    int32_t ret;
    uint32_t getStatus = 0;

    ret = g_inputInterfaces->GetPowerStatus(TOUCH_INDEX, getStatus);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's power status failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's power status is %u:", __func__, getStatus);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1600
  * @tc.name: GetPowerStatus002
  * @tc.desc: get device power status test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1600, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetPowerStatus002 enter", __func__);
    int32_t ret;
    uint32_t getStatus = 0;
    /* Device "5" is used for testing nonexistent device node */
    ret = g_inputInterfaces->GetPowerStatus(INVALID_INDEX, getStatus);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device %d's power status failed, ret %d", __func__, INVALID_INDEX, ret);
    }

    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1700
  * @tc.name: GetVendorName001
  * @tc.desc: get device vendor name test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1700, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetVendorName001 enter", __func__);
    int32_t ret;
    std::string vendorName;

    ret = g_inputInterfaces->GetVendorName(TOUCH_INDEX, vendorName);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's vendor name failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's vendor name is %s:", __func__, vendorName.c_str());
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1800
  * @tc.name: GetVendorName002
  * @tc.desc: get device vendor name test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1800, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetVendorName002 enter", __func__);
    int32_t ret;
    std::string vendorName;
    /* Device "5" is used for testing nonexistent device node */
    ret = g_inputInterfaces->GetVendorName(INVALID_INDEX, vendorName);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device %d's vendor name failed, ret %d", __func__, INVALID_INDEX, ret);
    }

    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0500
  * @tc.name: GetChipName001
  * @tc.desc: get device chip name test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0500, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetChipName001 enter", __func__);
    int32_t ret;
    std::string chipName;

    ret = g_inputInterfaces->GetChipName(TOUCH_INDEX, chipName);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device's chip name failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    HDF_LOGI("%s: device's chip name is %s", __func__, chipName.c_str());
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_0600
  * @tc.name: GetChipName002
  * @tc.desc: get device chip name test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_0600, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] GetChipName002 enter", __func__);
    int32_t ret;
    std::string chipName;
    /* Device "5" is used for testing nonexistent device node */
    ret = g_inputInterfaces->GetChipName(INVALID_INDEX, chipName);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get device %d's chip name failed, ret %d", __func__, INVALID_INDEX, ret);
    }

    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3200
  * @tc.name: SetGestureMode001
  * @tc.desc: set device gesture mode test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3200, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetGestureMode001 enter", __func__);
    int32_t ret;
    uint32_t gestureMode = 1;

    ret = g_inputInterfaces->SetGestureMode(TOUCH_INDEX, gestureMode);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device's gestureMode failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3300
  * @tc.name: SetGestureMode002
  * @tc.desc: set device gesture mode test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3300, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] SetGestureMode002 enter", __func__);
    int32_t ret;
    uint32_t gestureMode = 1;
    /* Device "5" is used for testing nonexistent device node */
    ret = g_inputInterfaces->SetGestureMode(INVALID_INDEX, gestureMode);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: set device %d's gestureMode failed, ret %d", __func__, INVALID_INDEX, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2400
  * @tc.name: RunCapacitanceTest001
  * @tc.desc: run capacitanceTest test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2400, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunCapacitanceTest001 enter", __func__);
    int32_t ret;
    std::string result;
    uint32_t testType = MMI_TEST;

    ret = g_inputInterfaces->RunCapacitanceTest(TOUCH_INDEX, testType, result, TEST_RESULT_LEN);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run capacitanceTest failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2500
  * @tc.name: RunCapacitanceTest002
  * @tc.desc: run capacitanceTest test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2500, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunCapacitanceTest002 enter", __func__);
    int32_t ret;
    std::string result;
    uint32_t testType = BASE_TEST;

    ret = g_inputInterfaces->RunCapacitanceTest(TOUCH_INDEX, testType, result, TEST_RESULT_LEN);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run capacitanceTest failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2600
  * @tc.name: RunCapacitanceTest003
  * @tc.desc: run capacitanceTest test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2600, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunCapacitanceTest003 enter", __func__);
    int32_t ret;
    std::string result;
    uint32_t testType = FULL_TEST;

    ret = g_inputInterfaces->RunCapacitanceTest(TOUCH_INDEX, testType, result, TEST_RESULT_LEN);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run capacitanceTest failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2700
  * @tc.name: RunCapacitanceTest004
  * @tc.desc: run capacitanceTest test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2700, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunCapacitanceTest004 enter", __func__);
    int32_t ret;
    std::string result;
    uint32_t testType = RUNNING_TEST;

    ret = g_inputInterfaces->RunCapacitanceTest(TOUCH_INDEX, testType, result, TEST_RESULT_LEN);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run capacitanceTest failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2800
  * @tc.name: RunCapacitanceTest005
  * @tc.desc: run capacitanceTest test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2800, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunCapacitanceTest005 enter", __func__);
    int32_t ret;
    std::string result;
    uint32_t testType = MMI_TEST;

    ret = g_inputInterfaces->RunCapacitanceTest(INVALID_INDEX, testType, result, TEST_RESULT_LEN);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run capacitanceTest failed, ret %d", __func__, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2900
  * @tc.name: RunExtraCommand001
  * @tc.desc: run extra command test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2900, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunExtraCommand001 enter", __func__);
    int32_t ret;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";

    ret = g_inputInterfaces->RunExtraCommand(TOUCH_INDEX, extraCmd);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run extraCommand failed, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3000
  * @tc.name: RunExtraCommand002
  * @tc.desc: run extra command test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3000, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RunExtraCommand002 enter", __func__);
    int32_t ret;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";

    ret = g_inputInterfaces->RunExtraCommand(INVALID_INDEX, extraCmd);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: run extraCommand failed, ret %d", __func__, ret);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_2300
  * @tc.name: RegisterCallbackAndReportData001
  * @tc.desc: register callback and report data test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_2300, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] RegisterCallbackAndReportData001 enter", __func__);
    int32_t ret;

    ret = g_inputInterfaces->RegisterReportCallback(TOUCH_INDEX, g_callback);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: register callback failed for device 1, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);
    printf("%s: wait 15s for testing, pls touch the panel now\n", __func__);
    printf("%s: The event data is as following:\n", __func__);
    OsalMSleep(KEEP_ALIVE_TIME_MS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_3800
  * @tc.name: UnregisterReportCallback001
  * @tc.desc: unregister reportCallback test
  * @tc.type: func
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_3800, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }
    HDF_LOGI("%s: [hdi-input] UnregisterReportCallback001 enter", __func__);
    int32_t ret;

    ret  = g_inputInterfaces->UnregisterReportCallback(TOUCH_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: unregister callback failed for device, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);

    ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: close device failed, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);
}

/**
  * @tc.number: SUB_Driver_Input_Passthrough_1900
  * @tc.name: HotPlugCallback001
  * @tc.desc: input device hot plug test
  * @tc.type: FUNC
  * @tc.require:
  */
HWTEST_F(HdfInputHdiTest, SUB_Driver_Input_Passthrough_1900, Function | MediumTest | Level1)
{
    if (g_inputInterfaces == nullptr) {
        ASSERT_NE(nullptr, g_inputInterfaces);
        return;
    }

    HDF_LOGI("%s: [hdi-input] HotPlugCallback001 enter", __func__);
    int32_t ret = INPUT_SUCCESS;
    std::vector<DevDesc> sta;

    ret = g_inputInterfaces->RegisterHotPlugCallback(g_hotplugCb);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: register hotplug callback failed for device manager, ret %d", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);
    OpenOnlineDev(sta);

    printf("%s: wait 15s for testing, pls hotplug now\n", __func__);
    printf("%s: The event data is as following:\n", __func__);
    OsalMSleep(KEEP_ALIVE_TIME_MS);

    CloseOnlineDev(sta);

    ret = g_inputInterfaces->UnregisterHotPlugCallback();
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: unregister hotplug callback failed for device manager, ret %d", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}
