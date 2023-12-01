/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "osal_mem.h"
#include <gtest/gtest.h>

#include "v1_0/audio_types.h"
#include "v1_0/iaudio_manager.h"
#include "v1_0/iaudio_render.h"

using namespace std;
using namespace testing::ext;

namespace {
const int BUFFER_LENTH = 1024 * 16;
const int DEEP_BUFFER_RENDER_PERIOD_SIZE = 4 * 1024;
const int MOVE_LEFT_NUM = 8;
const int32_t AUDIO_RENDER_CHANNELCOUNT = 2;
const int32_t AUDIO_SAMPLE_RATE_48K = 48000;
const int32_t MAX_AUDIO_ADAPTER_DESC = 5;
const uint64_t DEFAULT_BUFFER_SIZE = 16384;
const int32_t AUDIO_RENDER_BUF_TEST = 1024;

class AudioUtRenderTestAdditional : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct AudioAdapterDescriptor descs_[MAX_AUDIO_ADAPTER_DESC];
    struct AudioAdapterDescriptor *desc_;
    struct IAudioAdapter *adapter_ = nullptr;
    struct IAudioRender *render_ = nullptr;
    struct AudioDeviceDescriptor devDescRender_ = {};
    struct AudioSampleAttributes attrsRender_ = {};
    uint32_t renderId_ = 0;
    uint32_t size_ = MAX_AUDIO_ADAPTER_DESC;
    virtual void SetUp();
    virtual void TearDown();
    uint64_t GetRenderBufferSize();
    void InitRenderAttrs(struct AudioSampleAttributes &attrs);
    void InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

uint64_t AudioUtRenderTestAdditional::GetRenderBufferSize()
{
    int32_t ret = HDF_SUCCESS;
    uint64_t frameSize = 0;
    uint64_t frameCount = 0;
    uint64_t bufferSize = 0;

    if (render_ == nullptr) {
        return DEFAULT_BUFFER_SIZE;
    }

    ret = render_->GetFrameSize(render_, &frameSize);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }

    ret = render_->GetFrameCount(render_, &frameCount);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }

    bufferSize = frameCount * frameSize;
    if (bufferSize == 0) {
        bufferSize = DEFAULT_BUFFER_SIZE;
    }

    return bufferSize;
}

void AudioUtRenderTestAdditional::InitRenderAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.channelCount = AUDIO_RENDER_CHANNELCOUNT;
    attrs.sampleRate = AUDIO_SAMPLE_RATE_48K;
    attrs.interleaved = 0;
    attrs.type = AUDIO_IN_MEDIA;
    attrs.period = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    attrs.frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * AUDIO_RENDER_CHANNELCOUNT / MOVE_LEFT_NUM;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM);
    attrs.stopThreshold = INT_MAX;
    attrs.silenceThreshold = BUFFER_LENTH;
}

void AudioUtRenderTestAdditional::InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = PIN_OUT_SPEAKER;
    devDesc.desc = strdup("cardname");

    ASSERT_NE(desc_, nullptr);
    ASSERT_NE(desc_->ports, nullptr);
    for (uint32_t index = 0; index < desc_->portsLen; index++) {
        if (desc_->ports[index].dir == PORT_OUT) {
            devDesc.portId = desc_->ports[index].portId;
            return;
        }
    }
}

void AudioUtRenderTestAdditional::FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
{
    if (dataBlock == nullptr) {
        return;
    }

    OsalMemFree(dataBlock->adapterName);

    OsalMemFree(dataBlock->ports);

    if (freeSelf) {
        OsalMemFree(dataBlock);
    }
}

void AudioUtRenderTestAdditional::ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen)
{
    if (descs == nullptr || descsLen == 0) {
        return;
    }

    for (uint32_t i = 0; i < descsLen; i++) {
        FreeAdapterElements(&descs[i], false);
    }
}

void AudioUtRenderTestAdditional::SetUp()
{
    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    ASSERT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, descs_, &size_));
    ASSERT_NE(descs_, nullptr);
    EXPECT_GE(MAX_AUDIO_ADAPTER_DESC, size_);
    desc_ = &descs_[0];
    ASSERT_EQ(HDF_SUCCESS, manager_->LoadAdapter(manager_, desc_, &adapter_));
    ASSERT_NE(adapter_, nullptr);
    InitRenderDevDesc(devDescRender_);
    InitRenderAttrs(attrsRender_);

    attrsRender_.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
    int32_t ret = adapter_->CreateRender(adapter_, &devDescRender_, &attrsRender_, &render_, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrsRender_.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        ASSERT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devDescRender_, &attrsRender_, &render_, &renderId_));
    }
    ASSERT_NE(render_, nullptr);
}

void AudioUtRenderTestAdditional::TearDown()
{
    if (adapter_ != nullptr) {
        adapter_->DestroyRender(adapter_, renderId_);
        render_ = nullptr;
    }
    if (manager_ != nullptr) {
        manager_->UnloadAdapter(manager_, desc_->adapterName);
        adapter_ = nullptr;
        ReleaseAllAdapterDescs(descs_, size_);

        IAudioManagerRelease(manager_, false);
    }
}

/**
 * @tc.number : SUB_Driver_Audio_GetLatency_0400
 * @tc.name   : testCommonRenderGetLatency001
 * @tc.desc   : Reliability of function(GetLatency)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetLatency001, Function | MediumTest | Level1)
{
    uint32_t ms = 0;
    int i, ret = 0;
    for (i = 0; i < 1000; i++) {
        ret |= render_->GetLatency(render_, &ms);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Audio_RenderFrame_0300
 * @tc.name   : testCommonRenderRenderFrame001
 * @tc.desc   : Functionality of function(RenderFrame)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRenderFrame001, Function | MediumTest | Level1)
{
    uint64_t frameLen = (uint64_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = render_->RenderFrame(render_, frame, frameLen, &requestBytes);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_RenderFrame_0400
 * @tc.name   : testCommonRenderRenderFrame002
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRenderFrame002, Function | MediumTest | Level1)
{
    uint64_t frameLen = (uint64_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;

    int32_t ret = render_->Start(render_);
    int i = 0;
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);
    for (i = 0; i < 1000; i++) {
        ret |= render_->RenderFrame(render_, frame, frameLen, &requestBytes);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_RenderFrame_0500
 * @tc.name   : testCommonRenderRenderFrame003
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRenderFrame003, Function | MediumTest | Level2)
{
    uint64_t frameLen = (uint64_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = render_->RenderFrame(nullptr, frame, frameLen, &requestBytes);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_RenderFrame_0600
 * @tc.name   : testCommonRenderRenderFrame004
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRenderFrame004, Function | MediumTest | Level2)
{
    uint64_t frameLen = (uint64_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = render_->RenderFrame(render_, nullptr, frameLen, &requestBytes);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_RenderFrame_0700
 * @tc.name   : testCommonRenderRenderFrame005
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRenderFrame005, Function | MediumTest | Level2)
{
    uint64_t frameLen = (uint64_t)GetRenderBufferSize();

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = render_->RenderFrame(render_, frame, frameLen, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_GetRenderPosition_0600
 * @tc.name   : testCommonRenderGetRenderPosition001
 * @tc.desc   : Reliability of function(GetRenderPosition)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetRenderPosition001, Function | MediumTest | Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvSec = 0;
    time.tvNSec = 0;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret |= render_->GetRenderPosition(render_, &frames, &time);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_SetRenderSpeed_0300
 * @tc.name   : testCommonRenderSetRenderSpeed001
 * @tc.desc   : Reliability of function(SetRenderSpeed)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetRenderSpeed001, Function | MediumTest | Level1)
{
    float speed = 2.0;
    int i, ret = 0;
    for (i = 0; i < 1000; i++) {
        ret |= render_->SetRenderSpeed(render_, speed);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Audio_SetRenderSpeed_0400
 * @tc.name   : testCommonRenderSetRenderSpeed002
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetRenderSpeed002, Function | MediumTest | Level2)
{
    float speed = -1.0;
    EXPECT_NE(HDF_SUCCESS, render_->SetRenderSpeed(render_, speed));
}

/**
 * @tc.number : SUB_Driver_Audio_SetRenderSpeed_0500
 * @tc.name   : testCommonRenderSetRenderSpeed003
 * @tc.desc   : Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetRenderSpeed003, Function | MediumTest | Level2)
{
    float speed = 0;
    EXPECT_NE(HDF_SUCCESS, render_->SetRenderSpeed(render_, speed));
}

/**
 * @tc.number : SUB_Driver_Audio_GetRenderSpeed_0300
 * @tc.name   : testCommonRenderGetRenderSpeed001
 * @tc.desc   : Reliability of function(GetRenderSpeed)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetRenderSpeed001, Function | MediumTest | Level1)
{
    float speed = 0.0;
    int32_t ret = 0;
    int i = 0;
    ASSERT_EQ(HDF_SUCCESS, render_->Start(render_));
    for (i = 0; i < 1000; i++) {
        ret = render_->GetRenderSpeed(render_, &speed);
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0300
 * @tc.name   : testCommonRenderSetChannelMode001
 * @tc.desc   : Reliability of function(SetChannelMode)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode001, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetChannelMode(render_, mode);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0400
 * @tc.name   : testCommonRenderSetChannelMode002
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode002, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_BOTH_LEFT;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0500
 * @tc.name   : testCommonRenderSetChannelMode003
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode003, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_BOTH_RIGHT;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0600
 * @tc.name   : testCommonRenderSetChannelMode004
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode004, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_EXCHANGE;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, -1);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0700
 * @tc.name   : testCommonRenderSetChannelMode005
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode005, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_MIX;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, -1);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0800
 * @tc.name   : testCommonRenderSetChannelMode006
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode006, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_LEFT_MUTE;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, -1);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_0900
 * @tc.name   : testCommonRenderSetChannelMode007
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode007, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_RIGHT_MUTE;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, -1);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_1000
 * @tc.name   : testCommonRenderSetChannelMode008
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode008, Function | MediumTest | Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_BOTH_MUTE;
    int32_t ret = render_->SetChannelMode(render_, mode);
    EXPECT_EQ(ret, -1);
}

/**
 * @tc.number : SUB_Driver_Audio_SetChannelMode_1100
 * @tc.name   : testCommonRenderSetChannelMode009
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetChannelMode009, Function | MediumTest | Level2)
{
    AudioChannelMode mode = AUDIO_CHANNEL_BOTH_MUTE;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetChannelMode(nullptr, mode));
}

/**
 * @tc.number : SUB_Driver_Audio_GetChannelMode_0300
 * @tc.name   : testCommonRenderGetChannelMode001
 * @tc.desc   : Reliability of function(GetChannelMode)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetChannelMode001, Function | MediumTest | Level1)
{
    enum AudioChannelMode channelMode = AUDIO_CHANNEL_NORMAL;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetChannelMode(render_, &channelMode);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_RegCallback_0300
 * @tc.name   : testCommonRenderRegCallback001
 * @tc.desc   : Reliability of function(RegCallback)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRegCallback001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    int8_t cookie = 0;
    struct IAudioCallback audioCallback;
    for (i = 0; i < 1000; i++) {
        ret |= render_->RegCallback(render_, &audioCallback, cookie);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Audio_RegCallback_0400
 * @tc.name   : testCommonRenderRegCallback002
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRegCallback002, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int8_t cookie = 127;
    struct IAudioCallback audioCallback;
    ret = render_->RegCallback(render_, &audioCallback, cookie);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Audio_RegCallback_0500
 * @tc.name   : testCommonRenderRegCallback003
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRegCallback003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    int8_t cookie = -1;
    struct IAudioCallback audioCallback;
    ret = render_->RegCallback(render_, &audioCallback, cookie);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Audio_DrainBuffer_0300
 * @tc.name   : testCommonRenderDrainBufferk001
 * @tc.desc   : Reliability of function(DrainBuffer)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderDrainBufferk001, Function | MediumTest | Level2)
{
    enum AudioDrainNotifyType type = AUDIO_DRAIN_NORMAL_MODE;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret |= render_->DrainBuffer(render_, &type);
    }
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number : SUB_Driver_Audio_DrainBuffer_0400
 * @tc.name   : testCommonRenderDrainBufferk002
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderDrainBufferk002, Function | MediumTest | Level2)
{
    enum AudioDrainNotifyType type = AUDIO_DRAIN_NORMAL_MODE;
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->DrainBuffer(render_, &type));
}

/**
 * @tc.number : SUB_Driver_Audio_DrainBuffer_0500
 * @tc.name   : testCommonRenderDrainBufferk003
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderDrainBufferk003, Function | MediumTest | Level2)
{
    enum AudioDrainNotifyType type = AUDIO_DRAIN_EARLY_MODE;
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->DrainBuffer(render_, &type));
}

/**
 * @tc.number : SUB_Driver_Audio_IsSupportsDrain_0300
 * @tc.name   : testCommonRenderIsSupportsDrain001
 * @tc.desc   : Reliability of function(IsSupportsDrain)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsDrain001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    bool support = false;
    for (i = 0; i < 1000; i++) {
        ret |= render_->IsSupportsDrain(render_, &support);
    }
    EXPECT_EQ(ret, -2);
}

/**
 * @tc.number : SUB_Driver_Audio_IsSupportsDrain_0400
 * @tc.name   : testCommonRenderIsSupportsDrain002
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsDrain002, Function | MediumTest | Level2)
{
    int32_t ret = render_->IsSupportsDrain(nullptr, nullptr);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_0500
 * @tc.name   : testCommonRenderCheckSceneCapability001
 * @tc.desc   : Reliability of function(CheckSceneCapability)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    struct AudioSceneDescriptor scene;
    bool supported = false;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc = devDescRender_;
    for (i = 0; i < 1000; i++) {
        ret |= render_->CheckSceneCapability(render_, &scene, &supported);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_0600
 * @tc.name   : testCommonRenderCheckSceneCapability002
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability002, Function | MediumTest | Level2)
{
    bool supported = false;
    EXPECT_NE(HDF_SUCCESS, render_->CheckSceneCapability(nullptr, nullptr, &supported));
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_0700
 * @tc.name   : testCommonRenderCheckSceneCapability003
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability003, Function | MediumTest | Level2)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc = devDescRender_;
    EXPECT_NE(HDF_SUCCESS, render_->CheckSceneCapability(nullptr, &scene, nullptr));
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_0800
 * @tc.name   : testCommonRenderCheckSceneCapability004
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability004, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->CheckSceneCapability(render_, nullptr, nullptr));
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_0900
 * @tc.name   : testCommonRenderCheckSceneCapability005
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability005, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->CheckSceneCapability(nullptr, nullptr, nullptr));
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_1000
 * @tc.name   : testCommonRenderCheckSceneCapability006
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability006, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc = devDescRender_;
    bool supported = false;
    EXPECT_EQ(HDF_SUCCESS, render_->CheckSceneCapability(render_, &scene, &supported));
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_1100
 * @tc.name   : testCommonRenderCheckSceneCapability007
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability007, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc = devDescRender_;
    bool supported = false;
    EXPECT_EQ(HDF_SUCCESS, render_->CheckSceneCapability(render_, &scene, &supported));
}

/**
 * @tc.number : SUB_Driver_Audio_CheckSceneCapability_1200
 * @tc.name   : testCommonRenderCheckSceneCapability008
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderCheckSceneCapability008, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc = devDescRender_;
    bool supported = false;
    EXPECT_EQ(HDF_SUCCESS, render_->CheckSceneCapability(render_, &scene, &supported));
}

/**
 * @tc.number : SUB_Driver_Audio_SelectScene_0400
 * @tc.name   : testCommonRenderSelectScene001
 * @tc.desc   : Reliability of function(SelectScene)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_OUT_HEADSET;
    scene.desc.desc = strdup("mic");
    for (i = 0; i < 1000; i++) {
        ret |= render_->SelectScene(render_, &scene);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Audio_SelectScene_0500
 * @tc.name   : testCommonRenderSelectScene002
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene002, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->SelectScene(nullptr, nullptr));
}

/**
 * @tc.number : SUB_Driver_Audio_SelectScene_0600
 * @tc.name   : testCommonRenderSelectScene003
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene003, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_OUT_HEADSET;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number : SUB_Driver_Audio_SelectScene_0700
 * @tc.name   : testCommonRenderSelectScene004
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene004, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_OUT_HEADSET;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number : SUB_Driver_Audio_SelectScene_0800
 * @tc.name   : testCommonRenderSelectScene005
 * @tc.desc   : Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene005, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_OUT_HEADSET;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_0900
 * @tc.name  testCommonRenderSelectScene006
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene006, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_OUT_SPEAKER;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1000
 * @tc.name  testCommonRenderSelectScene007
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene007, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_OUT_SPEAKER;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1100
 * @tc.name  testCommonRenderSelectScene008
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene008, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_OUT_SPEAKER;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1200
 * @tc.name  testCommonRenderSelectScene009
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene009, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_OUT_SPEAKER;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1300
 * @tc.name  testCommonRenderSelectScene010
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene010, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_NONE;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1400
 * @tc.name  testCommonRenderSelectScene011
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene011, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_NONE;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1500
 * @tc.name  testCommonRenderSelectScene012
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene012, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_NONE;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1600
 * @tc.name  testCommonRenderSelectScene013
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene013, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_NONE;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1700
 * @tc.name  testCommonRenderSelectScene014
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene014, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_OUT_LINEOUT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1800
 * @tc.name  testCommonRenderSelectScene015
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene015, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_OUT_LINEOUT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_1900
 * @tc.name  testCommonRenderSelectScene016
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene016, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_OUT_LINEOUT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2000
 * @tc.name  testCommonRenderSelectScene017
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene017, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_OUT_LINEOUT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2100
 * @tc.name  testCommonRenderSelectScene018
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene018, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_OUT_HDMI;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2200
 * @tc.name  testCommonRenderSelectScene019
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene019, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_OUT_HDMI;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2300
 * @tc.name  testCommonRenderSelectScene020
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene020, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_OUT_HDMI;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2400
 * @tc.name  testCommonRenderSelectScene021
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene021, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_OUT_HDMI;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2500
 * @tc.name  testCommonRenderSelectScene022
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene022, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_IN_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2600
 * @tc.name  testCommonRenderSelectScene023
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene023, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_IN_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2700
 * @tc.name  testCommonRenderSelectScene024
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene024, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_IN_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2800
 * @tc.name  testCommonRenderSelectScene025
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene025, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_IN_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_2900
 * @tc.name  testCommonRenderSelectScene026
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene026, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_IN_HS_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3000
 * @tc.name  testCommonRenderSelectScene027
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene027, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_IN_HS_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3100
 * @tc.name  testCommonRenderSelectScene028
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene028, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_IN_HS_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3200
 * @tc.name  testCommonRenderSelectScene029
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene029, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_IN_HS_MIC;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3300
 * @tc.name  testCommonRenderSelectScene030
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene030, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_IN_LINEIN;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3400
 * @tc.name  testCommonRenderSelectScene031
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene031, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_IN_LINEIN;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3500
 * @tc.name  testCommonRenderSelectScene032
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene032, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_IN_LINEIN;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3600
 * @tc.name  testCommonRenderSelectScene033
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene033, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_IN_LINEIN;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3700
 * @tc.name  testCommonRenderSelectScene034
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene034, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_IN_USB_EXT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3800
 * @tc.name  testCommonRenderSelectScene035
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene035, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_COMMUNICATION;
    scene.desc.pins = PIN_IN_USB_EXT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_3900
 * @tc.name  testCommonRenderSelectScene036
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene036, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_RINGTONE;
    scene.desc.pins = PIN_IN_USB_EXT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SelectScene_4000
 * @tc.name  testCommonRenderSelectScene037
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSelectScene037, Function | MediumTest | Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_CALL;
    scene.desc.pins = PIN_IN_USB_EXT;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
}

/**
 * @tc.number  SUB_Driver_Audio_SetMutey_0300
 * @tc.name  testCommonRenderSetMute001
 * @tc.desc  Reliability of function(SetMute)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetMute001, Function | MediumTest | Level1)
{
    bool mute = false;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetMute(render_, mute);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_GetMute_0300
 * @tc.name  testCommonRenderGetMute001
 * @tc.desc  Reliability of function(GetMute)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetMute001, Function | MediumTest | Level1)
{
    bool mute = false;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetMute(render_, &mute);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_GetMute_0400
 * @tc.name  testCommonRenderGetMute002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetMute002, Function | MediumTest | Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMute(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_SetVolume_0300
 * @tc.name  testCommonRenderSetVolume001
 * @tc.desc  Reliability of function(SetVolume)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetVolume001, Function | MediumTest | Level1)
{
    float volume = 0.2;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetVolume(render_, volume);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_SetVolume_0400
 * @tc.name  testCommonRenderSetVolume002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetVolume002, Function | MediumTest | Level2)
{
    float volume = -0.1;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetVolume(render_, volume);
    }
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetVolume_0300
 * @tc.name  testCommonRenderGetVolume001
 * @tc.desc  Reliability of function(GetVolume)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetVolume001, Function | MediumTest | Level1)
{
    float volume = 0.2;
    int32_t ret = 0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetVolume(render_, &volume);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_GetVolume_0400
 * @tc.name  testCommonRenderGetVolume002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetVolume002, Function | MediumTest | Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetVolume(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetGainThreshold_0300
 * @tc.name  testCommonRenderGetGainThreshold001
 * @tc.desc  Reliability of function(GetGainThreshold)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGainThreshold001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    float min = 0.0;
    float max = 1.0;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetGainThreshold(render_, &min, &max);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_GetGainThreshold_0400
 * @tc.name  testCommonRenderGetGainThreshold002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGainThreshold002, Function | MediumTest | Level2)
{
    float max = 1.0;
    EXPECT_NE(HDF_SUCCESS, render_->GetGainThreshold(nullptr, nullptr, &max));
}

/**
 * @tc.number  SUB_Driver_Audio_GetGainThreshold_0500
 * @tc.name  testCommonRenderGetGainThreshold003
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGainThreshold003, Function | MediumTest | Level2)
{
    float min = 1.0;
    EXPECT_NE(HDF_SUCCESS, render_->GetGainThreshold(nullptr, &min, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetGainThreshold_0600
 * @tc.name  testCommonRenderGetGainThreshold004
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGainThreshold004, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->GetGainThreshold(render_, nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetGainThreshold_0700
 * @tc.name  testCommonRenderGetGainThreshold005
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGainThreshold005, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->GetGainThreshold(nullptr, nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetGain_0300
 * @tc.name  testCommonRenderGetGain001
 * @tc.desc  Reliability of function(GetGain)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGain001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    float gain = 0.1;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetGain(render_, &gain);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_GetGain_0400
 * @tc.name  testCommonRenderGetGain002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetGain002, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->GetGain(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_SetGainn_0300
 * @tc.name  testCommonRenderSetGain001
 * @tc.desc  Reliability of function(SetGain)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetGain001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    float gain = 0.1;
    int i = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetGain(render_, gain);
    }
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameSize_0400
 * @tc.name  testCommonRenderGetFrameSize001
 * @tc.desc  Reliability of function(GetFrameSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameSize001, Function | MediumTest | Level1)
{
    uint64_t frameSize = 0;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetFrameSize(render_, &frameSize);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameCount_0400
 * @tc.name  testCommonRenderGetFrameCount001
 * @tc.desc  Reliability of function(GetFrameCount)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameCount001, Function | MediumTest | Level1)
{
    uint64_t frameCount = 0;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetFrameCount(render_, &frameCount);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_SetSampleAttributes_0400
 * @tc.name  testCommonRenderSetSampleAttributes001
 * @tc.desc  Reliability of function(SetSampleAttributes)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetSampleAttributes001, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = attrsRender_;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetSampleAttributes(render_, &attrs);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_SetSampleAttributes_0500
 * @tc.name  testCommonRenderSetSampleAttributes002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetSampleAttributes002, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->SetSampleAttributes(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetSampleAttributes_0400
 * @tc.name  testCommonRenderGetSampleAttributes001
 * @tc.desc  Reliability of function(GetSampleAttributes)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetSampleAttributes001, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetSampleAttributes(render_, &attrs);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetSampleAttributes_0500
 * @tc.name  testCommonRenderGetSampleAttributes002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetSampleAttributes002, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->GetSampleAttributes(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetCurrentChannelId_0400
 * @tc.name  testCommonRenderGetCurrentChannelId001
 * @tc.desc  Reliability of function(GetCurrentChannelId)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetCurrentChannelId001, Function | MediumTest | Level1)
{
    uint32_t channelId;
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetCurrentChannelId(render_, &channelId);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetCurrentChannelId_0500
 * @tc.name  testCommonRenderGetCurrentChannelId002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetCurrentChannelId002, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->GetCurrentChannelId(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_RenderSetExtraParams_0400
 * @tc.name  testCommonRenderSetExtraParams001
 * @tc.desc  Reliability of function(SetExtraParams)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetExtraParams001, Function | MediumTest | Level1)
{
    char keyValueList[AUDIO_RENDER_BUF_TEST] =
        "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000";
    int i = 0;
    int32_t ret = 0;
    for (i = 0; i < 1000; i++) {
        ret = render_->SetExtraParams(render_, keyValueList);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_RenderSetExtraParams_0500
 * @tc.name  testCommonRenderSetExtraParams002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderSetExtraParams002, Function | MediumTest | Level2)
{
    EXPECT_NE(HDF_SUCCESS, render_->SetExtraParams(nullptr, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_RenderGetExtraParams_0400
 * @tc.name  testCommonRenderGetExtraParams001
 * @tc.desc  Reliability of function(GetExtraParams)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetExtraParams001, Function | MediumTest | Level1)
{
    int i = 0;
    int32_t ret = 0;
    char keyValueList[AUDIO_RENDER_BUF_TEST];
    uint32_t keyValueListLen = BUFFER_LENTH;
    for (i = 0; i < 1000; i++) {
        ret = render_->GetExtraParams(render_, keyValueList, keyValueListLen);
    }
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_RenderGetExtraParams_0500
 * @tc.name  testCommonRenderGetExtraParams002
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetExtraParams002, Function | MediumTest | Level2)
{
    uint32_t keyValueListLen = 0;
    EXPECT_NE(HDF_SUCCESS, render_->GetExtraParams(nullptr, nullptr, keyValueListLen));
}

/**
 * @tc.number  SUB_Driver_Audio_RenderGetExtraParams_0600
 * @tc.name  testCommonRenderGetExtraParams003
 * @tc.desc  Test input param
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetExtraParams003, Function | MediumTest | Level2)
{
    char keyValueList[AUDIO_RENDER_BUF_TEST] = {};
    uint32_t keyValueListLen = 0xffffffff;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetExtraParams(render_, keyValueList, keyValueListLen));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0100
 * @tc.name  testCommonRenderAddAudioEffect001
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect001, TestSize.Level2)
{
    uint64_t effectId = 256;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, effectId));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0200
 * @tc.name  testCommonRenderAddAudioEffect002
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect002, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, -256));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0300
 * @tc.name  testCommonRenderAddAudioEffect003
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect003, TestSize.Level2)
{
    uint64_t effectId = 42949672950;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, effectId));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0400
 * @tc.name  testCommonRenderAddAudioEffect004
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect004, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, -42949672950));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0500
 * @tc.name  testCommonRenderAddAudioEffect005
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect005, TestSize.Level2)
{
    int32_t i;
    uint64_t effectId = 0;
    for (i = 0; i < 20; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, effectId));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0600
 * @tc.name  testCommonRenderAddAudioEffect006
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect006, TestSize.Level2)
{
    int32_t i;
    uint64_t effectId = 5;
    for (i = 0; i < 20; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, effectId));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0700
 * @tc.name  testCommonRenderAddAudioEffect007
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect007, TestSize.Level2)
{
    uint64_t effectId = 1000;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, effectId));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0800
 * @tc.name  testCommonRenderAddAudioEffect008
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect008, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, 2));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_0900
 * @tc.name  testCommonRenderAddAudioEffect009
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect009, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, 40));
}

/**
 * @tc.number  SUB_Driver_Audio_AddAudioEffect_1000
 * @tc.name  testCommonRenderAddAudioEffect010
 * @tc.desc  Reliability of function(AddAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAddAudioEffect010, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, -60));
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0100
 * @tc.name  testCommonRenderRemoveAudioEffect001
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect001, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, -256));
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0200
 * @tc.name  testCommonRenderRemoveAudioEffect002
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect002, TestSize.Level2)
{
    uint64_t effectId = 42949672950;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, effectId));
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0300
 * @tc.name  testCommonRenderRemoveAudioEffect003
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect003, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, -42949672950));
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0400
 * @tc.name  testCommonRenderRemoveAudioEffect004
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect004, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, 6));
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0500
 * @tc.name  testCommonRenderRemoveAudioEffect005
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect005, TestSize.Level2)
{
    int32_t i;
    uint64_t effectId = 0;
    for (i = 0; i < 20; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, effectId));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0600
 * @tc.name  testCommonRenderRemoveAudioEffect006
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect006, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, 90));
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0700
 * @tc.name  testCommonRenderRemoveAudioEffect007
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect007, TestSize.Level2)
{
    int32_t i;
    for (i = 0; i < 20; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, 70));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_RemoveAudioEffect_0800
 * @tc.name  testCommonRenderRemoveAudioEffect008
 * @tc.desc  Reliability of function(RemoveAudioEffect)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderRemoveAudioEffect008, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, -80));
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0100
 * @tc.name  testCommonRenderGetFrameBufferSize001
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize001, TestSize.Level2)
{
    uint64_t bufferSize = 0;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0200
 * @tc.name  testCommonRenderGetFrameBufferSize002
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize002, TestSize.Level2)
{
    uint64_t bufferSize = 256;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0300
 * @tc.name  testCommonRenderGetFrameBufferSize003
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize003, TestSize.Level2)
{
    uint64_t bufferSize = 266;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0400
 * @tc.name  testCommonRenderGetFrameBufferSize004
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize004, TestSize.Level2)
{
    uint64_t bufferSize = 1600;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0500
 * @tc.name  testCommonRenderGetFrameBufferSize005
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize005, TestSize.Level2)
{
    uint64_t bufferSize = 300;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0600
 * @tc.name  testCommonRenderGetFrameBufferSize006
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize006, TestSize.Level2)
{
    uint64_t bufferSize = 4294967295;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0700
 * @tc.name  testCommonRenderGetFrameBufferSize007
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize007, TestSize.Level2)
{
    uint64_t bufferSize = 429496729500;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0800
 * @tc.name  testCommonRenderGetFrameBufferSize008
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize008, TestSize.Level2)
{
    int32_t i;
    uint64_t bufferSize = 429496;
    int32_t ret;
    for (i = 0; i < 50; i++) {
        ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
    }
    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_0900
 * @tc.name  testCommonRenderGetFrameBufferSize009
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize009, TestSize.Level2)
{
    int32_t ret = render_->GetFrameBufferSize(nullptr, nullptr);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_1000
 * @tc.name  testCommonRenderGetFrameBufferSize010
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize010, TestSize.Level2)
{
    uint64_t bufferSize = 0;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(nullptr, nullptr);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_1100
 * @tc.name  testCommonRenderGetFrameBufferSize011
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize011, TestSize.Level2)
{
    uint64_t bufferSize = 1000;
    int32_t ret = render_->GetFrameBufferSize(render_, &bufferSize);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_1200
 * @tc.name  testCommonRenderGetFrameBufferSize012
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize012, TestSize.Level2)
{
    int32_t i;
    uint64_t bufferSize = 8000;
    int32_t ret;
    for (i = 0; i < 100; i++) {
        ret = render_->GetFrameBufferSize(render_, &bufferSize);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetFrameBufferSize_1300
 * @tc.name  testCommonRenderGetFrameBufferSize013
 * @tc.desc  Reliability of function(GetFrameBufferSize)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderGetFrameBufferSize013, TestSize.Level2)
{
    int32_t ret = render_->GetFrameBufferSize(render_, nullptr);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number  SUB_Driver_Audio_Start_0100
 * @tc.name  testCommonRenderStart001
 * @tc.desc  Reliability of function(Start)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStart001, TestSize.Level2)
{
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Start_0200
 * @tc.name  testCommonRenderStart002
 * @tc.desc  Reliability of function(Start)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStart002, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number  SUB_Driver_Audio_Start_0300
 * @tc.name  testCommonRenderStart003
 * @tc.desc  Reliability of function(Start)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStart003, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
}

/**
 * @tc.number  SUB_Driver_Audio_Start_0400
 * @tc.name  testCommonRenderStart004
 * @tc.desc  Reliability of function(Start)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStart004, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_Start_0500
 * @tc.name  testCommonRenderStart005
 * @tc.desc  Reliability of function(Start)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStart005, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    int32_t i;
    for (i = 0; i < 10; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
        EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Start_0600
 * @tc.name  testCommonRenderStart006
 * @tc.desc  Reliability of function(Start)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStart006, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    int32_t i;
    for (i = 0; i < 10; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
    }
}
/**
 * @tc.number  SUB_Driver_Audio_Stop_0100
 * @tc.name  testCommonRenderStop001
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop001, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0200
 * @tc.name  testCommonRenderStop002
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop002, TestSize.Level2)
{
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0300
 * @tc.name  testCommonRenderStop003
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop003, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    int32_t i;
    for (i = 0; i < 200; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
        EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0400
 * @tc.name  testCommonRenderStop004
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop004, TestSize.Level2)
{
    int32_t i;
    for (i = 0; i < 150; i++) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0500
 * @tc.name  testCommonRenderStop005
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop005, TestSize.Level1)
{
    int32_t i;
    for (i = 0; i < 20; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
        EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0600
 * @tc.name  testCommonRenderStop006
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop006, TestSize.Level2)
{
    int32_t i;
    for (i = 0; i < 20; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0700
 * @tc.name  testCommonRenderStop007
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop007, TestSize.Level1)
{
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->TurnStandbyMode(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->Stop(render_);
    int32_t i;
    for (i = 0; i < 50; i++) {
        ret = render_->Start(render_);
        ret = render_->Stop(render_);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0800
 * @tc.name  testCommonRenderStop008
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop008, TestSize.Level1)
{
    float speed = 0.0;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->GetRenderSpeed(render_, &speed));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    int32_t i;
    for (i = 0; i < 25; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
        EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_0900
 * @tc.name  testCommonRenderStop009
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop009, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetRenderSpeed(nullptr, nullptr));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_1000
 * @tc.name  testCommonRenderStop010
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop010, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
        EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_1100
 * @tc.name  testCommonRenderStop011
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop011, TestSize.Level1)
{
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_1200
 * @tc.name  testCommonRenderStop012
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop012, TestSize.Level1)
{
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_1300
 * @tc.name  testCommonRenderStop013
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop013, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_Stop_1400
 * @tc.name  testCommonRenderStop014
 * @tc.desc  Reliability of function(Stop)
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderStop014, TestSize.Level2)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    int32_t i;
    for (i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0100
 * @tc.name: testCommonRenderAudioDevDump001
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump001, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 1;

    int32_t ret = render_->AudioDevDump(nullptr, range, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0200
 * @tc.name: testCommonRenderAudioDevDump002
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump002, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 2;

    int32_t ret = render_->AudioDevDump(nullptr, range, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0300
 * @tc.name: testCommonRenderAudioDevDump003
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump003, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 3;

    int32_t ret = render_->AudioDevDump(nullptr, range, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0400
 * @tc.name: testCommonRenderAudioDevDump004
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump004, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 5;

    int32_t ret = render_->AudioDevDump(nullptr, range, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0500
 * @tc.name: testCommonRenderAudioDevDump005
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump005, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 6;

    int32_t ret = render_->AudioDevDump(nullptr, range, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0600
 * @tc.name: testCommonRenderAudioDevDump006
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump006, TestSize.Level1)
{
    int32_t range = 1;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0700
 * @tc.name: testCommonRenderAudioDevDump007
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump007, TestSize.Level1)
{
    int32_t range = 2;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0800
 * @tc.name: testCommonRenderAudioDevDump008
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump008, TestSize.Level1)
{
    int32_t range = 3;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_0900
 * @tc.name: testCommonRenderAudioDevDump009
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump009, TestSize.Level1)
{
    int32_t range = 5;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1000
 * @tc.name: testCommonRenderAudioDevDump010
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump010, TestSize.Level1)
{
    int32_t range = 6;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1100
 * @tc.name: testCommonRenderAudioDevDump011
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump011, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 4;

    int32_t ret = render_->AudioDevDump(nullptr, range, 0);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1200
 * @tc.name: testCommonRenderAudioDevDump012
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump012, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 4;

    int32_t ret = render_->AudioDevDump(nullptr, range, 1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1300
 * @tc.name: testCommonRenderAudioDevDump013
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump013, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 4;

    int32_t ret = render_->AudioDevDump(nullptr, range, 2);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1400
 * @tc.name: testCommonRenderAudioDevDump014
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump014, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 4;

    int32_t ret = render_->AudioDevDump(nullptr, range, 3);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1500
 * @tc.name: testCommonRenderAudioDevDump015
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump015, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 4;

    int32_t ret = render_->AudioDevDump(nullptr, range, 4);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1600
 * @tc.name: testCommonRenderAudioDevDump016
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump016, TestSize.Level2)
{
    ASSERT_NE(render_->AudioDevDump, nullptr);
    int32_t range = 4;

    int32_t ret = render_->AudioDevDump(nullptr, range, -2);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1700
 * @tc.name: testCommonRenderAudioDevDump017
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump017, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = 0;

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1800
 * @tc.name: testCommonRenderAudioDevDump018
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump018, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = 1;

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_1900
 * @tc.name: testCommonRenderAudioDevDump019
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump019, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = 2;

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_2000
 * @tc.name: testCommonRenderAudioDevDump020
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump020, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = 3;

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_2100
 * @tc.name: testCommonRenderAudioDevDump021
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump021, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = 4;

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_AudioDevDump_2200
 * @tc.name: testCommonRenderAudioDevDump022
 * @tc.desc: Dumps information about the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderAudioDevDump022, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = -2;

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    EXPECT_EQ(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0100
 * @tc.name: testCommonRenderPause001
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause001, TestSize.Level2)
{
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Pause(nullptr));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0200
 * @tc.name: testCommonRenderPause002
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause002, TestSize.Level1)
{
    int32_t ret;
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
        ret = render_->Pause(render_);
        EXPECT_EQ(ret, HDF_SUCCESS);

        EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));
        EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0300
 * @tc.name: testCommonRenderPause003
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause003, TestSize.Level1)
{
    ASSERT_NE(render_->Pause, nullptr);
    ASSERT_NE(render_->Start, nullptr);

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));
    render_->Stop(render_);
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0400
 * @tc.name: testCommonRenderPause004
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause004, TestSize.Level2)
{
    ASSERT_NE(render_->Pause, nullptr);
    ASSERT_NE(render_->Start, nullptr);
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0500
 * @tc.name: testCommonRenderPause005
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause005, TestSize.Level1)
{
    ASSERT_NE(render_->Pause, nullptr);
    ASSERT_NE(render_->Start, nullptr);

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));

    ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));
    render_->Stop(render_);
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0600
 * @tc.name: testCommonRenderPause006
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause006, TestSize.Level1)
{
    ASSERT_NE(render_->Pause, nullptr);
    ASSERT_NE(render_->Start, nullptr);
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));
    for (int i = 0; i < 50; i++) {
        ret = render_->Pause(render_);
        EXPECT_EQ(ret, HDF_SUCCESS);
        EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));
    }
    render_->Stop(render_);
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0700
 * @tc.name: testCommonRenderPause007
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause007, TestSize.Level1)
{
    ASSERT_NE(render_->Pause, nullptr);
    ASSERT_NE(render_->Start, nullptr);
    ASSERT_NE(render_->Resume, nullptr);

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    render_->Resume(render_);
    ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_Pause_0800
 * @tc.name: testCommonRenderPause008
 * @tc.desc: Pauses audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderPause008, TestSize.Level1)
{
    ASSERT_NE(render_->Pause, nullptr);
    ASSERT_NE(render_->Start, nullptr);
    ASSERT_NE(render_->Resume, nullptr);

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (int i = 0; i < 50; i++) {
        ret = render_->Pause(render_);
        EXPECT_EQ(ret, HDF_SUCCESS);
        render_->Resume(render_);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0100
 * @tc.name: testCommonRenderResume001
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume001, TestSize.Level2)
{
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Resume(nullptr));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0200
 * @tc.name: testCommonRenderResume002
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume002, TestSize.Level2)
{
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Resume(render_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0300
 * @tc.name: testCommonRenderResume003
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume003, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Pause(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));

    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0400
 * @tc.name: testCommonRenderResume004
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume004, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Stop(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Resume(render_));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0500
 * @tc.name: testCommonRenderResume005
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume005, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Stop(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Resume(render_));
    }
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0600
 * @tc.name: testCommonRenderResume006
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume006, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Pause(render_));
    int32_t ret = render_->Resume(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0700
 * @tc.name: testCommonRenderResume007
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume007, TestSize.Level1)
{
    int32_t ret;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Pause(render_));
        ret = render_->Resume(render_);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0800
 * @tc.name: testCommonRenderResume008
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume008, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Pause(render_));
    render_->Flush(render_);
    int32_t ret = render_->Resume(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_0900
 * @tc.name: testCommonRenderResume009
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume009, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Pause(render_));
    int32_t ret = render_->Resume(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Resume(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_1000
 * @tc.name: testCommonRenderResume010
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume010, TestSize.Level2)
{
    int32_t ret;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_SUCCESS, render_->Pause(render_));
        ret = render_->Resume(render_);
        EXPECT_EQ(HDF_SUCCESS, ret);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Resume(render_));
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_1100
 * @tc.name: testCommonRenderResume011
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume011, TestSize.Level2)
{
    int32_t ret = render_->Resume(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_1200
 * @tc.name: testCommonRenderResume012
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume012, TestSize.Level2)
{
    int32_t ret;
    for (int i = 0; i < 50; i++) {
        ret = render_->Resume(render_);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Resume_1300
 * @tc.name: testCommonRenderResume013
 * @tc.desc: Resumes audio rendering or capturing.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderResume013, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Pause(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    render_->Flush(render_);
    int32_t ret = render_->Resume(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0100
 * @tc.name: testCommonRenderFlush001
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush001, TestSize.Level2)
{
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Flush(nullptr));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0200
 * @tc.name: testCommonRenderFlush002
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush002, TestSize.Level2)
{
    for (int i = 0; i < 50; i++) {
        EXPECT_NE(HDF_SUCCESS, render_->Flush(render_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0300
 * @tc.name: testCommonRenderFlush003
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush003, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Flush(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0400
 * @tc.name: testCommonRenderFlush004
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush004, TestSize.Level2)
{
    int32_t ret;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    for (int i = 0; i < 50; i++) {
        ret = render_->Flush(render_);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0500
 * @tc.name: testCommonRenderFlush005
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush005, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Flush(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    render_->Resume(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Flush(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0600
 * @tc.name: testCommonRenderFlush006
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush006, TestSize.Level2)
{
    int32_t ret;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    for (int i = 0; i < 50; i++) {
        ret = render_->Flush(render_);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
    render_->Resume(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Flush(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0700
 * @tc.name: testCommonRenderFlush007
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush007, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Flush(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    render_->Resume(render_);
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Flush(render_));
    }
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0800
 * @tc.name: testCommonRenderFlush008
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush008, TestSize.Level2)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    int32_t ret = render_->Flush(render_);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_Flush_0900
 * @tc.name: testCommonRenderFlush009
 * @tc.desc: Flushes data in the audio buffer.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderFlush009, TestSize.Level2)
{
    int32_t ret;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    for (int i = 0; i < 50; i++) {
        ret = render_->Flush(render_);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_TurnStandbyMode_0100
 * @tc.name: testCommonRenderTurnStandbyMode001
 * @tc.desc: Sets or cancels the standby mode of the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderTurnStandbyMode001, TestSize.Level2)
{
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->TurnStandbyMode(nullptr));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_TurnStandbyMode_0300
 * @tc.name: testCommonRenderTurnStandbyMode002
 * @tc.desc: Sets or cancels the standby mode of the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderTurnStandbyMode002, TestSize.Level1)
{
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Pause(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->TurnStandbyMode(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Stop(render_);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_TurnStandbyMode_0300
 * @tc.name: testCommonRenderTurnStandbyMode005
 * @tc.desc: Sets or cancels the standby mode of the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderTurnStandbyMode003, TestSize.Level1)
{
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    render_->Flush(render_);

    ret = render_->TurnStandbyMode(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Stop(render_);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_TurnStandbyMode_0400
 * @tc.name: testCommonRenderTurnStandbyMode004
 * @tc.desc: Sets or cancels the standby mode of the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderTurnStandbyMode004, TestSize.Level2)
{
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->Stop(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->TurnStandbyMode(render_);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_TurnStandbyMode_0500
 * @tc.name: testCommonRenderTurnStandbyMode005
 * @tc.desc: Sets or cancels the standby mode of the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderTurnStandbyMode005, TestSize.Level2)
{
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->Stop(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    for (int i = 0; i < 50; i++) {
        ret = render_->TurnStandbyMode(render_);
        EXPECT_EQ(HDF_FAILURE, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_TurnStandbyMode_0600
 * @tc.name: testCommonRenderTurnStandbyMode006
 * @tc.desc: Sets or cancels the standby mode of the audio device.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderTurnStandbyMode006, TestSize.Level1)
{
    int32_t ret = render_->Start(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->Pause(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->Resume(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->TurnStandbyMode(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);
    ret = render_->Stop(render_);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0100
 * @tc.name: testCommonRenderIsSupportsPauseAndResume001
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume001, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = false;
    bool supportResume = false;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

        ret = render_->IsSupportsPauseAndResume(render_, nullptr, &supportResume);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);

        ret = render_->IsSupportsPauseAndResume(render_, &supportPause, nullptr);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0200
 * @tc.name: testCommonRenderIsSupportsPauseAndResume002
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume002, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = false;
    bool supportResume = false;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0300
 * @tc.name: testCommonRenderIsSupportsPauseAndResume003
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume003, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = false;
    bool supportResume = true;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0400
 * @tc.name: testCommonRenderIsSupportsPauseAndResume004
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume004, TestSize.Level2)
{
    bool supportPause = true;
    bool supportResume = false;
    int32_t ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0500
 * @tc.name: testCommonRenderIsSupportsPauseAndResume005
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume005, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = true;
    bool supportResume = false;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0600
 * @tc.name: testCommonRenderIsSupportsPauseAndResume006
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume006, TestSize.Level2)
{
    bool supportPause = true;
    bool supportResume = true;
    int32_t ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0700
 * @tc.name: testCommonRenderIsSupportsPauseAndResume007
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume007, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = true;
    bool supportResume = true;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0800
 * @tc.name: testCommonRenderIsSupportsPauseAndResume008
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume008, TestSize.Level2)
{
    bool supportPause = true;
    bool supportResume = false;
    int32_t ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_0900
 * @tc.name: testCommonRenderIsSupportsPauseAndResume009
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume009, TestSize.Level2)
{
    bool supportPause = false;
    bool supportResume = true;
    int32_t ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_1000
 * @tc.name: testCommonRenderIsSupportsPauseAndResume010
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume010, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = false;
    bool supportResume = true;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_1100
 * @tc.name: testCommonRenderIsSupportsPauseAndResume011
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume011, TestSize.Level2)
{
    bool supportPause = true;
    bool supportResume = true;
    int32_t ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_1200
 * @tc.name: testCommonRenderIsSupportsPauseAndResume012
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume012, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = true;
    bool supportResume = true;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_IsSupportsPauseAndResume_1300
 * @tc.name: testCommonRenderIsSupportsPauseAndResume013
 * @tc.desc: Query whether the vendor support pause and resume.
 */
HWTEST_F(AudioUtRenderTestAdditional, testCommonRenderIsSupportsPauseAndResume013, TestSize.Level2)
{
    int32_t ret;
    bool supportPause = true;
    bool supportResume = false;
    for (int i = 0; i < 50; i++) {
        ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
    }
}
} // end of namespace