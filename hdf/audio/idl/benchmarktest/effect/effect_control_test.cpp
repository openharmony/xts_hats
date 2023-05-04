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
#include <benchmark/benchmark.h>
#include <climits>
#include <gtest/gtest.h>
#include "hdf_base.h"
#include "hdf_log.h"
#include "v1_0/effect_types.h"
#include "v1_0/ieffect_control.h"
#include "v1_0/ieffect_model.h"
#include "include/effect_common.h"
#include "osal_mem.h"

using namespace std;
using namespace testing::ext;
constexpr bool IS_DIRECTLY_CALL = false;
/* the input buffer len of the send command */
constexpr uint32_t SEND_COMMAND_LEN = 10;
/* the output buffer len of the command */
constexpr uint32_t GET_BUFFER_LEN = 10;
# define AUDIO_EFFECT_COMMAND_INVALID_LARGE 20

namespace {
const int32_t ITERATION_FREQUENCY = 100;
const int32_t REPETITION_FREQUENCY = 3;
class EffectControlBenchmarkTest : public benchmark::Fixture {
public:
    struct IEffectControl *controller_ = nullptr;
    struct IEffectModel *model_ = nullptr;
    struct ControllerId contollerId_;
    virtual void SetUp(const ::benchmark::State &state);
    virtual void TearDown(const ::benchmark::State &state);
    char *libName_ = nullptr;
    char *effectId_ = nullptr;
};

void EffectControlBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    // input testcase setup step,setup invoked before each testcases
    libName_ = strdup("libmock_effect_lib");
    effectId_ = strdup("aaaabbbb-8888-9999-6666-aabbccdd9966ff");
    struct EffectInfo info = {
        .libName = libName_,
        .effectId = effectId_,
        .ioDirection = 1,
    };

    model_ = IEffectModelGet(IS_DIRECTLY_CALL);
    ASSERT_NE(model_, nullptr);

    int32_t ret = model_->CreateEffectController(model_, &info, &controller_, &contollerId_);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_NE(controller_, nullptr);
}

void EffectControlBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    // input testcase teardown step,teardown invoked after each testcases
    if (libName_ != nullptr) {
        free(libName_);
        libName_ = nullptr;
    }

    if (effectId_ != nullptr) {
        free(effectId_);
        effectId_ = nullptr;
    }

    if (controller_ != nullptr && model_ != nullptr) {
        int32_t ret = model_->DestroyEffectController(model_, &contollerId_);
        EXPECT_EQ(ret, HDF_SUCCESS);
    }

    if (model_ != nullptr) {
        IEffectModelRelease(model_, IS_DIRECTLY_CALL);
    }
}

/**
 * @tc.name: HdfAudioEffectProcess
 * @tc.desc: Verify the EffectControlEffectProcess function.
 * @tc.type: FUNC
 * @tc.require: I6I658
 */
BENCHMARK_F(EffectControlBenchmarkTest, HdfAudioEffectProcess)(benchmark::State &state)
{
    int32_t ret;
    struct AudioEffectBuffer input = {0};
    struct AudioEffectBuffer output = {0};
    for (auto _ : state) {
        ret = controller_->EffectProcess(controller_, &input, &output);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}
BENCHMARK_REGISTER_F(EffectControlBenchmarkTest, HdfAudioEffectProcess)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();

/**
 * @tc.name: HdfAudioSendCommandInit
 * @tc.desc: Verify the EffectControlEffectProcess function when cmdId is AUDIO_EFFECT_COMMAND_INIT_CONTOLLER.
 * @tc.type: FUNC
 * @tc.require: I6I658
 */
BENCHMARK_F(EffectControlBenchmarkTest, HdfAudioSendCommandInit)(benchmark::State &state)
{
    int32_t ret;
    int8_t input[SEND_COMMAND_LEN] = {0};
    int8_t output[GET_BUFFER_LEN] = {0};
    uint32_t replyLen = GET_BUFFER_LEN;
    for (auto _ : state) {
        ret = controller_->SendCommand(controller_, AUDIO_EFFECT_COMMAND_INIT_CONTOLLER,
                                           input, SEND_COMMAND_LEN, output, &replyLen);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}
BENCHMARK_REGISTER_F(EffectControlBenchmarkTest, HdfAudioSendCommandInit)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();
/**
 * @tc.name: HdfAudioGetDescriptor
 * @tc.desc: Verify the EffectGetOwnDescriptor function.
 * @tc.type: FUNC
 * @tc.require: I6I658
 */
BENCHMARK_F(EffectControlBenchmarkTest, HdfAudioGetDescriptor)(benchmark::State &state)
{
    int32_t ret;
    struct EffectControllerDescriptor desc;
    for (auto _ : state) {
        ret = controller_->GetEffectDescriptor(controller_, &desc);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
    EffectControllerReleaseDesc(&desc);
}
BENCHMARK_REGISTER_F(EffectControlBenchmarkTest, HdfAudioGetDescriptor)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();
} // end of namespace
