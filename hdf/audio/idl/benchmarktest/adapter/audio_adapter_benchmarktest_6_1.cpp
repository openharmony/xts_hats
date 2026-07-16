/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#include <benchmark/benchmark.h>
#include <climits>
#include <gtest/gtest.h>
#include "hdf_base.h"
#include "osal_mem.h"
#include "v6_1/iaudio_manager.h"

using namespace testing::ext;
using namespace std;

#define AUDIO_CHANNELCOUNT             2
#define AUDIO_SAMPLE_RATE_48K          48000
#define DEEP_BUFFER_RENDER_PERIOD_SIZE 4096
#define INT_32_MAX                     0x7fffffff
#define PCM_16_BIT                     16
#define PCM_8_BIT                      8

namespace {
constexpr uint32_t g_audioAdapterNumMax = 5;
[[maybe_unused]]constexpr int32_t AUDIO_ADAPTER_BUF_TEST = 1024;
const int32_t ITERATION_FREQUENCY = 100;
const int32_t REPETITION_FREQUENCY = 3;

class AudioAdapterBenchmarkTest_6_1 : public benchmark::Fixture {
public:
    struct IAudioManager *manager_ = nullptr;
    struct IAudioAdapter *adapter_ = nullptr;
    struct AudioAdapterDescriptor *adapterDescs_ = nullptr;
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
    void AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf);
    void ReleaseAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen);
};

void AudioAdapterBenchmarkTest_6_1::AudioAdapterDescriptorFree(struct AudioAdapterDescriptor *dataBlock, bool freeSelf)
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

void AudioAdapterBenchmarkTest_6_1::ReleaseAdapterDescs(struct AudioAdapterDescriptor *descs, uint32_t descsLen)
{
    if ((descs == nullptr) || (descsLen == 0)) {
        return;
    }

    for (uint32_t i = 0; i < descsLen; i++) {
        AudioAdapterDescriptorFree(&descs[i], false);
    }
}

void AudioAdapterBenchmarkTest_6_1::SetUp(const ::benchmark::State &state)
{
    uint32_t size = g_audioAdapterNumMax;
    manager_ = IAudioManagerGetV6_1(false);
    if (manager_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }

    adapterDescs_ = (struct AudioAdapterDescriptor *)OsalMemCalloc(
        sizeof(struct AudioAdapterDescriptor) * (g_audioAdapterNumMax));
    ASSERT_NE(adapterDescs_, nullptr);

    ASSERT_EQ(HDF_SUCCESS, manager_->GetAllAdapters(manager_, adapterDescs_, &size));
    if (size > g_audioAdapterNumMax) {
        ReleaseAdapterDescs(adapterDescs_, g_audioAdapterNumMax);
        ASSERT_LT(size, g_audioAdapterNumMax);
    }

    if (manager_->LoadAdapter(manager_, &adapterDescs_[0], &adapter_) != HDF_SUCCESS) {
        ReleaseAdapterDescs(adapterDescs_, g_audioAdapterNumMax);
        ASSERT_TRUE(false);
    }

    if (adapter_ == nullptr) {
        ReleaseAdapterDescs(adapterDescs_, g_audioAdapterNumMax);
        ASSERT_TRUE(false);
    }
}

void AudioAdapterBenchmarkTest_6_1::TearDown(const ::benchmark::State &state)
{
    if (manager_ == nullptr) {
        GTEST_SKIP()<< "Audio HDI 6.1 not support" << std::endl;
    }

    manager_->UnloadAdapter(manager_, adapterDescs_[0].adapterName);
    ReleaseAdapterDescs(adapterDescs_, g_audioAdapterNumMax);
    adapter_ = nullptr;
    IAudioManagerReleaseV6_1(manager_, false);
    manager_ = nullptr;
}

BENCHMARK_F(AudioAdapterBenchmarkTest_6_1, CreateCallTransfer)(benchmark::State &state)
{
    if (adapter_ == nullptr) {
        state.SkipWithMessage("Current HDI not support CreateCallTransfer");
    }
    int32_t ret;

    for (auto _ : state) {
        ret = adapter_->CreateCallTransfer(adapter_);
        ASSERT_TRUE(ret == HDF_ERR_INVALID_PARAM || ret == HDF_ERR_NOT_SUPPORT);
    }
}

BENCHMARK_REGISTER_F(AudioAdapterBenchmarkTest_6_1, CreateCallTransfer)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();

BENCHMARK_F(AudioAdapterBenchmarkTest_6_1, SetPhoneCallScene)(benchmark::State &state)
{
    if (adapter_ == nullptr) {
        state.SkipWithMessage("Current HDI not support CreateCallTransfer");
    }
    int32_t ret;

    for (auto _ : state) {
        ret = adapter_->SetPhoneCallScene(adapter_, SCENE_TYPE_TRANSFER);
        ASSERT_TRUE(ret == HDF_SUCCESS || ret == HDF_ERR_NOT_SUPPORT);
    }
}

BENCHMARK_REGISTER_F(AudioAdapterBenchmarkTest_6_1, SetPhoneCallScene)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();
}
