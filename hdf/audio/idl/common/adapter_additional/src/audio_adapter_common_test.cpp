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

#include "hdf_dlist.h"
#include "osal_mem.h"
#include "v1_0/iaudio_adapter.h"
#include "v1_0/iaudio_manager.h"
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

namespace {
static const uint32_t AUDIO_ADAPTER_NUM_MAX = 5;
static const uint32_t AUDIO_ADAPTER_RENDER_ID = 0;
static const uint32_t AUDIO_ADAPTER_CAPTURE_ID = 0;

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
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
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
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
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
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
}

/**
 * @tc.number: SUB_Driver_Audio_CreateRender_1600
 * @tc.name: testCreateRender011
 * @tc.desc: Verify the reliability of the CreateRender function when pins is PIN_OUT_SPEAKER and type is
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
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
}

/**
 * @tc.number: SUB_Driver_Audio_DestroyRender_0200
 * @tc.name: testDestroyRender001
 * @tc.desc: Verify the DestroyRender function when the renderId_ is invalid.
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
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, AUDIO_ADAPTER_RENDER_ID));
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyRender(adapter_, AUDIO_ADAPTER_RENDER_ID));
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
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
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
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
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
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
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
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
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
 * @tc.number: SUB_Driver_Audio_SetExtraParams_0400
 * @tc.name  : testSetExtraParams001
 * @tc.desc  : Verify the reliability of the SetExtraParams function when
 * parameter key as AUDIO_EXT_PARAM_KEY_VOLUME.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetExtraParams001, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_FOCUS;
    char condition[1024];
    const char *value = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    int32_t ret = adapter_->SetExtraParams(adapter_, key, condition, value);
    EXPECT_EQ(HDF_SUCCESS, ret);
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
    EXPECT_EQ(HDF_SUCCESS, ret);
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
    EXPECT_EQ(HDF_SUCCESS, ret);
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
    EXPECT_EQ(HDF_SUCCESS, ret);
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
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_ReleaseAudioRoute_0300
 * @tc.name  : testReleaseAudioRoute001
 * @tc.desc  : Verify the reliability of the ReleaseAudioRoute function when
 * parameter routeHandle as 1.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testReleaseAudioRoute001, TestSize.Level1)
{
    int32_t routeHandle = 1;
    int32_t ret = adapter_->ReleaseAudioRoute(adapter_, routeHandle);
    EXPECT_EQ(HDF_SUCCESS, ret);
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
 * portId as 100.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testGetPassthroughMode001, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 100;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Audio_SetPassthroughMode_0500
 * @tc.name  : testSetPassthroughMode001
 * @tc.desc  : Verify the reliability of the SetPassthroughMode function when
 * portId as 100 100Times.
 */
HWTEST_F(HdfAudioUtAdapterTestAdditional, testSetPassthroughMode001, TestSize.Level1)
{
    int32_t ret = 0;
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char *>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    for (int i = 0; i < 100; i++) {
        ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
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
    port.portId = 1;
    port.portName = const_cast<char *>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    EXPECT_EQ(HDF_SUCCESS, ret);
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
        EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(nullptr, &devicedesc, &attrs, &capture, &captureId_));
        EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(nullptr, captureId_));
    }
}
} // namespace
