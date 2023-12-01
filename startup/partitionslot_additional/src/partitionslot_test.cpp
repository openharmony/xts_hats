/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "hdf_log.h"
#include "idevmgr_hdi.h"
#include "v1_0/ipartition_slot.h"
#include <gtest/gtest.h>
#include <osal_mem.h>
#include <unistd.h>

using namespace testing;
using namespace testing::ext;
using namespace OHOS::HDI::Partitionslot::V1_0;
using OHOS::HDI::DeviceManager::V1_0::IDeviceManager;

class StartupPartitionSlotTestAdditional : public testing::Test {
public:
    static void SetUpTestCase()
    {
        auto devmgr = IDeviceManager::Get();
        if (devmgr != nullptr) {
            devmgr->LoadDevice("partition_slot_service");
        } else {
            std::cout << "Get devmgr failed" << std::endl;
        }
    }
    static void TearDownTestCase()
    {
        auto devmgr = IDeviceManager::Get();
        if (devmgr != nullptr) {
            devmgr->UnloadDevice("partition_slot_service");
        } else {
            std::cout << "Get devmgr failed" << std::endl;
        }
    }
    void SetUp() {}
    void TearDown() {}
};

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0100
 * @tc.name: testPartitionSlotGetCurrentSlot001
 * @tc.desc: GetCurrentSlot, numOfSlots is 1, currentSlo is 0
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0100,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 1;
    int currentSlot = 0;
    for (int i = 0; i < 100; i++) {
        sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
        ASSERT_TRUE(partitionslot != nullptr);
        ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
    }
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0200
 * @tc.name: testPartitionSlotGetCurrentSlot002
 * @tc.desc: GetCurrentSlot, numOfSlots is 0, currentSlo is 1
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0200,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 0;
    int currentSlot = 1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0300
 * @tc.name: testPartitionSlotGetCurrentSlot003
 * @tc.desc: GetCurrentSlot, numOfSlots is 1, currentSlo is -1
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0300,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 1;
    int currentSlot = -1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0400
 * @tc.name: testPartitionSlotGetCurrentSlot004
 * @tc.desc: GetCurrentSlot, numOfSlots is 2, currentSlo is -1
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0400,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 2;
    int currentSlot = -1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0500
 * @tc.name: testPartitionSlotGetCurrentSlot005
 * @tc.desc: GetCurrentSlot, numOfSlots is 2, currentSlo is 1
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0500,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 2;
    int currentSlot = 1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0600
 * @tc.name: testPartitionSlotGetCurrentSlot006
 * @tc.desc: GetCurrentSlot, numOfSlots is 3, currentSlo is -1
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0600,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 3;
    int currentSlot = -1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0700
 * @tc.name: testPartitionSlotGetCurrentSlot007
 * @tc.desc: GetCurrentSlot, numOfSlots is 3, currentSlo is 1
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0700,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 3;
    int currentSlot = 1;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0800
 * @tc.name: testPartitionSlotGetCurrentSlot008
 * @tc.desc: GetCurrentSlot, numOfSlots is 3, currentSlo is 2
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0800,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 3;
    int currentSlot = 2;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetCurrentSlot_0900
 * @tc.name: testPartitionSlotGetCurrentSlot009
 * @tc.desc: GetCurrentSlot, numOfSlots is 2, currentSlo is 3
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetCurrentSlot_0900,
         Function | MediumTest | Level1)
{
    std::cout << "begin get currentslot by service" << std::endl;
    int numOfSlots = 2;
    int currentSlot = 3;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetCurrentSlot(currentSlot, numOfSlots) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetSlotSuffix_0100
 * @tc.name: testPartitionSlotGetSlotSuffix001
 * @tc.desc: GetSlotSuffix, slot is 2147483647
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetSlotSuffix_0100,
         Function | MediumTest | Level1)
{
    std::cout << "begin get suffix by service" << std::endl;
    std::string suffix = "";
    int slot = 2147483647;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetSlotSuffix(slot, suffix) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetSlotSuffix_0200
 * @tc.name: testPartitionSlotGetSlotSuffix002
 * @tc.desc: GetSlotSuffix, slot is -2147483648
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetSlotSuffix_0200,
         Function | MediumTest | Level1)
{
    std::cout << "begin get suffix by service" << std::endl;
    std::string suffix = "";
    int slot = -2147483648;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->GetSlotSuffix(slot, suffix) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_GetSlotSuffix_0300
 * @tc.name: testPartitionSlotGetSlotSuffix003
 * @tc.desc: GetSlotSuffix, stability test
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_GetSlotSuffix_0300,
         Function | MediumTest | Level1)
{
    std::cout << "begin get suffix by service" << std::endl;
    std::string suffix = "";
    int slot = 2;
    for (int i = 0; i < 100; i++) {
        sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
        ASSERT_TRUE(partitionslot != nullptr);
        ASSERT_TRUE(partitionslot->GetSlotSuffix(slot, suffix) == 0);
    }
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_SetActiveSlot_0100
 * @tc.name: testPartitionSlotSetActiveSlot001
 * @tc.desc: SetActiveSlot, slot is 2147483647
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_SetActiveSlot_0100,
         Function | MediumTest | Level1)
{
    std::cout << "begin set active slot by service" << std::endl;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->SetActiveSlot(2147483647) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_SetActiveSlot_0200
 * @tc.name: testPartitionSlotSetActiveSlot002
 * @tc.desc: SetActiveSlot, slot is -2147483648
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_SetActiveSlot_0200,
         Function | MediumTest | Level1)
{
    std::cout << "begin set active slot by service" << std::endl;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->SetActiveSlot(-2147483648) != 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_SetActiveSlot_0300
 * @tc.name: testPartitionSlotSetActiveSlot003
 * @tc.desc: SetActiveSlot, stability test
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_SetActiveSlot_0300,
         Function | MediumTest | Level1)
{
    for (int i = 0; i < 100; i++) {
        std::cout << "begin set active slot by service" << std::endl;
        sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
        ASSERT_TRUE(partitionslot != nullptr);
        ASSERT_TRUE(partitionslot->SetActiveSlot(2) == 0);
    }
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_SetSlotUnbootable_0100
 * @tc.name: testPartitionSlotSetSlotUnbootable001
 * @tc.desc: SetSlotUnbootable, slot is 2147483647
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_SetSlotUnbootable_0100,
         Function | MediumTest | Level1)
{
    std::cout << "begin set unbootable slot by service" << std::endl;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->SetSlotUnbootable(2147483647) == 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_SetSlotUnbootable_0200
 * @tc.name: testPartitionSlotSetSlotUnbootable002
 * @tc.desc: SetSlotUnbootable, slot is -2147483648
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_SetSlotUnbootable_0200,
         Function | MediumTest | Level1)
{
    std::cout << "begin set unbootable slot by service" << std::endl;
    sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
    ASSERT_TRUE(partitionslot != nullptr);
    ASSERT_TRUE(partitionslot->SetSlotUnbootable(-2147483648) != 0);
}

/**
 * @tc.number: SUB_Startup_PartitionSlot_SetSlotUnbootable_0300
 * @tc.name: testPartitionSlotSetSlotUnbootable003
 * @tc.desc: SetSlotUnbootable, stability test
 */
HWTEST_F(StartupPartitionSlotTestAdditional, SUB_Startup_PartitionSlot_SetSlotUnbootable_0300,
         Function | MediumTest | Level1)
{
    for (int i = 0; i < 100; i++) {
        std::cout << "begin set unbootable slot by service" << std::endl;
        sptr<IPartitionSlot> partitionslot = IPartitionSlot::Get();
        ASSERT_TRUE(partitionslot != nullptr);
        ASSERT_TRUE(partitionslot->SetSlotUnbootable(2) == 0);
    }
}