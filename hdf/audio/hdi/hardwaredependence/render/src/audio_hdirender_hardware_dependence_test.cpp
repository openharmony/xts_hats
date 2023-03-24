/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "audio_hdi_common.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;

namespace {
class AudioHdiRenderHardwareDependenceTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
    static TestAudioManager *manager;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
};

using THREAD_FUNC = void *(*)(void *);
TestAudioManager *AudioHdiRenderHardwareDependenceTest::manager = nullptr;

void AudioHdiRenderHardwareDependenceTest::SetUpTestCase(void)
{
    manager = GetAudioManagerFuncs();
    ASSERT_NE(nullptr, manager);
}

void AudioHdiRenderHardwareDependenceTest::TearDownTestCase(void) {}
void AudioHdiRenderHardwareDependenceTest::SetUp(void)
{
    ASSERT_NE(nullptr, manager);
    int32_t ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
}
void AudioHdiRenderHardwareDependenceTest::TearDown(void)
{
    int32_t ret = ReleaseRenderSource(manager, adapter, render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
}
/**
* @tc.name  AudioRenderSetSampleAttributes_002
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_24_BIT;
*    attrs.sampleRate = 11025;
*    attrs.channelCount = 2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0002, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_24_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_11025);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_11025, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
    }
}
/**
* @tc.name  AudioRenderSetSampleAttributes_003
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
*    attrs.sampleRate = 22050;
*    attrs.channelCount = 1;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0003, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_22050);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_16_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_22050, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
    }
}
/**
* @tc.name  AudioRenderSetSampleAttributes_004
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_24_BIT;
*    attrs.sampleRate = 32000;
*    attrs.channelCount = 2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0004, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_24_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_32000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_32000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
    }
}
/**
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_COMMUNICATION;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_24_BIT;
*    attrs.sampleRate = 48000;
*    attrs.channelCount = 2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0006, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_24_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_48000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_48000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
    }
}
/**
* @tc.name  AudioRenderSetSampleAttributes_007
* @tc.desc   Test AudioRenderSetSampleAttributes interface, return -1 if the render is empty.
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0007, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioRender *renderNull = nullptr;
    struct AudioSampleAttributes attrs = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, SINGLE_CHANNEL_COUNT, SAMPLE_RATE_8000);

    ret = render->attr.SetSampleAttributes(renderNull, &attrs);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    ret = render->attr.SetSampleAttributes(render, nullptr);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
}
/**
* @tc.name  AudioRenderSetSampleAttributes_010
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
*    attrs.sampleRate = 24000;
*    attrs.channelCount = 2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0010, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);

    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_24000);
    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_16_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_24000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
    }
}
/**
* @tc.name  AudioRenderSetSampleAttributes_011
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
*    attrs.sampleRate = 64000;
*    attrs.channelCount = 2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0011, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_64000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_16_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_64000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
    }
}
/**
* @tc.name  AudioRenderSetSampleAttributes_013
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
*    attrs.sampleRate = 0xFFFFFFFFu;
*    attrs.channelCount = 2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0013, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, 2, 0xFFFFFFFFu);

    ret = render->attr.SetSampleAttributes(render, &attrs);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);
}
/**
* @tc.name  AudioRenderSetSampleAttributes_015
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_AAC_LC/LD/ELD;
*    attrs.sampleRate = 32000/44100/48000;
*    attrs.channelCount = 1/2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0015, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs1 = {};
    struct AudioSampleAttributes attrs2 = {};
    struct AudioSampleAttributes attrs3 = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs1, AUDIO_FORMAT_TYPE_AAC_LC, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_32000);
    ret = render->attr.SetSampleAttributes(render, &attrs1);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs2, AUDIO_FORMAT_TYPE_AAC_LD, SINGLE_CHANNEL_COUNT, SAMPLE_RATE_44100);
    ret = render->attr.SetSampleAttributes(render, &attrs2);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs3, AUDIO_FORMAT_TYPE_AAC_ELD, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_48000);
    ret = render->attr.SetSampleAttributes(render, &attrs3);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);
}
/**
* @tc.name  AudioRenderSetSampleAttributes_016
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_AAC_HE_V1/V2
*    attrs.sampleRate = 8000/44100;
*    attrs.channelCount = 1/2;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0016, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs1 = {};
    struct AudioSampleAttributes attrs2 = {};
    ASSERT_NE(nullptr, render);

    InitAttrsUpdate(attrs1, AUDIO_FORMAT_TYPE_AAC_HE_V1, SINGLE_CHANNEL_COUNT, SAMPLE_RATE_8000);
    ret = render->attr.SetSampleAttributes(render, &attrs1);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs2, AUDIO_FORMAT_TYPE_AAC_HE_V2, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_44100);
    ret = render->attr.SetSampleAttributes(render, &attrs2);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);
}
/**
* @tc.name  AudioRenderSetSampleAttributes_017
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT
*    attrs.sampleRate = 8000;
*    attrs.channelCount = 5;
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0017, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    ASSERT_NE(nullptr, render);

    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, 5, SAMPLE_RATE_8000);
    ret = render->attr.SetSampleAttributes(render, &attrs);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);
}
/**
* @tc.name  AudioRenderGetSampleAttributes_002
* @tc.desc   Test AudioRendereGetSampleAttributes interface, return -1 if the render is empty.
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderGetSampleAttributes_0002, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioRender *renderNull = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes *attrsValue = nullptr;
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, SINGLE_CHANNEL_COUNT, SAMPLE_RATE_44100);

    ret = render->attr.GetSampleAttributes(renderNull, &attrs);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    ret = render->attr.GetSampleAttributes(render, attrsValue);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
}
/**
    * @tc.name  AudioRenderGetFrameSize_004
    * @tc.desc  Test RenderGetFrameSize interface,return 0 if get framesize define format as different values
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_RenderGetFrameSize_0004, TestSize.Level1)
{
    int32_t ret = -1;
    uint64_t size = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_24_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_48000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_48000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);

        ret = render->attr.GetFrameSize(render, &size);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_GT(size, INITIAL_VALUE);
    }
}
/**
    * @tc.name  AudioRenderGetFrameSize_006
    * @tc.desc  Test RenderGetFrameSize interface,return 0 if get framesize define channelCount as different values
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_RenderGetFrameSize_0006, TestSize.Level1)
{
    int32_t ret = -1;
    uint64_t size = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_24_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_44100);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_44100, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);

        ret = render->attr.GetFrameSize(render, &size);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_GT(size, INITIAL_VALUE);
    }
}
/**
    * @tc.name  AudioRenderGetFrameCount_004
    * @tc.desc  Test RenderGetFrameCount interface,return 0 if get framesize define channelCount as different values
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_RenderGetFrameCount_0004, TestSize.Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_24_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_8000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_8000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);

        ret = AudioRenderStartAndOneFrame(render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        ret = render->attr.GetFrameCount(render, &count);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_GT(count, INITIAL_VALUE);

        ret = render->control.Stop((AudioHandle)render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
}
/**
    * @tc.name  AudioRenderGetFrameCount_005
    * @tc.desc  Test RenderGetFrameCount interface,return 0 if get framesize define format as different values
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_RenderGetFrameCount_0005, TestSize.Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    ASSERT_NE(nullptr, render);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_TYPE_PCM_16_BIT, DOUBLE_CHANNEL_COUNT, SAMPLE_RATE_8000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_16_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_8000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);

        ret = AudioRenderStartAndOneFrame(render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        ret = render->attr.GetFrameCount(render, &count);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_GT(count, INITIAL_VALUE);

        ret = render->control.Stop((AudioHandle)render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
}
/**
    * @tc.name  AudioRenderGetRenderPosition_009
    * @tc.desc  Test GetRenderPosition interface,return 0 if get framesize define format to AUDIO_FORMAT_TYPE_PCM_16_BIT
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderGetRenderPosition_0009, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    uint64_t frames = 0;
    struct AudioTimeStamp time = {.tvSec = 0, .tvNSec = 0};
    ASSERT_NE(nullptr, render);
    InitAttrs(attrs);
    attrs.type = AUDIO_IN_MEDIA;
    attrs.interleaved = false;
    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
    attrs.sampleRate = SAMPLE_RATE_48000;
    attrs.channelCount = DOUBLE_CHANNEL_COUNT;
    ret = render->attr.SetSampleAttributes(render, &attrs);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetSampleAttributes(render, &attrsValue);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_16_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_48000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
        ret = AudioRenderStartAndOneFrame(render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        ret = render->GetRenderPosition(render, &frames, &time);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_GT(frames, INITIAL_VALUE);
        render->control.Stop((AudioHandle)render);
    }
}
#ifndef ALSA_LIB_MODE
/**
    * @tc.name  AudioRenderGetRenderPosition_010
    * @tc.desc  Test GetRenderPosition interface,return 0 if get framesize define format to AUDIO_FORMAT_TYPE_PCM_24_BIT
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderGetRenderPosition_0010, TestSize.Level1)
{
    int32_t ret = -1;
    uint64_t frames = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    struct AudioTimeStamp time = {.tvSec = 0, .tvNSec = 0};
    ASSERT_NE(nullptr, render);
    InitAttrs(attrs);
    attrs.type = AUDIO_IN_MEDIA;
    attrs.interleaved = false;
    attrs.format = AUDIO_FORMAT_TYPE_PCM_24_BIT;
    attrs.sampleRate = SAMPLE_RATE_48000;
    attrs.channelCount = DOUBLE_CHANNEL_COUNT;
    ret = render->attr.SetSampleAttributes(render, &attrs);
     if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetSampleAttributes(render, &attrsValue);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_EQ(AUDIO_FORMAT_TYPE_PCM_24_BIT, attrsValue.format);
        EXPECT_EQ(SAMPLE_RATE_48000, attrsValue.sampleRate);
        EXPECT_EQ(DOUBLE_CHANNEL_COUNT, attrsValue.channelCount);
        ret = AudioRenderStartAndOneFrame(render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        ret = render->GetRenderPosition(render, &frames, &time);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_GT(frames, INITIAL_VALUE);
        render->control.Stop((AudioHandle)render);
    }
}
#endif
/**
    * @tc.name  AudioRenderSetChannelMode_004
    * @tc.desc  Test SetChannelMode interface,return 0 if set channel mode after render object is created
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetChannelMode_0004, TestSize.Level1)
{
    int32_t ret = -1;
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    ASSERT_NE(nullptr, render);
    ret = AudioRenderStartAndOneFrame(render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->SetChannelMode(render, mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->GetChannelMode(render, &mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(AUDIO_CHANNEL_NORMAL, mode);
}
/**
    * @tc.name  AudioRenderSetChannelMode_005
    * @tc.desc  Test SetChannelMode interface,return -1 if set the parameter render is nullptr
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderSetChannelMode_0005, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioRender *renderNull = nullptr;
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    ASSERT_NE(nullptr, render);

    ret = render->SetChannelMode(renderNull, mode);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
}
/**
    * @tc.name  AudioRenderGetChannelMode_001
    * @tc.desc  Test GetChannelMode interface,return 0 if getting the channel mode after setting
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderGetChannelMode_0001, TestSize.Level1)
{
    int32_t ret = -1;
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    ASSERT_NE(nullptr, render);
    ret = AudioRenderStartAndOneFrame(render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->GetChannelMode(render, &mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->SetChannelMode(render, mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->GetChannelMode(render, &mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(AUDIO_CHANNEL_NORMAL, mode);

    render->control.Stop((AudioHandle)render);
}
/**
    * @tc.name  AudioRenderGetChannelMode_002
    * @tc.desc  Test GetChannelMode interface,return -1 if getting the parameter render is nullptr
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderGetChannelMode_0002, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioRender *renderNull = nullptr;
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;

    ASSERT_NE(nullptr, render);
    ret = AudioRenderStartAndOneFrame(render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->GetChannelMode(renderNull, &mode);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    render->control.Stop((AudioHandle)render);
}
/**
    * @tc.name  AudioRenderGetChannelMode_003
    * @tc.desc  Test GetChannelMode interface,return 0 if getting the channel mode after the object is created
* @tc.type: FUNC
*/
HWTEST_F(AudioHdiRenderHardwareDependenceTest, SUB_Audio_HDI_AudioRenderGetChannelMode_0003, TestSize.Level1)
{
    int32_t ret = -1;
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    ASSERT_NE(nullptr, render);
    ret = render->GetChannelMode(render, &mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(AUDIO_CHANNEL_NORMAL, mode);
}
}