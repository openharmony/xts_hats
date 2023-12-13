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

#include <mutex>
#include <chrono>
#include <cinttypes>
#include <algorithm>
#include <condition_variable>
#include <benchmark/benchmark.h>
#include "gtest/gtest.h"
#include "v1_1/include/idisplay_composer_interface.h"
#include "v1_1/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "display_test.h"
#include "display_test_utils.h"
#include "hdi_composition_check.h"
#include "hdi_test_device.h"
#include "hdi_test_device_common.h"
#include "hdi_test_display.h"
#include "hdi_test_render_utils.h"

using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_1;
using namespace OHOS::HDI::Display::TEST;
using namespace testing::ext;
using namespace std;

namespace {
class VblankCtr {
public:
    static VblankCtr& GetInstance()
    {
        static VblankCtr instance;
        return instance;
    }
    void NotifyVblank(unsigned int sequence, uint64_t ns, const void* data);
    int32_t WaitVblank(uint32_t ms);

protected:
    void TearDown();

private:
    std::mutex vblankMutex_;
    std::condition_variable vblankCondition_;
    VblankCtr() {}
    ~VblankCtr();
    bool hasVblank_ = false;
};

static sptr<Composer::V1_1::IDisplayComposerInterface> g_composerDevice = nullptr;
static std::shared_ptr<IDisplayBuffer> g_gralloc = nullptr;
static std::vector<uint32_t> g_displayIds;
const int SLEEP_CONT_100 = 100;
const int SLEEP_CONT_1000 = 1000;

class DisplayBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

static void TestVBlankCallback(unsigned int sequence, uint64_t ns, void* data)
{
    static uint64_t lastns;
    DISPLAY_TEST_LOGE("seq %{public}d  ns %" PRId64 " duration %" PRId64 " ns", sequence, ns, (ns - lastns));
    lastns = ns;
    VblankCtr::GetInstance().NotifyVblank(sequence, ns, data);
}

void VblankCtr::NotifyVblank(unsigned int sequence, uint64_t ns, const void* data)
{
    DISPLAY_TEST_LOGE();
    if (data != nullptr) {
        DISPLAY_TEST_LOGE("sequence = %{public}u, ns = %" PRIu64 "", sequence, ns);
    }
    std::unique_lock<std::mutex> lg(vblankMutex_);
    hasVblank_ = true;
    vblankCondition_.notify_one();
    DISPLAY_TEST_LOGE();
}

VblankCtr::~VblankCtr() {}

int32_t VblankCtr::WaitVblank(uint32_t ms)
{
    bool ret = false;
    DISPLAY_TEST_LOGE();
    std::unique_lock<std::mutex> lck(vblankMutex_);
    hasVblank_ = false; // must wait next vblank
    ret = vblankCondition_.wait_for(lck, std::chrono::milliseconds(ms), [=] { return hasVblank_; });
    DISPLAY_TEST_LOGE();
    if (!ret) {
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

void DisplayBenchmarkTest::SetUp(const ::benchmark::State &state)
{
}

void DisplayBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    auto display = HdiTestDevice::GetInstance().GetFirstDisplay();
    int32_t ret = display->SetDisplayVsyncEnabled(false);
    if (ret != DISPLAY_SUCCESS) {
        DISPLAY_TEST_LOGE("vsync disable failed");
    }
    VblankCtr::GetInstance().WaitVblank(SLEEP_CONT_100); // wait for last vsync 100ms.
    HdiTestDevice::GetInstance().Clear();
}

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3700)(benchmark::State &state)
{
    int ret;
    DISPLAY_TEST_LOGE();
    std::shared_ptr<HdiTestDisplay> display = HdiTestDevice::GetInstance().GetFirstDisplay();
    for (auto _ : state) {
        ret = display->RegDisplayVBlankCallback(TestVBlankCallback, nullptr);
    }
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "RegDisplayVBlankCallback failed";
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3700)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();


BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3800)(benchmark::State &state)
{
    int ret;
    DISPLAY_TEST_LOGE();
    std::shared_ptr<HdiTestDisplay> display = HdiTestDevice::GetInstance().GetFirstDisplay();
    ret = display->RegDisplayVBlankCallback(TestVBlankCallback, nullptr);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "RegDisplayVBlankCallback failed";
    for (auto _ : state) {
        ret = display->SetDisplayVsyncEnabled(true);
        ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "SetDisplayVsyncEnabled failed";
        ret = VblankCtr::GetInstance().WaitVblank(SLEEP_CONT_1000); // 1000ms
        ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "WaitVblank timeout";
        ret = display->SetDisplayVsyncEnabled(false);
        ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "SetDisplayVsyncEnabled failed";
    }
    usleep(SLEEP_CONT_100 * SLEEP_CONT_1000);                              // wait for 100ms avoid the last vsync.
    ret = VblankCtr::GetInstance().WaitVblank(SLEEP_CONT_1000); // 1000ms
    ASSERT_TRUE(ret != DISPLAY_SUCCESS) << "vblank do not disable";
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3800)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

} // namespace