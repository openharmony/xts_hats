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
 * @brief Test the delayTime of audio playback interface.
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
#include "audio_hdirender_performace_test.h"

using namespace std;
using namespace testing::ext;
using namespace OHOS::Audio;
namespace {
class AudioHdiRenderBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
    static TestAudioManager *(*GetAudioManager)();
    static void *handleSo;
};

TestAudioManager *(*AudioHdiRenderBenchmarkTest::GetAudioManager)() = nullptr;
void *AudioHdiRenderBenchmarkTest::handleSo = nullptr;

void AudioHdiRenderBenchmarkTest::SetUp(const ::benchmark::State &state)
{
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
}

void AudioHdiRenderBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    if (handleSo != nullptr) {
        dlclose(handleSo);
        handleSo = nullptr;
    }
    if (GetAudioManager != nullptr) {
        GetAudioManager = nullptr;
    }
}
/**
* @tc.name  the performace of AudioManagerGetAllAdapters
* @tc.number  SUB_DriverSystem_Benchmark_AudioManagerGetAllAdapters_0001
* @tc.desc  tests the performace of AudioManagerGetAllAdapters interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerGetAllAdapters_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    int size = 0;
    struct PrepareAudioPara audiopara = { .totalTime = 0 };

    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);

    for (auto _ : st) {
        ret = audiopara.manager->GetAllAdapters(audiopara.manager, &audiopara.descs, &size);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerGetAllAdapters_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioManagerLoadAdapter
* @tc.number  SUB_DriverSystem_Benchmark_AudioManagerLoadAdapter_0001
* @tc.desc  tests the performace of AudioManagerLoadAdapter interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerLoadAdapter_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    int size = 0;
    struct PrepareAudioPara audiopara = { .totalTime = 0 };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = audiopara.manager->GetAllAdapters(audiopara.manager, &audiopara.descs, &size);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.desc = &audiopara.descs[0];
    ASSERT_NE(nullptr, audiopara.desc);

    for (auto _ : st) {

        ret = audiopara.manager->LoadAdapter(audiopara.manager, audiopara.desc, &audiopara.adapter);
        audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
        audiopara.adapter = nullptr;
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerLoadAdapter_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioManagerUnLoadAdapter
* @tc.number  SUB_DriverSystem_Benchmark_AudioManagerUnLoadAdapter_0001
* @tc.desc  tests the performace of AudioManagerLoadAdapter interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerUnLoadAdapter_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    int size = 0;
    struct PrepareAudioPara audiopara = { .totalTime = 0 };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = audiopara.manager->GetAllAdapters(audiopara.manager, &audiopara.descs, &size);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.desc = &audiopara.descs[0];
    ASSERT_NE(nullptr, audiopara.desc);

    for (auto _ : st) {
        ret = audiopara.manager->LoadAdapter(audiopara.manager, audiopara.desc, &audiopara.adapter);
        audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
        audiopara.adapter = nullptr;
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerUnLoadAdapter_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioInitAllPorts
* @tc.number  SUB_DriverSystem_Benchmark_AudioManagerInitAllPorts_0001
* @tc.desc  tests the performace of AudioInitAllPorts interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerInitAllPorts_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName,
                         &audiopara.adapter, audiopara.audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.adapter->InitAllPorts(audiopara.adapter);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioManagerInitAllPorts_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioGetPortCapability
* @tc.number  SUB_DriverSystem_Benchmark_AudioGetPortCapability_0001
* @tc.desc  tests the performace of AudioGetPortCapability interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioGetPortCapability_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName,
                         &audiopara.adapter, audiopara.audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.adapter->InitAllPorts(audiopara.adapter);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.adapter->GetPortCapability(audiopara.adapter, audiopara.audioPort, &audiopara.capability);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioGetPortCapability_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioSetPassthroughMode
* @tc.number  SUB_DriverSystem_Benchmark_AudioSetPassthroughMode_0001
* @tc.desc  tests the performace of AudioSetPassthroughMode interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioSetPassthroughMode_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .mode = PORT_PASSTHROUGH_LPCM,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName,
                         &audiopara.adapter, audiopara.audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.adapter->InitAllPorts(audiopara.adapter);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.adapter->SetPassthroughMode(audiopara.adapter, audiopara.audioPort, audiopara.mode);
    }
    ret = audiopara.adapter->GetPassthroughMode(audiopara.adapter, audiopara.audioPort, &audiopara.mode);
    EXPECT_EQ(PORT_PASSTHROUGH_LPCM, audiopara.mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioSetPassthroughMode_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioGetPassthroughMode
* @tc.number  SUB_DriverSystem_Benchmark_AudioGetPassthroughMode_0001
* @tc.desc  tests the performace of AudioGetPassthroughMode interface by executing 100 times,
* and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioGetPassthroughMode_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .mode = PORT_PASSTHROUGH_LPCM,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = GetLoadAdapter(audiopara.manager, audiopara.portType, audiopara.adapterName,
                         &audiopara.adapter, audiopara.audioPort);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.adapter->InitAllPorts(audiopara.adapter);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.adapter->SetPassthroughMode(audiopara.adapter, audiopara.audioPort, audiopara.mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.adapter->GetPassthroughMode(audiopara.adapter, audiopara.audioPort, &audiopara.mode);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(PORT_PASSTHROUGH_LPCM, audiopara.mode);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioGetPassthroughMode_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetLatency
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetLatency_0001
* @tc.desc  tests the performace of AudioRenderGetLatency interface by executing 100 times,
* and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetLatency_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .path = AUDIO_FILE.c_str(), .totalTime = 0
    };
    uint32_t latencyTimeExpc = 0;
    uint32_t latencyTime = 0;
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = PlayAudioFile(audiopara);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->GetLatency(audiopara.render, &latencyTime);
    }

    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_LT(latencyTimeExpc, latencyTime);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetLatency_0001)
    ->Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioCreateRender
* @tc.number  SUB_DriverSystem_Benchmark_AudioCreateRender_0001
* @tc.desc  tests the performace of AudioCreateRender interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCreateRender_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
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
        ret = audiopara.adapter->CreateRender(audiopara.adapter, &audiopara.devDesc, &audiopara.attrs,
                                              &audiopara.render);
        audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    }
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioCreateRender_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioDestroyRender
* @tc.number  SUB_DriverSystem_Benchmark_AudioDestroyRender_0001
* @tc.desc  tests the performace of AudioDestroyRender interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioDestroyRender_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
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
        ret = audiopara.adapter->CreateRender(audiopara.adapter, &audiopara.devDesc, &audiopara.attrs,
                                              &audiopara.render);
        audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    }
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioDestroyRender_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetRenderPosition
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetRenderPosition_0001
* @tc.desc  tests the performace of AudioRenderGetRenderPosition interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetRenderPosition_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .path = AUDIO_FILE.c_str(), .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = PlayAudioFile(audiopara);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->GetRenderPosition(audiopara.render, &audiopara.character.getframes,
                &audiopara.time);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetRenderPosition_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderSetRenderSpeed
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderSetRenderSpeed_0001
* @tc.desc  tests the performace of AudioRenderSetRenderSpeed interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetRenderSpeed_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    float speedNormal = 30;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->SetRenderSpeed(audiopara.render, speedNormal);
    }
    ret = audiopara.render->GetRenderSpeed(audiopara.render, &speedNormal);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetRenderSpeed_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetRenderSpeed
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetRenderSpeed_0001
* @tc.desc  tests the performace of AudioRenderGetRenderSpeed interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetRenderSpeed_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    float speedValue = 30;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->GetRenderSpeed(audiopara.render, &speedValue);
    }
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetRenderSpeed_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderSetChannelMode
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderSetChannelMode_0001
* @tc.desc  tests the performace of AudioRenderSetChannelMode interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetChannelMode_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    enum AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->SetChannelMode(audiopara.render, mode);
    }
    ret = audiopara.render->GetChannelMode(audiopara.render, &mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(AUDIO_CHANNEL_NORMAL, mode);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetChannelMode_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetChannelMode
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetChannelMode_0001
* @tc.desc  tests the performace of AudioRenderGetChannelMode interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetChannelMode_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    enum AudioChannelMode mode = AUDIO_CHANNEL_NORMAL;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->SetChannelMode(audiopara.render, mode);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->GetChannelMode(audiopara.render, &mode);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(AUDIO_CHANNEL_NORMAL, mode);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetChannelMode_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetFrameCount
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetFrameCount_0001
* @tc.desc  tests the performace of AudioRenderGetFrameCount interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetFrameCount_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->attr.GetFrameCount(audiopara.render, &audiopara.character.getframecount);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetFrameCount_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetCurrentChannelId
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetCurrentChannelId_0001
* @tc.desc  tests the performace of AudioRenderGetCurrentChannelId interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetCurrentChannelId_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->attr.GetCurrentChannelId(audiopara.render, &audiopara.character.getcurrentchannelId);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetCurrentChannelId_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderFlush
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderFlush_0001
* @tc.desc  tests the performace of AudioRenderFlush interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderFlush_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                                &audiopara.render);
    audiopara.render->control.Start((AudioHandle)audiopara.render);
    for (auto _ : st) {
        ret = audiopara.render->control.Flush((AudioHandle)audiopara.render);
    }
    StopAudio(audiopara);
    EXPECT_EQ(HDF_ERR_NOT_SUPPORT, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderFlush_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetFrameSize
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetFrameSize_0001
* @tc.desc  tests the performace of AudioRenderGetFrameSize interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetFrameSize_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    uint64_t zero = 0;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.render->control.Start((AudioHandle)audiopara.render);
    for (auto _ : st) {
        ret = audiopara.render->attr.GetFrameSize(audiopara.render, &audiopara.character.getframesize);
    }
    audiopara.render->control.Stop((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_GT(audiopara.character.getframesize, zero);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetFrameSize_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
* @tc.name  the performace of AudioRenderCheckSceneCapability
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderCheckSceneCapability_0001
* @tc.desc  tests the performace of AudioRenderCheckSceneCapability interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderCheckSceneCapability_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    struct AudioSceneDescriptor scenes = {.scene.id = 0, .desc.pins = PIN_OUT_SPEAKER};
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->scene.CheckSceneCapability(audiopara.render, &scenes, &audiopara.character.supported);
    }
#ifdef AUDIO_HAL_NOTSUPPORT_PATHSELECT
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);
#else
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
#endif
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderCheckSceneCapability_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderSelectScene
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderSelectScene_0001
* @tc.desc  tests the performace of AudioRenderSelectScene interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSelectScene_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    struct AudioSceneDescriptor scenes = {.scene.id = 0, .desc.pins = PIN_OUT_SPEAKER};
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->scene.SelectScene(audiopara.render, &scenes);
    }
#ifdef AUDIO_HAL_NOTSUPPORT_PATHSELECT
    EXPECT_EQ(AUDIO_HAL_ERR_NOT_SUPPORT, ret);
#else
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
#endif
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSelectScene_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderSetMute
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderSetMute_0001
* @tc.desc  tests the performace of AudiorenderSetMute interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderSetMute_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->volume.SetMute(audiopara.render, false);
    }
    ret = audiopara.render->volume.GetMute(audiopara.render, &audiopara.character.getmute);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_FALSE(audiopara.character.getmute);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderSetMute_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderGetMute
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderGetMute_0001
* @tc.desc  tests the performace of AudiorenderGetMute interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetMute_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->volume.SetMute(audiopara.render, false);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->volume.GetMute(audiopara.render, &audiopara.character.getmute);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_FALSE(audiopara.character.getmute);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetMute_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderSetVolume
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderSetVolume_0001
* @tc.desc  tests the performace of AudiorenderSetVolume interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderSetVolume_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .character.setvolume = 0.8, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->volume.SetVolume(audiopara.render, audiopara.character.setvolume);
    }
    ret = audiopara.render->volume.GetVolume(audiopara.render, &audiopara.character.getvolume);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(audiopara.character.setvolume, audiopara.character.getvolume);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderSetVolume_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderGetVolume
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderGetVolume_0001
* @tc.desc  tests the performace of AudiorenderGetVolume interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetVolume_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->volume.GetVolume(audiopara.render, &audiopara.character.getvolume);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetVolume_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderGetGainThreshold
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderGetGainThreshold_0001
* @tc.desc  tests the performace of AudiorenderGetGainThreshold interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetGainThreshold_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->volume.GetGainThreshold(audiopara.render, &audiopara.character.gainthresholdmin,
                &audiopara.character.gainthresholdmax);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetGainThreshold_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderGetGain
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderGetGain_0001
* @tc.desc  tests the performace of AudiorenderGetGain interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetGain_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->volume.GetGain(audiopara.render, &audiopara.character.getgain);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderGetGain_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudiorenderSetGain
* @tc.number  SUB_DriverSystem_Benchmark_AudiorenderSetGain_0001
* @tc.desc  tests the performace of AudiorenderSetGain interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderSetGain_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .character.setgain = 7, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->volume.SetGain(audiopara.render, audiopara.character.setgain);
    }
    ret = audiopara.render->volume.GetGain(audiopara.render, &audiopara.character.getgain);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_EQ(audiopara.character.setgain, audiopara.character.getgain);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudiorenderSetGain_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderFrame
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderFrame_0001
* @tc.desc  tests the performace of AudioRenderFrame interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderFrame_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .path = AUDIO_FILE.c_str(), .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    if (ret < 0) {
        audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
        audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
        audiopara.render = nullptr;
        audiopara.adapter = nullptr;
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    ret = RenderFramePrepare(audiopara.path, audiopara.frame, audiopara.requestBytes);
    if (ret < 0) {
        audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
        audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
        audiopara.render = nullptr;
        audiopara.adapter = nullptr;
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    for (auto _ : st) {
        ret = audiopara.render->RenderFrame(audiopara.render, audiopara.frame, audiopara.requestBytes,
                                            &audiopara.replyBytes);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    if (audiopara.frame != nullptr) {
        free(audiopara.frame);
        audiopara.frame = nullptr;
    }
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderFrame_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderStart
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderStart_0001
* @tc.desc  tests the performace of AudioRenderStart interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderStart_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                                &audiopara.render);
    for (auto _ : st) {
        ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
        audiopara.render->control.Stop((AudioHandle)audiopara.render);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderStart_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderStop
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderStop_0001
* @tc.desc  tests the performace of AudioRenderStop interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderStop_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                                &audiopara.render);
    for (auto _ : st) {
        ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
        ret = audiopara.render->control.Stop((AudioHandle)audiopara.render);
    }
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderStop_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderSetSampleAttributes
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderSetSampleAttributes_0001
* @tc.desc  tests the performace of AudioRenderSetSampleAttributes interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetSampleAttributes_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(audiopara.attrs);
    for (auto _ : st) {
        ret = audiopara.render->attr.SetSampleAttributes(audiopara.render, &audiopara.attrs);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetSampleAttributes_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderPause
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderPause_0001
* @tc.desc  tests the performace of AudioRenderPause interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderPause_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        ret = audiopara.render->control.Pause((AudioHandle)audiopara.render);
        ret = audiopara.render->control.Resume((AudioHandle)audiopara.render);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderPause_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderResume
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderResume_0001
* @tc.desc  tests the performace of AudioRenderResume interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderResume_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    for (auto _ : st) {
        audiopara.render->control.Pause((AudioHandle)audiopara.render);
        ret = audiopara.render->control.Resume((AudioHandle)audiopara.render);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderResume_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetSampleAttributes
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetSampleAttributes_0001
* @tc.desc  tests the performace of AudioRenderGetSampleAttributes interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetSampleAttributes_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    InitAttrs(audiopara.attrs);

    for (auto _ : st) {
        ret = audiopara.render->attr.GetSampleAttributes(audiopara.render, &audiopara.attrs);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetSampleAttributes_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderReqMmapBuffer
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderReqMmapBuffer_0001
* @tc.desc  tests the performace of AudioRenderReqMmapBuffer interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderReqMmapBuffer_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    bool isRender = true;
    int32_t reqSize = 0;
    struct AudioMmapBufferDescripter desc = {};
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER, .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);

    audiopara.render = nullptr;
    FILE *fp = fopen(LOW_LATENCY_AUDIO_FILE.c_str(), "rb+");
    ASSERT_NE(nullptr, fp);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    if (ret < 0 || audiopara.render == nullptr) {
        fclose(fp);
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
        ASSERT_EQ(nullptr, audiopara.render);
    }
    InitAttrs(audiopara.attrs);
    audiopara.attrs.startThreshold = 0;
    ret = audiopara.render->attr.SetSampleAttributes(audiopara.render, &(audiopara.attrs));
    ret = InitMmapDesc(fp, desc, reqSize, isRender);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->control.Start((AudioHandle)audiopara.render);
    for (auto _ : st) {
        ret = audiopara.render->attr.ReqMmapBuffer((AudioHandle)audiopara.render, reqSize, &desc);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    if (ret == 0) {
        munmap(desc.memoryAddress, reqSize);
    }

    audiopara.render->control.Stop((AudioHandle)audiopara.render);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
    fclose(fp);
    usleep(500);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderReqMmapBuffer_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetMmapPosition
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetMmapPosition_0001
* @tc.desc  tests the performace of AudioRenderRenderGetMmapPosition interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetMmapPosition_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    uint64_t framesRendering = 0;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .pins = PIN_OUT_SPEAKER,
        .path = LOW_LATENCY_AUDIO_FILE.c_str(), .totalTime = 0
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateRender(audiopara.manager, audiopara.pins, audiopara.adapterName, &audiopara.adapter,
                            &audiopara.render);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    ret = PlayMapAudioFile(audiopara);
    if (ret != 0) {
        audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
        audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
        audiopara.render = nullptr;
        audiopara.adapter = nullptr;
        ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    }
    for (auto _ : st) {
        ret = audiopara.render->attr.GetMmapPosition(audiopara.render, &framesRendering, &(audiopara.time));
    }

    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    audiopara.adapter->DestroyRender(audiopara.adapter, audiopara.render);
    audiopara.manager->UnloadAdapter(audiopara.manager, audiopara.adapter);
    audiopara.render = nullptr;
    audiopara.adapter = nullptr;
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetMmapPosition_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderSetExtraParams
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderSetExtraParams_0001
* @tc.desc  tests the performace of AudioRenderSetExtraParams interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetExtraParams_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    char keyValueList[] = "attr-route=1;attr-format=32;attr-channels=2;attr-frame-count=82;attr-sampling-rate=48000";
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .adapterName = ADAPTER_NAME.c_str(), .self = this, .pins = PIN_OUT_SPEAKER,
    };
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);
    ret = AudioCreateStartRender(audiopara.manager, &audiopara.render, &audiopara.adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);

    for (auto _ : st) {
        ret = audiopara.render->attr.SetExtraParams((AudioHandle)audiopara.render, keyValueList);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderSetExtraParams_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
/**
* @tc.name  the performace of AudioRenderGetExtraParams
* @tc.number  SUB_DriverSystem_Benchmark_AudioRenderGetExtraParams_0001
* @tc.desc  tests the performace of AudioRenderGetExtraParams interface by executing 100 times,
*           and calculates the delay time and average of Delay Time.
*/
BENCHMARK_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetExtraParams_0001)
    (benchmark::State &st)
{
    int32_t ret = -1;
    struct PrepareAudioPara audiopara = {
        .portType = PORT_OUT, .self = this, .pins = PIN_OUT_SPEAKER, .path = AUDIO_FILE.c_str()
    };
    char keyValueList[] = "attr-format=24;attr-frame-count=4096;";
    char keyValueListExp[] = "attr-route=0;attr-format=24;attr-channels=2;attr-frame-count=4096;\
attr-sampling-rate=48000";
    int32_t listLenth = 256;
    ASSERT_NE(nullptr, GetAudioManager);
    audiopara.manager = GetAudioManager();
    ASSERT_NE(nullptr, audiopara.manager);

    ret = AudioCreateStartRender(audiopara.manager, &audiopara.render, &audiopara.adapter, ADAPTER_NAME);
    ASSERT_EQ(AUDIO_HAL_SUCCESS, ret);
    ret = audiopara.render->attr.SetExtraParams((AudioHandle)audiopara.render, keyValueList);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    char keyValueListValue[256] = {};
    for (auto _ : st) {
        (void)memset_s(&keyValueListValue, sizeof(keyValueListValue), 0, sizeof(keyValueListValue));
        ret = audiopara.render->attr.GetExtraParams((AudioHandle)audiopara.render, keyValueListValue, listLenth);
    }
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
    EXPECT_STREQ(keyValueListExp, keyValueListValue);
    ret = StopAudio(audiopara);
    EXPECT_EQ(AUDIO_HAL_SUCCESS, ret);
}
BENCHMARK_REGISTER_F(AudioHdiRenderBenchmarkTest, SUB_DriverSystem_Benchmark_AudioRenderGetExtraParams_0001)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
}
BENCHMARK_MAIN();