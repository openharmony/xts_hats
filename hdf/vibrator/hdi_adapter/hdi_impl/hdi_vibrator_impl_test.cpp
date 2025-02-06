/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include <string>
#include "hdf_base.h"
#include "osal_time.h"
#include "parameters.h"
#include "v1_3/ivibrator_interface.h"
#define HDF_LOG_TAG "hdi_vibrator_impl_test"

using namespace std;
using namespace OHOS::HDI::Vibrator;
using namespace OHOS::HDI::Vibrator::V1_3;
using namespace testing::ext;

namespace {
    HapticCapacity g_hapticCapacity;
    sptr<V1_3::IVibratorInterface> g_vibratorInterface = nullptr;
}

class HatsHdfVibratorImplTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfVibratorImplTest::SetUpTestCase()
{
    g_vibratorInterface = V1_3::IVibratorInterface::Get();
}
void HatsHdfVibratorImplTest::TearDownTestCase()
{
}
void HatsHdfVibratorImplTest::SetUp()
{
    if (g_vibratorInterface == nullptr) {
    printf("Motion is not supported ");
    GTEST_SKIP() << "Device not exist" << std::endl;
    return;
    }
}
void HatsHdfVibratorImplTest::TearDown()
{
}

/**
  * @tc.name: CheckVibratorInstanceIsEmpty
  * @tc.desc: Creat a vibrator instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_CheckVibratorInstanceIsEmpty_0010, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
}

/**
  * @tc.name: EnableCompositeEffectTest001
  * @tc.desc: Start periodic vibration with custom composite effect
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_EnableCompositeEffectTest_0010, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    if (g_hapticCapacity.isSupportPresetMapping) {
        PrimitiveEffect primitiveEffect1 { 0, 60007, 0};
        PrimitiveEffect primitiveEffect2 { 1000, 60007, 0};
        PrimitiveEffect primitiveEffect3 { 1000, 60007, 0};
        CompositeEffect effect1 = {
        .primitiveEffect = primitiveEffect1
        };
        CompositeEffect effect2 = {
        .primitiveEffect = primitiveEffect2
        };
        CompositeEffect effect3 = {
        .primitiveEffect = primitiveEffect3
        };
        std::vector<CompositeEffect> vec;
        vec.push_back(effect1);
        vec.push_back(effect2);
        vec.push_back(effect3);
        HdfCompositeEffect effect;
        effect.type = HDF_EFFECT_TYPE_PRIMITIVE;
        effect.compositeEffects = vec;
        int32_t ret = g_vibratorInterface -> EnableCompositeEffect(effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalMSleep(2000);
    }
}

/**
  * @tc.name: EnableCompositeEffectTest002
  * @tc.desc: Start periodic vibration with custom composite effect
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_EnableCompositeEffectTest_0020, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    if (g_hapticCapacity.isSupportPresetMapping) {
        PrimitiveEffect primitiveEffect1 { 0, 60007, 0};
        PrimitiveEffect primitiveEffect2 { 1000, 60007, 0};
        PrimitiveEffect primitiveEffect3 { 1000, 60007, 0};
        CompositeEffect effect1 = {
        .primitiveEffect = primitiveEffect1
        };
        CompositeEffect effect2 = {
        .primitiveEffect = primitiveEffect2
        };
        CompositeEffect effect3 = {
        .primitiveEffect = primitiveEffect3
        };
        std::vector<CompositeEffect> vec;
        vec.push_back(effect1);
        vec.push_back(effect2);
        vec.push_back(effect3);
        HdfCompositeEffect effect;
        effect.type = HDF_EFFECT_TYPE_PRIMITIVE;
        effect.compositeEffects = vec;
        int32_t ret = g_vibratorInterface -> EnableCompositeEffect(effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
        OsalMSleep(1000);
        ret = g_vibratorInterface -> StopV1_2(V1_2::HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
  * @tc.name: GetEffectInfoTest001
  * @tc.desc: Get effect information with the given effect type
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_GetEffectInfoTest_0010, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    HdfEffectInfo effectInfo;
    int32_t ret = g_vibratorInterface -> GetEffectInfo("haptic.pattern.type1", effectInfo);
        printf("isSupportEffect = [%d]\n\r", effectInfo.isSupportEffect);
        printf("duration = [%d]\n\r", effectInfo.duration);
        EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
  * @tc.name: GetEffectInfoTest002
  * @tc.desc: Get effect information with the given effect type
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_GetEffectInfoTest_0020, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    HdfEffectInfo effectInfo;
    int32_t ret = g_vibratorInterface -> GetEffectInfo("invalid.effect.id", effectInfo);
        EXPECT_EQ(HDF_SUCCESS, ret);
        EXPECT_EQ(effectInfo.duration, 0);
        EXPECT_EQ(effectInfo.isSupportEffect, false);
}

/**
  * @tc.name: IsVibratorRunningTest001
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_IsVibratorRunningTest_0010, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    if (g_hapticCapacity.isSupportPresetMapping) {
        PrimitiveEffect primitiveEffect1 { 0, 60007, 0};
        PrimitiveEffect primitiveEffect2 { 1000, 60007, 0};
        PrimitiveEffect primitiveEffect3 { 1000, 60007, 0};
        CompositeEffect effect1 = {
        .primitiveEffect = primitiveEffect1
        };
        CompositeEffect effect2 = {
        .primitiveEffect = primitiveEffect2
        };
        CompositeEffect effect3 = {
        .primitiveEffect = primitiveEffect3
        };
    std::vector<CompositeEffect> vec;
    vec.push_back(effect1);
    vec.push_back(effect2);
    vec.push_back(effect3);
    HdfCompositeEffect effect;
    effect.type = HDF_EFFECT_TYPE_PRIMITIVE;
    effect.compositeEffects = vec;
    int32_t ret = g_vibratorInterface -> EnableCompositeEffect(effect);
        EXPECT_EQ(HDF_SUCCESS, ret);
    bool state{false};
    g_vibratorInterface -> IsVibratorRunning(state);
        EXPECT_EQ(state, true);
    OsalMSleep(3000);
    g_vibratorInterface -> IsVibratorRunning(state);
        EXPECT_EQ(state, false);
    }
}

/**
  * @tc.name: IsVibratorRunningTest002
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorImplTest, SUB_Vibrator_HDI_IsVibratorRunningTest_0020, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    bool state {false};
    g_vibratorInterface -> IsVibratorRunning(state);
    EXPECT_EQ(state, false);
}
