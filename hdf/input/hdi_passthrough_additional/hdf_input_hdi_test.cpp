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
        // devIndex 1：touch
        ret |= g_inputInterfaces->OpenInputDevice(TOUCH_INDEX);
        g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0500
 * @tc.name   : testOpenInputDevice003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0600
 * @tc.name   : testOpenInputDevice004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0700
 * @tc.name   : testOpenInputDevice005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0800
 * @tc.name   : testOpenInputDevice006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_0900
 * @tc.name   : testOpenInputDevice007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1000
 * @tc.name   : testOpenInputDevice008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1100
 * @tc.name   : testOpenInputDevice009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1200
 * @tc.name   : testOpenInputDevice010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1300
 * @tc.name   : testOpenInputDevice011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1400
 * @tc.name   : testOpenInputDevice012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1500
 * @tc.name   : testOpenInputDevice013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1600
 * @tc.name   : testOpenInputDevice014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1700
 * @tc.name   : testOpenInputDevice015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1800
 * @tc.name   : testOpenInputDevice016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_1900
 * @tc.name   : testOpenInputDevice017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2000
 * @tc.name   : testOpenInputDevice018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2100
 * @tc.name   : testOpenInputDevice019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2200
 * @tc.name   : testOpenInputDevice020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2300
 * @tc.name   : testOpenInputDevice021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2400
 * @tc.name   : testOpenInputDevice022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2500
 * @tc.name   : testOpenInputDevice023
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice023, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2600
 * @tc.name   : testOpenInputDevice024
 * @tc.desc   : Reliability of function(同上)
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2700
 * @tc.name   : testOpenInputDevice025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2800
 * @tc.name   : testOpenInputDevice026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_2900
 * @tc.name   : testOpenInputDevice027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_3000
 * @tc.name   : testOpenInputDevice028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_3100
 * @tc.name   : testOpenInputDevice029
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_3200
 * @tc.name   : testOpenInputDevice030
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_3300
 * @tc.name   : testOpenInputDevice031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    if (IsOnlineDev(devIndex)) {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        g_inputInterfaces->CloseInputDevice(devIndex);
        EXPECT_EQ(ret, INPUT_SUCCESS);
    } else {
        ret = g_inputInterfaces->OpenInputDevice(devIndex);
        EXPECT_NE(ret, INPUT_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Input_OpenInputDevice_3400
 * @tc.name   : testOpenInputDevice032
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testOpenInputDevice032, Function | MediumTest | Level2)
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
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0500
 * @tc.name   : testCloseInputDevice003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0600
 * @tc.name   : testCloseInputDevice004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0700
 * @tc.name   : testCloseInputDevice005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0800
 * @tc.name   : testCloseInputDevice006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_0900
 * @tc.name   : testCloseInputDevice007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1000
 * @tc.name   : testCloseInputDevice008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1100
 * @tc.name   : testCloseInputDevice009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1200
 * @tc.name   : testCloseInputDevice010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1300
 * @tc.name   : testCloseInputDevice011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1400
 * @tc.name   : testCloseInputDevice012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1500
 * @tc.name   : testCloseInputDevice013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1600
 * @tc.name   : testCloseInputDevice014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1700
 * @tc.name   : testCloseInputDevice015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1800
 * @tc.name   : testCloseInputDevice016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_1900
 * @tc.name   : testCloseInputDevice017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2000
 * @tc.name   : testCloseInputDevice018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2100
 * @tc.name   : testCloseInputDevice019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2200
 * @tc.name   : testCloseInputDevice020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2300
 * @tc.name   : testCloseInputDevice021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2400
 * @tc.name   : testCloseInputDevice022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2500
 * @tc.name   : testCloseInputDevice023
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2600
 * @tc.name   : testCloseInputDevice024
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2700
 * @tc.name   : testCloseInputDevice025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2800
 * @tc.name   : testCloseInputDevice026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_2900
 * @tc.name   : testCloseInputDevice027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_3000
 * @tc.name   : testCloseInputDevice028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_3100
 * @tc.name   : testCloseInputDevice029
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_3200
 * @tc.name   : testCloseInputDevice030
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_3300
 * @tc.name   : testCloseInputDevice031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
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

/**
 * @tc.number : SUB_Driver_Input_CloseInputDevice_3400
 * @tc.name   : testCloseInputDevice032
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testCloseInputDevice032, Function | MediumTest | Level2)
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
 * @tc.number : SUB_Driver_Input_GetInputDevice_0500
 * @tc.name   : testGetInputDevice003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice003, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 2;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0600
 * @tc.name   : testGetInputDevice004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice004, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 3;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0700
 * @tc.name   : testGetInputDevice005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice005, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 4;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0800
 * @tc.name   : testGetInputDevice006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice006, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 6;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_0900
 * @tc.name   : testGetInputDevice007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice007, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 7;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1000
 * @tc.name   : testGetInputDevice008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice008, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 8;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1100
 * @tc.name   : testGetInputDevice009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice009, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 9;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1200
 * @tc.name   : testGetInputDevice010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice010, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 10;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1300
 * @tc.name   : testGetInputDevice011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice011, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 11;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1400
 * @tc.name   : testGetInputDevice012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice012, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 12;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1500
 * @tc.name   : testGetInputDevice013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice013, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 13;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1600
 * @tc.name   : testGetInputDevice014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice014, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 14;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1700
 * @tc.name   : testGetInputDevice015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice015, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 15;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1800
 * @tc.name   : testGetInputDevice016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice016, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 16;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_1900
 * @tc.name   : testGetInputDevice017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice017, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 17;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2000
 * @tc.name   : testGetInputDevice018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice018, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 18;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2100
 * @tc.name   : testGetInputDevice019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice019, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 19;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2200
 * @tc.name   : testGetInputDevice020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice020, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 20;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2300
 * @tc.name   : testGetInputDevice021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice021, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 21;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2400
 * @tc.name   : testGetInputDevice022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice022, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 22;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2500
 * @tc.name   : testGetInputDevice023
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice023, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 23;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2600
 * @tc.name   : testGetInputDevice024
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice024, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 24;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2700
 * @tc.name   : testGetInputDevice025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice025, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 25;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2800
 * @tc.name   : testGetInputDevice026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice026, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 26;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_2900
 * @tc.name   : testGetInputDevice027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice027, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 27;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_3000
 * @tc.name   : testGetInputDevice028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice028, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 28;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_3100
 * @tc.name   : testGetInputDevice029
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice029, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 29;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_3200
 * @tc.name   : testGetInputDevice030
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice030, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 30;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_3300
 * @tc.name   : testGetInputDevice031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice031, Function | MediumTest | Level1)
{
    struct DeviceInfo devInfo;
    int32_t ret = 0;
    uint32_t devIndex = 31;
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

/**
 * @tc.number : SUB_Driver_Input_GetInputDevice_3400
 * @tc.name   : testGetInputDevice032
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetInputDevice032, Function | MediumTest | Level2)
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
        // devIndex 1：touch
        ret = g_inputInterfaces->SetPowerStatus(TOUCH_INDEX, setStatus);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0700
 * @tc.name   : testSetPowerStatus003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0800
 * @tc.name   : testSetPowerStatus004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_0900
 * @tc.name   : testSetPowerStatus005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1000
 * @tc.name   : testSetPowerStatus006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1100
 * @tc.name   : testSetPowerStatus007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1200
 * @tc.name   : testSetPowerStatus008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1300
 * @tc.name   : testSetPowerStatus009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1400
 * @tc.name   : testSetPowerStatus010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1500
 * @tc.name   : testSetPowerStatus011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1600
 * @tc.name   : testSetPowerStatus012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1700
 * @tc.name   : testSetPowerStatus013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1800
 * @tc.name   : testSetPowerStatus014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_1900
 * @tc.name   : testSetPowerStatus015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2000
 * @tc.name   : testSetPowerStatus016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2100
 * @tc.name   : testSetPowerStatus017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2200
 * @tc.name   : testSetPowerStatus018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2300
 * @tc.name   : testSetPowerStatus019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2400
 * @tc.name   : testSetPowerStatus020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2500
 * @tc.name   : testSetPowerStatus021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2600
 * @tc.name   : testSetPowerStatus022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2700
 * @tc.name   : testSetPowerStatus023
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2800
 * @tc.name   : testSetPowerStatus024
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_2900
 * @tc.name   : testSetPowerStatus025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3000
 * @tc.name   : testSetPowerStatus026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3100
 * @tc.name   : testSetPowerStatus027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3200
 * @tc.name   : testSetPowerStatus028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3300
 * @tc.name   : testSetPowerStatus029
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3400
 * @tc.name   : testSetPowerStatus030
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3500
 * @tc.name   : testSetPowerStatus031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    uint32_t setStatus = INPUT_SUSPEND;
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

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3600
 * @tc.name   : testSetPowerStatus032
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = INPUT_SUSPEND;
    ret = g_inputInterfaces->SetPowerStatus(MAX_DEVICES, setStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3700
 * @tc.name   : testSetPowerStatus033
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus033, Function | MediumTest | Level2)
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
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3800
 * @tc.name   : testSetPowerStatus034
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus034, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t setStatus = INPUT_POWER_STATUS_UNKNOWN;
    ret = g_inputInterfaces->SetPowerStatus(MAX_DEVICES, setStatus);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetPowerStatus_3900
 * @tc.name   : testSetPowerStatus035
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus035, Function | MediumTest | Level2)
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
 * @tc.number : SUB_Driver_Input_SetPowerStatus_4000
 * @tc.name   : testSetPowerStatus036
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetPowerStatus036, Function | MediumTest | Level2)
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
        // devIndex 1：touch
        ret = g_inputInterfaces->GetPowerStatus(TOUCH_INDEX, getStatus);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
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
        // devIndex 1：touch
        ret = g_inputInterfaces->GetDeviceType(TOUCH_INDEX, devType);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0500
 * @tc.name   : testGetDeviceType003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0600
 * @tc.name   : testGetDeviceType004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0700
 * @tc.name   : testGetDeviceType005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0800
 * @tc.name   : testGetDeviceType006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_0900
 * @tc.name   : testGetDeviceType007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1000
 * @tc.name   : testGetDeviceType008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1100
 * @tc.name   : testGetDeviceType009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1200
 * @tc.name   : testGetDeviceType010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1300
 * @tc.name   : testGetDeviceType011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1400
 * @tc.name   : testGetDeviceType012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1500
 * @tc.name   : testGetDeviceType013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1600
 * @tc.name   : testGetDeviceType014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1700
 * @tc.name   : testGetDeviceType015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1800
 * @tc.name   : testGetDeviceType016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_1900
 * @tc.name   : testGetDeviceType017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2000
 * @tc.name   : testGetDeviceType018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2100
 * @tc.name   : testGetDeviceType019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2200
 * @tc.name   : testGetDeviceType020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2300
 * @tc.name   : testGetDeviceType021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2400
 * @tc.name   : testGetDeviceType022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2500
 * @tc.name   : testGetDeviceType023
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2600
 * @tc.name   : testGetDeviceType024
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2700
 * @tc.name   : testGetDeviceType025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2800
 * @tc.name   : testGetDeviceType026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_2900
 * @tc.name   : testGetDeviceType027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_3000
 * @tc.name   : testGetDeviceType028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_3100
 * @tc.name   : testGetDeviceType029
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_3200
 * @tc.name   : testGetDeviceType030
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_3300
 * @tc.name   : testGetDeviceType031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    uint32_t devType = INIT_DEFAULT_VALUE;
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

/**
 * @tc.number : SUB_Driver_Input_GetDeviceType_3400
 * @tc.name   : testGetDeviceType032
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testGetDeviceType032, Function | MediumTest | Level2)
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
        // devIndex 1：touch
        ret = g_inputInterfaces->SetGestureMode(TOUCH_INDEX, gestureMode);
    }
    g_inputInterfaces->CloseInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0500
 * @tc.name   : testSetGestureMode003
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 2;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0600
 * @tc.name   : testSetGestureMode004
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode004, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 3;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0700
 * @tc.name   : testSetGestureMode005
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode005, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 4;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0800
 * @tc.name   : testSetGestureMode006
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode006, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 6;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_0900
 * @tc.name   : testSetGestureMode007
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode007, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 7;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1000
 * @tc.name   : testSetGestureMode008
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode008, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 8;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1100
 * @tc.name   : testSetGestureMode009
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode009, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 9;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1200
 * @tc.name   : testSetGestureMode010
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode010, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 10;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1300
 * @tc.name   : testSetGestureMode011
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 11;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1400
 * @tc.name   : testSetGestureMode012
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode012, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 12;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1500
 * @tc.name   : testSetGestureMode013
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode013, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 13;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1600
 * @tc.name   : testSetGestureMode014
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode014, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 14;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1700
 * @tc.name   : testSetGestureMode015
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode015, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 15;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1800
 * @tc.name   : testSetGestureMode016
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode016, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 16;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_1900
 * @tc.name   : testSetGestureMode017
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode017, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 17;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2000
 * @tc.name   : testSetGestureMode018
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode018, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 18;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2100
 * @tc.name   : testSetGestureMode019
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode019, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 19;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2200
 * @tc.name   : testSetGestureMode020
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 20;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2300
 * @tc.name   : testSetGestureMode021
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode021, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 21;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2400
 * @tc.name   : testSetGestureMode022
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode022, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 22;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2500
 * @tc.name   : testSetGestureMode023
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode023, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 23;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2600
 * @tc.name   : testSetGestureMode024
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode024, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 24;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2700
 * @tc.name   : testSetGestureMode025
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode025, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 25;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2800
 * @tc.name   : testSetGestureMode026
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode026, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 26;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_2900
 * @tc.name   : testSetGestureMode027
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode027, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 27;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_3000
 * @tc.name   : testSetGestureMode028
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode028, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 28;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_3100
 * @tc.name   : testSetGestureMode029
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode029, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 29;
    uint32_t gestureMode = 1;
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

/**
 *
 * @tc.number : SUB_Driver_Input_SetGestureMode_3200
 * @tc.name   : testSetGestureMode030
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode030, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 30;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_3300
 * @tc.name   : testSetGestureMode031
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode031, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint32_t devIndex = 31;
    uint32_t gestureMode = 1;
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

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_3400
 * @tc.name   : testSetGestureMode032
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t gestureMode = 1;
    ret = g_inputInterfaces->SetGestureMode(MAX_DEVICES, gestureMode);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_SetGestureMode_3500
 * @tc.name   : testSetGestureMode033
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode033, Function | MediumTest | Level1)
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
 * @tc.number : SUB_Driver_Input_SetGestureMode_3600
 * @tc.name   : testSetGestureMode034
 * @tc.desc   : Test input param
 */
HWTEST_F(HdfInputHdiTestAdditional, testSetGestureMode034, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo001
 * @tc.desc   : GetChipInfo, stability test
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo001, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo003
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 2
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo003, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo004
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 3
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo004, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo005
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 4
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo005, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo006
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 6
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo006, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo007
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 7
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo007, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo008
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 8
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo008, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo009
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 9
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo009, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo010
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 10
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo010, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo011
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 11
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo011, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo012
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 12
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo012, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo013
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 13
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo013, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo014
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 14
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo014, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo015
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 15
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo015, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo016
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 16
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo016, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo017
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 17
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo017, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo018
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 18
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo018, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo019
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 19
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo019, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo020
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 20
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo020, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo021
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 21
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo021, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo022
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 22
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo022, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo023
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 23
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo023, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo024
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 24
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo024, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo025
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 25
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo025, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo026
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 26
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo026, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo027
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 27
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo027, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo028
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 28
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo028, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo029
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 29
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo029, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo030
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 30
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo030, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo031
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 31
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo031, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipInfo032
 * @tc.desc   : GetChipInfo, Test input param, devIndex = 32
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipInfo032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = MAX_DEVICES;
    std::string chipInfo;

    ret = g_inputInterfaces->GetChipInfo(devIndex, chipInfo);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetVendorName_0300
 * @tc.name   : tesGetVendorName001
 * @tc.desc   : GetVendorName, stability test
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName001, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName003
 * @tc.desc   : GetVendorName, Test input param, devIndex = 2
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName003, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName004
 * @tc.desc   : GetVendorName, Test input param, devIndex = 3
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName004, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName005
 * @tc.desc   : GetVendorName, Test input param, devIndex = 4
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName005, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName006
 * @tc.desc   : GetVendorName, Test input param, devIndex = 6
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName006, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName007
 * @tc.desc   : GetVendorName, Test input param, devIndex = 7
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName007, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName008
 * @tc.desc   : GetVendorName, Test input param, devIndex = 8
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName008, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName009
 * @tc.desc   : GetVendorName, Test input param, devIndex = 9
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName009, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName010
 * @tc.desc   : GetVendorName, Test input param, devIndex = 10
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName010, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName011
 * @tc.desc   : GetVendorName, Test input param, devIndex = 11
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName011, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName012
 * @tc.desc   : GetVendorName, Test input param, devIndex = 12
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName012, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName013
 * @tc.desc   : GetVendorName, Test input param, devIndex = 13
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName013, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName014
 * @tc.desc   : GetVendorName, Test input param, devIndex = 14
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName014, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName015
 * @tc.desc   : GetVendorName, Test input param, devIndex = 15
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName015, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName016
 * @tc.desc   : GetVendorName, Test input param, devIndex = 16
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName016, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName017
 * @tc.desc   : GetVendorName, Test input param, devIndex = 17
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName017, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName018
 * @tc.desc   : GetVendorName, Test input param, devIndex = 18
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName018, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName019
 * @tc.desc   : GetVendorName, Test input param, devIndex = 19
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName019, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName020
 * @tc.desc   : GetVendorName, Test input param, devIndex = 20
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName020, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName021
 * @tc.desc   : GetVendorName, Test input param, devIndex = 21
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName021, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName022
 * @tc.desc   : GetVendorName, Test input param, devIndex = 22
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName022, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName023
 * @tc.desc   : GetVendorName, Test input param, devIndex = 23
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName023, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName024
 * @tc.desc   : GetVendorName, Test input param, devIndex = 24
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName024, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName025
 * @tc.desc   : GetVendorName, Test input param, devIndex = 25
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName025, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName026
 * @tc.desc   : GetVendorName, Test input param, devIndex = 26
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName026, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName027
 * @tc.desc   : GetVendorName, Test input param, devIndex = 27
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName027, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName028
 * @tc.desc   : GetVendorName, Test input param, devIndex = 28
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName028, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName029
 * @tc.desc   : GetVendorName, Test input param, devIndex = 29
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName029, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName030
 * @tc.desc   : GetVendorName, Test input param, devIndex = 30
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName030, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName031
 * @tc.desc   : GetVendorName, Test input param, devIndex = 31
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName031, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetVendorName032
 * @tc.desc   : GetVendorName, Test input param, devIndex = 32
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetVendorName032, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint32_t devIndex = MAX_DEVICES;
    std::string vendorName;

    ret = g_inputInterfaces->GetVendorName(devIndex, vendorName);
    EXPECT_NE(ret, INPUT_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Input_GetChipName_0300
 * @tc.name   : tesGetChipName001
 * @tc.desc   : GetChipName, stability test
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName001, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName003
 * @tc.desc   : GetChipName, Test input param, devIndex = 2
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName003, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName004
 * @tc.desc   : GetChipName, Test input param, devIndex = 3
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName004, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName005
 * @tc.desc   : GetChipName, Test input param, devIndex = 4
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName005, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName006
 * @tc.desc   : GetChipName, Test input param, devIndex = 6
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName006, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName007
 * @tc.desc   : GetChipName, Test input param, devIndex = 7
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName007, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName008
 * @tc.desc   : GetChipName, Test input param, devIndex = 8
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName008, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName009
 * @tc.desc   : GetChipName, Test input param, devIndex = 9
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName009, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName010
 * @tc.desc   : GetChipName, Test input param, devIndex = 10
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName010, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName011
 * @tc.desc   : GetChipName, Test input param, devIndex = 11
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName011, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName012
 * @tc.desc   : GetChipName, Test input param, devIndex = 12
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName012, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName013
 * @tc.desc   : GetChipName, Test input param, devIndex = 13
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName013, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName014
 * @tc.desc   : GetChipName, Test input param, devIndex = 14
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName014, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName015
 * @tc.desc   : GetChipName, Test input param, devIndex = 15
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName015, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName016
 * @tc.desc   : GetChipName, Test input param, devIndex = 16
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName016, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName017
 * @tc.desc   : GetChipName, Test input param, devIndex = 17
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName017, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName018
 * @tc.desc   : GetChipName, Test input param, devIndex = 18
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName018, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName019
 * @tc.desc   : GetChipName, Test input param, devIndex = 19
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName019, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName020
 * @tc.desc   : GetChipName, Test input param, devIndex = 20
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName020, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName021
 * @tc.desc   : GetChipName, Test input param, devIndex = 21
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName021, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName022
 * @tc.desc   : GetChipName, Test input param, devIndex = 22
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName022, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName023
 * @tc.desc   : GetChipName, Test input param, devIndex = 23
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName023, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName024
 * @tc.desc   : GetChipName, Test input param, devIndex = 24
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName024, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName025
 * @tc.desc   : GetChipName, Test input param, devIndex = 25
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName025, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName026
 * @tc.desc   : GetChipName, Test input param, devIndex = 26
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName026, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName027
 * @tc.desc   : GetChipName, Test input param, devIndex = 27
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName027, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName028
 * @tc.desc   : GetChipName, Test input param, devIndex = 28
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName028, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName029
 * @tc.desc   : GetChipName, Test input param, devIndex = 29
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName029, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName030
 * @tc.desc   : GetChipName, Test input param, devIndex = 30
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName030, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName031
 * @tc.desc   : GetChipName, Test input param, devIndex = 31
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName031, Function | MediumTest | Level1)
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
 * @tc.name   : tesGetChipName032
 * @tc.desc   : GetChipName, Test input param, devIndex = 32
 */
HWTEST_F(HdfInputHdiTestAdditional, tesGetChipName032, Function | MediumTest | Level2)
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
