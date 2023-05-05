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
constexpr uint32_t MAX_DESCRIPTOR_NUM = 20;

namespace {
const int32_t ITERATION_FREQUENCY = 100;
const int32_t REPETITION_FREQUENCY = 3;
class EffectModelBenchmarkTest : public benchmark::Fixture {
public:
    struct IEffectModel *model_ = nullptr;
    struct ControllerId contollerId_;
    char *libName_ = nullptr;
    char *effectId_ = nullptr;
    virtual void SetUp(const ::benchmark::State &state);
    virtual void TearDown(const ::benchmark::State &state);
};

void EffectModelBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    // input testcase setup step,setup invoked before each testcases
    libName_ = strdup("libmock_effect_lib");
    effectId_ = strdup("aaaabbbb-8888-9999-6666-aabbccdd9966ff");
    model_ = IEffectModelGet(IS_DIRECTLY_CALL);
    ASSERT_NE(nullptr, model_);
}

void EffectModelBenchmarkTest::TearDown(const ::benchmark::State &state)
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

    if (model_ != nullptr) {
        IEffectModelRelease(model_, IS_DIRECTLY_CALL);
    }
}

/**
 * @tc.name: HdfAudioIsSupplyEffectLibs
 * @tc.desc: Verify the EffectModelIsSupplyEffectLibs function.
 */
BENCHMARK_F(EffectModelBenchmarkTest, HdfAudioIsSupplyEffectLibs)(benchmark::State &state)
{
    int32_t ret;
    bool isSupport = false;
    for (auto _ : state) {
        ret = model_->IsSupplyEffectLibs(model_, &isSupport);
    }
    EXPECT_EQ(ret, HDF_SUCCESS);
}
BENCHMARK_REGISTER_F(EffectModelBenchmarkTest, HdfAudioIsSupplyEffectLibs)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();

/**
 * @tc.name: HdfAudioGetAllEffectDescriptors
 * @tc.desc: Verify the EffectModelGetAllEffectDescriptors function.
 */
BENCHMARK_F(EffectModelBenchmarkTest, HdfAudioGetAllEffectDescriptors)(benchmark::State &state)
{
    int32_t ret;
    uint32_t descsLen = MAX_DESCRIPTOR_NUM;
    struct EffectControllerDescriptor descs[MAX_DESCRIPTOR_NUM];
    for (auto _ : state) {
        ret = model_->GetAllEffectDescriptors(model_, descs, &descsLen);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
    EXPECT_GE(MAX_DESCRIPTOR_NUM, descsLen);
}
BENCHMARK_REGISTER_F(EffectModelBenchmarkTest, HdfAudioGetAllEffectDescriptors)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();
/**
 * @tc.name: HdfAudioCreateDestroyController
 * @tc.desc: Verify the EffectModelCreateEffectController and EffectModelDestroyEffectController function.
 */
BENCHMARK_F(EffectModelBenchmarkTest, HdfAudioCreateDestroyController)(benchmark::State &state)
{
    int32_t ret;
    struct EffectInfo info = {
        .libName = libName_,
        .effectId = effectId_,
        .ioDirection = 1,
    };
    for (auto _ : state) {
        struct IEffectControl *contoller = NULL;
        ret = model_->CreateEffectController(model_, &info, &contoller, &contollerId_);
        if (ret == HDF_SUCCESS) {
            ASSERT_NE(contoller, nullptr);
        }

        if (contoller != nullptr) {
            ret = model_->DestroyEffectController(model_, &contollerId_);
        }
    }
}
BENCHMARK_REGISTER_F(EffectModelBenchmarkTest, HdfAudioCreateDestroyController)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();

/**
 * @tc.name: HdfAudioGetEffectDescriptor
 * @tc.desc: Verify the EffectModelGetEffectDescriptor function.
 */
BENCHMARK_F(EffectModelBenchmarkTest, HdfAudioGetEffectDescriptor)(benchmark::State &state)
{
    int32_t ret;
    struct EffectControllerDescriptor desc;
    for (auto _ : state) {
        ret = model_->GetEffectDescriptor(model_, effectId_, &desc);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
    EffectControllerReleaseDesc(&desc);
}
BENCHMARK_REGISTER_F(EffectModelBenchmarkTest, HdfAudioCreateDestroyController)->
    Iterations(ITERATION_FREQUENCY)->Repetitions(REPETITION_FREQUENCY)->ReportAggregatesOnly();
} // end of namespace
BENCHMARK_MAIN();