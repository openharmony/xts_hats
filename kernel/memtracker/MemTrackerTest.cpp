/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "gtest/gtest.h"

#define private public
#define protected public
#include <v1_0/imemory_tracker_interface.h>
#include "hdf_base.h"
#undef private
#undef protected

namespace OHOS {
namespace Memory {
using namespace testing;
using namespace testing::ext;
using namespace OHOS::HDI::MemoryTracker::V1_0;

class MemTrackerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void MemTrackerTest::SetUpTestCase()
{
}

void MemTrackerTest::TearDownTestCase()
{
}

void MemTrackerTest::SetUp()
{
}

void MemTrackerTest::TearDown()
{
}
/**
 * @tc.number SUB_KERNEL_Get_MemoryTest_0101
 * @tc.name   Unable to obtain related services
 * @tc.desc   [C- SOFTWARE -0200]
 */
HWTEST_F(MemTrackerTest, GetMemoryTest_01, TestSize.Level1)
{
    printf("begin call memtrack by service \n");
    sptr<IMemoryTrackerInterface> memtrack = IMemoryTrackerInterface::Get();
    if (memtrack == nullptr) {
        printf("memtrack service is null \n");
        return;
    }

    std::vector<MemoryRecord> records;
    int errCode = memtrack->GetDevMem(0, MEMORY_TRACKER_TYPE_GL, records);
    ASSERT_EQ(errCode, HDF_SUCCESS);
}
/**
 * @tc.number SUB_KERNEL_Get_MemoryTest_0102
 * @tc.name   Obtain relevant services and input legal parameters
 * @tc.desc   [C- SOFTWARE -0200]
 */
HWTEST_F(MemTrackerTest, GetMemoryTest_02, TestSize.Level1)
{
    printf("begin call memtrack passthrough \n");
    sptr<IMemoryTrackerInterface> memtrack = IMemoryTrackerInterface::Get(true);
    if (memtrack == nullptr) {
        printf("memtrack service is null \n");
        return;
    }

    std::vector<MemoryRecord> records;
    int errCode = memtrack->GetDevMem(0, MEMORY_TRACKER_TYPE_GL, records);
    ASSERT_EQ(errCode, HDF_SUCCESS);
}
/**
 * @tc.number SUB_KERNEL_Get_MemoryTest_0103
 * @tc.name   replace MemoryTrackerType is MEMORY_TRACKER_TYPE_CAM
 * @tc.desc   [C- SOFTWARE -0200]
 */
HWTEST_F(MemTrackerTest, GetMemoryTest_03, TestSize.Level1)
{
    printf("begin call memtrack passthrough \n");
    sptr<IMemoryTrackerInterface> memtrack = IMemoryTrackerInterface::Get(true);
    if (memtrack == nullptr) {
        printf("memtrack service is null \n");
        return;
    }

    std::vector<MemoryRecord> records;
    int errCode = memtrack->GetDevMem(0, MEMORY_TRACKER_TYPE_CAM, records);
    ASSERT_EQ(errCode, HDF_SUCCESS);
}

/**
 * @tc.number SUB_KERNEL_Get_MemoryTest_0104
 * @tc.name   replace MemoryTrackerType is MEMORY_TRACKER_TYPE_MM
 * @tc.desc   [C- SOFTWARE -0200]
 */
HWTEST_F(MemTrackerTest, GetMemoryTest_04, TestSize.Level1)
{
    printf("begin call memtrack passthrough \n");
    sptr<IMemoryTrackerInterface> memtrack = IMemoryTrackerInterface::Get(true);
    if (memtrack == nullptr) {
        printf("memtrack service is null \n");
        return;
    }

    std::vector<MemoryRecord> records;
    int errCode = memtrack->GetDevMem(0, MEMORY_TRACKER_TYPE_MM, records);
    ASSERT_EQ(errCode, HDF_SUCCESS);
}

/**
 * @tc.number SUB_KERNEL_Get_MemoryTest_0105
 * @tc.name   replace MemoryTrackerType is MEMORY_TRACKER_TYPE_GRAPH
 * @tc.desc   [C- SOFTWARE -0200]
 */
HWTEST_F(MemTrackerTest, GetMemoryTest_05, TestSize.Level1)
{
    printf("begin call memtrack passthrough \n");
    sptr<IMemoryTrackerInterface> memtrack = IMemoryTrackerInterface::Get(true);
    if (memtrack == nullptr) {
        printf("memtrack service is null \n");
        return;
    }

    std::vector<MemoryRecord> records;
    int errCode = memtrack->GetDevMem(0, MEMORY_TRACKER_TYPE_GRAPH, records);
    ASSERT_EQ(errCode, HDF_SUCCESS);
}

/**
 * @tc.number SUB_KERNEL_Get_MemoryTest_0106
 * @tc.name   replace MemoryTrackerType is MEMORY_TRACKER_TYPE_OTHER
 * @tc.desc   [C- SOFTWARE -0200]
 */
HWTEST_F(MemTrackerTest, GetMemoryTest_06, TestSize.Level1)
{
    printf("begin call memtrack passthrough \n");
    sptr<IMemoryTrackerInterface> memtrack = IMemoryTrackerInterface::Get(true);
    if (memtrack == nullptr) {
        printf("memtrack service is null \n");
        return;
    }

    std::vector<MemoryRecord> records;
    int errCode = memtrack->GetDevMem(0, MEMORY_TRACKER_TYPE_OTHER, records);
    ASSERT_EQ(errCode, HDF_SUCCESS);
}
}
}
