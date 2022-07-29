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
#include <cstdint>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "hdf_uhdf_test.h"
#include "hdf_wifi_test.h"
#include "hdf_sbuf.h"
#include "wifi_hal.h"
#include "wifi_hal_ap_feature.h"
#include "wifi_hal_base_feature.h"
#include "wifi_hal_sta_feature.h"

using namespace testing::ext;

namespace HalTest {
struct IWiFi *g_wifi = nullptr;
const int32_t WLAN_TX_POWER = 160;
const uint32_t IFNAME_MIN_NUM = 0;
const uint32_t IFNAME_MAX_NUM = 32;
const uint32_t MAX_IF_NAME_LENGTH = 16;
const uint32_t SIZE = 4;

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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0100, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);

    ret = g_wifi->destroyFeature(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalCreateAndDestroyFeature002
 * @tc.desc: Wifi hal create and destroy feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0110, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);

    ret = g_wifi->destroyFeature(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName001
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0120, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct IWiFiAp *apFeatureGet = nullptr;
    const char *ifName0 = "wlanTest";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getFeatureByIfName(nullptr, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getFeatureByIfName(ifName0, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->getFeatureByIfName(apFeature->baseFeature.ifName, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeatureGet);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalRegisterEventCallback001
 * @tc.desc: Wifi hal register callback function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0130, Function | MediumTest | Level1)
{
    int ret;

    ret = g_wifi->registerEventCallback(HalCallbackEvent, "wlan0");
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalUnRegisterEventCallback001
 * @tc.desc: Wifi hal unregister callback function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0140, Function | MediumTest | Level1)
{
    int ret;

    ret = g_wifi->unregisterEventCallback(HalCallbackEvent, "wlan0");
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetNetworkIfaceName001
 * @tc.desc: Wifi hal get network iface name function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_DriverSystem_WifiHdi_0150, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    const char *ifnameTest = apFeature->baseFeature.getNetworkIfaceName(nullptr);
    EXPECT_EQ(ifnameTest, nullptr);
    const char *ifName = apFeature->baseFeature.getNetworkIfaceName((const struct IWiFiBaseFeature *)apFeature);
    EXPECT_NE(nullptr, ifName);

    if (strncmp(ifName, "wlan", 4) == 0 || strncmp(ifName, "nan", 3) == 0 || strncmp(ifName, "p2p", 3) == 0) {
        ret = 0;
    } else {
        ret = -1;
    }
    EXPECT_EQ(0, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetFeatureType001
 * @tc.desc: Wifi hal get feature type function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0160, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t type;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    type = apFeature->baseFeature.getFeatureType(nullptr);
    EXPECT_EQ(type, HDF_FAILURE);
    type = apFeature->baseFeature.getFeatureType((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(PROTOCOL_80211_IFTYPE_AP, type);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress001
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_DriverSystem_WifiHdi_0170, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char errorMac[ETH_ADDR_LEN] = {0x11, 0x34, 0x56, 0x78, 0xab, 0xcd};
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress(nullptr, mac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, nullptr, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, mac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, errorMac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, mac, ETH_ADDR_LEN);
    printf("%s: ret = %d\n", __func__, ret);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_DEVICE_BUSY);
    ASSERT_TRUE(flag);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetMacAddress002
 * @tc.desc: Wifi hal set Mac address function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0180, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
    unsigned char errorMac[ETH_ADDR_LEN] = {0x11, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress(nullptr, mac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, nullptr, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, mac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, errorMac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
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
HWTEST_F(WifiHalTest,  SUB_DriverSystem_WifiHdi_0190, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setTxPower(nullptr, WLAN_TX_POWER);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)apFeature, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = apFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)apFeature, WLAN_TX_POWER);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalSetCountryCode001
 * @tc.desc: Wifi hal set country code function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_DriverSystem_WifiHdi_0200, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(apFeature, nullptr, 0);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->setCountryCode(nullptr, "CN", 2);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->setCountryCode(apFeature, "CN", 3);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->setCountryCode(apFeature, "99", 2);
    EXPECT_EQ(ret, HDF_SUCCESS);
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0210, Function | MediumTest | Level1)
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
    ret = staFeature->baseFeature.getIfNamesByChipId(100, &ifNames, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
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
 * @tc.name: WifiHalGetSupportFeature001
 * @tc.desc: Get supported features
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0220, Function | MediumTest | Level1)
{
    int32_t ret;
    uint8_t supType[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};

    ret = g_wifi->getSupportFeature(nullptr, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->getSupportFeature(supType, PROTOCOL_80211_IFTYPE_NUM);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);

    ret = g_wifi->getSupportFeature(supType, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(ret, RET_CODE_SUCCESS);
}

/**
 * @tc.name: HalGetChipId002
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0230, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getChipId(nullptr, &chipId);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)apFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress001
 * @tc.desc: Get device Mac address for ap mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0240, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = apFeature->baseFeature.getDeviceMacAddress(nullptr, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, nullptr, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, readMac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetDeviceMacAddress002
 * @tc.desc: Get device Mac address for sta mode
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0250, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    unsigned char readMac[ETH_ADDR_LEN] = {0};
    ret = staFeature->baseFeature.getDeviceMacAddress(nullptr, readMac, ETH_ADDR_LEN);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, nullptr, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, readMac, 0);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0260, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t bandNotSupport = IEEE80211_NUM_BANDS;
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
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature, band, freqs, 10, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        band, freqs, size, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        bandNotSupport, freqs, size, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)staFeature,
                                                        band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetAssociatedStas001
 * @tc.desc: Get asscociated STA info
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0270, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[MAX_ASSOC_STA_NUM];
    (void)memset_s(staInfo, sizeof(StaInfo) * MAX_ASSOC_STA_NUM, 0, sizeof(StaInfo) * MAX_ASSOC_STA_NUM);
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->getAsscociatedStas(nullptr, staInfo, MAX_ASSOC_STA_NUM, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->getAsscociatedStas(apFeature, nullptr, MAX_ASSOC_STA_NUM, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->getAsscociatedStas(apFeature, staInfo, 0, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->getAsscociatedStas(apFeature, staInfo, MAX_ASSOC_STA_NUM, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->getAsscociatedStas(apFeature, staInfo, MAX_ASSOC_STA_NUM, &num);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetScanningMacAddress001
 * @tc.desc: Set Mac address scanning
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0280, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char scanMac[WIFI_MAC_ADDR_LENGTH] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    ret = staFeature->setScanningMacAddress(nullptr, scanMac, WIFI_MAC_ADDR_LENGTH);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->setScanningMacAddress(staFeature, nullptr, WIFI_MAC_ADDR_LENGTH);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->setScanningMacAddress(staFeature, scanMac, 0);
    EXPECT_NE(ret, HDF_SUCCESS);
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0290, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct NetDeviceInfoResult netDeviceInfoResult;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getNetDevInfo(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->getNetDevInfo(&netDeviceInfoResult);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: WifiHalResetDriver001
 * @tc.name: Reset the WiFi driver
 * @tc.size:Medium
 * @tc.level: Level 3
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0300, Function | MediumTest | Level3)
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

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalStartScan001
 * @tc.desc: start scan is successfully
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest,  SUB_DriverSystem_WifiHdi_0310, Function | MediumTest | Level3)
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
    ret = staFeature->startScan(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->startScan(ifName, &scan);
    EXPECT_EQ(HDF_SUCCESS, ret);
    sleep(10);
}

/**
 * @tc.number: WifiHalResetDriver002
 * @tc.name: Reset the WiFi driver
 * @tc.size:Medium
 * @tc.level: Level 3
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0320, Function | MediumTest | Level3)
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

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: WifiHalGetNetDevInfo002
 * @tc.desc: Wifi hdi get netdev info function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0330, Function | MediumTest | Level1)
{
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    struct NetDeviceInfoResult netDeviceInfoResult;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = g_wifi->getNetDevInfo(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->getNetDevInfo(&netDeviceInfoResult);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.name: GetPowerModeTest_001
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0340, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    printf("GetPowerMode001: ifname is %s\n", apFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(nullptr, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(ifName, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(apFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(apFeature->baseFeature.ifName, &mode);
    printf("%s: ret = %d\n", __func__, ret);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetPowerModeTest_002
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0350, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    const char *ifName = "eth0";
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    printf("GetPowerMode002: ifname is %s\n", staFeature->baseFeature.ifName);
    ret = g_wifi->getPowerMode(nullptr, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(ifName, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(staFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0360, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0370, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0380, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0390, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0400, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0410, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0420, Function | MediumTest | Level1)
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
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0430, Function | MediumTest | Level1)
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
 * @tc.name: WifiHalCreateFeature003
 * @tc.desc: Wifi hal create feature function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0440, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, nullptr);
    EXPECT_EQ(ret, HDF_FAILURE);
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, nullptr);
    EXPECT_EQ(ret, HDF_FAILURE);

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_NUM, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->createFeature(-1, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalGetFeatureByIfName002
 * @tc.desc: Wifi hal get feature by ifname function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0450, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    struct IWiFiSta *staFeatureGet = nullptr;
    const char *ifName0 = "wlanTest";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = g_wifi->getFeatureByIfName(nullptr, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getFeatureByIfName(ifName0, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getFeatureByIfName(staFeature->baseFeature.ifName, (struct IWiFiBaseFeature **)&staFeatureGet);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeatureGet, nullptr);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalRegisterEventCallback002
 * @tc.desc: Wifi hal register event callback test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0460, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_wifi->registerEventCallback(nullptr, "wlan0");
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->registerEventCallback(HalCallbackEvent, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: WifiHalUnRegisterEventCallback002
 * @tc.desc: Wifi hal unregister event callback test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0470, Function | MediumTest | Level1)
{
    int32_t ret;

    ret = g_wifi->unregisterEventCallback(nullptr, "wlan0");
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = g_wifi->unregisterEventCallback(HalCallbackEvent, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.name: WifiHalGetNetworkIfaceName002
 * @tc.desc: Wifi hal get network iface name function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0480, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    const char *ifnameTest = staFeature->baseFeature.getNetworkIfaceName(nullptr);
    EXPECT_EQ(ifnameTest, nullptr);
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
 * @tc.name: WifiHalGetGetFeatureType002
 * @tc.desc: Wifi hal get feature type function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0490, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    int32_t type;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    type = staFeature->baseFeature.getFeatureType(nullptr);
    EXPECT_EQ(type, HDF_FAILURE);
    type = staFeature->baseFeature.getFeatureType((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(type, PROTOCOL_80211_IFTYPE_STATION);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: GetValidFreqsWithBand002
 * @tc.desc: Wifi hal get valid frequency with specific band test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0500, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t band = IEEE80211_BAND_2GHZ;
    int32_t bandNotSupport = IEEE80211_NUM_BANDS;
    int32_t freqs[MAX_CHANNEL_NUM] = {0};
    uint32_t size = MAX_CHANNEL_NUM;
    uint32_t num = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = apFeature->baseFeature.getValidFreqsWithBand(nullptr, band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature, band, nullptr, size, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature, band, freqs, 10, &num);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature,
                                                       band, freqs, size, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature,
                                                       bandNotSupport, freqs, size, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature, band, freqs, size, &num);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.name: WifiHalSetTxPower002
 * @tc.desc: Wifi hal set transmit power function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0510, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);
    ret = staFeature->baseFeature.setTxPower(nullptr, WLAN_TX_POWER);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)staFeature, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)staFeature, WLAN_TX_POWER);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}


/**
 * @tc.name: WifiHalGetIfNamesByChipId002
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0520, Function | MediumTest | Level1)
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
    ret = apFeature->baseFeature.getIfNamesByChipId(100, &ifNames, &num);
    EXPECT_NE(ret, HDF_SUCCESS);
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
 * @tc.name: HalGetChipId001
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */
HWTEST_F(WifiHalTest, SUB_DriverSystem_WifiHdi_0530, Function | MediumTest | Level1)
{
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(staFeature, nullptr);

    ret = staFeature->baseFeature.getChipId(nullptr, &chipId);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
};