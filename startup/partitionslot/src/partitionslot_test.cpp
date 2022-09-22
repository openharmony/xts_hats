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

#include <gtest/gtest.h>
#include <osal_mem.h>
#include <unistd.h>
#include "hdf_log.h"
#include "v1_0/ipartition_slot.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS::HDI::Partitionslot::V1_0;

class StartupPartitionSlotTest : public testing::Test {
   public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp() {}
    void TearDown() {}
};

/**
 * @tc.number: StartupPartitionSlotTest_001
 * @tc.name: testGetCurrentSlotFun001
 * @tc.desc: NA
 */
HWTEST_F(StartupPartitionSlotTest, StartupPartitionSlotTest_001, Function | MediumTest | Level1) {
    printf("begin get currentslot by service \n");
    int numOfSlots = 0;
    int currentSlot = -1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: StartupPartitionSlotTest_002
 * @tc.name: testGetSlotSuffixFun001
 * @tc.desc: NA
 */
HWTEST_F(StartupPartitionSlotTest, StartupPartitionSlotTest_002, Function | MediumTest | Level1) {
    printf("begin get suffix by service \n");
    std::string suffix = "";
    int slot = 2;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetSlotSuffix(slot, suffix) == 0);
}

/**
 * @tc.number: StartupPartitionSlotTest_003
 * @tc.name: testGetCurrentSlotFun001
 * @tc.desc: NA
 */
HWTEST_F(StartupPartitionSlotTest, StartupPartitionSlotTest_003, Function | MediumTest | Level1) {
    printf("begin set active slot by service \n");
    int numOfSlots = 0;
    int currentSlot = 0;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    partitionslot->GetCurrentSlot(currentSlot, numOfSlots);
    ASSERT_TRUE(partitionslot->SetActiveSlot(2) == 0);
    partitionslot->SetActiveSlot(currentSlot);
}

/**
 * @tc.number: StartupPartitionSlotTest_004
 * @tc.name: testSetSlotUnbootableFun001
 * @tc.desc: NA
 */
HWTEST_F(StartupPartitionSlotTest, StartupPartitionSlotTest_004, Function | MediumTest | Level1) {
    printf("begin set unbootable slot by service \n");
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->SetSlotUnbootable(2) == 0);
}
