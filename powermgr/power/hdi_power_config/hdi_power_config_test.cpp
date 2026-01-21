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
#include <cJSON.h>
#include <config_policy_utils.h>
#include <fstream>
#include <gtest/gtest.h>
#include <hdf_core_log.h>

namespace {
constexpr const char* POWER_CONFIG_PATH = "etc/power_config/power_config.json";
class HdfPowerConfigTest : public testing::Test {
public:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() {}
    void TearDown() {}
};

/**
 * @tc.name: HdfPowerConfigTest001
 * @tc.desc: Test vendor component reading uevent node
 * @tc.type: Compatibility
 */
HWTEST_F(HdfPowerConfigTest, HdfPowerConfigTest001, testing::ext::TestSize.Level1)
{
    HDF_LOGI("HdfPowerConfigTest start");
    char buf[MAX_PATH_LEN];
    char* path = GetOneCfgFile(POWER_CONFIG_PATH, buf, MAX_PATH_LEN);
    if (path) {
        std::ifstream ifsConf {path};
        ASSERT_TRUE(ifsConf.is_open());
        std::string content {std::istreambuf_iterator<char>(ifsConf), std::istreambuf_iterator<char>()};
        ASSERT_FALSE(content.empty());

        std::unique_ptr<cJSON, decltype(&cJSON_Delete)> rootJson {cJSON_Parse(content.c_str()), &cJSON_Delete};
        ASSERT_TRUE(rootJson != nullptr);
        cJSON* scene = cJSON_GetObjectItemCaseSensitive(rootJson.get(), "scene");
        ASSERT_TRUE(scene != nullptr);
        cJSON* wakeuo_cause = cJSON_GetObjectItemCaseSensitive(scene, "wakeuo_cause");
        ASSERT_TRUE(wakeuo_cause != nullptr);
        cJSON* get = cJSON_GetObjectItemCaseSensitive(wakeuo_cause, "get");
        ASSERT_TRUE(get != nullptr);
        cJSON* getPath = cJSON_GetObjectItemCaseSensitive(get, "path");
        EXPECT_TRUE(cJSON_IsString(getPath) && getPath->valuestring);
    } else {
        SUCCEED() << "WARNING: the config file was not found, this could be a problem if the device is PC";
    }
    HDF_LOGI("HdfPowerConfigTest end");
}
} // namespace