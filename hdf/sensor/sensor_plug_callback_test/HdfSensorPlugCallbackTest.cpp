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
#include "sensor_type.h"
#include "sensor_plug_callback_impl.h"
using namespace OHOS::HDI::Sensor::V3_0;
using namespace testing::ext;

namespace {
    sptr<ISensorInterface>  g_sensorInterface = nullptr;
    sptr<SensorPlugCallbackImpl> g_sensorPlugCallback = new SensorPlugCallbackImpl();
}

class HatsHdfSensorPlugCallbackTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfSensorPlugCallbackTest::SetUpTestCase()
{
    g_sensorInterface = ISensorInterface::Get();
}

void HatsHdfSensorPlugCallbackTest::TearDownTestCase()
{
}

void HatsHdfSensorPlugCallbackTest::SetUp()
{
    if (g_sensorInterface == nullptr) {
        printf("Sensor list is empty");
        GTEST_SKIP() << "Device not exist" << std::endl;
        return;
    }
}

void HatsHdfSensorPlugCallbackTest::TearDown()
{
}

/**
  * @tc.name: SUB_Driver_Sensor_HdiSensor_0100
  * @tc.number : SUB_Driver_Sensor_HdiSensor_0100
  * @tc.desc: Get a client and check whether the client is empty.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfSensorPlugCallbackTest, SUB_Driver_Sensor_HdiSensor_0100, TestSize.Level1)
{
    ASSERT_NE(nullptr, g_sensorInterface);
}

/**
 * @tc.name: testHdiSensorRegSensorPlugCallBack
 * @tc.number: testHdiSensorRegSensorPlugCallBack
 * @tc.desc: Read event data for the specified sensor.
 * @tc.type: FUNC
 */
HWTEST_F(HatsHdfSensorPlugCallbackTest, testHdiSensorRegSensorPlugCallBack, TestSize.Level1)
{
    if (g_sensorInterface == nullptr) {
        ASSERT_NE(nullptr, g_sensorInterface);
        return;
    }
    int32_t ret = g_sensorInterface->RegSensorPlugCallBack(g_sensorPlugCallback);
    EXPECT_EQ(0, ret);
    printf("RegSensorPlugCallBack\n");

    OsalMSleep(3000);

    ret = g_sensorInterface->UnRegSensorPlugCallBack(g_sensorPlugCallback);
    EXPECT_EQ(0, ret);
    printf("RegSensorPlugCallBack\n");
}