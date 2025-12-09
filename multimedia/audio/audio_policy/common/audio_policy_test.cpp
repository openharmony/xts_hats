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

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <mutex>
#include <unordered_set>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <hdf_core_log.h>
#include <fcntl.h>
#include <functional>
#include <securec.h>
#include <unistd.h>
#include <fstream>

#include "./../../../tools/audio_xml_parser.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS;

namespace {

static constexpr char CHIP_PROD_CONFIG_FILE[] = "/chip_prod/etc/audio/audio_policy_config.xml";
static constexpr char CONFIG_FILE[] = "/vendor/etc/audio/audio_policy_config.xml";

class HatsAudioPolicyTest : public testing::Test {
public:

    static void SetUpTestCase()
    {
        std::cout << "AudioConcurrencyPolicyStructuralTest: SetUpTestCase" << std::endl;
    }

    static void TearDownTestCase()
    {
        std::cout << "AudioConcurrencyPolicyStructuralTest: TearDownTestCase" << std::endl;
    }

    void SetUp() override
    {
        std::cout << "AudioConcurrencyPolicyStructuralTest: SetUp" << std::endl;
    }

    void TearDown() override
    {
        std::cout << "AudioConcurrencyPolicyStructuralTest: TearDown" << std::endl;
    }
};

bool FileExists(const std::string& filePath)
{
    std::ifstream file(filePath);
    return file.good();
}

/**
 * @tc.number: AUDIO_CONFIG_0100
 * @tc.name: AUDIO_CONFIG_0100
 * @tc.desc: Check whether there is an XML file.
 */
HWTEST_F(HatsAudioPolicyTest, AUDIO_CONFIG_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_CONFIG_0100" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();
    bool ret = FileExists(CHIP_PROD_CONFIG_FILE);
    if (!ret) {
        ret = FileExists(CONFIG_FILE);
    }
    ASSERT_TRUE(ret);
}

/**
 * @tc.number: AUDIO_POLICY_CONFIGURATION_0100
 * @tc.name: AUDIO_POLICY_CONFIGURATION_0100
 * @tc.desc: Check the file structure.
 */
HWTEST_F(HatsAudioPolicyTest, AUDIO_POLICY_CONFIGURATION_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_POLICY_CONFIGURATION_0200" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();
    std::unordered_set<std::string> validNodeNames_ = {
        "defaultOutput",
        "commonConfigs",
        "paConfigs"
    };
    if (curNode_->CompareName("audioPolicyConfiguration")) {
        std::string version;
        curNode_->GetProp("version", version);
        ASSERT_TRUE(!version.empty());
        ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
        while (curNode_->IsNodeValid()) {
            std::string name = curNode_->GetName();
            ASSERT_TRUE(!name.empty());
            if (name == "adapters") {
                ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
                EXPECT_EQ(curNode_->GetName(), "adapter");
            } else if (name == "volumeGroups" || name == "interruptGroups") {
                ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
                EXPECT_EQ(curNode_->GetName(), "groups");
            } else if (name == "globalConfigs") {
                ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
                ASSERT_TRUE(validNodeNames_.count(curNode_->GetName()));
            } else {
                ASSERT_TRUE(curNode_->IsNodeValid());
            }
        curNode_->MoveToNext();
        }
    }
}

/**
 * @tc.number: AUDIO_CONFIG_ADAPTERS_0100
 * @tc.name: AUDIO_CONFIG_ADAPTERS_0100
 * @tc.desc: Check adapters structure.
 */
HWTEST_F(HatsAudioPolicyTest, AUDIO_CONFIG_ADAPTERS_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_CONFIG_ADAPTERS_0100" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();
    std::unordered_set<std::string> validNodeNames_ = {
        "pipes",
        "devices"
    };
    if (curNode_->CompareName("adapters")) {
        ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
        std::string name = curNode_->GetName();
        ASSERT_TRUE(!name.empty());
        ASSERT_TRUE(validNodeNames_.count(curNode_->GetName()));
    }
}

/**
 * @tc.number: AUDIO_CONFIG_PIPES_0100
 * @tc.name: AUDIO_CONFIG_PIPES_0100
 * @tc.desc: Check pipes structure.
 */
HWTEST_F(HatsAudioPolicyTest, AUDIO_CONFIG_PIPES_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_CONFIG_PIPES_0100" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();
    std::unordered_set<std::string> validNodeNames_ = {
        "paProp",
        "streamProps",
        "attributes"
    };
    if (curNode_->CompareName("pipes")) {
        ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
        std::string name = curNode_->GetName();
        ASSERT_TRUE(!name.empty());
        ASSERT_TRUE(validNodeNames_.count(curNode_->GetName()));
    }
}

}