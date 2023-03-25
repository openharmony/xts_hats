/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "../GrallocAllocTest.h"
#include <securec.h>
#include "gtest/gtest.h"
#include "display_gralloc.h"
#include "../../common/display_test.h"
#include "../../common/display_test_utils.h"

namespace OHOS {
namespace HDI {
namespace DISPLAY {
namespace TEST {
struct AllocTestPrms {
    AllocInfo allocInfo;
    int32_t expectStride;
    int32_t expectSize;
};

class GrallocAllocTest : public ::testing::TestWithParam<AllocTestPrms> {
protected:
    virtual void SetUp();
    virtual void TearDown();
    int32_t AllocMemTest(AllocTestPrms &info);
    GrallocFuncs *mGrallocFuncs = nullptr;
};

class HdiDisplayTest : public ::testing::Test {
protected:
    void TearDown();
    int ret;
};

int ALIGN_UP(int x, int a)
{
    return ((x + (a-1)) / a) * a;
}
const int HEIGHT_ALIGN = 2U;
const AllocTestPrms GRALLOC_TEST_SETS[] = {
    // num0
    // SUB_DriverSystem_DisplayHdi_0050
    {
        .allocInfo = {
            .width = 1920,
            .height = 1080,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_8888
        },
        .expectStride = 1920 * 4,
        .expectSize = 1920 * 1080 * 4
    },
    // num1
    // SUB_DriverSystem_DisplayHdi_0060
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_8888
            },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num2
    // SUB_DriverSystem_DisplayHdi_0070
    {
        .allocInfo = {
            .width = 1280,
            .height = 720,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_8888
        },
        .expectStride = 1280 * 4,
        .expectSize = 1280 * 720 * 4
    },
    // num3
    // SUB_DriverSystem_DisplayHdi_0080
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBA_8888
            },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num4
    // SUB_DriverSystem_DisplayHdi_0090
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGB_888
        },
        .expectStride = 1088 * 3,
        .expectSize =  1920 * 1088 * 3
    },
    // num5
    // SUB_DriverSystem_DisplayHdi_0100
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGRA_8888
            },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num6
    // SUB_DriverSystem_DisplayHdi_0110
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGRX_8888
        },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num7
    // SUB_DriverSystem_DisplayHdi_0120
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBA_4444
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num8
    // SUB_DriverSystem_DisplayHdi_0130
    {
        .allocInfo =
        {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_4444
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num9
    // SUB_DriverSystem_DisplayHdi_0140
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGRA_4444
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num10
    // SUB_DriverSystem_DisplayHdi_0150
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGRX_4444
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num11
    // SUB_DriverSystem_DisplayHdi_0160
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGR_565
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num12
    // SUB_DriverSystem_DisplayHdi_0170
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGRA_5551
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num13
    // SUB_DriverSystem_DisplayHdi_0180
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_BGRX_5551
        },
        .expectStride = 1088 * 2,
        .expectSize = 1920 * 1088 * 2
    },
    // num14
    // SUB_DriverSystem_DisplayHdi_0190
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_YCBCR_420_SP
        },
        .expectStride = 1088,
        .expectSize = 1920 * 1088 * 3 / 2,
    },
    // num15
    // SUB_DriverSystem_DisplayHdi_0200
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_YCRCB_420_SP
        },
        .expectStride = 1088,
        .expectSize = 1920 * 1088 * 3 / 2,
    },
    // num16
    // SUB_DriverSystem_DisplayHdi_0210
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_YCBCR_420_P
        },
        .expectStride = 1088,
        .expectSize = 1920 * 1088 * 3 / 2
    },
    // num17
    // SUB_DriverSystem_DisplayHdi_0220
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_YCRCB_420_P
        },
        .expectStride = 1088,
        .expectSize = 1920 * 1088 * 3 / 2
    },
    // num18
    // SUB_DriverSystem_DisplayHdi_0230
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA,
            .format = PIXEL_FMT_RGBX_8888
        },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num19
    // SUB_DriverSystem_DisplayHdi_0240
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ,
            .format = PIXEL_FMT_RGBX_8888
        },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num20
    // SUB_DriverSystem_DisplayHdi_0250
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_8888
        },
        .expectStride = 1088 * 4,
        .expectSize = 1920 * 1088 * 4
    },
    // num21
    // SUB_DriverSystem_DisplayHdi_0260
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_8888
            },
        .expectStride = 4352,
        .expectSize = 8355840
    },
    // num22
    // SUB_DriverSystem_DisplayHdi_0270
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGB_888
        },
        .expectStride = 3264,
        .expectSize = 6266880
    },
    // num23
    // SUB_DriverSystem_DisplayHdi_0280
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_READ | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBA_4444
        },
        .expectStride = 2176,
        .expectSize = 4177920
    },
    // num24
    // SUB_DriverSystem_DisplayHdi_0290
    {
        .allocInfo = {
            .width = 1080,
            .height = 1920,
            .usage = HBM_USE_MEM_DMA | HBM_USE_CPU_WRITE,
            .format = PIXEL_FMT_RGBX_8888
        },
        .expectStride = 4352, // expectStride
        .expectSize = 8355840 // expectSize
    },
};

static bool CheckBufferHandle(AllocTestPrms &info, BufferHandle &buffer)
{
    if (buffer.stride != (ALIGN_UP(info.expectStride, HEIGHT_ALIGN))) {
        DISPLAY_TEST_LOGE("stride check failed stride %d, expect stride %d ", buffer.stride, info.expectStride);
        DISPLAY_TEST_LOGE("stride check failed format %d width %d, height %d ", info.allocInfo.format,
            info.allocInfo.width, info.allocInfo.height);
        return false;
    }

    if (buffer.size != info.expectSize) {
        DISPLAY_TEST_LOGE("size check failed size %d, expect size %d ", buffer.size, info.expectSize);
        DISPLAY_TEST_LOGE("stride check failed format %d width %d, height %d ", info.allocInfo.format,
            info.allocInfo.width, info.allocInfo.height);
        return false;
    }
    return true;
}

void GrallocAllocTest::SetUp()
{
    if (GrallocInitialize(&mGrallocFuncs) != DISPLAY_SUCCESS) {
        DISPLAY_TEST_LOGE("DisplayInit failure\n");
        ASSERT_TRUE(0);
    }
}

void GrallocAllocTest::TearDown()
{
    if (GrallocUninitialize(mGrallocFuncs) != DISPLAY_SUCCESS) {
        DISPLAY_TEST_LOGE("DisplayUninit failure\n");
        ASSERT_TRUE(0);
    }
}

void HdiDisplayTest::TearDown()
{
}

int32_t GrallocAllocTest::AllocMemTest(AllocTestPrms &info)
{
    int ret;
    BufferHandle *buffer = nullptr;
    const int testCount = 21; // test 40 times
    for (int i = 0; i < testCount; i++) {
        ret = mGrallocFuncs->AllocMem(&info.allocInfo, &buffer);
        if (ret != DISPLAY_SUCCESS) {
            return ret;
        }
        void *vAddr = mGrallocFuncs->Mmap(buffer);
        if (vAddr == nullptr) {
            return DISPLAY_FAILURE;
        }

        if (info.allocInfo.usage & (HBM_USE_CPU_READ | HBM_USE_CPU_WRITE)) {
            ret = mGrallocFuncs->InvalidateCache(buffer);
            if (ret != DISPLAY_SUCCESS) {
                return ret;
            }
        }
        if (memset_s(vAddr, buffer->size, 0, buffer->size) != EOK) {
            return DISPLAY_NOMEM;
        }
        DISPLAY_TEST_CHK_RETURN(!CheckBufferHandle(info, *buffer), DISPLAY_FAILURE,
            DISPLAY_TEST_LOGE("buffer check failed"));
        if (info.allocInfo.usage & (HBM_USE_CPU_READ | HBM_USE_CPU_WRITE)) {
            ret = mGrallocFuncs->FlushCache(buffer);
            if (ret != DISPLAY_SUCCESS) {
                return ret;
            }
        }
        mGrallocFuncs->Unmap((buffer));
        mGrallocFuncs->FreeMem(buffer);
    }

    return DISPLAY_SUCCESS;
}

TEST(GrallocAllocTest, NULLPTR)
{
    int ret = GrallocInitialize(nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);

    GrallocFuncs *grallocFuncs;
    AllocInfo allocInfo;
    BufferHandle *hdl;
    ret = GrallocInitialize(&grallocFuncs);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS);
    ret = grallocFuncs->AllocMem(nullptr, nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);
    ret = grallocFuncs->AllocMem(&allocInfo, nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);
    ret = grallocFuncs->AllocMem(nullptr, &hdl);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);
    ret = grallocFuncs->InvalidateCache(nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);
    ret = grallocFuncs->FlushCache(nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);
    grallocFuncs->FreeMem(nullptr);
    void *vAddr = grallocFuncs->Mmap(nullptr);
    ASSERT_TRUE(vAddr == nullptr);
    ret = grallocFuncs->Unmap(nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);
    ret = GrallocUninitialize(nullptr);
    ASSERT_TRUE(ret != DISPLAY_SUCCESS);

    ret = GrallocUninitialize(grallocFuncs);
    ASSERT_TRUE(ret == DISPLAY_SUCCESS);
}

TEST_P(GrallocAllocTest, GrallocAlloc)
{
    AllocTestPrms params = GetParam();
    int ret = AllocMemTest(params);
    if((ret == DISPLAY_SUCCESS) || (ret == DISPLAY_FAILURE) || (ret == DISPLAY_NOMEM)){
        EXPECT_TRUE(true);
    }else{
        EXPECT_TRUE(false);
    }
}
INSTANTIATE_TEST_SUITE_P(AllocTest, GrallocAllocTest, ::testing::ValuesIn(GRALLOC_TEST_SETS));

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_001)
{
    uint32_t devId = 0xffffff;
    uint32_t level = 0x7FFFFFFF;
    ret=HdiDisplay::SetDisplayBacklight(devId, level);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}


TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_002)
{
    uint32_t devId = 0xffffff;
    ret = HdiDisplay::SetDisplayClientCrop(devId, nullptr);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_003)
{
    uint32_t devId = 0xffffff;
    ret = HdiDisplay::SetDisplayClientDestRect(devId, nullptr);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_004)
{
    uint32_t devId = UINT32_MAX;
    uint32_t modeId = 0;
    ret = HdiDisplay::SetDisplayMode(devId, modeId);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_005)
{
    uint32_t devId = 0xffffff;
    ret = HdiDisplay::SetDisplayPowerStatus(devId, DispPowerStatus::POWER_STATUS_ON);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_006)
{
    uint32_t devId = 0xffffff;
    uint32_t level = 0x7FFFFFFF;
    ret = HdiDisplay::GetDisplayBacklight(devId, level);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}


TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_007)
{
    uint32_t devId = 0xffffff;
    uint32_t id = 0;
    uint64_t value = 1000;
    ret = HdiDisplay::SetDisplayProperty(devId, id, value);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_008)
{
    uint32_t devId = 0xffffff;
    ret = HdiDisplay::DestroyVirtualDisplay(devId);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_009)
{
    uint32_t devId = 0xffffff;
    DispPowerStatus status = DispPowerStatus::POWER_STATUS_OFF;
    ret = HdiDisplay::GetDisplayPowerStatus(devId, status);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_010)
{
    uint32_t devId = 0xffffff;
    uint32_t id = 0;
    uint64_t value = 1000;
    ret = HdiDisplay::GetDisplayProperty(devId, id, value);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_011)
{
    uint32_t devId = 0;
    uint32_t fence = -1;
    ret = HdiDisplay::SetVirtualDisplayBuffer(devId, 0, fence);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_012)
{
    uint32_t devId = 0xffffff;
    uint32_t layerId = 0x7FFFFFFF;
    ret = HdiDisplay::DestroyLayer(devId, layerId);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_013)
{
    uint32_t request = 0xffffff;
    ret = HdiDisplay::InitCmdRequest(request);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_014)
{
    uint32_t width = 1080;
    uint32_t height = 1080;
    uint32_t format = PIXEL_FMT_YCRCB_422_P;
    uint32_t devId = 0x7FFFFFFF;
    ret = HdiDisplay::CreateVirtualDisplay(width, height, format, devId);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_015)
{
    uint32_t inEleCnt = 0xffffff;
    uint32_t inFds = 0x7FFFFFFF;
    uint32_t outEleCnt = 0x7FFFFFFF;
    ret = HdiDisplay::CmdRequest(inEleCnt, inFds, outEleCnt);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_016)
{
    uint32_t inEleCnt = 0xffffff;
    uint32_t inFds = 0xffffff;
    uint32_t outEleCnt = 0xffffff;
    ret = HdiDisplay::CmdRequest(inEleCnt, inFds, outEleCnt);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_017)
{
    uint32_t reply = 0xffffff;
    ret = HdiDisplay::GetCmdReply(reply);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}

TEST_F(HdiDisplayTest, SUB_DriverSystem_DisplayHdi_018)
{
    uint32_t devId = 0x7FFFFFFF;
    uint32_t outputId = 0xffffff;
    bool connected = 0x7FFFFFFF;
    unsigned int sequence = 0;
    uint64_t ns =0;
    ret = DisplayRegisterCallbackBase::OnHotplugIn(outputId, connected);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
    ret = DisplayRegisterCallbackBase::OnHotplugIn(sequence, ns);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
    ret = DisplayRegisterCallbackBase::OnRefreshCallback(devId);
    EXPECT_EQ(DISPLAY_NOT_SUPPORT, ret);  
}
} // OHOS
} // HDI
} // DISPLAY
} // TEST