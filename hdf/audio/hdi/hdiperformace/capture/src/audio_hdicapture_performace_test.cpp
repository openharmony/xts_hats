/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
 * @brief Test audio recording interface delayTime.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_hdi_common.h
 *
 * @brief Declares APIs for operations related to the audio delayTime.
 *
 * @since 1.0
 * @version 1.0
 */
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include "audio_hdi_common.h"
#include "audio_hdicapture_performace_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;

namespace {
const int BUFFER = 1024 * 4;
class AudioCaptureBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
    static TestAudioManager *(*GetAudioManager)();
    static void *handleSo;
};

TestAudioManager *(*AudioCaptureBenchmarkTest::GetAudioManager)() = nullptr;
void *AudioCaptureBenchmarkTest::handleSo = nullptr;

void AudioCaptureBenchmarkTest::SetUp(const ::benchmark::State &state) {
    char absPath[PATH_MAX] = {0};
    if (realpath(RESOLVED_PATH.c_str(), absPath) == nullptr) {
        return;
    }
    handleSo = dlopen(absPath, RTLD_LAZY);
    if (handleSo == nullptr) {
        return;
    }
    GetAudioManager = (TestAudioManager *(*)())(dlsym(handleSo, FUNCTION_NAME.c_str()));
    if (GetAudioManager == nullptr) {
        return;
    }
};

void AudioCaptureBenchmarkTest::TearDown(const ::benchmark::State &state) {
    if (handleSo != nullptr) {
        dlclose(handleSo);
        handleSo = nullptr;
    }
    if (GetAudioManager != nullptr) {
        GetAudioManager = nullptr;
    }
};

/**
* @tc.name  the performace of AudioCreateCapture
* @tc.number  SUB_DriverSystem_Benchmark_AudioCreateCapture_0001
* @tc.devDesc  tests the performace of AudioCreateCapture interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCreateCapture_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName,
                         &audiopara.adapter, audiopara.audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(audiopara.attrs);
    InitDevDesc(audiopara.devDesc, audiopara.audioPort->portId, audiopara.pins);
    for (auto _ : st) {
        ret = audiopara.adapter->CreateCapture(audiopara.adapter, &audiopara.devDesc, &audiopara.attrs,
                                               &audiopara.capture);
        ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
        audiopara.capture = nullptr;
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCreateCapture_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioDestroyCapture
* @tc.number  SUB_DriverSystem_Benchmark_AudioDestroyCapture_0001
* @tc.devDesc  tests the performace of AudioDestroyCapture interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioDestroyCapture_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName,
                         &audiopara.adapter, audiopara.audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(audiopara.attrs);
    InitDevDesc(audiopara.devDesc, audiopara.audioPort->portId, audiopara.pins);
    for (auto _ : st) {
        ret = audiopara.adapter->CreateCapture(audiopara.adapter, &audiopara.devDesc, &audiopara.attrs,
                                               &audiopara.capture);
        ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
        audiopara.capture = nullptr;
    }

    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioDestroyCapture_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureStart
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureStart_0001
* @tc.devDesc  tests the performace of AudioCaptureStart interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureStart_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                                 &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->control.Start((AudioHandle)audiopara.capture);
        audiopara.capture->control.Stop((AudioHandle)audiopara.capture);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureStart_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCapturePause
* @tc.number  SUB_DriverSystem_Benchmark_AudioCapturePause_0001
* @tc.devDesc  tests the performace of AudioCapturePause interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCapturePause_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->control.Start((AudioHandle)audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->control.Pause((AudioHandle)audiopara.capture);
        audiopara.capture->control.Resume((AudioHandle)audiopara.capture);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCapturePause_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioCaptureResume
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureResume_0001
* @tc.devDesc  tests the performace of AudioCaptureResume interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureResume_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->control.Start((AudioHandle)audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->control.Pause((AudioHandle)audiopara.capture);
        ret = audiopara.capture->control.Resume((AudioHandle)audiopara.capture);
    }
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureResume_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioCaptureStop
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureStop_0001
* @tc.devDesc  tests the performace of AudioCaptureStop interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureStop_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                                 &audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        audiopara.capture->control.Start((AudioHandle)audiopara.capture);
        ret = audiopara.capture->control.Stop((AudioHandle)audiopara.capture);
    }
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureStop_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureSetSampleAttributes
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureSetSampleAttributes_0001
* @tc.devDesc  tests the performace of AudioCaptureSetSampleAttributes interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetSampleAttributes_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(audiopara.attrs);
    for (auto _ : st) {
        ret = audiopara.capture->attr.SetSampleAttributes(audiopara.capture, &audiopara.attrs);
    }
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetSampleAttributes_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureCaptureFrame
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureCaptureFrame_0001
* @tc.devDesc  tests the performace of AudioCaptureCaptureFrame interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureCaptureFrame_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0,
        .requestBytes = BUFFER_LENTH
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    audiopara.frame = (char *)calloc(1, BUFFER_LENTH);
    ASSERT_NE(nullptr, audiopara.frame);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName, &(audiopara.adapter),
                         audiopara.audioPort);
    if (ret < 0 || audiopara.adapter == nullptr) {
        free(audiopara.frame);
        audiopara.frame = nullptr;
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    InitAttrs(audiopara.attrs);
    audiopara.attrs.silenceThreshold = BUFFER;
    InitDevDesc(audiopara.devDesc, audiopara.audioPort->portId, audiopara.pins);
    ret = audiopara.adapter->CreateCapture(audiopara.adapter, &audiopara.devDesc, &audiopara.attrs,
                                           &audiopara.capture);
    if (ret < 0) {
        audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
        free(audiopara.frame);
        audiopara.frame = nullptr;
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    ret = audiopara.capture->control.Start((AudioHandle)audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->CaptureFrame(audiopara.capture, audiopara.frame, audiopara.requestBytes,
                                              &audiopara.replyBytes);
    }
    ret = audiopara.capture->control.Stop((AudioHandle)audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    free(audiopara.frame);
    audiopara.frame = nullptr;
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureCaptureFrame_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetSampleAttributes
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetSampleAttributes_0001
* @tc.devDesc  tests the performace of AudioCaptureGetSampleAttributes interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetSampleAttributes_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(audiopara.attrs);
    ret = audiopara.capture->attr.SetSampleAttributes(audiopara.capture, &audiopara.attrs);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->attr.GetSampleAttributes(audiopara.capture, &audiopara.attrsValue);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetSampleAttributes_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureSetMute
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureSetMute_0001
* @tc.devDesc  tests the performace of AudioCaptureSetMute interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetMute_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->volume.SetMute(audiopara.capture, false);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->volume.GetMute(audiopara.capture, &audiopara.character.getmute);
    EXPECT_FALSE(audiopara.character.getmute);

    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetMute_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetMute
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetMute_0001
* @tc.devDesc  tests the performace of AudioCaptureGetMute interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetMute_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->volume.SetMute(audiopara.capture, false);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->volume.GetMute(audiopara.capture, &audiopara.character.getmute);
    }
    EXPECT_FALSE(audiopara.character.getmute);

    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetMute_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureSetVolume
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureSetVolume_0001
* @tc.devDesc  tests the performace of AudioCaptureSetVolume interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetVolume_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0,
        .character.setvolume = 0.7
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->volume.SetVolume(audiopara.capture, audiopara.character.setvolume);
    }
    ret = audiopara.capture->volume.GetVolume(audiopara.capture, &audiopara.character.getvolume);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(audiopara.character.setvolume, audiopara.character.getvolume);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetVolume_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioCaptureGetVolume
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetVolume_0001
* @tc.devDesc  tests the performace of AudioCaptureGetVolume interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetVolume_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0,
        .character.setvolume = 0.8
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->volume.SetVolume(audiopara.capture, audiopara.character.setvolume);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->volume.GetVolume(audiopara.capture, &audiopara.character.getvolume);
    }
    EXPECT_EQ(audiopara.character.setvolume, audiopara.character.getvolume);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetVolume_0001)->
    Iterations(100)-> Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioCaptureGetGain
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetGain_0001
* @tc.devDesc  tests the performace of AudioCaptureGetGain interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetGain_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0,
        .character.setgain = 7
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->volume.SetGain(audiopara.capture, audiopara.character.setgain);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->volume.GetGain(audiopara.capture, &audiopara.character.getgain);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(audiopara.character.setgain, audiopara.character.getgain);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetGain_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureSetGain
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureSetGain_0001
* @tc.devDesc  tests the performace of AudioCaptureSetGain interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetGain_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0,
        .character.setgain = 8
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->volume.SetGain(audiopara.capture, audiopara.character.setgain);
    }
    ret = audiopara.capture->volume.GetGain(audiopara.capture, &audiopara.character.getgain);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(audiopara.character.setgain, audiopara.character.getgain);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetGain_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetCurrentChannelId
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetCurrentChannelId_0001
* @tc.devDesc  tests the performace of AudioCaptureGetCurrentChannelId interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetCurrentChannelId_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
       
        ret = audiopara.capture->attr.GetCurrentChannelId(audiopara.capture, &audiopara.character.getcurrentchannelId);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);;
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetCurrentChannelId_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetFrameCount
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetFrameCount_0001
* @tc.devDesc  tests the performace of AudioCaptureGetFrameCount interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetFrameCount_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->attr.GetFrameCount(audiopara.capture, &audiopara.character.getframecount);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(INITIAL_VALUE, audiopara.character.getframecount);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetFrameCount_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetFrameSize
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetFrameSize_0001
* @tc.devDesc  tests the performace of AudioCaptureGetFrameSize interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetFrameSize_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->attr.GetFrameSize(audiopara.capture, &audiopara.character.getframesize);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_GT(audiopara.character.getframesize, INITIAL_VALUE);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetFrameSize_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureFlush
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureFlush_0001
* @tc.devDesc  tests the performace of AudioCaptureFlush interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureFlush_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->control.Start((AudioHandle)audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->control.Flush((AudioHandle)audiopara.capture);
    }
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureFlush_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetGainThreshold
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetGainThreshold_0001
* @tc.devDesc  tests the performace of AudioCaptureGetGainThreshold interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetGainThreshold_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
       
        ret = audiopara.capture->volume.GetGainThreshold(audiopara.capture, &audiopara.character.gainthresholdmin,
                &audiopara.character.gainthresholdmax);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(audiopara.character.gainthresholdmin, GAIN_MIN);
    EXPECT_EQ(audiopara.character.gainthresholdmax, GAIN_MAX);
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetGainThreshold_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureCheckSceneCapability
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureCheckSceneCapability_0001
* @tc.devDesc  tests the performace of AudioCaptureCheckSceneCapability interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureCheckSceneCapability_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    struct AudioSceneDescriptor scenes = { .scene.id = 0, .desc.pins = PIN_IN_MIC };
    bool supported = false;
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->scene.CheckSceneCapability(audiopara.capture, &scenes, &supported);
    }
#ifdef AUDIO_HAL_NOTSUPPORT_PATHSELECT
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);
#else
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
#endif
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureCheckSceneCapability_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureSelectScene
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureSelectScene_0001
* @tc.devDesc  tests the performace of AudioCaptureSelectScene interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSelectScene_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    struct AudioSceneDescriptor scenes = { .scene.id = 0, .desc.pins = PIN_IN_MIC };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->scene.SelectScene(audiopara.capture, &scenes);
    }
#ifdef AUDIO_HAL_NOTSUPPORT_PATHSELECT
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);
#else
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
#endif
    ret = audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.capture = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSelectScene_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioGetCapturePosition
* @tc.number  SUB_DriverSystem_Benchmark_AudioGetCapturePosition_0001
* @tc.devDesc  tests the performace of AudioCaptureGetCapturePosition interface by executing 100 times,
*              and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioGetCapturePosition_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->control.Start((AudioHandle)audiopara.capture);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.capture->GetCapturePosition(audiopara.capture, &audiopara.character.getframes, &audiopara.time);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioGetCapturePosition_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioCaptureSetExtraParams
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureSetExtraParams_0001
* @tc.desc  tests the performace of AudioCaptureSetExtraParams interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetExtraParams_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    char keyValueList[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000";
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateStartCapture(audiopara.manager, &audiopara.capture, &audiopara.adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.capture->attr.SetExtraParams((AudioHandle)audiopara.capture, keyValueList);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureSetExtraParams_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetExtraParams
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetExtraParams_0001
* @tc.desc  tests the performace of AudioCaptureGetExtraParams interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetExtraParams_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    char keyValueList[] = "attr-format=24;attr-frame-count=4096;";
    char keyValueListExp[] = "attr-route=0;attr-format=24;attr-channels=2;attr-frame-count=4096;attr-sampling-rate=48000";
    char keyValueListValue[256] = {};
    int32_t listLenth = 256;
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);

    ret = AudioCreateStartCapture(audiopara.manager, &audiopara.capture, &audiopara.adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.capture->attr.SetExtraParams((AudioHandle)audiopara.capture, keyValueList);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        (void)memset_s(&keyValueListValue, sizeof(keyValueListValue), 0, sizeof(keyValueListValue));
        ret = audiopara.capture->attr.GetExtraParams((AudioHandle)audiopara.capture, keyValueListValue, listLenth);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_STREQ(keyValueListExp, keyValueListValue);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetExtraParams_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCaptureGetMmapPosition
* @tc.number  SUB_DriverSystem_Benchmark_AudioCaptureGetMmapPosition_0001
* @tc.desc  tests the performace of AudioCaptureGetMmapPosition interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetMmapPosition_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    uint64_t frames = 0;
    int64_t timeExp = 0;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_IN, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_IN_MIC, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateCapture(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                             &audiopara.capture);
    if (ret < 0 || audiopara.capture == nullptr) {
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
        ASSERT_EQ(nullptr, audiopara.capture);
    }

    for (auto _ : st) {
        ret = audiopara.capture->attr.GetMmapPosition(audiopara.capture, &frames, &(audiopara.time));
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ((audiopara.time.tvSec) * SECTONSEC + (audiopara.time.tvNSec), timeExp);
    EXPECT_EQ(frames, INITIAL_VALUE);
    audiopara.adapter->DestroyCapture(audiopara.adapter, audiopara.capture);
    audiopara.adapter = nullptr;
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioCaptureBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCaptureGetMmapPosition_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
}
BENCHMARK_MAIN();