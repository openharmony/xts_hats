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

#include "hdi_death_test.h"
#include <chrono>
#include <cinttypes>
#include <algorithm>
#include "display_test.h"
#include "display_test_utils.h"
#include "hdi_test_device.h"
#include "hdi_test_device_common.h"
#include "hdi_test_display.h"

using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::TEST;
using namespace testing::ext;

static bool g_isServiceDead = false;

void ComposerDiedRecipient::OnRemoteDied(const wptr<IRemoteObject>& remote)
{
    if (remote == nullptr) {
        return;
    }
    EXPECT_EQ(g_isServiceDead, true);
}

/**
  * @tc.number: SUB_Driver_Display_Death_0300
  * @tc.name: test_AddDeathRecipient
  * @tc.desc: Composer_host adds death listening
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(DeathTest, SUB_Driver_Display_Death_0300, TestSize.Level1)
{
    displayComposer_ = Composer::V1_2::IDisplayComposerInterface::Get();
    ASSERT_TRUE(displayComposer_ != nullptr);
    sptr<IRemoteObject::DeathRecipient> recipient = new ComposerDiedRecipient();
    ASSERT_TRUE(recipient != nullptr);
    auto ret = displayComposer_->AddDeathRecipient(recipient);
    EXPECT_EQ(ret, true);
    g_isServiceDead = true;
    system("killall composer_host");
}

/**
  * @tc.number: SUB_Driver_Display_Death_0400
  * @tc.name: test_RemoveDeathRecipient
  * @tc.desc: Composer_host removes death listening
  * @tc.size: MediumTest
  * @tc.type: Function
  */
HWTEST_F(DeathTest, SUB_Driver_Display_Death_0400, TestSize.Level1)
{
    displayComposer_ = Composer::V1_2::IDisplayComposerInterface::Get();
    ASSERT_TRUE(displayComposer_ != nullptr);
    sptr<IRemoteObject::DeathRecipient> recipient = new ComposerDiedRecipient();
    ASSERT_TRUE(recipient != nullptr);
    auto ret = displayComposer_->AddDeathRecipient(recipient);
    EXPECT_EQ(ret, true);
    ret = displayComposer_->RemoveDeathRecipient();
    EXPECT_EQ(ret, true);
    g_isServiceDead = true;
    system("killall composer_host");
}