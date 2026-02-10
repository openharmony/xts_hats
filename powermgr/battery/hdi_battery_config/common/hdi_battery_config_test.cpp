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

#include <gtest/gtest.h>
#include <cJSON.h>
#include <fstream>
#include <memory>
#include <string>
#include <hdf_core_log.h>
#include <config_policy_utils.h>

using namespace testing::ext;
using namespace OHOS;

namespace {
}

class HdfBatteryConfigTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    static inline const char* configPath_;
    static inline cJSON* config_;
};

void HdfBatteryConfigTest::SetUpTestCase(void)
{
    config_ = nullptr;
    configPath_ = "/vendor/etc/battery/battery_config.json";
}

void HdfBatteryConfigTest::TearDownTestCase(void)
{
    if (config_) {
        cJSON_Delete(config_);
        config_ = nullptr;
    }
}

void HdfBatteryConfigTest::SetUp(void)
{
    const auto* test_info = testing::UnitTest::GetInstance()->current_test_info();
    HDF_LOGI("%{public}s.%{public}s start", test_info->test_suite_name(), test_info->name());
}

void HdfBatteryConfigTest::TearDown(void)
{
    const auto* test_info = testing::UnitTest::GetInstance()->current_test_info();
    HDF_LOGI("%{public}s.%{public}s end", test_info->test_suite_name(), test_info->name());
}

bool IsEmptyJsonParse(const cJSON* jsonValue)
{
    return jsonValue && (cJSON_IsNull(jsonValue) || (cJSON_IsObject(jsonValue) && (jsonValue->child == nullptr)) ||
        (cJSON_IsArray(jsonValue) && (cJSON_GetArraySize(jsonValue) == 0)));
}

namespace {
/**
 * @tc.name: HdfBatteryConfigTest001
 * @tc.desc: Test /vendor/etc/battery/battery_config.json exist
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryConfigTest, HdfBatteryConfigTest001, TestSize.Level1)
{
    std::ifstream ifsConf;
    bool isOpen = false;
    ifsConf.open(configPath_);
    isOpen = ifsConf.is_open();
    EXPECT_EQ(isOpen, true);
    std::string content((std::istreambuf_iterator<char>(ifsConf)), std::istreambuf_iterator<char>());
    EXPECT_FALSE(content.empty());
    config_ = cJSON_Parse(content.c_str());
    EXPECT_NE(config_, nullptr);
    ifsConf.close();
    EXPECT_FALSE(IsEmptyJsonParse(config_));
}

/**
 * @tc.name: HdfBatteryConfigTest002
 * @tc.desc: Test battery_config.json light node exist
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryConfigTest, HdfBatteryConfigTest002, TestSize.Level1)
{
    cJSON* light = cJSON_GetObjectItemCaseSensitive(config_, "light");
    cJSON* low = cJSON_GetObjectItemCaseSensitive(light, "low");
    cJSON* socItem = cJSON_GetObjectItemCaseSensitive(low, "soc");
    cJSON* rgbItem = cJSON_GetObjectItemCaseSensitive(low, "rgb");
    EXPECT_TRUE(socItem && cJSON_IsArray(socItem));
    EXPECT_TRUE(rgbItem && cJSON_IsArray(rgbItem));

    cJSON* normal = cJSON_GetObjectItemCaseSensitive(light, "normal");
    socItem = cJSON_GetObjectItemCaseSensitive(normal, "soc");
    rgbItem = cJSON_GetObjectItemCaseSensitive(normal, "rgb");
    EXPECT_TRUE(socItem && cJSON_IsArray(socItem));
    EXPECT_TRUE(rgbItem && cJSON_IsArray(rgbItem));

    cJSON* high = cJSON_GetObjectItemCaseSensitive(light, "high");
    socItem = cJSON_GetObjectItemCaseSensitive(high, "soc");
    rgbItem = cJSON_GetObjectItemCaseSensitive(high, "rgb");
    EXPECT_TRUE(socItem && cJSON_IsArray(socItem));
    EXPECT_TRUE(rgbItem && cJSON_IsArray(rgbItem));
}

/**
 * @tc.name: HdfBatteryConfigTest003
 * @tc.desc: Test battery_config.json soc node exist
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryConfigTest, HdfBatteryConfigTest003, TestSize.Level1)
{
    cJSON* soc = cJSON_GetObjectItemCaseSensitive(config_, "soc");
    cJSON* shutdownItem = cJSON_GetObjectItemCaseSensitive(soc, "shutdown");
    EXPECT_TRUE(shutdownItem && cJSON_IsNumber(shutdownItem));

    cJSON* criticalItem = cJSON_GetObjectItemCaseSensitive(soc, "critical");
    EXPECT_TRUE(criticalItem && cJSON_IsNumber(criticalItem));

    cJSON* warningItem = cJSON_GetObjectItemCaseSensitive(soc, "warning");
    EXPECT_TRUE(warningItem && cJSON_IsNumber(warningItem));

    cJSON* lowItem = cJSON_GetObjectItemCaseSensitive(soc, "low");
    EXPECT_TRUE(lowItem && cJSON_IsNumber(lowItem));

    cJSON* normalItem = cJSON_GetObjectItemCaseSensitive(soc, "normal");
    EXPECT_TRUE(normalItem && cJSON_IsNumber(normalItem));

    cJSON* highItem = cJSON_GetObjectItemCaseSensitive(soc, "high");
    EXPECT_TRUE(highItem && cJSON_IsNumber(highItem));

    cJSON* fullItem = cJSON_GetObjectItemCaseSensitive(soc, "full");
    EXPECT_TRUE(fullItem && cJSON_IsNumber(fullItem));
}

/**
 * @tc.name: HdfBatteryConfigTest004
 * @tc.desc: Test battery_config.json temperature node exist
 * @tc.type: Compatibility
 */
HWTEST_F(HdfBatteryConfigTest, HdfBatteryConfigTest004, TestSize.Level1)
{
    cJSON* temperature = cJSON_GetObjectItemCaseSensitive(config_, "temperature");
    cJSON* highItem = cJSON_GetObjectItemCaseSensitive(temperature, "high");
    cJSON* lowItem = cJSON_GetObjectItemCaseSensitive(temperature, "low");
    EXPECT_TRUE(highItem && cJSON_IsNumber(highItem));
    EXPECT_TRUE(lowItem && cJSON_IsNumber(lowItem));
}

constexpr const char* VENDOR_BATTERY_SPLIT_CONFIG_PATH = "/vendor/etc/battery/charge_config.json";
constexpr const char* BATTERY_CONFIG_PATH = "etc/battery/battery_config.json";
/**
 * @tc.name: VendorBatteryConfigTest001
 * @tc.desc: Test vendor component reading light node from battery_config.json
 * @tc.type: Compatibility
 */
HWTEST_F(HdfBatteryConfigTest, VendorBatteryConfigTest001, TestSize.Level1)
{
    char buf[MAX_PATH_LEN] = {0};
    char* path = GetOneCfgFile(BATTERY_CONFIG_PATH, buf, MAX_PATH_LEN);
    ASSERT_TRUE(path != nullptr);
    std::ifstream ifsConf {path};
    std::string content {std::istreambuf_iterator<char>(ifsConf), std::istreambuf_iterator<char>()};
    ASSERT_FALSE(content.empty());
    ifsConf.close();

    std::unique_ptr<cJSON, decltype(&cJSON_Delete)> rootJson {cJSON_Parse(content.c_str()), &cJSON_Delete};

    cJSON* light = cJSON_GetObjectItemCaseSensitive(rootJson.get(), "light");
    cJSON* low = cJSON_GetObjectItemCaseSensitive(light, "low");
    cJSON* socItem = cJSON_GetObjectItemCaseSensitive(low, "soc");
    cJSON* rgbItem = cJSON_GetObjectItemCaseSensitive(low, "rgb");
    EXPECT_TRUE(socItem && cJSON_IsArray(socItem));
    EXPECT_TRUE(rgbItem && cJSON_IsArray(rgbItem));

    cJSON* normal = cJSON_GetObjectItemCaseSensitive(light, "normal");
    socItem = cJSON_GetObjectItemCaseSensitive(normal, "soc");
    rgbItem = cJSON_GetObjectItemCaseSensitive(normal, "rgb");
    EXPECT_TRUE(socItem && cJSON_IsArray(socItem));
    EXPECT_TRUE(rgbItem && cJSON_IsArray(rgbItem));

    cJSON* high = cJSON_GetObjectItemCaseSensitive(light, "high");
    socItem = cJSON_GetObjectItemCaseSensitive(high, "soc");
    rgbItem = cJSON_GetObjectItemCaseSensitive(high, "rgb");
    EXPECT_TRUE(socItem && cJSON_IsArray(socItem));
    EXPECT_TRUE(rgbItem && cJSON_IsArray(rgbItem));
}

/**
 * @tc.name: VendorBatteryConfigTest002
 * @tc.desc: Test vendor component reading charger node from battery_config.json
 * @tc.type: Compatibility
 */
HWTEST_F(HdfBatteryConfigTest, VendorBatteryConfigTest002, TestSize.Level1)
{
    char buf[MAX_PATH_LEN] = {0};
    char* path = GetOneCfgFile(BATTERY_CONFIG_PATH, buf, MAX_PATH_LEN);
    ASSERT_TRUE(path != nullptr);
    std::ifstream ifsConf {path};
    std::string content {std::istreambuf_iterator<char>(ifsConf), std::istreambuf_iterator<char>()};
    ASSERT_FALSE(content.empty());
    ifsConf.close();

    std::unique_ptr<cJSON, decltype(&cJSON_Delete)> rootJson {cJSON_Parse(content.c_str()), &cJSON_Delete};

    cJSON* charger = cJSON_GetObjectItemCaseSensitive(rootJson.get(), "charger");
    cJSON* current_limit = cJSON_GetObjectItemCaseSensitive(charger, "current_limit");
    cJSON* voltage_limit = cJSON_GetObjectItemCaseSensitive(charger, "voltage_limit");
    cJSON* type = cJSON_GetObjectItemCaseSensitive(charger, "type");
    for (cJSON* toCheck : {current_limit, voltage_limit, type}) {
        cJSON* path = cJSON_GetObjectItemCaseSensitive(toCheck, "path");
        EXPECT_TRUE(cJSON_IsString(path) && path->valuestring);
    }
}

/**
 * @tc.name: VendorBatteryConfigTest003
 * @tc.desc: Test vendor component reading charge_scene node
 * @tc.type: Compatibility
 */
HWTEST_F(HdfBatteryConfigTest, VendorBatteryConfigTest003, TestSize.Level1)
{
    char buf[MAX_PATH_LEN] = {0};
    char* path = GetOneCfgFile(BATTERY_CONFIG_PATH, buf, MAX_PATH_LEN);
    ASSERT_TRUE(path != nullptr);
    std::ifstream ifsConf {path};
    std::ifstream ifsSplitConf {VENDOR_BATTERY_SPLIT_CONFIG_PATH};
    std::string content {};
    // check split config file if it exists, otherwise the configs should be present in battery_config.json
    content = std::string {std::istreambuf_iterator<char>(ifsSplitConf.is_open() ? ifsSplitConf : ifsConf),
        std::istreambuf_iterator<char>()};
    ASSERT_FALSE(content.empty());
    std::unique_ptr<cJSON, decltype(&cJSON_Delete)> rootJson {cJSON_Parse(content.c_str()), &cJSON_Delete};

    cJSON* charge_scene = cJSON_GetObjectItemCaseSensitive(rootJson.get(), "charge_scene");
    // considering impl of cJSON the nullptr check is not needed but we add it here to give a hint to failure reason
    ASSERT_FALSE(IsEmptyJsonParse(charge_scene));
    // Below is the format pattern of "charge_scene" mode
    const std::unordered_map<std::string, std::set<std::string>> items {
        {"support", {"path", "type", "expect_value"}},
        {"get",     {"path"}                        },
        {"set",     {"path"}                        }
    };
    cJSON* valueObj = nullptr;
    cJSON_ArrayForEach(valueObj, charge_scene) {
        // the sub node should not be empty and should have a name.
        EXPECT_TRUE(valueObj->string != nullptr);
        ASSERT_FALSE(IsEmptyJsonParse(valueObj));
        for (const auto& [node, childs] : items) {
            cJSON* curNode = cJSON_GetObjectItemCaseSensitive(valueObj, node.c_str());
            // may contain empty entry
            if (!curNode) {
                continue;
            }
            for (const auto& entry : childs) {
                cJSON* cur = cJSON_GetObjectItemCaseSensitive(
                    cJSON_GetObjectItemCaseSensitive(valueObj, node.c_str()), entry.c_str());
                EXPECT_TRUE(cJSON_IsString(cur) && cur->valuestring);
            }
        }
    }
}

/**
 * @tc.name: VendorBatteryConfigTest004
 * @tc.desc: Test vendor component reading uevent node
 * @tc.type: Compatibility
 */
HWTEST_F(HdfBatteryConfigTest, VendorBatteryConfigTest004, TestSize.Level1)
{
    char buf[MAX_PATH_LEN] = {0};
    char* path = GetOneCfgFile(BATTERY_CONFIG_PATH, buf, MAX_PATH_LEN);
    ASSERT_TRUE(path != nullptr);
    std::ifstream ifsConf {path};
    std::ifstream ifsSplitConf {VENDOR_BATTERY_SPLIT_CONFIG_PATH};
    std::string content {};
    content = std::string {std::istreambuf_iterator<char>(ifsSplitConf.is_open() ? ifsSplitConf : ifsConf),
        std::istreambuf_iterator<char>()};
    ASSERT_FALSE(content.empty());
    std::unique_ptr<cJSON, decltype(&cJSON_Delete)> rootJson {cJSON_Parse(content.c_str()), &cJSON_Delete};

    cJSON* uevent = cJSON_GetObjectItemCaseSensitive(rootJson.get(), "uevent");
    ASSERT_FALSE(IsEmptyJsonParse(uevent));
    cJSON* outer = nullptr;
    cJSON_ArrayForEach(outer, uevent) {
        EXPECT_TRUE(outer->string != nullptr);
        cJSON* inner = nullptr;
        cJSON_ArrayForEach(inner, outer)
        {
            EXPECT_TRUE(inner->string != nullptr);
            EXPECT_TRUE(cJSON_IsString(inner));
        }
    }
}
} // namespace
