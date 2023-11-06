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

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameSize_0100
 * @tc.name   : testCaptureGetFrameSize001
 * @tc.desc   : ret = HDF_SUCCESS
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameSize001, Function | MediumTest | Level1)
{
    uint64_t frameSize = 0;
    int32_t ret = 0;

    ret = capture_->GetFrameSize(capture_, &frameSize);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameSize_0200
 * @tc.name   : testCaptureGetFrameSize002
 * @tc.desc   : Call 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameSize002, Function | MediumTest | Level1)
{
    uint64_t frameSize = 0;
    int32_t ret = 0;

    for (int i = 0; i < 1000; i++) {
        ret = capture_->GetFrameSize(capture_, &frameSize);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameSize_0300
 * @tc.name   : testCaptureGetFrameSize003
 * @tc.desc   : capture_ is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameSize003, Function | MediumTest | Level2)
{
    uint64_t frameSize = 0;
    int32_t ret = 0;

    ret = capture_->GetFrameSize(nullptr, &frameSize);
    EXPECT_EQ(ret, HDF_ERR_INVALID_OBJECT);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameSize_0400
 * @tc.name   : testCaptureGetFrameSize004
 * @tc.desc   : &frameSize is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameSize004, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    ret = capture_->GetFrameSize(capture_, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameCount_0100
 * @tc.name   : testCaptureGetFrameCount001
 * @tc.desc   : ret = HDF_SUCCESS
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameCount001, Function | MediumTest | Level1)
{
    uint64_t frameCount = 0;
    int32_t ret = 0;

    ret = capture_->GetFrameCount(capture_, &frameCount);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameCount_0200
 * @tc.name   : testCaptureGetFrameCount002
 * @tc.desc   : Call 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameCount002, Function | MediumTest | Level1)
{
    uint64_t frameCount = 0;
    int32_t ret = 0;

    for (int i = 0; i < 1000; i++) {
        ret = capture_->GetFrameCount(capture_, &frameCount);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameCount_0300
 * @tc.name   : testCaptureGetFrameCount003
 * @tc.desc   : capture_ is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameCount003, Function | MediumTest | Level2)
{
    uint64_t frameCount = 0;
    int32_t ret = 0;

    ret = capture_->GetFrameCount(nullptr, &frameCount);
    EXPECT_EQ(ret, HDF_ERR_INVALID_OBJECT);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetFrameCount_0400
 * @tc.name   : testCaptureGetFrameCount004
 * @tc.desc   : &frameCount is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetFrameCount004, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    ret = capture_->GetFrameCount(capture_, nullptr);
    EXPECT_EQ(ret, HDF_ERR_INVALID_PARAM);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0200
 * @tc.name   : testCaptureSetSampleAttributes001
 * @tc.desc   : ret = success
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes001, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0300
 * @tc.name   : testCaptureSetSampleAttributes002
 * @tc.desc   : format is AUDIO_FORMAT_TYPE_PCM_8_BIT
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes002, Function | MediumTest | Level2)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_8_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0400
 * @tc.name   : testCaptureSetSampleAttributes003
 * @tc.desc   : sampleRate is 44000
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes003, Function | MediumTest | Level2)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = 44000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0500
 * @tc.name   : testCaptureSetSampleAttributes004
 * @tc.desc   : channelCount is 3
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes004, Function | MediumTest | Level2)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = 3,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0600
 * @tc.name   : testCaptureSetSampleAttributes005
 * @tc.desc   : channelCount is 1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes005, Function | MediumTest | Level2)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = 1,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0700
 * @tc.name   : testCaptureSetSampleAttributes006
 * @tc.desc   : channelCount is 0
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes006, Function | MediumTest | Level2)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = 0,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_FAILURE);
}
/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0800
 * @tc.name   : testCaptureSetSampleAttributes007
 * @tc.desc   : period is 3 * 1024
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes007, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_COMMUNICATION,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = 3 * 1024,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_0900
 * @tc.name   : testCaptureSetSampleAttributes008
 * @tc.desc   : period is 5 * 1024
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes008, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = 5 * 1024,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_1000
 * @tc.name   : testCaptureSetSampleAttributes009
 * @tc.desc   : frameSize is AUDIO_FORMAT_TYPE_PCM_16_BIT * 3 / MOVE_LEFT_NUM
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes009, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * 3 / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_1100
 * @tc.name   : testCaptureSetSampleAttributes010
 * @tc.desc   : isBigEndian is true
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes010, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = true,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_1200
 * @tc.name   : testCaptureSetSampleAttributes011
 * @tc.desc   : isSignedData is false
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes011, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = false,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_1300
 * @tc.name   : testCaptureSetSampleAttributes012
 * @tc.desc   : startThreshold is (5 * 1024) / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM)
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes012, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = (5 * 1024) / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_1400
 * @tc.name   : testCaptureSetSampleAttributes013
 * @tc.desc   : stopThreshold is INT_MAX - 1024
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes013, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX - 1024,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_SetSampleAttributes_1500
 * @tc.name   : testCaptureSetSampleAttributes014
 * @tc.desc   : silenceThreshold is 1024
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureSetSampleAttributes014, Function | MediumTest | Level2)
{
    struct AudioSampleAttributes attrs = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = false,
        .isSignedData = true,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = INT_MAX,
        .silenceThreshold = 1024,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_FAILURE);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetSampleAttributes_0300
 * @tc.name   : testCaptureGetSampleAttributes001
 * @tc.desc   : ret = success, the obtained property values are correct
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetSampleAttributes001, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrs = {};

    int32_t ret = capture_->GetSampleAttributes(capture_, &attrs);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(attrs.format, AUDIO_FORMAT_TYPE_PCM_16_BIT);
    EXPECT_EQ(attrs.sampleRate, TEST_SAMPLE_RATE_MASK_48000);
    EXPECT_EQ(attrs.channelCount, TEST_CHANNEL_COUNT);
    EXPECT_EQ(attrs.interleaved, 0);
    EXPECT_EQ(attrs.type, AUDIO_IN_MEDIA);
    EXPECT_EQ(attrs.period, DEEP_BUFFER_CAPTURE_PERIOD_SIZE);
    EXPECT_EQ(attrs.frameSize, AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM);
    EXPECT_EQ(attrs.isBigEndian, false);
    EXPECT_EQ(attrs.isSignedData, true);
    EXPECT_EQ(attrs.startThreshold,
              DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM));
    EXPECT_EQ(attrs.stopThreshold, INT_MAX);
    EXPECT_EQ(attrs.silenceThreshold, BUFFER_LENTH);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetSampleAttributes_0400
 * @tc.name   : testCaptureGetSampleAttributes002
 * @tc.desc   : Change the isBigEndian, isSignedData, and stopThreshold attribute values to get attribute values correct
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetSampleAttributes002, Function | MediumTest | Level1)
{
    struct AudioSampleAttributes attrsGet = {};
    struct AudioSampleAttributes attrsSet = {
        .type = AUDIO_IN_MEDIA,
        .interleaved = 0,
        .format = AUDIO_FORMAT_TYPE_PCM_16_BIT,
        .sampleRate = TEST_SAMPLE_RATE_MASK_48000,
        .channelCount = TEST_CHANNEL_COUNT,
        .period = DEEP_BUFFER_CAPTURE_PERIOD_SIZE,
        .frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * TEST_CHANNEL_COUNT / MOVE_LEFT_NUM,
        .isBigEndian = true,
        .isSignedData = false,
        .startThreshold = DEEP_BUFFER_CAPTURE_PERIOD_SIZE / (attrsSet.format * attrsSet.channelCount / MOVE_LEFT_NUM),
        .stopThreshold = 15 * 1024,
        .silenceThreshold = BUFFER_LENTH,
    };
    int32_t ret = 0;

    ret = capture_->SetSampleAttributes(capture_, &attrsSet);
    EXPECT_EQ(ret, HDF_SUCCESS);

    ret = capture_->GetSampleAttributes(capture_, &attrsGet);
    EXPECT_EQ(ret, HDF_SUCCESS);
    EXPECT_EQ(attrsGet.isBigEndian, true);
    EXPECT_EQ(attrsGet.isSignedData, false);
    EXPECT_EQ(attrsGet.stopThreshold, 15 * 1024);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_ReqMmapBuffer_0100
 * @tc.name   : testCaptureReqMmapBuffer001
 * @tc.desc   : ret = success or ret = not_support
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureReqMmapBuffer001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    struct AudioMmapBufferDescriptor desc = {};

    ret = capture_->ReqMmapBuffer(capture_, MMAP_SUGGUEST_REQ_SIZE, &desc);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_ReqMmapBuffer_0200
 * @tc.name   : testCaptureReqMmapBuffer002
 * @tc.desc   : capture_ is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureReqMmapBuffer002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    struct AudioMmapBufferDescriptor desc = {};

    ret = capture_->ReqMmapBuffer(nullptr, MMAP_SUGGUEST_REQ_SIZE, &desc);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_ReqMmapBuffer_0300
 * @tc.name   : testCaptureReqMmapBuffer003
 * @tc.desc   : MMAP_SUGGUEST_REQ_SIZE is 0
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureReqMmapBuffer003, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    struct AudioMmapBufferDescriptor desc = {};

    ret = capture_->ReqMmapBuffer(capture_, 0, &desc);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_ReqMmapBuffer_0400
 * @tc.name   : testCaptureReqMmapBuffer004
 * @tc.desc   : MMAP_SUGGUEST_REQ_SIZE is -1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureReqMmapBuffer004, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    struct AudioMmapBufferDescriptor desc = {};

    ret = capture_->ReqMmapBuffer(capture_, -1, &desc);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_ReqMmapBuffer_0500
 * @tc.name   : testCaptureReqMmapBuffer005
 * @tc.desc   : MMAP_SUGGUEST_REQ_SIZE is 2147483647
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureReqMmapBuffer005, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    struct AudioMmapBufferDescriptor desc = {};

    ret = capture_->ReqMmapBuffer(capture_, 2147483647, &desc);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_ReqMmapBuffer_0600
 * @tc.name   : testCaptureReqMmapBuffer006
 * @tc.desc   : &desc is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureReqMmapBuffer006, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    ret = capture_->ReqMmapBuffer(capture_, MMAP_SUGGUEST_REQ_SIZE, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetMmapPosition_0100
 * @tc.name   : testCaptureGetMmapPosition001
 * @tc.desc   : capture_ is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetMmapPosition001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    uint64_t frames = 0;
    struct AudioTimeStamp time = {0};

    ret = capture_->GetMmapPosition(nullptr, &frames, &time);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetMmapPosition_0200
 * @tc.name   : testCaptureGetMmapPosition002
 * @tc.desc   : &frames is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetMmapPosition002, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    struct AudioTimeStamp time = {0};

    ret = capture_->GetMmapPosition(capture_, nullptr, &time);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_GetMmapPosition_0300
 * @tc.name   : testCaptureGetMmapPosition003
 * @tc.desc   : &time is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureGetMmapPosition003, Function | MediumTest | Level2)
{
    int32_t ret = 0;
    uint64_t frames = 0;

    ret = capture_->GetMmapPosition(capture_, &frames, nullptr);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_Pause_0500
 * @tc.name   : testCapturePause001
 * @tc.desc   : Direct call, ret != success
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCapturePause001, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    ret = capture_->Pause(capture_);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_Pause_0600
 * @tc.name   : testCapturePause002
 * @tc.desc   : After calling start and stop, pause is called
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCapturePause002, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = capture_->Pause(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = capture_->Stop(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_Stop_0300
 * @tc.name   : testCaptureStop001
 * @tc.desc   : Direct call, ret = success
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureStop001, Function | MediumTest | Level1)
{
    int32_t ret = 0;

    ret = capture_->Stop(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_Stop_0400
 * @tc.name   : testCaptureStop002
 * @tc.desc   : Loop through start and stop 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureStop002, Function | MediumTest | Level1)
{
    int32_t ret = 0;

    for (int32_t i = 0; i < 1000; i++) {
        ret = capture_->Start(capture_);
        EXPECT_EQ(ret, HDF_SUCCESS);
        ret = capture_->Stop(capture_);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_Resume_0400
 * @tc.name   : testCaptureResume001
 * @tc.desc   : Loop through Pause and Resume 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureResume001, Function | MediumTest | Level1)
{
    int32_t ret = 0;

    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    for (int32_t i = 0; i < 1000; i++) {
        ret = capture_->Pause(capture_);
        EXPECT_EQ(ret, HDF_SUCCESS);
        ret = capture_->Resume(capture_);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
    ret = capture_->Stop(capture_);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_TurnStandbyMode_0400
 * @tc.name   : testCaptureTurnStandbyMode001
 * @tc.desc   : Call start, then call TurnStandbyMode
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureTurnStandbyMode001, Function | MediumTest | Level1)
{
    int32_t ret = 0;

    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    sleep(1);
    ret = capture_->TurnStandbyMode(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = capture_->Stop(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_TurnStandbyMode_0500
 * @tc.name   : testCaptureTurnStandbyMode002
 * @tc.desc   : Call TurnStandbyMode first, then start and stop
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureTurnStandbyMode002, Function | MediumTest | Level1)
{
    int32_t ret = 0;

    ret = capture_->TurnStandbyMode(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = capture_->Start(capture_);
    EXPECT_EQ(ret, HDF_SUCCESS);
    ret = capture_->Stop(capture_);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_TurnStandbyMode_0600
 * @tc.name   : testCaptureTurnStandbyMode003
 * @tc.desc   : Loop call 1000 times
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureTurnStandbyMode003, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    for (int32_t i = 0; i < 1000; i++) {
        ret = capture_->TurnStandbyMode(capture_);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0300
 * @tc.name   : testCaptureAudioDevDump001
 * @tc.desc   : Dump to binary file
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump001, Function | MediumTest | Level1)
{
    int32_t range = 3;
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "wb+");
    ASSERT_NE(nullptr, file);
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    ret = capture_->AudioDevDump(capture_, range, fd);
    EXPECT_TRUE(ret == HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0400
 * @tc.name   : testCaptureAudioDevDump002
 * @tc.desc   : Dump to a text file
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump002, Function | MediumTest | Level1)
{
    int32_t range = 3;
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "w+");
    ASSERT_NE(nullptr, file);
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    ret = capture_->AudioDevDump(capture_, range, fd);
    EXPECT_TRUE(ret == HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0500
 * @tc.name   : testCaptureAudioDevDump003
 * @tc.desc   : Dump to a read-only file
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump003, Function | MediumTest | Level1)
{
    int32_t range = 3;
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "w");
    ASSERT_NE(nullptr, file);
    ret = fclose(file);
    ASSERT_EQ(0, ret);

    file = fopen("/data/CaptureDump.log", "r");
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    ret = capture_->AudioDevDump(capture_, range, fd);
    EXPECT_NE(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0600
 * @tc.name   : testCaptureAudioDevDump004
 * @tc.desc   : range is 5
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump004, Function | MediumTest | Level1)
{
    int32_t range = 5;
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "wb+");
    ASSERT_NE(nullptr, file);
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }

    ret = capture_->AudioDevDump(capture_, range, fd);
    EXPECT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0700
 * @tc.name   : testCaptureAudioDevDump005
 * @tc.desc   : capture_ is nullptr
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump005, Function | MediumTest | Level2)
{
    int32_t range = 4;
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "wb+");
    ASSERT_NE(nullptr, file);
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }
    ret = capture_->AudioDevDump(nullptr, range, fd);
    EXPECT_NE(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0800
 * @tc.name   : testCaptureAudioDevDump006
 * @tc.desc   : range is 2
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump006, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "wb+");
    ASSERT_NE(nullptr, file);
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }
    ret = capture_->AudioDevDump(capture_, 2, fd);
    EXPECT_NE(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_0900
 * @tc.name   : testCaptureAudioDevDump007
 * @tc.desc   : range is -1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump007, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "wb+");
    ASSERT_NE(nullptr, file);
    int fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }
    ret = capture_->AudioDevDump(capture_, -1, fd);
    EXPECT_NE(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_1000
 * @tc.name   : testCaptureAudioDevDump008
 * @tc.desc   : range is 2147483647
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump008, Function | MediumTest | Level2)
{
    int32_t ret = 0;

    FILE *file = fopen("/data/CaptureDump.log", "wb+");
    ASSERT_NE(nullptr, file);
    int32_t fd = fileno(file);
    if (fd == -1) {
        fclose(file);
        ASSERT_NE(fd, -1);
    }
    ret = capture_->AudioDevDump(capture_, 2147483647, fd);
    EXPECT_NE(ret, HDF_SUCCESS);
    fclose(file);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_1100
 * @tc.name   : testCaptureAudioDevDump009
 * @tc.desc   : fd is -1
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump009, Function | MediumTest | Level2)
{
    int32_t range = 4;
    int32_t ret = 0;

    ret = capture_->AudioDevDump(capture_, range, -1);
    EXPECT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number : SUB_Driver_Audio_Capture_AudioDevDump_1200
 * @tc.name   : testCaptureAudioDevDump010
 * @tc.desc   : fd is 2147483647
 */
HWTEST_F(AudioUtCaptureTestAdditional, testCaptureAudioDevDump010, Function | MediumTest | Level2)
{
    int32_t range = 4;
    int32_t ret = 0;

    ret = capture_->AudioDevDump(capture_, range, 2147483647);
    EXPECT_NE(ret, HDF_SUCCESS);
}

} // namespace
