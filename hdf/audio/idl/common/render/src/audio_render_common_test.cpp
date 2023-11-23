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

#include <gtest/gtest.h>
#include "osal_mem.h"

#include "v1_1/audio_types.h"
#include "v1_1/iaudio_manager.h"
#include "v1_1/iaudio_render.h"

using namespace std;
using namespace testing::ext;

namespace {
const float MAX_GAINTHRESHOLD = 15.0;
const float MIN_GAINTHRESHOLD = 0.0;
const int BUFFER_LENTH = 1024 * 16;
const int DEEP_BUFFER_RENDER_PERIOD_SIZE = 4 * 1024;
const int MOVE_LEFT_NUM = 8;
const int32_t AUDIO_RENDER_BUF_TEST = 1024;
const int32_t AUDIO_RENDER_CHANNELCOUNT = 2;
const int32_t AUDIO_SAMPLE_RATE_48K = 48000;
const int32_t MAX_AUDIO_ADAPTER_DESC = 5;
const uint64_t DEFAULT_BUFFER_SIZE = 16384;

class AudioUtRenderTest : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct AudioAdapterDescriptor descs_[MAX_AUDIO_ADAPTER_DESC];
    struct AudioAdapterDescriptor *desc_;
    struct IAudioAdapter *adapter_ = nullptr;
    struct IAudioRender *render_ = nullptr;
    struct AudioDeviceDescriptor devDescRender_ = {};
    struct AudioSampleAttributes attrsRender_ = {};
    uint32_t renderId_ = 0;
    char *devDescriptorName_ = nullptr;
    uint32_t size_ = MAX_AUDIO_ADAPTER_DESC;
    virtual void SetUp();
    virtual void TearDown();
    uint64_t GetRenderBufferSize();
    void InitRenderAttrs(struct AudioSampleAttributes &attrs);
    void InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

uint64_t AudioUtRenderTest::GetRenderBufferSize()
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

void AudioUtRenderTest::InitRenderAttrs(struct AudioSampleAttributes &attrs)
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

void AudioUtRenderTest::InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = PIN_OUT_SPEAKER;
    devDescriptorName_ = strdup("cardname");
    devDesc.desc = devDescriptorName_;

    ASSERT_NE(desc_, nullptr);
    ASSERT_NE(desc_->ports, nullptr);
    for (uint32_t index = 0; index < desc_->portsLen; index++) {
        if (desc_->ports[index].dir == PORT_OUT) {
            devDesc.portId = desc_->ports[index].portId;
            return;
        }
    }
}

void AudioUtRenderTest::FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
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

void AudioUtRenderTest::ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen)
{
    if (descs == nullptr || descsLen == 0) {
        return;
    }

    for (uint32_t i = 0; i < descsLen; i++) {
        FreeAdapterElements(&descs[i], false);
    }
}

void AudioUtRenderTest::SetUp()
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

void AudioUtRenderTest::TearDown()
{
    ASSERT_NE(devDescriptorName_, nullptr);
    free(devDescriptorName_);
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

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0100, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Start(nullptr));
    render_->Stop(render_);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0200, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_NE(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
    render_->Stop(render_);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0300, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0400, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Flush(nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0500, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, render_->Flush(render_));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0600, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Stop(nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0700, TestSize.Level1)
{
    int32_t ret = render_->Stop(render_);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0800, TestSize.Level1)
{
    uint64_t frameSize = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetFrameSize(nullptr, &frameSize));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_0900, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetFrameSize(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1000, TestSize.Level1)
{
    uint64_t frameSize = 0;
    EXPECT_EQ(HDF_SUCCESS, render_->GetFrameSize(render_, &frameSize));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1100, TestSize.Level1)
{
    uint64_t frameCount = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetFrameCount(nullptr, &frameCount));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1200, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetFrameCount(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1300, TestSize.Level1)
{
    uint64_t frameCount = 0;
    EXPECT_EQ(HDF_SUCCESS, render_->GetFrameCount(render_, &frameCount));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1400, TestSize.Level1)
{
    struct AudioSampleAttributes attrs;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetSampleAttributes(nullptr, &attrs));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1500, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->SetSampleAttributes(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1600, TestSize.Level1)
{
    struct AudioSampleAttributes attrs = attrsRender_;
    EXPECT_EQ(HDF_SUCCESS, render_->SetSampleAttributes(render_, &attrs));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1700, TestSize.Level1)
{
    struct AudioSampleAttributes attrs;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetSampleAttributes(nullptr, &attrs));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1800, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetSampleAttributes(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_1900, TestSize.Level1)
{
    struct AudioSampleAttributes attrs;
    EXPECT_EQ(HDF_SUCCESS, render_->GetSampleAttributes(render_, &attrs));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2000, TestSize.Level1)
{
    uint32_t channelId;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetCurrentChannelId(nullptr, &channelId));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2100, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetCurrentChannelId(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2200, TestSize.Level1)
{
    uint32_t channelId;
    EXPECT_EQ(HDF_SUCCESS, render_->GetCurrentChannelId(render_, &channelId));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2300, TestSize.Level1)
{
    struct AudioSceneDescriptor scene;
    bool supported = false;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->CheckSceneCapability(nullptr, &scene, &supported));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2400, TestSize.Level1)
{
    bool supported = false;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->CheckSceneCapability(render_, nullptr, &supported));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2500, TestSize.Level1)
{
    struct AudioSceneDescriptor scene;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->CheckSceneCapability(render_, &scene, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2600, TestSize.Level1)
{
    struct AudioSceneDescriptor scene;
    bool supported = false;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc = devDescRender_;
    EXPECT_EQ(HDF_SUCCESS, render_->CheckSceneCapability(render_, &scene, &supported));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2700, TestSize.Level1)
{
    struct AudioSceneDescriptor scene;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SelectScene(nullptr, &scene));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2800, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->SelectScene(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_2900, TestSize.Level1)
{
    struct AudioSceneDescriptor scene;
    scene.scene.id = AUDIO_IN_MEDIA;
    scene.desc.pins = PIN_OUT_HEADSET;
    scene.desc.desc = strdup("mic");
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
    scene.desc.pins = PIN_OUT_SPEAKER;
    EXPECT_EQ(HDF_SUCCESS, render_->SelectScene(render_, &scene));
    free(scene.desc.desc);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3000, TestSize.Level1)
{
    uint32_t ms = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetLatency(nullptr, &ms));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3100, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetLatency(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3200, TestSize.Level1)
{
    uint32_t ms = 0;
    EXPECT_EQ(HDF_SUCCESS, render_->GetLatency(render_, &ms));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3300, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetRenderPosition(nullptr, &frames, &time));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3400, TestSize.Level1)
{
    struct AudioTimeStamp time;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetRenderPosition(render_, nullptr, &time));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3500, TestSize.Level1)
{
    uint64_t frames = 0;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetRenderPosition(render_, &frames, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3600, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvSec = 0;
    time.tvNSec = 0;
    int32_t ret = render_->GetRenderPosition(render_, &frames, &time);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3700, TestSize.Level1)
{
    uint64_t frames;
    struct AudioTimeStamp time;
    uint32_t frameLen = (uint64_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;

    int32_t ret = render_->Start(render_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = render_->RenderFrame(render_, frame, frameLen, &requestBytes);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = render_->GetRenderPosition(render_, &frames, &time);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);

    render_->Stop(render_);
    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3800, TestSize.Level1)
{
    char keyValueList[AUDIO_RENDER_BUF_TEST];
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetExtraParams(nullptr, keyValueList));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_3900, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->SetExtraParams(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4000, TestSize.Level1)
{
    char keyValueList[AUDIO_RENDER_BUF_TEST] =
        "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000";
    EXPECT_EQ(HDF_SUCCESS, render_->SetExtraParams(render_, keyValueList));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4100, TestSize.Level1)
{
    char keyValueList[AUDIO_RENDER_BUF_TEST];
    uint32_t keyValueListLen = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetExtraParams(nullptr, keyValueList, keyValueListLen));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4200, TestSize.Level1)
{
    uint32_t keyValueListLen = 0;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->GetExtraParams(render_, nullptr, keyValueListLen));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4300, TestSize.Level1)
{
    char keyValueList[AUDIO_RENDER_BUF_TEST] = {};
    uint32_t keyValueListLen = BUFFER_LENTH;
    int32_t ret = render_->GetExtraParams(render_, keyValueList, keyValueListLen);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4400, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->TurnStandbyMode(nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4500, TestSize.Level1)
{
    int32_t ret = render_->Start(render_);
	EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->TurnStandbyMode(render_);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = render_->Stop(render_);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4600, TestSize.Level1)
{
    int32_t range = 4;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AudioDevDump(nullptr, range, -1));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4700, TestSize.Level1)
{
    int32_t range = 4;
    char pathBuf[] = "/data/RenderDump.log";

    FILE *file = fopen(pathBuf, "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    int32_t ret = render_->AudioDevDump(render_, range, fd);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    fclose(file);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4800, TestSize.Level1)
{
    float gain;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetGain(nullptr, &gain));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_4900, TestSize.Level1)
{
    float gain;
    int32_t ret = render_->GetGain(render_, &gain);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5000, TestSize.Level1)
{
    float min = 0.0;
    float max = 1.0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetGainThreshold(nullptr, &min, &max));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5100, TestSize.Level1)
{
    float min = 0.0;
    float max = 1.0;
    int32_t ret = render_->GetGainThreshold(render_, &min, &max);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    EXPECT_GE(min, MIN_GAINTHRESHOLD);
    EXPECT_LE(max, MAX_GAINTHRESHOLD);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5200, TestSize.Level1)
{
    bool isMute = false;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMute(nullptr, &isMute));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5300, TestSize.Level1)
{
    bool isMute = false;
    int32_t ret = render_->GetMute(render_, &isMute);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5400, TestSize.Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetVersion(nullptr, &majorVer, &minorVer));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5500, TestSize.Level1)
{
    uint32_t majorVer;
    uint32_t minorVer;
    ASSERT_EQ(HDF_SUCCESS, render_->GetVersion(render_, &majorVer, &minorVer));
    EXPECT_EQ(IAUDIO_MANAGER_MAJOR_VERSION, majorVer);
    EXPECT_EQ(IAUDIO_MANAGER_MINOR_VERSION, minorVer);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5600, TestSize.Level1)
{
    float val = 0.0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetVolume(nullptr, &val));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5700, TestSize.Level1)
{
    float val = 0.0;
    int32_t ret = render_->GetVolume(render_, &val);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5800, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Pause(nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_5900, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->Resume(nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6000, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->Resume(render_));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6100, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t ret = render_->Pause(render_);

    if (ret == HDF_SUCCESS) {
        EXPECT_EQ(HDF_SUCCESS, render_->Resume(render_));
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_TRUE(false);
    }

    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6200, TestSize.Level1)
{
    uint32_t frameLen = DEFAULT_BUFFER_SIZE;
    uint64_t requestBytes = frameLen;
    int8_t *frame = (int8_t *)calloc(1, frameLen);
    ASSERT_NE(nullptr, frame);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RenderFrame(nullptr, frame, frameLen, &requestBytes));
    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6300, TestSize.Level1)
{
    uint32_t frameLen = (uint64_t)GetRenderBufferSize();
    uint64_t requestBytes = frameLen;
    EXPECT_EQ(HDF_SUCCESS, render_->Start(render_));

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    ASSERT_NE(nullptr, frame);
    EXPECT_EQ(HDF_SUCCESS, render_->RenderFrame(render_, frame, frameLen, &requestBytes));

    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6400, TestSize.Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetChannelMode(nullptr, mode));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6500, TestSize.Level1)
{
    AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    int32_t ret = render_->SetChannelMode(render_, mode);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6600, TestSize.Level1)
{
    float gain = 1.0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetGain(nullptr, gain));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6700, TestSize.Level1)
{
    float gain = 1.0;
    int32_t ret = render_->SetGain(render_, gain);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6800, TestSize.Level1)
{
    bool mute = false;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetMute(nullptr, mute));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_6900, TestSize.Level1)
{
    bool mute = false;
    int32_t ret = render_->SetMute(render_, mute);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7000, TestSize.Level1)
{
    float speed = 2.0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetRenderSpeed(nullptr, speed));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7100, TestSize.Level1)
{
    float speed = 2.0;
    EXPECT_NE(HDF_SUCCESS, render_->SetRenderSpeed(render_, speed));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7200, TestSize.Level1)
{
    float volume = 0.2;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->SetVolume(nullptr, volume));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7300, TestSize.Level1)
{
    float volume = -1.0;
    EXPECT_NE(HDF_SUCCESS, render_->SetVolume(render_, volume));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7400, TestSize.Level1)
{
    float volume = 2.0;
    EXPECT_NE(HDF_SUCCESS, render_->SetVolume(render_, volume));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7500, TestSize.Level1)
{
    float volume = 0.2;
    int32_t ret = render_->SetVolume(render_, volume);

    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7600, TestSize.Level1)
{
    float speed = 0.0;
    ASSERT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetRenderSpeed(nullptr, &speed));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->GetRenderSpeed(render_, nullptr));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7700, TestSize.Level1)
{
    float speed = 0.0;
    ASSERT_EQ(HDF_SUCCESS, render_->Start(render_));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->GetRenderSpeed(render_, &speed));
    EXPECT_EQ(HDF_SUCCESS, render_->Stop(render_));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7800, TestSize.Level1)
{
    enum AudioChannelMode channelMode = AUDIO_CHANNEL_NORMAL;
    int32_t ret = render_->GetChannelMode(nullptr, &channelMode);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetChannelMode(render_, nullptr);
    ASSERT_TRUE(ret == HDF_ERR_INVALID_PARAM || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_7900, TestSize.Level1)
{
    enum AudioChannelMode channelMode = AUDIO_CHANNEL_NORMAL;
    int32_t ret = render_->GetChannelMode(render_, &channelMode);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8000, TestSize.Level1)
{
    int8_t cookie = 0;
    struct IAudioCallback *audioCallback = nullptr;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RegCallback(nullptr, audioCallback, cookie));
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, render_->RegCallback(render_, nullptr, cookie));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8100, TestSize.Level1)
{
    int8_t cookie = 0;
    struct IAudioCallback *audioCallback = nullptr;
    int32_t ret = render_->RegCallback(render_, audioCallback, cookie);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8200, TestSize.Level1)
{
    enum AudioDrainNotifyType type = AUDIO_DRAIN_NORMAL_MODE;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->DrainBuffer(nullptr, &type));
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->DrainBuffer(render_, nullptr));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8300, TestSize.Level1)
{
    enum AudioDrainNotifyType type = AUDIO_DRAIN_NORMAL_MODE;
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, render_->DrainBuffer(render_, &type));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8400, TestSize.Level1)
{
    bool support = false;
    int32_t ret = render_->IsSupportsDrain(nullptr, &support);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);
    ret = render_->IsSupportsDrain(render_, nullptr);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8500, TestSize.Level1)
{
    bool support = false;
    int32_t ret = render_->IsSupportsDrain(render_, &support);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8600, TestSize.Level1)
{
    uint64_t effectId = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->AddAudioEffect(nullptr, effectId));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8700, TestSize.Level1)
{
    uint64_t effectId = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->RemoveAudioEffect(nullptr, effectId));
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8800, TestSize.Level1)
{
    uint64_t bufferSize = BUFFER_LENTH;
    int32_t ret = render_->GetFrameBufferSize(nullptr, &bufferSize);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->GetFrameBufferSize(render_, nullptr);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_8900, TestSize.Level1)
{
    uint64_t bufferSize = BUFFER_LENTH;
    int32_t ret = render_->GetFrameBufferSize(render_, &bufferSize);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_9000, TestSize.Level1)
{
    bool supportPause = false;
    bool supportResume = false;
    int32_t ret = render_->IsSupportsPauseAndResume(nullptr, &supportPause, &supportResume);
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, ret);

    ret = render_->IsSupportsPauseAndResume(render_, nullptr, &supportResume);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_INVALID_PARAM);

    ret = render_->IsSupportsPauseAndResume(render_, &supportPause, nullptr);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_INVALID_PARAM);
}

HWTEST_F(AudioUtRenderTest, SUB_Driver_Audio_RenderHdi_9100, TestSize.Level1)
{
    bool supportPause = false;
    bool supportResume = false;
    int32_t ret = render_->IsSupportsPauseAndResume(render_, &supportPause, &supportResume);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_ERR_INVALID_PARAM);
}
} // end of namespace