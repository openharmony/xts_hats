/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "hdf_base.h"
#include "light_type.h"
#include "osal_time.h"
#include "v1_0/ilight_interface.h"
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>

using namespace OHOS::HDI::Light::V1_0;
using namespace testing::ext;

namespace {
constexpr uint32_t SLEEPTIME = 3;
constexpr int32_t MIN_VALUE = 0;
constexpr int32_t MAX_VALUE = 255;
std::vector<HdfLightInfo> g_info;
sptr<ILightInterface> g_lightInterface = nullptr;

class HatsHdfLightCommonTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfLightCommonTestAdditional::SetUpTestCase() { g_lightInterface = ILightInterface::Get(); }

void HatsHdfLightCommonTestAdditional::TearDownTestCase() {}

void HatsHdfLightCommonTestAdditional::SetUp() {}

void HatsHdfLightCommonTestAdditional::TearDown() {}

void InitConfig(HdfLightEffect &effect)
{
    effect.lightColor.colorValue.rgbColor.r = MIN_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MIN_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MIN_VALUE;
    effect.flashEffect.flashMode = HDF_LIGHT_FLASH_NONE;
    effect.flashEffect.onTime = 1;
    effect.flashEffect.offTime = 1;
}

/* *
 * @tc.number: SUB_Driver_Light_GetlightInfo_0200
 * @tc.name  : testhdiServiceGetLightInfoTimes001
 * @tc.desc  : Get light info 10times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceGetLightInfoTimes001, Function | MediumTest | Level1)
{
    int32_t ret;

    ASSERT_NE(nullptr, g_lightInterface);

    for (int i = 0; i < 10; i++) {
        ret = g_lightInterface->GetLightInfo(g_info);
        EXPECT_EQ(HDF_SUCCESS, ret);
        EXPECT_GT(g_info.size(), 0);
        printf("get light list num[%zu]\n\r", g_info.size());

        for (auto iter : g_info) {
            printf("lightId[%d], lightName[%s], lightNumber[%d]\n\r", iter.lightId, iter.lightName.c_str(),
                   iter.lightNumber);
        }
    }
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_0500
 * @tc.name  : testhdiServiceTurnOnLightMAX001
 * @tc.desc  : Enter the minimum excess value to turn on the red light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightMAX001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_0600
 * @tc.name  : testhdiServiceTurnOnLightRedMAX001
 * @tc.desc  : Enter the maximum excess value to turn on the red light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightRedMAX001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_0700
 * @tc.name  : testhdiServiceTurnOnLightRedMAXTimes001
 * @tc.desc  : Turn on the light and it's always red 10 times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightRedMAXTimes001, Function | MediumTest | Level1)
{
    int32_t ret;

    ASSERT_NE(nullptr, g_lightInterface);

    for (int i = 0; i < 10; i++) {
        HdfLightEffect effect;
        InitConfig(effect);
        effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;
        ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalSleep(SLEEPTIME);

        ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_0800
 * @tc.name  : testhdiServiceTurnOnLightGreenMAX001
 * @tc.desc  : Enter the maximum excess value to turn on the green light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightGreenMAX001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_0900
 * @tc.name  : testhdiServiceTurnOnLightGreenMAXTimes001
 * @tc.desc  : Turn on the light and it's always green 10 times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightGreenMAXTimes001, Function | MediumTest | Level1)
{
    int32_t ret;
    ASSERT_NE(nullptr, g_lightInterface);

    for (int i = 0; i < 10; i++) {
        HdfLightEffect effect;
        InitConfig(effect);
        effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;
        ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalSleep(SLEEPTIME);

        ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1000
 * @tc.name  : testhdiServiceTurnOnLightBlueMAX001
 * @tc.desc  : Enter the maximum excess value to turn on the blue light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightBlueMAX001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1100
 * @tc.name  : testhdiServiceTurnOnLightBlueMAXTimes001
 * @tc.desc  : Turn on the light and it's always blue 10 times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightBlueMAXTimes001, Function | MediumTest | Level1)
{
    int32_t ret;
    ASSERT_NE(nullptr, g_lightInterface);

    for (int i = 0; i < 10; i++) {
        HdfLightEffect effect;
        InitConfig(effect);
        effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;
        ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalSleep(SLEEPTIME);

        ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1200
 * @tc.name  : testhdiServiceTurnOnLightFM001
 * @tc.desc  : The input is Abnormal flashmode.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightFM001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.flashEffect.flashMode = 0;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1300
 * @tc.name  : testhdiServiceTurnOnLightTime001
 * @tc.desc  : Turn on the light Abnormal times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightTime001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.flashEffect.onTime = -1;
    effect.flashEffect.offTime = 10;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1400
 * @tc.name  : testhdiServiceTurnOnLightTime002
 * @tc.desc  : Turn on the light Abnormal times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightTime002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.flashEffect.onTime = 10;
    effect.flashEffect.offTime = -1;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1500
 * @tc.name  : testhdiServiceTurnOnLightTime003
 * @tc.desc  : Turn on the light times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightTime003, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.flashEffect.onTime = 256;
    effect.flashEffect.offTime = 0;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnLight_1600
 * @tc.name  : testhdiServiceTurnOnLightTime004
 * @tc.desc  : Turn on the light times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnLightTime004, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.flashEffect.onTime = 0;
    effect.flashEffect.offTime = 256;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnMultiLights_0200
 * @tc.name  : testhdiServiceTurnOnMultiLightsRedMAX001
 * @tc.desc  : Turn on the light Abnormal lightid.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnMultiLightsRedMAX001, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightColor> lightColor;
    struct HdfLightColor light;
    light.colorValue.rgbColor.r = MAX_VALUE;
    lightColor.push_back(light);

    int32_t ret = g_lightInterface->TurnOnMultiLights(HDF_LIGHT_ID_BATTERY, lightColor);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_NOTIFICATIONS);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOnMultiLights_0300
 * @tc.name  : testhdiServiceTurnOnMultiLightsTimes001
 * @tc.desc  : Turn on the light lightid 10 times.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOnMultiLightsTimes001, Function | MediumTest | Level1)
{
    int32_t ret;

    ASSERT_NE(nullptr, g_lightInterface);
    for (int i = 0; i < 10; i++) {
        std::vector<HdfLightColor> lightColor;
        struct HdfLightColor light;
        light.colorValue.rgbColor.r = MAX_VALUE;
        light.colorValue.rgbColor.g = MIN_VALUE;
        light.colorValue.rgbColor.b = MIN_VALUE;
        lightColor.push_back(light);
        ret = g_lightInterface->TurnOnMultiLights(HDF_LIGHT_ID_BATTERY, lightColor);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalSleep(SLEEPTIME);

        ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_BATTERY);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOffLight_0100
 * @tc.name  : testhdiServiceTurnOffBattery001
 * @tc.desc  : Turn Off Light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOffBattery001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightColor> lightColor;
    struct HdfLightColor light;
    light.colorValue.rgbColor.r = MAX_VALUE;
    lightColor.push_back(light);

    int32_t ret = g_lightInterface->TurnOnMultiLights(HDF_LIGHT_ID_BATTERY, lightColor);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_BATTERY);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOffLight_0200
 * @tc.name  : testhdiServiceTurnOffNotifications001
 * @tc.desc  : Turn Off Light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOffNotifications001, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightColor> lightColor;
    struct HdfLightColor light;
    light.colorValue.rgbColor.r = MAX_VALUE;
    lightColor.push_back(light);

    int32_t ret = g_lightInterface->TurnOnMultiLights(HDF_LIGHT_ID_BATTERY, lightColor);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_NOTIFICATIONS);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOffLight_0300
 * @tc.name  : testhdiServiceTurnOffAttention001
 * @tc.desc  : Turn Off Light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOffAttention001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightColor> lightColor;
    struct HdfLightColor light;
    light.colorValue.rgbColor.r = MAX_VALUE;
    lightColor.push_back(light);

    int32_t ret = g_lightInterface->TurnOnMultiLights(HDF_LIGHT_ID_BATTERY, lightColor);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_ATTENTION);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOffLight_0400
 * @tc.name  : testhdiServiceTurnOffButt001
 * @tc.desc  : Turn Off Light.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOffButt001, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightColor> lightColor;
    struct HdfLightColor light;
    light.colorValue.rgbColor.r = MAX_VALUE;
    lightColor.push_back(light);

    int32_t ret = g_lightInterface->TurnOnMultiLights(HDF_LIGHT_ID_BUTT, lightColor);
    EXPECT_EQ(-2, ret);
    OsalSleep(SLEEPTIME);

    ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_BUTT);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/* *
 * @tc.number: SUB_Driver_Light_TurnOffLight_0500
 * @tc.name  : testhdiServiceTurnOffNoOn001
 * @tc.desc  : Turn it off when it's not on.
 */
HWTEST_F(HatsHdfLightCommonTestAdditional, testhdiServiceTurnOffNoOn001, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_lightInterface);

    std::vector<HdfLightColor> lightColor;
    struct HdfLightColor light;
    light.colorValue.rgbColor.r = MAX_VALUE;
    lightColor.push_back(light);

    int32_t ret = g_lightInterface->TurnOffLight(HDF_LIGHT_ID_BUTT);
    EXPECT_EQ(HDF_FAILURE, ret);
}
} // namespace