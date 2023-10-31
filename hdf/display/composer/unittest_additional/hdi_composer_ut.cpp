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
    uint32_t mode = 0;
    auto ret = g_composerDevice->GetDisplayMode(20, mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0200
 * @tc.name   : testDisplayMode002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode002, Function | MediumTest | Level1)
{
    uint32_t mode = 0;
    auto ret = g_composerDevice->GetDisplayMode(0, mode);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0300
 * @tc.name   : testDisplayMode003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode003, Function | MediumTest | Level1)
{
    uint32_t mode = 0;
    auto ret = g_composerDevice->GetDisplayMode(15, mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayMode_0400
 * @tc.name   : testDisplayMode004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testDisplayMode004, Function | MediumTest | Level1)
{
    uint32_t mode = 0;
    auto ret = g_composerDevice->GetDisplayMode(-1, mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0100
 * @tc.name   : testSetDisplayMode001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode001, Function | MediumTest | Level1)
{
    const uint32_t mode = 0;
    auto ret = g_composerDevice->SetDisplayMode(20, mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0200
 * @tc.name   : testSetDisplayMode002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode002, Function | MediumTest | Level1)
{
    const uint32_t mode = 0;
    auto ret = g_composerDevice->SetDisplayMode(0, mode);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0300
 * @tc.name   : testSetDisplayMode003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode003, Function | MediumTest | Level1)
{
    const uint32_t mode = 0;
    auto ret = g_composerDevice->SetDisplayMode(15, mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0400
 * @tc.name   : testSetDisplayMode004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode004, Function | MediumTest | Level1)
{
    const uint32_t mode = 0;
    auto ret = g_composerDevice->SetDisplayMode(-1, mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0500
 * @tc.name   : testSetDisplayMode005
 * @tc.desc   : test ModeId Max
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode005, Function | MediumTest | Level1)
{
    const uint32_t mode = 20;
    auto ret = g_composerDevice->SetDisplayMode(g_displayIds[0], mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0600
 * @tc.name   : testSetDisplayMode006
 * @tc.desc   : test devId Boundary
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode006, Function | MediumTest | Level1)
{
    const uint32_t mode = 8;
    auto ret = g_composerDevice->SetDisplayMode(g_displayIds[0], mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0700
 * @tc.name   : testSetDisplayMode007
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode007, Function | MediumTest | Level1)
{
    const uint32_t mode = 15;
    auto ret = g_composerDevice->SetDisplayMode(g_displayIds[0], mode);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayMode_0800
 * @tc.name   : testSetDisplayMode008
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayMode008, Function | MediumTest | Level1)
{
    const uint32_t mode = -1;
    auto ret = g_composerDevice->SetDisplayMode(g_displayIds[0], mode);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayPowerStatus_0100
 * @tc.name   : testDisplayPowerStatus001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testDisplayPowerStatus001, Function | MediumTest | Level2)
{
    DispPowerStatus powerStatus = DispPowerStatus::POWER_STATUS_OFF;
    auto ret = g_composerDevice->GetDisplayPowerStatus(20, powerStatus);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayPowerStatus_0200
 * @tc.name   : testDisplayPowerStatus002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testDisplayPowerStatus002, Function | MediumTest | Level1)
{
    DispPowerStatus powerStatus = DispPowerStatus::POWER_STATUS_OFF;
    auto ret = g_composerDevice->GetDisplayPowerStatus(0, powerStatus);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayPowerStatus_0300
 * @tc.name   : testDisplayPowerStatus003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testDisplayPowerStatus003, Function | MediumTest | Level2)
{
    DispPowerStatus powerStatus = DispPowerStatus::POWER_STATUS_OFF;
    auto ret = g_composerDevice->GetDisplayPowerStatus(15, powerStatus);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_DisplayPowerStatus_0400
 * @tc.name   : testDisplayPowerStatus004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testDisplayPowerStatus004, Function | MediumTest | Level2)
{
    DispPowerStatus powerStatus = DispPowerStatus::POWER_STATUS_OFF;
    auto ret = g_composerDevice->GetDisplayPowerStatus(-1, powerStatus);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0100
 * @tc.name   : testSetDisplayPowerStatus001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus001, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(20, DispPowerStatus::POWER_STATUS_ON);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0200
 * @tc.name   : testSetDisplayPowerStatus002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus002, Function | MediumTest | Level1)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(0, DispPowerStatus::POWER_STATUS_ON);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0300
 * @tc.name   : testSetDisplayPowerStatus003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus003, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(15, DispPowerStatus::POWER_STATUS_ON);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0400
 * @tc.name   : testSetDisplayPowerStatus004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus004, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(-1, DispPowerStatus::POWER_STATUS_ON);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0500
 * @tc.name   : testSetDisplayPowerStatus005
 * @tc.desc   : test devId Max when power status is POWER_STATUS_STANDBY
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus005, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(20, DispPowerStatus::POWER_STATUS_STANDBY);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0600
 * @tc.name   : testSetDisplayPowerStatus006
 * @tc.desc   : test devId Min when power status is POWER_STATUS_STANDBY
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus006, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(0, DispPowerStatus::POWER_STATUS_STANDBY);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0700
 * @tc.name   : testSetDisplayPowerStatus007
 * @tc.desc   : test devId Random when power status is POWER_STATUS_STANDBY
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus007, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(15, DispPowerStatus::POWER_STATUS_STANDBY);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0800
 * @tc.name   : testSetDisplayPowerStatus008
 * @tc.desc   : test devId Negative Number when power status is POWER_STATUS_STANDBY
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus008, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(-1, DispPowerStatus::POWER_STATUS_STANDBY);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_0900
 * @tc.name   : testSetDisplayPowerStatus009
 * @tc.desc   : test power status is POWER_STATUS_SUSPEND
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus009, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0], DispPowerStatus::POWER_STATUS_STANDBY);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_1000
 * @tc.name   : testSetDisplayPowerStatus010
 * @tc.desc   : test power status is POWER_STATUS_OFF
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus010, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0], DispPowerStatus::POWER_STATUS_OFF);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayPowerStatus_1100
 * @tc.name   : testSetDisplayPowerStatus011
 * @tc.desc   : test power status is POWER_STATUS_BUTT
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayPowerStatus011, Function | MediumTest | Level2)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0], DispPowerStatus::POWER_STATUS_BUTT);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayBacklight_0100
 * @tc.name   : GetDisplayBacklight001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, GetDisplayBacklight001, Function | MediumTest | Level2)
{
    uint32_t level;
    auto ret = g_composerDevice->GetDisplayBacklight(20, level);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayBacklight_0200
 * @tc.name   : GetDisplayBacklight002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, GetDisplayBacklight002, Function | MediumTest | Level1)
{
    uint32_t level;
    auto ret = g_composerDevice->GetDisplayBacklight(0, level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayBacklight_0300
 * @tc.name   : GetDisplayBacklight003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, GetDisplayBacklight003, Function | MediumTest | Level2)
{
    uint32_t level;
    auto ret = g_composerDevice->GetDisplayBacklight(15, level);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayBacklight_0400
 * @tc.name   : GetDisplayBacklight004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, GetDisplayBacklight004, Function | MediumTest | Level2)
{
    uint32_t level;
    auto ret = g_composerDevice->GetDisplayBacklight(-1, level);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0100
 * @tc.name   : testSetDisplayBacklight001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight001, Function | MediumTest | Level2)
{
    const uint32_t level = 10;
    auto ret = g_composerDevice->SetDisplayBacklight(20, level);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0200
 * @tc.name   : testSetDisplayBacklight002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight002, Function | MediumTest | Level1)
{
    const uint32_t level = 10;
    auto ret = g_composerDevice->SetDisplayBacklight(0, level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0300
 * @tc.name   : testSetDisplayBacklight003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight003, Function | MediumTest | Level2)
{
    const uint32_t level = 10;
    auto ret = g_composerDevice->SetDisplayBacklight(15, level);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0400
 * @tc.name   : testSetDisplayBacklight004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight004, Function | MediumTest | Level2)
{
    const uint32_t level = 10;
    auto ret = g_composerDevice->SetDisplayBacklight(-1, level);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0500
 * @tc.name   : testSetDisplayBacklight005
 * @tc.desc   : test level Max 
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight005, Function | MediumTest | Level1)
{
    const uint32_t level = 20;
    auto ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0600
 * @tc.name   : testSetDisplayBacklight006
 * @tc.desc   : test level Min 
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight006, Function | MediumTest | Level1)
{
    const uint32_t level = 0;
    auto ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0700
 * @tc.name   : testSetDisplayBacklight007
 * @tc.desc   : test level Random
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight007, Function | MediumTest | Level1)
{
    const uint32_t level = 15;
    auto ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_SetDisplayBacklight_0800
 * @tc.name   : testSetDisplayBacklight008
 * @tc.desc   : test level Negative Number
 */
HWTEST_F(DeviceTestAdditional, testSetDisplayBacklight008, Function | MediumTest | Level2)
{
    const uint32_t level = -1;
    auto ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0100
 * @tc.name   : testGetDisplayProperty001
 * @tc.desc   : test devId Max
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty001, Function | MediumTest | Level2)
{
    const uint32_t propertyId = 1;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(20, propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0200
 * @tc.name   : testGetDisplayProperty002
 * @tc.desc   : test devId Min
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty002, Function | MediumTest | Level2)
{
    const uint32_t propertyId = 1;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(0, propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0300
 * @tc.name   : testGetDisplayProperty003
 * @tc.desc   : test devId Random
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty003, Function | MediumTest | Level2)
{
    const uint32_t propertyId = 1;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(15, propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0400
 * @tc.name   : testGetDisplayProperty004
 * @tc.desc   : test devId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty004, Function | MediumTest | Level2)
{
    const uint32_t propertyId = 1;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(-1, propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0500
 * @tc.name   : testGetDisplayProperty005
 * @tc.desc   : test propertyId Max
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty005, Function | MediumTest | Level1)
{
    const uint32_t propertyId = 20;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(g_displayIds[0], propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0600
 * @tc.name   : testGetDisplayProperty006
 * @tc.desc   : test propertyId Min
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty006, Function | MediumTest | Level1)
{
    const uint32_t propertyId = 0;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(g_displayIds[0], propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0700
 * @tc.name   : testGetDisplayProperty007
 * @tc.desc   : test propertyId Random
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty007, Function | MediumTest | Level1)
{
    const uint32_t propertyId = 15;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(g_displayIds[0], propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Display_GetDisplayProperty_0800
 * @tc.name   : testGetDisplayProperty008
 * @tc.desc   : test propertyId Negative Number
 */
HWTEST_F(DeviceTestAdditional, testGetDisplayProperty008, Function | MediumTest | Level2)
{
    const uint32_t propertyId = -1;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(g_displayIds[0], propertyId, propertyValue);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}
