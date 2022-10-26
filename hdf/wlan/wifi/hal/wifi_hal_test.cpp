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
const int32_t WLAN_TX_POWER = 160;
const uint32_t IFNAME_MIN_NUM = 0;
const uint32_t IFNAME_MAX_NUM = 32;
const uint32_t MAX_IF_NAME_LENGTH = 16;
const uint32_t TEST_PARAM_BUF_SIZE = 64;
const uint32_t SIZE = 4;
const int32_t TEST_CMD = 123;
const uint32_t TEST_BUF_SIZE = 64;
const uint32_t DEFAULT_COMBO_SIZE = 10;
const uint32_t RESET_TIME = 20;

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

void WifiHalTest::SetUp()
{
    int ret;

    ret = g_wifi->start(nullptr);
    ASSERT_EQ(HDF_ERR_INVALID_PARAM, ret);
    ret = g_wifi->start(g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret);
}

void WifiHalTest::TearDown()
{
    int ret;

    ret = g_wifi->stop(nullptr);
    ASSERT_EQ(HDF_ERR_INVALID_PARAM, ret);
    ret = g_wifi->stop(g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret);
}

static void ParseScanResult(WifiScanResult *scanResult)
{
    printf("ParseScanResult: flags=%d, caps=%d, freq=%d, beaconInt=%d,\n",
        scanResult->flags, scanResult->caps, scanResult->freq, scanResult->beaconInt);
    printf("ParseScanResult: qual=%d, beaconIeLen=%d, level=%d, age=%d, ieLen=%d,\n",
        scanResult->qual, scanResult->beaconIeLen, scanResult->level, scanResult->age, scanResult->ieLen);
}

static int32_t HalCallbackEvent(uint32_t event, void *respData, const char *ifName)
{
    (void)event;
    if (respData == nullptr) {
        return HDF_FAILURE;
    }
    printf("HalCallbackEvent ifName = %s, event = %d\n", ifName, event);
    switch (event) {
        case WIFI_EVENT_SCAN_DONE:
            printf("HalCallbackEvent WIFI_EVENT_SCAN_DONE Process\n");
            break;
        case WIFI_EVENT_SCAN_RESULT:
            ParseScanResult((WifiScanResult *)respData);
            break;
        default:
            break;
    }
    return HDF_SUCCESS;
}

/**
 * @tc.name: WifiHalCreateAndDestroyFeature001
 * @tc.desc: Wifi hal create and destroy feature function test
 * @tc.type: FUNC
 */

HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateAndDestroyFeature_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);

    ret = g_wifi->destroyFeature(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalCreateAndDestroyFeature002
 * @tc.desc: Wifi hal create and destroy feature function test
 * @tc.type: FUNC
 */

HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateAndDestroyFeature_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalCreateAndDestroyFeature003
 * @tc.desc: Wifi hal create and destroy feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateAndDestroyFeature_0003, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);

    ret = g_wifi->destroyFeature(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalCreateAndDestroyFeature004
 * @tc.desc: Wifi hal create and destroy feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateAndDestroyFeature_0004, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalCreateFeature001
 * @tc.desc: Wifi hal create feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateFeature_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, nullptr);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.name: WifiHalCreateFeature002
 * @tc.desc: Wifi hal create feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateFeature_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, nullptr);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.name: WifiHalCreateFeature003
 * @tc.desc: Wifi hal create feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateFeature_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_NUM, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalCreateFeature004
 * @tc.desc: Wifi hal create feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_CreateFeature_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(-1, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName001
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureByIfName_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct IWiFiAp *apFeatureGet = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getFeatureByIfName(nullptr, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName002
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureByIfName_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct IWiFiAp *apFeatureGet = nullptr;
    const char *ifName0 = "wlanTest";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getFeatureByIfName(ifName0, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName003
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureByIfName_0003, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct IWiFiAp *apFeatureGet = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getFeatureByIfName(apFeature->baseFeature.ifName, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeatureGet);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName004
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureByIfName_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = g_wifi->getFeatureByIfName(nullptr, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName005
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureByIfName_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName0 = "wlanTest";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = g_wifi->getFeatureByIfName(ifName0, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
 
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName006
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureByIfName_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    struct IWiFiSta *staFeatureGet = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = g_wifi->getFeatureByIfName(staFeature->baseFeature.ifName, (struct IWiFiBaseFeature **)&staFeatureGet);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeatureGet, nullptr);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.name: WifiHalRegisterEventCallback001
 * @tc.desc: Wifi hal register callback function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_RegisterEventCallback_0001, Function | MediumTest | Level1)
{
    int ret;

    ret = g_wifi->registerEventCallback(HalCallbackEvent, "wlan0");
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalRegisterEventCallback002
 * @tc.desc: Wifi hal register event callback test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_RegisterEventCallback_0002, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_wifi->registerEventCallback(nullptr, "wlan0");
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

}

/**
 * @tc.name: WifiHalRegisterEventCallback003
 * @tc.desc: Wifi hal register event callback test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_RegisterEventCallback_0003, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_wifi->registerEventCallback(HalCallbackEvent, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: WifiHalUnRegisterEventCallback001
 * @tc.desc: Wifi hal unregister callback function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_UnRegisterEventCallback_0001, Function | MediumTest | Level1)
{
    int ret;

    ret = g_wifi->unregisterEventCallback(HalCallbackEvent, "wlan0");
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalUnRegisterEventCallback002
 * @tc.desc: Wifi hal unregister event callback test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_UnRegisterEventCallback_0002, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_wifi->unregisterEventCallback(nullptr, "wlan0");
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

}

/**
 * @tc.name: WifiHalUnRegisterEventCallback003
 * @tc.desc: Wifi hal unregister event callback test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_UnRegisterEventCallback_0003, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_wifi->unregisterEventCallback(HalCallbackEvent, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: WifiHalGetNetworkIfaceName001
 * @tc.desc: Wifi hal get network iface name function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetNetworkIfaceName_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    const char *ifnameTest = apFeature->baseFeature.getNetworkIfaceName(nullptr);
    EXPECT_EQ(ifnameTest, nullptr);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetNetworkIfaceName002
 * @tc.desc: Wifi hal get network iface name function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetNetworkIfaceName_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    const char *ifName = apFeature->baseFeature.getNetworkIfaceName((const struct IWiFiBaseFeature *)apFeature);
    EXPECT_NE(nullptr, ifName);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetNetworkIfaceName003
 * @tc.desc: Wifi hal get network iface name function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetNetworkIfaceName_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    const char *ifnameTest = staFeature->baseFeature.getNetworkIfaceName(nullptr);
    EXPECT_EQ(ifnameTest, nullptr);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetNetworkIfaceName004
 * @tc.desc: Wifi hal get network iface name function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetNetworkIfaceName_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    const char *ifName = staFeature->baseFeature.getNetworkIfaceName((const struct IWiFiBaseFeature *)staFeature);
    EXPECT_NE(ifName, nullptr);
    if (strncmp(ifName, "wlan", 4) == 0 || strncmp(ifName, "nan", 3) == 0 || strncmp(ifName, "p2p", 3) == 0) {
        ret = 0;
    } else {
        ret = -1;
    }
    EXPECT_EQ(ret, 0);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetFeatureType001
 * @tc.desc: Wifi hal get feature type function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureType_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t type;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    type = apFeature->baseFeature.getFeatureType(nullptr);
    EXPECT_EQ(type, HDF_FAILURE);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureType002
 * @tc.desc: Wifi hal get feature type function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureType_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t type;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    type = apFeature->baseFeature.getFeatureType((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(PROTOCOL_80211_IFTYPE_AP, type);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureType003
 * @tc.desc: Wifi hal get feature type function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureType_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t type;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    type = staFeature->baseFeature.getFeatureType(nullptr);
    EXPECT_EQ(type, HDF_FAILURE);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetFeatureType004
 * @tc.desc: Wifi hal get feature type function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetFeatureType_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t type;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    type = staFeature->baseFeature.getFeatureType((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(type, PROTOCOL_80211_IFTYPE_STATION);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetMacAddress001
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_SetMacAddress_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress(nullptr, mac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
 
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress002
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_SetMacAddress_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, nullptr, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress003
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_SetMacAddress_0003, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, mac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress004
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_SetMacAddress_0004, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char errorMac[ETH_ADDR_LEN] = {0x11, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, errorMac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress005
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_SetMacAddress_0005, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, mac, ETH_ADDR_LEN);
    printf("%s: ret = %d\n", __func__, ret);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_DEVICE_BUSY);
    ASSERT_TRUE(flag);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress006
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetMacAddress_0006, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress(nullptr, mac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress007
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetMacAddress_0007, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, nullptr, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress008
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetMacAddress_0008, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, mac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress009
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetMacAddress_0009, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char errorMac[ETH_ADDR_LEN] = {0x11, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, errorMac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress010
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetMacAddress_0010, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, mac, ETH_ADDR_LEN);
    printf("%s: ret = %d\n", __func__, ret);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_DEVICE_BUSY);
    ASSERT_TRUE(flag);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetTxPower001
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetTxPower_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setTxPower(nullptr, WLAN_TX_POWER);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetTxPower002
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetTxPower_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)apFeature, 0);
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetTxPower003
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetTxPower_0003, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)apFeature, WLAN_TX_POWER);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetTxPower004
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetTxPower_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.setTxPower(nullptr, WLAN_TX_POWER);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetTxPower005
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetTxPower_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)staFeature, -1);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetTxPower006
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetTxPower_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)staFeature, WLAN_TX_POWER);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetCountryCode001
 * @tc.desc: Wifi hal set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetCountryCode_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(apFeature, nullptr, 0);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetCountryCode002
 * @tc.desc: Wifi hal set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetCountryCode_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(nullptr, "CN", 2);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetCountryCode003
 * @tc.desc: Wifi hal set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetCountryCode_0003, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(apFeature, "CN", 3);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetCountryCode004
 * @tc.desc: Wifi hal set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetCountryCode_0004, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(apFeature, "99", 2);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetCountryCode005
 * @tc.desc: Wifi hal set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetCountryCode_0005, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(apFeature, "CN", 2);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetIfNamesByChipId001
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetIfNamesByChipId_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    char *ifNames = nullptr;
    unsigned int num = 0;
    unsigned char chipId = 0;
    uint8_t i;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.getIfNamesByChipId(chipId, nullptr, nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    ASSERT_TRUE(num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}


/**
 * @tc.name: WifiHalGetIfNamesByChipId002
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetIfNamesByChipId_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    char *ifNames = nullptr;
    unsigned int num = 0;
    unsigned char chipId = 0;
    uint8_t i;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.getIfNamesByChipId(100, &ifNames, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
    ASSERT_TRUE(num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetIfNamesByChipId003
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetIfNamesByChipId_0003, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    char *ifNames = nullptr;
    unsigned int num = 0;
    unsigned char chipId = 0;
    uint8_t i;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.getIfNamesByChipId(chipId, &ifNames, &num);
    EXPECT_NE(nullptr, ifNames);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ASSERT_TRUE(num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetIfNamesByChipId004
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetIfNamesByChipId_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    char *ifNames = nullptr;
    unsigned int num = 0;
    unsigned char chipId = 0;
    uint8_t i;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.getIfNamesByChipId(chipId, nullptr, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
 
    bool flag = (num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    ASSERT_TRUE(flag);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetIfNamesByChipId005
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetIfNamesByChipId_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    char *ifNames = nullptr;
    unsigned int num = 0;
    unsigned char chipId = 0;
    uint8_t i;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.getIfNamesByChipId(100, &ifNames, &num);
    EXPECT_NE(ret, HDF_SUCCESS);

    bool flag = (num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    ASSERT_TRUE(flag);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetIfNamesByChipId006
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetIfNamesByChipId_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    char *ifNames = nullptr;
    unsigned int num = 0;
    unsigned char chipId = 0;
    uint8_t i;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = apFeature->baseFeature.getIfNamesByChipId(chipId, &ifNames, &num);
    EXPECT_NE(ifNames, nullptr);
    EXPECT_EQ(ret, HDF_SUCCESS);
    bool flag = (num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    ASSERT_TRUE(flag);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetSupportFeature001
 * @tc.desc: Get supported features
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetSupportFeature_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    ret = g_wifi->getSupportFeature(nullptr, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: WifiHalGetSupportFeature002
 * @tc.desc: Get supported features
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetSupportFeature_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t supType[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};
    ret = g_wifi->getSupportFeature(supType, PROTOCOL_80211_IFTYPE_NUM);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: WifiHalGetSupportFeature003
 * @tc.desc: Get supported features
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetSupportFeature_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t supType[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};

    ret = g_wifi->getSupportFeature(supType, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(ret, RET_CODE_SUCCESS);
}

/**
 * @tc.name: HalGetChipId001
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetChipId_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId(nullptr, &chipId);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: HalGetChipId002
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetChipId_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: HalGetChipId003
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetChipId_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: HalGetChipId004
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetChipId_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getChipId(nullptr, &chipId);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: HalGetChipId005
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetChipId_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: HalGetChipId006
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetChipId_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress001
 * @tc.desc: Get device Mac address for ap mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = apFeature->baseFeature.getDeviceMacAddress(nullptr, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress002
 * @tc.desc: Get device Mac address for ap mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);

    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, nullptr, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress003
 * @tc.desc: Get device Mac address for ap mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, readMac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress004
 * @tc.desc: Get device Mac address for ap mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress005
 * @tc.desc: Get device Mac address for sta mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = staFeature->baseFeature.getDeviceMacAddress(nullptr, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress006
 * @tc.desc: Get device Mac address for sta mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, nullptr, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress007
 * @tc.desc: Get device Mac address for sta mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0007, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, readMac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress008
 * @tc.desc: Get device Mac address for sta mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetDeviceMacAddress_0008, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHaGetValidFreqsWithBand001
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getValidFreqsWithBand(nullptr, band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHaGetValidFreqsWithBand002
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getValidFreqsWithBand(nullptr, band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        band, nullptr, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHaGetValidFreqsWithBand003
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature, band, freqs, 10, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHaGetValidFreqsWithBand004
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        band, freqs, size, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHaGetValidFreqsWithBand005
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t bandNotSupport = IEEE80211_NUM_BANDS;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        bandNotSupport, freqs, size, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
 
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHaGetValidFreqsWithBand006
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand007
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0007, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand(nullptr, band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand008
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0008, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature, band, nullptr, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand009
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0009, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature, band, freqs, 10, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand010
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0010, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature,
                                                       band, freqs, size, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand011
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0011, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t bandNotSupport = IEEE80211_NUM_BANDS;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature,
                                                       bandNotSupport, freqs, size, &num);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand012
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetValidFreqsWithBand_0012, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature, band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetAssociatedStas001
 * @tc.desc: Get asscociated STA info
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetAssociatedStas_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[MAX_ASSOC_STA_NUM];
    (void)memset_s(staInfo, sizeof(StaInfo) * MAX_ASSOC_STA_NUM, 0, sizeof(StaInfo) * MAX_ASSOC_STA_NUM);
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->getAssociatedStas(nullptr, staInfo, MAX_ASSOC_STA_NUM, &num);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetAssociatedStas002
 * @tc.desc: Get asscociated STA info
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetAssociatedStas_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[MAX_ASSOC_STA_NUM];
    (void)memset_s(staInfo, sizeof(StaInfo) * MAX_ASSOC_STA_NUM, 0, sizeof(StaInfo) * MAX_ASSOC_STA_NUM);
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->getAssociatedStas(apFeature, nullptr, MAX_ASSOC_STA_NUM, &num);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetAssociatedStas003
 * @tc.desc: Get asscociated STA info
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetAssociatedStas_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[MAX_ASSOC_STA_NUM];
    (void)memset_s(staInfo, sizeof(StaInfo) * MAX_ASSOC_STA_NUM, 0, sizeof(StaInfo) * MAX_ASSOC_STA_NUM);
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->getAssociatedStas(apFeature, staInfo, 0, &num);
    EXPECT_NE(ret, HDF_SUCCESS);


    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetAssociatedStas004
 * @tc.desc: Get asscociated STA info
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetAssociatedStas_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[MAX_ASSOC_STA_NUM];
    (void)memset_s(staInfo, sizeof(StaInfo) * MAX_ASSOC_STA_NUM, 0, sizeof(StaInfo) * MAX_ASSOC_STA_NUM);

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->getAssociatedStas(apFeature, staInfo, MAX_ASSOC_STA_NUM, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);


    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetAssociatedStas005
 * @tc.desc: Get asscociated STA info
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetAssociatedStas_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[MAX_ASSOC_STA_NUM];
    (void)memset_s(staInfo, sizeof(StaInfo) * MAX_ASSOC_STA_NUM, 0, sizeof(StaInfo) * MAX_ASSOC_STA_NUM);
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->getAssociatedStas(apFeature, staInfo, MAX_ASSOC_STA_NUM, &num);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetScanningMacAddress001
 * @tc.desc: Set Mac address scanning
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetScanningMacAddress_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char scanMac[WIFI_MAC_ADDR_LENGTH] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->setScanningMacAddress(nullptr, scanMac, WIFI_MAC_ADDR_LENGTH);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetScanningMacAddress002
 * @tc.desc: Set Mac address scanning
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetScanningMacAddress_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->setScanningMacAddress(staFeature, nullptr, WIFI_MAC_ADDR_LENGTH);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetScanningMacAddress003
 * @tc.desc: Set Mac address scanning
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetScanningMacAddress_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char scanMac[WIFI_MAC_ADDR_LENGTH] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->setScanningMacAddress(staFeature, scanMac, 0);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetScanningMacAddress004
 * @tc.desc: Set Mac address scanning
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetScanningMacAddress_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char scanMac[WIFI_MAC_ADDR_LENGTH] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->setScanningMacAddress(staFeature, scanMac, WIFI_MAC_ADDR_LENGTH);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetNetDevInfo001
 * @tc.desc: Wifi hdi get netdev info function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetNetDevInfo_0001, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getNetDevInfo(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetNetDevInfo002
 * @tc.desc: Wifi hdi get netdev info function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetNetDevInfo_0002, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct NetDeviceInfoResult netDeviceInfoResult;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getNetDevInfo(&netDeviceInfoResult);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalStartScan001
 * @tc.desc: start scan is successfully
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_StartScan_0001, Function | MediumTest | Level3)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";
    WifiScan scan = {0};

    ret = g_wifi->registerEventCallback(HalCallbackEvent, ifName);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->startScan(nullptr, &scan);
    EXPECT_NE(ret, HDF_SUCCESS);
    sleep(10);
}

/**
 * @tc.name: WifiHalStartScan002
 * @tc.desc: start scan is successfully
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_StartScan_0002, Function | MediumTest | Level3)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->registerEventCallback(HalCallbackEvent, ifName);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->startScan(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    sleep(10);
}

/**
 * @tc.name: WifiHalStartScan003
 * @tc.desc: start scan is successfully
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_WLAN_HDI_StartScan_0003, Function | MediumTest | Level3)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";
    WifiScan scan = {0};

    ret = g_wifi->registerEventCallback(HalCallbackEvent, ifName);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->startScan(ifName, &scan);
    EXPECT_EQ(HDF_SUCCESS, ret);
    sleep(10);
}

/**
 * @tc.name: GetPowerModeTest_001
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("GetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(nullptr, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_002
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("GetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_003
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("GetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(ifName, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_004
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("GetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(apFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_005
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("GetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(apFeature->baseFeature.ifName, &mode);
    printf("%s: ret = %d\n", __func__, ret);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_006
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("GetPowerMode002: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(nullptr, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_007
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0007, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("GetPowerMode002: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_008
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0008, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("GetPowerMode002: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(ifName, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_009
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0009, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("GetPowerMode002: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(staFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_010
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetPowerMode_0010, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("GetPowerMode002: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(staFeature->baseFeature.ifName, &mode);
    printf("%s: ret = %d\n", __func__, ret);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_001
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("SetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_NUM);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_NUM);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(apFeature->baseFeature.ifName, WIFI_POWER_MODE_NUM);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_002
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("SetPowerMode002: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_SLEEPING);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_SLEEPING);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(apFeature->baseFeature.ifName, WIFI_POWER_MODE_SLEEPING);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_003
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("SetPowerMode003: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_GENERAL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_GENERAL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(apFeature->baseFeature.ifName, WIFI_POWER_MODE_GENERAL);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_004
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("SetPowerMode004: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_THROUGH_WALL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_THROUGH_WALL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(apFeature->baseFeature.ifName, WIFI_POWER_MODE_THROUGH_WALL);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_005
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("SetPowerMode005: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_NUM);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_NUM);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(staFeature->baseFeature.ifName, WIFI_POWER_MODE_NUM);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_006
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("SetPowerMode005: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_SLEEPING);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_SLEEPING);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(staFeature->baseFeature.ifName, WIFI_POWER_MODE_SLEEPING);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_007
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0007, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("SetPowerMode005: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_GENERAL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_GENERAL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(staFeature->baseFeature.ifName, WIFI_POWER_MODE_GENERAL);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SetPowerModeTest_008
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetPowerMode_0008, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("SetPowerMode005: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_THROUGH_WALL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_THROUGH_WALL);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(staFeature->baseFeature.ifName, WIFI_POWER_MODE_THROUGH_WALL);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetSupportCombo001
 * @tc.desc: Get supported combo
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetSupportCombo_0001, Function | MediumTest | Level1)
{
    int ret;
    uint8_t support[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};
  
    ret = g_wifi->getSupportFeature(support, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->getSupportCombo(nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetSupportCombo002
 * @tc.desc: Get supported combo
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetSupportCombo_0002, Function | MediumTest | Level1)
{
    int ret;
    uint8_t support[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};
    uint64_t combo[DEFAULT_COMBO_SIZE] = {0};
  
    ret = g_wifi->getSupportFeature(support, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->getSupportCombo(combo, DEFAULT_COMBO_SIZE);
    if (support[PROTOCOL_80211_IFTYPE_NUM] == 0) {
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
} else {
    EXPECT_EQ(ret, HDF_SUCCESS);
}
}

/**
 * @tc.name: SetProjectionScreenParam001
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(nullptr, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam002
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(apFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam003
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(nullptr, param);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam004
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(apFeature->baseFeature.ifName, param);
    EXPECT_NE(ret, HDF_SUCCESS);
  
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam005
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    bool flag;
    struct IWiFiAp *apFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (int i = CMD_CLOSE_GO_CAC; i <= CMD_ID_CTRL_ROAM_CHANNEL; i++) {
        param->cmdId = i;
        ret = g_wifi->setProjectionScreenParam(apFeature->baseFeature.ifName, param);
        printf("SetProjectionScreenParam001_%d: ret = %d\n", __LINE__, ret);
        flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
        ASSERT_TRUE(flag);
    }
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam006
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(nullptr, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam007
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0007, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(staFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam008
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0008, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(nullptr, param);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam009
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0009, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(staFeature->baseFeature.ifName, param);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SetProjectionScreenParam010
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SetProjectionScreenParam_0010, Function | MediumTest | Level1)
{
    int32_t ret;
    bool flag;
    struct IWiFiSta *staFeature = nullptr;
    ProjScrnCmdParam *param;

    param = (ProjScrnCmdParam *)OsalMemCalloc(sizeof(ProjScrnCmdParam) + TEST_PARAM_BUF_SIZE);
    EXPECT_NE(nullptr, param);
    param->cmdId = TEST_CMD;
    param->bufLen = 1;
    param->buf[0] = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (int i = CMD_CLOSE_GO_CAC; i <= CMD_ID_CTRL_ROAM_CHANNEL; i++) {
        param->cmdId = i;
        ret = g_wifi->setProjectionScreenParam(staFeature->baseFeature.ifName, param);
        printf("SetProjectionScreenParam001_%d: ret = %d\n", __LINE__, ret);
        flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
        ASSERT_TRUE(flag);
    }
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
}

/**
 * @tc.name: SendCmdIoctl001
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0001, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(nullptr, cmdId, nullptr, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl002
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0002, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(ifName, cmdId, nullptr, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl003
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0003, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int8_t data[TEST_BUF_SIZE] = {0};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(nullptr, cmdId, data, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl004
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0004, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    bool flag;
    struct IWiFiAp *apFeature = nullptr;
    int8_t data[TEST_BUF_SIZE] = {0};
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (cmdId = CMD_HID2D_MODULE_INIT; cmdId <= CMD_SET_CHAN_ADJUST; cmdId++) {
        ret = g_wifi->sendCmdIoctl(ifName, cmdId, data, TEST_BUF_SIZE);
        printf("sendCmdIoctl_%d: ret = %d\n", __LINE__, ret);
        flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
        ASSERT_TRUE(flag);
    }
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl005
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0005, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(nullptr, cmdId, nullptr, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl006
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0006, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(ifName, cmdId, nullptr, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl007
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0007, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int8_t data[TEST_BUF_SIZE] = {0};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(nullptr, cmdId, data, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: SendCmdIoctl008
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_SendCmdIoctl_0008, Function | MediumTest | Level1)
{
    int32_t cmdId = 0;
    int32_t ret;
    bool flag;
    struct IWiFiSta *staFeature = nullptr;
    int8_t data[TEST_BUF_SIZE] = {0};
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (cmdId = CMD_HID2D_MODULE_INIT; cmdId <= CMD_SET_CHAN_ADJUST; cmdId++) {
        ret = g_wifi->sendCmdIoctl(ifName, cmdId, data, TEST_BUF_SIZE);
        printf("sendCmdIoctl_%d: ret = %d\n", __LINE__, ret);
        flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
        ASSERT_TRUE(flag);
    }
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo001
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0001, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, nullptr, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo002
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0002, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, nullptr, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo003
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0003, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, &info, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo004
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0004, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, nullptr, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo005
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0005, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, &info, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo006
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0006, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, &info, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo007
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0007, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, nullptr, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo008
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0008, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    bool flag;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, &info, mac, ETH_ADDR_LEN);
    flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo009
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0009, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, nullptr, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo010
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0010, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, nullptr, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo011
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0011, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, &info, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo012
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0012, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiSta *staFeature = nullptr;
 
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, nullptr, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo013
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0013, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, &info, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo014
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0014, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiSta *staFeature = nullptr;
 
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, &info, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo015
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0015, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, nullptr, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetStationInfo016
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_GetStationInfo_0016, Function | MediumTest | Level1)
{
    int32_t ret;
    StationInfo info;
    bool flag;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, &info, mac, ETH_ADDR_LEN);
    flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number: WifiHalResetDriver001
 * @tc.name: Reset the WiFi driver
 * @tc.size:Medium
 * @tc.level: Level 3
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_ResetDriver_0001, Function | MediumTest | Level3)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    uint8_t chipId = 0;
    uint8_t chipIdInvalid = 20;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->resetDriver(chipIdInvalid, "wlan0");
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->resetDriver(chipId, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->resetDriver(chipId, staFeature->baseFeature.ifName);
    EXPECT_EQ(ret, HDF_SUCCESS);
    sleep(RESET_TIME);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: WifiHalResetDriver002
 * @tc.name: Reset the WiFi driver
 * @tc.size:Medium
 * @tc.level: Level 3
 */
HWTEST_F(WifiHalTest, SUB_WLAN_HDI_ResetDriver_0002, Function | MediumTest | Level3)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    uint8_t chipId = 0;
    uint8_t chipIdInvalid = 20;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->resetDriver(chipIdInvalid, "wlan0");
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->resetDriver(chipId, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->resetDriver(chipId, apFeature->baseFeature.ifName);
    EXPECT_EQ(ret, HDF_SUCCESS);
    sleep(RESET_TIME);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
};