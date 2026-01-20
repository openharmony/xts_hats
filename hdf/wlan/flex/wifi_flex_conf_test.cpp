/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <numeric>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <gtest/gtest.h>

using namespace testing::ext;

class HatsFlexWifiConfKeywordsTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HatsFlexWifiConfKeywordsTest::SetUpTestCase() {}
void HatsFlexWifiConfKeywordsTest::TearDownTestCase() {}
void HatsFlexWifiConfKeywordsTest::SetUp() {}
void HatsFlexWifiConfKeywordsTest::TearDown() {}

HWTEST_F(HatsFlexWifiConfKeywordsTest, testWpaSupplicantConf, Function | MediumTest | Level1)
{
    const std::string confPath = "/data/service/el1/public/wifi/wpa_supplicant/wpa_supplicant.conf";
    std::ifstream file(confPath);
    EXPECT_TRUE(file.is_open());

    const std::unordered_set<std::string> requiredKeywords = {
        "country", "ctrl_interface", "sae_pwe", "network"
    };
    std::unordered_set<std::string> foundKeywords;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        size_t pos = line.find_first_not_of(" \t");
        if (pos == std::string::npos) {
            continue;
        }
        std::string trimmed = line.substr(pos);
        for (const auto& key : requiredKeywords) {
            if (trimmed.rfind(key + "=", 0) == 0) {
                foundKeywords.insert(key);
            }
        }
    }
    file.close();

    EXPECT_EQ(foundKeywords.size(), requiredKeywords.size());
}

HWTEST_F(HatsFlexWifiConfKeywordsTest, testP2pSupplicantConf, Function | MediumTest | Level1)
{
    const std::string confPath = "/data/service/el1/public/wifi/wpa_supplicant/p2p_supplicant.conf";
    std::ifstream file(confPath);
    EXPECT_TRUE(file.is_open());

    const std::unordered_set<std::string> requiredKeywords = {
        "ctrl_interface", "device_name", "manufacturer", "device_type",
        "config_methods", "p2p_listen_reg_class", "p2p_listen_channel", "p2p_oper_reg_class",
        "p2p_oper_channel", "p2p_go_intent", "persistent_reconnect", "serial_number",
        "p2p_ssid_postfix", "p2p_go_ht40", "p2p_go_vht", "update_config", "p2p_device_random_mac_addr",
        "p2p_interface_random_mac_addr"
    };
    std::unordered_set<std::string> foundKeywords;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        size_t pos = line.find_first_not_of(" \t");
        if (pos == std::string::npos) {
            continue;
        }
        std::string trimmed = line.substr(pos);
        for (const auto& key : requiredKeywords) {
            if (trimmed.rfind(key + "=", 0) == 0) {
                foundKeywords.insert(key);
            }
        }
    }
    file.close();

    EXPECT_EQ(foundKeywords.size(), requiredKeywords.size());
}

HWTEST_F(HatsFlexWifiConfKeywordsTest, testChbaSupplicantConf, Function | MediumTest | Level1)
{
    const std::string confPath = "/data/service/el1/public/wifi/wpa_supplicant/chba_supplicant.conf";
    std::ifstream file(confPath);
    EXPECT_TRUE(file.is_open());

    const std::unordered_set<std::string> requiredKeywords = {
        "ctrl_interface", "device_name", "manufacturer", "device_type",
        "config_methods", "p2p_listen_reg_class", "p2p_listen_channel", "p2p_oper_reg_class",
        "p2p_oper_channel", "p2p_go_intent", "persistent_reconnect", "serial_number",
        "p2p_ssid_postfix", "p2p_go_ht40", "p2p_go_vht", "update_config", "p2p_device_random_mac_addr",
        "p2p_interface_random_mac_addr"
    };
    std::unordered_set<std::string> foundKeywords;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        size_t pos = line.find_first_not_of(" \t");
        if (pos == std::string::npos) {
            continue;
        }
        std::string trimmed = line.substr(pos);
        for (const auto& key : requiredKeywords) {
            if (trimmed.rfind(key + "=", 0) == 0) {
                foundKeywords.insert(key);
            }
        }
    }
    file.close();

    EXPECT_EQ(foundKeywords.size(), requiredKeywords.size());
}
