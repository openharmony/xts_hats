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

namespace {
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
}

namespace {
int ALIGN_UP(int x, int a)
{
    return ((x + (a-1)) / a) * a;
}
const int WIDTH_ALIGN = 8U;
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
        .expectStride = 4352,
        .expectSize = 8355840
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
        .expectStride = 4352,
        .expectSize = 8355840
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
        .expectStride = 4352,
        .expectSize = 8355840
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
        .expectStride = 4352,
        .expectSize = 8355840
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
        .expectStride = 4352, // expectStride
        .expectSize = 8355840 // expectSize
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
        .expectStride = 4352, // expectStride
        .expectSize = 8355840 // expectSize
    },
};

static bool CheckBufferHandle(AllocTestPrms &info, BufferHandle &buffer)
{
    if (buffer.stride != (ALIGN_UP(info.expectStride, WIDTH_ALIGN))) {
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
    ASSERT_TRUE(ret == DISPLAY_SUCCESS);
}
INSTANTIATE_TEST_SUITE_P(AllocTest, GrallocAllocTest, ::testing::ValuesIn(GRALLOC_TEST_SETS));
}