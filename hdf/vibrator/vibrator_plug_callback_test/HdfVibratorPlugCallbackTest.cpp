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
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "v2_0/ivibrator_interface.h"
#include "vibrator_type.h"
#include "vibrator_plug_callback_impl.h"

using namespace OHOS::HDI::Vibrator;
using namespace OHOS::HDI::Vibrator::V2_0;
using namespace testing::ext;

namespace {
    sptr<OHOS::HDI::Vibrator::V2_0::IVibratorInterface> g_vibratorInterface = nullptr;
    sptr<VibratorPlugCallbackImpl> g_vibratorPlugCallback = new VibratorPlugCallbackImpl();
}

class HatsHdfVibratorPlugCallbackTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsHdfVibratorPlugCallbackTest::SetUpTestCase()
{
    g_vibratorInterface = OHOS::HDI::Vibrator::V2_0::IVibratorInterface::Get();
}

void HatsHdfVibratorPlugCallbackTest::TearDownTestCase()
{
}

void HatsHdfVibratorPlugCallbackTest::SetUp()
{
    if (g_vibratorInterface == nullptr) {
        printf("g_vibratorInterface is nullptr");
        GTEST_SKIP() << "Device not exist" << std::endl;
        return;
    }
}

void HatsHdfVibratorPlugCallbackTest::TearDown()
{
}

/**
  * @tc.name: SUB_Driver_Vibrator_HdiVibrator_0100
  * @tc.number : SUB_Driver_Vibrator_HdiVibrator_0100
  * @tc.desc: Creat a vibrator instance. The instance is not empty.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorPlugCallbackTest, SUB_Driver_Vibrator_HdiVibrator_0100, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_vibratorInterface);
}

/**
 * @tc.name: testHdiVibratorRegVibratorPlugCallBack
 * @tc.number : testHdiVibratorRegVibratorPlugCallBack
 * @tc.desc: Read event data for the specified vibrator.
 */
HWTEST_F(HatsHdfVibratorPlugCallbackTest, testHdiVibratorRegVibratorPlugCallBack, TestSize.Level1)
{
    if (g_vibratorInterface == nullptr) {
        ASSERT_NE(nullptr, g_vibratorInterface);
        return;
    }
    int32_t ret = g_vibratorInterface->RegVibratorPlugCallback(g_vibratorPlugCallback);
    EXPECT_EQ(0, ret);
    printf("RegVibratorPlugCallBack\n");

    OsalMSleep(3000);

    ret = g_vibratorInterface->UnRegVibratorPlugCallback(g_vibratorPlugCallback);
    EXPECT_EQ(0, ret);
    printf("RegVibratorPlugCallBack\n");
}