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

namespace {
static sptr<Composer::V1_1::IDisplayComposerInterface> g_composerDevice = nullptr;
static std::shared_ptr<IDisplayBuffer> g_gralloc = nullptr;
static std::vector<uint32_t> g_displayIds;

class DisplayBenchmarkTest : public benchmark::Fixture {
public:
    void TearDown(const ::benchmark::State &state);
    static void OnMode(uint32_t modeId, uint64_t vBlankPeriod, void* data);
    static void OnseamlessChange(uint32_t devId, void* data);
};

void DisplayBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    HdiTestDevice::GetInstance().Clear();
}

void DisplayBenchmarkTest::OnMode(uint32_t modeId, uint64_t vBlankPeriod, void* data)
{
}

void DisplayBenchmarkTest::OnseamlessChange(uint32_t devId, void* data)
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

static int PrepareAndPrensent()
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
                setting.rectRatio.h,setting.displayRect.x,setting.displayRect.y,
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
    int ret = PrepareAndPrensent();
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
    EXPECT_EQ(DISPLAY_FAILURE, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        PrepareAndPrensent();
        auto layer = layers[0];
        bool preMul = true;
        ret = g_composerDevice->SetLayerPreMulti(g_displayIds[0], layer->GetId(), preMul);
    }
    PrepareAndPrensent();
    HdiTestDevice::GetInstance().Clear();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    PrepareAndPrensent();
    HdiTestDevice::GetInstance().Clear();
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2500)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SUB_Driver_Display_Performace_2600)(benchmark::State &state)
{
    int32_t ret;
    std::vector<LayerSettings> settings = {
        {.rectRatio = {0, 0, 1.0f, 1.0f}, .color = GREEN, .alpha = 0xFF}
    };
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));

        const int32_t WIDTH = 100;
        const int32_t HEIGHT = 100;
        auto layer = layers[0];
        IRect rect = {0, 0, WIDTH, HEIGHT};
        ret = g_composerDevice->SetLayerRegion(g_displayIds[0], layer->GetId(), rect);
    }
    PrepareAndPrensent();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        const int32_t WIDTH = 100;
        const int32_t HEIGHT = 100;
        IRect rect = {0, 0, WIDTH, HEIGHT};
        std::vector<IRect> vRects;
        vRects.push_back(rect);
        ret = g_composerDevice->SetLayerDirtyRegion(g_displayIds[0], layer->GetId(), vRects);
    }
    PrepareAndPrensent();
    HdiTestDevice::GetInstance().Clear();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    PrepareAndPrensent();
    auto layer = layers[0];
    for (auto _ : state) {
        TransformType type = TransformType::ROTATE_90;
        ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
        PrepareAndPrensent();

        type = TransformType::ROTATE_180;
        ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
        PrepareAndPrensent();

        type = TransformType::ROTATE_270;
        ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
        PrepareAndPrensent();
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        PrepareAndPrensent();
        auto layer = layers[0];
        const int32_t WIDTH = 500;
        const int32_t HEIGHT = 500;
        IRect region = {0, 0, WIDTH, HEIGHT};
        std::vector<IRect> regions = {};
        regions.push_back(region);
        ret = g_composerDevice->SetLayerVisibleRegion(g_displayIds[0], layer->GetId(), regions);
    }
    PrepareAndPrensent();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_CLIENT;
        ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);
    }
    PrepareAndPrensent();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        BlendType type = BlendType::BLEND_NONE;
        ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);
    }
    PrepareAndPrensent();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        MaskInfo maskInfo = MaskInfo::LAYER_HBM_SYNC;
        ret = g_composerDevice->SetLayerMaskInfo(g_displayIds[0], layer->GetId(), maskInfo);
    }
    PrepareAndPrensent();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        const uint32_t COLOR_R = 155;
        const uint32_t COLOR_G = 224;
        const uint32_t COLOR_B = 88;
        const uint32_t COLOR_A = 128;
        LayerColor layerColor = {
            .r = COLOR_R,
            .g = COLOR_G,
            .b = COLOR_B,
            .a = COLOR_A
        };
        ret = g_composerDevice->SetLayerColor(g_displayIds[0], layer->GetId(), layerColor);
    }
    PrepareAndPrensent();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    for (auto _ : state) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        auto layer = layers[0];
        PrepareAndPrensent();
        ret = g_composerDevice->DestroyLayer(g_displayIds[0], layer->GetId());
    }
    PrepareAndPrensent();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
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
    PrepareAndPrensent();
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

}
int main(int argc, char** argv)
{
    int ret = HdiTestDevice::GetInstance().InitDevice();
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("Init Device Failed"));
    ::testing::InitGoogleTest(&argc, argv);
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1; // 1： Benchmark native test suite native solution
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