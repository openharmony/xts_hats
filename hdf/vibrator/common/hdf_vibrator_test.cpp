/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include "vibrator_if.h"
#include "vibrator_type.h"

using namespace testing::ext;

namespace {
    uint32_t g_duration = 1000;
    uint32_t g_noDuration = 0;
    uint32_t g_sleepTime = 2000;
    int32_t g_intensity1 = 30;
    int32_t g_intensity2 = -30;
    int32_t g_frequency1 = 200;
    int32_t g_frequency2 = -200;
    const char *g_timeSequence = "haptic.clock.timer";
    const char *g_builtIn = "haptic.default.effect";
    const char *g_arbitraryStr = "arbitraryString";
    const struct VibratorInterface *g_vibratorDev = nullptr;
    static struct VibratorInfo *g_vibratorInfo = nullptr;

class HdfVibratorTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfVibratorTest::SetUpTestCase()
{
    g_vibratorDev = NewVibratorInterfaceInstance();
}

void HdfVibratorTest::TearDownTestCase()
{
    if (g_vibratorDev != nullptr) {
        FreeVibratorInterfaceInstance();
        g_vibratorDev = nullptr;
    }
}

void HdfVibratorTest::SetUp()
{
}

void HdfVibratorTest::TearDown()
{
}

/**
  * @tc.name: CheckVibratorInstanceIsEmpty
  * @tc.desc: Creat a vibrator instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0010, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);
}

/**
  * @tc.name: PerformOneShotVibratorDuration_001
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at a given
  duration. Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0020, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->StartOnce(g_duration);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 ms for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: PerformOneShotVibratorDuration_002
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at 0
  millisecond. Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0030, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->StartOnce(g_noDuration);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_001
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
      Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0040, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 ms for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_002
  * @tc.desc: Controls this Performing built-in Vibrator Effects.
    Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0050, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 ms for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_003
  * @tc.desc: Controls this Perform a Empty vibrator effect.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0060, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(NULL);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_004
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0090, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 ms for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_FAILURE);

    endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_005
  * @tc.desc: Controls this vibrator to stop the vibrator.
      Controls this Performing Time Series Vibrator Effects.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0070, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 ms for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_006
  * @tc.desc: Controls this vibrator to stop the vibrator.
      Controls this Perform built-in Vibrator Effects.
      Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0080, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("pls wait 2 ms for vibrator testing now\n");
    OsalMSleep(g_sleepTime);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: ExecuteVibratorEffect_007
  * @tc.desc: Controls this Perform a one-shot vibrator with a arbitrary string.
    Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->Start(g_arbitraryStr);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    int32_t endRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.name: GetVibratorInfo_001
 * @tc.desc: Obtain the vibrator setting strength, frequency capability and
 * range in the system. Validity check of input parameters.
 * @tc.type: FUNC
 */

HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0110, Function | MediumTest | Level1) {
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->GetVibratorInfo(&g_vibratorInfo);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    EXPECT_NE(g_vibratorInfo, nullptr);

    printf("intensity = %d, intensityMaxValue = %d, intensityMinValue = %d\n\t",
        g_vibratorInfo->isSupportIntensity, g_vibratorInfo->intensityMaxValue, g_vibratorInfo->intensityMinValue);
    printf("frequency = %d, frequencyMaxValue = %d, frequencyMinValue = %d\n\t",
        g_vibratorInfo->isSupportFrequency, g_vibratorInfo->frequencyMaxValue, g_vibratorInfo->frequencyMinValue);
}

/**
 * @tc.name: GetVibratorInfo_002
 * @tc.desc: Obtain the vibrator setting strength, frequency capability and
 * range in the system. Validity check of input parameters.
 * @tc.type: FUNC
 */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0120, Function | MediumTest | Level1) {
    ASSERT_NE(nullptr, g_vibratorDev);

    int32_t startRet = g_vibratorDev->GetVibratorInfo(nullptr);
    EXPECT_EQ(startRet, HDF_FAILURE);
}

/**
 * @tc.name: EnableVibratorModulation_001
 * @tc.desc: Start vibrator based on the setting vibration effect.
 * @tc.type: FUNC
 */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0130, Function | MediumTest | Level1) {
    int32_t startRet;
    ASSERT_NE(nullptr, g_vibratorDev);
    EXPECT_GT(g_duration, 0);

    if ((g_vibratorInfo->isSupportIntensity == 1) || (g_vibratorInfo->isSupportFrequency == 1)) {
        EXPECT_GE(g_intensity1, g_vibratorInfo->intensityMinValue);
        EXPECT_LE(g_intensity1, g_vibratorInfo->intensityMaxValue);
        EXPECT_GE(g_frequency1, g_vibratorInfo->frequencyMinValue);
        EXPECT_LE(g_frequency1, g_vibratorInfo->frequencyMaxValue);

        startRet = g_vibratorDev->EnableVibratorModulation(g_duration, g_intensity1, g_frequency1);
        EXPECT_EQ(startRet, HDF_SUCCESS);
        OsalMSleep(g_sleepTime);
        startRet = g_vibratorDev->Stop(VIBRATOR_MODE_ONCE);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
}

/**
 * @tc.name: EnableVibratorModulation_002
 * @tc.desc: Start vibrator based on the setting vibration effect.
 * Validity check of input parameters.
 * @tc.type: FUNC
 */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0140, Function | MediumTest | Level1) {
    ASSERT_NE(nullptr, g_vibratorDev);
    int32_t startRet;

    if ((g_vibratorInfo->isSupportIntensity == 1) || (g_vibratorInfo->isSupportFrequency == 1)) {
        startRet = g_vibratorDev->EnableVibratorModulation(g_noDuration, g_intensity1, g_frequency1);
        EXPECT_EQ(startRet, VIBRATOR_NOT_PERIOD);
    }
}

/**
 * @tc.name: EnableVibratorModulation_003
 * @tc.desc: Start vibrator based on the setting vibration effect.
 * Validity check of input parameters.
 * @tc.type: FUNC
 */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0150, Function | MediumTest | Level1) {
    ASSERT_NE(nullptr, g_vibratorDev);
    int32_t startRet;

    if ((g_vibratorInfo->isSupportIntensity == 1) || (g_vibratorInfo->isSupportFrequency == 1)) {
        startRet = g_vibratorDev->EnableVibratorModulation(g_duration, g_intensity2, g_frequency1);
        EXPECT_EQ(startRet, VIBRATOR_NOT_INTENSITY);
    }
}

/**
 * @tc.name: EnableVibratorModulation_004
 * @tc.desc: Start vibrator based on the setting vibration effect.
 * Validity check of input parameters.
 * @tc.type: FUNC
 */
HWTEST_F(HdfVibratorTest, SUB_DriverSystem_VibratorHdi_0160, Function | MediumTest | Level1) {
    ASSERT_NE(nullptr, g_vibratorDev);
    int32_t startRet;

    if ((g_vibratorInfo->isSupportIntensity == 1) || (g_vibratorInfo->isSupportFrequency == 1)) {
        startRet = g_vibratorDev->EnableVibratorModulation(g_duration, g_intensity1, g_frequency2);
        EXPECT_EQ(startRet, VIBRATOR_NOT_FREQUENCY);
    }
}
}