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
 * @brief Defines audio-related APIs, including custom data types and functions for  capturing drivers,
 * accessing a driver adapter, and capturing audios.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_hdi_common.h
 *
 * @brief Declares APIs for operations related to audio adapter capturing
 *
 * @since 1.0
 * @version 1.0
 */

#include "audio_hdi_common.h"
#include "audio_hdicapture_control_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;

namespace {
class AudioHdiCaptureControlTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};
void AudioHdiCaptureControlTest::SetUpTestCase(void) {}

void AudioHdiCaptureControlTest::TearDownTestCase(void) {}

void AudioHdiCaptureControlTest::SetUp(void) {}

void AudioHdiCaptureControlTest::TearDown(void) {}

/**
* @tc.name  Test AudioCreateCapture API via legal input
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0001
* @tc.desc  Test the AudioCreateCapture API ,check whether 0 is returned if the AudioCapture object is created successfully
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via creating a capture object when a render object was created
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0002
* @tc.desc  Test the AudioCreateCapture API as follows:
     (1)service mode: 0 is returned  if the AudioCapture object  can be created successfully
     (2)passthrough mode: -1 is returned  if the AudioCapture object falis to be created
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *firstCapture = nullptr;
    struct AudioCapture *secondCapture = nullptr;
    struct AudioPort* audioPort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor DevDesc = {};

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    InitDevDesc(DevDesc, audioPort->portId, PIN_IN_MIC);
    ret = adapter->CreateCapture(adapter, &DevDesc, &attrs, &firstCapture);
    if (ret < 0) {
        manager->UnloadAdapter(manager, adapter);
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    ret = adapter->CreateCapture(adapter, &DevDesc, &attrs, &secondCapture);
#if defined (AUDIO_ADM_SERVICE)
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, secondCapture);
#endif
#if defined (AUDIO_ADM_SO) || defined (__LITEOS__)
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    adapter->DestroyCapture(adapter, firstCapture);
#endif
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via creating a capture object when a render object was created
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0003
* @tc.desc  After calling the  AudioCreateCapture API,check whether 0 is returned if the AudioCapture object is created successfully
            when AudioRender was created
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioRender *render = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioPort* audioPort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor renderDevDesc = {};
    struct AudioDeviceDescriptor captureDevDesc = {};

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(attrs);
    InitDevDesc(renderDevDesc, audioPort->portId, PIN_OUT_SPEAKER);
    InitDevDesc(captureDevDesc, audioPort->portId, PIN_IN_MIC);
    ret = adapter->CreateRender(adapter, &renderDevDesc, &attrs, &render);
    if (ret < 0) {
        manager->UnloadAdapter(manager, adapter);
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    ret = adapter->CreateCapture(adapter, &captureDevDesc, &attrs, &capture);
    if (ret < 0) {
        adapter->DestroyRender(adapter, render);
        manager->UnloadAdapter(manager, adapter);
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    adapter->DestroyRender(adapter, render);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via setting the incoming parameter adapter is nullptr
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0005
* @tc.desc  After calling the AudioCreateCapture API,check whether -1 is returned if the input parameter adapter is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes attrs = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioAdapter *adapterNull = nullptr;
    struct AudioCapture *capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitAttrs(attrs);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitDevDesc(devDesc, capturePort->portId, PIN_IN_MIC);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->CreateCapture(adapterNull, &devDesc, &attrs, &capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via setting the incoming parameter desc is nullptr
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0006
* @tc.desc  After calling the AudioCreateCapture API,check whether -1 is returned if the input parameter desc is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioSampleAttributes attrs = {};
    struct AudioDeviceDescriptor *devDesc = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitAttrs(attrs);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->CreateCapture(adapter, devDesc, &attrs, &capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via setting the incoming parameter attrs is nullptr
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0007
* @tc.desc After calling the AudioCreateCapture API,check whether -1 is returned if the input parameter attrs is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0007, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes *attrs = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitDevDesc(devDesc, capturePort->portId, PIN_IN_MIC);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->CreateCapture(adapter, &devDesc, attrs, &capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via setting the incoming parameter capture is nullptr
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0008
* @tc.desc  After calling the AudioCreateCapture API,check whether -1 is returned if the input parameter capture is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0008, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes attrs = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture **capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitAttrs(attrs);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitDevDesc(devDesc, capturePort->portId, PIN_IN_MIC);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->CreateCapture(adapter, &devDesc, &attrs, capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via setting the incoming parameter adapter which port type is PORT_OUT
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0008
* @tc.desc  After calling the AudioCreateCapture API,check whether -1 is returned if the port type of the specified adapter is PORT_OUT
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0009, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes attrs = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_OUT, ADAPTER_NAME_OUT, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitAttrs(attrs);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitDevDesc(devDesc, capturePort->portId, PIN_OUT_SPEAKER);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->CreateCapture(adapter, &devDesc, &attrs, &capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCreateCapture API via setting the incoming parameter desc which portID is not configed
* @tc.number  SUB_Audio_HDI_AudioCreateCapture_0010
* @tc.desc  Test AudioCreateCapture interface,Returns -1 if the incoming parameter desc which portID is not configed
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioCreateCapture_0010, TestSize.Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioDeviceDescriptor devDesc = {};
    struct AudioSampleAttributes attrs = {};
    uint32_t portID = 12;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitAttrs(attrs);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = InitDevDesc(devDesc, portID, PIN_IN_MIC);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->CreateCapture(adapter, &devDesc, &attrs, &capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioDestroyCapture API via legal input
* @tc.number  SUB_Audio_HDI_AudioDestroyCapture_0001
* @tc.desc  After calling the AudioDestroyCapture API, check whether 0 is returned if the AudioCapture object is destroyed
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioDestroyCapture_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->DestroyCapture(adapter, capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioDestroyCapture API via setting the incoming parameter adapter is nullptr
* @tc.number  SUB_Audio_HDI_AudioDestroyCapture_0002
* @tc.desc  After calling the AudioDestroyCapture API, check whether -1 is returned if the input parameter adapter is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioDestroyCapture_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioAdapter *adapter = nullptr;
    struct AudioAdapter *adapterNull = nullptr;
    struct AudioCapture *capture = nullptr;

    TestAudioManager* manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->DestroyCapture(adapterNull, capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    ret = adapter->DestroyCapture(adapter, capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioDestroyCapture API via setting the incoming parameter capture is nullptr
* @tc.number  SUB_Audio_HDI_AudioDestroyCapture_0003
* @tc.desc  After calling the AudioDestroyCapture API, check whether -1 is returned if the input parameter capture is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_AudioDestroyCapture_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    struct AudioPort* capturePort = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    
    TestAudioManager* manager = GetAudioManagerFuncs();
    ret = GetLoadAdapter(manager, PORT_IN, ADAPTER_NAME, &adapter, capturePort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = adapter->DestroyCapture(adapter, capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureStart API via legal input
* @tc.number  SUB_Audio_HDI_StartCapture_0001
* @tc.desc  After calling the CaptureStart API, check whether 0 is returned if the audiocapture object is started successfully
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStart_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Start((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureStart API via setting the incoming parameter handle is nullptr
* @tc.number  SUB_Audio_HDI_CaptureStart_0002
* @tc.desc  After calling the CaptureStart API, check whether -1 is returned if the incoming parameter handle is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStart_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = nullptr;
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Start((AudioHandle)captureNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureStart API via startingtwo capture objects continuously
* @tc.number  SUB_Audio_HDI_CaptureStart_0003
* @tc.desc  After the AudioCaptureStart API is successfully called twice , check whether 0 is returned 
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStart_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Start((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Start((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_AI_BUSY, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureStop API via legal input
* @tc.number  SUB_Audio_HDI_CaptureStop_0001
* @tc.desc  After calling the CaptureStop API, check whether 0 is returned if the audiocapture object is stopped successfully
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStop_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureStop API via stopping two capture objects continuously
* @tc.number  SUB_Audio_HDI_CaptureStop_0002
* @tc.desc  After calling the CaptureStop API twice , check whether -4 is returned 
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStop_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureStop API via starting an AudioCapture object that has been stopped
* @tc.number  SUB_Audio_HDI_CaptureStop_0003
* @tc.desc   After calling the CaptureStop API, check whether 0 is returned if the AudioCapture object that has been stopped is started
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStop_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Start((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test AudioCaptureStop API via stopping an AudioCapture object that has not been started
* @tc.number  SUB_Audio_HDI_CaptureStop_0004
* @tc.desc After calling the CaptureStop API, check whether -4 is returned if the Audiocapture object has not been
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStop_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name Test CaptureStop API via setting the  parameter handle to nullptr
* @tc.number  SUB_Audio_HDI_CaptureStop_0005
* @tc.desc After calling the CaptureStop API, check whether -1 is returned if the input parameter handle is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureStop_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)captureNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CapturePause API via legal input
* @tc.number  SUB_Audio_HDI_CapturePause_0001
* @tc.desc  After calling the CapturePause API， check whether -1 is returned if the capture that has been started is paused 
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CapturePause_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CapturePause API via calling it twice
* @tc.number  SUB_Audio_HDI_CapturePause_0002
* @tc.desc   After calling the CapturePause API twice ,check whether -1 is returned 
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CapturePause_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CapturePause API via setting the  parameter handle to nullptr
* @tc.number  SUB_Audio_HDI_CapturePause_0003
* @tc.desc   After calling the CapturePause API, check whether -1 is returned if the input parameter handle is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CapturePause_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)captureNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CapturePause API via pausing capture that has  not Started 
* @tc.number  SUB_Audio_HDI_CapturePause_0004
* @tc.desc After calling the CapturePause API, check whether -1 is returned if the capture that has  not Started is paused .
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CapturePause_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateCapture(manager, PIN_IN_MIC, ADAPTER_NAME, &adapter, &capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CapturePause API via pausing capture that has stopped
* @tc.number  SUB_Audio_HDI_CapturePause_0005
* @tc.desc After calling the CapturePause API,check whether -1 is returned if the capture that has stopped is paused
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CapturePause_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    manager = GetAudioManagerFuncs();
    

    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_INTERNAL, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureResume API via legal input
* @tc.number  SUB_Audio_HDI_CaptureResume_0001
* @tc.desc  After calling the CaptureResume API, check whether 0 is returned if the capture is resumed after being paused
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureResume API via calling it twice
* @tc.number  SUB_Audio_HDI_CaptureResume_0002
* @tc.desc   After calling the CaptureResume API twice ,check whether -1 is returned 
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}

/**
* @tc.name  Test CaptureResume API via resuming capture that has started
* @tc.number  SUB_Audio_HDI_CaptureResume_0003
* @tc.desc  After calling the CaptureResume API, check whether -1 is returned if the capture is resumed after started
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0003, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureResume API via setting the input parameter handle to nullptr
* @tc.number  SUB_Audio_HDI_CaptureResume_0004
* @tc.desc  After calling the CaptureResume API,check whether -1 is returned if the input parameter handle is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0004, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)captureNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureResume API via  resuming capture after stopped
* @tc.number  SUB_Audio_HDI_CaptureResume_0005
* @tc.desc  After calling the CaptureResume API, check whether -1 is returned if the capture is resumed after stopped
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0005, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureResume API via the continuing to start capture after resumed
* @tc.number  SUB_Audio_HDI_CaptureResume_0006
* @tc.desc  After calling the CaptureResume API, check whether -1 is returned if the capture is started contionously after resumed
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0006, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Pause((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Resume((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Start((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_ERR_AI_BUSY, ret);

    capture->control.Stop((AudioHandle)capture);
    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test RenderResume API via starting,pausing,resuming, and stopping different capture objects     
* @tc.number  SUB_Audio_HDI_CaptureResume_0007
* @tc.desc  After calling the RenderResume API, check whether 0 is returned if different objects are started,paused,resumed and stopped
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureResume_0007, Function | MediumTest | Level1)
{
    int32_t ret1 = -1;
    int32_t ret2 = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapterOne = nullptr;
    struct AudioAdapter *adapterSec = nullptr;
    struct AudioCapture *captureOne = nullptr;
    struct AudioCapture *captureSec = nullptr;
    manager = GetAudioManagerFuncs();
    
    ret1 = AudioCreateStartCapture(manager, &captureOne, &adapterOne, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret1);
    ret1 = captureOne->control.Pause((AudioHandle)captureOne);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret1);
    ret1 = captureOne->control.Resume((AudioHandle)captureOne);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret1);
    ret1 = captureOne->control.Stop((AudioHandle)captureOne);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret1);
    adapterOne->DestroyCapture(adapterOne, captureOne);
    manager->UnloadAdapter(manager, adapterOne);
    ret2 = AudioCreateStartCapture(manager, &captureSec, &adapterSec, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret2);
    ret2 = captureSec->control.Pause((AudioHandle)captureSec);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret2);
    ret2 = captureSec->control.Resume((AudioHandle)captureSec);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret2);
    ret2 = captureSec->control.Stop((AudioHandle)captureSec);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret2);
    adapterSec->DestroyCapture(adapterSec, captureSec);
    manager->UnloadAdapter(manager, adapterSec);
}
/**
    * @tc.name  Test CaptureFlush API via flushing the data in the buffer  after stopped
    * @tc.number  SUB_Audio_HDI_CaptureFlush_0001
    * @tc.desc   After calling the CaptureFlush API,check whether -2 is returned if the data in the buffer is flushed successfully after stopped.
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureFlush_0001, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Flush((AudioHandle)capture);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
/**
* @tc.name  Test CaptureFlush via flushing the data in the buffer when handle is nullptr
* @tc.number  SUB_Audio_HDI_CaptureFlush_0002
* @tc.desc After calling the CaptureFlush API,check whether -1 is returned if the data in the buffer is flushed when handle is nullptr
*/
HWTEST_F(AudioHdiCaptureControlTest, SUB_Audio_HDI_CaptureFlush_0002, Function | MediumTest | Level1)
{
    int32_t ret = -1;
    TestAudioManager* manager = {};
    struct AudioAdapter *adapter = nullptr;
    struct AudioCapture *capture = nullptr;
    struct AudioCapture *captureNull = nullptr;

    manager = GetAudioManagerFuncs();
    
    ret = AudioCreateStartCapture(manager, &capture, &adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Stop((AudioHandle)capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = capture->control.Flush((AudioHandle)captureNull);
    EXPECT_EQ(AUDIO_HAL_ERR_INVALID_PARAM, ret);

    adapter->DestroyCapture(adapter, capture);
    manager->UnloadAdapter(manager, adapter);
}
}
