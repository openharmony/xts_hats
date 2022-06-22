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

#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_1/ivibrator_interface.h"

using namespace OHOS::HDI::Vibrator::V1_1;
using namespace testing::ext;
using namespace std;

namespace {
    uint32_t g_duration = 1;
    uint32_t g_sleepTime1 = 2;
    uint32_t g_sleepTime2 = 5;
    std::string g_timeSequence = "haptic.clock.timer";
    std::string g_builtIn = "haptic.default.effect";
    std::string g_arbitraryStr = "arbitraryString";
    sptr<IVibratorInterface> g_vibratorInterface = nullptr;
}

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
  * @tc.name: CheckVibratorInstanceIsEmpty
  * @tc.desc: Creat a vibrator instance. The instance is not empty.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, CheckVibratorInstanceIsEmpty)(benchmark::State &st)
{
    for (auto _ : st){
        ASSERT_NE(nullptr, g_vibratorInterface);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, CheckVibratorInstanceIsEmpty)->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: PerformOneShotVibratorDuration001
  * @tc.desc: Controls this vibrator to perform a one-shot vibrator at a given duration.
  * Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, PerformOneShotVibratorDuration001)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    for (auto _ : st){
        int32_t startRet = g_vibratorInterface->StartOnce(g_duration);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime1);

        int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_ONCE);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, PerformOneShotVibratorDuration001)->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: ExecuteVibratorEffect001
  * @tc.desc: Controls this Performing Time Series Vibrator Effects.
  * Controls this vibrator to stop the vibrator
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, ExecuteVibratorEffect001)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    for (auto _ : st){
        int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime2);

        int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, ExecuteVibratorEffect001)->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: ExecuteVibratorEffect002
  * @tc.desc: Controls this Performing built-in Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, ExecuteVibratorEffect002)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);

    for (auto _ : st){
        int32_t startRet = g_vibratorInterface->Start(g_builtIn);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime1);

        int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, ExecuteVibratorEffect002)->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: ExecuteVibratorEffect005
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * Controls this Performing Time Series Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, ExecuteVibratorEffect005)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    for (auto _ : st){
        int32_t startRet = g_vibratorInterface->Start(g_timeSequence);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime2);

        int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, ExecuteVibratorEffect005)->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: ExecuteVibratorEffect006
  * @tc.desc: Controls this vibrator to stop the vibrator.
  * Controls this Perform built-in Vibrator Effects.
  * Controls this vibrator to stop the vibrator.
  * @tc.type: FUNC
  */
BENCHMARK_F(vibratorBenchmarkTest, ExecuteVibratorEffect006)(benchmark::State &st)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
    for (auto _ : st){
        int32_t startRet = g_vibratorInterface->Start(g_builtIn);
        EXPECT_EQ(startRet, HDF_SUCCESS);

        OsalMSleep(g_sleepTime2);

        int32_t endRet = g_vibratorInterface->Stop(HDF_VIBRATOR_MODE_PRESET);
        EXPECT_EQ(endRet, HDF_SUCCESS);
    }
}

BENCHMARK_REGISTER_F(vibratorBenchmarkTest, ExecuteVibratorEffect006)->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_MAIN();