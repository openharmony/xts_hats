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
#include <string>
#include <hdf_core_log.h>

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
 * @tc.type: FUNC
 */
HWTEST_F(HdfBatteryConfigTest, HdfBatteryConfigTest004, TestSize.Level1)
{
    cJSON* temperature = cJSON_GetObjectItemCaseSensitive(config_, "temperature");
    cJSON* highItem = cJSON_GetObjectItemCaseSensitive(temperature, "high");
    cJSON* lowItem = cJSON_GetObjectItemCaseSensitive(temperature, "low");
    EXPECT_TRUE(highItem && cJSON_IsNumber(highItem));
    EXPECT_TRUE(lowItem && cJSON_IsNumber(lowItem));
}
}
