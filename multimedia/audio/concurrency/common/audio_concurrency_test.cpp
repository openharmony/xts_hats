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
#include "./../../../tools/audio_concurrency_parser.h"

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
}