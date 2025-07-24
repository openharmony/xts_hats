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
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v3_0/isensor_interface.h"
#include "sensor_type.h"
#include "sensor_callback_impl.h"
using namespace OHOS::HDI::Sensor::V3_0;
using namespace testing::ext;

namespace {
    sptr<ISensorInterface>  g_sensorInterface = nullptr;
    sptr<ISensorCallback> g_traditionalCallback = new SensorCallbackImpl();
    sptr<ISensorCallback> g_medicalCallback = new SensorCallbackImpl();
    std::vector<HdfSensorInformation> g_info;
    std::vector<HdfSensorEvents> g_events;
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
    struct SensorValueRange g_alsRange[] = {{10000000, 0}};
    struct SensorValueRange g_pedometerRange[] = {{10000, 0}};
    struct SensorValueRange g_proximityRange[] = {{5, 0}};
    struct SensorValueRange g_hallRange[] = {{4294967295, 0}};
    struct SensorValueRange g_barometerRange[] = {{1100, -1100}, {1100, -1100}};
    struct SensorValueRange g_magneticRange[] = {{2000, -2000}, {2000, -2000}, {2000, -2000}};
    struct SensorValueRange g_gyroscopeRange[] = {{35, -35}, {35, -35}, {35, -35}};
    struct SensorValueRange g_gravityRange[] = {{78, -78}, {78, -78}, {78, -78}};
    struct SensorValueRange g_humidityRange[] = {{100, 0}};
    struct SensorValueRange g_temperatureRange[] = {{125, -40}};

    struct SensorDevelopmentList g_sensorList[] = {
        {SENSOR_TYPE_NONE, "sensor_test",  1, 1, g_testRange},
        {SENSOR_TYPE_ACCELEROMETER, "accelerometer",  1, 3, g_accelRange},
        {SENSOR_TYPE_PEDOMETER, "pedometer", 1, 1, g_pedometerRange},
        {SENSOR_TYPE_PROXIMITY, "proximity",  0, 1, g_proximityRange},
        {SENSOR_TYPE_HALL, "hallrometer",  1, 1, g_hallRange},
        {SENSOR_TYPE_BAROMETER, "barometer",  1, 2, g_barometerRange},
        {SENSOR_TYPE_AMBIENT_LIGHT, "als", 1, 1, g_alsRange},
        {SENSOR_TYPE_MAGNETIC_FIELD, "magnetometer",  1, 3, g_magneticRange},
        {SENSOR_TYPE_GYROSCOPE, "gyroscope", 1, 3, g_gyroscopeRange},
        {SENSOR_TYPE_GRAVITY, "gravity", 1, 3, g_gravityRange},
        {SENSOR_TYPE_HUMIDITY, "humidity", 1, 1, g_humidityRange},
        {SENSOR_TYPE_TEMPERATURE, "tenperature", 1, 1, g_temperatureRange}
    };

    int32_t IsSuppprtedSensorId(DeviceSensorInfo deviceSensorInfo)
    {
        EXPECT_GT(g_info.size(), 0);
        for (auto iter : g_info) {
            if (iter.deviceSensorInfo.deviceId == deviceSensorInfo.deviceId &&
                iter.deviceSensorInfo.sensorType == deviceSensorInfo.sensorType &&
                iter.deviceSensorInfo.sensorId == deviceSensorInfo.sensorId &&
                iter.deviceSensorInfo.location == deviceSensorInfo.location) {
                return SENSOR_SUCCESS;
            }
        }
        return SENSOR_NOT_SUPPORT;
    }
   
    constexpr int g_listNum = sizeof(g_sensorList) / sizeof(g_sensorList[0]);
    constexpr int64_t SENSOR_INTERVAL1 = 200000000;
    constexpr int64_t SENSOR_INTERVAL2 = 20000000;
    constexpr int64_t SENSOR_INTERVAL3 = 40000000;
    constexpr int64_t SENSOR_INTERVAL4 = 20000000;
    constexpr int32_t SENSOR_POLL_TIME = 1;
    constexpr int32_t SENSOR_WAIT_TIME = 100;
    constexpr int32_t SENSOR_WAIT_TIME2 = 20000;
    constexpr int32_t SENSOR_WAIT_TIME3 = 2000;
    constexpr int32_t ABNORMAL_SENSORID = -1;
    constexpr int32_t RATE_LEVEL = 50;
}

class HdfSensorHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfSensorHdiTest::SetUpTestCase()
{
    OsalMSleep(SENSOR_WAIT_TIME3);
    g_sensorInterface = ISensorInterface::Get();
    if (g_sensorInterface == nullptr) {
        g_sensorInterface->GetAllSensorInfo(g_info);
    }
}

void HdfSensorHdiTest::TearDownTestCase()
{
}

void HdfSensorHdiTest::SetUp()
{
    if (g_sensorInterface == nullptr) {
        printf("Sensor list is empty");
        GTEST_SKIP() << "Device not exist" << std::endl;
        return;
    }
}

void HdfSensorHdiTest::TearDown()
{
}

/**
  * @tc.name: GetSensorClient0001
  * @tc.desc: Get a client and check whether the client is empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0100, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
}

/**
  * @tc.name: GetSensorList0001
  * @tc.desc: Obtains information about all sensors in the system.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0200, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    EXPECT_GT(g_info.size(), 0);
    printf("get sensor list num[%zu]\n\r", g_info.size());

    for (auto iter : g_info) {
        printf("get deviceId[%d]sensorType[%d]sensorId[%d]location[%d], info name[%s], power[%f]\n\r",
               iter.deviceSensorInfo.deviceId, iter.deviceSensorInfo.sensorType, iter.deviceSensorInfo.sensorId,
               iter.deviceSensorInfo.location, iter.sensorName.c_str(), iter.power);
        for (int j =0; j < g_listNum; ++j) {
            if (iter.deviceSensorInfo.sensorType == g_sensorList[j].sensorTypeId) {
                EXPECT_GT(iter.sensorName.size(), 0);
                break;
            }
        }
    }
}

/**
  * @tc.name: RegisterSensorDataCb0001
  * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0300, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
  * @tc.name: RegisterSensorDataCb0002
  * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0400, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(MEDICAL_SENSOR_TYPE, g_medicalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(MEDICAL_SENSOR_TYPE, g_medicalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
  * @tc.name: RegisterDataCb001
  * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0500, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(SENSOR_GROUP_TYPE_MAX, g_medicalCallback);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    ret = g_sensorInterface->Unregister(SENSOR_GROUP_TYPE_MAX, g_medicalCallback);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
  * @tc.name: EnableSensor0001
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0700, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_GT(g_info.size(), 0);

    for (auto iter : g_info) {
        ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_INTERVAL1,
            SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Enable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalSleep(SENSOR_POLL_TIME);
        ret = g_sensorInterface->Disable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(0, ret);
    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

/**
  * @tc.name: SetSensorBatch0001
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0800, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    for (auto iter : g_info) {
        ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1},
            SENSOR_INTERVAL2, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Enable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME3);
        ret = g_sensorInterface->Disable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }

    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

/** @tc.name: SetSensorBatch0002
    @tc.desc: Sets the sampling time and data report interval for sensors in batches.
    @tc.type: FUNC
    */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_0900, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->SetBatch({-1, ABNORMAL_SENSORID, 0, 1}, 0, 0);
    EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
}

/**
  * @tc.name: SetSensorBatch0003
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1000, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, -1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
  * @tc.name: SetSensorBatch0004
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1100, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, -1, -1);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
  * @tc.name: SetSensorBatch0005
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1200, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_INTERVAL2, -1);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
  * @tc.name: SetSensorMode0001
  * @tc.desc: Sets the data reporting mode for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1400, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_INTERVAL1,
            SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        if (iter.deviceSensorInfo.sensorType == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_ON_CHANGE);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        } else {
            ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_REALTIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
        ret = g_sensorInterface->Enable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME3);
        ret = g_sensorInterface->Disable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
  * @tc.name: SetSensorMode0001
  * @tc.desc: Sets the data reporting mode for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1500, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_INTERVAL1,
            SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        if (iter.deviceSensorInfo.sensorType == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_ONE_SHOT);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        } else {
            ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_REALTIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
        ret = g_sensorInterface->Enable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME3);
        ret = g_sensorInterface->Disable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
  * @tc.name: SetSensorMode0001
  * @tc.desc: Sets the data reporting mode for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1600, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_INTERVAL1,
            SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        if (iter.deviceSensorInfo.sensorType == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_FIFO_MODE);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        } else {
            ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_REALTIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
        ret = g_sensorInterface->Enable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME3);
        ret = g_sensorInterface->Disable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}


/**
  * @tc.name: SetSensorMode0002
  * @tc.desc: Sets the data reporting mode for the specified sensor.The current real-time polling mode is valid.
  * Other values are invalid.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1700, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->SetMode({-1, ABNORMAL_SENSORID, 0, 1}, SENSOR_MODE_REALTIME);
    EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
}

/**
  * @tc.name: SetSensorMode0003
  * @tc.desc: Sets the data reporting mode for the specified sensor.The current real-time polling mode is valid.
  * Other values are invalid.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1800, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetBatch({-1, iter.deviceSensorInfo.sensorType, 0, 1},
            SENSOR_INTERVAL1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->SetMode({-1, iter.deviceSensorInfo.sensorType, 0, 1}, SENSOR_MODE_DEFAULT);
        EXPECT_EQ(SENSOR_FAILURE, ret);
        ret = g_sensorInterface->Enable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME3);
        ret = g_sensorInterface->Disable({-1, iter.deviceSensorInfo.sensorType, 0, 1});
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
  * @tc.name: SetSensorOption0001
  * @tc.desc: Sets options for the specified sensor, including its measurement range and accuracy.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_1900, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        int32_t ret = g_sensorInterface->SetOption({-1, iter.deviceSensorInfo.sensorType, 0, 1}, 0);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
  * @tc.name: SetSensorOption0002
  * @tc.desc: Sets options for the specified sensor, including its measurement range and accuracy.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2300, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->SetOption({-1, ABNORMAL_SENSORID, 0, 1}, 0);
    EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
}

/**
  * @tc.name: EnableSensor0002
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2400, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_NONE, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_NONE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_NONE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0003
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2500, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1});
    int32_t ret = g_sensorInterface->SetBatch({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1},
        SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0004
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2600, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GYROSCOPE, 0, 1});
    int32_t ret = g_sensorInterface->SetBatch({-1, SENSOR_TYPE_GYROSCOPE, 0, 1}, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_GYROSCOPE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_GYROSCOPE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0005
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2700, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_PHOTOPLETHYSMOGRAPH, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_PHOTOPLETHYSMOGRAPH, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_PHOTOPLETHYSMOGRAPH, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0006
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2800, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_ELECTROCARDIOGRAPH, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_ELECTROCARDIOGRAPH, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_ELECTROCARDIOGRAPH, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0007
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_2900, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1});
    int32_t ret = g_sensorInterface->SetBatch({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1}, SENSOR_INTERVAL1,
        SENSOR_POLL_TIME);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0008
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3100, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1});
    int32_t ret = g_sensorInterface->SetBatch({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1},
        SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0009
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3200, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_CAPACITIVE, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_CAPACITIVE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_CAPACITIVE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0010
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3300, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_BAROMETER, 0, 1});
    int32_t ret = g_sensorInterface->SetBatch({-1, SENSOR_TYPE_BAROMETER, 0, 1}, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_BAROMETER, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_BAROMETER, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0011
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3400, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_TEMPERATURE, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_TEMPERATURE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_TEMPERATURE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0012
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3500, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_HALL, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_HALL, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_HALL, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0013
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3600, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GESTURE, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_GESTURE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_GESTURE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0014
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3700, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_PROXIMITY, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_PROXIMITY, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_PROXIMITY, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0015
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_3800, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_HUMIDITY, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_HUMIDITY, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_HUMIDITY, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0016
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5100, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_MEDICAL_BEGIN, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_MEDICAL_BEGIN, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_MEDICAL_BEGIN, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0017
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5200, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_MEDICAL_END, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_MEDICAL_END, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_MEDICAL_END, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0018
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5300, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_PHYSICAL_MAX, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_PHYSICAL_MAX, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_PHYSICAL_MAX, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0019
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5400, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_ORIENTATION, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_ORIENTATION, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_ORIENTATION, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0020
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5500, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GRAVITY, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_GRAVITY, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_GRAVITY, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0021
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5600, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_LINEAR_ACCELERATION, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_LINEAR_ACCELERATION, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_LINEAR_ACCELERATION, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0022
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5700, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_ROTATION_VECTOR, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_ROTATION_VECTOR, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_ROTATION_VECTOR, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0023
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5800, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_AMBIENT_TEMPERATURE, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_AMBIENT_TEMPERATURE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_AMBIENT_TEMPERATURE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0024
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_5900, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0025
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6000, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GAME_ROTATION_VECTOR, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_GAME_ROTATION_VECTOR, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_GAME_ROTATION_VECTOR, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0026
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6100, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GYROSCOPE_UNCALIBRATED, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_GYROSCOPE_UNCALIBRATED, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_GYROSCOPE_UNCALIBRATED, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0027
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6200, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_SIGNIFICANT_MOTION, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_SIGNIFICANT_MOTION, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_SIGNIFICANT_MOTION, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0028
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6300, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_PEDOMETER_DETECTION, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_PEDOMETER_DETECTION, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_PEDOMETER_DETECTION, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0029
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6400, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_PEDOMETER, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_PEDOMETER, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_PEDOMETER, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0030
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6500, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0031
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6600, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_HEART_RATE, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_HEART_RATE, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_HEART_RATE, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0032
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6700, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_DEVICE_ORIENTATION, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_DEVICE_ORIENTATION, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_DEVICE_ORIENTATION, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0033
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6800, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_WEAR_DETECTION, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_WEAR_DETECTION, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_WEAR_DETECTION, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0034
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_6900, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: EnableSensor0035
  * @tc.desc: Enables the sensor unavailable in the sensor list based on the specified sensor ID.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7000, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_MAX, 0, 1});
    int32_t ret = g_sensorInterface->Enable({-1, SENSOR_TYPE_MAX, 0, 1});
    EXPECT_EQ(status,ret);
    OsalMSleep(SENSOR_WAIT_TIME3);
    ret = g_sensorInterface->Disable({-1, SENSOR_TYPE_MAX, 0, 1});
    EXPECT_EQ(status,ret);
}

/**
  * @tc.name: GetSdcSensorInfo
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7200, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);

    EXPECT_GT(g_info.size(), 0);
    std::vector<OHOS::HDI::Sensor::V3_0::SdcSensorInfo> sdcSensorInfo;
    int32_t ret = g_sensorInterface->GetSdcSensorInfo(sdcSensorInfo);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    std::string infoMsg = "[";
    for (auto it : sdcSensorInfo) {
        if (infoMsg != "[") {
            infoMsg += ", ";
        }
        infoMsg += "{";
        infoMsg += "offset = " + std::to_string(it.offset) + ", ";
        infoMsg += "sensorId = " + std::to_string(it.deviceSensorInfo.sensorType) + ", ";
        infoMsg += "ddrSize = " + std::to_string(it.ddrSize) + ", ";
        infoMsg += "minRateLevel = " + std::to_string(it.minRateLevel) + ", ";
        infoMsg += "maxRateLevel = " + std::to_string(it.maxRateLevel) + ", ";
        infoMsg += "memAddr = " + std::to_string(it.memAddr) + ", ";
        infoMsg += "reserved = " + std::to_string(it.reserved);
        infoMsg += "}";
    }
    infoMsg += "]";
}

/**
  * @tc.name: ReportFrequencyTest0001
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7300, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);

    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_GT(g_info.size(), 0);
    int32_t sensorId = g_info[0].deviceSensorInfo.sensorType;

    ret = g_sensorInterface->SetBatch({-1, sensorId, 0, 1}, SENSOR_INTERVAL1, SENSOR_INTERVAL1);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Enable({-1, sensorId, 0, 1});
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    OsalMSleep(SENSOR_WAIT_TIME2);

    ret = g_sensorInterface->Disable({-1, sensorId, 0, 1});
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

/**
  * @tc.name: ReportFrequencyTest0002
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7400, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);

    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_GT(g_info.size(), 0);
    int32_t sensorId = g_info[0].deviceSensorInfo.sensorType;

    ret = g_sensorInterface->SetBatch({-1, sensorId, 0, 1}, SENSOR_INTERVAL3, SENSOR_INTERVAL1);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Enable({-1, sensorId, 0, 1});
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    OsalMSleep(SENSOR_WAIT_TIME2);

    ret = g_sensorInterface->Disable({-1, sensorId, 0, 1});
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

/**
  * @tc.name: ReportFrequencyTest0003
  * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7500, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);

    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_GT(g_info.size(), 0);
    int32_t sensorId = g_info[0].deviceSensorInfo.sensorType;

    ret = g_sensorInterface->SetBatch({-1, sensorId, 0, 1}, SENSOR_INTERVAL4, SENSOR_INTERVAL1);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Enable({-1, sensorId, 0, 1});
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    OsalMSleep(SENSOR_WAIT_TIME2);

    ret = g_sensorInterface->Disable({-1, sensorId, 0, 1});
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);

    EXPECT_EQ(SensorCallbackImpl::sensorDataFlag, 1);
    SensorCallbackImpl::sensorDataFlag = 1;
}

/**
  * @tc.name: SetSdcSensor1
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7600, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_ELECTROCARDIOGRAPH, 0, 1}, true, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_ELECTROCARDIOGRAPH, 0, 1}, false, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
}

/**
  * @tc.name: SetSdcSensor2
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7700, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_ACCELEROMETER, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor3
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7800, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_NONE, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_NONE, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_NONE, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor4
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_7900, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GYROSCOPE, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_GYROSCOPE, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_GYROSCOPE, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor5
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8000, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_PHOTOPLETHYSMOGRAPH, 0, 1}, true, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_PHOTOPLETHYSMOGRAPH, 0, 1}, false, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
}

/**
  * @tc.name: SetSdcSensor6
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8100, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_AMBIENT_LIGHT, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor7
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8200, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_MAGNETIC_FIELD, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor8
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8300, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_CAPACITIVE, 0, 1}, true, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_CAPACITIVE, 0, 1}, false, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
}

/**
  * @tc.name: SetSdcSensor9
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8400, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_BAROMETER, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_BAROMETER, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_BAROMETER, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor10
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8500, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_TEMPERATURE, 0, 1}, true, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_TEMPERATURE, 0, 1}, false, RATE_LEVEL);
        EXPECT_NE(SENSOR_SUCCESS, ret);
}

/**
  * @tc.name: SetSdcSensor11
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8600, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_HALL, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_HALL, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_HALL, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}

/**
  * @tc.name: SetSdcSensor12
  * @tc.desc: Read event data for the specified sensor.
  * @tc.type: FUNC
  */
HWTEST_F(HdfSensorHdiTest, SUB_Driver_Sensor_HdiSensor_8700, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
    EXPECT_GT(g_info.size(), 0);
        int32_t status = IsSuppprtedSensorId({-1, SENSOR_TYPE_GESTURE, 0, 1});
        int32_t ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_GESTURE, 0, 1}, true, RATE_LEVEL);
        EXPECT_EQ(status, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->SetSdcSensor({-1, SENSOR_TYPE_GESTURE, 0, 1}, false, RATE_LEVEL);
        EXPECT_EQ(status, ret);
}