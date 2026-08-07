/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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

#include <climits>
#include <cstring>
#include <gtest/gtest.h>
#include "hdf_dlist.h"
#include "osal_mem.h"
#include "v6_0/iaudio_adapter.h"
#include "v6_0/iaudio_manager.h"

using namespace std;
using namespace testing::ext;

#define AUDIO_CHANNELCOUNT             2
#define AUDIO_SAMPLE_RATE_48K          48000
#define DEEP_BUFFER_RENDER_PERIOD_SIZE 4096
#define INT_32_MAX                     0x7fffffff
#define PCM_16_BIT                     16
#define PCM_8_BIT                      8

namespace {
static const uint32_t g_audioAdapterNumMax = 5;
const int32_t AUDIO_ADAPTER_BUF_TEST = 1024;

class HdfAudioUtAdapterTest : public testing::Test {
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

void HdfAudioUtAdapterTest::AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
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

void HdfAudioUtAdapterTest::ReleaseAdapterDescs(struct AudioAdapterDescriptor **descs, uint32_t descsLen)
{
    if ((descsLen > 0) && (descs != nullptr) && ((*descs) != nullptr)) {
        for (uint32_t i = 0; i < descsLen; i++) {
            AudioAdapterDescriptorFree(&(*descs)[i], false);
        }
        OsalMemFree(*descs);
        *descs = nullptr;
    }
}

void HdfAudioUtAdapterTest::InitAttrs(struct AudioSampleAttributes &attrs)
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
}

void HdfAudioUtAdapterTest::InitDevDesc(struct AudioDeviceDescriptor &devDesc)
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

void HdfAudioUtAdapterTest::SetUp()
{
    uint32_t size = g_audioAdapterNumMax;
    manager_ = IAudioManagerGet(false);
    ASSERT_NE(manager_, nullptr);

    adapterDescs_ = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * (g_audioAdapterNumMax));
    ASSERT_NE(adapterDescs_, nullptr);

    ASSERT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size));
    if (size > g_audioAdapterNumMax) {
        ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
        ASSERT_LT(size, g_audioAdapterNumMax);
    }

    if (manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_) != HDF_SUCCESS) {
        ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
        ASSERT_TRUE(false);
    }
    if (adapter_ == nullptr) {
        ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
        ASSERT_TRUE(false);
    }
}

void HdfAudioUtAdapterTest::TearDown()
{
    ASSERT_NE(manager_, nullptr);
    ASSERT_NE(adapter_, nullptr);

    manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName);
    ReleaseAdapterDescs(&adapterDescs_, g_audioAdapterNumMax);
    adapter_ = nullptr;
    IAudioManagerRelease(manager_, false);
    manager_ = nullptr;
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0100, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->InitAllPorts(nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0200, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, adapter_->InitAllPorts(adapter_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0300, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(nullptr, nullptr, nullptr, nullptr, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0400, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(nullptr, &devicedesc, &attrs, &render, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0500, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateRender(adapter_, nullptr, &attrs, &render, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0600, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateRender(adapter_, &devicedesc, nullptr, &render, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0700, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char*>("primary");
    devicedesc.pins = PIN_OUT_SPEAKER;
    InitAttrs(attrs);
    attrs.silenceThreshold = 0;
    attrs.streamId = 0;
    int32_t ret = adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_);
    if (ret != HDF_SUCCESS) {
        attrs.format = AUDIO_FORMAT_TYPE_PCM_32_BIT;
        ASSERT_EQ(HDF_SUCCESS, adapter_->CreateRender(adapter_, &devicedesc, &attrs, &render, &renderId_));
    }
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyRender(adapter_, renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0800, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyRender(nullptr, renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_0900, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(nullptr, nullptr, nullptr, nullptr, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1000, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(nullptr, &devicedesc, &attrs, &capture, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1100, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioSampleAttributes attrs = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateCapture(adapter_, nullptr, &attrs, &capture, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1200, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateCapture(adapter_, &devicedesc, nullptr, &capture, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1300, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    InitDevDesc(devicedesc);
    devicedesc.desc = const_cast<char*>("primary");
    devicedesc.pins = PIN_IN_MIC;
    InitAttrs(attrs);
    attrs.silenceThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    EXPECT_EQ(HDF_SUCCESS, adapter_->CreateCapture(adapter_, &devicedesc, &attrs, &capture, &captureId_));
    EXPECT_EQ(HDF_SUCCESS, adapter_->DestroyCapture(adapter_, captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1400, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyCapture(nullptr, captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1500, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPortCapability(nullptr, nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1600, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPortCapability(nullptr, &port, &capability));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1700, TestSize.Level1)
{
    struct AudioPortCapability capability = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetPortCapability(adapter_, nullptr, &capability));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1800, TestSize.Level1)
{
    struct AudioPort port = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetPortCapability(adapter_, &port, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_1900, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char*>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2000, TestSize.Level1)
{
    struct AudioPort port = adapterDescs_[0].ports[0];
    struct AudioPortCapability capability = {};
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    if (ret == HDF_ERR_NOT_SUPPORT) {
        GTEST_SKIP()<< "skip this test" << std::endl;
    }
    ASSERT_TRUE(ret == HDF_SUCCESS);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2100, TestSize.Level1)
{
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetPassthroughMode(nullptr, nullptr, mode));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2200, TestSize.Level1)
{
    struct AudioPort port = {};
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetPassthroughMode(nullptr, &port, mode));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2300, TestSize.Level1)
{
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->SetPassthroughMode(adapter_, nullptr, mode));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2400, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char*>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2500, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPassthroughMode(nullptr, nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2600, TestSize.Level1)
{
    struct AudioPort port = {};
    enum AudioPortPassthroughMode mode;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPassthroughMode(nullptr, &port, &mode));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2700, TestSize.Level1)
{
    enum AudioPortPassthroughMode mode;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetPassthroughMode(adapter_, nullptr, &mode));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2800, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char*>("primary");
    enum AudioPortPassthroughMode mode;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_2900, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetDeviceStatus(nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3000, TestSize.Level1)
{
    struct AudioDeviceStatus status = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->GetDeviceStatus(nullptr, &status));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3100, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetDeviceStatus(adapter_, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3200, TestSize.Level1)
{
    struct AudioDeviceStatus status = {};
    EXPECT_EQ(HDF_SUCCESS, adapter_->GetDeviceStatus(adapter_, &status));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3300, TestSize.Level1)
{
    bool mute = false;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetMicMute(nullptr, mute));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3400, TestSize.Level1)
{
    bool mute = false;
    int32_t ret = adapter_->SetMicMute(adapter_, mute);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3500, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetMicMute(nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3600, TestSize.Level1)
{
    bool mute = false;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetMicMute(nullptr, &mute));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3700, TestSize.Level1)
{
    bool mute = false;
    int32_t ret = adapter_->GetMicMute(adapter_, &mute);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3800, TestSize.Level1)
{
    float volume = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetVoiceVolume(nullptr, volume));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_3900, TestSize.Level1)
{
    float volume = 0;
    int32_t ret = adapter_->SetVoiceVolume(adapter_, volume);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4000, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetExtraParams(nullptr, key, nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4100, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    char value[AUDIO_ADAPTER_BUF_TEST];
    EXPECT_NE(HDF_SUCCESS, adapter_->SetExtraParams(nullptr, key, condition, value));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4200, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_LOWPOWER;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    const char *value = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    int32_t ret = adapter_->SetExtraParams(adapter_, key, condition, value);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4300, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    uint32_t valueLen = AUDIO_ADAPTER_BUF_TEST;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetExtraParams(nullptr, key, nullptr, nullptr, valueLen));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4400, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    char value[AUDIO_ADAPTER_BUF_TEST];
    uint32_t valueLen = AUDIO_ADAPTER_BUF_TEST;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetExtraParams(nullptr, key, condition, value, valueLen));
}
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4500, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    char value[AUDIO_ADAPTER_BUF_TEST] = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    uint32_t valueLen = AUDIO_ADAPTER_BUF_TEST;

    int32_t ret = adapter_->GetExtraParams(adapter_, key, condition, value, valueLen);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4600, TestSize.Level1)
{
    struct AudioRoute route = {};
    int32_t routeHandle = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->UpdateAudioRoute(nullptr, &route, &routeHandle));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4700, TestSize.Level1)
{
    struct AudioRoute route = {};
    int32_t routeHandle = 0;
    int32_t ret = adapter_->UpdateAudioRoute(adapter_, &route, &routeHandle);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4800, TestSize.Level1)
{
    int32_t routeHandle = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->ReleaseAudioRoute(nullptr, routeHandle));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_4900, TestSize.Level1)
{
    int32_t routeHandle = 0;
    int32_t ret = adapter_->ReleaseAudioRoute(adapter_, routeHandle);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_5000, TestSize.Level1)
{
    uint32_t majorVer = 0;
    uint32_t minorVer = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetVersion(nullptr, &majorVer, &minorVer));
}

HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_5100, TestSize.Level1)
{
    uint32_t majorVer = 0;
    uint32_t minorVer = 0;
    int32_t ret = adapter_->GetVersion(adapter_, &majorVer, &minorVer);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

#ifdef AUDIO_COMMUNITY
/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0100, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0100: start.");
    EXPECT_TRUE(PORT_OUT == 1);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0100: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0200, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0200: start.");
    EXPECT_TRUE(PORT_IN == 2);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0200: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0300, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0300: start.");
    EXPECT_TRUE(PORT_OUT_IN == 3);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0300: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0400, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0400: start.");
    EXPECT_TRUE(PIN_NONE == 0);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest_0400: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest006: start.");
    EXPECT_TRUE(PIN_OUT_SPEAKER == 1 << 0);
    printf("HdfAudioHdiEnumTest006: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest007: start.");
    EXPECT_TRUE(PIN_OUT_HEADSET == 1 << 1);
    printf("HdfAudioHdiEnumTest007: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest008: start.");
    EXPECT_TRUE(PIN_OUT_LINEOUT == 1 << 2);
    printf("HdfAudioHdiEnumTest008: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest009: start.");
    EXPECT_TRUE(PIN_OUT_HDMI == 1 << 3);
    printf("HdfAudioHdiEnumTest009: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_0900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_0900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest010: start.");
    EXPECT_TRUE(PIN_OUT_USB == 1 << 4);
    printf("HdfAudioHdiEnumTest010: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest011: start.");
    EXPECT_TRUE(PIN_OUT_USB_EXT == 1 << 5);
    printf("HdfAudioHdiEnumTest011: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest012: start.");
    EXPECT_TRUE(PIN_OUT_EARPIECE == (1 << 5 | 1 << 4));
    printf("HdfAudioHdiEnumTest012: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest013: start.");
    EXPECT_TRUE(PIN_OUT_BLUETOOTH_SCO == 1 << 6);
    printf("HdfAudioHdiEnumTest013: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest014: start.");
    EXPECT_TRUE(PIN_OUT_DAUDIO_DEFAULT == 1 << 7);
    printf("HdfAudioHdiEnumTest014: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest015: start.");
    EXPECT_TRUE(PIN_OUT_HEADPHONE == 1 << 8);
    printf("HdfAudioHdiEnumTest015: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest016: start.");
    EXPECT_TRUE(PIN_OUT_USB_HEADSET == 1 << 9);
    printf("HdfAudioHdiEnumTest016: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest017: start.");
    EXPECT_TRUE(PIN_IN_MIC == (1 << 27 | 1 << 0));
    printf("HdfAudioHdiEnumTest017: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest018: start.");
    EXPECT_TRUE(PIN_IN_HS_MIC == (1 << 27 | 1 << 1));
    printf("HdfAudioHdiEnumTest018: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest019: start.");
    EXPECT_TRUE(PIN_IN_LINEIN == (1 << 27 | 1 << 2));
    printf("HdfAudioHdiEnumTest019: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_1900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_1900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest020: start.");
    EXPECT_TRUE(PIN_IN_USB_EXT == (1 << 27 | 1 << 3));
    printf("HdfAudioHdiEnumTest020: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest021: start.");
    EXPECT_TRUE(PIN_IN_BLUETOOTH_SCO_HEADSET == (1 << 27 | 1 << 4));
    printf("HdfAudioHdiEnumTest021: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest022: start.");
    EXPECT_TRUE(PIN_IN_DAUDIO_DEFAULT == (1 << 27 | 1 << 5));
    printf("HdfAudioHdiEnumTest022: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest023: start.");
    EXPECT_TRUE(PIN_IN_USB_HEADSET == (1 << 27 | 1 << 6));
    printf("HdfAudioHdiEnumTest023: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest024: start.");
    EXPECT_TRUE(AUDIO_IN_MEDIA == 0);
    printf("HdfAudioHdiEnumTest024: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest025: start.");
    EXPECT_TRUE(AUDIO_IN_COMMUNICATION == 1);
    printf("HdfAudioHdiEnumTest025: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest026: start.");
    EXPECT_TRUE(AUDIO_IN_RINGTONE == 2);
    printf("HdfAudioHdiEnumTest026: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest027: start.");
    EXPECT_TRUE(AUDIO_IN_CALL == 3);
    printf("HdfAudioHdiEnumTest027: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest028: start.");
    EXPECT_TRUE(AUDIO_MMAP_NOIRQ == 4);
    printf("HdfAudioHdiEnumTest028: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest029: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_8_BIT == 1 << 0);
    printf("HdfAudioHdiEnumTest029: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_2900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_2900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest030: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_16_BIT == 1 << 1);
    printf("HdfAudioHdiEnumTest030: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest031: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_24_BIT == (1 << 1 | 1 << 0));
    printf("HdfAudioHdiEnumTest031: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest032: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_32_BIT == 1 << 2);
    printf("HdfAudioHdiEnumTest032: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest033: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_MAIN == (1 << 24 | 1 << 0));
    printf("HdfAudioHdiEnumTest033: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest034: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_LC == (1 << 24 | 1 << 1));
    printf("HdfAudioHdiEnumTest034: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest035: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_LD == (1 << 24 | 1 << 1 | 1 << 0));
    printf("HdfAudioHdiEnumTest035: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest036: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_ELD == (1 << 24 | 1 << 2));
    printf("HdfAudioHdiEnumTest036: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest037: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_HE_V1 == (1 << 24 | 1 << 2 | 1 << 0));
    printf("HdfAudioHdiEnumTest037: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest038: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_HE_V2 == (1 << 24 | 1 << 2 | 1 << 1));
    printf("HdfAudioHdiEnumTest038: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest039: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_G711A == (1 << 25 | 1 << 0));
    printf("HdfAudioHdiEnumTest039: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_3900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_3900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest040: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_G711U == (1 << 25 | 1 << 1));
    printf("HdfAudioHdiEnumTest040: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest041: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_G726 == (1 << 25 | 1 << 1 | 1 << 0));
    printf("HdfAudioHdiEnumTest041: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest042: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_FRONT_LEFT == 1);
    printf("HdfAudioHdiEnumTest042: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest043: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_FRONT_RIGHT == 2);
    printf("HdfAudioHdiEnumTest043: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest044: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_MONO == 1);
    printf("HdfAudioHdiEnumTest044: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest045: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_STEREO == 3);
    printf("HdfAudioHdiEnumTest045: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest046: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_8000 == 1 << 0);
    printf("HdfAudioHdiEnumTest046: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest047: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_12000 == 1 << 1);
    printf("HdfAudioHdiEnumTest047: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest048: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_11025 == 1 << 2);
    printf("HdfAudioHdiEnumTest048: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest049: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_16000 == 1 << 3);
    printf("HdfAudioHdiEnumTest049: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_4900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_4900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest050: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_22050 == 1 << 4);
    printf("HdfAudioHdiEnumTest050: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest051: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_24000 == 1 << 5);
    printf("HdfAudioHdiEnumTest051: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest052: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_32000 == 1 << 6);
    printf("HdfAudioHdiEnumTest052: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest053: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_44100 == 1 << 7);
    printf("HdfAudioHdiEnumTest053: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest054: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_48000 == 1 << 8);
    printf("HdfAudioHdiEnumTest054: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest055: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_64000 == 1 << 9);
    printf("HdfAudioHdiEnumTest055: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest056: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_96000 == 1 << 10);
    printf("HdfAudioHdiEnumTest056: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest057: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_INVALID == 4294967295);
    printf("HdfAudioHdiEnumTest057: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest058: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_LPCM == 1 << 0);
    printf("HdfAudioHdiEnumTest058: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest059: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_RAW == 1 << 1);
    printf("HdfAudioHdiEnumTest059: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_5900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_5900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest060: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_HBR2LBR == 1 << 2);
    printf("HdfAudioHdiEnumTest060: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest061: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_AUTO == 1 << 3);
    printf("HdfAudioHdiEnumTest061: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6100
* @tc.desc: Judgment AudioSampleFormat enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest062: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S8 == 0);
    printf("HdfAudioHdiEnumTest062: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest063: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S8P == 1);
    printf("HdfAudioHdiEnumTest063: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest064: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U8 == 2);
    printf("HdfAudioHdiEnumTest064: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest065: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U8P == 3);
    printf("HdfAudioHdiEnumTest065: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest066: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S16 == 4);
    printf("HdfAudioHdiEnumTest066: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest067: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S16P == 5);
    printf("HdfAudioHdiEnumTest067: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest068: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U16 == 6);
    printf("HdfAudioHdiEnumTest068: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest069: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U16P == 7);
    printf("HdfAudioHdiEnumTest069: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_6900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_6900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest070: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S24 == 8);
    printf("HdfAudioHdiEnumTest070: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest071: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S24P == 9);
    printf("HdfAudioHdiEnumTest071: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest072: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U24 == 10);
    printf("HdfAudioHdiEnumTest072: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest073: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U24P == 11);
    printf("HdfAudioHdiEnumTest073: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest074: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S32 == 12);
    printf("HdfAudioHdiEnumTest074: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest075: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S32P == 13);
    printf("HdfAudioHdiEnumTest075: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest076: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U32 == 14);
    printf("HdfAudioHdiEnumTest076: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest077: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U32P == 15);
    printf("HdfAudioHdiEnumTest077: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest078: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S64 == 16);
    printf("HdfAudioHdiEnumTest078: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest079: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S64P == 17);
    printf("HdfAudioHdiEnumTest079: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_7900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_7900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest080: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U64 == 18);
    printf("HdfAudioHdiEnumTest080: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest081: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U64P == 19);
    printf("HdfAudioHdiEnumTest081: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest082: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F32 == 20);
    printf("HdfAudioHdiEnumTest082: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest083: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F32P == 21);
    printf("HdfAudioHdiEnumTest083: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest084: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F64 == 22);
    printf("HdfAudioHdiEnumTest084: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest085: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F64P == 23);
    printf("HdfAudioHdiEnumTest085: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8500
* @tc.desc: Judgment AudioChannelMode enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest086: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_NORMAL == 0);
    printf("HdfAudioHdiEnumTest086: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest087: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_BOTH_LEFT == 1);
    printf("HdfAudioHdiEnumTest087: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest088: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_BOTH_RIGHT == 2);
    printf("HdfAudioHdiEnumTest088: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest089: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_EXCHANGE == 3);
    printf("HdfAudioHdiEnumTest089: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_8900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_8900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest090: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_MIX == 4);
    printf("HdfAudioHdiEnumTest090: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest091: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_LEFT_MUTE == 5);
    printf("HdfAudioHdiEnumTest091: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest092: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_RIGHT_MUTE == 6);
    printf("HdfAudioHdiEnumTest092: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest093: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_BOTH_MUTE == 7);
    printf("HdfAudioHdiEnumTest093: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9300
* @tc.desc: Judgment AudioDrainNotifyType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest094: start.");
    EXPECT_TRUE(AUDIO_DRAIN_NORMAL_MODE == 0);
    printf("HdfAudioHdiEnumTest094: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest095: start.");
    EXPECT_TRUE(AUDIO_DRAIN_EARLY_MODE == 1);
    printf("HdfAudioHdiEnumTest095: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9500
* @tc.desc: Judgment AudioCallbackType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest096: start.");
    EXPECT_TRUE(AUDIO_NONBLOCK_WRITE_COMPLETED == 0);
    printf("HdfAudioHdiEnumTest096: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest097: start.");
    EXPECT_TRUE(AUDIO_DRAIN_COMPLETED == 1);
    printf("HdfAudioHdiEnumTest097: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest098: start.");
    EXPECT_TRUE(AUDIO_FLUSH_COMPLETED == 2);
    printf("HdfAudioHdiEnumTest098: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest099: start.");
    EXPECT_TRUE(AUDIO_RENDER_FULL == 3);
    printf("HdfAudioHdiEnumTest099: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest_9900
* @tc.desc: Judgment AudioCallbackType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest_9900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest100: start.");
    EXPECT_TRUE(AUDIO_ERROR_OCCUR == 4);
    printf("HdfAudioHdiEnumTest100: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0100
* @tc.desc: Judgment AudioPortRole enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest101: start.");
    EXPECT_TRUE(AUDIO_PORT_UNASSIGNED_ROLE == 0);
    printf("HdfAudioHdiEnumTest101: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest102: start.");
    EXPECT_TRUE(AUDIO_PORT_SOURCE_ROLE == 1);
    printf("HdfAudioHdiEnumTest102: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest103: start.");
    EXPECT_TRUE(AUDIO_PORT_SINK_ROLE == 2);
    printf("HdfAudioHdiEnumTest103: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0400
* @tc.desc: Judgment AudioPortType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest104: start.");
    EXPECT_TRUE(AUDIO_PORT_UNASSIGNED_TYPE == 0);
    printf("HdfAudioHdiEnumTest104: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest105: start.");
    EXPECT_TRUE(AUDIO_PORT_DEVICE_TYPE == 1);
    printf("HdfAudioHdiEnumTest105: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest106: start.");
    EXPECT_TRUE(AUDIO_PORT_MIX_TYPE == 2);
    printf("HdfAudioHdiEnumTest106: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest107: start.");
    EXPECT_TRUE(AUDIO_PORT_SESSION_TYPE == 3);
    printf("HdfAudioHdiEnumTest107: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0800
* @tc.desc: Judgment AudioSessionType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest108: start.");
    EXPECT_TRUE(AUDIO_OUTPUT_STAGE_SESSION == 0);
    printf("HdfAudioHdiEnumTest108: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_0900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_0900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest109: start.");
    EXPECT_TRUE(AUDIO_OUTPUT_MIX_SESSION == 1);
    printf("HdfAudioHdiEnumTest109: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest110: start.");
    EXPECT_TRUE(AUDIO_ALLOCATE_SESSION == 2);
    printf("HdfAudioHdiEnumTest110: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest111: start.");
    EXPECT_TRUE(AUDIO_INVALID_SESSION == 3);
    printf("HdfAudioHdiEnumTest111: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1200
* @tc.desc: Judgment AudioDeviceType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest112: start.");
    EXPECT_TRUE(AUDIO_LINEOUT == 1 << 0);
    printf("HdfAudioHdiEnumTest112: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest113: start.");
    EXPECT_TRUE(AUDIO_HEADPHONE == 1 << 1);
    printf("HdfAudioHdiEnumTest113: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest114: start.");
    EXPECT_TRUE(AUDIO_HEADSET == 1 << 2);
    printf("HdfAudioHdiEnumTest114: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest115: start.");
    EXPECT_TRUE(AUDIO_USB_HEADSET == 1 << 3);
    printf("HdfAudioHdiEnumTest115: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest116: start.");
    EXPECT_TRUE(AUDIO_USB_HEADPHONE == 1 << 4);
    printf("HdfAudioHdiEnumTest116: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest117: start.");
    EXPECT_TRUE(AUDIO_USBA_HEADSET == 1 << 5);
    printf("HdfAudioHdiEnumTest117: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest118: start.");
    EXPECT_TRUE(AUDIO_USBA_HEADPHONE == 1 << 6);
    printf("HdfAudioHdiEnumTest118: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_1900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_1900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest119: start.");
    EXPECT_TRUE(AUDIO_PRIMARY_DEVICE == 1 << 7);
    printf("HdfAudioHdiEnumTest119: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest120: start.");
    EXPECT_TRUE(AUDIO_USB_DEVICE == 1 << 8);
    printf("HdfAudioHdiEnumTest120: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest121: start.");
    EXPECT_TRUE(AUDIO_A2DP_DEVICE == 1 << 9);
    printf("HdfAudioHdiEnumTest121: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest122: start.");
    EXPECT_TRUE(AUDIO_HDMI_DEVICE == 1 << 10);
    printf("HdfAudioHdiEnumTest122: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest123: start.");
    EXPECT_TRUE(AUDIO_ADAPTER_DEVICE == 1 << 11);
    printf("HdfAudioHdiEnumTest123: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest124: start.");
    EXPECT_TRUE(AUDIO_DP_DEVICE == 1 << 12);
    printf("HdfAudioHdiEnumTest124: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2500
* @tc.desc: Judgment AudioEventType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest125: start.");
    EXPECT_TRUE(AUDIO_DEVICE_ADD == 1);
    printf("HdfAudioHdiEnumTest125: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest126: start.");
    EXPECT_TRUE(AUDIO_DEVICE_REMOVE == 2);
    printf("HdfAudioHdiEnumTest126: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest127: start.");
    EXPECT_TRUE(AUDIO_LOAD_SUCCESS == 3);
    printf("HdfAudioHdiEnumTest127: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest128: start.");
    EXPECT_TRUE(AUDIO_LOAD_FAILURE == 4);
    printf("HdfAudioHdiEnumTest128: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_2900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_2900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest129: start.");
    EXPECT_TRUE(AUDIO_UNLOAD == 5);
    printf("HdfAudioHdiEnumTest129: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3000, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest130: start.");
    EXPECT_TRUE(AUDIO_SERVICE_VALID == 7);
    printf("HdfAudioHdiEnumTest130: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest131: start.");
    EXPECT_TRUE(AUDIO_SERVICE_INVALID == 8);
    printf("HdfAudioHdiEnumTest131: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3200
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3200, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest132: start.");
    EXPECT_TRUE(AUDIO_CAPTURE_THRESHOLD == 9);
    printf("HdfAudioHdiEnumTest132: end.");
}
/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3300, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest133: start.");
    EXPECT_TRUE(AUDIO_EVENT_UNKNOWN == 10);
    printf("HdfAudioHdiEnumTest133: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3400
* @tc.desc: Judgment AudioExtParamKey enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3400, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest134: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_NONE == 0);
    printf("HdfAudioHdiEnumTest134: end.");
}
/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3500, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest135: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_VOLUME == 1);
    printf("HdfAudioHdiEnumTest135: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3600, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest136: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_FOCUS == 2);
    printf("HdfAudioHdiEnumTest136: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3700
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3700, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest137: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_BUTTON == 3);
    printf("HdfAudioHdiEnumTest137: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3800
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3800, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest138: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_EFFECT == 4);
    printf("HdfAudioHdiEnumTest138: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_3900
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_3900, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest139: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_STATUS == 5);
    printf("HdfAudioHdiEnumTest139: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4000
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4000, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4000: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_USB_DEVICE == 101);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4000: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4100
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4100, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4100: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_LOWPOWER == 1000);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4100: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4200
* @tc.desc: Judgment AudioInputType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4200, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4200: start.");
    EXPECT_TRUE(AUDIO_INPUT_DEFAULT_TYPE == 0);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4200: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4300
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4300, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4300: start.");
    EXPECT_TRUE(AUDIO_INPUT_MIC_TYPE == 1 << 0);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4300: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4400
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4400, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4400: start.");
    EXPECT_TRUE(AUDIO_INPUT_SPEECH_WAKEUP_TYPE == 1 << 1);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4400: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4500
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4500, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4500: start.");
    EXPECT_TRUE(AUDIO_INPUT_VOICE_COMMUNICATION_TYPE == 1 << 2);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4500: end.");
}

/**
* @tc.name: SUB_Driver_Audio_AdapterHdi_EnumTest1_4600
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, SUB_Driver_Audio_AdapterHdi_EnumTest1_4600, TestSize.Level3)
{
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4600: start.");
    EXPECT_TRUE(AUDIO_INPUT_VOICE_RECOGNITION_TYPE == 1 << 3);
    printf("SUB_Driver_Audio_AdapterHdi_EnumTest1_4600: end.");
}
#endif
}

