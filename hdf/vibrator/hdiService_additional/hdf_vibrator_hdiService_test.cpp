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
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_1/ivibrator_interface.h"
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>

using namespace OHOS::HDI::Vibrator::V1_1;
using namespace testing::ext;

namespace {
uint32_t SLEEP_TIME = 200;
std::string g_timeSequence = "haptic.clock.timer";
std::string g_builtIn = "haptic.helloworld.effect";
sptr<IVibratorInterface> g_vibratorInterface = nullptr;
sptr<IVibratorInterface> g_nullvalue = nullptr;

class HdfVibratorHdiServiceTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfVibratorHdiServiceTestAdditional::SetUpTestCase() { g_vibratorInterface = IVibratorInterface::Get(); }

void HdfVibratorHdiServiceTestAdditional::TearDownTestCase() {}

void HdfVibratorHdiServiceTestAdditional::SetUp() {}

void HdfVibratorHdiServiceTestAdditional::TearDown() {}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_1500
 * @tc.name   : testStartOnce001
 * @tc.desc   : Controls this vibrator to perform a one-shot vibrator at 0 millisecond.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStartOnce001, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(0);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_1600
 * @tc.name   : testStartOnce002
 * @tc.desc   : Controls this vibrator to perform a one-shot vibrator at 30 millisecond.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStartOnce002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(30);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_1700
 * @tc.name   : testStartOnce003
 * @tc.desc   : Controls this vibrator to perform a one-shot vibrator at 80 millisecond.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStartOnce003, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(80);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_1800
 * @tc.name   : testStartOnce004
 * @tc.desc   : Controls this vibrator to perform a one-shot vibrator at 256 millisecond.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStartOnce004, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(256);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_1900
 * @tc.name   : testStartOnce005
 * @tc.desc   : Controls this vibrator to perform a one-shot vibrator at 500 millisecond.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStartOnce005, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(500);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2000
 * @tc.name   : testStartOnce006
 * @tc.desc   : Control this vibrator to execute it once in 2 milliseconds and cycle 50 times.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStartOnce006, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t i;
    int32_t startRet;
    int32_t endRet;
    for (i = 0; i < 50; i++) {
        startRet = g_vibratorInterface->StartOnce(2);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(SLEEP_TIME);

        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2100
 * @tc.name   : testStart001
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Start = g_timeSequence.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2200
 * @tc.name   : testStart002
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Start = g_builtIn.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2300
 * @tc.name   : testStart003
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Start = g_timeSequence.Stop =
 * HDF_VIBRATOR_MODE_ONCE.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart003, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2400
 * @tc.name   : testStart004
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Start = g_builtIn.Stop = HDF_VIBRATOR_MODE_ONCE.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart004, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet = g_vibratorInterface->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2500
 * @tc.name   : testStart005
 * @tc.desc   : Control this execution time series vibrator effect twice, two effects.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart005, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    startRet = g_vibratorInterface->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    OsalMSleep(SLEEP_TIME);

    endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2600
 * @tc.name   : testStart006
 * @tc.desc   : Control this execution time series vibrator effect twice, two effects.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart006, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet = g_vibratorInterface->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);

    startRet = g_vibratorInterface->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2700
 * @tc.name   : testStart007
 * @tc.desc   : Control the vibrator effect of this execution time series. And cycle 60 times
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart007, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t i;
    int32_t startRet;
    int32_t endRet;
    for (i = 0; i < 60; i++) {
        startRet = g_vibratorInterface->Start(g_builtIn);
        EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

        OsalMSleep(SLEEP_TIME);

        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2800
 * @tc.name   : testStart008
 * @tc.desc   : Control the vibrator effect of this execution time series. And cycle 30 times
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStart008, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t i;
    int32_t startRet;
    int32_t endRet;
    for (i = 0; i < 30; i++) {
        startRet = g_vibratorInterface->Start(g_timeSequence);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(SLEEP_TIME);

        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_2900
 * @tc.name   : testStop001
 * @tc.desc   : Controls this vibrator to stop the vibrator.Stop = HDF_VIBRATOR_MODE_ONCE.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3000
 * @tc.name   : testStop002
 * @tc.desc   : Controls this vibrator to stop the vibrator.Stop = HDF_VIBRATOR_MODE_PRESET.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3100
 * @tc.name   : testStop003
 * @tc.desc   : Control this vibrator to stop it twice in a row.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop003, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);
    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_FAILURE);
    endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3200
 * @tc.name   : testStop004
 * @tc.desc   : Controls this vibrator to stop the vibrator.Stop = HDF_VIBRATOR_MODE_ONCE.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop004, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3300
 * @tc.name   : testStop005
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Controls this vibrator to stop the
 * vibrator.StartOnce = 25.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop005, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(25);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3400
 * @tc.name   : testStop006
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Controls this vibrator to stop the vibrator.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop006, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3500
 * @tc.name   : testStop007
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Controls this vibrator to stop the
 * vibrator.StartOnce = 256.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop007, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce(256);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3600
 * @tc.name   : testStop008
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Controls this vibrator to stop the vibrator.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop008, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3700
 * @tc.name   : testStop009
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Controls this vibrator to stop the vibrator.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop009, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);

    OsalMSleep(SLEEP_TIME);

    endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3800
 * @tc.name   : testStop010
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.Stop = HDF_VIBRATOR_MODE_BUTT.And cycle 10 times.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop010, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OsalMSleep(SLEEP_TIME);
    int32_t i;
    int32_t endRet;
    for (i = 0; i < 10; i++) {
        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
        EXPECT_EQ(endRet, HDF_FAILURE);
    }
}

/**
 * @tc.number : SUB_Driver_Sensor_HdiVibrator_3900
 * @tc.name   : testStop011
 * @tc.desc   : Controls this Performing Time Series Vibrator Effects.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testStop011, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->Start(g_builtIn);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    OsalMSleep(SLEEP_TIME);

    int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4000
 * @tc.name   : testGetVibratorInfo001
 * @tc.desc   : Testing the GetVibratorInfo function.And cycle 10 times.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testGetVibratorInfo001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;
    int32_t i;
    int32_t startRet;
    for (i = 0; i < 10; i++) {
        startRet = g_vibratorInterface->GetVibratorInfo(info);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
    printf("intensity = %d, intensityMaxValue = %d, intensityMinValue = %d\n\t", info[0].isSupportIntensity,
           info[0].intensityMaxValue, info[0].intensityMinValue);
    printf("frequency = %d, intensityMaxValue = %d, intensityMinValue = %d\n\t", info[0].isSupportFrequency,
           info[0].frequencyMaxValue, info[0].frequencyMinValue);
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4100
 * @tc.name   : testGetVibratorInfo002
 * @tc.desc   : Testing the GetVibratorInfo function.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testGetVibratorInfo002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        EXPECT_GT(256, 0);
        EXPECT_GE(256, info[0].intensityMinValue);
        EXPECT_LE(256, info[0].intensityMaxValue);
        EXPECT_GE(256, info[0].frequencyMinValue);
        EXPECT_LE(256, info[0].frequencyMaxValue);
        int32_t i;
        for (i = 0; i < 20; i++) {
            startRet = g_vibratorInterface->EnableVibratorModulation(256, 256, 256);
            EXPECT_EQ(startRet, HDF_SUCCESS);
            OsalMSleep(SLEEP_TIME);
            startRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
            EXPECT_EQ(startRet, HDF_SUCCESS);
        }
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4200
 * @tc.name   : testEnableVibratorModulation001
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (0, 0, 0).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(0, 0, 0);
        EXPECT_EQ(startRet, 0);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4300
 * @tc.name   : testEnableVibratorModulation002
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (20, 20, 20).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(20, 20, 20);
        EXPECT_EQ(startRet, 0);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4400
 * @tc.name   : testEnableVibratorModulation003
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (-100, 20, 20).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation003, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(-100, 20, 20);
        EXPECT_EQ(startRet, -1);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4500
 * @tc.name   : testEnableVibratorModulation004
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (20, -100, 20).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation004, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(20, -100, 20);
        EXPECT_EQ(startRet, -2);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4600
 * @tc.name   : testEnableVibratorModulation005
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (20, 20, -100).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation005, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(20, 20, -100);
        EXPECT_EQ(startRet, -3);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4700
 * @tc.name   : testEnableVibratorModulation006
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (-100, -100, 20).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation006, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(-100, -100, 20);
        EXPECT_EQ(startRet, HDF_FAILURE);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4800
 * @tc.name   : testEnableVibratorModulation007
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (20, -100, -100).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation007, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(20, -100, -100);
        EXPECT_EQ(startRet, HDF_FAILURE);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_4900
 * @tc.name   : testEnableVibratorModulation008
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (-100, 20, -100).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation008, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(-100, 20, -100);
        EXPECT_EQ(startRet, HDF_FAILURE);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_5000
 * @tc.name   : testEnableVibratorModulation009
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (-100, -100, -100).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation009, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(-100, -100, -100);
        EXPECT_EQ(startRet, HDF_FAILURE);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_5100
 * @tc.name   : testEnableVibratorModulation010
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (3000, 3000, 3000).
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation010, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation(3000, 3000, 3000);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_5200
 * @tc.name   : testEnableVibratorModulation011
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (0, 0, 0).Stop is set
 * to HDF_VIBRATOR_MODE_PRESET.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation011, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        EXPECT_GT(256, 0);
        EXPECT_GE(256, info[0].intensityMinValue);
        EXPECT_LE(256, info[0].intensityMaxValue);
        EXPECT_GE(256, info[0].frequencyMinValue);
        EXPECT_LE(256, info[0].frequencyMaxValue);
        startRet = g_vibratorInterface->EnableVibratorModulation(0, 0, 0);
        EXPECT_EQ(startRet, HDF_SUCCESS);
        OsalMSleep(SLEEP_TIME);
        startRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_5300
 * @tc.name   : testEnableVibratorModulation012
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (-256, -256, -256).Stop
 * is set to HDF_VIBRATOR_MODE_ONCE.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation012, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        EXPECT_GT(256, 0);
        EXPECT_GE(256, info[0].intensityMinValue);
        EXPECT_LE(256, info[0].intensityMaxValue);
        EXPECT_GE(256, info[0].frequencyMinValue);
        EXPECT_LE(256, info[0].frequencyMaxValue);
        startRet = g_vibratorInterface->EnableVibratorModulation(-256, -256, -256);
        EXPECT_EQ(startRet, HDF_FAILURE);
        OsalMSleep(SLEEP_TIME);
        startRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
}

/**
 * @tc.name   : SUB_Driver_Sensor_HdiVibrator_5400
 * @tc.name   : testEnableVibratorModulation013
 * @tc.desc   : Testing the EnableVibratorModulation function.EnableVibratorModulation is set to (-256, -256, -256).Stop
 * is set to HDF_VIBRATOR_MODE_BUTT.
 */
HWTEST_F(HdfVibratorHdiServiceTestAdditional, testEnableVibratorModulation013, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        EXPECT_GT(256, 0);
        EXPECT_GE(256, info[0].intensityMinValue);
        EXPECT_LE(256, info[0].intensityMaxValue);
        EXPECT_GE(256, info[0].frequencyMinValue);
        EXPECT_LE(256, info[0].frequencyMaxValue);
        startRet = g_vibratorInterface->EnableVibratorModulation(-256, -256, -256);
        EXPECT_EQ(startRet, HDF_FAILURE);
        OsalMSleep(SLEEP_TIME);
        startRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_BUTT);
        EXPECT_EQ(startRet, HDF_FAILURE);
    }
}

} // namespace