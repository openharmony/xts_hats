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
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <osal_mem.h>
#include "hdf_base.h"
#include "hdf_sbuf.h"
#include "wifi_hal.h"
#include "wifi_hal_ap_feature.h"
#include "wifi_hal_base_feature.h"
#include "wifi_hal_sta_feature.h"
#include "securec.h"

using namespace std;
using namespace testing::ext;

namespace  {
struct IWiFi *g_wifi = nullptr;
const int32_t WLAN_TX_POWER = 160;
const uint32_t WLAN_MIN_CHIPID = 0;
const uint32_t WLAN_MAX_CHIPID = 2;
const uint32_t IFNAME_MIN_NUM = 0;
const uint32_t IFNAME_MAX_NUM = 32;
const uint32_t MAX_IF_NAME_LENGTH = 16;
const uint32_t SIZE = 4;
const int32_t WLAN_BAND_2G = 0;
const int32_t WLAN_FREQ_MAX_NUM = 14;
const int32_t WLAN_MAX_NUM_STA_WITH_AP = 4;
const uint32_t DEFAULT_COMBO_SIZE = 10;
const uint32_t TEST_BUF_SIZE = 64;
const uint32_t TEST_PARAM_BUF_SIZE = 64;
const int32_t TEST_CMD = 123;
const uint32_t RESET_TIME = 20;

class wlanBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void wlanBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    int ret = WifiConstruct(&g_wifi);
    ASSERT_EQ(HDF_SUCCESS, ret);
}

void wlanBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    int ret = WifiDestruct(&g_wifi);
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
    printf("HalCallbackEvent ifName = %s, event = %u\n", ifName, event);
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
 * @tc.name:wifiHalcreateFeaturete001
 * @tc.desc: Wifi hal create and destroy feature for ap mode benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, wifiHalcreateFeaturete001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    for (auto _ : st) {
        ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
        g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, wifiHalcreateFeaturete001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name:wifiHalcreateFeaturete002
 * @tc.desc: Wifi hal create and destroy feature for sta mode benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, wifiHalcreateFeaturete002)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    for (auto _ : st) {
        g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
        ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, wifiHalcreateFeaturete002)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetFeatureByIfName001
 * @tc.desc: Wifi hal get feature by ifname benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, wifiHalGetFeatureByIfName001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct IWiFiAp *apFeatureGet = nullptr;
    const char *ifName0 = "wlanTest";
    const char *ifName1 = "wlan0";
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = g_wifi->getFeatureByIfName(ifName0, (struct IWiFiBaseFeature **)&apFeatureGet);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = g_wifi->getFeatureByIfName(ifName1, (struct IWiFiBaseFeature **)&apFeatureGet);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeatureGet);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, wifiHalGetFeatureByIfName001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalRegisterEventCallback001
 * @tc.desc: Wifi hal register callback benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, wifiHalregisterEventCallback001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    for (auto _ : st) {
        ret = g_wifi->registerEventCallback(HalCallbackEvent, "wlan0");
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, wifiHalregisterEventCallback001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalUnRegisterEventCallback001
 * @tc.desc: Wifi hal unregister callback benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, wifiHalunregisterEventCallback001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    for (auto _ : st) {
        ret = g_wifi->unregisterEventCallback(HalCallbackEvent, "wlan0");
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, wifiHalunregisterEventCallback001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetNetworkIfaceName001
 * @tc.desc: Wifi hal get network iface name benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetNetworkIfaceName001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    const char *ifName=nullptr;
    struct IWiFiAp *apFeature = nullptr;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    for (auto _ : st) {
        ifName = apFeature->baseFeature.getNetworkIfaceName((const struct IWiFiBaseFeature *)apFeature);
    }
    EXPECT_NE(nullptr, ifName);
    ret = strcmp(ifName, "wlan0");
    EXPECT_EQ(0, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetNetworkIfaceName001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetGetFeatureType001
 * @tc.desc: Wifi hal get feature type benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetFeatureType001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t type;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    for (auto _ : st) {
        type = apFeature->baseFeature.getFeatureType((struct IWiFiBaseFeature *)apFeature);
    }
    EXPECT_EQ(PROTOCOL_80211_IFTYPE_AP, type);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetFeatureType001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

 /**
 * @tc.name: WifiHalSetMacAddress001
 * @tc.desc: Wifi hal set Mac address benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalsetMacAddress001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char errorMac[ETH_ADDR_LEN] = {0x11, 0x34, 0x56, 0x78, 0xab, 0xcd};
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, errorMac, ETH_ADDR_LEN);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = apFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)apFeature, mac, ETH_ADDR_LEN);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalsetMacAddress001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalSetMacAddress002
 * @tc.desc: Wifi hal set Mac address benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalsetMacAddress002)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char errorMac[ETH_ADDR_LEN] = {0};
    unsigned char mac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, errorMac, ETH_ADDR_LEN);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = staFeature->baseFeature.setMacAddress((struct IWiFiBaseFeature *)staFeature, mac, ETH_ADDR_LEN);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalsetMacAddress002)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalSetTxPower001
 * @tc.desc: Wifi hal set transmit power benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalsetTxPower001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)apFeature, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = apFeature->baseFeature.setTxPower((struct IWiFiBaseFeature *)apFeature, WLAN_TX_POWER);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalsetTxPower001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalSetCountryCode001
 * @tc.desc: Wifi hal set country code benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalsetCountryCode001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->setCountryCode(apFeature, nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = apFeature->setCountryCode(apFeature, "CN", 2);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalsetCountryCode001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetIfNamesByChipId001
 * @tc.desc: Obtain all ifNames and the number of the current chip
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetIfNamesByChipId001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
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
    ASSERT_TRUE(chipId <= WLAN_MAX_CHIPID && chipId >= WLAN_MIN_CHIPID);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.getIfNamesByChipId(chipId, nullptr, nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = staFeature->baseFeature.getIfNamesByChipId(chipId, &ifNames, &num);
    }
    EXPECT_NE(nullptr, ifNames);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ASSERT_TRUE(num <= IFNAME_MAX_NUM && num >= IFNAME_MIN_NUM);
    for (i = 0; i < num; i++) {
        EXPECT_EQ(0, strncmp("wlan", ifNames + i * MAX_IF_NAME_LENGTH, SIZE));
    }
    free(ifNames);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetIfNamesByChipId001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetSupportFeature001
 * @tc.desc: Get supported features
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetSupportFeature001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    uint8_t supportTest[PROTOCOL_80211_IFTYPE_NUM] = {0};
    uint8_t support[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};
    ret = g_wifi->getSupportFeature(nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->getSupportFeature(supportTest, PROTOCOL_80211_IFTYPE_NUM);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = g_wifi->getSupportFeature(support, PROTOCOL_80211_IFTYPE_NUM + 1);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetSupportFeature001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetSupportCombo001
 * @tc.desc: Get supported combo
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetSupportCombo001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    uint8_t support[PROTOCOL_80211_IFTYPE_NUM + 1] = {0};
    uint64_t combo[DEFAULT_COMBO_SIZE] = {0};
    ret = g_wifi->getSupportFeature(support, PROTOCOL_80211_IFTYPE_NUM + 1);
    EXPECT_EQ(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = g_wifi->getSupportCombo(nullptr, 0);
    }
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = g_wifi->getSupportCombo(combo, DEFAULT_COMBO_SIZE);
    if (support[PROTOCOL_80211_IFTYPE_NUM] == 0) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    } else {
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetSupportCombo001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetDeviceMacAddress001
 * @tc.desc: Get device Mac address for ap mode
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetDeviceMacAddress001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0};
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, mac, ETH_ADDR_LEN - 1);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = apFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)apFeature, mac, ETH_ADDR_LEN);
    }
    EXPECT_NE(HDF_FAILURE, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetDeviceMacAddress001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetDeviceMacAddress002
 * @tc.desc: Get device Mac address for sta mode
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetDeviceMacAddress002)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char mac[ETH_ADDR_LEN] = {0};
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, mac, ETH_ADDR_LEN - 1);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = staFeature->baseFeature.getDeviceMacAddress((struct IWiFiBaseFeature *)staFeature, mac, ETH_ADDR_LEN);
    }
    EXPECT_NE(HDF_FAILURE, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetDeviceMacAddress002)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHaGetValidFreqsWithBand001
 * @tc.desc: Get available frequencies
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetValidFreqsWithBand001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    int32_t freq[WLAN_FREQ_MAX_NUM] = {0};
    uint32_t num = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature,
    WLAN_BAND_2G, nullptr, 0, nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = apFeature->baseFeature.getValidFreqsWithBand((struct IWiFiBaseFeature *)apFeature,
            WLAN_BAND_2G, freq, WLAN_FREQ_MAX_NUM, &num);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetValidFreqsWithBand001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetAssociatedStas001
 * @tc.desc: Get asscociated STA info benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetAssociatedStas001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiAp *apFeature = nullptr;
    struct StaInfo staInfo[WLAN_MAX_NUM_STA_WITH_AP] = {{0}};
    uint32_t num = 0;
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, apFeature);
    ret = apFeature->getAssociatedStas(apFeature, nullptr, 0, nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = apFeature->getAssociatedStas(apFeature, staInfo, WLAN_MAX_NUM_STA_WITH_AP, &num);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetAssociatedStas001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalSetScanningMacAddress001
 * @tc.desc: Set Mac address scanning benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalsetScanningMacAddress001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char scanMac[ETH_ADDR_LEN] = {0x12, 0x34, 0x56, 0x78, 0xab, 0xcd};
    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->setScanningMacAddress(staFeature, nullptr, 0);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = staFeature->setScanningMacAddress(staFeature, scanMac, ETH_ADDR_LEN);
    }
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalsetScanningMacAddress001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: WifiHalGetNetdevInfo001
 * @tc.desc: Wifi hdi get netdev info benchmark test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, WifiHalgetNetDevInfo001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int ret;
    struct NetDeviceInfoResult netDeviceInfoResult;
    (void)memset_s(&netDeviceInfoResult, sizeof(struct NetDeviceInfoResult), 0, sizeof(struct NetDeviceInfoResult));
    for (auto _ : st) {
        ret = g_wifi->getNetDevInfo(&netDeviceInfoResult);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, WifiHalgetNetDevInfo001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

 
/**
 * @tc.name: GetPowerModeTest001
 * @tc.desc: Wifi hdi get power mode function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, GetPowerMode001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";
    uint8_t mode;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    for (auto _ : st) {
        ret = g_wifi->getPowerMode(nullptr, &mode);
    }
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(ifName, &mode);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(apFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getPowerMode(apFeature->baseFeature.ifName, &mode);
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, GetPowerMode001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SetPowerModeTest001
 * @tc.desc: Wifi hdi set power mode function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, SetPowerModeTest001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int32_t ret;
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "eth0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_NE(apFeature, nullptr);
    ret = g_wifi->setPowerMode(nullptr, WIFI_POWER_MODE_SLEEPING);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setPowerMode(ifName, WIFI_POWER_MODE_SLEEPING);
    EXPECT_NE(ret, HDF_SUCCESS);
    for (auto _ : st) {
        ret = g_wifi->setPowerMode(apFeature->baseFeature.ifName, WIFI_POWER_MODE_SLEEPING);
    }
    bool flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, SetPowerModeTest001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: SetProjectionScreenParam001
 * @tc.desc: wifi hal config projection screen function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, SetProjectionScreenParam001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
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
    ret = g_wifi->setProjectionScreenParam(nullptr, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(apFeature->baseFeature.ifName, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->setProjectionScreenParam(nullptr, param);
    EXPECT_NE(ret, HDF_SUCCESS);
    for (auto _ : st) {
    ret = g_wifi->setProjectionScreenParam(apFeature->baseFeature.ifName, param);
    }
    EXPECT_NE(ret, HDF_SUCCESS);
    for (int i = CMD_CLOSE_GO_CAC; i <= CMD_ID_CTRL_ROAM_CHANNEL; i++) {
        param->cmdId = i;
        ret = g_wifi->setProjectionScreenParam(apFeature->baseFeature.ifName, param);
        flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
        ASSERT_TRUE(flag);
    }
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    OsalMemFree(param);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, SetProjectionScreenParam001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

    
/**
 * @tc.name: SendCmdIoctl001
 * @tc.desc: wifi hal send ioctl command function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, SendCmdIoctl001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int32_t cmdId = 0;
    int32_t ret;
    bool flag;
    struct IWiFiAp *apFeature = nullptr;
    int8_t data[TEST_BUF_SIZE] = {0};
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(nullptr, cmdId, nullptr, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->sendCmdIoctl(ifName, cmdId, nullptr, TEST_BUF_SIZE);
    EXPECT_NE(ret, HDF_SUCCESS);
    for (auto _ : st) {
        ret = g_wifi->sendCmdIoctl(nullptr, cmdId, data, TEST_BUF_SIZE);
    }
    EXPECT_NE(ret, HDF_SUCCESS);
    for (cmdId = CMD_HID2D_MODULE_INIT; cmdId <= CMD_SET_CHAN_ADJUST; cmdId++) {
        ret = g_wifi->sendCmdIoctl(ifName, cmdId, data, TEST_BUF_SIZE);
        flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
        ASSERT_TRUE(flag);
    }
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, SendCmdIoctl001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();
  
/**
 * @tc.name: GetStationInfo001
 * @tc.desc: Wifi hdi get station information function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, GetStationInfo001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int32_t ret;
    StationInfo info;
    bool flag;
    uint8_t mac[ETH_ADDR_LEN] = {0};
    struct IWiFiAp *apFeature = nullptr;
    const char *ifName = "wlan0";

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_AP, (struct IWiFiBaseFeature **)&apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, nullptr, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, nullptr, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, &info, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, nullptr, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, &info, nullptr, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(nullptr, &info, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    ret = g_wifi->getStationInfo(ifName, nullptr, mac, ETH_ADDR_LEN);
    EXPECT_NE(ret, HDF_SUCCESS);
    for (auto _ : st) {
        ret = g_wifi->getStationInfo(ifName, &info, mac, ETH_ADDR_LEN);
    }
    flag = (ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    ASSERT_TRUE(flag);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)apFeature);
    EXPECT_EQ(ret, HDF_SUCCESS);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, GetStationInfo001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: HalGetChipId001
 * @tc.desc: wifi hal get chip ID function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, GetChipId001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    unsigned char chipId = 0;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);

    ret = staFeature->baseFeature.getChipId(nullptr, &chipId);
    EXPECT_NE(HDF_SUCCESS, ret);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    }
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, GetChipId001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
 * @tc.name: ResetDriver001
 * @tc.desc: wifi hal reset driver function test
 * @tc.type: FUNC
 */

BENCHMARK_F(wlanBenchmarkTest, ResetDriver001)(
    benchmark::State &st)
{
    g_wifi->start(g_wifi);
    int32_t ret;
    struct IWiFiSta *staFeature = nullptr;
    uint8_t chipId = 0;
    uint8_t chipIdInvalid = 20;

    ret = g_wifi->createFeature(PROTOCOL_80211_IFTYPE_STATION, (struct IWiFiBaseFeature **)&staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_NE(nullptr, staFeature);
    ret = staFeature->baseFeature.getChipId((struct IWiFiBaseFeature *)staFeature, &chipId);
    ASSERT_TRUE(chipId < MAX_WLAN_DEVICE);
    EXPECT_EQ(HDF_SUCCESS, ret);
    for (auto _ : st) {
        ret = g_wifi->resetDriver(chipIdInvalid, "wlan0");
    }
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
    ret = g_wifi->resetDriver(chipId, nullptr);
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
    ret = g_wifi->resetDriver(chipId, staFeature->baseFeature.ifName);
    EXPECT_EQ(HDF_SUCCESS, ret);
    sleep(RESET_TIME);
    ret = g_wifi->destroyFeature((struct IWiFiBaseFeature *)staFeature);
    EXPECT_EQ(HDF_SUCCESS, ret);
    g_wifi->stop(g_wifi);
}

BENCHMARK_REGISTER_F(wlanBenchmarkTest, ResetDriver001)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();
}
BENCHMARK_MAIN();

