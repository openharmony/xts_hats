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
#include "v1_0/include/idisplay_composer_interface.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/display_buffer_type.h"
#include "display_test.h"
#include "display_test_utils.h"
#include "hdi_composition_check.h"
#include "hdi_test_device.h"
#include "hdi_test_device_common.h"
#include "hdi_test_display.h"
#include "hdi_test_render_utils.h"

using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::TEST;
using namespace testing::ext;
using namespace std;

namespace {
std::shared_ptr<IDisplayComposerInterface> g_composerDevice {};
static std::shared_ptr<HdiTestLayer> g_testFreshLayer;
std::shared_ptr<IDisplayBuffer> g_gralloc = nullptr;
std::vector<uint32_t> g_displayIds;

class DisplayBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void DisplayBenchmarkTest::SetUp(const ::benchmark::State &state)
{
}

void DisplayBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    HdiTestDevice::GetInstance().Clear();
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
            setting.rectRatio.x, setting.rectRatio.y, setting.rectRatio.w, setting.rectRatio.h, setting.displayRect.x,
                setting.displayRect.y, setting.displayRect.w, setting.displayRect.h);
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
  * @tc.name: GetDisplayCapabilityTest
  * @tc.desc: Benchmarktest for interface GetDisplayCapability.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayCapabilityTest)(benchmark::State &state)
{
    int32_t ret;
    DisplayCapability info;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayCapability(g_displayIds[0], info);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayCapabilityTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplaySupportedModesTest
  * @tc.desc: Benchmarktest for interface GetDisplaySupportedModes.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplaySupportedModesTest)(benchmark::State &state)
{
    int32_t ret;
    std::vector<DisplayModeInfo> modes;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplaySupportedModes(g_displayIds[0], modes);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplaySupportedModesTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayModeTest
  * @tc.desc: Benchmarktest for interface GetDisplayMode.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayModeTest)(benchmark::State &state)
{
    int32_t ret;
    uint32_t modeId = 0;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayMode(modeId, modeId);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayModeTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayModeTest
  * @tc.desc: Benchmarktest for interface SetDisplayMode.
  */
BENCHMARK_F(DisplayBenchmarkTest, SetDisplayModeTest)(benchmark::State &state)
{
    int32_t ret;
    const uint32_t modeId = 0;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayMode(g_displayIds[0], modeId);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetDisplayModeTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayPowerStatusTest
  * @tc.desc: Benchmarktest for interface GetDisplayPowerStatus.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayPowerStatusTest)(benchmark::State &state)
{
    int32_t ret;
    DispPowerStatus powerStatus = DispPowerStatus::POWER_STATUS_OFF;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayPowerStatus(g_displayIds[0], powerStatus);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayPowerStatusTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayPowerStatusTest
  * @tc.desc: Benchmarktest for interface SetDisplayPowerStatus.
  */
BENCHMARK_F(DisplayBenchmarkTest, SetDisplayPowerStatusTest)(benchmark::State &state)
{
    int32_t ret;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0], DispPowerStatus::POWER_STATUS_ON);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetDisplayPowerStatusTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayBacklightTest
  * @tc.desc: Benchmarktest for interface GetDisplayBacklight.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayBacklightTest)(benchmark::State &state)
{
    int32_t ret;
    uint32_t level;
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayBacklight(g_displayIds[0], level);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayBacklightTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: SetDisplayBacklightTest
  * @tc.desc: Benchmarktest for interface SetDisplayBacklight.
  */
BENCHMARK_F(DisplayBenchmarkTest, SetDisplayBacklightTest)(benchmark::State &state)
{
    int32_t ret;
    const uint32_t level = 10;
    for (auto _ : state) {
        ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], level);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetDisplayBacklightTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayCompChangeTest
  * @tc.desc: Benchmarktest for interface GetDisplayCompChange.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayCompChangeTest)(benchmark::State &state)
{
    int32_t ret;
    std::vector<uint32_t> layers {};
    std::vector<int32_t> type {};
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayCompChange(g_displayIds[0], layers, type);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayCompChangeTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.name: GetDisplayReleaseFenceTest
  * @tc.desc: Benchmarktest for interface GetDisplayReleaseFence.
  */
BENCHMARK_F(DisplayBenchmarkTest, GetDisplayReleaseFenceTest)(benchmark::State &state)
{
    int32_t ret;
    std::vector<uint32_t> layers {};
    std::vector<int32_t> fences {};
    for (auto _ : state) {
        ret = g_composerDevice->GetDisplayReleaseFence(g_displayIds[0], layers, fences);
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, GetDisplayReleaseFenceTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, AllocMemTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, AllocMemTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerCropTest)(benchmark::State &state)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = RED
        },
    };
    std::vector<uint32_t> splitColors = { { RED, GREEN, YELLOW, BLUE, PINK, PURPLE, CYAN, TRANSPARENT } };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    // split the buffer
    auto layer = layers[0];
    HdiGrallocBuffer* handle = layer->GetBackBuffer(); // the backbuffer has not present now
    ASSERT_TRUE((handle != nullptr));
    auto splitRects = SplitBuffer(*(handle->Get()), splitColors);
    PrepareAndPrensent();
    for (uint32_t i = 0; i < splitRects.size(); i++) {
        settings[0].color = splitColors[i];
        for (auto _ : state) {
            layer->SetLayerCrop(splitRects[i]);
        }
        PresentAndCheck(settings);
    }
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerCropTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetZorderTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetZorderTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerPreMultiTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerPreMultiTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetAlphaTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetAlphaTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerRegionTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerRegionTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerDirtyRegionTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerDirtyRegionTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerTransformModeTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerTransformModeTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerVisibleRegionTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerVisibleRegionTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerCompositionTypeTest)(benchmark::State &state)
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
        CompositionType type = CompositionType::COMPOSITION_CLIENT;
        ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);
    }
    PrepareAndPrensent();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerCompositionTypeTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, SetLayerBlendTypeTest)(benchmark::State &state)
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

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, SetLayerBlendTypeTest)->
    Iterations(100)->Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_F(DisplayBenchmarkTest, DestroyLayerTest)(benchmark::State &state)
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
        sleep(1);
        ret = g_composerDevice->DestroyLayer(g_displayIds[0], layer->GetId());
    }
    PrepareAndPrensent();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

BENCHMARK_REGISTER_F(DisplayBenchmarkTest, DestroyLayerTest)->
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
    g_gralloc.reset(IDisplayBuffer::Get());
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