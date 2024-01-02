/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
sptr<IInputInterfaces> g_inputInterfaces = nullptr;
sptr<IInputCallback> g_callback = nullptr;
sptr<IInputCallback> g_hotplugCb = nullptr;
constexpr int32_t TOUCH_INDEX = 1;
constexpr int32_t MAX_DEVICES = 32;
constexpr int32_t INIT_DEFAULT_VALUE = 255;
constexpr int32_t TEST_RESULT_LEN = 32;
std::vector<DevDesc> g_sta;
} // namespace

class HdfInputHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfInputHdiTestAdditional::SetUpTestCase()
{
    g_inputInterfaces = IInputInterfaces::Get(true);
    if (g_inputInterfaces != nullptr) {
        g_callback = new InputCallbackImpl(g_inputInterfaces, nullptr);
        g_hotplugCb = new InputCallbackImpl(g_inputInterfaces, g_callback);
        g_inputInterfaces->ScanInputDevice(g_sta);
    }
}

void HdfInputHdiTestAdditional::TearDownTestCase() {}

void HdfInputHdiTestAdditional::SetUp() {}

void HdfInputHdiTestAdditional::TearDown() {}

static bool IsOnlineDev(uint32_t devIndex)
{
    bool ret = false;
    int32_t i = 0;
    for (i = 0; i < g_sta.size(); i++) {
        if (g_sta[i].devIndex == devIndex) {
            ret = true;
            break;
        }
    }
    return ret;
}

/**
 * @tc.number : SUB_Driver_Input_ScanInputDevice_0200
 * @tc.name   : testScanInputDevice001
 * @tc.desc   : Reliability of function(ScanInputDevice)
 */
HWTEST_F(HdfInputHdiTestAdditional, testScanInputDevice001, Function | MediumTest | Level1)
{
    std::vector<DevDesc> sta;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_inputInterfaces->ScanInputDevice(sta);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0300
 * @tc.name   : testOpenInputDevice001
 * @tc.desc   : Reliability of function(OpenInputDevice)
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret |= g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
        g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0400
 * @tc.name   : testOpenInputDevice002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 0;
    ret = g_inputInterfaces->OpenInputDevice(devIndex);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0500
 * @tc.name   : testOpenInputDevice003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    for(devIndex = TOUCH_INDEX; devIndex < MAX_DEVICES; devIndex++){
        if (IsOnlineDev(devIndex)) {
            ret = g_inputInterfaces->OpenInputDevice(devIndex);
            g_inputInterfaces->CloseInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
        } else {
            ret = g_inputInterfaces->OpenInputDevice(devIndex);
            EXPECT_NE(ret, INPUT_SUCCESS);
        }
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0600
 * @tc.name   : testOpenInputDevice004
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice004, Function | MediumTest | Level2)
{
    int32_t ret = g_inputInterfaces->OpenInputDevice(MAX_DEVICES);
    g_inputInterfaces->CloseInputDevice(MAX_DEVICES);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0300
 * @tc.name   : testCloseInputDevice001
 * @tc.desc   : Reliability of function(CloseInputDevice)
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
        ret |= g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0400
 * @tc.name   : testCloseInputDevice002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 0;
    ret = g_inputInterfaces->CloseInputDevice(devIndex);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0500
 * @tc.name   : testCloseInputDevice003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    for(devIndex = TOUCH_INDEX; devIndex < MAX_DEVICES; devIndex++){
        if (IsOnlineDev(devIndex)) {
            ret = g_inputInterfaces->OpenInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
            ret = g_inputInterfaces->CloseInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
        } else {
            ret = g_inputInterfaces->CloseInputDevice(devIndex);
            EXPECT_NE(ret, INPUT_SUCCESS);
        }
    }
}

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0600
 * @tc.name   : testCloseInputDevice004
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice004, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    ret = g_inputInterfaces->CloseInputDevice(MAX_DEVICES);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0300
 * @tc.name   : testGetInputDevice001
 * @tc.desc   : Reliability of function(GetInputDevice)
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice001, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
        ret |= g_inputInterfaces->GetInputDevice(TOUCH_INDEX, devInfo);
        g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0400
 * @tc.name   : testGetInputDevice002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice002, Function | MediumTest | Level2)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 0;
    ret = g_inputInterfaces->GetInputDevice(devIndex, devInfo);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0500
 * @tc.name   : testGetInputDevice003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice003, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    for(devIndex = TOUCH_INDEX; devIndex < MAX_DEVICES; devIndex++){
        if (IsOnlineDev(devIndex)) {
            if ((ret = g_inputInterfaces->OpenInputDevice(devIndex)) == 0) {
                ret = g_inputInterfaces->GetInputDevice(devIndex, devInfo);
                EXPECT_EQ(ret, INPUT_SUCCESS);
                g_inputInterfaces->CloseInputDevice(devIndex);
            } else {
                EXPECT_EQ(ret, INPUT_SUCCESS);
            }
        } else {
            ret = g_inputInterfaces->GetInputDevice(devIndex, devInfo);
            EXPECT_NE(ret, INPUT_SUCCESS);
        }
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0600
 * @tc.name   : testGetInputDevice004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice004, Function | MediumTest | Level2)
{
    struct DeviceInfo devInfo;
    int32_t ret = g_inputInterfaces->GetInputDevice(MAX_DEVICES, devInfo);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetInputDeviceList_0200
 * @tc.name   : testGetInputDeviceList001
 * @tc.desc   : Reliability of function(GetInputDeviceList)
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDeviceList001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int i = 0;
    uint32_t num = 0;
    std::vector<DeviceInfo> dev;
    g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    for (i = 0; i < 1000; i++) {
        g_inputInterfaces->GetInputDeviceList(num, dev, MAX_INPUT_DEV_NUM);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 *
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0500
 * @tc.name   : testSetPowerStatus001
 * @tc.desc   : Reliability of function(SetPowerStatus)
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int i = 0;
    uint32_t setStatus = INPUT_SUSPEND;
    g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    for (i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0600
 * @tc.name   : testSetPowerStatus002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 0;
    uint32_t setStatus = INPUT_SUSPEND;
    ret = g_inputInterfaces->SetPowerStatus(devIndex, setStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0700
 * @tc.name   : testSetPowerStatus003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    uint32_t setStatus = INPUT_SUSPEND;
    for(devIndex = TOUCH_INDEX; devIndex < MAX_DEVICES; devIndex++){
        if (IsOnlineDev(devIndex)) {
            ret = g_inputInterfaces->OpenInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
            ret = g_inputInterfaces->SetPowerStatus(devIndex, setStatus);
            EXPECT_EQ(ret, INPUT_SUCCESS);
            g_inputInterfaces->CloseInputDevice(devIndex);
        } else {
            ret = g_inputInterfaces->SetPowerStatus(devIndex, setStatus);
            EXPECT_NE(ret, INPUT_SUCCESS);
        }
    }
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0800
 * @tc.name   : testSetPowerStatus004
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus004, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = INPUT_SUSPEND;
    ret = g_inputInterfaces->SetPowerStatus(MAX_DEVICES, setStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0900
 * @tc.name   : testSetPowerStatus005
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus005, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = INPUT_POWER_STATUS_UNKNOWN;
    ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1000
 * @tc.name   : testSetPowerStatus006
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus006, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = INPUT_POWER_STATUS_UNKNOWN;
    ret = g_inputInterfaces->SetPowerStatus(MAX_DEVICES, setStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1100
 * @tc.name   : testSetPowerStatus007
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus007, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = 5;
    ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1200
 * @tc.name   : testSetPowerStatus008
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus008, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = 0x7fffffff;
    ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_0300
 * @tc.name   : testGetPowerStatus001
 * @tc.desc   : Reliability of function(GetPowerStatus)
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int i = 0;
    uint32_t getStatus = 0;
    g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    for (i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->GetPowerStatus(TOUCH_INDEX, getStatus);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_0400
 * @tc.name   : testGetPowerStatus002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 0;
    uint32_t getStatus = 0;
    ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_0600
 * @tc.name   : testGetPowerStatus004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_0700
 * @tc.name   : testGetPowerStatus005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_0800
 * @tc.name   : testGetPowerStatus006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_0900
 * @tc.name   : testGetPowerStatus007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1000
 * @tc.name   : testGetPowerStatus008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1100
 * @tc.name   : testGetPowerStatus009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1200
 * @tc.name   : testGetPowerStatus010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1300
 * @tc.name   : testGetPowerStatus011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1400
 * @tc.name   : testGetPowerStatus012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1500
 * @tc.name   : testGetPowerStatus013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1600
 * @tc.name   : testGetPowerStatus014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1700
 * @tc.name   : testGetPowerStatus015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1800
 * @tc.name   : testGetPowerStatus016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_1900
 * @tc.name   : testGetPowerStatus017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2000
 * @tc.name   : testGetPowerStatus018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2100
 * @tc.name   : testGetPowerStatus019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2200
 * @tc.name   : testGetPowerStatus020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2300
 * @tc.name   : testGetPowerStatus021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2400
 * @tc.name   : testGetPowerStatus022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2500
 * @tc.name   : testGetPowerStatus023
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2600
 * @tc.name   : testGetPowerStatus024
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2700
 * @tc.name   : testGetPowerStatus025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2800
 * @tc.name   : testGetPowerStatus026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_2900
 * @tc.name   : testGetPowerStatus027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_3000
 * @tc.name   : testGetPowerStatus028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_3100
 * @tc.name   : testGetPowerStatus029
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_3200
 * @tc.name   : testGetPowerStatus030
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_3300
 * @tc.name   : testGetPowerStatus031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    uint32_t getStatus = 0;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetPowerStatus(devIndex, getStatus);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetPowerStatus_3400
 * @tc.name   : testGetPowerStatus032
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetPowerStatus032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t getStatus = 0;
    ret = g_inputInterfaces->GetPowerStatus(MAX_DEVICES, getStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0300
 * @tc.name   : testGetDeviceType001
 * @tc.desc   : Reliability of function(GetDeviceType)
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int i = 0;
    uint32_t devType = INIT_DEFAULT_VALUE;
    g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    for (i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->GetDeviceType(TOUCH_INDEX, devType);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0400
 * @tc.name   : testGetDeviceType002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 0;
    uint32_t devType = INIT_DEFAULT_VALUE;
    ret = g_inputInterfaces->GetDeviceType(devIndex, devType);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0500
 * @tc.name   : testGetDeviceType003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    uint32_t devType = INIT_DEFAULT_VALUE;
    for(devIndex = TOUCH_INDEX; devIndex < MAX_DEVICES; devIndex++){
        if (IsOnlineDev(devIndex)) {
            ret = g_inputInterfaces->OpenInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
            ret = g_inputInterfaces->GetDeviceType(devIndex, devType);
            g_inputInterfaces->CloseInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
        } else {
            ret = g_inputInterfaces->GetDeviceType(devIndex, devType);
            EXPECT_NE(ret, INPUT_SUCCESS);
        }
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0600
 * @tc.name   : testGetDeviceType004
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType004, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devType = INIT_DEFAULT_VALUE;
    ret = g_inputInterfaces->GetDeviceType(MAX_DEVICES, devType);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0300
 * @tc.name   : testSetGestureMode001
 * @tc.desc   : Reliability of function(SetGestureMode)
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t gestureMode = 1;
    int i = 0;
    g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    for (i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->SetGestureMode(TOUCH_INDEX, gestureMode);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0400
 * @tc.name   : testSetGestureMode002
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 0;
    uint32_t gestureMode = 1;
    ret = g_inputInterfaces->SetGestureMode(devIndex, gestureMode);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0500
 * @tc.name   : testSetGestureMode003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    uint32_t gestureMode = 1;
    for(devIndex = TOUCH_INDEX; devIndex < MAX_DEVICES; devIndex++){
        if (IsOnlineDev(devIndex)) {
            ret = g_inputInterfaces->OpenInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
            ret = g_inputInterfaces->SetGestureMode(devIndex, gestureMode);
            g_inputInterfaces->CloseInputDevice(devIndex);
            EXPECT_EQ(ret, INPUT_SUCCESS);
        } else {
            ret = g_inputInterfaces->SetGestureMode(devIndex, gestureMode);
            EXPECT_NE(ret, INPUT_SUCCESS);
        }
    }
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0600
 * @tc.name   : testSetGestureMode004
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode004, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t gestureMode = 1;
    ret = g_inputInterfaces->SetGestureMode(MAX_DEVICES, gestureMode);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0700
 * @tc.name   : testSetGestureMode005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    uint32_t gestureMode = 0;
    g_inputInterfaces->OpenInputDevice(devIndex);
    ret = g_inputInterfaces->SetGestureMode(devIndex, gestureMode);
    g_inputInterfaces->CloseInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0800
 * @tc.name   : testSetGestureMode006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = TOUCH_INDEX;
    uint32_t gestureMode = 0x7fffffff;
    g_inputInterfaces->OpenInputDevice(devIndex);
    ret = g_inputInterfaces->SetGestureMode(devIndex, gestureMode);
    g_inputInterfaces->CloseInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_0300
 * @tc.name   : testGetChipInfo001
 * @tc.desc   : GetChipInfo, stability test
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo001, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);

    std::string chipInfo;

    for (int i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->GetChipInfo(TOUCH_INDEX, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    }

    ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_0500
 * @tc.name   : testGetChipInfo003
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 2
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_0600
 * @tc.name   : testGetChipInfo004
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 3
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_0700
 * @tc.name   : testGetChipInfo005
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 4
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_0800
 * @tc.name   : testGetChipInfo006
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 6
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_0900
 * @tc.name   : testGetChipInfo007
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 7
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1000
 * @tc.name   : testGetChipInfo008
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 8
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1100
 * @tc.name   : testGetChipInfo009
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 9
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1200
 * @tc.name   : testGetChipInfo010
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 10
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1300
 * @tc.name   : testGetChipInfo011
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 11
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1400
 * @tc.name   : testGetChipInfo012
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 12
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1500
 * @tc.name   : testGetChipInfo013
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 13
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1600
 * @tc.name   : testGetChipInfo014
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 14
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1700
 * @tc.name   : testGetChipInfo015
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 15
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1800
 * @tc.name   : testGetChipInfo016
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 16
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_1900
 * @tc.name   : testGetChipInfo017
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 17
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2000
 * @tc.name   : testGetChipInfo018
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 18
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2100
 * @tc.name   : testGetChipInfo019
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 19
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2200
 * @tc.name   : testGetChipInfo020
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 20
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2300
 * @tc.name   : testGetChipInfo021
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 21
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2400
 * @tc.name   : testGetChipInfo022
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 22
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2500
 * @tc.name   : testGetChipInfo023
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 23
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2600
 * @tc.name   : testGetChipInfo024
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 24
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2700
 * @tc.name   : testGetChipInfo025
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 25
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2800
 * @tc.name   : testGetChipInfo026
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 26
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_2900
 * @tc.name   : testGetChipInfo027
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 27
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_3000
 * @tc.name   : testGetChipInfo028
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 28
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_3100
 * @tc.name   : testGetChipInfo029
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 29
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_3200
 * @tc.name   : testGetChipInfo030
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 30
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_3300
 * @tc.name   : testGetChipInfo031
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 31
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    std::string chipInfo;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipInfo_3400
 * @tc.name   : testGetChipInfo032
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 32
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipInfo032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = MAX_DEVICES;
    std::string chipInfo;

    ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0300
 * @tc.name   : testGetVendorName001
 * @tc.desc   : GetVendorName, stability test
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName001, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);

    std::string vendorName;

    for (int i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->GetVendorName(TOUCH_INDEX, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    }

    ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0500
 * @tc.name   : testGetVendorName003
 * @tc.desc   : GetVendorName, Test input param, devIndex = 2
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0600
 * @tc.name   : testGetVendorName004
 * @tc.desc   : GetVendorName, Test input param, devIndex = 3
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0700
 * @tc.name   : testGetVendorName005
 * @tc.desc   : GetVendorName, Test input param, devIndex = 4
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0800
 * @tc.name   : testGetVendorName006
 * @tc.desc   : GetVendorName, Test input param, devIndex = 6
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0900
 * @tc.name   : testGetVendorName007
 * @tc.desc   : GetVendorName, Test input param, devIndex = 7
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1000
 * @tc.name   : testGetVendorName008
 * @tc.desc   : GetVendorName, Test input param, devIndex = 8
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1100
 * @tc.name   : testGetVendorName009
 * @tc.desc   : GetVendorName, Test input param, devIndex = 9
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1200
 * @tc.name   : testGetVendorName010
 * @tc.desc   : GetVendorName, Test input param, devIndex = 10
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1300
 * @tc.name   : testGetVendorName011
 * @tc.desc   : GetVendorName, Test input param, devIndex = 11
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1400
 * @tc.name   : testGetVendorName012
 * @tc.desc   : GetVendorName, Test input param, devIndex = 12
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1500
 * @tc.name   : testGetVendorName013
 * @tc.desc   : GetVendorName, Test input param, devIndex = 13
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1600
 * @tc.name   : testGetVendorName014
 * @tc.desc   : GetVendorName, Test input param, devIndex = 14
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1700
 * @tc.name   : testGetVendorName015
 * @tc.desc   : GetVendorName, Test input param, devIndex = 15
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1800
 * @tc.name   : testGetVendorName016
 * @tc.desc   : GetVendorName, Test input param, devIndex = 16
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_1900
 * @tc.name   : testGetVendorName017
 * @tc.desc   : GetVendorName, Test input param, devIndex = 17
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2000
 * @tc.name   : testGetVendorName018
 * @tc.desc   : GetVendorName, Test input param, devIndex = 18
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2100
 * @tc.name   : testGetVendorName019
 * @tc.desc   : GetVendorName, Test input param, devIndex = 19
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2200
 * @tc.name   : testGetVendorName020
 * @tc.desc   : GetVendorName, Test input param, devIndex = 20
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2300
 * @tc.name   : testGetVendorName021
 * @tc.desc   : GetVendorName, Test input param, devIndex = 21
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2400
 * @tc.name   : testGetVendorName022
 * @tc.desc   : GetVendorName, Test input param, devIndex = 22
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2500
 * @tc.name   : testGetVendorName023
 * @tc.desc   : GetVendorName, Test input param, devIndex = 23
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2600
 * @tc.name   : testGetVendorName024
 * @tc.desc   : GetVendorName, Test input param, devIndex = 24
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2700
 * @tc.name   : testGetVendorName025
 * @tc.desc   : GetVendorName, Test input param, devIndex = 25
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2800
 * @tc.name   : testGetVendorName026
 * @tc.desc   : GetVendorName, Test input param, devIndex = 26
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_2900
 * @tc.name   : testGetVendorName027
 * @tc.desc   : GetVendorName, Test input param, devIndex = 27
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_3000
 * @tc.name   : testGetVendorName028
 * @tc.desc   : GetVendorName, Test input param, devIndex = 28
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_3100
 * @tc.name   : testGetVendorName029
 * @tc.desc   : GetVendorName, Test input param, devIndex = 29
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_3200
 * @tc.name   : testGetVendorName030
 * @tc.desc   : GetVendorName, Test input param, devIndex = 30
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_3300
 * @tc.name   : testGetVendorName031
 * @tc.desc   : GetVendorName, Test input param, devIndex = 31
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    std::string vendorName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_3400
 * @tc.name   : testGetVendorName032
 * @tc.desc   : GetVendorName, Test input param, devIndex = 32
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetVendorName032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = MAX_DEVICES;
    std::string vendorName;

    ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0300
 * @tc.name   : testGetChipName001
 * @tc.desc   : GetChipName, stability test
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName001, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);

    std::string chipName;

    for (int i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->GetChipName(TOUCH_INDEX, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    }

    ret = g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0500
 * @tc.name   : testGetChipName003
 * @tc.desc   : GetChipName, Test input param, devIndex = 2
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0600
 * @tc.name   : testGetChipName004
 * @tc.desc   : GetChipName, Test input param, devIndex = 3
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0700
 * @tc.name   : testGetChipName005
 * @tc.desc   : GetChipName, Test input param, devIndex = 4
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0800
 * @tc.name   : testGetChipName006
 * @tc.desc   : GetChipName, Test input param, devIndex = 6
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0900
 * @tc.name   : testGetChipName007
 * @tc.desc   : GetChipName, Test input param, devIndex = 7
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1000
 * @tc.name   : testGetChipName008
 * @tc.desc   : GetChipName, Test input param, devIndex = 8
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1100
 * @tc.name   : testGetChipName009
 * @tc.desc   : GetChipName, Test input param, devIndex = 9
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1200
 * @tc.name   : testGetChipName010
 * @tc.desc   : GetChipName, Test input param, devIndex = 10
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1300
 * @tc.name   : testGetChipName011
 * @tc.desc   : GetChipName, Test input param, devIndex = 11
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1400
 * @tc.name   : testGetChipName012
 * @tc.desc   : GetChipName, Test input param, devIndex = 12
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1500
 * @tc.name   : testGetChipName013
 * @tc.desc   : GetChipName, Test input param, devIndex = 13
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1600
 * @tc.name   : testGetChipName014
 * @tc.desc   : GetChipName, Test input param, devIndex = 14
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1700
 * @tc.name   : testGetChipName015
 * @tc.desc   : GetChipName, Test input param, devIndex = 15
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1800
 * @tc.name   : testGetChipName016
 * @tc.desc   : GetChipName, Test input param, devIndex = 16
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_1900
 * @tc.name   : testGetChipName017
 * @tc.desc   : GetChipName, Test input param, devIndex = 17
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2000
 * @tc.name   : testGetChipName018
 * @tc.desc   : GetChipName, Test input param, devIndex = 18
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2100
 * @tc.name   : testGetChipName019
 * @tc.desc   : GetChipName, Test input param, devIndex = 19
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2200
 * @tc.name   : testGetChipName020
 * @tc.desc   : GetChipName, Test input param, devIndex = 20
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2300
 * @tc.name   : testGetChipName021
 * @tc.desc   : GetChipName, Test input param, devIndex = 21
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2400
 * @tc.name   : testGetChipName022
 * @tc.desc   : GetChipName, Test input param, devIndex = 22
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2500
 * @tc.name   : testGetChipName023
 * @tc.desc   : GetChipName, Test input param, devIndex = 23
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2600
 * @tc.name   : testGetChipName024
 * @tc.desc   : GetChipName, Test input param, devIndex = 24
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2700
 * @tc.name   : testGetChipName025
 * @tc.desc   : GetChipName, Test input param, devIndex = 25
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2800
 * @tc.name   : testGetChipName026
 * @tc.desc   : GetChipName, Test input param, devIndex = 26
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_2900
 * @tc.name   : testGetChipName027
 * @tc.desc   : GetChipName, Test input param, devIndex = 27
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_3000
 * @tc.name   : testGetChipName028
 * @tc.desc   : GetChipName, Test input param, devIndex = 28
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_3100
 * @tc.name   : testGetChipName029
 * @tc.desc   : GetChipName, Test input param, devIndex = 29
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_3200
 * @tc.name   : testGetChipName030
 * @tc.desc   : GetChipName, Test input param, devIndex = 30
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_3300
 * @tc.name   : testGetChipName031
 * @tc.desc   : GetChipName, Test input param, devIndex = 31
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    std::string chipName;

    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->GetChipName(devIndex, chipName);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_3400
 * @tc.name   : testGetChipName032
 * @tc.desc   : GetChipName, Test input param, devIndex = 32
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetChipName032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = MAX_DEVICES;
    std::string chipName;

    ret = g_inputInterfaces->GetChipName(devIndex, chipName);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0800
 * @tc.name   : testRunCapacitanceTest003
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 3 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest003, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 3;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0900
 * @tc.name   : testRunCapacitanceTest004
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 4 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest004, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 4;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1000
 * @tc.name   : testRunCapacitanceTest005
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 5 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest005, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 5;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1100
 * @tc.name   : testRunCapacitanceTest006
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 6 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest006, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 6;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1200
 * @tc.name   : testRunCapacitanceTest007
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 7 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest007, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 7;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1300
 * @tc.name   : testRunCapacitanceTest008
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 8 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest008, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 8;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1400
 * @tc.name   : testRunCapacitanceTest009
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 9 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest009, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 9;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1500
 * @tc.name   : testRunCapacitanceTest010
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 10 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest010, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 10;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1600
 * @tc.name   : testRunCapacitanceTest011
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 11 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest011, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 11;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1700
 * @tc.name   : testRunCapacitanceTest012
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 12 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest012, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 12;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1800
 * @tc.name   : testRunCapacitanceTest013
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 13 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest013, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 13;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_1900
 * @tc.name   : testRunCapacitanceTest014
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 14 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest014, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 14;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2000
 * @tc.name   : testRunCapacitanceTest015
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 15 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest015, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 15;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2100
 * @tc.name   : testRunCapacitanceTest016
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 16 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest016, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 16;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2200
 * @tc.name   : testRunCapacitanceTest017
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 17 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest017, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 17;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2300
 * @tc.name   : testRunCapacitanceTest018
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 18 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest018, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 18;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2400
 * @tc.name   : testRunCapacitanceTest019
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 19 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest019, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 19;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2500
 * @tc.name   : testRunCapacitanceTest020
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 20 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest020, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 20;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2600
 * @tc.name   : testRunCapacitanceTest021
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 21 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest021, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 21;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2700
 * @tc.name   : testRunCapacitanceTest022
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 22 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest022, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 22;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2800
 * @tc.name   : testRunCapacitanceTest023
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 23 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest023, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 23;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_2900
 * @tc.name   : testRunCapacitanceTest024
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 24 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest024, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 24;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3000
 * @tc.name   : testRunCapacitanceTest025
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 25 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest025, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 25;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3100
 * @tc.name   : testRunCapacitanceTest026
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 26 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest026, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 26;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3200
 * @tc.name   : testRunCapacitanceTest027
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 27 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest027, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 27;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3300
 * @tc.name   : testRunCapacitanceTest028
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 28 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest028, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 28;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3400
 * @tc.name   : testRunCapacitanceTest029
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 29 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest029, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 29;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3500
 * @tc.name   : testRunCapacitanceTest030
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 30 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest030, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 30;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3600
 * @tc.name   : testRunCapacitanceTest031
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 31 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest031, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 31;
    uint32_t testType = BASE_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3700
 * @tc.name   : testRunCapacitanceTest032
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 32 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest032, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = MAX_DEVICES;
    uint32_t testType = BASE_TEST;
    std::string result;
    ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3800
 * @tc.name   : testRunCapacitanceTest033
 * @tc.desc   : Verify the stability of the RunCapacitanceTest function when devIndex is 1 and testType is BASE_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest033, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = TOUCH_INDEX;
    uint32_t testType = BASE_TEST;
    std::string result;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_3900
 * @tc.name   : testRunCapacitanceTest034
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 1 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest034, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 1;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4000
 * @tc.name   : testRunCapacitanceTest035
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 31 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest035, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 31;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4100
 * @tc.name   : testRunCapacitanceTest036
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 1 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest036, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 1;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4200
 * @tc.name   : testRunCapacitanceTest037
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 31 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest037, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 31;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4300
 * @tc.name   : testRunCapacitanceTest038
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 1 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest038, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 1;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4400
 * @tc.name   : testRunCapacitanceTest039
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 31 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest039, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 31;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4500
 * @tc.name   : testRunCapacitanceTest040
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 32 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest040, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = MAX_DEVICES;
    uint32_t testType = FULL_TEST;
    std::string result;
    ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4600
 * @tc.name   : testRunCapacitanceTest041
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 32 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest041, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = MAX_DEVICES;
    uint32_t testType = MMI_TEST;
    std::string result;
    ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4700
 * @tc.name   : testRunCapacitanceTest042
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 32 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest042, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = MAX_DEVICES;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_4900
 * @tc.name   : testRunCapacitanceTest044
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 3 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest044, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 3;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5000
 * @tc.name   : testRunCapacitanceTest045
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 4 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest045, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 4;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5100
 * @tc.name   : testRunCapacitanceTest046
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 5 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest046, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 5;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5200
 * @tc.name   : testRunCapacitanceTest047
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 6 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest047, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 6;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5300
 * @tc.name   : testRunCapacitanceTest048
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 7 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest048, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 7;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5400
 * @tc.name   : testRunCapacitanceTest049
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 8 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest049, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 8;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5500
 * @tc.name   : testRunCapacitanceTest050
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 9 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest050, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 9;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5600
 * @tc.name   : testRunCapacitanceTest051
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 10 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest051, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 10;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5700
 * @tc.name   : testRunCapacitanceTest052
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 11 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest052, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 11;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5800
 * @tc.name   : testRunCapacitanceTest053
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 12 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest053, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 12;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_5900
 * @tc.name   : testRunCapacitanceTest054
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 13 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest054, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 13;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6000
 * @tc.name   : testRunCapacitanceTest055
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 14 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest055, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 14;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6100
 * @tc.name   : testRunCapacitanceTest056
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 15 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest056, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 15;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6200
 * @tc.name   : testRunCapacitanceTest057
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 16 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest057, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 16;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6300
 * @tc.name   : testRunCapacitanceTest058
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 17 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest058, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 17;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6400
 * @tc.name   : testRunCapacitanceTest059
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 18 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest059, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 18;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6500
 * @tc.name   : testRunCapacitanceTest060
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 19 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest060, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 19;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6600
 * @tc.name   : testRunCapacitanceTest061
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 20 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest061, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 20;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6700
 * @tc.name   : testRunCapacitanceTest062
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 21 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest062, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 21;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6800
 * @tc.name   : testRunCapacitanceTest063
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 22 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest063, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 22;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_6900
 * @tc.name   : testRunCapacitanceTest064
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 23 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest064, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 23;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7000
 * @tc.name   : testRunCapacitanceTest065
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 24 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest065, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 24;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7100
 * @tc.name   : testRunCapacitanceTest066
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 25 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest066, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 25;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7200
 * @tc.name   : testRunCapacitanceTest067
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 26 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest067, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 26;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7300
 * @tc.name   : testRunCapacitanceTest068
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 27 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest068, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 27;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7400
 * @tc.name   : testRunCapacitanceTest069
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 28 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest069, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 28;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7500
 * @tc.name   : testRunCapacitanceTest070
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 29 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest070, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 29;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7600
 * @tc.name   : testRunCapacitanceTest071
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 30 and testType is FULL_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest071, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 30;
    uint32_t testType = FULL_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7800
 * @tc.name   : testRunCapacitanceTest073
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 3 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest073, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 3;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_7900
 * @tc.name   : testRunCapacitanceTest074
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 4 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest074, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 4;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8000
 * @tc.name   : testRunCapacitanceTest075
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 6 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest075, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 6;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8100
 * @tc.name   : testRunCapacitanceTest076
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 7 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest076, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 7;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8200
 * @tc.name   : testRunCapacitanceTest077
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 8 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest077, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 8;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8300
 * @tc.name   : testRunCapacitanceTest078
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 9 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest078, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 9;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8400
 * @tc.name   : testRunCapacitanceTest079
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 10 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest079, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 10;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8500
 * @tc.name   : testRunCapacitanceTest080
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 11 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest080, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 11;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8600
 * @tc.name   : testRunCapacitanceTest081
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 12 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest081, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 12;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8700
 * @tc.name   : testRunCapacitanceTest082
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 13 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest082, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 13;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8800
 * @tc.name   : testRunCapacitanceTest083
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 14 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest083, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 14;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_8900
 * @tc.name   : testRunCapacitanceTest084
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 15 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest084, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 15;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9000
 * @tc.name   : testRunCapacitanceTest085
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 16 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest085, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 16;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9100
 * @tc.name   : testRunCapacitanceTest086
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 17 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest086, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 17;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9200
 * @tc.name   : testRunCapacitanceTest087
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 18 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest087, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 18;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9300
 * @tc.name   : testRunCapacitanceTest088
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 19 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest088, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 19;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9400
 * @tc.name   : testRunCapacitanceTest089
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 20 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest089, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 20;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9500
 * @tc.name   : testRunCapacitanceTest090
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 21 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest090, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 21;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9600
 * @tc.name   : testRunCapacitanceTest091
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 22 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest091, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 22;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9700
 * @tc.name   : testRunCapacitanceTest092
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 23 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest092, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 23;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9800
 * @tc.name   : testRunCapacitanceTest093
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 24 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest093, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 24;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_9900
 * @tc.name   : testRunCapacitanceTest094
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 25 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest094, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 25;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0010
 * @tc.name   : testRunCapacitanceTest095
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 26 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest095, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 26;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0020
 * @tc.name   : testRunCapacitanceTest096
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 27 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest096, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 27;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0030
 * @tc.name   : testRunCapacitanceTest097
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 28 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest097, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 28;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0040
 * @tc.name   : testRunCapacitanceTest098
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 29 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest098, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 29;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0050
 * @tc.name   : testRunCapacitanceTest099
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 30 and testType is MMI_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest099, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 30;
    uint32_t testType = MMI_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0070
 * @tc.name   : testRunCapacitanceTest101
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 3 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest101, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 3;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0080
 * @tc.name   : testRunCapacitanceTest102
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 4 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest102, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 4;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0090
 * @tc.name   : testRunCapacitanceTest103
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 6 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest103, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 6;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0110
 * @tc.name   : testRunCapacitanceTest104
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 7 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest104, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 7;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0120
 * @tc.name   : testRunCapacitanceTest105
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 8 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest105, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 8;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0130
 * @tc.name   : testRunCapacitanceTest106
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 9 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest106, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 9;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0140
 * @tc.name   : testRunCapacitanceTest107
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 10 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest107, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 10;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0150
 * @tc.name   : testRunCapacitanceTest108
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 11 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest108, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 11;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0160
 * @tc.name   : testRunCapacitanceTest109
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 12 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest109, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 12;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0170
 * @tc.name   : testRunCapacitanceTest110
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 13 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest110, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 13;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0180
 * @tc.name   : testRunCapacitanceTest111
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 14 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest111, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 14;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0190
 * @tc.name   : testRunCapacitanceTest112
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 15 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest112, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 15;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0210
 * @tc.name   : testRunCapacitanceTest113
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 16 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest113, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 16;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0220
 * @tc.name   : testRunCapacitanceTest114
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 17 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest114, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 17;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0230
 * @tc.name   : testRunCapacitanceTest115
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 18 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest115, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 18;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0240
 * @tc.name   : testRunCapacitanceTest116
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 19 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest116, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 19;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0250
 * @tc.name   : testRunCapacitanceTest117
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 20 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest117, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 20;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0260
 * @tc.name   : testRunCapacitanceTest118
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 21 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest118, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 21;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0270
 * @tc.name   : testRunCapacitanceTest119
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 22 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest119, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 22;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0280
 * @tc.name   : testRunCapacitanceTest120
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 23 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest120, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 23;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0290
 * @tc.name   : testRunCapacitanceTest121
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 24 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest121, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 24;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0310
 * @tc.name   : testRunCapacitanceTest122
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 25 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest122, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 25;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0320
 * @tc.name   : testRunCapacitanceTest123
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 26 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest123, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 26;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0330
 * @tc.name   : testRunCapacitanceTest124
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 27 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest124, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 27;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0340
 * @tc.name   : testRunCapacitanceTest125
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 28 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest125, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 28;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0350
 * @tc.name   : testRunCapacitanceTest126
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 29 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest126, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 29;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunCapacitanceTest_0360
 * @tc.name   : testRunCapacitanceTest127
 * @tc.desc   : Verify the reliability of the RunCapacitanceTest function when devIndex is 30 and testType is
 * RUNNING_TEST.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunCapacitanceTest127, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 30;
    uint32_t testType = RUNNING_TEST;
    std::string result;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunCapacitanceTest(devIndex, testType, result, TEST_RESULT_LEN);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_0400
 * @tc.name   : testRunExtraCommand002
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 2.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand002, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 2;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_0500
 * @tc.name   : testRunExtraCommand003
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 3.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand003, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 3;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_0600
 * @tc.name   : testRunExtraCommand004
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 4.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand004, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 4;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_0700
 * @tc.name   : testRunExtraCommand005
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 6.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand005, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 6;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_0800
 * @tc.name   : testRunExtraCommand006
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 7.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand006, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 7;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_0900
 * @tc.name   : testRunExtraCommand007
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 8.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand007, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 8;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1000
 * @tc.name   : testRunExtraCommand008
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 9.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand008, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 9;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1100
 * @tc.name   : testRunExtraCommand009
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 10.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand009, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 10;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1200
 * @tc.name   : testRunExtraCommand010
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 11.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand010, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 11;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1300
 * @tc.name   : testRunExtraCommand011
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 12.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand011, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 12;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1400
 * @tc.name   : testRunExtraCommand012
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 13.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand012, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 13;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1500
 * @tc.name   : testRunExtraCommand013
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 14.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand013, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 14;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1600
 * @tc.name   : testRunExtraCommand014
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 15.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand014, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 15;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1700
 * @tc.name   : testRunExtraCommand015
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 16.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand015, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 16;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1800
 * @tc.name   : testRunExtraCommand016
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 17.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand016, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 17;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_1900
 * @tc.name   : testRunExtraCommand017
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 18.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand017, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 18;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2000
 * @tc.name   : testRunExtraCommand018
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 19.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand018, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 19;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2100
 * @tc.name   : testRunExtraCommand019
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 20.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand019, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 20;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2200
 * @tc.name   : testRunExtraCommand020
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 21.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand020, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 21;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2300
 * @tc.name   : testRunExtraCommand021
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 22.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand021, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 22;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2400
 * @tc.name   : testRunExtraCommand022
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 23.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand022, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 23;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2500
 * @tc.name   : testRunExtraCommand023
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 24.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand023, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 24;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2600
 * @tc.name   : testRunExtraCommand024
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 25.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand024, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 25;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2700
 * @tc.name   : testRunExtraCommand025
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 26.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand025, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 26;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2800
 * @tc.name   : testRunExtraCommand026
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 27.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand026, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 27;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_2900
 * @tc.name   : testRunExtraCommand027
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 28.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand027, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 28;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3000
 * @tc.name   : testRunExtraCommand028
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 29.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand028, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 29;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3100
 * @tc.name   : testRunExtraCommand029
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 30.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand029, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 30;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3200
 * @tc.name   : testRunExtraCommand030
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 31.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand030, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 31;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3300
 * @tc.name   : testRunExtraCommand031
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 32.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand031, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = MAX_DEVICES;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3400
 * @tc.name   : testRunExtraCommand032
 * @tc.desc   : Verify the stability of the RunCapacitanceTest function when devIndex is 1.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand032, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = TOUCH_INDEX;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3500
 * @tc.name   : testRunExtraCommand033
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 1.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand033, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 1;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "";
    extraCmd.cmdValue = "";
    ret = g_inputInterfaces->OpenInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->CloseInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3600
 * @tc.name   : testRunExtraCommand034
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 1.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand034, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 1;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "";
    extraCmd.cmdValue = "Enable";
    ret = g_inputInterfaces->OpenInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->CloseInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RunExtraCommand_3700
 * @tc.name   : testRunExtraCommand035
 * @tc.desc   : Verify the reliability of the RunExtraCommand function when devIndex is 1.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRunExtraCommand035, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 1;
    struct ExtraCmd extraCmd;
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "";
    ret = g_inputInterfaces->OpenInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->RunExtraCommand(devIndex, extraCmd);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->CloseInputDevice(devIndex);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_0400
 * @tc.name   : testRegisterReportCallback002
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 2.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback002, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 2;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_0500
 * @tc.name   : testRegisterReportCallback003
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 3.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback003, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 3;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_0600
 * @tc.name   : testRegisterReportCallback004
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 4.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback004, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 4;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_0700
 * @tc.name   : testRegisterReportCallback005
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 6.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback005, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 6;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_0800
 * @tc.name   : testRegisterReportCallback006
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 7.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback006, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 7;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_0900
 * @tc.name   : testRegisterReportCallback007
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 8.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback007, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 8;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1000
 * @tc.name   : testRegisterReportCallback008
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 9.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback008, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 9;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1100
 * @tc.name   : testRegisterReportCallback009
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 10.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback009, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 10;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1200
 * @tc.name   : testRegisterReportCallback010
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 11.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback010, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 11;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1300
 * @tc.name   : testRegisterReportCallback011
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 12.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback011, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 12;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1400
 * @tc.name   : testRegisterReportCallback012
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 13.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback012, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 13;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1500
 * @tc.name   : testRegisterReportCallback013
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 14.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback013, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 14;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1600
 * @tc.name   : testRegisterReportCallback014
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 15.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback014, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 15;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1700
 * @tc.name   : testRegisterReportCallback015
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 16.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback015, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 16;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1800
 * @tc.name   : testRegisterReportCallback016
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 17.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback016, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 17;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_1900
 * @tc.name   : testRegisterReportCallback017
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 18.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback017, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 18;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2000
 * @tc.name   : testRegisterReportCallback018
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 19.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback018, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 19;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2100
 * @tc.name   : testRegisterReportCallback019
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 20.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback019, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 20;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2200
 * @tc.name   : testRegisterReportCallback020
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 21.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback020, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 21;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2300
 * @tc.name   : testRegisterReportCallback021
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 22.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback021, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 22;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2400
 * @tc.name   : testRegisterReportCallback022
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 23.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback022, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 23;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2500
 * @tc.name   : testRegisterReportCallback023
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 24.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback023, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 24;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2600
 * @tc.name   : testRegisterReportCallback024
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 25.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback024, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 25;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2700
 * @tc.name   : testRegisterReportCallback025
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 26.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback025, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 26;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2800
 * @tc.name   : testRegisterReportCallback026
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 27.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback026, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 27;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_2900
 * @tc.name   : testRegisterReportCallback027
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 28.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback027, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 28;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_3000
 * @tc.name   : testRegisterReportCallback028
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 29.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback028, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 29;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_3100
 * @tc.name   : testRegisterReportCallback029
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 30.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback029, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 30;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_3200
 * @tc.name   : testRegisterReportCallback030
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 31.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback030, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = 31;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_3300
 * @tc.name   : testRegisterReportCallback031
 * @tc.desc   : Verify the reliability of the RegisterReportCallback function when devIndex is 32.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback031, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = MAX_DEVICES;
    ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RegisterReportCallback_3400
 * @tc.name   : testRegisterReportCallback032
 * @tc.desc   : Verify the stability of the RegisterReportCallback function when devIndex is 1.
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterReportCallback032, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    uint32_t devIndex = TOUCH_INDEX;
    for (int32_t i = 0; i < 1000; i++) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->RegisterReportCallback(devIndex, g_callback);
        EXPECT_EQ(ret, INPUT_SUCCESS);
        ret = g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_RegisterHotPlugCallback_0100
 * @tc.name   : testRegisterHotPlugCallback001
 * @tc.desc   : Validation function RegisterHotPlugCallback results in success.
 * when the hotPlugCallback parameter is g_callback
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterHotPlugCallback001, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    ret = g_inputInterfaces->RegisterHotPlugCallback(g_callback);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterfaces->UnregisterHotPlugCallback();
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_RegisterHotPlugCallback_0200
 * @tc.name   : testRegisterHotPlugCallback002
 * @tc.desc   : Validation function RegisterHotPlugCallback results in success.
 * when the hotPlugCallback parameter is nullptr
 */
HWTEST_F(HdfInputHdiTestAdditional, testRegisterHotPlugCallback002, Function | MediumTest | Level2)
{
    int32_t ret = INPUT_SUCCESS;
    ret = g_inputInterfaces->RegisterHotPlugCallback(nullptr);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_UnregisterHotPlugCallback_0100
 * @tc.name   : testUnregisterHotPlugCallback001
 * @tc.desc   : Verify the reliability of UnregisterHotPlugCallback function and functional
 */
HWTEST_F(HdfInputHdiTestAdditional, testUnregisterHotPlugCallback001, Function | MediumTest | Level1)
{
    int32_t ret = INPUT_SUCCESS;
    ret = g_inputInterfaces->UnregisterHotPlugCallback();
    EXPECT_EQ(ret, INPUT_SUCCESS);
}
