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
const int32_t MMAP_SUGGEST_BUFFER_SIZE = 1920;

class AudioUtRenderMmapTestAdditional : public testing::Test {
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
    void InitRenderAttrs(struct AudioSampleAttributes &attrs);
    void InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc);
    void FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

void AudioUtRenderMmapTestAdditional::InitRenderAttrs(struct AudioSampleAttributes &attrs)
{
    attrs.channelCount = AUDIO_RENDER_CHANNELCOUNT;
    attrs.sampleRate = AUDIO_SAMPLE_RATE_48K;
    attrs.interleaved = 0;
    attrs.type = AUDIO_MMAP_NOIRQ;
    attrs.period = DEEP_BUFFER_RENDER_PERIOD_SIZE;
    attrs.frameSize = AUDIO_FORMAT_TYPE_PCM_16_BIT * AUDIO_RENDER_CHANNELCOUNT / MOVE_LEFT_NUM;
    attrs.isBigEndian = false;
    attrs.isSignedData = true;
    attrs.startThreshold = DEEP_BUFFER_RENDER_PERIOD_SIZE / (attrs.format * attrs.channelCount / MOVE_LEFT_NUM);
    attrs.stopThreshold = INT_MAX;
    attrs.silenceThreshold = BUFFER_LENTH;
}

void AudioUtRenderMmapTestAdditional::InitRenderDevDesc(struct AudioDeviceDescriptor &devDesc)
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
    free(devDesc.desc);
}

void AudioUtRenderMmapTestAdditional::FreeAdapterElements(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
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

void AudioUtRenderMmapTestAdditional::ReleaseAllAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen)
{
    if (descs == nullptr || descsLen == 0) {
        return;
    }

    for (uint32_t i = 0; i < descsLen; i++) {
        FreeAdapterElements(&descs[i], false);
    }
}

void AudioUtRenderMmapTestAdditional::SetUp()
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

void AudioUtRenderMmapTestAdditional::TearDown()
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
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0100
 * @tc.name  testCommonRenderGetMmapPosition001
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition001, TestSize.Level1)
{
    uint64_t frames = 256;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0200
 * @tc.name  testCommonRenderGetMmapPosition002
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition002, TestSize.Level1)
{
    uint64_t frames = -256;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0300
 * @tc.name  testCommonRenderGetMmapPosition003
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition003, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = -1;
    time.tvSec = -1;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0400
 * @tc.name  testCommonRenderGetMmapPosition004
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition004, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = 21474836470;
    time.tvSec = 21474836470;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0500
 * @tc.name  testCommonRenderGetMmapPosition005
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition005, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = -21474836480;
    time.tvSec = -21474836480;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0600
 * @tc.name  testCommonRenderGetMmapPosition006
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition006, TestSize.Level1)
{
    uint64_t frames = 2;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0700
 * @tc.name  testCommonRenderGetMmapPosition007
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition007, TestSize.Level1)
{

    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, nullptr, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0800
 * @tc.name  testCommonRenderGetMmapPosition008
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition008, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, nullptr));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_0900
 * @tc.name  testCommonRenderGetMmapPosition009
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition009, TestSize.Level1)
{

    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t i;
    for (i = 0; i < 10; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, 0, nullptr));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1000
 * @tc.name  testCommonRenderGetMmapPosition010
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition010, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t reqSize = MMAP_SUGGEST_BUFFER_SIZE;
    struct AudioMmapBufferDescriptor desc;

    int32_t ret = render_->ReqMmapBuffer(render_, reqSize, &desc);
    ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_INVALID_PARAM);

    ASSERT_EQ(HDF_SUCCESS, render_->Start(render_));
    int32_t i;
    for (i = 0; i < 50; i++) {
        ret = render_->GetMmapPosition(render_, &frames, &time);
        ASSERT_TRUE(ret == HDF_SUCCESS);
    }
}
/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1100
 * @tc.name  testCommonRenderGetMmapPosition011
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition011, TestSize.Level1)
{
    uint64_t frames = 42949672950;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1200
 * @tc.name  testCommonRenderGetMmapPosition012
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition012, TestSize.Level1)
{
    uint64_t frames = 200;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1300
 * @tc.name  testCommonRenderGetMmapPosition013
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition013, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = 3;
    time.tvSec = 3;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1400
 * @tc.name  testCommonRenderGetMmapPosition014
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition014, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = -60;
    time.tvSec = -60;
    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1500
 * @tc.name  testCommonRenderGetMmapPosition015
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition015, TestSize.Level1)
{

    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t i;
    for (i = 0; i < 50; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, nullptr, nullptr));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1600
 * @tc.name  testCommonRenderGetMmapPosition016
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition016, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t i;
    for (i = 0; i < 60; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, &time));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1700
 * @tc.name  testCommonRenderGetMmapPosition017
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition017, TestSize.Level1)
{
    uint64_t frames = 0;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t i;
    for (i = 0; i < 70; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, nullptr));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1800
 * @tc.name  testCommonRenderGetMmapPosition018
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition018, TestSize.Level1)
{

    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t i;
    for (i = 0; i < 70; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, nullptr, &time));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_1900
 * @tc.name  testCommonRenderGetMmapPosition019
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition019, TestSize.Level1)
{

    struct AudioTimeStamp time;
    time.tvNSec = 256;
    time.tvSec = 256;
    int32_t i;
    for (i = 0; i < 70; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, nullptr, &time));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_GetMmapPosition_2000
 * @tc.name  testCommonRenderGetMmapPosition020
 * @tc.desc  Reliability of function(GetMmapPosition)
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderGetMmapPosition020, TestSize.Level1)
{
    uint64_t frames = 256;
    struct AudioTimeStamp time;
    time.tvNSec = 0;
    time.tvSec = 0;
    int32_t i;
    for (i = 0; i < 70; i++) {
        EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->GetMmapPosition(nullptr, &frames, nullptr));
    }
}

/**
 * @tc.number  SUB_Driver_Audio_ReqMmapBuffer_0300
 * @tc.name  testCommonRenderReqMmapBuffer003
 * @tc.desc  Test parameters with abnormal input
 */
HWTEST_F(AudioUtRenderMmapTestAdditional, testCommonRenderReqMmapBuffer003, Function | MediumTest | Level2)
{
    int32_t reqSize = 256;
    struct AudioMmapBufferDescriptor desc;

    EXPECT_EQ(HDF_ERR_INVALID_OBJECT, render_->ReqMmapBuffer(nullptr, reqSize, &desc));
}
} // end of namespace