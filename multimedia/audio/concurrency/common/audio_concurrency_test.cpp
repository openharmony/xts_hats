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

static constexpr char AUDIO_CONCURRENCY_CONFIG_FILE[] = "/vendor/etc/audio/audio_concurrency_config.xml";

class MultimediaAudioTest : public testing::Test {
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

/**
 * @tc.number: AUDIO_CONFIG_0100
 * @tc.name: AUDIO_CONFIG_0100
 * @tc.desc: Check whether there is an XML file.
 */
HWTEST_F(MultimediaAudioTest, AUDIO_CONFIG_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_CONFIG_0100" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();
    int32_t ret = curNode_->Config(AUDIO_CONCURRENCY_CONFIG_FILE, nullptr, 0);
    EXPECT_EQ(0, ret);
}

/**
 * @tc.number: AUDIO_CONCURRENCY_POLICY_0100
 * @tc.name: AUDIO_CONCURRENCY_POLICY_0100
 * @tc.desc: Check audioConcurrencyPolicy structure.
 */
HWTEST_F(MultimediaAudioTest, AUDIO_CONCURRENCY_POLICY_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_CONCURRENCY_POLICY_0100" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();

    if (curNode_->CompareName("audioConcurrencyPolicy")) {
        std::string version;
        curNode_->GetProp("version", version);
        ASSERT_TRUE(!version.empty());
        ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
        EXPECT_EQ(curNode_->GetName(), "existingStream");
        ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
        EXPECT_EQ(curNode_->GetName(), "incomingStream");
    }
}

/**
 * @tc.number: AUDIO_CONFIG_EXISTING_STREAM_0100
 * @tc.name: AUDIO_CONFIG_EXISTING_STREAM_0100
 * @tc.desc: Check existingStream structure.
 */
HWTEST_F(MultimediaAudioTest, AUDIO_CONFIG_EXISTING_STREAM_0100, TestSize.Level2)
{
    std::cout << "begin AUDIO_CONFIG_EXISTING_STREAM_0100" << std::endl;
    std::shared_ptr<OHOS::AudioStandard::AudioXmlNode> curNode_ = OHOS::AudioStandard::AudioXmlNode::Create();
    std::unordered_set<std::string> validNodeAtion_ = {
        "play both",
        "concede incoming",
        "concede existing",
        "mix"
    };
    std::unordered_set<std::string> validNodeNames_ = {
        "primary out",
        "primary in",
        "fast out normal",
        "fast in normal",
        "offload out",
        "multichannel out",
        "direct out normal",
        "voip out",
        "voip in",
        "cellular call out",
        "cellular call in",
        "primary in AI",
        "primary in unprocess",
        "primary in ULTRASONIC"
    };
    if (curNode_->CompareName("existingStream")) {
        std::string existingStreamName;
        curNode_->GetProp("name", existingStreamName);
        ASSERT_TRUE(validNodeNames_.count(existingStreamName));
        ASSERT_TRUE(curNode_->GetChildrenNode() != nullptr);
        EXPECT_EQ(curNode_->GetName(), "incomingStream");
        std::string incoming;
        std::string action;
        curNode_->GetProp("name", incoming);
        ASSERT_TRUE(validNodeNames_.count(incoming));
        curNode_->GetProp("action", action);
        ASSERT_TRUE(validNodeAtion_.count(action));
        curNode_->MoveToNext();
    }
}

}