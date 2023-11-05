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
#include "v1_0/iaudio_capture.h"
#include "v1_0/iaudio_manager.h"
#include <climits>
#include <gtest/gtest.h>

using namespace std;
using namespace testing::ext;
namespace {
static const uint32_t MAX_AUDIO_ADAPTER_NUM = 5;
const int BUFFER_LENTH = 1024 * 16;
const int DEEP_BUFFER_CAPTURE_PERIOD_SIZE = 4 * 1024;
const int DEFAULT_BUFFER_SIZE = 16384;
const float HALF_OF_MAX_VOLUME = 0.5;
const int MOVE_LEFT_NUM = 8;
const int TEST_SAMPLE_RATE_MASK_48000 = 48000;
const int TEST_CHANNEL_COUNT = 2;

class AudioUtCaptureTestAdditional : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct IAudioCapture *capture_ = nullptr;
    uint32_t captureId_ = 0;
    struct AudioAdapterDescriptor *adapterDescs_ = nullptr;
    virtual void SetUp();
    virtual void TearDown();
    uint64_t GetCaptureBufferSize();
    void InitCaptureDevDesc(struct AudioDeviceDescriptor &devDesc);
    void InitCaptureAttrs(struct AudioSampleAttributes &attrs);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen);
};

/* common method for capture ceate */
uint64_t AudioUtCaptureTestAdditional::GetCaptureBufferSize()
{
    int32_t ret = HDF_SUCCESS;
    uint64_t frameSize = 0;
    uint64_t frameCount = 0;
    uint64_t bufferSize = 0;

    if (capture_ == nullptr) {
        return DEFAULT_BUFFER_SIZE;
    }

    ret = capture_->GetFrameSize(capture_, &frameSize);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }

    ret = capture_->GetFrameCount(capture_, &frameCount);
    if (ret != HDF_SUCCESS) {
        return DEFAULT_BUFFER_SIZE;
    }

    bufferSize = frameCount * frameSize;
    if (bufferSize == 0) {
        bufferSize = DEFAULT_BUFFER_SIZE;
    }

    return bufferSize;
}

void AudioUtCaptureTestAdditional::InitCaptureDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    devDesc.pins = (enum AudioPortPin)PIN_IN_MIC;
    devDesc.desc = strdup("cardname");

    ASSERT_NE(adapterDescs_, nullptr);
    ASSERT_NE(adapterDescs_->ports, nullptr);
    for (uint32_t index = 0; index < adapterDescs_->portsLen; index++) {
        if (adapterDescs_->ports[index].dir == PORT_IN) {
            devDesc.portId = adapterDescs_->ports[index].portId;
            return;
        }
    }
}

void AudioUtCaptureTestAdditional::InitCaptureAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
    attrs.channelCount = TEST_CHANNEL_COUNT;
    attrs.sampleRate = TEST_SAMPLE_RATE_MASK_48000;
    attrs.interleaved = 0;
    attrs.type = AUDIO_IN_MEDIA;
    attrs.period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE;
    attrs.frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM);
    attrs.stopThreshold = INT_MAX;
    attrs.silenceThreshold = BUFFER_LENTH;
}

void AudioUtCaptureTestAdditional::FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
{
    if (dataBlock == nullptr) {
        return;
    }

    if (dataBlock->adapterName != nullptr) {
        OsalMemFree(dataBlock->adapterName);
        dataBlock->adapterName = nullptr;
    }

    if (dataBlock->ports != nullptr) {
        OsalMemFree(dataBlock->ports);
    }

    if (freeSelf) {
        OsalMemFree(dataBlock);
    }
}

void AudioUtCaptureTestAdditional::ReleaseAllAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen)
{
    if ((descsLen > 0) && (descs != nullptr) && ((*descs) != nullptr)) {
        for (uint32_t i = 0; i < descsLen; i++) {
            FreeAdapterElements(&(*descs)[i], false);
        }
        OsalMemFree(*descs);
        *descs = nullptr;
    }
}

void AudioUtCaptureTestAdditional::SetUp()
{
    uint32_t size = MAX_AUDIO_ADAPTER_NUM;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes attrs = {};

    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    adapterDescs_ =
        (struct AudioAdapterDescriptor *)OsalMemCalloc(sizeof(struct AudioAdapterDescriptor) * (MAX_AUDIO_ADAPTER_NUM));
    ASSERT_NE(adapterDescs_, nullptr);

    EXPECT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size));
    if (size > MAX_AUDIO_ADAPTER_NUM) {
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        ASSERT_LT(size, MAX_AUDIO_ADAPTER_NUM);
    }

    EXPECT_EQ(HDF_SUCCESS, manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_));
    if (adapter_ == nullptr) {
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
        EXPECT_NE(adapter_, nullptr);
    }

    InitCaptureDevDesc(devDesc);
    InitCaptureAttrs(attrs);
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devDesc, &attrs, &capture_, &captureId_));
    if (capture_ == nullptr) {
        (void)manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName);
        ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);
    }
    ASSERT_NE(capture_, nullptr);
}

void AudioUtCaptureTestAdditional::TearDown()
{
    ASSERT_NE(capture_, nullptr);
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));

    ASSERT_NE(manager_, nullptr);
    EXPECT_EQ(HDF_SUCCESS, manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName));
    ReleaseAllAdapterDescs(&adapterDescs_, MAX_AUDIO_ADAPTER_NUM);

    IAudioManagerRelease(manager_, false);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetVolume_0400
 * @tc.name   : testAudioCaptureSetVolume001
 * @tc.desc   : test API SetVolume input parameter volume = 0.0
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetVolume001, TestSize.Level1)
{
    float volume = 0.0;
    EXPECT_NE(capture_->SetVolume, nullptr);

    int32_t ret = capture_->SetVolume(capture_, volume);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetVolume_0500
 * @tc.name   : testAudioCaptureSetVolume002
 * @tc.desc   : test API SetVolume input parameter volume = 1.0
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetVolume002, TestSize.Level1)
{
    float volume = 1.0;
    EXPECT_NE(capture_->SetVolume, nullptr);

    int32_t ret = capture_->SetVolume(capture_, volume);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetVolume_0600
 * @tc.name   : testAudioCaptureSetVolume003
 * @tc.desc   : test API SetVolume input parameter volume = -0.1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetVolume003, TestSize.Level2)
{
    float volume = -0.1;
    EXPECT_NE(capture_->SetVolume, nullptr);

    int32_t ret = capture_->SetVolume(capture_, volume);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetVolume_0700
 * @tc.name   : testAudioCaptureSetVolume004
 * @tc.desc   : test API SetVolume input parameter volume = 1.1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetVolume004, TestSize.Level2)
{
    float volume = 1.1;
    EXPECT_NE(capture_->SetVolume, nullptr);

    int32_t ret = capture_->SetVolume(capture_, volume);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetVolume_0800
 * @tc.name   : testAudioCaptureSetVolume005
 * @tc.desc   : test API SetVolume 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetVolume005, TestSize.Level1)
{
    float volume = 0.8;
    int i = 0;
    int32_t ret = HDF_ERR_NOT_SUPPORT;
    EXPECT_NE(capture_->SetVolume, nullptr);
    for (i = 0; i < 1000; i++) {
        ret = capture_->SetVolume(capture_, volume);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetVolume_0300
 * @tc.name   : testAudioCaptureGetVolume001
 * @tc.desc   : test API GetVolume 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetVolume001, TestSize.Level1)
{
    float volume = HALF_OF_MAX_VOLUME;
    int i = 0;
    EXPECT_NE(capture_->SetVolume, nullptr);

    int32_t ret = capture_->SetVolume(capture_, volume);
    if (ret == HDF_SUCCESS) {
        for (i = 0; i < 1000; i++) {
            ret = capture_->GetVolume(capture_, &volume);
            ASSERT_EQ(volume, HALF_OF_MAX_VOLUME);
        }
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_TRUE(false);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetMute_0400
 * @tc.name   : testAudioCaptureSetMute001
 * @tc.desc   : test API GetMute SetMute
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetMute001, TestSize.Level1)
{
    bool isSupport = true;
    ASSERT_NE(capture_->SetMute, nullptr);
    ASSERT_NE(capture_->GetMute, nullptr);

    int32_t ret = capture_->SetMute(capture_, isSupport);
    if (ret == HDF_SUCCESS) {
        ret = capture_->GetMute(capture_, &isSupport);
        ASSERT_EQ(isSupport, true);
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_TRUE(false);
    }

    isSupport = false;
    ret = capture_->SetMute(capture_, isSupport);
    if (ret == HDF_SUCCESS) {
        ret = capture_->GetMute(capture_, &isSupport);
        ASSERT_EQ(isSupport, false);
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_TRUE(false);
    }
    isSupport = true;
    ret = capture_->GetMute(capture_, &isSupport);
    ASSERT_EQ(isSupport, false);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetMute_0500
 * @tc.name   : testAudioCaptureSetMute002
 * @tc.desc   : test API SetMute Audio Handle is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetMute002, TestSize.Level2)
{
    EXPECT_NE(capture_->SetMute, nullptr);

    int32_t ret = capture_->SetMute(nullptr, false);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetMute_0600
 * @tc.name   : testAudioCaptureSetMute003
 * @tc.desc   : test API SetMute 1000times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetMute003, TestSize.Level1)
{
    int i = 0;
    int32_t ret = HDF_ERR_NOT_SUPPORT;
    EXPECT_NE(capture_->SetMute, nullptr);
    for (i = 0; i < 1000; i++) {
        ret = capture_->SetMute(capture_, false);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetMute_0300
 * @tc.name   : testAudioCaptureGetMute001
 * @tc.desc   : test API GetMute Audio Handle is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetMute001, TestSize.Level2)
{
    bool isSupport = false;
    EXPECT_NE(capture_->GetMute, nullptr);

    int32_t ret = capture_->GetMute(nullptr, &isSupport);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetMute_0400
 * @tc.name   : testAudioCaptureGetMute002
 * @tc.desc   : test API GetMute Audio Handle is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetMute002, TestSize.Level2)
{
    bool isSupport = false;
    EXPECT_NE(capture_->GetMute, nullptr);

    int32_t ret = capture_->GetMute(nullptr, &isSupport);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetMute_0500
 * @tc.name   : testAudioCaptureGetMute003
 * @tc.desc   : test API GetMute 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetMute003, TestSize.Level1)
{
    int i = 0;
    bool isSupport = false;
    EXPECT_NE(capture_->SetMute, nullptr);
    EXPECT_NE(capture_->GetMute, nullptr);

    int32_t ret = capture_->SetMute(capture_, true);
    if (ret == HDF_SUCCESS) {
        for (i = 0; i < 1000; i++) {
            ret = capture_->GetMute(capture_, &isSupport);
            ASSERT_EQ(isSupport, true);
        }
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_TRUE(false);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetMute_0600
 * @tc.name   : testAudioCaptureGetMute004
 * @tc.desc   : test API GetMute 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetMute004, TestSize.Level1)
{
    int i = 0;
    bool isSupport = true;
    EXPECT_NE(capture_->SetMute, nullptr);
    EXPECT_NE(capture_->GetMute, nullptr);

    int32_t ret = capture_->SetMute(capture_, false);
    if (ret == HDF_SUCCESS) {
        for (i = 0; i < 1000; i++) {
            ret = capture_->GetMute(capture_, &isSupport);
            ASSERT_EQ(isSupport, false);
        }
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ASSERT_TRUE(true);
    } else {
        ASSERT_TRUE(false);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetGain_0300
 * @tc.name   : testAudioCaptureSetGain001
 * @tc.desc   : test API SetGain param is 0
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetGain001, TestSize.Level1)
{
    float mute = 0;
    EXPECT_NE(capture_->SetGain, nullptr);

    int32_t ret = capture_->SetGain(capture_, mute);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetGain_0400
 * @tc.name   : testAudioCaptureSetGain002
 * @tc.desc   : test API SetGain param is 1.0
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetGain002, TestSize.Level1)
{
    float mute = 1.0;
    EXPECT_NE(capture_->SetGain, nullptr);

    int32_t ret = capture_->SetGain(capture_, mute);
    EXPECT_EQ(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetGain_0500
 * @tc.name   : testAudioCaptureSetGain003
 * @tc.desc   : test API SetGain param is -0.1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetGain003, TestSize.Level2)
{
    float mute = -0.1;
    EXPECT_NE(capture_->SetGain, nullptr);

    int32_t ret = capture_->SetGain(capture_, mute);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetGain_0800
 * @tc.name   : testAudioCaptureSetGain006
 * @tc.desc   : test API SetGain 1000times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetGain006, TestSize.Level1)
{
    float mute = 0.5;
    int i = 0;
    int32_t ret = HDF_ERR_NOT_SUPPORT;

    EXPECT_NE(capture_->SetGain, nullptr);
    for (i = 0; i < 1000; i++) {
        ret = capture_->SetGain(capture_, mute);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSetGain_0900
 * @tc.name   : testAudioCaptureSetGain007
 * @tc.desc   : test API SetGain Audio Handle is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSetGain007, TestSize.Level2)
{
    float mute = 0.5;
    EXPECT_NE(capture_->SetGain, nullptr);

    int32_t ret = capture_->SetGain(nullptr, mute);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetGain_0300
 * @tc.name   : testAudioCaptureGetGain001
 * @tc.desc   : test API GetGain 1000times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetGain001, TestSize.Level1)
{
    float gain = 2.0;
    int i = 0;
    int32_t ret = 0;
    EXPECT_NE(capture_->SetGain, nullptr);
    for (i = 0; i < 1000; i++) {
        ret = capture_->GetGain(capture_, &gain);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetGainThreshold_0300
 * @tc.name   : testAudioCaptureGetGainThreshold001
 * @tc.desc   : test API GetGainThreshold
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetGainThreshold001, TestSize.Level2)
{
    float top = 0;
    EXPECT_NE(capture_->GetGainThreshold, nullptr);

    int32_t ret = capture_->GetGainThreshold(nullptr, nullptr, &top);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetGainThreshold_0400
 * @tc.name   : testAudioCaptureGetGainThreshold002
 * @tc.desc   : test API GetGainThreshold
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetGainThreshold002, TestSize.Level2)
{
    float bottom = 0;
    EXPECT_NE(capture_->GetGainThreshold, nullptr);

    int32_t ret = capture_->GetGainThreshold(capture_, &bottom, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetGainThreshold_0500
 * @tc.name   : testAudioCaptureGetGainThreshold003
 * @tc.desc   : test API GetGainThreshold
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetGainThreshold003, TestSize.Level2)
{
    float top = 0;
    EXPECT_NE(capture_->GetGainThreshold, nullptr);

    int32_t ret = capture_->GetGainThreshold(capture_, nullptr, &top);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetGainThreshold_0600
 * @tc.name   : testAudioCaptureGetGainThreshold004
 * @tc.desc   : test API GetGainThreshold
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetGainThreshold004, TestSize.Level2)
{
    float bottom = 0;
    EXPECT_NE(capture_->GetGainThreshold, nullptr);

    int32_t ret = capture_->GetGainThreshold(nullptr, &bottom, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetGainThreshold_0700
 * @tc.name   : testAudioCaptureGetGainThreshold005
 * @tc.desc   : test API GetGainThreshold
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetGainThreshold005, TestSize.Level1)
{
    float bottom = 0;
    float top = 0;
    int32_t ret = HDF_ERR_NOT_SUPPORT;
    int i = 0;
    EXPECT_NE(capture_->GetGainThreshold, nullptr);
    for (i = 0; i < 1000; i++) {
        ret = capture_->GetGainThreshold(capture_, &bottom, &top);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_0300
 * @tc.name   : testAudioCaptureSelectScene001
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene001, TestSize.Level1)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_MEDIA;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_0400
 * @tc.name   : testAudioCaptureSelectScene002
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene002, TestSize.Level1)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_COMMUNICATION;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_0600
 * @tc.name   : testAudioCaptureSelectScene004
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene004, TestSize.Level1)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_MEDIA;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_0700
 * @tc.name   : testAudioCaptureSelectScene005
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene005, TestSize.Level1)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_COMMUNICATION;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_0900
 * @tc.name   : testAudioCaptureSelectScene007
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene007, TestSize.Level1)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_CALL;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1000
 * @tc.name   : testAudioCaptureSelectScene008
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene008, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_MEDIA;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1100
 * @tc.name   : testAudioCaptureSelectScene009
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene009, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_COMMUNICATION;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1200
 * @tc.name   : testAudioCaptureSelectScene010
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene010, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_RINGTONE;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1300
 * @tc.name   : testAudioCaptureSelectScene011
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene011, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_CALL;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1400
 * @tc.name   : testAudioCaptureSelectScene012
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene012, TestSize.Level1)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    int i = 0;
    int32_t ret = HDF_ERR_NOT_SUPPORT;
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_CALL;
    for (i = 0; i < 1000; i++) {
        ret = capture_->SelectScene(capture_, &sceneDesc);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1500
 * @tc.name   : testAudioCaptureSelectScene013
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene013, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_OUT_SPEAKER;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_MEDIA;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1600
 * @tc.name   : testAudioCaptureSelectScene014
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene014, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_OUT_SPEAKER;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_COMMUNICATION;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1700
 * @tc.name   : testAudioCaptureSelectScene015
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene015, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_OUT_SPEAKER;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_RINGTONE;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureSelectScene_1800
 * @tc.name   : testAudioCaptureSelectScene016
 * @tc.desc   : test API SelectScene
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureSelectScene016, TestSize.Level2)
{
    ASSERT_NE(capture_->SelectScene, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_OUT_SPEAKER;
    sceneDesc.desc.desc = strdup("speaker");
    sceneDesc.scene.id = AUDIO_IN_CALL;

    int32_t ret = capture_->SelectScene(capture_, &sceneDesc);
    EXPECT_NE(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCheckSceneCapability_0300
 * @tc.name   : testAudioCaptureCheckSceneCapability001
 * @tc.desc   : test API CheckSceneCapability
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCheckSceneCapability001, TestSize.Level1)
{
    ASSERT_NE(capture_->CheckSceneCapability, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_MEDIA;
    bool isSupport = false;

    int32_t ret = capture_->CheckSceneCapability(capture_, &sceneDesc, &isSupport);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCheckSceneCapability_0400
 * @tc.name   : testAudioCaptureCheckSceneCapability002
 * @tc.desc   : test API CheckSceneCapability
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCheckSceneCapability002, TestSize.Level1)
{
    ASSERT_NE(capture_->CheckSceneCapability, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    int i = 0;
    int32_t ret;

    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_MEDIA;
    bool isSupport = false;
    for (i = 0; i < 1000; i++) {
        ret = capture_->CheckSceneCapability(capture_, &sceneDesc, &isSupport);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCheckSceneCapability_0500
 * @tc.name   : testAudioCaptureCheckSceneCapability003
 * @tc.desc   : test API CheckSceneCapability
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCheckSceneCapability003, TestSize.Level1)
{
    ASSERT_NE(capture_->CheckSceneCapability, nullptr);
    struct AudioSceneDescriptor sceneDesc = {};
    sceneDesc.desc.pins = PIN_IN_MIC;
    sceneDesc.desc.desc = strdup("mic");
    sceneDesc.scene.id = AUDIO_IN_CALL;
    bool isSupport = false;

    int32_t ret = capture_->CheckSceneCapability(capture_, &sceneDesc, &isSupport);
    EXPECT_EQ(ret, HDF_SUCCESS);
    free(sceneDesc.desc.desc);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0300
 * @tc.name   : testAudioCaptureCaptureFrame001
 * @tc.desc   : test API CaptureFrame 1000times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame001, TestSize.Level1)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);
    int i = 0;

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);
    for (i = 0; i < 1000; i++) {
        ret = capture_->CaptureFrame(capture_, frame, &frameLen, &requestBytes);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
    capture_->Stop(capture_);

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0400
 * @tc.name   : testAudioCaptureCaptureFrame002
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame002, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(nullptr, nullptr, nullptr, &requestBytes));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0500
 * @tc.name   : testAudioCaptureCaptureFrame003
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame003, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(nullptr, nullptr, &frameLen, nullptr));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0600
 * @tc.name   : testAudioCaptureCaptureFrame004
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame004, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(nullptr, nullptr, &frameLen, &requestBytes));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0700
 * @tc.name   : testAudioCaptureCaptureFrame005
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame005, TestSize.Level2)
{
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, sizeof(int));
    EXPECT_NE(nullptr, frame);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(nullptr, frame, nullptr, nullptr));

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0800
 * @tc.name   : testAudioCaptureCaptureFrame006
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame006, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, sizeof(int));
    EXPECT_NE(nullptr, frame);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(nullptr, frame, nullptr, &requestBytes));

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_0900
 * @tc.name   : testAudioCaptureCaptureFrame007
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame007, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, sizeof(int));
    EXPECT_NE(nullptr, frame);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(nullptr, frame, &frameLen, nullptr));

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_1000
 * @tc.name   : testAudioCaptureCaptureFrame008
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame008, TestSize.Level2)
{
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(capture_, nullptr, nullptr, nullptr));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_1100
 * @tc.name   : testAudioCaptureCaptureFrame009
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame009, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(capture_, nullptr, nullptr, &requestBytes));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_1200
 * @tc.name   : testAudioCaptureCaptureFrame010
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame010, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(capture_, nullptr, &frameLen, nullptr));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_1300
 * @tc.name   : testAudioCaptureCaptureFrame011
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame011, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(capture_, nullptr, &frameLen, &requestBytes));
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureCaptureFrame_1400
 * @tc.name   : testAudioCaptureCaptureFrame012
 * @tc.desc   : test API CaptureFrame parameter is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureCaptureFrame012, TestSize.Level2)
{
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    ASSERT_NE(capture_->CaptureFrame, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, sizeof(int));
    EXPECT_NE(nullptr, frame);

    EXPECT_NE(HDF_SUCCESS, capture_->CaptureFrame(capture_, frame, nullptr, &requestBytes));

    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetCapturePosition_0300
 * @tc.name   : testAudioCaptureGetCapturePosition001
 * @tc.desc   : test API GetCapturePosition 1000times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetCapturePosition001, TestSize.Level1)
{
    uint64_t frames;
    struct AudioTimeStamp time;
    uint32_t frameLen = (uint64_t)GetCaptureBufferSize();
    uint64_t requestBytes = frameLen;
    int i = 0;
    ASSERT_NE(capture_->CaptureFrame, nullptr);
    ASSERT_NE(capture_->GetCapturePosition, nullptr);

    int32_t ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);

    int8_t *frame = (int8_t *)calloc(1, frameLen);
    EXPECT_NE(nullptr, frame);

    ret = capture_->CaptureFrame(capture_, frame, &frameLen, &requestBytes);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (i = 0; i < 1000; i++) {
        ret = capture_->GetCapturePosition(capture_, &frames, &time);
        ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    }

    capture_->Stop(capture_);
    if (frame != nullptr) {
        free(frame);
        frame = nullptr;
    }
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetCapturePosition_0400
 * @tc.name   : testAudioCaptureGetCapturePosition002
 * @tc.desc   : test API GetCapturePosition All parameters are nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetCapturePosition002, TestSize.Level2)
{
    int32_t ret;
    ASSERT_NE(capture_->GetCapturePosition, nullptr);
    ret = capture_->GetCapturePosition(nullptr, nullptr, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetCapturePosition_0500
 * @tc.name   : testAudioCaptureGetCapturePosition003
 * @tc.desc   : test API GetCapturePosition input capture and frames are nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetCapturePosition003, TestSize.Level2)
{
    int32_t ret;
    struct AudioTimeStamp time;
    ASSERT_NE(capture_->GetCapturePosition, nullptr);
    ret = capture_->GetCapturePosition(nullptr, nullptr, &time);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetCapturePosition_0600
 * @tc.name   : testAudioCaptureGetCapturePosition004
 * @tc.desc   : test API GetCapturePosition input capture and time are nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetCapturePosition004, TestSize.Level2)
{
    int32_t ret;
    uint64_t frames;
    ASSERT_NE(capture_->GetCapturePosition, nullptr);

    ret = capture_->GetCapturePosition(nullptr, &frames, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetCapturePosition_0700
 * @tc.name   : testAudioCaptureGetCapturePosition005
 * @tc.desc   : test API GetCapturePosition input capture is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetCapturePosition005, TestSize.Level2)
{
    int32_t ret;
    uint64_t frames;
    struct AudioTimeStamp time;
    ASSERT_NE(capture_->GetCapturePosition, nullptr);

    ret = capture_->GetCapturePosition(nullptr, &frames, &time);
    EXPECT_NE(ret, HDF_SUCCESS);
}
/**
 * @tc.number : SUB_Driver_Audio_CaptureGetCapturePosition_0800
 * @tc.name   : testAudioCaptureGetCapturePosition006
 * @tc.desc   : test API GetCapturePosition input frames is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testAudioCaptureGetCapturePosition006, TestSize.Level2)
{
    int32_t ret;
    struct AudioTimeStamp time;
    ASSERT_NE(capture_->GetCapturePosition, nullptr);

    ret = capture_->GetCapturePosition(capture_, nullptr, &time);
    EXPECT_NE(ret, HDF_SUCCESS);
}
} // namespace
