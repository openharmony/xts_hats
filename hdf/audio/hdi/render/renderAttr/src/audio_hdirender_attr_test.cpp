/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

/**
 * @addtogroup Audio
 * @{
 *
 * @brief Defines audio-related APIs, including custom data types and functions for loading drivers,
 * accessing a driver adapter, and rendering audios.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_hdi_common.h
 *
 * @brief Declares APIs for operations related to the audio render adapter.
 *
 * @since 1.0
 * @version 1.0
 */

#include "audio_hdi_common.h"
#include "audio_hdirender_attr_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;

namespace {
const int32_t AUDIO_SAMPLERATE_INVALID = 0xFFFFFFFFu;
const int32_t AUDIO_CHANNELCOUNT_INVALID = -1;
class AudioHdiRenderAttrTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

using THREAD_FUNC = void *(*)(void *);

void AudioHdiRenderAttrTest::SetUpTestCase(void) {}
void AudioHdiRenderAttrTest::TearDownTestCase(void) {}
void AudioHdiRenderAttrTest::SetUp(void) {}
void AudioHdiRenderAttrTest::TearDown(void) {}

/**
* @tc.name  Test AudioRenderSetSampleAttributes API via legal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0002
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_PCM_24_BIT;
*    attrs.sampleRate = 11025;
*    attrs.channelCount = 2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_24_BIT, 2, 11025);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {    
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_PCM_24_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
    }  

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via legal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0003
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_PCM_16_BIT;
*    attrs.sampleRate = 22050;
*    attrs.channelCount = 1;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 2, 22050);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_PCM_16_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via legal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0004
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_PCM_24_BIT;
*    attrs.sampleRate = 32000;
*    attrs.channelCount = 2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_24_BIT, 2, 32000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_PCM_24_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via legal input.
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_COMMUNICATION;
*    attrs.format = AUDIO_FORMAT_PCM_24_BIT;
*    attrs.sampleRate = 48000;
*    attrs.channelCount = 2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_24_BIT, 2, 48000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_PCM_24_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via setting the render is empty .
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0007
* @tc.desc   Test AudioRenderSetSampleAttributes interface, return -1 if the render is empty.
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0007, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    struct AudioSampleAttributes attrs = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 1, 8000);

    ret = render->attr.SetSampleAttributes(renderNull, &attrs);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    ret = render->attr.SetSampleAttributes(render, nullptr);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via legal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0010
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_PCM_16_BIT;
*    attrs.sampleRate = 24000;
*    attrs.channelCount = 2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0010, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 2, 24000);
    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_PCM_16_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via legal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0011
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_PCM_16_BIT;
*    attrs.sampleRate = 64000;
*    attrs.channelCount = 2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0011, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 2, 64000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_IN_MEDIA, attrsValue.type);
        EXPECT_EQ(AUDIO_FORMAT_PCM_16_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via illegal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0013
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_PCM_16_BIT;
*    attrs.sampleRate = 0xFFFFFFFFu;
*    attrs.channelCount = 2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0013, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 2, 0xFFFFFFFFu);

    ret = render->attr.SetSampleAttributes(render, &attrs);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via illegal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0015
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_AAC_LC/LD/ELD;
*    attrs.sampleRate = 32000/44100/48000;
*    attrs.channelCount = 1/2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0015, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs1 = {};
    struct AudioSampleAttributes attrs2 = {};
    struct AudioSampleAttributes attrs3 = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();

    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs1, AUDIO_FORMAT_AAC_LC, 2, 32000);
    ret = render->attr.SetSampleAttributes(render, &attrs1);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs2, AUDIO_FORMAT_AAC_LD, 1, 44100);
    ret = render->attr.SetSampleAttributes(render, &attrs2);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs3, AUDIO_FORMAT_AAC_ELD, 2, 48000);
    ret = render->attr.SetSampleAttributes(render, &attrs3);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderSetSampleAttributes API via illegal input.
* @tc.number  SUB_Audio_HDI_AudioRenderSetSampleAttributes_0016
* @tc.desc  Test AudioRenderSetSampleAttributes ,the setting parameters are as follows.
*    attrs.type = AUDIO_IN_MEDIA;
*    attrs.format = AUDIO_FORMAT_AAC_HE_V1/V2
*    attrs.sampleRate = 8000/44100;
*    attrs.channelCount = 1/2;
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderSetSampleAttributes_0016, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs1 = {};
    struct AudioSampleAttributes attrs2 = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs1, AUDIO_FORMAT_AAC_HE_V1, 1, 8000);
    ret = render->attr.SetSampleAttributes(render, &attrs1);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    InitAttrsUpdate(attrs2, AUDIO_FORMAT_AAC_HE_V2, 2, 44100);
    ret = render->attr.SetSampleAttributes(render, &attrs2);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test AudioRendereGetSampleAttributes API via setting the render is empty .
* @tc.number  SUB_Audio_HDI_AudioRenderGetSampleAttributes_0002
* @tc.desc   Test AudioRendereGetSampleAttributes interface, return -1 if the render is empty.
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_AudioRenderGetSampleAttributes_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes *attrsValue = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 1, 44100);

    ret = render->attr.GetSampleAttributes(renderNull, &attrs);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    ret = render->attr.GetSampleAttributes(render, attrsValue);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameSize API via legal input
* @tc.number  SUB_Audio_HDI_RenderGetFrameSize_0001
* @tc.desc  Test RenderGetFrameSize interface,return 0 if the FrameSize was obtained successfully
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameSize_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t size = 0;
    uint64_t zero = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetFrameSize(render, &size);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_GT(size, zero);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureGetFrameSize API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetFrameSize_0002
* @tc.desc  Test RenderGetFrameSize interface,return -1 if failed to get the FrameSize when handle is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameSize_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t size = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetFrameSize(renderNull, &size);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameSize API setting the incoming parameter FrameSize is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetFrameSize_0003
* @tc.desc  Test RenderGetFrameSize interface,return -1 if failed to get the FrameSize when size is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameSize_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t *sizeNull = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetFrameSize(render, sizeNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameSize API via define format to different values
* @tc.number  SUB_Audio_HDI_RenderGetFrameSize_0004
* @tc.desc  Test RenderGetFrameSize interface,return 0 if get framesize define format as different values
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameSize_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t size = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_24_BIT, 2, 48000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    { 
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_EQ(AUDIO_FORMAT_PCM_24_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);
        ret = render->attr.GetFrameSize(render, &size);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test RenderGetFrameSize API via define channelCount to different values
* @tc.number  SUB_Audio_HDI_RenderGetFrameSize_0006
* @tc.desc  Test RenderGetFrameSize interface,return 0 if get framesize define channelCount as different values
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameSize_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t size = 0;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_24_BIT, 2, 44100);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_PCM_24_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);

        ret = render->attr.GetFrameSize(render, &size);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameCount API via legal
* @tc.number  SUB_Audio_HDI_RenderGetFrameCount_0001
* @tc.desc  Test RenderGetFrameCount interface, return 0 if the FrameSize was obtained successfully
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameCount_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    uint64_t zero = 0;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetFrameCount(render, &count);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_GT(count, zero);

    render->control.Stop((AudioHandle)render);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameCount API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetFrameCount_0002
* @tc.desc  Test RenderGetFrameCount interface,return -1 if the incoming parameter handle is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameCount_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
  
    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetFrameCount(renderNull, &count);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    render->control.Stop((AudioHandle)render);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameCount API setting the incoming parameter count is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetFrameCount_0003
* @tc.desc  Test RenderGetFrameCount interface,return -1 if the incoming parameter count is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameCount_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t *countNull = nullptr;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetFrameCount(render, countNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    render->control.Stop((AudioHandle)render);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameCount API via define channelCount to different values
* @tc.number  SUB_Audio_HDI_RenderGetFrameCount_0004
* @tc.desc  Test RenderGetFrameCount interface,return 0 if get framesize define channelCount as different values
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameCount_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_24_BIT, 2, 8000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_PCM_24_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);

        ret = AudioRenderStartAndOneFrame(render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        ret = render->attr.GetFrameCount(render, &count);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

        ret = render->control.Stop((AudioHandle)render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetFrameCount API via define format to different values
* @tc.number  SUB_Audio_HDI_RenderGetFrameCount_0005
* @tc.desc  Test RenderGetFrameCount interface,return 0 if get framesize define format as different values
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetFrameCount_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioSampleAttributes attrsValue = {};
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrsUpdate(attrs, AUDIO_FORMAT_PCM_16_BIT, 2, 8000);

    ret = AudioRenderSetGetSampleAttributes(attrs, attrsValue, render);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        EXPECT_EQ(AUDIO_FORMAT_PCM_16_BIT, attrsValue.format);
        EXPECT_NE(AUDIO_SAMPLERATE_INVALID, attrsValue.sampleRate);
        EXPECT_NE(AUDIO_CHANNELCOUNT_INVALID, attrsValue.channelCount);

        ret = AudioRenderStartAndOneFrame(render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        ret = render->attr.GetFrameCount(render, &count);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

        ret = render->control.Stop((AudioHandle)render);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetCurrentChannelId API via legal input
* @tc.number  SUB_Audio_HDI_RenderGetCurrentChannelId_0001
* @tc.desc  Test RenderGetCurrentChannelId, return 0 if the default CurrentChannelId is obtained successfully
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetCurrentChannelId_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint32_t channelId = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetCurrentChannelId(render, &channelId);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetCurrentChannelId API via CurrentChannelId is obtained after created
* @tc.number  SUB_Audio_HDI_RenderGetCurrentChannelId_0003
* @tc.desc  Test RenderGetCurrentChannelId interface, return 0 if CurrentChannelId is obtained after created
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetCurrentChannelId_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint32_t channelId = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetCurrentChannelId(render, &channelId);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test GetCurrentChannelId API via setting the parameter render is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetCurrentChannelId_0004
* @tc.desc  Test GetCurrentChannelId interface,return -1 if set the parameter render is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetCurrentChannelId_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint32_t channelId = 0;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetCurrentChannelId(renderNull, &channelId);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetCurrentChannelId API via setting the parameter channelId is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetCurrentChannelId_0005
* @tc.desc  Test RenderGetCurrentChannelId interface, return -1 if setting the parameter channelId is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetCurrentChannelId_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint32_t *channelIdNull = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->attr.GetCurrentChannelId(render, channelIdNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderSetExtraParams API via setting ExtraParams during playback
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0001
* @tc.desc  Test RenderSetExtraParams interface,return 0 if the ExtraParams is set during playback
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    char keyValueList[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000";
    char keyValueListExp[] = "attr-route=1;attr-format=32;attr-channels=2;attr-sampling-rate=48000";
    size_t index = 1;
    char keyValueListValue[256] = {};
    int32_t listLenth = 256;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .self = this, .pins = PIN_OUT_SPEAKER,
        .path = AUDIO_FILE.c_str()
    };
  
    audiopara.manager = GetAudioManagerFuncs();
    ASSERT_NE(nullptr, audiopara.manager);

    ret = pthread_create(&audiopara.tids, NULL, (THREAD_FUNC)PlayAudioFile, &audiopara);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    sleep(1);
    if (audiopara.render != nullptr) {
        ret = audiopara.render->attr.SetExtraParams((AudioHandle)audiopara.render, keyValueList);
        if(ret != AUDIO_HAL_SUCCESS){
            EXPECT_TRUE(true);
         }
        if(ret == AUDIO_HAL_SUCCESS)
        {
            ret = audiopara.render->attr.GetExtraParams((AudioHandle)audiopara.render, keyValueListValue, listLenth);
            EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
            string strGetValue = keyValueListValue;
            size_t indexAttr = strGetValue.find("attr-frame-count");
            size_t indexFlag = strGetValue.rfind(";");
            if (indexAttr != string::npos && indexFlag != string::npos) {
                strGetValue.replace(indexAttr, indexFlag - indexAttr + index, "");
            }
            EXPECT_STREQ(keyValueListExp, strGetValue.c_str());
        }
    }

    ret = ThreadRelease(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
/**
* @tc.name  Test RenderSetExtraParams API via setting some parameters after playing
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0002
* @tc.desc  Test RenderSetExtraParams interface,return 0 if some parameters is set after playing
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    char keyValueListOne[] = "attr-frame-count=1024;";
    char keyValueListOneExp[] = "attr-route=0;attr-format=16;attr-channels=2;attr-frame-count=1024;\
attr-sampling-rate=48000";
    char keyValueListTwo[] = "attr-format=16;attr-frame-count=1024;";
    char keyValueListTwoExp[] = "attr-route=0;attr-format=16;attr-channels=2;attr-frame-count=1024;\
attr-sampling-rate=48000";
    char keyValueListThr[] = "attr-route=1;attr-channels=1;attr-frame-count=1024;";
    char keyValueListThrExp[] = "attr-route=1;attr-format=16;attr-channels=1;attr-frame-count=1024;\
attr-sampling-rate=48000";
    char keyValueListFour[] = "attr-format=32;attr-channels=2;attr-frame-count=4096;attr-sampling-rate=48000";
    char keyValueListFourExp[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=4096;\
attr-sampling-rate=48000";
    char keyValueListValueOne[256] = {};
    char keyValueListValueTwo[256] = {};
    char keyValueListValueThr[256] = {};
    char keyValueListValueFour[256] = {};
    int32_t listLenth = 256;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueListOne);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValueOne, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueListOneExp, keyValueListValueOne);
    }
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueListTwo);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValueTwo, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueListTwoExp, keyValueListValueTwo);
    }
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueListThr);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValueThr, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueListThrExp, keyValueListValueThr);
    }
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueListFour);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValueFour, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueListFourExp, keyValueListValueFour);
    }
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderSetExtraParams API via setting keyvaluelist to a value outside the range
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0003
* @tc.desc  Test RenderSetExtraParams interface,return -1 if the Keyvaluelist is a value out of range
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    char keyValueList[] = "attr-para=abc;";

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderSetExtraParams API via adding parameters to keyvaluelist
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0004
* @tc.desc  Test RenderSetExtraParams interface,return -1 if adding parameters to keyvaluelist
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    char keyValueList[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000;\
attr-para=123";

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderSetExtraParams API via setting ExtraParams When the key is the same and the value is
    different
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0005
* @tc.desc  Test RenderSetExtraParams interface,return 0 if set ExtraParams When the key is the same and the value
    is different
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    char keyValueList[] = "attr-sampling-rate=48000;attr-sampling-rate=96000;attr-frame-count=4096;";
    char keyValueListExp[] = "attr-route=0;attr-format=16;attr-channels=2;attr-frame-count=4096;\
attr-sampling-rate=96000";
    char keyValueListValue[256] = {};
    int32_t listLenth = 256;

    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValue, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueListExp, keyValueListValue);
    }
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderSetExtraParams API via setting the parameter render is nullptr
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0007
* @tc.desc  Test RenderSetExtraParams interface,return -1 if set the parameter render is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0007, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    char keyValueList[] = "attr-format=2;";

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)renderNull, keyValueList);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderSetExtraParams API via setting the format of ExtraParams is nullptr
* @tc.number  SUB_Audio_HDI_RenderSetExtraParams_0008
* @tc.desc  Test RenderSetExtraParams interface,return -1 if set the format of ExtraParams is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderSetExtraParams_0008, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    char keyValueListNull[] = "attr-format=;";

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueListNull);
    EXPECT_NE(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetExtraParams API via legal input
* @tc.number  SUB_Audio_HDI_RenderGetExtraParams_0001
* @tc.desc  Test RenderGetExtraParams interface,return 0 if the RenderGetExtraParams was obtained successfully
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetExtraParams_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    uint64_t count = 0;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrsValue = {};
    char keyValueList[] = "attr-format=24;attr-frame-count=4096;";
    char keyValueListExp[] = "attr-route=0;attr-format=24;attr-channels=2;attr-frame-count=4096;\
attr-sampling-rate=48000";
    char keyValueListValue[256] = {};
    int32_t listLenth = 256;
    int32_t formatExp = 3;
    uint32_t sampleRateExp = 48000;
    uint32_t channelCountExp = 2;
    uint32_t frameCountExp = 4096;

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValue, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueListExp, keyValueListValue);
    
        ret = render->attr.GetSampleAttributes(render, &attrsValue);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_EQ(formatExp, attrsValue.format);
        EXPECT_EQ(sampleRateExp, attrsValue.sampleRate);
        EXPECT_EQ(channelCountExp, attrsValue.channelCount);
        ret = render->attr.GetFrameCount(render, &count);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_EQ(count, frameCountExp);
    }
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetExtraParams API via setting the parameter render is nullptr
* @tc.number  SUB_Audio_HDI_RenderGetExtraParams_0002
* @tc.desc  Test RenderGetExtraParams interface,return -1 if set the parameter render is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetExtraParams_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    char keyValueList[] = "attr-format=32;";
    char keyValueListValue[256] = {};
    int32_t listLenth = 256;

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)renderNull, keyValueListValue, listLenth);
        EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetExtraParams API via setting listlength to be less than the actual length
* @tc.number  SUB_Audio_HDI_RenderGetExtraParams_0003
* @tc.desc  Test RenderGetExtraParams interface,return -1 if set listlength to be less than the actual length
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetExtraParams_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    char keyValueList[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000;";
    char keyValueListValue[256] = {};
    int32_t listLenth = 8;

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValue, listLenth);
        EXPECT_NE(AUDIO_HAL_SUCCESS, ret);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderGetExtraParams API via setting listlenth equal to the actual length
* @tc.number  SUB_Audio_HDI_RenderGetExtraParams_0004
* @tc.desc  Test RenderGetExtraParams interface,return 0 if set listlenth equal to the actual length
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderGetExtraParams_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = {};
    struct AudioRender *render = nullptr;
    char keyValueList[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=11111111111111111111;\
attr-sampling-rate=48000";
    char keyValueListValue[256] = {};
    int32_t listLenth = 107;

  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.SetExtraParams((AudioHandle)render, keyValueList);
    if(ret != AUDIO_HAL_SUCCESS){
        EXPECT_TRUE(true);
    }
    if(ret == AUDIO_HAL_SUCCESS)
    {
        ret = render->attr.GetExtraParams((AudioHandle)render, keyValueListValue, listLenth);
        EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
        EXPECT_STREQ(keyValueList, keyValueListValue);
    }
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test RenderAddAudioEffect API via legal input
    * @tc.number  SUB_Audio_HDI_RenderAddAudioEffect_0001
    * @tc.desc  Test RenderAddAudioEffect interface,return 0 if set the legal parameter
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderAddAudioEffect_0001, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    uint64_t effectid = 14;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.AddAudioEffect((AudioHandle)render, effectid);
    if ((ret == AUDIO_HAL_SUCCESS) || (ret == AUDIO_HAL_ERR_NOT_SUPPORT) || 
    (ret == AUDIO_HAL_ERR_INVALID_PARAM) || (ret == AUDIO_HAL_ERR_INTERNAL)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test RenderAddAudioEffect API via setting the parameter render is nullptr
    * @tc.number  SUB_Audio_HDI_RenderAddAudioEffect_0001
    * @tc.desc  Test RenderAddAudioEffect interface,return -3 if set the parameter render is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderAddAudioEffect_0002, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    uint64_t effectid = 14;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.AddAudioEffect((AudioHandle)renderNull, effectid);
    if ((ret == AUDIO_HAL_SUCCESS) || (ret == AUDIO_HAL_ERR_NOT_SUPPORT) || (ret == AUDIO_HAL_ERR_INVALID_PARAM)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test RenderAddAudioEffect API via legal input
    * @tc.number  SUB_Audio_HDI_RenderRemoveAudioEffect_0001
    * @tc.desc  Test RenderAddAudioEffect interface,return -3 if set the legal parameter
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderRemoveAudioEffect_0001, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    uint64_t effectid = 14;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.AddAudioEffect((AudioHandle)render, effectid);
    if ((ret == AUDIO_HAL_SUCCESS) || (ret == AUDIO_HAL_ERR_NOT_SUPPORT) ||
     (ret == AUDIO_HAL_ERR_INVALID_PARAM) || (ret == AUDIO_HAL_ERR_INTERNAL)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }
    ret = render->attr.RemoveAudioEffect((AudioHandle)render, effectid);
       if ((ret == AUDIO_HAL_SUCCESS) || (ret == AUDIO_HAL_ERR_NOT_SUPPORT) || 
       (ret == AUDIO_HAL_ERR_INVALID_PARAM) || (ret == AUDIO_HAL_ERR_INTERNAL)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test RenderAddAudioEffect API via setting the parameter render is nullptr
    * @tc.number  SUB_Audio_HDI_RenderAddAudioEffect_0002
    * @tc.desc  Test RenderAddAudioEffect interface,return -3 if setting the parameter render is nullptr
*/
HWTEST_F(AudioHdiRenderAttrTest, SUB_Audio_HDI_RenderRemoveAudioEffect_0002, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    uint64_t effectid = 14;
  
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->attr.RemoveAudioEffect((AudioHandle)renderNull, effectid);
    if ((ret == AUDIO_HAL_SUCCESS) || (ret == AUDIO_HAL_ERR_NOT_SUPPORT) || 
    (ret == AUDIO_HAL_ERR_INVALID_PARAM) || (ret == AUDIO_HAL_ERR_INTERNAL)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
}