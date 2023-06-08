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
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "v1_0/codec_callback_service.h"
#include "v1_0/icodec_callback.h"
#include "v1_0/icodec_component_manager.h"
using namespace std;
using namespace testing::ext;
using OHOS::sptr;
using namespace OHOS::HDI::Codec::V1_0;
constexpr int64_t APP_DATA = 3;
namespace {
class CodecBenchmarkManagerTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state)
    {
        manager_ = ICodecComponentManager::Get();
        callback_ = new CodecCallbackService();
    }
    void TearDown(const ::benchmark::State &state)
    {
        manager_ = nullptr;
        callback_ = nullptr;
    }

public:
    sptr<ICodecComponentManager> manager_;
    sptr<ICodecCallback> callback_;
};

BENCHMARK_F(CodecBenchmarkManagerTest, GetComponentNum)(benchmark::State &state)
{
    ASSERT_TRUE(manager_ != nullptr);
    int32_t count = 0;
    int32_t ret;
    for (auto _ : state) {
        ret = manager_->GetComponentNum(count);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
    EXPECT_TRUE(count >= 0);
}

BENCHMARK_REGISTER_F(CodecBenchmarkManagerTest, GetComponentNum)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkManagerTest, GetComponentCapabilityList)(benchmark::State &state)
{
    ASSERT_TRUE(manager_ != nullptr);
    int32_t count = 0;
    auto ret = manager_->GetComponentNum(count);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(count > 0);
    
    std::vector<CodecCompCapability> capList;
    for (auto _ : state) {
        ret = manager_->GetComponentCapabilityList(capList, count);
    }
    ASSERT_EQ(ret, HDF_SUCCESS);
}

BENCHMARK_REGISTER_F(CodecBenchmarkManagerTest, GetComponentCapabilityList)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkManagerTest, CreateComponent)(benchmark::State &state)
{
    ASSERT_TRUE(callback_ != nullptr);
    ASSERT_TRUE(manager_ != nullptr);
    sptr<ICodecComponent> component;
    uint32_t componentId = 0;
    int32_t ret;
    for (auto _ : state) {
        ret = manager_->CreateComponent(component, componentId, "", APP_DATA, callback_);
    }
    EXPECT_NE(ret, HDF_SUCCESS);
    ASSERT_EQ(component, nullptr);
}

BENCHMARK_REGISTER_F(CodecBenchmarkManagerTest, CreateComponent)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(CodecBenchmarkManagerTest, DestoryComponent)(benchmark::State &state)
{
    ASSERT_TRUE(manager_ != nullptr);
    std::string compName("");
    int32_t count = 0;
    auto ret = manager_->GetComponentNum(count);
    ASSERT_EQ(ret, HDF_SUCCESS);
    ASSERT_TRUE(count > 0);
   
    std::vector<CodecCompCapability> capList;
    ret = manager_->GetComponentCapabilityList(capList, count);
    ASSERT_EQ(ret, HDF_SUCCESS);

    compName = capList[0].compName;
    ASSERT_FALSE(compName.empty());
    sptr<ICodecComponent> component;
    uint32_t componentId = 0;
    ret = manager_->CreateComponent(component, componentId, compName, APP_DATA, callback_);
    ASSERT_EQ(ret, HDF_SUCCESS);
    for (auto _ : state) {
        if (componentId != 0) {
            manager_->DestroyComponent(componentId);
        }
    }
}

BENCHMARK_REGISTER_F(CodecBenchmarkManagerTest, DestoryComponent)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();
}  // namespace
