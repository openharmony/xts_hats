/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>
#include <osal_mem.h>
#include "hdf_base.h"
#include "hdf_sbuf.h"
#include "wifi_hal.h"
#include "wifi_hal_ap_feature.h"
#include "wifi_hal_base_feature.h"
#include "wifi_hal_sta_feature.h"
#include "securec.h"

using namespace testing::ext;

namespace HalTest {
struct IWiFi *g_wifi = nullptr;
class WifiHalTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void WifiHalTest::SetUpTestCase()
{
    int ret;

    ret = WifiConstruct(&g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret);
}

void WifiHalTest::TearDownTestCase()
{
    int ret;

    ret = WifiDestruct(&g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret);
}

void WifiHalTest::SetUp() {}

void WifiHalTest::TearDown() {}

/**
 * @tc.name: WifiHalStartAndStop001
 * @tc.desc: Wifi hal start and stop g_wifi test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_StartAndStop_0001, Function | MediumTest | Level1)
{
    int ret1, ret2;
    ret1 = g_wifi->start(nullptr);
    ASSERT_EQ(HDF_ERR_INVALID_PARAM, ret1);
    ret1 = g_wifi->start(g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret1);
    ret2 = g_wifi->stop(nullptr);
    ASSERT_EQ(HDF_ERR_INVALID_PARAM, ret2);
    ret2 = g_wifi->stop(g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret2);
}

/**
 * @tc.name: WifiHalStart001
 * @tc.desc: Wifi hal start g_wifi test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_Start_0001, Function | MediumTest | Level1)
{
    int ret;
    ret = g_wifi->start(nullptr);
    ASSERT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalStop001
 * @tc.desc: Wifi hal stop g_wifi test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_Stop_0001, Function | MediumTest | Level1)
{
    int ret;
    ret = g_wifi->stop(nullptr);
    ASSERT_NE(HDF_SUCCESS, ret);
}
};