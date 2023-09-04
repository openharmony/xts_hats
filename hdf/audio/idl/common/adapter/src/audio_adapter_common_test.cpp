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

#include <climits>
#include <cstring>
#include <gtest/gtest.h>
#include "hdf_dlist.h"
#include "osal_mem.h"
#include "v1_0/iaudio_adapter.h"
#include "v1_0/iaudio_manager.h"

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

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterInitAllPortsNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->InitAllPorts(nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterInitAllPortsParaIsvalid001, TestSize.Level1)
{
    EXPECT_EQ(HDF_SUCCESS, adapter_->InitAllPorts(adapter_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateRenderNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(nullptr, nullptr, nullptr, nullptr, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateRenderNull002, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateRender(nullptr, &devicedesc, &attrs, &render, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateRenderInvalid001, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateRender(adapter_, nullptr, &attrs, &render, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateRenderInvalid002, TestSize.Level1)
{
    struct IAudioRender *render = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateRender(adapter_, &devicedesc, nullptr, &render, &renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateRenderIsvalid001, TestSize.Level1)
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

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterDestroyRenderNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyRender(nullptr, renderId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateCaptureNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(nullptr, nullptr, nullptr, nullptr, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateCaptureNull002, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    struct AudioSampleAttributes attrs = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->CreateCapture(nullptr, &devicedesc, &attrs, &capture, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateCaptureInvalid001, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioSampleAttributes attrs = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateCapture(adapter_, nullptr, &attrs, &capture, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateCaptureInvalid002, TestSize.Level1)
{
    struct IAudioCapture *capture = nullptr;
    struct AudioDeviceDescriptor devicedesc = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->CreateCapture(adapter_, &devicedesc, nullptr, &capture, &captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterCreateCaptureIsvalid001, TestSize.Level1)
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

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterDestroyCaptureNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->DestroyCapture(nullptr, captureId_));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPortCapabilityNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPortCapability(nullptr, nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPortCapabilityNull002, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPortCapability(nullptr, &port, &capability));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPortCapabilityInvalid001, TestSize.Level1)
{
    struct AudioPortCapability capability = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetPortCapability(adapter_, nullptr, &capability));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPortCapabilityInvalid002, TestSize.Level1)
{
    struct AudioPort port = {};
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetPortCapability(adapter_, &port, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPortCapabilityInvalid003, TestSize.Level1)
{
    struct AudioPort port = {};
    struct AudioPortCapability capability = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char*>("primary");
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPortCapabilityIsvalid001, TestSize.Level1)
{
    struct AudioPort port = adapterDescs_[0].ports[0];
    struct AudioPortCapability capability = {};
    int32_t ret = adapter_->GetPortCapability(adapter_, &port, &capability);
    ASSERT_TRUE(ret == HDF_SUCCESS);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetPassthroughModeNull001, TestSize.Level1)
{
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetPassthroughMode(nullptr, nullptr, mode));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetPassthroughModeNull002, TestSize.Level1)
{
    struct AudioPort port = {};
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetPassthroughMode(nullptr, &port, mode));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetPassthroughModeInvalid001, TestSize.Level1)
{
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->SetPassthroughMode(adapter_, nullptr, mode));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetPassthroughModeIsvalid001, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char*>("primary");
    enum AudioPortPassthroughMode mode = PORT_PASSTHROUGH_LPCM;
    int32_t ret = adapter_->SetPassthroughMode(adapter_, &port, mode);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPassthroughModeNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPassthroughMode(nullptr, nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPassthroughModeNull002, TestSize.Level1)
{
    struct AudioPort port = {};
    enum AudioPortPassthroughMode mode;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetPassthroughMode(nullptr, &port, &mode));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPassthroughModeInvalid001, TestSize.Level1)
{
    enum AudioPortPassthroughMode mode;
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetPassthroughMode(adapter_, nullptr, &mode));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetPassthroughModeIsvalid001, TestSize.Level1)
{
    struct AudioPort port = {};
    port.dir = PORT_OUT;
    port.portId = 0;
    port.portName = const_cast<char*>("primary");
    enum AudioPortPassthroughMode mode;
    int32_t ret = adapter_->GetPassthroughMode(adapter_, &port, &mode);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetDeviceStatusNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetDeviceStatus(nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetDeviceStatusNull002, TestSize.Level1)
{
    struct AudioDeviceStatus status = {};
    EXPECT_NE(HDF_SUCCESS, adapter_->GetDeviceStatus(nullptr, &status));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetDeviceStatusInvalid001, TestSize.Level1)
{
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, adapter_->GetDeviceStatus(adapter_, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetDeviceStatusIsvalid001, TestSize.Level1)
{
    struct AudioDeviceStatus status = {};
    EXPECT_EQ(HDF_SUCCESS, adapter_->GetDeviceStatus(adapter_, &status));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetMicMuteNull001, TestSize.Level1)
{
    bool mute = false;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetMicMute(nullptr, mute));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetMicMuteIsvalid001, TestSize.Level1)
{
    bool mute = false;
    int32_t ret = adapter_->SetMicMute(adapter_, mute);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetMicMuteNull001, TestSize.Level1)
{
    EXPECT_NE(HDF_SUCCESS, adapter_->GetMicMute(nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetMicMuteNull002, TestSize.Level1)
{
    bool mute = false;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetMicMute(nullptr, &mute));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetMicMuteIsvalid001, TestSize.Level1)
{
    bool mute = false;
    int32_t ret = adapter_->GetMicMute(adapter_, &mute);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetVoiceVolumeNull001, TestSize.Level1)
{
    float volume = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetVoiceVolume(nullptr, volume));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetVoiceVolumeIsvalid001, TestSize.Level1)
{
    float volume = 0;
    int32_t ret = adapter_->SetVoiceVolume(adapter_, volume);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetExtraParamsNull001, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    EXPECT_NE(HDF_SUCCESS, adapter_->SetExtraParams(nullptr, key, nullptr, nullptr));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetExtraParamsNull002, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    char value[AUDIO_ADAPTER_BUF_TEST];
    EXPECT_NE(HDF_SUCCESS, adapter_->SetExtraParams(nullptr, key, condition, value));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterSetExtraParamsIsvalid001, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_LOWPOWER;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    const char *value = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    int32_t ret = adapter_->SetExtraParams(adapter_, key, condition, value);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetExtraParamsNull001, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    uint32_t valueLen = AUDIO_ADAPTER_BUF_TEST;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetExtraParams(nullptr, key, nullptr, nullptr, valueLen));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetExtraParamsNull002, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    char value[AUDIO_ADAPTER_BUF_TEST];
    uint32_t valueLen = AUDIO_ADAPTER_BUF_TEST;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetExtraParams(nullptr, key, condition, value, valueLen));
}
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetExtraParamsIsvalid001, TestSize.Level1)
{
    enum AudioExtParamKey key = AUDIO_EXT_PARAM_KEY_NONE;
    char condition[AUDIO_ADAPTER_BUF_TEST];
    char value[AUDIO_ADAPTER_BUF_TEST] = "sup_sampling_rates=4800;sup_channels=1;sup_formats=2;";
    uint32_t valueLen = AUDIO_ADAPTER_BUF_TEST;

    int32_t ret = adapter_->GetExtraParams(adapter_, key, condition, value, valueLen);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterUpdateAudioRouteNull001, TestSize.Level1)
{
    struct AudioRoute route = {};
    int32_t routeHandle = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->UpdateAudioRoute(nullptr, &route, &routeHandle));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterUpdateAudioRouteIsvalid001, TestSize.Level1)
{
    struct AudioRoute route = {};
    int32_t routeHandle = 0;
    int32_t ret = adapter_->UpdateAudioRoute(adapter_, &route, &routeHandle);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterReleaseAudioRouteNull001, TestSize.Level1)
{
    int32_t routeHandle = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->ReleaseAudioRoute(nullptr, routeHandle));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterReleaseAudioRouteIsvalid001, TestSize.Level1)
{
    int32_t routeHandle = 0;
    int32_t ret = adapter_->ReleaseAudioRoute(adapter_, routeHandle);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_FAILURE);
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetVersionNull001, TestSize.Level1)
{
    uint32_t majorVer = 0;
    uint32_t minorVer = 0;
    EXPECT_NE(HDF_SUCCESS, adapter_->GetVersion(nullptr, &majorVer, &minorVer));
}

HWTEST_F(HdfAudioUtAdapterTest, HdfAudioAdapterGetVersionIsvalid001, TestSize.Level1)
{
    uint32_t majorVer = 0;
    uint32_t minorVer = 0;
    int32_t ret = adapter_->GetVersion(adapter_, &majorVer, &minorVer);
    ASSERT_TRUE(ret == HDF_ERR_NOT_SUPPORT || ret == HDF_SUCCESS);
}

/**
* @tc.name: HdfAudioHdiEnumTest001
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest001, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest001: start.");
    EXPECT_TRUE(PORT_OUT == 1);
    printf("HdfAudioHdiEnumTest001: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest002
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest002, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest002: start.");
    EXPECT_TRUE(PORT_IN == 2);
    printf("HdfAudioHdiEnumTest002: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest003
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest003, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest003: start.");
    EXPECT_TRUE(PORT_OUT_IN == 3);
    printf("HdfAudioHdiEnumTest003: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest005
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest005, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest005: start.");
    EXPECT_TRUE(PIN_NONE == 0);
    printf("HdfAudioHdiEnumTest005: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest006
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest006, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest006: start.");
    EXPECT_TRUE(PIN_OUT_SPEAKER == 1 << 0);
    printf("HdfAudioHdiEnumTest006: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest007
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest007, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest007: start.");
    EXPECT_TRUE(PIN_OUT_HEADSET == 1 << 1);
    printf("HdfAudioHdiEnumTest007: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest008
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest008, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest008: start.");
    EXPECT_TRUE(PIN_OUT_LINEOUT == 1 << 2);
    printf("HdfAudioHdiEnumTest008: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest009
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest009, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest009: start.");
    EXPECT_TRUE(PIN_OUT_HDMI == 1 << 3);
    printf("HdfAudioHdiEnumTest009: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest010
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest010, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest010: start.");
    EXPECT_TRUE(PIN_OUT_USB == 1 << 4);
    printf("HdfAudioHdiEnumTest010: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest011
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest011, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest011: start.");
    EXPECT_TRUE(PIN_OUT_USB_EXT == 1 << 5);
    printf("HdfAudioHdiEnumTest011: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest012
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest012, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest012: start.");
    EXPECT_TRUE(PIN_OUT_EARPIECE == (1 << 5 | 1 << 4));
    printf("HdfAudioHdiEnumTest012: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest013
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest013, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest013: start.");
    EXPECT_TRUE(PIN_OUT_BLUETOOTH_SCO == 1 << 6);
    printf("HdfAudioHdiEnumTest013: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest014
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest014, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest014: start.");
    EXPECT_TRUE(PIN_OUT_DAUDIO_DEFAULT == 1 << 7);
    printf("HdfAudioHdiEnumTest014: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest015
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest015, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest015: start.");
    EXPECT_TRUE(PIN_OUT_HEADPHONE == 1 << 8);
    printf("HdfAudioHdiEnumTest015: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest016
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest016, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest016: start.");
    EXPECT_TRUE(PIN_OUT_USB_HEADSET == 1 << 9);
    printf("HdfAudioHdiEnumTest016: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest017
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest017, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest017: start.");
    EXPECT_TRUE(PIN_IN_MIC == (1 << 27 | 1 << 0));
    printf("HdfAudioHdiEnumTest017: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest018
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest018, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest018: start.");
    EXPECT_TRUE(PIN_IN_HS_MIC == (1 << 27 | 1 << 1));
    printf("HdfAudioHdiEnumTest018: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest019
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest019, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest019: start.");
    EXPECT_TRUE(PIN_IN_LINEIN == (1 << 27 | 1 << 2));
    printf("HdfAudioHdiEnumTest019: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest020
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest020, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest020: start.");
    EXPECT_TRUE(PIN_IN_USB_EXT == (1 << 27 | 1 << 3));
    printf("HdfAudioHdiEnumTest020: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest021
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest021, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest021: start.");
    EXPECT_TRUE(PIN_IN_BLUETOOTH_SCO_HEADSET == (1 << 27 | 1 << 4));
    printf("HdfAudioHdiEnumTest021: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest022
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest022, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest022: start.");
    EXPECT_TRUE(PIN_IN_DAUDIO_DEFAULT == (1 << 27 | 1 << 5));
    printf("HdfAudioHdiEnumTest022: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest023
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest023, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest023: start.");
    EXPECT_TRUE(PIN_IN_USB_HEADSET == (1 << 27 | 1 << 6));
    printf("HdfAudioHdiEnumTest023: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest024
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest024, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest024: start.");
    EXPECT_TRUE(AUDIO_IN_MEDIA == 0);
    printf("HdfAudioHdiEnumTest024: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest025
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest025, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest025: start.");
    EXPECT_TRUE(AUDIO_IN_COMMUNICATION == 1);
    printf("HdfAudioHdiEnumTest025: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest026
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest026, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest026: start.");
    EXPECT_TRUE(AUDIO_IN_RINGTONE == 2);
    printf("HdfAudioHdiEnumTest026: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest027
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest027, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest027: start.");
    EXPECT_TRUE(AUDIO_IN_CALL == 3);
    printf("HdfAudioHdiEnumTest027: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest028
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest028, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest028: start.");
    EXPECT_TRUE(AUDIO_MMAP_NOIRQ == 4);
    printf("HdfAudioHdiEnumTest028: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest029
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest029, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest029: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_8_BIT == 1 << 0);
    printf("HdfAudioHdiEnumTest029: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest030
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest030, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest030: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_16_BIT == 1 << 1);
    printf("HdfAudioHdiEnumTest030: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest031
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest031, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest031: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_24_BIT == (1 << 1 | 1 << 0));
    printf("HdfAudioHdiEnumTest031: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest032
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest032, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest032: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_PCM_32_BIT == 1 << 2);
    printf("HdfAudioHdiEnumTest032: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest033
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest033, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest033: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_MAIN == (1 << 24 | 1 << 0));
    printf("HdfAudioHdiEnumTest033: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest034
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest034, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest034: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_LC == (1 << 24 | 1 << 1));
    printf("HdfAudioHdiEnumTest034: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest035
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest035, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest035: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_LD == (1 << 24 | 1 << 1 | 1 << 0));
    printf("HdfAudioHdiEnumTest035: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest036
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest036, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest036: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_ELD == (1 << 24 | 1 << 2));
    printf("HdfAudioHdiEnumTest036: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest037
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest037, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest037: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_HE_V1 == (1 << 24 | 1 << 2 | 1 << 0));
    printf("HdfAudioHdiEnumTest037: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest038
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest038, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest038: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_AAC_HE_V2 == (1 << 24 | 1 << 2 | 1 << 1));
    printf("HdfAudioHdiEnumTest038: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest039
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest039, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest039: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_G711A == (1 << 25 | 1 << 0));
    printf("HdfAudioHdiEnumTest039: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest040
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest040, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest040: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_G711U == (1 << 25 | 1 << 1));
    printf("HdfAudioHdiEnumTest040: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest041
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest041, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest041: start.");
    EXPECT_TRUE(AUDIO_FORMAT_TYPE_G726 == (1 << 25 | 1 << 1 | 1 << 0));
    printf("HdfAudioHdiEnumTest041: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest042
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest042, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest042: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_FRONT_LEFT == 1);
    printf("HdfAudioHdiEnumTest042: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest043
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest043, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest043: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_FRONT_RIGHT == 2);
    printf("HdfAudioHdiEnumTest043: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest044
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest044, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest044: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_MONO == 1);
    printf("HdfAudioHdiEnumTest044: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest045
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest045, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest045: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_STEREO == 3);
    printf("HdfAudioHdiEnumTest045: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest046
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest046, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest046: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_8000 == 1 << 0);
    printf("HdfAudioHdiEnumTest046: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest047
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest047, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest047: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_12000 == 1 << 1);
    printf("HdfAudioHdiEnumTest047: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest048
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest048, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest048: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_11025 == 1 << 2);
    printf("HdfAudioHdiEnumTest048: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest049
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest049, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest049: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_16000 == 1 << 3);
    printf("HdfAudioHdiEnumTest049: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest050
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest050, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest050: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_22050 == 1 << 4);
    printf("HdfAudioHdiEnumTest050: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest051
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest051, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest051: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_24000 == 1 << 5);
    printf("HdfAudioHdiEnumTest051: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest052
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest052, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest052: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_32000 == 1 << 6);
    printf("HdfAudioHdiEnumTest052: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest053
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest053, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest053: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_44100 == 1 << 7);
    printf("HdfAudioHdiEnumTest053: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest054
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest054, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest054: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_48000 == 1 << 8);
    printf("HdfAudioHdiEnumTest054: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest055
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest055, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest055: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_64000 == 1 << 9);
    printf("HdfAudioHdiEnumTest055: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest056
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest056, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest056: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_96000 == 1 << 10);
    printf("HdfAudioHdiEnumTest056: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest057
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest057, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest057: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_RATE_MASK_INVALID == 4294967295);
    printf("HdfAudioHdiEnumTest057: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest058
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest058, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest058: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_LPCM == 1 << 0);
    printf("HdfAudioHdiEnumTest058: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest059
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest059, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest059: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_RAW == 1 << 1);
    printf("HdfAudioHdiEnumTest059: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest060
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest060, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest060: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_HBR2LBR == 1 << 2);
    printf("HdfAudioHdiEnumTest060: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest061
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest061, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest061: start.");
    EXPECT_TRUE(PORT_PASSTHROUGH_AUTO == 1 << 3);
    printf("HdfAudioHdiEnumTest061: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest062
* @tc.desc: Judgment AudioSampleFormat enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest062, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest062: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S8 == 0);
    printf("HdfAudioHdiEnumTest062: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest063
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest063, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest063: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S8P == 1);
    printf("HdfAudioHdiEnumTest063: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest064
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest064, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest064: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U8 == 2);
    printf("HdfAudioHdiEnumTest064: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest065
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest065, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest065: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U8P == 3);
    printf("HdfAudioHdiEnumTest065: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest066
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest066, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest066: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S16 == 4);
    printf("HdfAudioHdiEnumTest066: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest067
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest067, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest067: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S16P == 5);
    printf("HdfAudioHdiEnumTest067: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest068
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest068, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest068: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U16 == 6);
    printf("HdfAudioHdiEnumTest068: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest069
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest069, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest069: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U16P == 7);
    printf("HdfAudioHdiEnumTest069: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest070
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest070, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest070: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S24 == 8);
    printf("HdfAudioHdiEnumTest070: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest071
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest071, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest071: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S24P == 9);
    printf("HdfAudioHdiEnumTest071: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest072
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest072, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest072: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U24 == 10);
    printf("HdfAudioHdiEnumTest072: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest073
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest073, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest073: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U24P == 11);
    printf("HdfAudioHdiEnumTest073: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest074
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest074, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest074: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S32 == 12);
    printf("HdfAudioHdiEnumTest074: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest075
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest075, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest075: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S32P == 13);
    printf("HdfAudioHdiEnumTest075: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest076
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest076, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest076: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U32 == 14);
    printf("HdfAudioHdiEnumTest076: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest077
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest077, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest077: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U32P == 15);
    printf("HdfAudioHdiEnumTest077: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest078
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest078, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest078: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S64 == 16);
    printf("HdfAudioHdiEnumTest078: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest079
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest079, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest079: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_S64P == 17);
    printf("HdfAudioHdiEnumTest079: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest080
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest080, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest080: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U64 == 18);
    printf("HdfAudioHdiEnumTest080: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest081
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest081, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest081: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_U64P == 19);
    printf("HdfAudioHdiEnumTest081: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest082
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest082, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest082: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F32 == 20);
    printf("HdfAudioHdiEnumTest082: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest083
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest083, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest083: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F32P == 21);
    printf("HdfAudioHdiEnumTest083: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest084
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest084, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest084: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F64 == 22);
    printf("HdfAudioHdiEnumTest084: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest085
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest085, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest085: start.");
    EXPECT_TRUE(AUDIO_SAMPLE_FORMAT_F64P == 23);
    printf("HdfAudioHdiEnumTest085: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest086
* @tc.desc: Judgment AudioChannelMode enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest086, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest086: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_NORMAL == 0);
    printf("HdfAudioHdiEnumTest086: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest087
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest087, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest087: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_BOTH_LEFT == 1);
    printf("HdfAudioHdiEnumTest087: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest088
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest088, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest088: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_BOTH_RIGHT == 2);
    printf("HdfAudioHdiEnumTest088: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest089
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest089, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest089: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_EXCHANGE == 3);
    printf("HdfAudioHdiEnumTest089: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest090
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest090, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest090: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_MIX == 4);
    printf("HdfAudioHdiEnumTest090: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest091
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest091, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest091: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_LEFT_MUTE == 5);
    printf("HdfAudioHdiEnumTest091: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest092
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest092, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest092: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_RIGHT_MUTE == 6);
    printf("HdfAudioHdiEnumTest092: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest093
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest093, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest093: start.");
    EXPECT_TRUE(AUDIO_CHANNEL_BOTH_MUTE == 7);
    printf("HdfAudioHdiEnumTest093: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest094
* @tc.desc: Judgment AudioDrainNotifyType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest094, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest094: start.");
    EXPECT_TRUE(AUDIO_DRAIN_NORMAL_MODE == 0);
    printf("HdfAudioHdiEnumTest094: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest095
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest095, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest095: start.");
    EXPECT_TRUE(AUDIO_DRAIN_EARLY_MODE == 1);
    printf("HdfAudioHdiEnumTest095: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest096
* @tc.desc: Judgment AudioCallbackType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest096, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest096: start.");
    EXPECT_TRUE(AUDIO_NONBLOCK_WRITE_COMPLETED == 0);
    printf("HdfAudioHdiEnumTest096: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest097
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest097, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest097: start.");
    EXPECT_TRUE(AUDIO_DRAIN_COMPLETED == 1);
    printf("HdfAudioHdiEnumTest097: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest098
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest098, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest098: start.");
    EXPECT_TRUE(AUDIO_FLUSH_COMPLETED == 2);
    printf("HdfAudioHdiEnumTest098: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest099
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest099, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest099: start.");
    EXPECT_TRUE(AUDIO_RENDER_FULL == 3);
    printf("HdfAudioHdiEnumTest099: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest100
* @tc.desc: Judgment AudioCallbackType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest100, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest100: start.");
    EXPECT_TRUE(AUDIO_ERROR_OCCUR == 4);
    printf("HdfAudioHdiEnumTest100: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest101
* @tc.desc: Judgment AudioPortRole enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest101, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest101: start.");
    EXPECT_TRUE(AUDIO_PORT_UNASSIGNED_ROLE == 0);
    printf("HdfAudioHdiEnumTest101: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest102
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest102, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest102: start.");
    EXPECT_TRUE(AUDIO_PORT_SOURCE_ROLE == 1);
    printf("HdfAudioHdiEnumTest102: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest103
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest103, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest103: start.");
    EXPECT_TRUE(AUDIO_PORT_SINK_ROLE == 2);
    printf("HdfAudioHdiEnumTest103: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest104
* @tc.desc: Judgment AudioPortType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest104, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest104: start.");
    EXPECT_TRUE(AUDIO_PORT_UNASSIGNED_TYPE == 0);
    printf("HdfAudioHdiEnumTest104: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest105
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest105, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest105: start.");
    EXPECT_TRUE(AUDIO_PORT_DEVICE_TYPE == 1);
    printf("HdfAudioHdiEnumTest105: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest106
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest106, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest106: start.");
    EXPECT_TRUE(AUDIO_PORT_MIX_TYPE == 2);
    printf("HdfAudioHdiEnumTest106: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest107
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest107, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest107: start.");
    EXPECT_TRUE(AUDIO_PORT_SESSION_TYPE == 3);
    printf("HdfAudioHdiEnumTest107: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest108
* @tc.desc: Judgment AudioSessionType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest108, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest108: start.");
    EXPECT_TRUE(AUDIO_OUTPUT_STAGE_SESSION == 0);
    printf("HdfAudioHdiEnumTest108: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest109
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest109, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest109: start.");
    EXPECT_TRUE(AUDIO_OUTPUT_MIX_SESSION == 1);
    printf("HdfAudioHdiEnumTest109: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest110
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest110, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest110: start.");
    EXPECT_TRUE(AUDIO_ALLOCATE_SESSION == 2);
    printf("HdfAudioHdiEnumTest110: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest111
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest111, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest111: start.");
    EXPECT_TRUE(AUDIO_INVALID_SESSION == 3);
    printf("HdfAudioHdiEnumTest111: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest112
* @tc.desc: Judgment AudioDeviceType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest112, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest112: start.");
    EXPECT_TRUE(AUDIO_LINEOUT == 1 << 0);
    printf("HdfAudioHdiEnumTest112: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest113
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest113, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest113: start.");
    EXPECT_TRUE(AUDIO_HEADPHONE == 1 << 1);
    printf("HdfAudioHdiEnumTest113: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest114
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest114, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest114: start.");
    EXPECT_TRUE(AUDIO_HEADSET == 1 << 2);
    printf("HdfAudioHdiEnumTest114: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest115
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest115, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest115: start.");
    EXPECT_TRUE(AUDIO_USB_HEADSET == 1 << 3);
    printf("HdfAudioHdiEnumTest115: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest116
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest116, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest116: start.");
    EXPECT_TRUE(AUDIO_USB_HEADPHONE == 1 << 4);
    printf("HdfAudioHdiEnumTest116: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest117
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest117, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest117: start.");
    EXPECT_TRUE(AUDIO_USBA_HEADSET == 1 << 5);
    printf("HdfAudioHdiEnumTest117: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest118
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest118, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest118: start.");
    EXPECT_TRUE(AUDIO_USBA_HEADPHONE == 1 << 6);
    printf("HdfAudioHdiEnumTest118: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest119
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest119, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest119: start.");
    EXPECT_TRUE(AUDIO_PRIMARY_DEVICE == 1 << 7);
    printf("HdfAudioHdiEnumTest119: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest120
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest120, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest120: start.");
    EXPECT_TRUE(AUDIO_USB_DEVICE == 1 << 8);
    printf("HdfAudioHdiEnumTest120: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest121
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest121, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest121: start.");
    EXPECT_TRUE(AUDIO_A2DP_DEVICE == 1 << 9);
    printf("HdfAudioHdiEnumTest121: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest122
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest122, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest122: start.");
    EXPECT_TRUE(AUDIO_HDMI_DEVICE == 1 << 10);
    printf("HdfAudioHdiEnumTest122: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest123
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest123, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest123: start.");
    EXPECT_TRUE(AUDIO_ADAPTER_DEVICE == 1 << 11);
    printf("HdfAudioHdiEnumTest123: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest124
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest124, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest124: start.");
    EXPECT_TRUE(AUDIO_DEVICE_UNKNOWN == (1 << 11) + 1);
    printf("HdfAudioHdiEnumTest124: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest125
* @tc.desc: Judgment AudioEventType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest125, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest125: start.");
    EXPECT_TRUE(AUDIO_DEVICE_ADD == 1);
    printf("HdfAudioHdiEnumTest125: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest126
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest126, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest126: start.");
    EXPECT_TRUE(AUDIO_DEVICE_REMOVE == 2);
    printf("HdfAudioHdiEnumTest126: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest127
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest127, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest127: start.");
    EXPECT_TRUE(AUDIO_LOAD_SUCCESS == 3);
    printf("HdfAudioHdiEnumTest127: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest128
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest128, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest128: start.");
    EXPECT_TRUE(AUDIO_LOAD_FAILURE == 4);
    printf("HdfAudioHdiEnumTest128: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest129
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest129, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest129: start.");
    EXPECT_TRUE(AUDIO_UNLOAD == 5);
    printf("HdfAudioHdiEnumTest129: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest130
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest130, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest130: start.");
    EXPECT_TRUE(AUDIO_SERVICE_VALID == 7);
    printf("HdfAudioHdiEnumTest130: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest131
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest131, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest131: start.");
    EXPECT_TRUE(AUDIO_SERVICE_INVALID == 8);
    printf("HdfAudioHdiEnumTest131: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest132
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest132, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest132: start.");
    EXPECT_TRUE(AUDIO_CAPTURE_THRESHOLD == 9);
    printf("HdfAudioHdiEnumTest132: end.");
}
/**
* @tc.name: HdfAudioHdiEnumTest133
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest133, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest133: start.");
    EXPECT_TRUE(AUDIO_EVENT_UNKNOWN == 10);
    printf("HdfAudioHdiEnumTest133: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest134
* @tc.desc: Judgment AudioExtParamKey enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest134, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest134: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_NONE == 0);
    printf("HdfAudioHdiEnumTest134: end.");
}
/**
* @tc.name: HdfAudioHdiEnumTest135
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest135, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest135: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_VOLUME == 1);
    printf("HdfAudioHdiEnumTest135: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest136
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest136, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest136: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_FOCUS == 2);
    printf("HdfAudioHdiEnumTest136: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest137
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest137, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest137: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_BUTTON == 3);
    printf("HdfAudioHdiEnumTest137: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest138
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest138, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest138: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_EFFECT == 4);
    printf("HdfAudioHdiEnumTest138: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest139
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest139, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest139: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_STATUS == 5);
    printf("HdfAudioHdiEnumTest139: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest140
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest140, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest140: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_USB_DEVICE == 101);
    printf("HdfAudioHdiEnumTest140: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest141
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest141, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest141: start.");
    EXPECT_TRUE(AUDIO_EXT_PARAM_KEY_LOWPOWER == 1000);
    printf("HdfAudioHdiEnumTest141: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest142
* @tc.desc: Judgment AudioInputType enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest142, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest142: start.");
    EXPECT_TRUE(AUDIO_INPUT_DEFAULT_TYPE == 0);
    printf("HdfAudioHdiEnumTest142: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest143
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest143, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest143: start.");
    EXPECT_TRUE(AUDIO_INPUT_MIC_TYPE == 1 << 0);
    printf("HdfAudioHdiEnumTest143: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest144
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest144, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest144: start.");
    EXPECT_TRUE(AUDIO_INPUT_SPEECH_WAKEUP_TYPE == 1 << 1);
    printf("HdfAudioHdiEnumTest144: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest145
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest145, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest145: start.");
    EXPECT_TRUE(AUDIO_INPUT_VOICE_COMMUNICATION_TYPE == 1 << 2);
    printf("HdfAudioHdiEnumTest145: end.");
}

/**
* @tc.name: HdfAudioHdiEnumTest146
* @tc.desc: Judgment enum value of the AudioTypes.idl
* @tc.type: FUNC
*/
HWTEST_F(HdfAudioUtAdapterTest, HdfAudioHdiEnumTest146, TestSize.Level3)
{
    printf("HdfAudioHdiEnumTest146: start.");
    EXPECT_TRUE(AUDIO_INPUT_VOICE_RECOGNITION_TYPE == 1 << 3);
    printf("HdfAudioHdiEnumTest146: end.");
}
}

