/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#include "v2_0/ivibrator_interface.h"
#include "../common/vibrator_type.h"

using namespace OHOS::HDI::Vibrator;
using namespace OHOS::HDI::Vibrator::V2_0;
using namespace testing::ext;

namespace {
    uint32_t g_duration = 1000;
    uint32_t g_noDuration = 0;
    uint32_t g_sleepTime1 = 2000;
    uint32_t g_sleepTime2 = 5000;
    int32_t g_intensity1 = 30;
    int32_t g_intensity2 = -30;
    int32_t g_frequency1 = 200;
    int32_t g_frequency2 = -200;
    V2_0::HapticPaket g_pkg = {434, 1, {{V2_0::CONTINUOUS, 0, 149, 100, 50, 0, 4,
        {{0, 0, 0}, {1, 1, 0}, {32, 1, -39}, {149, 0, -39}}}}};
    constexpr int32_t MIN_DURATION = 0;
    constexpr int32_t MAX_DURATION = 3600000;
    std::vector<std::string> g_effect_list = {"haptic.clock.timer", "haptic.long_press.light", \
        "haptic.long_press.medium", "haptic.long_press.light", "haptic.fail", "haptic.charging", \
        "haptic.slide.light", "haptic.threshold"};
    std::string g_builtIn = "haptic.default.effect";
    std::string g_effect1 = "haptic.long_press.light";
    std::string g_arbitraryStr = "arbitraryString";
    sptr<OHOS::HDI::Vibrator::V2_0::IVibratorInterface> g_vibratorInterface = nullptr;
}

class HdfVibratorHdiServiceTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfVibratorHdiServiceTest::SetUpTestCase()
{
    g_vibratorInterface = OHOS::HDI::Vibrator::V2_0::IVibratorInterface::Get();
}

void HdfVibratorHdiServiceTest::TearDownTestCase()
{
}

void HdfVibratorHdiServiceTest::SetUp()
{
}

void HdfVibratorHdiServiceTest::TearDown()
{
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0100
  * @tc.desc: Creat a vibrator instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0200
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at a given duration.
  * Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0200, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce({-1, 1}, g_duration);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    OsalMSleep(g_sleepTime1);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0300
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at 0 millisecond.
  * Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0300, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce({-1, 1}, g_noDuration);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0400
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
  * Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0400, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t ret;
    for (auto iter : g_effect_list) {
        printf("VibratorEffect : %s\n", iter.c_str());
        ret = g_vibratorInterface->Start({-1, 1}, iter);
        EXPECT_EQ(ret, HDF_SUCCESS);

    OsalMSleep(g_sleepTime2);

        ret = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0500
  * @tc.desc: Controls this Performing built-in Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0500, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t ret;
    for (auto iter : g_effect_list) {
        printf("VibratorEffect : %s\n", iter.c_str());
        ret = g_vibratorInterface->Start({-1, 1}, iter);
        EXPECT_EQ(ret, HDF_SUCCESS);

    OsalMSleep(g_sleepTime1);

        ret = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0600
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0600, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t ret;
    for (auto iter : g_effect_list) {
        printf("VibratorEffect : %s\n", iter.c_str());
        ret = g_vibratorInterface->Start({-1, 1}, iter);
        EXPECT_EQ(ret, HDF_SUCCESS);

    OsalMSleep(g_sleepTime2);

        ret = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_BUTT);
        EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

        ret = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0700
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * Controls this Performing Time Series Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0700, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t ret;
    for (auto iter : g_effect_list) {
        printf("VibratorEffect : %s\n", iter.c_str());
        ret = g_vibratorInterface->Start({-1, 1}, iter);
        EXPECT_EQ(ret, HDF_SUCCESS);

    OsalMSleep(g_sleepTime2);

        ret = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0800
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * Controls this Perform built-in Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0800, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t ret;
    for (auto iter : g_effect_list) {
        printf("VibratorEffect : %s\n", iter.c_str());
        ret = g_vibratorInterface->Start({-1, 1}, iter);
        EXPECT_EQ(ret, HDF_SUCCESS);

    OsalMSleep(g_sleepTime2);

        ret = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_0900
  * @tc.desc: Controls this Perform a one-shot vibrator with a arbitrary string.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_0900, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->Start({-1, 1}, g_arbitraryStr);
    EXPECT_EQ(startRet, HDF_ERR_INVALID_PARAM);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1000
  * @tc.desc: Obtain the vibrator setting strength, frequency capability and range in the system.
  * Validity check of input parameters.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1000, Function | MediumTest | Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    if (g_vibratorInterface->GetVersion(majorVer, minorVer) != HDF_SUCCESS) {
        printf("get version failed!\n\t");
        return;
    }

    if (majorVer > 0 && minorVer <= 0) {
        printf("version not support!\n\t");
        return;
    }
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    printf("intensity = %d, intensityMaxValue = %d, intensityMinValue = %d\n\t",
        info[0].isSupportIntensity, info[0].intensityMaxValue, info[0].intensityMinValue);
    printf("frequency = %d, intensityMaxValue = %d, intensityMinValue = %d\n\t",
        info[0].isSupportFrequency, info[0].frequencyMaxValue, info[0].frequencyMinValue);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1100
  * @tc.desc: Start vibrator based on the setting vibration effect.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1100, Function | MediumTest | Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    if (g_vibratorInterface->GetVersion(majorVer, minorVer) != HDF_SUCCESS) {
        printf("get version failed!\n\t");
        return;
    }

    if (majorVer > 0 && minorVer <= 0) {
        printf("version not support!\n\t");
        return;
    }
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        EXPECT_GT(g_duration, 0);
        EXPECT_GE(g_intensity1, info[0].intensityMinValue);
        EXPECT_LE(g_intensity1, info[0].intensityMaxValue);
        EXPECT_GE(g_frequency1, info[0].frequencyMinValue);
        EXPECT_LE(g_frequency1, info[0].frequencyMaxValue);

        startRet = g_vibratorInterface->EnableVibratorModulation({-1, 1}, g_duration, g_intensity1, g_frequency1);
        EXPECT_EQ(startRet, HDF_SUCCESS);
        OsalMSleep(g_sleepTime1);
        startRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1200
  * @tc.desc: Start vibrator based on the setting vibration effect.
  * Validity check of input parameters.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1200, Function | MediumTest | Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    if (g_vibratorInterface->GetVersion(majorVer, minorVer) != HDF_SUCCESS) {
        printf("get version failed!\n\t");
        return;
    }

    if (majorVer > 0 && minorVer <= 0) {
        printf("version not support!\n\t");
        return;
    }
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation({-1, 1}, g_noDuration, g_intensity1, g_frequency1);
        EXPECT_EQ(startRet, VIBRATOR_NOT_PERIOD);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1300
  * @tc.desc: Start vibrator based on the setting vibration effect.
  * Validity check of input parameters.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1300, Function | MediumTest | Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    if (g_vibratorInterface->GetVersion(majorVer, minorVer) != HDF_SUCCESS) {
        printf("get version failed!\n\t");
        return;
    }

    if (majorVer > 0 && minorVer <= 0) {
        printf("version not support!\n\t");
        return;
    }
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation({-1, 1}, g_duration, g_intensity2, g_frequency1);
        EXPECT_EQ(startRet, VIBRATOR_NOT_INTENSITY);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1400
  * @tc.desc: Start vibrator based on the setting vibration effect.
  * Validity check of input parameters.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1400, Function | MediumTest | Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    if (g_vibratorInterface->GetVersion(majorVer, minorVer) != HDF_SUCCESS) {
        printf("get version failed!\n\t");
        return;
    }

    if (majorVer > 0 && minorVer <= 0) {
        printf("version not support!\n\t");
        return;
    }
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
        startRet = g_vibratorInterface->EnableVibratorModulation({-1, 1}, g_duration, g_intensity1, g_frequency2);
        EXPECT_EQ(startRet, VIBRATOR_NOT_FREQUENCY);
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1500
  * @tc.desc: Get effect information with the given effect type.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1500, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    HdfEffectInfo effectInfo;
    int32_t ret;
    for (auto iter : g_effect_list) {
        printf("VibratorEffect : %s\n", iter.c_str());
        ret = g_vibratorInterface->GetEffectInfo({-1, 1}, iter, effectInfo);
        printf("isSupportEffect = [%d]\n\r", effectInfo.isSupportEffect);
        printf("duration = [%d]\n\r", effectInfo.duration);
        EXPECT_EQ(ret, HDF_SUCCESS);
        if (effectInfo.isSupportEffect) {
            EXPECT_GT(effectInfo.duration, MIN_DURATION);
            EXPECT_LE(effectInfo.duration, MAX_DURATION);
        }
    }
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1600
  * @tc.desc: Get effect information with the given effect type.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1600, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    HdfEffectInfo effectInfo;
    int32_t ret = g_vibratorInterface->GetEffectInfo({-1, 1}, "invaild effect id", effectInfo);
    printf("isSupportEffect = [%d]\n\r", effectInfo.isSupportEffect);
    printf("duration = [%d]\n\r", effectInfo.duration);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(effectInfo.isSupportEffect, false);
    EXPECT_EQ(effectInfo.duration, 0);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1700
  * @tc.desc: HD vibration data packet delivery.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1700, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->PlayHapticPattern({-1, 1}, g_pkg);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1800
  * @tc.desc: Obtains the vibration capability of the motor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1800, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    OHOS::HDI::Vibrator::V2_0::HapticCapacity hapticCapacity;
    int32_t startRet = g_vibratorInterface->GetHapticCapacity({-1, 1}, hapticCapacity);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("hapticCapacity.isSupportHdHaptic = %d\n", hapticCapacity.isSupportHdHaptic);
    printf("hapticCapacity.isSupportPresetMapping = %d\n", hapticCapacity.isSupportPresetMapping);
    printf("hapticCapacity.isSupportTimeDelay = %d\n", hapticCapacity.isSupportTimeDelay);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_1900
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_1900, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime;
    int32_t mode = 0;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2000
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2000, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->StartOnce({-1, 1}, g_duration);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2100
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->Start({-1, 1}, g_effect1);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2200
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2200, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->PlayHapticPattern({-1, 1}, g_pkg);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_HDHAPTIC);
    EXPECT_EQ(endRet, HDF_ERR_NOT_SUPPORT);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2300
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2300, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet = g_vibratorInterface->Start({-1, 1}, g_effect1);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_BUTT);
    EXPECT_EQ(endRet, HDF_ERR_INVALID_PARAM);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2700
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2700, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime;
    int32_t mode = 1;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2800
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2800, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime;
    int32_t mode = 2;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_2900
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_2900, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime;
    int32_t mode = 3;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_3000
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_3000, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime;
    int32_t mode = -1;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_3100
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_3100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime = 0;
    int32_t mode = 0;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiVibrator_3200
  * @tc.desc: Indicates the time from command is issued to the time the motor starts.
  * @tc.type: FUNC
  */
HWTEST_F(HdfVibratorHdiServiceTest, SUB_Driver_Sensor_HdiVibrator_3200, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    
    int32_t startUpTime = -1;
    int32_t mode = 0;
    int32_t startRet = g_vibratorInterface->GetHapticStartUpTime({-1, 1}, mode, startUpTime);
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->Stop({-1, 1}, HdfVibratorMode::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

