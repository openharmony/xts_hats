/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "hdf_dlist.h"
#include "osal_mem.h"
#include "v5_0/iaudio_adapter.h"
#include "v5_0/iaudio_manager.h"
#include <climits>
#include <cstring>
#include <gtest/gtest.h>

using namespace std;
using namespace testing::ext;

#define AUDIO_CHANNELCOUNT 2
#define AUDIO_SAMPLE_RATE_48K 48000
#define DEEP_BUFFER_RENDER_PERIOD_SIZE 4096
#define INT_32_MAX 0x7fffffff
#define PCM_16_BIT 16
#define PCM_8_BIT 8
#define PRIMARY_OUTPUT_STREAM_ID 14

namespace {
static const uint32_t AUDIO_ADAPTER_NUM_MAX = 5;
static const uint32_t AUDIO_ADAPTER_CAPTURE_ID = 11;

class HdfAudioUtAdapterTestAdditional : public testing::Test {
public:
    struct IAudioManager *manager_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct AudioAdapterDescriptor *adapterDescs_ = nullptr;
    uint32_t renderId_ = 0;
    uint32_t captureId_ = 0;
    virtual void SetUp();
    virtual void TearDown();
    void InitAttrs(struct AudioSampleAttributes &attrs);
    void InitDevDesc(struct AudioDeviceDescriptor &devDesc);
    void AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen);
};

void HdfAudioUtAdapterTestAdditional::AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock,
                                                                 bool freeSelf)
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

void HdfAudioUtAdapterTestAdditional::ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen)
{
    if ((descsLen > 0) && (descs != nullptr) && ((*descs) != nullptr)) {
        for (uint32_t i = 0; i < descsLen; i++) {
            AudioAdapterDescriptorFree(&(*descs)[i], false);
        }
        OsalMemFree(*descs);
        *descs = nullptr;
    }
}

void HdfAudioUtAdapterTestAdditional::InitAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.format = AUDIO_FORMAT_TYPE_PCM_16_BIT;
    attrs.channelCount = AUDIO_CHANNELCOUNT;
    attrs.sampleRate = AUDIO_SAMPLE_RATE_48K;
    attrs.interleaved = 1;
    attrs.type = AUDIO_IN_MEDIA;
    attrs.period = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    attrs.frameSize = PCM_16_BIT * attrs.channelCount / PCM_8_BIT;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE / (attrs.frameSize);
    attrs.stopThreshold = INT_32_MAX;
    attrs.silenceThreshold = 0;
    attrs.streamId = 0;
}

void HdfAudioUtAdapterTestAdditional::InitDevDesc(struct AudioDeviceDescriptor &devDesc)
{
    ASSERT_NE(adapterDescs_, nullptr);
    ASSERT_NE(adapterDescs_->ports, nullptr);
    for (uint32_t index = 0; index < adapterDescs_->portsLen; index++) {
        if (adapterDescs_->ports[index].dir == PORT_OUT) {
            devDesc.portId = adapterDescs_->ports[index].portId;
            return;
        }
    }
}

void HdfAudioUtAdapterTestAdditional::SetUp()
{
    uint32_t size = AUDIO_ADAPTER_NUM_MAX;
    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    adapterDescs_ =
        (struct AudioAdapterDescriptor *)OsalMemCalloc(sizeof(struct AudioAdapterDescriptor) * (AUDIO_ADAPTER_NUM_MAX));
    ASSERT_NE(adapterDescs_, nullptr);

    ASSERT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size));
    if (size > AUDIO_ADAPTER_NUM_MAX) {
        ReleaseAdapterDescs(&adapterDescs_, AUDIO_ADAPTER_NUM_MAX);
        ASSERT_LT(size, AUDIO_ADAPTER_NUM_MAX);
    }

    if (manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_) != HDF_SUCCESS) {
        ReleaseAdapterDescs(&adapterDescs_, AUDIO_ADAPTER_NUM_MAX);
        ASSERT_TRUE(false);
    }
    if (adapter_ == nullptr) {
        ReleaseAdapterDescs(&adapterDescs_, AUDIO_ADAPTER_NUM_MAX);
        ASSERT_TRUE(false);
    }
}

void HdfAudioUtAdapterTestAdditional::TearDown()
{
    ASSERT_NE(manager_, nullptr);
    ASSERT_NE(adapter_, nullptr);

    manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName);
    ReleaseAdapterDescs(&adapterDescs_, AUDIO_ADAPTER_NUM_MAX);
    adapter_ = nullptr;
    IAudioManagerRelease(manager_, false);
    manager_ = nullptr;
}

/**
 * @tc.number: SUB_Driver_Audio_InitAllPorts_0300
 * @tc.name: testInitAllPorts001
 * @tc.desc: Verify the reliability of the InitAllPorts function.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testInitAllPorts001, TestSize.Level1)
{
    for (uint32_t i = 0; i < 1000; i++) {
        EXPECT_EQ(HDF_SUCCESS, adapter_->InitAllPorts(adapter_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_0600
 * @tc.name: testCreateRender001
 * @tc.desc: Verify the CreateRender function when the output parameter is invalid.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender001, TestSize.Level2)
{
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, nullptr, &renderId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_0700
 * @tc.name: testCreateRender002
 * @tc.desc: Verify the CreateRender function when pins is PIN_OUT_HEADSET and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender002, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_HEADSET;
    InitAttrs(attrs);
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_0800
 * @tc.name: testCreateRender003
 * @tc.desc: Verify the CreateRender function when pins is PIN_OUT_LINEOUT and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender003, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_LINEOUT;
    InitAttrs(attrs);
#if defined ALSA_LIB_MODE
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
#else
        EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_0900
 * @tc.name: testCreateRender004
 * @tc.desc: Verify the CreateRender function when pins is PIN_OUT_HDMI and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender004, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_HDMI;
    InitAttrs(attrs);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
#elif
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1000
 * @tc.name: testCreateRender005
 * @tc.desc: Verify the CreateRender function when pins is PIN_IN_MIC and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender005, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_MIC;
    InitAttrs(attrs);
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1100
 * @tc.name: testCreateRender006
 * @tc.desc: Verify the CreateRender function when pins is PIN_IN_HS_MIC and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender006, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_HS_MIC;
    InitAttrs(attrs);
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1200
 * @tc.name: testCreateRender007
 * @tc.desc: Verify the CreateRender function when pins is PIN_IN_LINEIN and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender007, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_LINEIN;
    InitAttrs(attrs);
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1300
 * @tc.name: testCreateRender008
 * @tc.desc: Verify the CreateRender function when pins is PIN_IN_USB_EXT and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender008, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_USB_EXT;
    InitAttrs(attrs);
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1400
 * @tc.name: testCreateRender009
 * @tc.desc: Verify the CreateRender function when pins is PIN_NONE and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender009, TestSize.Level2)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_NONE;
    InitAttrs(attrs);
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1500
 * @tc.name: testCreateRender010
 * @tc.desc: Verify the reliability of the CreateRender function when pins is PIN_OUT_SPEAKER and type is
 * AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender010, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    int32_t ret = HDF_SUCCESS;
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_SPEAKER;
    InitAttrs(attrs);
    attrs.silenceThreshold = 0;
    attrs.streamId = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
        if (ret != HDF_SUCCESS) {
            attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
            EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
        }
        EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1600
 * @tc.name: testCreateRender011
 * @tc.desc: Verify the reliability of the CreateRender function when pins is PIN_OUT_HEADSET and type is
 * AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateRender011, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    int32_t ret = HDF_SUCCESS;
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_HEADSET;
    InitAttrs(attrs);
    attrs.silenceThreshold = 0;
    attrs.streamId = 0;
    for (uint32_t i = 0; i < 1000; i++) {
        ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
        if (ret != HDF_SUCCESS) {
            attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
            EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
        }
        EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_DestroyRender_0200
 * @tc.name: testDestroyRender001
 * @tc.desc: Verify the DestroyRender function when the renderId_ is valid.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testDestroyRender001, TestSize.Level2)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_HEADSET;
    InitAttrs(attrs);
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_0700
 * @tc.name: testCreateCapture002
 * @tc.desc: Verify the CreateCapture function when the output parameter is invalid.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture002, TestSize.Level2)
{
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devicedesc = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, nullptr, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_0800
 * @tc.name: testCreateCapture003
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_OUT_HEADSET and type is
 * AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture003, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_HEADSET;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_0900
 * @tc.name: testCreateCapture004
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_OUT_LINEOUT and type is
 * AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture004, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_LINEOUT;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_1000
 * @tc.name: testCreateCapture005
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_OUT_HDMI and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture005, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_OUT_HDMI;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_1100
 * @tc.name: testCreateCapture006
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_IN_MIC and type is AUDIO_IN_RINGTONE.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture006, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_MIC;
    InitAttrs(attrs);
    attrs.type = AUDIO_IN_RINGTONE;
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_1200
 * @tc.name: testCreateCapture007
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_IN_HS_MIC and type is
 * AUDIO_IN_RINGTONE.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture007, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_HS_MIC;
    InitAttrs(attrs);
    attrs.type = AUDIO_IN_RINGTONE;
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_1300
 * @tc.name: testCreateCapture008
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_IN_LINEIN and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture008, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_LINEIN;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
#if defined ALSA_LIB_MODE
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
#else
        EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_1400
 * @tc.name: testCreateCapture009
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_IN_USB_EXT and type is
 * AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture009, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_USB_EXT;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
#else
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_1500
 * @tc.name: testCreateCapture010
 * @tc.desc: Verify the reliability of the CreateCapture function when pins is PIN_NONE and type is AUDIO_IN_MEDIA.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture010, TestSize.Level2)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_NONE;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_DestroyCapture_0200
 * @tc.name: testDestroyCapture001
 * @tc.desc: Verify the DestroyCapture function when the captureId_ is invalid.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testDestroyCapture001, TestSize.Level2)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, AUDIO_ADAPTER_CAPTURE_ID));
}

/**
 * @tc.number: SUB_Driver_Audio_SetVoiceVolume_0300
 * @tc.name  : testSetVoiceVolume001
 * @tc.desc  : Verify the reliability of the SetVoiceVolume function when
 * parameter volume as 0.5.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetVoiceVolume001, TestSize.Level1)
{
    float volume = 0.5;
    int32_t ret = adapter_->SetVoiceVolume(adapter_, volume);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT);
#else
    ASSERT_TRUE(ret == HDF_SUCCESS);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_SetVoiceVolume_0400
 * @tc.name  : testSetVoiceVolume002
 * @tc.desc  : Verify the reliability of the SetVoiceVolume function when
 * parameter volume as 1.5.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetVoiceVolume002, TestSize.Level2)
{
    float volume = 1.5;
    int32_t ret = adapter_->SetVoiceVolume(adapter_, volume);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_UpdateAudioRoute_0300
 * @tc.name  : testUpdateAudioRoute001
 * @tc.desc  : Verify the reliability of the UpdateAudioRoute function when
 * parameter as nullptr.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testUpdateAudioRoute001, TestSize.Level2)
{
    struct AudioRoute route = {};
    int32_t ret = adapter_->UpdateAudioRoute(adapter_, &route, nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetDeviceStatus_0500
 * @tc.name  : testGetDeviceStatus001
 * @tc.desc  : Verify the reliability of the GetDeviceStatus function .
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetDeviceStatus001, TestSize.Level1)
{
    struct AudioDeviceStatus status = {};
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_SUCCESS, adapter_->GetDeviceStatus(adapter_, &status));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_0500
 * @tc.name  : testGetPassthroughMode001
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * port as ports.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode001, TestSize.Level1)
{
    struct AudioPort port = {};
    port = adapterDescs_->ports[0];
    enum AudioPortPassthroughMode mode;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_FAILURE, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_0700
 * @tc.name  : testGetPortCapability001
 * @tc.desc  : Verify the reliability of the GetPortCapability function when
 * portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability001, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_DestroyCapture_0300
 * @tc.name  : testDestroyCapture002
 * @tc.desc  : Verify the reliability of the DestroyCapture function when
 * captureId_ as 1000.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testDestroyCapture002, TestSize.Level2)
{
    captureId_ = 1000;
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateCapture_0600
 * @tc.name  : testCreateCapture001
 * @tc.desc  : Verify the reliability of the CreateCapture function 100 Times.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testCreateCapture001, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char *>("primary");
    devicedesc.pins = PIN_IN_MIC;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
        EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
    }
}

/**
 * @tc.number: SUB_Driver_Audio_ReleaseAudioRoute_0300
 * @tc.name  : testReleaseAudioRoute001
 * @tc.desc  : Verify the reliability of the ReleaseAudioRoute function when
 * parameter routeHandle as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testReleaseAudioRoute001, TestSize.Level1)
{
    int32_t routeHandle = -1;
    AudioRouteNode source = {};
    AudioRouteNode sink = {};

    source.portId = static_cast<int32_t>(0);
    source.role = AUDIO_PORT_SOURCE_ROLE;
    source.type = AUDIO_PORT_MIX_TYPE;
    source.ext.mix.moduleId = static_cast<int32_t>(0);
    source.ext.mix.streamId = PRIMARY_OUTPUT_STREAM_ID;
    source.ext.device.desc = (char *)"";

    sink.ext.device.type = PIN_OUT_EARPIECE;
    sink.ext.device.desc = (char *)"pin_out_earpiece";
    sink.portId = static_cast<int32_t>(0);
    sink.role = AUDIO_PORT_SINK_ROLE;
    sink.type = AUDIO_PORT_DEVICE_TYPE;
    sink.ext.device.moduleId = static_cast<int32_t>(0);
    sink.ext.device.desc = (char *)"";

    AudioRoute route = {
        .sources = &source,
        .sourcesLen = 1,
        .sinks = &sink,
        .sinksLen = 1,
    };
    int32_t ret = adapter_->UpdateAudioRoute(adapter_, &route, &routeHandle);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#else
    ASSERT_EQ(HDF_SUCCESS, ret);
#endif
    ret = adapter_->ReleaseAudioRoute(adapter_, routeHandle);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT);
#else
    ASSERT_TRUE(ret == HDF_SUCCESS);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_ReleaseAudioRoute_0400
 * @tc.name  : testReleaseAudioRoute002
 * @tc.desc  : Verify the reliability of the ReleaseAudioRoute function when parameter routeHandle as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testReleaseAudioRoute002, TestSize.Level2)
{
    int32_t routeHandle = -1;
    int32_t ret = adapter_->ReleaseAudioRoute(adapter_, routeHandle);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_0800
 * @tc.name  : testGetPortCapability002
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability002, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_0900
 * @tc.name  : testGetPortCapability003
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability003, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1000
 * @tc.name  : testGetPortCapability004
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability004, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1100
 * @tc.name  : testGetPortCapability005
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability005, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1200
 * @tc.name  : testGetPortCapability006
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability006, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1300
 * @tc.name  : testGetPortCapability007
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability007, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1400
 * @tc.name  : testGetPortCapability008
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability008, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1500
 * @tc.name  : testGetPortCapability009
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability009, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1600
 * @tc.name  : testGetPortCapability010
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability010, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1700
 * @tc.name  : testGetPortCapability011
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability011, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1800
 * @tc.name  : testGetPortCapability012
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability012, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_1900
 * @tc.name  : testGetPortCapability013
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability013, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2000
 * @tc.name  : testGetPortCapability014
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability014, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2100
 * @tc.name  : testGetPortCapability015
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability015, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2200
 * @tc.name  : testGetPortCapability016
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability016, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2300
 * @tc.name  : testGetPortCapability017
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability017, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2400
 * @tc.name  : testGetPortCapability018
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability018, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2500
 * @tc.name  : testGetPortCapability019
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability019, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2600
 * @tc.name  : testGetPortCapability020
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability020, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2700
 * @tc.name  : testGetPortCapability021
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability021, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    EXPECT_EQ(HDF_SUCCESS, ret);
#else
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2800
 * @tc.name  : testGetPortCapability022
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability022, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_2900
 * @tc.name  : testGetPortCapability023
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability023, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("AOP");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_3000
 * @tc.name  : testGetPortCapability024
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability024, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_3100
 * @tc.name  : testGetPortCapability025
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability025, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPortCapability_3200
 * @tc.name  : testGetPortCapability026
 * @tc.desc  : Verify the reliability of the GetPortCapability function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPortCapability026, TestSize.Level2)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = nullptr;
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_0500
 * @tc.name  : testSetPassthroughMode001
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when
 * portId as 100 100Times.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode001, TestSize.Level2)
{
    int32_t ret = 0;
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    for (int i = 0; i < 100; i++) {
        ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
        ASSERT_NE(ret, HDF_SUCCESS);
    }
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_0600
 * @tc.name  : testSetPassthroughMode002
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode002, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_0700
 * @tc.name  : testSetPassthroughMode003
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode003, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_0800
 * @tc.name  : testSetPassthroughMode004
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode004, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_0900
 * @tc.name  : testSetPassthroughMode005
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode005, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1000
 * @tc.name  : testSetPassthroughMode006
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode006, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1100
 * @tc.name  : testSetPassthroughMode007
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode007, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1200
 * @tc.name  : testSetPassthroughMode008
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode008, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1300
 * @tc.name  : testSetPassthroughMode009
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode009, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1400
 * @tc.name  : testSetPassthroughMode010
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode010, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1500
 * @tc.name  : testSetPassthroughMode011
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode011, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1600
 * @tc.name  : testSetPassthroughMode012
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode012, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1800
 * @tc.name  : testSetPassthroughMode014
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode014, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = nullptr;
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1900
 * @tc.name  : testSetPassthroughMode015
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode015, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2000
 * @tc.name  : testSetPassthroughMode016
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode016, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2100
 * @tc.name  : testSetPassthroughMode017
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode017, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2200
 * @tc.name  : testSetPassthroughMode018
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode018, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2700
 * @tc.name  : testSetPassthroughMode023
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode023, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2800
 * @tc.name  : testSetPassthroughMode024
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode024, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2900
 * @tc.name  : testSetPassthroughMode025
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode025, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3000
 * @tc.name  : testSetPassthroughMode026
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode026, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3100
 * @tc.name  : testSetPassthroughMode027
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode027, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3200
 * @tc.name  : testSetPassthroughMode028
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode028, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3300
 * @tc.name  : testSetPassthroughMode029
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode029, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3400
 * @tc.name  : testSetPassthroughMode030
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode030, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3900
 * @tc.name  : testSetPassthroughMode035
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode035, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_4000
 * @tc.name  : testSetPassthroughMode036
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode036, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_4100
 * @tc.name  : testSetPassthroughMode037
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode037, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_4200
 * @tc.name  : testSetPassthroughMode038
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as -1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode038, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_0900
 * @tc.name  : testGetPassthroughMode005
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = -1,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode005, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1000
 * @tc.name  : testGetPassthroughMode006
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = -1,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode006, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1100
 * @tc.name  : testGetPassthroughMode007
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = -1,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode007, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1200
 * @tc.name  : testGetPassthroughMode008
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 1,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode008, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1300
 * @tc.name  : testGetPassthroughMode009
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 1,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode009, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1400
 * @tc.name  : testGetPassthroughMode010
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 1,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode010, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1800
 * @tc.name  : testGetPassthroughMode014
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = -1,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode014, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1900
 * @tc.name  : testGetPassthroughMode015
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = -1,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode015, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2000
 * @tc.name  : testGetPassthroughMode016
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = -1,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode016, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2100
 * @tc.name  : testGetPassthroughMode017
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 1,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode017, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2200
 * @tc.name  : testGetPassthroughMode018
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 1,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode018, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2300
 * @tc.name  : testGetPassthroughMode019
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 1,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode019, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2700
 * @tc.name  : testGetPassthroughMode023
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = -1,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode023, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2800
 * @tc.name  : testGetPassthroughMode024
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = -1,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode024, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2900
 * @tc.name  : testGetPassthroughMode025
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = -1,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode025, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3000
 * @tc.name  : testGetPassthroughMode026
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 1,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode026, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3100
 * @tc.name  : testGetPassthroughMode027
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 1,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode027, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3200
 * @tc.name  : testGetPassthroughMode028
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 1,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode028, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3600
 * @tc.name  : testGetPassthroughMode032
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = -1,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode032, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3700
 * @tc.name  : testGetPassthroughMode033
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = -1,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode033, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3800
 * @tc.name  : testGetPassthroughMode034
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = -1,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode034, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = -1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3900
 * @tc.name  : testGetPassthroughMode035
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 1,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode035, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_4000
 * @tc.name  : testGetPassthroughMode036
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 1,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode036, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_4100
 * @tc.name  : testGetPassthroughMode037
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 1,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode037, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetExtraParams_0400
 * @tc.name  : testSetExtraParams001
 * @tc.desc  : Verify the reliability of the SetExtraParams function when
 * parameter key as AUDIO_EXT_PARAM_KEY_VOLUME.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetExtraParams001, Function | MediumTest | Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_FOCUS;
    char condition[1024];
    const char *value = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    int32_t ret = adapter_->SetExtraParams(adapter_, key, condition, value);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT);
#else
    ASSERT_TRUE(ret == HDF_SUCCESS);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetExtraParams_0400
 * @tc.name  : testGetExtraParams001
 * @tc.desc  : Verify the reliability of the GetExtraParams function when
 * parameter key as AUDIO_EXT_PARAM_KEY_VOLUME.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetExtraParams001, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_VOLUME;
    char condition[1024];
    char value[1024] = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    uint32_t valueLen = 1024;
    int32_t ret = adapter_->GetExtraParams(adapter_, key, condition, value, valueLen);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT);
#else
    ASSERT_TRUE(ret == HDF_FAILURE);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_GetMicMute_0400
 * @tc.name  : testGetMicMute002
 * @tc.desc  : Verify the reliability of the GetMicMute function when parameter
 * mute as true.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetMicMute002, TestSize.Level1)
{
    bool mute = true;
    int32_t ret = 0;
    ret = adapter_->GetMicMute(adapter_, &mute);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT);
#else
    ASSERT_TRUE(ret == HDF_SUCCESS);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_SetMicMute_0300
 * @tc.name  : testSetMicMute001
 * @tc.desc  : Verify the reliability of the SetMicMute function when parameter
 * mute as true.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetMicMute001, TestSize.Level1)
{
    bool mute = true;
    int32_t ret = adapter_->SetMicMute(adapter_, mute);
#if defined DISPLAY_COMMUNITY || defined ALSA_LIB_MODE
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT);
#else
    ASSERT_TRUE(ret == HDF_SUCCESS);
#endif
}

/**
 * @tc.number: SUB_Driver_Audio_RegExtraParamObserver_0300
 * @tc.name  : testRegExtraParamObserver001
 * @tc.desc  : Verify the reliability of the RegExtraParamObserver function cookie as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testRegExtraParamObserver001, TestSize.Level2)
{
    ASSERT_NE(adapter_, nullptr);
    int8_t cookie = 1;
    struct IAudioCallback *audioCallback = nullptr;

    int32_t ret = adapter_->RegExtraParamObserver(adapter_, audioCallback, cookie);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_1700
 * @tc.name  : testSetPassthroughMode013
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode013, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2300
 * @tc.name  : testSetPassthroughMode019
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode019, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2400
 * @tc.name  : testSetPassthroughMode020
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode020, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2500
 * @tc.name  : testSetPassthroughMode021
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode021, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_2600
 * @tc.name  : testSetPassthroughMode022
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode022, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3500
 * @tc.name  : testSetPassthroughMode031
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode031, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3600
 * @tc.name  : testSetPassthroughMode032
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode032, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3700
 * @tc.name  : testSetPassthroughMode033
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode033, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_3800
 * @tc.name  : testSetPassthroughMode034
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when portId as 0.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode034, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_0600
 * @tc.name  : testGetPassthroughMode002
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 0,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode002, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_0700
 * @tc.name  : testGetPassthroughMode003
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 0,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode003, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_0800
 * @tc.name  : testGetPassthroughMode004
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 0,mode = PORT_PASSTHROUGH_LPCM
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode004, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1500
 * @tc.name  : testGetPassthroughMode011
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 0,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode011, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1600
 * @tc.name  : testGetPassthroughMode012
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 0,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode012, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_1700
 * @tc.name  : testGetPassthroughMode013
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 0,mode = PORT_PASSTHROUGH_RAW
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode013, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_RAW;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2400
 * @tc.name  : testGetPassthroughMode020
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 0,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode020, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2500
 * @tc.name  : testGetPassthroughMode021
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 0,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode021, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_2600
 * @tc.name  : testGetPassthroughMode022
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 0,mode = PORT_PASSTHROUGH_HBR2LBR
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode022, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_HBR2LBR;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3300
 * @tc.name  : testGetPassthroughMode029
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT,portId = 0,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode029, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3400
 * @tc.name  : testGetPassthroughMode030
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_IN,portId = 0,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode030, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}

/**
 * @tc.number: SUB_Driver_Audio_GetPassthroughMode_3500
 * @tc.name  : testGetPassthroughMode031
 * @tc.desc  : Verify the reliability of the GetPassthroughMode function when
 * when AudioPort.dir=PORT_OUT_IN,portId = 0,mode = PORT_PASSTHROUGH_AUTO
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode031, TestSize.Level2)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT_IN;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_AUTO;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_NE(ret, HDF_SUCCESS);
}
} // namespace
