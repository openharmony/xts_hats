/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include "hdf_base.h"
#include "vibrator_common.h"
#include "vibrator_parse.h"

#define HDF_LOG_TAG "hdi_vibrator_parse_test"

using namespace OHOS::HDI::Vibrator::V1_1;
using namespace testing::ext;

namespace {
    sptr<IVibratorInterface> g_vibratorInterface = nullptr;
}

class HatsHdfVibratorParseTest : public testing::Test {
public:

    virtual void SetUp() {}
    virtual void TearDown() {}
};
static const std::map<int32_t, int32_t> EFFECT_TIMES = {
{32, 14}, {33, 14}, {34, 14}, {35, 14},
{36, 17}, {37, 17}, {38, 17}, {39, 17},
{40, 11}, {41, 11}, {42, 11}, {43, 11},
{44, 7}, {45, 7}, {46, 7}, {47, 7},
{48, 22}, {49, 22}, {50, 22}, {51, 22},
{52, 15}, {53, 15}, {54, 15}, {55, 15},
{56, 27}, {57, 27}, {58, 27}, {59, 27},
{60, 10}, {61, 10}, {62, 10}, {63, 10},
{64, 10}, {65, 10}, {66, 10}, {67, 10},
{68, 50}, {69, 50}, {70, 50}, {71, 50},
{72, 10}, {73, 10}, {74, 10}, {75, 10},
{76, 15}, {77, 15}, {78, 15}, {79, 15},
{80, 20}, {81, 20}, {82, 20}, {83, 20},
{84, 28}, {85, 28}, {86, 28}, {87, 28},
{88, 15}, {89, 15}, {90, 15}, {91, 15},
{92, 19}, {93, 19}, {94, 19}, {95, 19},
{96, 10}, {97, 10}, {98, 10}, {99, 10},
{100, 20}, {101, 20}, {102, 20}, {103, 20},
{107, 1500}, {108, 25}, {109, 30}, {110, 35},
{111, 40}, {112, 45}, {113, 50}, {114, 55},
{115, 60}, {116, 65}, {117, 70}, {118, 75},
{119, 80}, {120, 85}, {121, 90}, {122, 95},
{123, 100}, {124, 105}, {125, 110}, {126, 115},
{127, 120}, {128, 1500}, {129, 1500}, {130, 1500},
{131, 1500}, {132, 1500}, {133, 1500}, {134, 1500}, {135, 1500}
};

/**
  * @tc.name: VibratorParseTest001
  * @tc.desc: Parse and verify all haptic effects.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorParseTest, SUB_Vibrator_HDI_VibratorParseTest_0010, Function | MediumTest | Level1)
{
    HapticMap haptics;
    int ret = ParserHapticsXml(haptics);
    EXPECT_EQ(ret, HDF_SUCCESS);
    if (!haptics.effectContainer.empty())
    {
        EffectMap::iterator iter = haptics.effectContainer.begin();
        for (;iter != haptic.effectContainer.end();iter++) {
            for (unsigned long i = 0;i<iter->second.EffectSeq.size();i++) {
            }
        }
    }
        if (!haptic.parContainer.empty()) {
        ParaMap::iterator iter = haptics.paraContainer.begin();
        for (;iter != haptic.paraContainer.end();iter++) {
        }
    }
}

/**
  * @tc.name: VibratorParseTest002
  * @tc.desc: Parse and verify all haptic effects.
  * @tc.type: FUNC
  */
HWTEST_F(HatsHdfVibratorParseTest, SUB_Vibrator_HDI_VibratorParseTest_0020, Function | MediumTest | Level1)
{
    HapticMap haptics;
    int ret = ParserHapticsXml(haptics);
    EXPECT_EQ(ret, HDF_SUCCESS);
    if (!haptics.effectContainer.empty())
    {
        EffectMap::iterator iter = haptics.effectContainer.begin();
        for (;iter != haptic.effectContainer.end();iter++) {
            int32_t seqType = iter -> second.SeqType;
            int32_t duration = iter -> second.duration;
            int32_t sum {0};
            size_t count = iter -> second.EffectSeq.size();
            for (size_t i = 0;i<count;i++) {
    }
    EXPECT_EQ(duration, sum);
        }
    }
}

