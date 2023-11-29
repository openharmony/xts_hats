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
#include "hdf_log.h"
#include "osal_time.h"
#include "sensor_callback_impl.h"
#include "sensor_if.h"
#include "sensor_type.h"
#include "v1_1/isensor_interface.h"
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include <unistd.h>

using namespace OHOS::HDI::Sensor::V1_1;
using namespace testing::ext;

namespace {
sptr<ISensorInterface> g_sensorInterface = nullptr;
const struct SensorInterface *g_sensorDev = nullptr;
sptr<ISensorCallback> g_traditionalCallback = new SensorCallbackImpl();
sptr<ISensorCallback> g_medicalCallback = new SensorCallbackImpl();
std::vector<HdfSensorInformation> g_info;
constexpr int32_t SENSOR_INTERVAL2 = 20000000;
constexpr int32_t SENSOR_POLL_TIME = 1;
struct SensorInformation *g_sensorInfo = nullptr;
int32_t g_count = 0;

class HatsHdfSensorServiceTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfSensorServiceTestAdditional::SetUpTestCase()
{
    g_sensorInterface = ISensorInterface::Get();
    g_sensorDev = NewSensorInterfaceInstance();
    if (g_sensorDev == nullptr) {
        printf("test sensorHdi get Module instance failed\n\r");
        return;
    }
    int32_t ret = g_sensorDev->GetAllSensors(&g_sensorInfo, &g_count);
    if (ret == -1) {
        printf("get sensor information failed\n\r");
    }
}

void HatsHdfSensorServiceTestAdditional::TearDownTestCase()
{
    if (g_sensorDev != nullptr) {
        FreeSensorInterfaceInstance();
        g_sensorDev = nullptr;
    }
}

void HatsHdfSensorServiceTestAdditional::SetUp() {}

void HatsHdfSensorServiceTestAdditional::TearDown() {}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7100
 * @tc.name: testHdiSensorSetBatch001
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->SetBatch(HDF_SENSOR_TYPE_ACCELEROMETER, SENSOR_INTERVAL2, SENSOR_POLL_TIME);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7200
 * @tc.name: testHdiSensorSetBatch002
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch002, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto iter : g_info) {
        int i = 0;
        ret = g_sensorInterface->SetBatch(iter.sensorId, i, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7300
 * @tc.name: testHdiSensorSetBatch003
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch003, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }

    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (int a = 0; a < 50; a++) {
        for (auto iter : g_info) {
            int i = 0;
            ret = g_sensorInterface->SetBatch(iter.sensorId, i, SENSOR_POLL_TIME);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7400
 * @tc.name: testHdiSetBatchSensor004
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch004, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->SetBatch(-1, SENSOR_INTERVAL2, SENSOR_POLL_TIME);
    EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7500
 * @tc.name: testHdiSensorSetBatch005
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch005, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (int i = 0; i < 50; i++) {
        ret = g_sensorInterface->SetBatch(-1, SENSOR_INTERVAL2, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7600
 * @tc.name: testHdiSensorSetBatch006
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch006, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto iter : g_info) {
        int i = -1;
        ret = g_sensorInterface->SetBatch(iter.sensorId, i, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7700
 * @tc.name: testHdiSensorSetBatch007
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch007, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (int a = 0; a < 50; a++) {
        for (auto iter : g_info) {
            int i = -1;
            ret = g_sensorInterface->SetBatch(iter.sensorId, i, SENSOR_POLL_TIME);
            EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7800
 * @tc.name: testHdiSensorSetBatch008
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch008, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto iter : g_info) {
        int i = 1;
        ret = g_sensorInterface->SetBatch(iter.sensorId, i, SENSOR_POLL_TIME);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_7900
 * @tc.name: testHdiSensorSetBatch009
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch009, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto iter : g_info) {
        int i = 0;
        ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL2, i);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8000
 * @tc.name: testHdiSensorSetBatch010
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch010, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (int a = 0; a < 50; a++) {
        for (auto iter : g_info) {
            int i = 0;
            ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL2, i);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8100
 * @tc.name: testHdiSensorSetBatch011
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch011, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (auto iter : g_info) {
        int i = -1;
        ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL2, i);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8200
 * @tc.name: testHdiSensorSetBatch012
 * @tc.desc: Sets the sampling time and data report interval for sensors in batches.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetBatch012, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (int a = 0; a < 50; a++) {
        for (auto iter : g_info) {
            int i = -1;
            ret = g_sensorInterface->SetBatch(iter.sensorId, SENSOR_INTERVAL2, i);
            EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8300
 * @tc.name: testHdiSensorRegister001
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_MEDICAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(HDF_MEDICAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8400
 * @tc.name: testHdiSensorRegister002
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister002, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_MEDICAL_SENSOR_TYPE, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8500
 * @tc.name: testHdiSensorRegister003
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister003, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(HDF_MEDICAL_SENSOR_TYPE, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8600
 * @tc.name: testHdiSensorRegister004
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister004, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_SENSOR_TYPE_ACCELEROMETER, g_medicalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(HDF_SENSOR_TYPE_ACCELEROMETER, g_medicalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8700
 * @tc.name: testHdiSensorRegister005
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister005, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(HDF_MEDICAL_SENSOR_TYPE, g_traditionalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Unregister(HDF_MEDICAL_SENSOR_TYPE, g_traditionalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8800
 * @tc.name: testHdiSensorRegister006
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister006, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_MEDICAL_SENSOR_TYPE, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    ret = g_sensorInterface->Unregister(HDF_MEDICAL_SENSOR_TYPE, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_8900
 * @tc.name: testHdiSensorRegister007
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister007, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(HDF_MEDICAL_SENSOR_TYPE, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
        ret = g_sensorInterface->Unregister(HDF_MEDICAL_SENSOR_TYPE, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9000
 * @tc.name: testHdiSensorRegister008
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister008, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_TRADITIONAL_SENSOR_TYPE, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    ret = g_sensorInterface->Unregister(HDF_TRADITIONAL_SENSOR_TYPE, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9100
 * @tc.name: testHdiSensorRegister009
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister009, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(HDF_TRADITIONAL_SENSOR_TYPE, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
        ret = g_sensorInterface->Unregister(HDF_TRADITIONAL_SENSOR_TYPE, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9200
 * @tc.name: testHdiSensorRegister010
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister010, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_SENSOR_GROUP_TYPE_MAX, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    ret = g_sensorInterface->Unregister(HDF_SENSOR_GROUP_TYPE_MAX, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9300
 * @tc.name: testHdiSensorRegister011
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorRegister011, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(HDF_SENSOR_GROUP_TYPE_MAX, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
        ret = g_sensorInterface->Unregister(HDF_SENSOR_GROUP_TYPE_MAX, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9400
 * @tc.name: testHdiSensorUnregister001
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister001, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_SENSOR_TYPE_MAGNETIC_FIELD, g_traditionalCallback);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    ret = g_sensorInterface->Unregister(-1, g_traditionalCallback);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9500
 * @tc.name: testHdiSensorUnregister002
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister002, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(HDF_SENSOR_TYPE_MAGNETIC_FIELD, g_traditionalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Unregister(-1, g_traditionalCallback);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9600
 * @tc.name: testHdiSensorUnregister003
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister003, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(HDF_SENSOR_TYPE_CAPACITIVE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    for (int i = 0; i < 50; i++) {
        ret = g_sensorInterface->Unregister(-1, g_traditionalCallback);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9700
 * @tc.name: testHdiSensorUnregister004
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister004, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(SENSOR_GROUP_TYPE_MAX, g_medicalCallback);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    ret = g_sensorInterface->Unregister(SENSOR_GROUP_TYPE_MAX, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9800
 * @tc.name: testHdiSensorUnregister005
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister005, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(SENSOR_GROUP_TYPE_MAX, g_medicalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Unregister(SENSOR_GROUP_TYPE_MAX, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_9900
 * @tc.name: testHdiSensorUnregister006
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister006, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(SENSOR_GROUP_TYPE_MAX, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(SENSOR_GROUP_TYPE_MAX, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0010
 * @tc.name: testHdiSensorUnregister007
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister007, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->Register(SENSOR_GROUP_TYPE_MAX, g_traditionalCallback);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Unregister(SENSOR_GROUP_TYPE_MAX, nullptr);
        EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0020
 * @tc.name: testHdiSensorUnregister008
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister008, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_medicalCallback);
    EXPECT_EQ(SENSOR_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0030
 * @tc.name: testHdiSensorUnregister009
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister009, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
        ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, g_medicalCallback);
        EXPECT_EQ(SENSOR_FAILURE, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0040
 * @tc.name: testHdiSensorUnregister010
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister010, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(TRADITIONAL_SENSOR_TYPE, nullptr);
    EXPECT_EQ(SENSOR_INVALID_PARAM, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0050
 * @tc.name: testHdiSensorUnregister011
 * @tc.desc: Returns 0 if the callback is successfully registered; returns a negative value otherwise.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorUnregister011, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(MEDICAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Unregister(MEDICAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0060
 * @tc.name: testHdiSensorEnable001
 * @tc.desc: Enables the sensor available in the sensor list based on the specified sensor ID.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorEnable001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Enable(HDF_SENSOR_TYPE_ACCELEROMETER);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0070
 * @tc.name: testHdiSensorEnable002
 * @tc.desc: Enables the sensor available in the sensor list based on the specified sensor ID.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorEnable002, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Enable(-1);
    EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0080
 * @tc.name: testHdiSensorEnable003
 * @tc.desc: Enables the sensor available in the sensor list based on the specified sensor ID.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorEnable003, TestSize.Level2)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    for (int i = 0; i < 50; i++) {
        ret = g_sensorInterface->Enable(-1);
        EXPECT_EQ(SENSOR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0090
 * @tc.name: testHdiSensorDisable001
 * @tc.desc: Enables the sensor available in the sensor list based on the specified sensor ID.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorDisable001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->Disable(HDF_SENSOR_TYPE_ACCELEROMETER);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0110
 * @tc.name: testHdiSensorSetMode001
 * @tc.desc: Sets the data reporting mode for the specified sensor.The current real-time polling mode is valid.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetMode001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->Register(TRADITIONAL_SENSOR_TYPE, g_traditionalCallback);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
    ret = g_sensorInterface->SetMode(HDF_SENSOR_TYPE_ACCELEROMETER, SENSOR_MODE_ONE_SHOT);
    EXPECT_EQ(SENSOR_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0120
 * @tc.name: testHdiSensorGetAllSensorInfo001
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorGetAllSensorInfo001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    for (int i = 0; i < 50; i++) {
        int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0130
 * @tc.name: testHdiSensorSetOption001
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetOption001, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        ret = g_sensorInterface->SetOption(iter.sensorId, 100);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0140
 * @tc.name: testHdiSensorSetOption002
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetOption002, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_GT(g_info.size(), 0);
    for (int i = 0; i < 50; i++) {
        for (auto iter : g_info) {
            ret = g_sensorInterface->SetOption(iter.sensorId, 100);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0150
 * @tc.name: testHdiSensorSetOption003
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetOption003, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        ret = g_sensorInterface->SetOption(iter.sensorId, 1000);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0160
 * @tc.name: testHdiSensorSetOption004
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetOption004, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_GT(g_info.size(), 0);
    for (int i = 0; i < 50; i++) {
        for (auto iter : g_info) {
            ret = g_sensorInterface->SetOption(iter.sensorId, 1000);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0170
 * @tc.name: testHdiSensorSetOption005
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetOption005, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_GT(g_info.size(), 0);
    for (auto iter : g_info) {
        ret = g_sensorInterface->SetOption(iter.sensorId, 10000);
        EXPECT_EQ(SENSOR_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0180
 * @tc.name: testHdiSensorSetOption006
 * @tc.desc: Obtains information about all sensors in the system.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorSetOption006, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->GetAllSensorInfo(g_info);
    EXPECT_GT(g_info.size(), 0);
    for (int i = 0; i < 50; i++) {
        for (auto iter : g_info) {
            ret = g_sensorInterface->SetOption(iter.sensorId, 10000);
            EXPECT_EQ(SENSOR_SUCCESS, ret);
        }
    }
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0190
 * @tc.name: testHdiSensorReadData001
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData001, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_ACCELEROMETER, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0210
 * @tc.name: testHdiSensorReadData002
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData002, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_GYROSCOPE, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0220
 * @tc.name: testHdiSensorReadData003
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData003, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_PHOTOPLETHYSMOGRAPH, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0230
 * @tc.name: testHdiSensorReadData004
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData004, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_ELECTROCARDIOGRAPH, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0240
 * @tc.name: testHdiSensorReadData005
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData005, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_MAGNETIC_FIELD, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0250
 * @tc.name: testHdiSensorReadData006
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData006, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_CAPACITIVE, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0260
 * @tc.name: testHdiSensorReadData007
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData007, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_BAROMETER, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0270
 * @tc.name: testHdiSensorReadData008
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData008, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_TEMPERATURE, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0280
 * @tc.name: testHdiSensorReadData009
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData009, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_HALL, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0290
 * @tc.name: testHdiSensorReadData010
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData010, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_GESTURE, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0310
 * @tc.name: testHdiSensorReadData011
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData011, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_PROXIMITY, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0320
 * @tc.name: testHdiSensorReadData012
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData012, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_HUMIDITY, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0330
 * @tc.name: testHdiSensorReadData013
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData013, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_ORIENTATION, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

/**
 * @tc.number: SUB_Driver_Sensor_HdiSensor_0340
 * @tc.name: testHdiSensorReadData014
 * @tc.desc: Read event data for the specified sensor.
 */
HWTEST_F(HatsHdfSensorServiceTestAdditional, testHdiSensorReadData014, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_sensorDev);
    int32_t ret = g_sensorDev->ReadData(HDF_SENSOR_TYPE_GRAVITY, nullptr);
    EXPECT_EQ(SENSOR_NULL_PTR, ret);
}

} // namespace