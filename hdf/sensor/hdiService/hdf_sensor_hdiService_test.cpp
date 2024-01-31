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
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v2_0/isensor_interface.h"
#include "sensor_type.h"
#include "sensor_callback_impl.h"

using namespace OHOS::HDI::Sensor::V2_0;
using namespace testing::ext;

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
    struct SensorValueRange g_hallRange[] = {{2, 0}};
    struct SensorValueRange g_barometerRange[] = {{1100, -1100}, {1100, -1100}};
    struct SensorValueRange g_magneticRange[] = {{2000, -2000}, {2000, -2000}, {2000, -2000}};
    struct SensorValueRange g_gyroscopeRange[] = {{35, -35}, {35, -35}, {35, -35}};
    struct SensorValueRange g_gravityRange[] = {{78, -78}, {78, -78}, {78, -78}};

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
        {SENSOR_TYPE_GRAVITY, "gravity", 1, 3, g_gravityRange}
    };
 
    int32_t IsSuppprtedSensorId(int32_t sensorTypeId)
    {
      EXPECT_GT(g_info.size(),0);
      for (auto iter : g_info){
          if (iter.sensorId == sensorTypeId)
          {
            return SENSOR_SUCCESS;
          }
      }
      return SENSOR_NOT_SUPPORT;
    }
   
    constexpr int g_listNum = sizeof(g_sensorList) / sizeof(g_sensorList[0]);
    constexpr int32_t SENSOR_INTERVAL1 = 200000000;
    constexpr int32_t SENSOR_INTERVAL2 = 20000000;
    constexpr int32_t SENSOR_POLL_TIME = 1;
    constexpr int32_t SENSOR_WAIT_TIME = 100;
    constexpr int32_t ABNORMAL_SENSORID = -1;
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
    g_sensorInterface = ISensorInterface::Get();
}

void HdfSensorHdiTest::TearDownTestCase()
{
}

void HdfSensorHdiTest::SetUp()
{
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
        printf("get sensoriId[%d], info name[%s], power[%f]\n\r", iter.sensorId, iter.sensorName.c_str(), iter.power);
        for (int j =0; j < g_listNum; ++j) {
            if (iter.sensorId == g_sensorList[j].sensorTypeId) {
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
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_medicalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_medicalCallback);
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
        ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Enable(iter.sensorId);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalSleep(SENSOR_POLL_TIME);
        ret = g_sensorInterface->Disable(iter.sensorId);
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
        ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL2, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Enable(iter.sensorId);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(iter.sensorId);
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
    int32_t ret = g_sensorInterface->SetBatch(ABNORMAL_SENSORID, 0, 0);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, -1, SENSOR_POLL_TIME);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, -1, -1);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL2, -1);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        if (iter.sensorId == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_ON_CHANGE);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        } else {
            ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_REALTIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
        ret = g_sensorInterface->Enable(iter.sensorId);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(iter.sensorId);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        if (iter.sensorId == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_ONE_SHOT);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        } else {
            ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_REALTIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
        ret = g_sensorInterface->Enable(iter.sensorId);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(iter.sensorId);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        if (iter.sensorId == SENSOR_TYPE_HALL) {
            ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_FIFO_MODE);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        } else {
            ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_REALTIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
        ret = g_sensorInterface->Enable(iter.sensorId);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(iter.sensorId);
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
    int32_t ret = g_sensorInterface->SetMode(ABNORMAL_SENSORID, SENSOR_MODE_REALTIME);
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
        int32_t ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL1, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->SetMode(iter.sensorId, SENSOR_MODE_DEFAULT);
        EXPECT_EQ(SENSOR_FAILURE, ret);
        ret = g_sensorInterface->Enable(iter.sensorId);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        OsalMSleep(SENSOR_WAIT_TIME);
        ret = g_sensorInterface->Disable(iter.sensorId);
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
        int32_t ret = g_sensorInterface->SetOption(iter.sensorId, 0);
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
    int32_t ret = g_sensorInterface->SetOption(ABNORMAL_SENSORID, 0);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_NONE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_NONE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_NONE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_ACCELEROMETER );
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_ACCELEROMETER );
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_ACCELEROMETER);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_GYROSCOPE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_GYROSCOPE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_GYROSCOPE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_PHOTOPLETHYSMOGRAPH);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_PHOTOPLETHYSMOGRAPH);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_PHOTOPLETHYSMOGRAPH);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_ELECTROCARDIOGRAPH);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_ELECTROCARDIOGRAPH);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_ELECTROCARDIOGRAPH);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_AMBIENT_LIGHT);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_AMBIENT_LIGHT);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_AMBIENT_LIGHT);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_MAGNETIC_FIELD);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_MAGNETIC_FIELD);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_MAGNETIC_FIELD);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_CAPACITIVE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_CAPACITIVE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_CAPACITIVE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_BAROMETER);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_BAROMETER);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_BAROMETER);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_TEMPERATURE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_TEMPERATURE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_TEMPERATURE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_HALL);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_HALL);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_HALL);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_GESTURE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_GESTURE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_GESTURE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_PROXIMITY);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_PROXIMITY);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_PROXIMITY);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_HUMIDITY);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_HUMIDITY);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_HUMIDITY);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_MEDICAL_BEGIN);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_MEDICAL_BEGIN);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_MEDICAL_BEGIN);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_MEDICAL_END);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_MEDICAL_END);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_MEDICAL_END);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_PHYSICAL_MAX);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_PHYSICAL_MAX);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_PHYSICAL_MAX);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_ORIENTATION);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_ORIENTATION);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_ORIENTATION);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_GRAVITY);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_GRAVITY);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_GRAVITY);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_LINEAR_ACCELERATION);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_LINEAR_ACCELERATION);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_LINEAR_ACCELERATION);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_ROTATION_VECTOR);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_ROTATION_VECTOR);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_ROTATION_VECTOR);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_AMBIENT_TEMPERATURE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_AMBIENT_TEMPERATURE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_AMBIENT_TEMPERATURE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_GAME_ROTATION_VECTOR);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_GAME_ROTATION_VECTOR);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_GAME_ROTATION_VECTOR);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_GYROSCOPE_UNCALIBRATED);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_GYROSCOPE_UNCALIBRATED);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_GYROSCOPE_UNCALIBRATED);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_SIGNIFICANT_MOTION);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_SIGNIFICANT_MOTION);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_SIGNIFICANT_MOTION);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_PEDOMETER_DETECTION);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_PEDOMETER_DETECTION);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_PEDOMETER_DETECTION);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_PEDOMETER);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_PEDOMETER);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_PEDOMETER);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_HEART_RATE);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_HEART_RATE);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_HEART_RATE);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_DEVICE_ORIENTATION);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_DEVICE_ORIENTATION);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_DEVICE_ORIENTATION);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_WEAR_DETECTION);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_WEAR_DETECTION);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_WEAR_DETECTION);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED);
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

    int32_t status = IsSuppprtedSensorId(SENSOR_TYPE_MAX);
    int32_t ret = g_sensorInterface->Enable(SENSOR_TYPE_MAX);
    EXPECT_EQ(status,ret);
    ret = g_sensorInterface->Disable(SENSOR_TYPE_MAX);
    EXPECT_EQ(status,ret);
}