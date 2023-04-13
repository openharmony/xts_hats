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
#include "audio_hdirender_control_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;

namespace {
class AudioHdiRenderControlTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void AudioHdiRenderControlTest::SetUpTestCase(void){}

void AudioHdiRenderControlTest::TearDownTestCase(void) {}

void AudioHdiRenderControlTest::SetUp(void) {}

void AudioHdiRenderControlTest::TearDown(void) {}

/**
* @tc.name  Test AudioRenderStart API via  legal input
* @tc.number  SUB_Audio_HDI_RenderStart_0001
* @tc.desc  Test AudioRenderStart interface,return 0 if the audiorender object is created successfully.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderStart_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Start((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioRenderStart API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_RenderStart_0002
* @tc.desc  Test AudioRenderStart interface, return -1 if the  incoming parameter handle is nullptr
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderStart_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Start((AudioHandle)renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test AudioRenderStop API via legal input
* @tc.number  SUB_Audio_HDI_RenderStop_0001
* @tc.desc  test AudioRenderStop interface. return 0 if the rendering is successfully stopped.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderStop_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Start((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test AudioRenderStop API via the render does not start and stop only
* @tc.number  SUB_Audio_HDI_RenderStop_0002
* @tc.desc  test AudioRenderStop interface. return -4 if the render does not start and stop only
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderStop_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test RenderStop API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_RenderStop_0004
* @tc.desc  Test RenderStop interface, return -1 if the incoming parameter handle is nullptr
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderStop_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Start((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Stop((AudioHandle)renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderPause API via legal input
* @tc.number  SUB_Audio_HDI_RenderPause_001
* @tc.desc  test HDI RenderPause interface，return 0 if the render is paused after start
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderPause_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test AudioRenderPause API via the render is paused after created.
* @tc.number  SUB_Audio_HDI_RenderPause_0003
* @tc.desc  Test AudioRenderPause interface,return -1 if the render is paused after created.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderPause_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test AudioRenderPause API via the render is paused after resumed.
* @tc.number  SUB_Audio_HDI_RenderPause_0004
* @tc.desc  Test AudioRenderPause interface,return 0 if the render is paused after resumed.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderPause_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Resume((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test AudioRenderPause API via the render is paused after stoped.
* @tc.number  SUB_Audio_HDI_RenderPause_0005
* @tc.desc  Test AudioRenderPause interface, return -1 the render is paused after stopped.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderPause_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    TestAudioManager* manager = {};

    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Stop((AudioHandle)render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test RenderPause API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_RenderPause_0006
* @tc.desc  Test RenderPause interface, return -1 if the incoming parameter handle is nullptr
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderPause_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    TestAudioManager* manager = {};

    manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderResume API via the render is resumed after started
* @tc.number  SUB_Audio_HDI_RenderResume_0001
* @tc.desc  test HDI RenderResume interface,return -1 if the render is resumed after started
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderResume_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Resume((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderResume API via the render is resumed after stopped
* @tc.number  SUB_Audio_HDI_RenderResume_0002
* @tc.desc  test HDI RenderResume interface,return -1 if the render is resumed after stopped
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderResume_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Resume((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderResume API via legal input
* @tc.number  SUB_Audio_HDI_RenderResume_0003
* @tc.desc  Test AudioRenderResume interface,return 0 if the render is resumed after paused
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderResume_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Resume((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderResume API via the render Continue to start after resume
* @tc.number  SUB_Audio_HDI_RenderResume_0005
* @tc.desc  test HDI RenderResume interface,return -1 if the render Continue to start after resume
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderResume_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Resume((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Start((AudioHandle)render);
    if ((ret == AUDIO_HAL_SUCCESS) || (ret == AUDIO_HAL_ERR_AO_BUSY)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test RenderResume API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_RenderResume_0007
* @tc.desc  Test RenderResume interface, return -1 if the incoming parameter handle is nullptr
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderResume_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Resume((AudioHandle)renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateRender API via legal input.
* @tc.number  SUB_Audio_HDI_CreateRender_0001
* @tc.desc  test AudioCreateRender interface,return 0 if render is created successful.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test AudioCreateRender API via setting the incoming parameter pins is PIN_IN_MIC.
    * @tc.number  SUB_Audio_HDI_CreateRender_0003
    * @tc.desc  test AudioCreateRender interface,return -1 if the incoming parameter pins is PIN_IN_MIC.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioPort* renderPort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devDesc = {};

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    InitDevDesc(devDesc, renderPort->portId, PIN_IN_MIC);
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test AudioCreateRender API via setting the incoming parameter attr is error.
* @tc.number  SUB_Audio_HDI_CreateRender_0004
* @tc.desc  test AudioCreateRender interface,return -1 if the incoming parameter attr is error.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioPort* renderPort = nullptr;
    uint32_t channelCountErr = 5;
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    InitDevDesc(devDesc, renderPort->portId, PIN_OUT_SPEAKER);
    attrs.format = AUDIO_FORMAT_AAC_MAIN;
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    attrs.channelCount = channelCountErr;
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    attrs.type = AUDIO_IN_COMMUNICATION;
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateRender API via setting the incoming parameter adapter is nullptr
* @tc.number  SUB_Audio_HDI_CreateRender_0005
* @tc.desc  test AudioCreateRender interface,Returns -1 if the incoming parameter adapter is nullptr.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioAdapter *adapterNull = nullptr;
    struct AudioPort* renderPort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devDesc = {};
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    InitDevDesc(devDesc, renderPort->portId, PIN_OUT_SPEAKER);
    ret = adapter->CreateRender(adapterNull, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test AudioCreateRender API via setting the incoming parameter devDesc is nullptr
* @tc.number  SUB_Audio_HDI_CreateRender_0006
* @tc.desc  test AudioCreateRender interface,Returns -1 if the incoming parameter devDesc is nullptr.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioRender *render = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioPort* renderPort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor *devDescNull = nullptr;
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    ret = adapter->CreateRender(adapter, devDescNull, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test AudioCreateRender API via setting the incoming parameter attrs is nullptr
* @tc.number  SUB_Audio_HDI_CreateRender_0007
* @tc.desc  test AudioCreateRender interface,Returns -1 if the incoming parameter attrs is nullptr.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0007, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* renderPort = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes *attrsNull = nullptr;
    struct AudioDeviceDescriptor devDesc = {};

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitDevDesc(devDesc, renderPort->portId, PIN_OUT_SPEAKER);
    ret = adapter->CreateRender(adapter, &devDesc, attrsNull, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test AudioCreateRender API via setting the incoming parameter render is nullptr
* @tc.number  SUB_Audio_HDI_CreateRender_0008
* @tc.desc  test AudioCreateRender interface,Returns -1 if the incoming parameter render is nullptr.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0008, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender **renderNull = nullptr;
    struct AudioPort* renderPort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devDesc = {};

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    InitDevDesc(devDesc, renderPort->portId, PIN_OUT_SPEAKER);
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test AudioCreateRender API via setting the incoming parameter devDesc is error
* @tc.number  SUB_Audio_HDI_CreateRender_0009
* @tc.desc  test AudioCreateRender interface,Returns -1 if the incoming parameter devDesc is error.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0009, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioPort* renderPort = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = InitAttrs(attrs);
    InitDevDesc(devDesc, renderPort->portId, PIN_OUT_SPEAKER);
    devDesc.portId = -5;
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    devDesc.pins = PIN_NONE;
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    devDesc.desc = "devtestname";
    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test AudioCreateRender API via setting the incoming parameter desc which portID is not configed
    * @tc.number  SUB_Audio_HDI_CreateRender_0010
    * @tc.desc  test AudioCreateRender interface,Returns -1 if the incoming desc which portID is not configed
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_CreateRender_0010, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioPort* renderPort = nullptr;
    uint32_t portID = 10;

    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME, &adapter, renderPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = InitAttrs(attrs);
    InitDevDesc(devDesc, portID, PIN_OUT_SPEAKER);

    ret = adapter->CreateRender(adapter, &devDesc, &attrs, &render);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    ret = adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
    * @tc.name  Test AudioDestroyRender API via legal input.
    * @tc.number  SUB_Audio_HDI_DestroyRender_0001
    * @tc.desc  Test AudioDestroyRender interface, return 0 if render is destroyed successful.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_DestroyRender_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    TestAudioManager* manager = GetAudioManagerFuncs();

    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = adapter->DestroyRender(adapter, render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioDestroyRender API,where the parameter render is empty.
* @tc.number  SUB_Audio_HDI_DestroyRender_0002
* @tc.desc  Test AudioDestroyRender interface, return -1 if the parameter render is empty.
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_DestroyRender_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;
    TestAudioManager* manager = GetAudioManagerFuncs();

    ret = AudioCreateRender(manager, PIN_OUT_SPEAKER, ADAPTER_NAME, &adapter, &render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = adapter->DestroyRender(adapter, renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test RenderFlush API via legal input Verify that the data in the buffer is flushed after stop
* @tc.number  SUB_Audio_HDI_RenderFlush_0001
* @tc.desc  Test RenderFlush interface,return -2 if the data in the buffer is flushed successfully after stop
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderFlush_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;

    manager = GetAudioManagerFuncs();

    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Flush((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderFlush that the data in the buffer is flushed when handle is nullptr after paused
* @tc.number  SUB_Audio_HDI_RenderFlush_0002
* @tc.desc  Test RenderFlush, return -1 if the data in the buffer is flushed when handle is nullptr after paused
*/
HWTEST_F(AudioHdiRenderControlTest, SUB_Audio_HDI_RenderFlush_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioRender *renderNull = nullptr;

    manager = GetAudioManagerFuncs();

    ret = AudioCreateStartRender(manager, &render, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Pause((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = render->control.Flush((AudioHandle)renderNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = render->control.Stop((AudioHandle)render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyRender(adapter, render);
    manager->UnloadAdapter(manager, adapter);
}
}
