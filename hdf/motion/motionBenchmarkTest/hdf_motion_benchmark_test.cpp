/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include <vector>
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_1/imotion_interface.h"
#include "../hdi/motion_callback_impl.h"

#define DATA_NUM 12
#define DATA_VALUE 6

using namespace OHOS::HDI::Motion::V1_1;
using namespace testing::ext;

namespace {
    sptr<OHOS::HDI::Motion::V1_1::IMotionInterface> g_motionInterface = nullptr;
    sptr<IMotionCallback> g_motionCallback = new MotionCallbackImpl();
    sptr<IMotionCallback> g_motionCallbackUnregistered = new MotionCallbackImpl();
    std::vector<uint8_t> g_motionConfigData(DATA_NUM, DATA_VALUE);
}

class HdfMotionTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfMotionTest::SetUpTestCase()
{
    g_motionInterface = OHOS::HDI::Motion::V1_1::IMotionInterface::Get();
}

void HdfMotionTest::TearDownTestCase()
{
}

void HdfMotionTest::SetUp()
{
}

void HdfMotionTest::TearDown()
{
}

/**
  * @tc.name: SUB_Driver_Motion_BenchMark_0100
  * @tc.desc: Benchmarktest for interface Register.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0100)(benchmark::State &st)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret;
    for (auto _ : st) {
    ret =g_motionInterface->Register(g_motionCallback);
    g_motionInterface->Unregister(g_motionCallback);
    }
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.name: SUB_Driver_Motion_BenchMark_0200
  * @tc.desc: Benchmarktest for interface Unregister.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0200)(benchmark::State &st)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret;
    for (auto _ : st) {
    g_motionInterface->Register(g_motionCallback);
    ret = g_motionInterface->Unregister(g_motionCallback);
    }
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.name: SUB_Driver_Motion_BenchMark_0300
  * @tc.desc: Benchmarktest for interface EnableMotion.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0300)(benchmark::State &st)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    vector<int32_t> vec;
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_PICKUP);
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_FLIP);
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_SHAKE);
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_ROTATION);
    int32_t ret;
    ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);

    for (auto _ : st) {
    ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_MAX);
    g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_MAX);
    }
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_motionInterface->Unregister(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_Driver_Motion_BenchMark_0400
  * @tc.desc: Benchmarktest for interface DisableMotion.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0400)(benchmark::State &st)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    vector<int32_t> vec;
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_PICKUP);
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_FLIP);
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_SHAKE);
    vec.push_back(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_ROTATION);
    int32_t ret;
    ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);

    for (auto _ : st) {
    g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_MAX);
    ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_MAX);
    }
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_motionInterface->Unregister(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0400)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_Driver_Motion_BenchMark_0500
  * @tc.desc: Benchmarktest for interface SetMotionConfig.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0500)(benchmark::State &st)
{
if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    int32_t motionType = -1;
    int32_t ret;
    for (auto _ : st) {
    ret = g_motionInterface->SetMotionConfig(motionType, g_motionConfigData);
    }
    EXPECT_NE(HDF_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_Driver_Motion_BenchMark_0500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();