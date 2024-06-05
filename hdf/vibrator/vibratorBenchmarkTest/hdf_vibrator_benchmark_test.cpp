/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#include <benchmark/benchmark.h>
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_2/ivibrator_interface.h"
#include "parameters.h"

using namespace OHOS::HDI::Vibrator;
using namespace OHOS::HDI::Vibrator::V1_2;
using namespace testing::ext;
using namespace std;

namespace {
    uint32_t g_duration = 1;
    uint32_t g_sleepTime1 = 1;
    uint32_t g_sleepTime2 = 2;
    int32_t g_intensity1 = 3;
    int32_t g_frequency1 = 20;
    int32_t g_intensity3 = 60;
    V1_2::HapticPaket g_pkg = {434, 1, {{V1_2::CONTINUOUS, 0, 149, 100, 50, 0, 4,
        {{0, 0, 0}, {1, 1, 0}, {32, 1, -39}, {149, 0, -39}}}}};
    std::string g_timeSequence = "haptic.clock.timer";
    std::string g_builtIn = "haptic.default.effect";
    std::string g_arbitraryStr = "arbitraryString";
    std::string g_effect1 = "haptic.long_press.light";
    sptr<IVibratorInterface> g_vibratorInterface = nullptr;

class vibratorBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void vibratorBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    g_vibratorInterface = IVibratorInterface::Get();
}

void vibratorBenchmarkTest::TearDown(const ::benchmark::State &state)
{
}

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0010
  * @tc.desc: Benchmarktest for interface StartOnce
  * Controls this vibrator to perform a one-shot vibrator at a given duration.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0100)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet;
    int32_t endRet;
    for (auto _ : st) {
        startRet = g_vibratorInterface->StartOnce(g_duration);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime1);

        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0020
  * @tc.desc: Benchmarktest for interface Start
  * Controls this Performing Time Series Vibrator Effects.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0200)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    int32_t startRet;
    int32_t endRet;
    for (auto _ : st) {
        startRet = g_vibratorInterface->Start(g_timeSequence);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime2);

        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0030
  * @tc.desc: Benchmarktest for interface Stop
  * Controls this Performing built-in Vibrator Effects.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0300)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet;
    int32_t endRet;

    for (auto _ : st) {
        startRet = g_vibratorInterface->Start(g_builtIn);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime1);

        endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0040
  * @tc.desc: Benchmarktest for interface GetVibratorInfo.
  * Controls this Performing Time Series Vibrator Effects.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0400)(benchmark::State &state)
{
    uint32_t majorVer;
    uint32_t minorVer;
    uint32_t ret;

    ASSERT_NE(nullptr, g_vibratorInterface);

    ret = g_vibratorInterface->GetVersion(majorVer, minorVer);
    ASSERT_EQ(HDF_SUCCESS, ret);

    ASSERT_LT(0, minorVer);
    ASSERT_LT(0, majorVer);

    std::vector<HdfVibratorInfo> info;

    for (auto _ : state) {
        int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
        EXPECT_EQ(startRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0400)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0050
  * @tc.desc: Benchmarktest for interface EnableVibratorModulation.
  * Controls this Performing built-in Vibrator Effects.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0500)(benchmark::State &state)
{
    uint32_t majorVer;
    uint32_t minorVer;
    if (g_vibratorInterface->GetVersion(majorVer, minorVer) != HDF_SUCCESS) {
        return;
    }

    if (majorVer > 0 && minorVer <= 0) {
        return;
    }
    ASSERT_NE(nullptr, g_vibratorInterface);

    std::vector<HdfVibratorInfo> info;

    int32_t startRet = g_vibratorInterface->GetVibratorInfo(info);
    EXPECT_EQ(startRet, HDF_SUCCESS);

    for (auto _ : state) {
        if ((info[0].isSupportIntensity == 1) || (info[0].isSupportFrequency == 1)) {
            printf("vibratot benchmarktest successed!\n\t");
            EXPECT_GT(g_duration, 0);
            EXPECT_GE(g_intensity1, info[0].intensityMinValue);
            EXPECT_LE(g_intensity1, info[0].intensityMaxValue);
            EXPECT_GE(g_frequency1, info[0].frequencyMinValue);
            EXPECT_LE(g_frequency1, info[0].frequencyMaxValue);
            printf("vibratot benchmark test successed!\n\t");
            startRet = g_vibratorInterface->EnableVibratorModulation(g_duration, g_intensity1, g_frequency1);
            EXPECT_EQ(startRet, HDF_SUCCESS);
            OsalMSleep(g_sleepTime1);
            startRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
            EXPECT_EQ(startRet, HDF_SUCCESS);
        }
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0060
  * @tc.desc: Start periodic vibration with custom composite effect
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0600)(benchmark::State &state)
{
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
        int32_t ret;
        for (auto _ : state) {
        ret = g_vibratorInterface -> EnableCompositeEffect(effect);
        }
        OsalMSleep(2);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0600)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0070
  * @tc.desc: Get effect information with the given effect type
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0700)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    HdfEffectInfo effectInfo;
    int32_t ret;
    for (auto _ : state) {
    ret = g_vibratorInterface -> GetEffectInfo("haptic.pattern.type1", effectInfo);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0700)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0080
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0800)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    bool stat {false};
    for (auto _ : state) {
    g_vibratorInterface -> IsVibratorRunning(stat);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0800)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0090
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0900)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet;
    for (auto _ : state) {
    startRet = g_vibratorInterface->StartByIntensity(g_effect1, g_intensity3);
    }
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->StopV1_2(HdfVibratorModeV1_2::HDF_VIBRATOR_MODE_PRESET);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_0900)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0100
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1000)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet;
    for (auto _ : state) {
    startRet = g_vibratorInterface->PlayHapticPattern(g_pkg);
    }
    EXPECT_EQ(startRet, HDF_SUCCESS);

    int32_t endRet = g_vibratorInterface->StopV1_2(HdfVibratorModeV1_2::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1000)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0110
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1100)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    OHOS::HDI::Vibrator::V1_2::HapticCapacity hapticCapacity;
    int32_t startRet;
    for (auto _ : state) {
    startRet = g_vibratorInterface->GetHapticCapacity(hapticCapacity);
    }
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("hapticCapacity.isSupportHdHaptic = %d\n", hapticCapacity.isSupportHdHaptic);
    printf("hapticCapacity.isSupportPresetMapping = %d\n", hapticCapacity.isSupportPresetMapping);
    printf("hapticCapacity.isSupportTimeDelay = %d\n", hapticCapacity.isSupportTimeDelay);

    int32_t endRet = g_vibratorInterface->StopV1_2(HdfVibratorModeV1_2::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0120
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1200)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startUpTime;
    int32_t mode = 0;
    int32_t startRet;
    for (auto _ : state) {
    startRet = g_vibratorInterface->GetHapticStartUpTime(mode, startUpTime);
    }
    EXPECT_EQ(startRet, HDF_SUCCESS);
    printf("startUpTime = %d\n", startUpTime);

    int32_t endRet = g_vibratorInterface->StopV1_2(HdfVibratorModeV1_2::HDF_VIBRATOR_MODE_ONCE);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_VibratorBenchmark_0130
  * @tc.desc: Get vibration status.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1300)(benchmark::State &state)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    int32_t startRet;
    int32_t endRet;
    for (auto _ : st) {
    startRet = g_vibratorInterface->StartOnce(g_duration);

    OsalMSleep(g_sleepTime1);

    endRet = g_vibratorInterface->StopV1_2(HdfVibratorModeV1_2::HDF_VIBRATOR_MODE_ONCE);
    }
    EXPECT_EQ(startRet, HDF_SUCCESS);
    EXPECT_EQ(endRet, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, SUB_Driver_Sensor_VibaratorPerf_1300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
}
BENCHMARK_MAIN();
