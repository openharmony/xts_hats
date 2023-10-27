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

#include "hdi_composer_ut.h"
#include <chrono>
#include <cinttypes>
#include <algorithm>
#include "v1_0/include/idisplay_composer_interface.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "display_test.h"
#include "display_test_utils.h"
#include "hdi_composition_check.h"
#include "hdi_test_device.h"
#include "hdi_test_device_common.h"
#include "hdi_test_display.h"
#include "hdi_test_render_utils.h"

using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::TEST;
using namespace testing::ext;

static sptr<IDisplayComposerInterface> g_composerDevice = nullptr;
static std::shared_ptr<IDisplayBuffer> g_gralloc = nullptr;
static std::vector<uint32_t> g_displayIds;

void DeviceTestAdditional::SetUpTestCase()
{
    int ret = HdiTestDevice::GetInstance().InitDevice();
    ASSERT_TRUE(ret == DISPLAY_SUCCESS);

    g_composerDevice = HdiTestDevice::GetInstance().GetDeviceInterface();
    ASSERT_TRUE(g_composerDevice != nullptr);

    g_gralloc.reset(IDisplayBuffer::Get());
    ASSERT_TRUE(g_gralloc != nullptr);

    g_displayIds = HdiTestDevice::GetInstance().GetDevIds();
    ASSERT_TRUE(g_displayIds.size() > 0);
}

void DeviceTestAdditional::TearDownTestCase()
{
    HdiTestDevice::GetInstance().Clear();
    HdiTestDevice::GetInstance().GetFirstDisplay()->ResetClientLayer();
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0100
 * @tc.name   : testClientBufferCount001
 * @tc.desc   : test cache count Max
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount001, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = 20;
    auto ret = g_composerDevice->SetClientBufferCacheCount(g_displayIds[0], cacheCount);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0200
 * @tc.name   : testClientBufferCount002
 * @tc.desc   : test cache count Min
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount002, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = 0;
    auto ret = g_composerDevice->SetClientBufferCacheCount(g_displayIds[0], cacheCount);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0300
 * @tc.name   : testClientBufferCount003
 * @tc.desc   : test cache count Random
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount003, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = 15;
    auto ret = g_composerDevice->SetClientBufferCacheCount(g_displayIds[0], cacheCount);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0400
 * @tc.name   : testClientBufferCount004
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount004, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = 5;
    auto ret = g_composerDevice->SetClientBufferCacheCount(20, cacheCount);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0500
 * @tc.name   : testClientBufferCount005
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount005, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = 5;
    auto ret = g_composerDevice->SetClientBufferCacheCount(0, cacheCount);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0600
 * @tc.name   : testClientBufferCount006
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount006, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = 5;
    auto ret = g_composerDevice->SetClientBufferCacheCount(15, cacheCount);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0700
 * @tc.name   : testClientBufferCount007
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount007, Function | MediumTest | Level2)
{
    const uint32_t cacheCount = 5;
    auto ret = g_composerDevice->SetClientBufferCacheCount(-1, cacheCount);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_ClientBufferCount_0800
 * @tc.name   : testClientBufferCount008
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testClientBufferCount008, Function | MediumTest | Level1)
{
    const uint32_t cacheCount = -1;
    auto ret = g_composerDevice->SetClientBufferCacheCount(g_displayIds[0], cacheCount);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Capability_0100
 * @tc.name   : testCapability001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testCapability001, Function | MediumTest | Level1)
{
    DisplayCapability info;
    auto ret = g_composerDevice->GetDisplayCapability(20, info);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Capability_0200
 * @tc.name   : testCapability002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testCapability002, Function | MediumTest | Level1)
{
    DisplayCapability info;
    auto ret = g_composerDevice->GetDisplayCapability(0, info);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Capability_0300
 * @tc.name   : testCapability003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testCapability003, Function | MediumTest | Level1)
{
    DisplayCapability info;
    auto ret = g_composerDevice->GetDisplayCapability(8, info);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_Capability_0400
 * @tc.name   : testCapability004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testCapability004, Function | MediumTest | Level2)
{
    DisplayCapability info;
    auto ret = g_composerDevice->GetDisplayCapability(-1, info);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SupportedModes_0100
 * @tc.name   : testSupportedModes001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testSupportedModes001, Function | MediumTest | Level1)
{
    std::vector<DisplayModeInfo> modes;
    auto ret = g_composerDevice->GetDisplaySupportedModes(20, modes);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SupportedModes_0200
 * @tc.name   : testSupportedModes002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testSupportedModes002, Function | MediumTest | Level1)
{
    std::vector<DisplayModeInfo> modes;
    auto ret = g_composerDevice->GetDisplaySupportedModes(0, modes);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SupportedModes_0300
 * @tc.name   : testSupportedModes003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testSupportedModes003, Function | MediumTest | Level1)
{
    std::vector<DisplayModeInfo> modes;
    auto ret = g_composerDevice->GetDisplaySupportedModes(8, modes);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SupportedModes_0400
 * @tc.name   : testSupportedModes004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testSupportedModes004, Function | MediumTest | Level2)
{
    std::vector<DisplayModeInfo> modes;
    auto ret = g_composerDevice->GetDisplaySupportedModes(-1, modes);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0100
 * @tc.name   :	testDisplayMode001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode001, Function | MediumTest | Level1)
{
    uint32_t MODE = 0;
    auto ret = g_composerDevice->GetDisplayMode(20, MODE);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0200
 * @tc.name   : testDisplayMode002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode002, Function | MediumTest | Level1)
{
    uint32_t MODE = 0;
    auto ret = g_composerDevice->GetDisplayMode(0, MODE);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0300
 * @tc.name   : testDisplayMode003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode003, Function | MediumTest | Level1)
{
    uint32_t MODE = 0;
    auto ret = g_composerDevice->GetDisplayMode(15, MODE);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0400
 * @tc.name   : testDisplayMode004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode004, Function | MediumTest | Level1)
{
    uint32_t MODE = 0;
    auto ret = g_composerDevice->GetDisplayMode(-1, MODE);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

