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

#include "hdi_composer_ut.h"
#include <chrono>
#include <cinttypes>
#include <algorithm>
#include "v1_0/include/idisplay_composer_interface.h"
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
#include "timer.h"
#include <sys/time.h>
#include <thread>

using namespace OHOS::HDI::Display::Buffer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_1;
using namespace OHOS::HDI::Display::TEST;
using namespace testing::ext;

static sptr<Composer::V1_1::IDisplayComposerInterface> g_composerDevice = nullptr;
static std::shared_ptr<IDisplayBuffer> g_gralloc = nullptr;
static std::vector<uint32_t> g_displayIds;
const int SLEEP_CONT_10 = 10;
const int SLEEP_CONT_100 = 100;
const int SLEEP_CONT_2000 = 2000;
static bool g_isOnSeamlessChangeCalled = false;
static bool g_isOnModeCalled = false;
static bool g_threadCtrl = false;

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

    ret = display->PrepareDisplayLayers(); // 确定顶压策略(是否走GPU合成)、刷新layer列表
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE,
        DISPLAY_TEST_LOGE("PrepareDisplayLayers failed"));

    ret = display->Commit(); // 送显
    DISPLAY_TEST_CHK_RETURN((ret != DISPLAY_SUCCESS), DISPLAY_FAILURE, DISPLAY_TEST_LOGE("Commit failed"));
    return DISPLAY_SUCCESS;
}

static void LoopCommit()
{
    while (!g_threadCtrl) {
        PrepareAndCommit();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_CONT_10));
    }
}

static void TestVBlankCallback(unsigned int sequence, uint64_t ns, void* data)
{
    static uint64_t lastns;
    DISPLAY_TEST_LOGE("seq %{public}d  ns %" PRId64 " duration %" PRId64 " ns", sequence, ns, (ns - lastns));
    lastns = ns;
    VblankCtr::GetInstance().NotifyVblank(sequence, ns, data);
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
    int ret = PrepareAndCommit();
    ASSERT_TRUE((ret == DISPLAY_SUCCESS));
    if ((GetFirstDisplay()->SnapShot()) != nullptr) {
        HdiTestDevice::GetInstance().GetGrallocInterface()->InvalidateCache(*(GetFirstDisplay()->SnapShot()));
        ret = CheckComposition(layerSettings, GetFirstDisplay()->SnapShot(), checkType);
        ASSERT_TRUE((ret == DISPLAY_SUCCESS));
    }
}

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

void DeviceTest::SetUpTestCase()
{
    int ret = HdiTestDevice::GetInstance().InitDevice();
    ASSERT_TRUE(ret == DISPLAY_SUCCESS);

    g_composerDevice = HdiTestDevice::GetInstance().GetDeviceInterface();
    ASSERT_TRUE(g_composerDevice != nullptr);

    g_gralloc.reset(IDisplayBuffer::Get());
    ASSERT_TRUE(g_gralloc != nullptr);

    g_displayIds = HdiTestDevice::GetInstance().GetDevIds();
    ASSERT_TRUE(g_displayIds.size() > 0);
}

void DeviceTest::TearDownTestCase()
{
    HdiTestDevice::GetInstance().Clear();
    HdiTestDevice::GetInstance().GetFirstDisplay()->ResetClientLayer();
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
    ret = vblankCondition_.wait_for(lck, std::chrono::milliseconds(ms), [=] { return hasVblank_; });
    DISPLAY_TEST_LOGE();
    if (!ret) {
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

/**
 * @tc.number: SUB_Driver_Display_HDI_4600
 * @tc.name: test_SetClientBufferCacheCount
 * @tc.desc: test cache count Random
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_4600, TestSize.Level1)
{
    const uint32_t CACHE_COUNT = 5;
    auto ret = g_composerDevice->SetClientBufferCacheCount(g_displayIds[0], CACHE_COUNT);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_4700
 * @tc.name: test_GetDisplayCapability
 * @tc.desc: Obtains the display modes supported by a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_4700, TestSize.Level1)
{
    DisplayCapability info;
    auto ret = g_composerDevice->GetDisplayCapability(g_displayIds[0], info);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_4800
 * @tc.name: test_GetDisplaySupportedModes
 * @tc.desc:  Obtains the current display mode of a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_4800, TestSize.Level1)
{
    std::vector<DisplayModeInfo> modes;
    auto ret = g_composerDevice->GetDisplaySupportedModes(g_displayIds[0], modes);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_4900
 * @tc.name: test_GetDisplayMode
 * @tc.desc: Get the pattern with pattern ID 0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_4900, TestSize.Level1)
{
    uint32_t MODE = 0;
    auto ret = g_composerDevice->GetDisplayMode(g_displayIds[0], MODE);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5000
 * @tc.name: test_SetDisplayMode
 * @tc.desc: Set the display mode ID to 0
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5000, TestSize.Level1)
{
    const uint32_t MODE = 0;
    auto ret = g_composerDevice->SetDisplayMode(g_displayIds[0], MODE);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5100
 * @tc.name: test_GetDisplayPowerStatus
 * @tc.desc: Set the power status of the display device POWER_STATUS_OFF
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5100, TestSize.Level1)
{
    Composer::V1_0::DispPowerStatus powerStatus = Composer::V1_0::DispPowerStatus::POWER_STATUS_OFF;
    auto ret = g_composerDevice->GetDisplayPowerStatus(g_displayIds[0], powerStatus);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5200
 * @tc.name: test_SetDisplayPowerStatus_001
 * @tc.desc: Set the power status of the display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5200, TestSize.Level1)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0],
        Composer::V1_0::DispPowerStatus::POWER_STATUS_STANDBY);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0],
        Composer::V1_0::DispPowerStatus::POWER_STATUS_ON);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5300
 * @tc.name: test_GetDisplayBacklight
 * @tc.desc: Set the backlight value of the display device to leve1
 * @tc.size: MediumTest
 * @tc.type: Function
 */
#ifdef DISPLAY_COMMUNITY
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5300, TestSize.Level1)
{
    uint32_t level;
    auto ret = g_composerDevice->GetDisplayBacklight(g_displayIds[0], level);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}
#endif

/**
 * @tc.number: SUB_Driver_Display_HDI_5400
 * @tc.name: test_SetDisplayBacklight
 * @tc.desc: Sets the backlight value for a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5400, TestSize.Level1)
{
    const uint32_t LEVEL = 10;
    auto ret = g_composerDevice->SetDisplayBacklight(g_displayIds[0], LEVEL);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5500
 * @tc.name: test_GetDisplayProperty
 * @tc.desc: Obtains the property for a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5500, TestSize.Level1)
{
    const uint32_t PROPERTY_ID = 1;
    uint64_t propertyValue = 0;
    auto ret = g_composerDevice->GetDisplayProperty(g_displayIds[0], PROPERTY_ID, propertyValue);
#ifdef DISPLAY_COMMUNITY
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);
#else
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
#endif
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5600
 * @tc.name: test_GetDisplayCompChange
 * @tc.desc: Obtains the layers whose composition types have changed
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5600, TestSize.Level1)
{
    std::vector<uint32_t> layers {};
    std::vector<int32_t> type {};
    auto ret = g_composerDevice->GetDisplayCompChange(g_displayIds[0], layers, type);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5700
 * @tc.name: test_SetDisplayClientCrop
 * @tc.desc: Sets the cropped region for a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5700, TestSize.Level1)
{
    const int32_t WIDTH = 1920;
    const int32_t HEIGHT = 1080;
    IRect rect = {0, 0, WIDTH, HEIGHT};
    auto ret = g_composerDevice->SetDisplayClientCrop(g_displayIds[0], rect);
    // not support
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5800
 * @tc.name: test_GetDisplayReleaseFence
 * @tc.desc: Obtains the fences of the display layers after the commit operation
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5800, TestSize.Level1)
{
    std::vector<uint32_t> layers {};
    std::vector<int32_t> fences {};
    auto ret = g_composerDevice->GetDisplayReleaseFence(g_displayIds[0], layers, fences);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_5900
 * @tc.name: test_SetDisplayClientBuffer
 * @tc.desc: Sets the display buffer for a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_5900, TestSize.Level1)
{
    BufferHandle* buffer = nullptr;
    const int32_t WIDTH = 800;
    const int32_t HEIGHT = 600;

    AllocInfo info;
    info.width  = WIDTH;
    info.height = HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;

    g_gralloc->AllocMem(info, buffer);
    ASSERT_TRUE(buffer != nullptr);

    uint32_t bufferSeq = 1;
    auto ret = g_composerDevice->SetDisplayClientBuffer(g_displayIds[0], buffer, bufferSeq, -1);
    g_gralloc->FreeMem(*buffer);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6000
 * @tc.name: test_SetDisplayClientDamage
 * @tc.desc: Sets the dirty region for a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6000, TestSize.Level1)
{
    const int32_t WIDTH = 1920;
    const int32_t HEIGHT = 1080;
    IRect rect = {0, 0, WIDTH, HEIGHT};
    std::vector<IRect> vRects;
    vRects.push_back(rect);
    auto ret = g_composerDevice->SetDisplayClientDamage(g_displayIds[0], vRects);
    // not support
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6100
 * @tc.name: test_CreateVirtualDisplay
 * @tc.desc: Creates a virtual display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6100, TestSize.Level1)
{
    const uint32_t WIDTH = 1920;
    const uint32_t HEIGHT = 1080;
    int32_t format = 0;
    uint32_t devId = 0;
    auto ret = g_composerDevice->CreateVirtualDisplay(WIDTH, HEIGHT, format, devId);
    // not support
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6200
 * @tc.name: test_DestroyVirtualDisplay
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6200, TestSize.Level1)
{
    uint32_t devId = 0;
    auto ret = g_composerDevice->DestroyVirtualDisplay(devId);
    // not support
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6300
 * @tc.name: test_SetVirtualDisplayBuffer
 * @tc.desc: Destroys a virtual display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6300, TestSize.Level1)
{
    BufferHandle* buffer = nullptr;
    int32_t fence = -1;
    const int32_t WIDTH = 800;
    const int32_t HEIGHT = 600;

    AllocInfo info;
    info.width  = WIDTH;
    info.height = HEIGHT;
    info.usage = OHOS::HDI::Display::Composer::V1_0::HBM_USE_MEM_DMA |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_READ |
            OHOS::HDI::Display::Composer::V1_0::HBM_USE_CPU_WRITE;
    info.format = Composer::V1_0::PIXEL_FMT_RGBA_8888;

    g_gralloc->AllocMem(info, buffer);
    ASSERT_TRUE(buffer != nullptr);

    auto ret = g_composerDevice->SetVirtualDisplayBuffer(g_displayIds[0], *buffer, fence);
    g_gralloc->FreeMem(*buffer);
    // not support
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6400
 * @tc.name: test_SetDisplayProperty
 * @tc.desc: Sets the property for a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6400, TestSize.Level1)
{
    const uint32_t PROPERTY_ID = 1;
    const uint64_t PROPERTY_VALUE = 0;
    auto ret = g_composerDevice->SetDisplayProperty(g_displayIds[0], PROPERTY_ID, PROPERTY_VALUE);
    // not support
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6500
 * @tc.name: test_SetLayerCrop
 * @tc.desc: Sets the rectangular area to crop for a layer, Please note that failing to save the composite
 * data using clientLayer during display HDI adaptation image synthesis will cause this use case to fail
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6500, TestSize.Level1)
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
    PrepareAndCommit();
    for (uint32_t i = 0; i < splitRects.size(); i++) {
        settings[0].color = splitColors[i];
        layer->SetLayerCrop(splitRects[i]);
        PresentAndCheck(settings);
    }

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6600
 * @tc.name: test_SetLayerZorder
 * @tc.desc: Sets the z-order for a layer, Please note that failing to save the composite data
 * using clientLayer during display HDI adaptation image synthesis will cause this use case to fail
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6600, TestSize.Level1)
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
    HdiTestDevice::GetInstance().Clear();
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6700
 * @tc.name: test_SetLayerPreMulti
 * @tc.desc: Sets layer premultiplication
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6700, TestSize.Level1)
{
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
    auto ret = g_composerDevice->SetLayerPreMulti(g_displayIds[0], layer->GetId(), preMul);

    PrepareAndCommit();
    HdiTestDevice::GetInstance().Clear();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6800
 * @tc.name: test_SetLayerAlpha
 * @tc.desc: Sets the alpha value for a layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6800, TestSize.Level1)
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
    layer->SetAlpha(alpha);

    PrepareAndCommit();
    HdiTestDevice::GetInstance().Clear();

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_6900
 * @tc.name: test_SetLayerRegion
 * @tc.desc: Sets the region for a layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_6900, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {.rectRatio = {0, 0, 1.0f, 1.0f}, .color = GREEN, .alpha = 0xFF}
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    const int32_t WIDTH = 100;
    const int32_t HEIGHT = 100;
    auto layer = layers[0];
    IRect rect = {0, 0, WIDTH, HEIGHT};
    auto ret = g_composerDevice->SetLayerRegion(g_displayIds[0], layer->GetId(), rect);

    PrepareAndCommit();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7000
 * @tc.name: test_SetLayerDirtyRegion
 * @tc.desc: Sets the flushing area for a layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7000, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    const int32_t WIDTH = 100;
    const int32_t HEIGHT = 100;
    IRect rect = {0, 0, WIDTH, HEIGHT};
    std::vector<IRect> vRects;
    vRects.push_back(rect);
    auto ret = g_composerDevice->SetLayerDirtyRegion(g_displayIds[0], layer->GetId(), vRects);

    PrepareAndCommit();
    HdiTestDevice::GetInstance().Clear();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7100
 * @tc.name: test_SetLayerTransformMode_001
 * @tc.desc: Sets the type of graphic rotation
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7100, TestSize.Level1)
{
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
    auto ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    type = TransformType::ROTATE_180;
    ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    type = TransformType::ROTATE_270;
    ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7200
 * @tc.name: test_SetLayerVisibleRegion
 * @tc.desc: Set the visible region for a layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7200, TestSize.Level1)
{
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

    const int32_t WIDTH = 500;
    const int32_t HEIGHT = 500;
    IRect region = {0, 0, WIDTH, HEIGHT};
    std::vector<IRect> regions = {};
    regions.push_back(region);
    auto ret = g_composerDevice->SetLayerVisibleRegion(g_displayIds[0], layer->GetId(), regions);
    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7300
 * @tc.name: test_SetLayerBuffer
 * @tc.desc: Set the buffer for a layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7300, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    auto graphicBuffer = layer->AcquireBackBuffer();
    int32_t ret = graphicBuffer->SetGraphicBuffer([&](const BufferHandle* buffer, uint32_t seqNo) -> int32_t {
        std::vector<uint32_t> deletingList;
        int32_t result = g_composerDevice->SetLayerBuffer(g_displayIds[0], layer->GetId(), buffer, seqNo, -1,
            deletingList);
        return result;
    });
    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7400
 * @tc.name: test_SetLayerCompositionType_001
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7400, TestSize.Level1)
{
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
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7500
 * @tc.name: test_SetLayerBlendType_001
 * @tc.desc: Set the blend type to BLEND-NONE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7500, TestSize.Level1)
{
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
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_MaskInfo_0100
 * @tc.name: test_SetLayerMaskInfo_001
 * @tc.desc: Sets the current mask frame information to the vendor driver
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_MaskInfo_0100, TestSize.Level1)
{
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
    auto ret = g_composerDevice->SetLayerMaskInfo(g_displayIds[0], layer->GetId(), maskInfo);

    HdiTestDevice::GetInstance().Clear();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_Luminance_0100
 * @tc.name: test_SetLayerColor
 * @tc.desc: Sets the solid color layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_Luminance_0100, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

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

    auto ret = g_composerDevice->SetLayerColor(g_displayIds[0], layer->GetId(), layerColor);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7600
 * @tc.name: test_DestroyLayer
 * @tc.desc: Closes a layer that is no longer required on a specified display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7600, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = PURPLE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    PrepareAndCommit();

    sleep(1);
    auto ret = g_composerDevice->DestroyLayer(g_displayIds[0], layer->GetId());
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_CONT_100));
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7700
 * @tc.name: test_RegDisplayVBlankCallback
 * @tc.desc: Registers the callback to be invoked when a VBLANK event occurs
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7700, TestSize.Level1)
{
    int ret;
    DISPLAY_TEST_LOGE();
    std::shared_ptr<HdiTestDisplay> display = HdiTestDevice::GetInstance().GetFirstDisplay();
    ASSERT_TRUE(display != nullptr) << "get display failed";
    ret = display->RegDisplayVBlankCallback(TestVBlankCallback, nullptr);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "RegDisplayVBlankCallback failed";
    ret = display->SetDisplayVsyncEnabled(true);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "SetDisplayVsyncEnabled failed";

    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = PINK
        },
    };
    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    VblankCtr::GetInstance().hasVblank_ = false;
    PrepareAndCommit();
    ret = VblankCtr::GetInstance().WaitVblank(SLEEP_CONT_100); // 100ms
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "WaitVblank timeout";
    ret = display->SetDisplayVsyncEnabled(false);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "SetDisplayVsyncEnabled failed";
    usleep(SLEEP_CONT_100 * SLEEP_CONT_2000); // wait for 100ms avoid the last vsync.
    VblankCtr::GetInstance().hasVblank_ = false;
    ret = VblankCtr::GetInstance().WaitVblank(SLEEP_CONT_100); // 100ms
    ASSERT_TRUE(ret != DISPLAY_SUCCESS) << "vblank do not disable";

    DestroyLayer(layers[0]);
}

void DeviceTest::OnMode(uint32_t modeId, uint64_t vBlankPeriod, void* data)
{
    g_isOnModeCalled = true;
}

void DeviceTest::OnSeamlessChange(uint32_t devId, void* data)
{
    g_isOnSeamlessChangeCalled = true;
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7800
 * @tc.name: test_GetDisplaySupportedModesExt
 * @tc.desc: Obtains the display modes supported by a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7800, TestSize.Level1)
{
    std::vector<DisplayModeInfoExt> modes;
    auto ret = g_composerDevice->GetDisplaySupportedModesExt(g_displayIds[0], modes);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetDisplaySupportedModesExt not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_7900
 * @tc.name: test_SetDisplayModeAsync
 * @tc.desc: Sets the display mode of a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_7900, TestSize.Level1)
{
    g_isOnModeCalled = false;
    std::vector<DisplayModeInfo> oldModes;
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = RED
        }
    };

    // 先注册VBlankCallback
    auto ret = g_composerDevice->RegDisplayVBlankCallback(g_displayIds[0], TestVBlankCallback, nullptr);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS) << "RegDisplayVBlankCallback failed";

    ret = g_composerDevice->GetDisplaySupportedModes(g_displayIds[0], oldModes);
    ASSERT_EQ(DISPLAY_SUCCESS, ret);
    ASSERT_EQ(oldModes.size() > 0, true);

    uint32_t modeid = oldModes[0].id;
    ret = g_composerDevice->SetDisplayModeAsync(g_displayIds[0], modeid, OnMode);
    if (ret == DISPLAY_NOT_SUPPORT) {
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    if (ret == DISPLAY_SUCCESS) {
        std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
        ASSERT_TRUE((layers.size() > 0));
        g_threadCtrl = false;
        std::thread commitThread(LoopCommit);
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_CONT_100));
        g_threadCtrl = true;
        commitThread.join();
        ASSERT_EQ(g_isOnModeCalled, true);

        DestroyLayer(layers[0]);
    }
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8000
 * @tc.name: test_GetDisplayVBlankPeriod
 * @tc.desc: Get the current vblank period
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8000, TestSize.Level1)
{
    uint64_t period = 0;
    auto ret = g_composerDevice->GetDisplayVBlankPeriod(g_displayIds[0], period);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetDisplayVBlankPeriod not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    EXPECT_EQ(period != 0, true);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8100
 * @tc.name: test_RegSeamlessChangeCallback
 * @tc.desc: UpdateSettings, OHOS_CONTROL_AE_MODE
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8100, TestSize.Level1)
{
    g_isOnSeamlessChangeCalled = false;
    auto ret = g_composerDevice->RegSeamlessChangeCallback(OnSeamlessChange, nullptr);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("RegSeamlessChangeCallback not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    if (ret == DISPLAY_SUCCESS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        ASSERT_EQ(g_isOnSeamlessChangeCalled, true);
    }
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8200
 * @tc.name: test_SetLayerPerFrameParameter
 * @tc.desc: Sets parameter for the given layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8200, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        },
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));
    auto layer = layers[0];
    std::vector<std::string> ValidKeys = { "FilmFilter", "ArsrDoEnhance", "SDRBrightnessRatio", "BrightnessNit",
        "ViewGroupHasValidAlpha", "SourceCropTuning" };
    std::string key;
    std::vector<int8_t> value = { 1 };
    key = "NotSupportKey";
    auto ret = g_composerDevice->SetLayerPerFrameParameter(g_displayIds[0], layer->GetId(), key, value);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("SetLayerPerFrameParameter not support");
        return;
    }
    ASSERT_EQ(ret, -1) << "key not support, ret:" << ret;
    key = ValidKeys[0];
    ret = g_composerDevice->SetLayerPerFrameParameter(g_displayIds[0], layer->GetId(), key, value);
    ASSERT_EQ(ret, 0) << "key support, ret:" << ret;
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("SetLayerPerFrameParameter not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8300
 * @tc.name: test_GetSupportedLayerPerFrameParameterKey
 * @tc.desc: returns the list of supported parameter keys
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8300, TestSize.Level1)
{
    std::vector<std::string> keys;
    auto ret = g_composerDevice->GetSupportedLayerPerFrameParameterKey(keys);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetSupportedLayerPerFrameParameterKey not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8400
 * @tc.name: test_SetDisplayOverlayResolution
 * @tc.desc: Sets parameter for the given layer
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8400, TestSize.Level1)
{
    DisplayModeInfo mode = GetFirstDisplay()->GetCurrentMode();
    auto ret = g_composerDevice->SetDisplayOverlayResolution(g_displayIds[0], mode.width, mode.height);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("SetDisplayOverlayResolution not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

static void TestRefreshCallback(uint32_t devId, void* data)
{
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8500
 * @tc.name: test_RegRefreshCallback
 * @tc.desc: Registers the callback to be invoked when a refresh event occurs
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8500, TestSize.Level1)
{
    auto ret = g_composerDevice->RegRefreshCallback(TestRefreshCallback, nullptr);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("RegRefreshCallback not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8600
 * @tc.name: test_GetDisplaySupportedColorGamuts
 * @tc.desc: Obtains the color gamuts of a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8600, TestSize.Level1)
{
    std::vector<ColorGamut> gamuts;
    auto ret = g_composerDevice->GetDisplaySupportedColorGamuts(g_displayIds[0], gamuts);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetDisplaySupportedColorGamuts not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Driver_Display_HDI_8700
 * @tc.name: test_GetHDRCapabilityInfos
 * @tc.desc: Obtains the capabilities of a display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_Driver_Display_HDI_8700, TestSize.Level1)
{
    HDRCapability info = { 0 };
    auto ret = g_composerDevice->GetHDRCapabilityInfos(g_displayIds[0], info);
    if (ret == DISPLAY_NOT_SUPPORT) {
        DISPLAY_TEST_LOGE("GetHDRCapabilityInfos not support");
        return;
    }
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0340
 * @tc.name: test_SetLayerTransformMode_002
 * @tc.desc: Sets the type of graphic rotation
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0340, TestSize.Level1)
{
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

    TransformType type = TransformType::MIRROR_H;
    auto ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    type = TransformType::MIRROR_V;
    ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0350
 * @tc.name: test_SetLayerTransformMode_003
 * @tc.desc: Sets the type of graphic rotation
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0350, TestSize.Level1)
{
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

    TransformType type = TransformType::MIRROR_H_ROTATE_90;
    auto ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    type = TransformType::MIRROR_V_ROTATE_90;
    ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0360
 * @tc.name: test_SetLayerTransformMode_004
 * @tc.desc: Sets the type of graphic rotation
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0360, TestSize.Level1)
{
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

    TransformType type = TransformType::ROTATE_BUTT;
    auto ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0370
 * @tc.name: test_SetDisplayPowerStatus_002
 * @tc.desc: Set the power status of the display device
 * @tc.size: MediumTest
 * @tc.type: Function
 */
#ifdef DISPLAY_COMMUNITY
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0370, TestSize.Level1)
{
    auto ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0],
        Composer::V1_0::DispPowerStatus::POWER_STATUS_SUSPEND);
    EXPECT_EQ(DISPLAY_SUCCESS, ret);

    ret = g_composerDevice->SetDisplayPowerStatus(g_displayIds[0],
        Composer::V1_0::DispPowerStatus::POWER_STATUS_BUTT);
    EXPECT_EQ(DISPLAY_FAILURE, ret);
}
#endif

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0380
 * @tc.name: test_SetLayerBlendType_002
 * @tc.desc: Set the blend type to BLEND_CLEAR
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0380, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_CLEAR;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0390
 * @tc.name: test_SetLayerBlendType_003
 * @tc.desc: Set the blend type to BLEND_SRC
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0390, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_SRC;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0400
 * @tc.name: test_SetLayerBlendType_004
 * @tc.desc: Set the blend type to BLEND_SRCOVER
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0400, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_SRCOVER;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0410
 * @tc.name: test_SetLayerBlendType_005
 * @tc.desc: Set the blend type to BLEND_DSTOVER
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0410, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_DSTOVER;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0420
 * @tc.name: test_SetLayerBlendType_006
 * @tc.desc: Set the blend type to BLEND_SRCIN
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0420, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_SRCIN;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0430
 * @tc.name: test_SetLayerBlendType_007
 * @tc.desc: Set the blend type to BLEND_DSTIN
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0430, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_DSTIN;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0440
 * @tc.name: test_SetLayerBlendType_008
 * @tc.desc: Set the blend type to BLEND_SRCOUT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0440, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_SRCOUT;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0450
 * @tc.name: test_SetLayerBlendType_009
 * @tc.desc: Set the blend type to BLEND_DSTOUT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0450, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_DSTOUT;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0460
 * @tc.name: test_SetLayerBlendType_010
 * @tc.desc: Set the blend type to BLEND_SRCATOP
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0460, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_SRCATOP;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0470
 * @tc.name: test_SetLayerBlendType_011
 * @tc.desc: Set the blend type to BLEND_DSTATOP
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0470, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_DSTATOP;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0480
 * @tc.name: test_SetLayerBlendType_012
 * @tc.desc: Set the blend type to BLEND_ADD
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0480, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_ADD;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0490
 * @tc.name: test_SetLayerBlendType_013
 * @tc.desc: Set the blend type to BLEND_XOR
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0490, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_XOR;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0500
 * @tc.name: test_SetLayerBlendType_014
 * @tc.desc: Set the blend type to BLEND_DST
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0500, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_DST;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0510
 * @tc.name: test_SetLayerBlendType_015
 * @tc.desc: Set the blend type to BLEND_AKS
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0510, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_AKS;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0520
 * @tc.name: test_SetLayerBlendType_016
 * @tc.desc: Set the blend type to BLEND_AKD
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0520, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_AKD;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0530
 * @tc.name: test_SetLayerBlendType_017
 * @tc.desc: Set the blend type to BLEND_BUTT
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0530, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    BlendType type = BlendType::BLEND_BUTT;
    auto ret = g_composerDevice->SetLayerBlendType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0540
 * @tc.name: test_SetLayerMaskInfo_002
 * @tc.desc: Sets the current mask frame information to the vendor driver
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0540, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = GREEN
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    MaskInfo maskInfo = MaskInfo::LAYER_NORAML;
    auto ret = g_composerDevice->SetLayerMaskInfo(g_displayIds[0], layer->GetId(), maskInfo);

    HdiTestDevice::GetInstance().Clear();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0550
 * @tc.name: test_SetLayerCompositionType_002
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0550, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_DEVICE;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0560
 * @tc.name: test_SetLayerCompositionType_003
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0560, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_CURSOR;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0570
 * @tc.name: test_SetLayerCompositionType_004
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0570, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_VIDEO;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0580
 * @tc.name: test_SetLayerCompositionType_005
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0580, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_DEVICE_CLEAR;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0590
 * @tc.name: test_SetLayerCompositionType_006
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0590, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_CLIENT_CLEAR;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0600
 * @tc.name: test_SetLayerCompositionType_007
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0600, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_TUNNEL;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0610
 * @tc.name: test_SetLayerCompositionType_008
 * @tc.desc: set the composition type which the client expect
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0610, TestSize.Level1)
{
    std::vector<LayerSettings> settings = {
        {
            .rectRatio = { 0, 0, 1.0f, 1.0f },
            .color = BLUE
        }
    };

    std::vector<std::shared_ptr<HdiTestLayer>> layers = CreateLayers(settings);
    ASSERT_TRUE((layers.size() > 0));

    auto layer = layers[0];

    Composer::V1_0::CompositionType type = Composer::V1_0::CompositionType::COMPOSITION_BUTT;
    auto ret = g_composerDevice->SetLayerCompositionType(g_displayIds[0], layer->GetId(), type);

    PrepareAndCommit();

    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}

/**
 * @tc.number: SUB_DriverSystem_DisplayComposer_0620
 * @tc.name: test_SetLayerTransformMode_005
 * @tc.desc: Sets the type of graphic rotation
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DeviceTest, SUB_DriverSystem_DisplayComposer_0620, TestSize.Level1)
{
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

    TransformType type = TransformType::ROTATE_NONE;
    auto ret = g_composerDevice->SetLayerTransformMode(g_displayIds[0], layer->GetId(), type);
    PrepareAndCommit();
    EXPECT_EQ(DISPLAY_SUCCESS, ret);
    DestroyLayer(layer);
}