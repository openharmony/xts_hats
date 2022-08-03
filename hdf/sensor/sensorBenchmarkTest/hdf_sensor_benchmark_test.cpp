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
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_0/isensor_interface.h"
#include "sensor_type.h"
#include "../hdiService/sensor_callback_impl.h"

using namespace OHOS::HDI::Sensor::V1_0;
using namespace testing::ext;
using namespace std;

namespace {
    sptr<ISensorInterface>  g_sensorInterface = nullptr;
    sptr<ISensorCallback> g_traditionalCallback = new SensorCallbackImpl();
    sptr<ISensorCallback> g_medicalCallback = new SensorCallbackImpl();
    std::vector<HdfSensorInformation> g_info;
    struct SensorValueRange {
        float highThreshold;
        float lowThreshold;
    };

    struct SensorDevelopmentList {
        int32_t sensorTypeId;
        char sensorName[SENSOR_NAME_MAX_LEN];
        int32_t dataForm;    // 0: fixed, 1: range
        int32_t dataDimension;
        struct SensorValueRange *valueRange;
    };

    struct SensorValueRange g_testRange[] = {{1e5, 0}};
    struct SensorValueRange g_accelRange[] = {{78, -78}, {78, -78}, {78, -78}};
    struct SensorValueRange g_alsRange[] = {{10000, 0}};
    struct SensorValueRange g_pedometerRange[] = {{10000, 0}};
    struct SensorValueRange g_proximityRange[] = {{5, 0}};
    struct SensorValueRange g_hallRange[] = {{1, 0}};
    struct SensorValueRange g_barometerRange[] = {{1100, -1100}, {1100, -1100}};
    struct SensorValueRange g_magneticRange[] = {{2000, -2000}, {2000, -2000}, {2000, -2000}};
    struct SensorValueRange g_gyroscopeRange[] = {{35, -35}, {35, -35}, {35, -35}};
    struct SensorValueRange g_gravityRange[] = {{78, -78}, {78, -78}, {78, -78}};

    struct SensorDevelopmentList g_sensorList[] = {
        {SENSOR_TYPE_NONE, "sensor_test",  1, 1, g_testRange},
        {SENSOR_TYPE_ACCELEROMETER, "accelerometer",  1, 3, g_accelRange},
        {SENSOR_TYPE_PEDOMETER, "pedometer", 1, 1, g_pedometerRange},
        {SENSOR_TYPE_PROXIMITY, "proximity",  0, 1, g_proximityRange},
        {SENSOR_TYPE_HALL, "hallrometer",  0, 1, g_hallRange},
        {SENSOR_TYPE_BAROMETER, "barometer",  1, 2, g_barometerRange},
        {SENSOR_TYPE_AMBIENT_LIGHT, "als", 1, 1, g_alsRange},
        {SENSOR_TYPE_MAGNETIC_FIELD, "magnetometer",  1, 3, g_magneticRange},
        {SENSOR_TYPE_GYROSCOPE, "gyroscope", 1, 3, g_gyroscopeRange},
        {SENSOR_TYPE_GRAVITY, "gravity", 1, 3, g_gravityRange}
    };

    constexpr int g_listNum = sizeof(g_sensorList) / sizeof(g_sensorList[0]);
    constexpr int32_t SENSOR_INTERVAL1 = 20;
    constexpr int32_t SENSOR_INTERVAL2 = 2;
    constexpr int32_t SENSOR_POLL_TIME = 1;
    constexpr int32_t SENSOR_WAIT_TIME = 10;

class sensorBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void sensorBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    g_sensorInterface = ISensorInterface::Get();
}

void sensorBenchmarkTest::TearDown(const ::benchmark::State &state)
{
}

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0010
  * @tc.desc: Benchmarktest for interface GetAllSensorInfo.
  * Obtains information about all sensors in the system.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0010)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret;
    for (auto _ : st) {
        ret = g_sensorInterface->GetAllSensorInfo(g_info);
    }
        EXPECT_EQ(SENSOR_SUCCESS, ret);

    for (auto iter : g_info) {
        for (int j =0; j < g_listNum; ++j) {
            if (iter.sensorId == g_sensorList[j].sensorTypeId) {
                EXPECT_GT(iter.sensorName.size(), 0);
                break;
            }
        }
    }
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0010)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0020
  * @tc.desc: Benchmarktest for interface register.
  * Returns 0 if the callback is successfully registered; returns a negative value otherwise.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0020)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret;
    for (auto _ : st) {
        ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_medicalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_medicalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0020)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0040
  * @tc.desc: Benchmarktest for interface Enable.
  * Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */

BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0040)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_GT(g_info.size(), 0);

    ret = g_sensorInterface->SetBatch(0, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto _ : st) {
        ret = g_sensorInterface->Enable(0);
    }
    OsalMSleep(SENSOR_POLL_TIME);
    ret = g_sensorInterface->Disable(0);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0040)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0050
  * @tc.desc: Benchmarktest for interface Disable.
  * Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */

BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0050)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_GT(g_info.size(), 0);

    ret = g_sensorInterface->SetBatch(0, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Enable(0);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    OsalMSleep(SENSOR_POLL_TIME);
    for (auto _ : st) {
        ret = g_sensorInterface->Disable(0);
    }
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0050)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0060
  * @tc.desc: Benchmarktest for interface SetBatch.
  * Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0060)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

        for (auto _ : st) {
            ret = g_sensorInterface->SetBatch(0, SENSOR_INTERVAL2, SENSOR_POLL_TIME);
        }
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Enable(0);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(0);
        EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0060)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0070
  * @tc.desc: Benchmarktest for interface SetMode.
  * Sets the data reporting mode for the specified sensor.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0070)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    int32_t ret ;
    ret = g_sensorInterface->SetBatch(0, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto _ : st) {
        if (0 == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode(0, SENSOR_MODE_ON_CHANGE);
        } else {
                ret = g_sensorInterface->SetMode(0, SENSOR_MODE_REALTIME);
        }
    }
        ret = g_sensorInterface->Enable(0);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(0);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0070)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SUB_DriverSystem_SensorBenchmark_0080
  * @tc.desc: Benchmarktest for interface SetOption.
  * Sets options for the specified sensor, including its measurement range and accuracy.
  * @tc.type: FUNC
  */
BENCHMARK_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0080)(benchmark::State &st)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    int32_t ret;
 
    for (auto _ : st) {
        ret = g_sensorInterface->SetOption(0, 0);
    }
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(sensorBenchmarkTest, SUB_DriverSystem_SensorBenchmark_0080)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
}

BENCHMARK_MAIN();