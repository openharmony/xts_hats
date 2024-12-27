/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include <string>
#include <vector>
#include <mutex>
#include "../unittest/hdi_composer_ut.h"
#include <chrono>
#include <cinttypes>
#include <algorithm>
#include <condition_variable>
#include <benchmark/benchmark.h>
#include "gtest/gtest.h"
#include "v1_2/include/idisplay_composer_interface.h"
#include "v1_1/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "display_test.h"
#include "display_test_utils.h"
#include "hdi_composition_check.h"
#include "hdi_test_device.h"
#include "hdi_test_device_common.h"
#include "hdi_test_display.h"
#include "hdi_test_render_utils.h"
#include "timer.h"
#include <sys/time.h>
#include <thread>

using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_2;
using namespace OHOS::HDI::Display::TEST;
using namespace testing::ext;

namespace {
static sptr<Composer::V1_2::IDisplayComposerInterface> g_composerDevice = nullptr;
static std::shared_ptr<IDisplayBuffer> g_gralloc = nullptr;
static std::vector<uint32_t> g_displayIds;
const int SLEEP_CONT_100 = 100;


class DisplayBenchmarkTest : public benchmark::Fixture {
public:
    void TearDown(const ::benchmark::State &state);
    static void OnMode(uint32_t modeId, uint64_t vBlankPeriod, void* data);
    static void OnseamlessChange(uint32_t devId, void* data);
    static void TestRefreshCallback(uint32_t devId, void* data);
};

static void DestroyLayer(std::shared_ptr<HdiTestLayer> layer)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_CONT_100));
    auto ret = g_composerDevice->DestroyLayer(g_displayIds[0], layer->GetId());
    if (ret != DISPLAY_SUCCESS && ret != DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("DestroyLayer fail or not support, ret: %{public}d", ret);
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_CONT_100));
}

void DisplayBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    HdiTestDevice::GetInstance().Clear();
    HdiTestDevice::GetInstance().GetFirstDisplay()->ResetClientLayer();
}

void DisplayBenchmarkTest::OnMode(uint32_t modeId, uint64_t vBlankPeriod, void* data)
{
}

void DisplayBenchmarkTest::OnseamlessChange(uint32_t devId, void* data)
{
}

void DisplayBenchmarkTest::TestRefreshCallback(uint32_t devId, void* data)
{
}

static inline std::shared_ptr<HdiTestDisplay> GetFirstDisplay()
{
    return HdiTestDevice::GetInstance().GetFirstDisplay();
}

static int32_t CheckComposition(std::vector<LayerSettings> &layers, BufferHandle* clientBuffer,
    uint32_t checkType = HdiCompositionCheck::CHECK_VERTEX)
{
    DISPLAY_TEST_CHK_RETURN((clientBuffer == nullptr), DISPLAY_NULL_PTR, DISPLAY_TEST_LOGE("client buffer is nullptr"));
    return HdiCompositionCheck::GetInstance().Check(layers, *clientBuffer, checkType);
}

static std::shared_ptr<HdiTestLayer> CreateTestLayer(LayerSettings setting, uint32_t zorder)
{
    int ret;
    HdiTestDevice::GetInstance();
    DISPLAY_TEST_LOGE("color 0x%x", setting.color);
    std::shared_ptr<HdiTestDisplay> display = HdiTestDevice::GetInstance().GetFirstDisplay();
    DISPLAY_TEST_CHK_RETURN((display == nullptr), nullptr, DISPLAY_TEST_LOGE("can not get display"));

    std::shared_ptr<HdiTestLayer> layer = display->CreateHdiTestLayer(setting.bufferSize.w, setting.bufferSize.h);
    DISPLAY_TEST_CHK_RETURN((layer == nullptr), nullptr, DISPLAY_TEST_LOGE("can not create hdi test layer"));

    layer->SetLayerPosition(setting.displayRect);

    layer->SetCompType(setting.compositionType);

    if ((setting.alpha >= 0) && (setting.alpha <= 0xff)) { // alpha rang 0x00 ~ 0xff
        LayerAlpha alpha = { 0 };
        alpha.gAlpha = setting.alpha;
        alpha.enGlobalAlpha = true;
        layer->SetAlpha(alpha);
    }
    HdiGrallocBuffer* handle = layer->GetFrontBuffer();
    DISPLAY_TEST_CHK_RETURN((handle == nullptr), nullptr, DISPLAY_TEST_LOGE("can not get front buffer"));
    ClearColor(*(handle->Get()), setting.color);
    ret = layer->SwapFrontToBackQ();
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), nullptr, DISPLAY_TEST_LOGE("SwapFrontToBackQ failed"));
    layer->SetZorder(zorder);
    layer->SetBlendType(setting.blendType);
    layer->SetTransform(setting.rotate);
    return layer;
}

static int PrepareAndCommit()
{
    int ret;
    DISPLAY_TEST_LOGE();
    std::shared_ptr<HdiTestDisplay> display = HdiTestDevice::GetInstance().GetFirstDisplay();
    DISPLAY_TEST_CHK_RETURN((display == nullptr), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("can not get display"));

    ret = display->PrepareDisplayLayers();
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("PrepareDisplayLayers failed"));

    ret = display->Commit();
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("Commit failed"));
    return DISPLAY_SUCCESS;
}

static void AdjustLayerSettings(std::vector<LayerSettings> &settings, uint32_t w, uint32_t h)
{
    DISPLAY_TEST_LOGE();
    for (uint32_t i = 0; i < settings.size(); i++) {
        LayerSettings& setting = settings[i];
        DISPLAY_TEST_LOGE(" ratio w: %f  ratio h: %f", setting.rectRatio.w, setting.rectRatio.h);
        if ((setting.rectRatio.w > 0.0f) && (setting.rectRatio.h > 0.0f)) {
            setting.displayRect.h = static_cast<uint32_t>(setting.rectRatio.h * h);
            setting.displayRect.w = static_cast<uint32_t>(setting.rectRatio.w * w);
            setting.displayRect.x = static_cast<uint32_t>(setting.rectRatio.x * w);
            setting.displayRect.y = static_cast<uint32_t>(setting.rectRatio.y * h);
            DISPLAY_TEST_LOGE("display rect adust form %f %f %f %f to %{public}d %{public}d %{public}d %{public}d ",
                setting.rectRatio.x, setting.rectRatio.y, setting.rectRatio.w,
                setting.rectRatio.h, setting.displayRect.x, setting.displayRect.y,
                setting.displayRect.w, setting.displayRect.h);
        }

        if ((setting.bufferRatio.h > 0.0f) || (setting.bufferRatio.w > 0.0f)) {
            setting.bufferSize.h = static_cast<uint32_t>(setting.bufferRatio.h * h);
            setting.bufferSize.w = static_cast<uint32_t>(setting.bufferRatio.w * w);
            DISPLAY_TEST_LOGE("buffer size adjust for %f %f to %{public}d %{public}d",
                setting.bufferRatio.w, setting.bufferRatio.h, setting.bufferSize.w, setting.bufferSize.h);
        }

        if ((setting.bufferSize.w == 0) || (setting.bufferSize.h == 0)) {
            DISPLAY_TEST_LOGE("buffer size adjust for %{public}d %{public}d to %{public}d %{public}d",
                setting.bufferSize.w, setting.bufferSize.h, setting.displayRect.w, setting.displayRect.h);

            setting.bufferSize.w = setting.displayRect.w;
            setting.bufferSize.h = setting.displayRect.h;
        }
    }
}

static std::vector<std::shared_ptr<HdiTestLayer>> CreateLayers(std::vector<LayerSettings> &settings)
{
    DISPLAY_TEST_LOGE("settings %{public}zd", settings.size());
    std::vector<std::shared_ptr<HdiTestLayer>> layers;
    DisplayModeInfo mode = GetFirstDisplay()->GetCurrentMode();
    AdjustLayerSettings(settings, mode.width, mode.height);
    for (uint32_t i = 0; i < settings.size(); i++) {
        LayerSettings setting = settings[i];

        auto layer = CreateTestLayer(setting, i);
        layers.push_back(layer);
    }
    return layers;
}

static inline void PresentAndCheck(std::vector<LayerSettings> &layerSettings,
    uint32_t checkType = HdiCompositionCheck::CHECK_VERTEX)
{
    int ret = PrepareAndCommit();
    ASSERT_TRUE((ret == DISPLAY_SUCCESS));
    if ((GetFirstDisplay()->SnapShot()) != nullptr) {
        HdiTestDevice::GetInstance().GetGrallocInterface()->InvalidateCache(*(GetFirstDisplay()->SnapShot()));
        ret = CheckComposition(layerSettings, GetFirstDisplay()->SnapShot(), checkType);
        ASSERT_TRUE((ret == DISPLAY_SUCCESS));
    }
}

/**
  * @tc.name: SetClientBufferCacheCountTest
  * @tc.desc: Benchmarktest for interface SetClientBufferCacheCount.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0100)(benchmark::State &state)
{
    int32_t ret;
    const uint32_t CACHE_COUNT = 5;
    for (auto _ : state) {
        ret = g_composerDevice->SetClientBufferCacheCount(g_displayIds[0], CACHE_COUNT);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayCapabilityTest
  * @tc.desc: Benchmarktest for interface GetDisplayCapability.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0200)(benchmark::State &state)
{
    int32_t ret;
    DisplayCapability info;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayCapability(g_displayIds[0], info);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplaySupportedModesTest
  * @tc.desc: Benchmarktest for interface GetDisplaySupportedModes.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0300)(benchmark::State &state)
{
    int32_t ret;
    std::vector<DisplayModeInfo> modes;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplaySupportedModes(g_displayIds[0], modes);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayModeTest
  * @tc.desc: Benchmarktest for interface GetDisplayMode.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0400)(benchmark::State &state)
{
    int32_t ret;
    uint32_t modeId = 0;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayMode(modeId, modeId);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0400)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayModeTest
  * @tc.desc: Benchmarktest for interface SetDisplayMode.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0500)(benchmark::State &state)
{
    int32_t ret;
    const uint32_t modeId = 0;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayMode(g_displayIds[0], modeId);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayPowerStatusTest
  * @tc.desc: Benchmarktest for interface GetDisplayPowerStatus.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0600)(benchmark::State &state)
{
    int32_t ret;
    Composer::V1_0::DispPowerStatus powerStatus = Composer::V1_0::DispPowerStatus::POWER_STATUS_OFF;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayPowerStatus(g_displayIds[0], powerStatus);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0600)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayPowerStatusTest
  * @tc.desc: Benchmarktest for interface SetDisplayPowerStatus.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0700)(benchmark::State &state)
{
    int32_t ret;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0],
            Composer::V1_0::DispPowerStatus::POWER_STATUS_ON);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0700)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayBacklightTest
  * @tc.desc: Benchmarktest for interface GetDisplayBacklight.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0800)(benchmark::State &state)
{
    int32_t ret;
    uint32_t level;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayBacklight(g_displayIds[0], level);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0800)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayBacklightTest
  * @tc.desc: Benchmarktest for interface SetDisplayBacklight.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0900)(benchmark::State &state)
{
    int32_t ret;
    const uint32_t level = 10;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], level);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_0900)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: CreateAndDestroyLayerTest
  * @tc.desc: Benchmarktest for interface CreateLayer And DestroyLayer.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1000)(benchmark::State &state)
{
    int32_t ret;
    LayerInfo layerInfo;
    uint32_t layerId;
    for (auto _ : state) {
        uint32_t bufferCount = 3;
        ret = g_composerDevice->CreateLayer(g_displayIds[0], layerInfo, bufferCount, layerId);
        EXPECT_EQ(DISPLAY_SUCCESS, ret);
        ret = g_composerDevice->DestroyLayer(g_displayIds[0], layerId);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1000)->
    Iterations(10)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayCompChangeTest
  * @tc.desc: Benchmarktest for interface GetDisplayCompChange.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1100)(benchmark::State &state)
{
    std::vector<uint32_t> layers {};
    std::vector<int32_t> type {};
    int32_t ret;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayCompChange(g_displayIds[0], layers, type);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayReleaseFenceTest
  * @tc.desc: Benchmarktest for interface GetDisplayReleaseFence.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1200)(benchmark::State &state)
{
    int32_t ret;
    std::vector<uint32_t> layers {};
    std::vector<int32_t> fences {};
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayReleaseFence(g_displayIds[0], layers, fences);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1300)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    AllocInfo info;
        info.width  = 800;
        info.height = 600;
        info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
                OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
                OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    for (auto _ : state) {
        g_gralloc->AllocMem(info, buffer);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1400)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    AllocInfo info;
    info.width  = 100;
    info.height = 100;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;
    g_gralloc->AllocMem(info, buffer);
    for (auto _ : state) {
        g_gralloc->Mmap(*buffer);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1400)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1500)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    AllocInfo info;
    info.width  = 100;
    info.height = 100;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;
    g_gralloc->AllocMem(info, buffer);
    for (auto _ : state) {
        g_gralloc->InvalidateCache(*buffer);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1600)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    AllocInfo info;
    info.width  = 100;
    info.height = 100;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;
    g_gralloc->AllocMem(info, buffer);
    for (auto _ : state) {
        g_gralloc->FlushCache(*buffer);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1600)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1700)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    AllocInfo info;
    info.width  = 100;
    info.height = 100;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;
    g_gralloc->AllocMem(info, buffer);
    for (auto _ : state) {
        g_gralloc->Unmap(*buffer);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1700)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: CreateAndDestroyVirtualDisplayTest
  * @tc.desc: Benchmarktest for interface CreateVirtualDisplay and DestroyVirtualDisplay.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1800)(benchmark::State &state)
{
    int32_t ret;
    const uint32_t WIDTH = 100;
    const uint32_t HEIGHT = 100;
    int32_t format = 0;
    for (auto _ : state) {
        ret = g_composerDevice->CreateVirtualDisplay(WIDTH, HEIGHT, format, g_displayIds[0]);
        EXPECT_EQ(DISPLAY_FAILURE, ret);
        ret = g_composerDevice->DestroyVirtualDisplay(g_displayIds[0]);
    }
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1800)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetVirtualDisplayBufferTest
  * @tc.desc: Benchmarktest for interface SetVirtualDisplayBuffer.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1900)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    int32_t ret;
    int32_t fence = -1;

    AllocInfo info;
    info.width  = 100;
    info.height = 100;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;

    g_gralloc->AllocMem(info, buffer);
    ASSERT_TRUE(buffer != nullptr);

    for (auto _ : state) {
        ret = g_composerDevice->SetVirtualDisplayBuffer(g_displayIds[0], *buffer, fence);
    }
    g_gralloc->FreeMem(*buffer);
    // not support
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_1900)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayPropertyTest
  * @tc.desc: Benchmarktest for interface SetDisplayProperty.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2000)(benchmark::State &state)
{
    int32_t ret;
    uint32_t id = 1;
    uint64_t value = 0;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayProperty(g_displayIds[0], id, value);
    }
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2000)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayPropertyTest
  * @tc.desc: Benchmarktest for interface GetDisplayProperty.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2100)(benchmark::State &state)
{
    int32_t ret;
    uint32_t id = 1;
    uint64_t value = 0;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayProperty(g_displayIds[0], id, value);
    }
#ifdef DISPLAY_COMMUNITY
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);
#else
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetDisplayProperty not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
#endif
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.name: GetDisplaySupportedModesExtTest
  * @tc.desc: Benchmarktest for interface GetDisplaySupportedModesExtTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplaySupportedModesExtTest)(benchmark::State &state)
{
    int32_t ret;
    std::vector<DisplayModeInfoExt> modes;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplaySupportedModesExt(g_displayIds[0], modes);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplaySupportedModesExtTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayModeAsyncTest
  * @tc.desc: Benchmarktest for interface SetDisplayModeAsyncTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, SetDisplayModeAsyncTest)(benchmark::State &state)
{
    int32_t ret;
    uint32_t modeid = 0;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayModeAsync(g_displayIds[0], modeid, OnMode);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetDisplayModeAsyncTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayVBlankPeriodTest
  * @tc.desc: Benchmarktest for interface GetDisplayVBlankPeriodTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayVBlankPeriodTest)(benchmark::State &state)
{
    int32_t ret;
    uint64_t period = 0;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayVBlankPeriod(g_displayIds[0], period);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayVBlankPeriodTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: RegSeamlessChangeCallbackTest
  * @tc.desc: Benchmarktest for interface RegSeamlessChangeCallbackTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, RegSeamlessChangeCallbackTest)(benchmark::State &state)
{
    int32_t ret;
    for (auto _ : state) {
        ret = g_composerDevice->RegSeamlessChangeCallback(OnseamlessChange, nullptr);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, RegSeamlessChangeCallbackTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetLayerPerFrameParameterTest
  * @tc.desc: Benchmarktest for interface SetLayerPerFrameParameter.
  */
BENCHMARK_F(DisplayBenchmarkTest, SetLayerPerFrameParameterTest)(benchmark::State &state)
{
    int32_t ret;
    LayerInfo layerInfo;
    uint32_t layerId;
    std::string key = "FilmFilter";
    std::vector<int8_t> value = { 1 };
    uint32_t bufferCount = 3;
    ret = g_composerDevice->CreateLayer(g_displayIds[0], layerInfo, bufferCount, layerId);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerPerFrameParameter(g_displayIds[0], layerId, key, value);
    }
    g_composerDevice->DestroyLayer(g_displayIds[0], layerId);
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerPerFrameParameterTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetSupportedLayerPerFrameParameterKeyTest
  * @tc.desc: Benchmarktest for interface GetSupportedLayerPerFrameParameterKey.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetSupportedLayerPerFrameParameterKeyTest)(benchmark::State &state)
{
    int32_t ret;
    std::vector<std::string> keys;
    for (auto _ : state) {
        ret = g_composerDevice->GetSupportedLayerPerFrameParameterKey(keys);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetSupportedLayerPerFrameParameterKeyTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayOverlayResolutionTest
  * @tc.desc: Benchmarktest for interface SetDisplayOverlayResolution.
  */
BENCHMARK_F(DisplayBenchmarkTest, SetDisplayOverlayResolutionTest)(benchmark::State &state)
{
    int32_t ret;
    DisplayModeInfo mode = HdiTestDevice::GetInstance().GetFirstDisplay()->GetCurrentMode();
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayOverlayResolution(g_displayIds[0], mode.width, mode.height);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetDisplayOverlayResolutionTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: RegRefreshCallbackTest
  * @tc.desc: Benchmarktest for interface RegRefreshCallback.
  */
BENCHMARK_F(DisplayBenchmarkTest, RegRefreshCallbackTest)(benchmark::State &state)
{
    int32_t ret;
    for (auto _ : state) {
        ret = g_composerDevice->RegRefreshCallback(TestRefreshCallback, nullptr);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, RegRefreshCallbackTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplaySupportedColorGamutsTest
  * @tc.desc: Benchmarktest for interface GetDisplaySupportedColorGamuts.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplaySupportedColorGamutsTest)(benchmark::State &state)
{
    int32_t ret;
    std::vector<ColorGamut> gamuts;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplaySupportedColorGamuts(g_displayIds[0], gamuts);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplaySupportedColorGamutsTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetHDRCapabilityInfosTest
  * @tc.desc: Benchmarktest for interface GetHDRCapabilityInfos.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetHDRCapabilityInfosTest)(benchmark::State &state)
{
    int32_t ret;
    HDRCapability info = { 0 };
    for (auto _ : state) {
        ret = g_composerDevice->GetHDRCapabilityInfos(g_displayIds[0], info);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetHDRCapabilityInfosTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayClientCropTest
  * @tc.desc: Benchmarktest for interface SetDisplayClientCrop.
  */
BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2200)(benchmark::State &state)
{
    int32_t ret;
    int32_t width = 100;
    int32_t height = 100;
    IRect rect = {0, 0, width, height};
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayClientCrop(g_displayIds[0], rect);
    }
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: UpdateHardwareCursorTest
  * @tc.desc: Benchmarktest for interface UpdateHardwareCursorTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, UpdateHardwareCursorTest)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;

    AllocInfo info;
    info.width  = 512;
    info.height = 512;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_HW_COMPOSER;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;

    g_gralloc->AllocMem(info, buffer);
    ASSERT_TRUE(buffer != nullptr);

    int32_t ret = 0;
    int32_t x = 1;
    int32_t y = 1;
    for (auto _ : state) {
        ret = g_composerDevice->UpdateHardwareCursor(g_displayIds[0], x, y, buffer);
    }
    g_gralloc->FreeMem(*buffer);
#ifdef DISPLAY_COMMUNITY
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);
#else
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("UpdateHardwareCursor not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
#endif
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, UpdateHardwareCursorTest)->
    Iterations(30)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: EnableHardwareCursorStatsTest
  * @tc.desc: Benchmarktest for interface EnableHardwareCursorStatsTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, EnableHardwareCursorStatsTest)(benchmark::State &state)
{
    int32_t ret = 0;
    bool enable = true;
    for (auto _ : state) {
        ret = g_composerDevice->EnableHardwareCursorStats(g_displayIds[0], enable);
    }
#ifdef DISPLAY_COMMUNITY
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);
#else
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("EnableHardwareCursorStats not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
#endif
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, EnableHardwareCursorStatsTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetHardwareCursorStatsTest
  * @tc.desc: Benchmarktest for interface GetHardwareCursorStatsTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetHardwareCursorStatsTest)(benchmark::State &state)
{
    int32_t ret = 0;
    uint32_t frameCount = 0;
    uint32_t vsyncCount = 0;
    for (auto _ : state) {
        ret = g_composerDevice->GetHardwareCursorStats(g_displayIds[0], frameCount, vsyncCount);
    }
#ifdef DISPLAY_COMMUNITY
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);
#else
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetHardwareCursorStats not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
#endif
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetHardwareCursorStatsTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: ClearClientBufferTest
  * @tc.desc: Benchmarktest for interface ClearClientBufferTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, ClearClientBufferTest)(benchmark::State &state)
{
    int32_t ret = 0;
    for (auto _ : state) {
        ret = g_composerDevice->ClearClientBuffer(g_displayIds[0]);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, ClearClientBufferTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: ClearLayerBufferTest
  * @tc.desc: Benchmarktest for interface ClearLayerBufferTest.
  */
BENCHMARK_F(DisplayBenchmarkTest, ClearLayerBufferTest)(benchmark::State &state)
{
    int32_t ret = 0;
    uint32_t layerId = 1;
    for (auto _ : state) {
        ret = g_composerDevice->ClearLayerBuffer(g_displayIds[0], layerId);
    }
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, ClearLayerBufferTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();


BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2300)(benchmark::State &state)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = RED
        },
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        },
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = YELLOW
        },
    };

    std::vector<std::vector<int>> zorders = {
        { 3, 2, 1 }, { 1, 3, 2 }, { 3, 1, 2 }, { 1, 2, 3 }, { 2, 1, 3 }, { 2, 3, 1 },
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);

    for (auto _ : state) {
        for (const auto& zorderList : zorders) {
            // adjust the zorder
            for (uint32_t i = 0; i < zorderList.size(); i++) {
                settings[i].zorder = zorderList[i];
                layers[i]->SetZorder(zorderList[i]);
            }
            std::vector<LayerSettings> tempSettings = settings;
            std::sort(tempSettings.begin(), tempSettings.end(),
                [=](const auto& l, auto const & r) { return l.zorder < r.zorder; });
            // present and check
            PresentAndCheck(tempSettings);
        }
    }
    HdiTestDevice::GetInstance().Clear();
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2400)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        },
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    PrepareAndCommit();
    auto layer = layers[0];
    bool preMul = true;
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerPreMulti(g_displayIds[0], layer->GetId(), preMul);
        PrepareAndCommit();
        HdiTestDevice::GetInstance().Clear();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2400)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2500)(benchmark::State &state)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        },
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = RED
        },
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[1];
    LayerAlpha alpha = { 0 };
    alpha.enGlobalAlpha = true;
    alpha.enPixelAlpha = true;
    alpha.gAlpha = 0;
    alpha.alpha0 = 0;
    alpha.alpha1 = 0;
    for (auto _ : state) {
        layer->SetAlpha(alpha);
    }
    PrepareAndCommit();
    HdiTestDevice::GetInstance().Clear();

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2600)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {.rectRatio = {0, 0, 1.0f, 1.0f}, .color = GREEN, .alpha = 0xFF}
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    const int32_t x = 100;
    const int32_t y = 100;
    auto layer = layers[0];
    for (auto _ : state) {
        IRect rect = {0, 0, x, y};
        ret = g_composerDevice->SetLayerRegion(g_displayIds[0], layer->GetId(), rect);
        PrepareAndCommit();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2600)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2700)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    const int32_t x = 100;
    const int32_t y = 100;
    IRect rect = {0, 0, x, y};
    std::vector<IRect> vRects;
    vRects.push_back(rect);
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerDirtyRegion(g_displayIds[0], layer->GetId(), vRects);
        PrepareAndCommit();
        HdiTestDevice::GetInstance().Clear();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2700)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2800)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = RED
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    PrepareAndCommit();
    auto layer = layers[0];
    TransformType type = TransformType::ROTATE_90;
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
        PrepareAndCommit();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2800)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2900)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    PrepareAndCommit();
    auto layer = layers[0];
    const int32_t x = 500;
    const int32_t y = 500;
    IRect region = {0, 0, x, y};
    std::vector<IRect> regions = {};
    regions.push_back(region);
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerVisibleRegion(g_displayIds[0], layer->GetId(), regions);
        PrepareAndCommit();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2900)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3000)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_CLIENT;
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);
        PrepareAndCommit();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3000)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3100)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    BlendType type = BlendType::BLEND_NONE;
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);
        PrepareAndCommit();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3100)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3200)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    MaskInfo maskInfo = MaskInfo::LAYER_HBM_SYNC;
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerMaskInfo(g_displayIds[0], layer->GetId(), maskInfo);
        PrepareAndCommit();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3200)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3300)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    const uint32_t colorRValue = 155;
    const uint32_t colorGValue = 224;
    const uint32_t colorBValue = 88;
    const uint32_t colorAValue = 128;
    LayerColor layerColor = {
        .r = colorRValue,
        .g = colorGValue,
        .b = colorBValue,
        .a = colorAValue
    };
    for (auto _ : state) {
        ret = g_composerDevice->SetLayerColor(g_displayIds[0], layer->GetId(), layerColor);
        PrepareAndCommit();
    }

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3300)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3400)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = PURPLE
        }
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        PrepareAndCommit();

        ret = g_composerDevice->DestroyLayer(g_displayIds[0], layer->GetId());
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_CONT_100));
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3400)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3500)(benchmark::State &state)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = PURPLE
        }
    };
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
    }
    PrepareAndCommit();
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();


BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3600)(benchmark::State &state)
{
    BufferHandle* buffer = nullptr;
    AllocInfo info;
    info.width  = 100;
    info.height = 100;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;
    for (auto _ : state) {
        g_gralloc->AllocMem(info, buffer);
        g_gralloc->Unmap(*buffer);
        g_gralloc->FreeMem(*buffer);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_3600)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

} // namespace
int main(int argc, char** argv)
{
    int ret = HdiTestDevice::GetInstance().InitDevice();
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("Init Device Failed"));
    ::testing::InitGoogleTest(&argc, argv);
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1; // 1: Benchmark native test suite native solution
    }
    g_composerDevice = HdiTestDevice::GetInstance().GetDeviceInterface();
    DISPLAY_TEST_CHK_RETURN((g_composerDevice == nullptr), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("get composer interface failed"));
    g_gralloc.reset(IDisplayBuffer::Get());
    DISPLAY_TEST_CHK_RETURN((g_gralloc == nullptr), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("get buffer interface failed"));
    auto display = HdiTestDevice::GetInstance().GetFirstDisplay();
    if (display != nullptr) {
        g_displayIds = HdiTestDevice::GetInstance().GetDevIds();
        display->SetDisplayVsyncEnabled(false);
    }
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    HdiTestDevice::GetInstance().GetFirstDisplay()->ResetClientLayer();
    return ret;
}