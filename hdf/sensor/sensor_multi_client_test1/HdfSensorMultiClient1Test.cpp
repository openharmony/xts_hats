/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "../common/sensor_type.h"
#include "sensor_callback_impl.h"
using namespace OHOS::HDI::Sensor::V3_0;
using namespace testing::ext;

namespace {
    sptr<ISensorInterface>  g_sensorInterface = nullptr;
    sptr<ISensorCallback> g_traditionalCallback = new SensorCallbackImpl();
    std::vector<HdfSensorInformation> g_info;
}

class HatsHdfSensorMultiClientTest1 : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfSensorMultiClientTest1::SetUpTestCase()
{
    g_sensorInterface = ISensorInterface::Get();
    if (g_sensorInterface != nullptr) {
        g_sensorInterface->GetAllSensorInfo(g_info);
    }
}

void HatsHdfSensorMultiClientTest1::TearDownTestCase()
{
}

void HatsHdfSensorMultiClientTest1::SetUp()
{
    if (g_sensorInterface == nullptr) {
        printf("Sensor list is empty");
        GTEST_SKIP() << "Device not exist" << std::endl;
        return;
    }
}

void HatsHdfSensorMultiClientTest1::TearDown()
{
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiSensor_0100
  * @tc.number : SUB_Driver_Sensor_HdiSensor_0100
  * @tc.desc: Get a client and check whether the client is empty.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfSensorMultiClientTest1, SUB_Driver_Sensor_HdiSensor_0100, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiSensor_0200
  * @tc.number : SUB_Driver_Sensor_HdiSensor_0200
  * @tc.desc: Obtains information about all sensors in the system.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfSensorMultiClientTest1, SUB_Driver_Sensor_HdiSensor_0200, TestSize.Level1)
{
    if (g_info.size() > 0) {
        DeviceSensorInfo deviceSensorInfo = g_info[0].deviceSensorInfo;
        printf("test deviceSensorInfo is %s\n", SENSOR_HANDLE_TO_C_STR(deviceSensorInfo));
        int32_t ret = -1;

        ret = g_sensorInterface->Register(0, g_traditionalCallback);
        EXPECT_EQ(0, ret);

        ret = g_sensorInterface->SetBatch(deviceSensorInfo, 100000000, 0);
        EXPECT_EQ(0, ret);
        printf("SetBatch samplingInterval is 100000000\n");

        ret = g_sensorInterface->Enable(deviceSensorInfo);
        EXPECT_EQ(0, ret);

        OsalMSleep(3000);

        ret = g_sensorInterface->Disable(deviceSensorInfo);
        EXPECT_EQ(0, ret);

        ret = g_sensorInterface->Unregister(0, g_traditionalCallback);
        EXPECT_EQ(0, ret);
    }
}