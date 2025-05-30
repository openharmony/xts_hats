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
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_0/ilight_interface.h"
#include "light_type.h"

using namespace OHOS::HDI::Light::V1_0;
using namespace testing::ext;

namespace {
    constexpr uint32_t g_sleepTime = 3;
    constexpr int32_t MIN_VALUE = 0;
    constexpr int32_t MAX_VALUE = 255;
    std::vector<HdfLightInfo> g_info;
    sptr<ILightInterface> g_lightInterface = nullptr;
}

class HdfLightHdiServiceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfLightHdiServiceTest::SetUpTestCase()
{
    g_lightInterface = ILightInterface::Get();
}

void HdfLightHdiServiceTest::TearDownTestCase()
{
}

void HdfLightHdiServiceTest::SetUp()
{
}

void HdfLightHdiServiceTest::TearDown()
{
}

void InitConfig(HdfLightEffect &effect)
{
    effect.lightColor.colorValue.rgbColor.r = MIN_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MIN_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MIN_VALUE;
    effect.flashEffect.flashMode = HDF_LIGHT_FLASH_NONE;
}

/**
  * @tc.name: CheckLightInstanceIsEmpty
  * @tc.desc: Create a light instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_2400, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);
}

/**
  * @tc.name: GetLightInfo001
  * @tc.desc: Get light info.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_2500, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    int32_t ret = g_lightInterface->GetLightInfo(g_info);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_GT(g_info.size(), 0);
    printf("get light list num[%zu]\n\r", g_info.size());

    for (auto iter : g_info) {
        printf("lightId[%d], lightName[%s], lightNumber[%d]\n\r",\
            iter.lightId, iter.lightName.c_str(), iter.lightNumber);
    }
}

/**
  * @tc.name: TurnOnLightRed001
  * @tc.desc: Turn on the light always on red.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_2600, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    OsalSleep(g_sleepTime);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: TurnOnLightGreen001
  * @tc.desc: Turn on the light always on green.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_2700, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    OsalSleep(g_sleepTime);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
/**
  * @tc.name: TurnOnLightBlue001
  * @tc.desc: Turn on the light always on blue.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_2800, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_EQ(HDF_SUCCESS, ret);

    OsalSleep(g_sleepTime);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: TurnOnLightAbnormal001
  * @tc.desc: Abnormal flashmode.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_2900, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;
    effect.flashEffect.flashMode = -1;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: TurnOnLightAbnormal002
  * @tc.desc: Abnormal flashmode.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_3000, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;
    effect.flashEffect.flashMode = HDF_LIGHT_FLASH_BUTT;

    int32_t ret = g_lightInterface->TurnOnLight(g_info[0].lightId, effect);
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_lightInterface->TurnOffLight(g_info[0].lightId);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: TurnOnLightAbnormal003
  * @tc.desc: Abnormal lightID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_3100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(-1, effect);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
  * @tc.name: TurnOnLightAbnormal004
  * @tc.desc: Abnormal lightID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfLightHdiServiceTest, SUB_Driver_Sensor_HdiLight_3200, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_lightInterface);

    HdfLightEffect effect;
    InitConfig(effect);
    effect.lightColor.colorValue.rgbColor.r = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.g = MAX_VALUE;
    effect.lightColor.colorValue.rgbColor.b = MAX_VALUE;

    int32_t ret = g_lightInterface->TurnOnLight(LIGHT_ID_BUTT, effect);
    EXPECT_NE(HDF_SUCCESS, ret);
}