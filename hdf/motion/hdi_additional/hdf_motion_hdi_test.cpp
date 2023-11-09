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

#include "hdf_base.h"
#include "motion_callback_impl.h"
#include "osal_time.h"
#include "v1_1/imotion_interface.h"
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include <unistd.h>
#include <vector>

using namespace OHOS::HDI::Motion::V1_1;
using namespace testing::ext;

namespace {
sptr<OHOS::HDI::Motion::V1_1::IMotionInterface> g_motionInterface = nullptr;
sptr<IMotionCallback> g_motionCallback = new MotionCallbackImpl();
sptr<IMotionCallback> g_motionCallbackUnregistered = new MotionCallbackImpl();
} // namespace

class HdfMotionTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void HdfMotionTestAdditional::SetUpTestCase() { g_motionInterface = OHOS::HDI::Motion::V1_1::IMotionInterface::Get(); }

void HdfMotionTestAdditional::TearDownTestCase() {}

void HdfMotionTestAdditional::SetUp() {}

void HdfMotionTestAdditional::TearDown() {}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0100
 * @tc.name   : testHdiEnableMotion001
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion001, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_CLOSE_TO_EAR);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0200
 * @tc.name   : testHdiEnableMotion002
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion002, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_POCKET_MODE);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0300
 * @tc.name   : testHdiEnableMotion003
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion003, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_LEAVE_EAR);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0400
 * @tc.name   : testHdiEnableMotion004
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion004, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_WRIST_UP);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0500
 * @tc.name   : testHdiEnableMotion005
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion005, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }
    int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_WRIST_DOWN);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0600
 * @tc.name   : testHdiEnableMotion006
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion006, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    for (size_t i = 0; i < 1000; i++) {
        int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_CLOSE_TO_EAR);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0700
 * @tc.name   : testHdiEnableMotion007
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion007, Function | MediumTest | Level2)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->EnableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_MAX);
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0800
 * @tc.name   : testHdiEnableMotion008
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion008, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->EnableMotion(0);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdienablemotion_0900
 * @tc.name   : testHdiEnableMotion009
 * @tc.desc   : Testing the effectiveness of the EnableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiEnableMotion009, Function | MediumTest | Level2)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->EnableMotion(-1);
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0100
 * @tc.name   : testHdiDisableMotion001
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion001, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_CLOSE_TO_EAR);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0200
 * @tc.name   : testHdiDisableMotion002
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion002, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_POCKET_MODE);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0300
 * @tc.name   : testHdiDisableMotion003
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion003, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_LEAVE_EAR);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0400
 * @tc.name   : testHdiDisableMotion004
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion004, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_WRIST_UP);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0500
 * @tc.name   : testHdiDisableMotion005
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion005, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_WRIST_DOWN);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0600
 * @tc.name   : testHdiDisableMotion006
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion006, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    for (size_t i = 0; i < 1000; i++) {
        int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_CLOSE_TO_EAR);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0700
 * @tc.name   : testHdiDisableMotion007
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion007, Function | MediumTest | Level2)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(OHOS::HDI::Motion::V1_1::HDF_MOTION_TYPE_MAX);
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0800
 * @tc.name   : testHdiDisableMotion008
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion008, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(0);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdidisablemotion_0900
 * @tc.name   : testHdiDisableMotion009
 * @tc.desc   : Testing the effectiveness of the DisableMotion function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiDisableMotion009, Function | MediumTest | Level2)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->DisableMotion(-1);
    EXPECT_EQ(HDF_ERR_INVALID_PARAM, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdiregister_0100
 * @tc.name   : testHdiRegister001
 * @tc.desc   : Testing the effectiveness of the Register function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiRegister001, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdiregister_0200
 * @tc.name   : testHdiRegister002
 * @tc.desc   : Testing the effectiveness of the Register function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiRegister002, Function | MediumTest | Level2)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdiregister_0300
 * @tc.name   : testHdiRegister003
 * @tc.desc   : Testing the effectiveness of the Register function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiRegister003, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    for (size_t i = 0; i < 1000; i++) {
        int32_t ret = g_motionInterface->Register(g_motionCallback);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = g_motionInterface->Unregister(g_motionCallback);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number : SUB_Driver_Motion_Hdiunregister_0100
 * @tc.name   : testHdiUnregister001
 * @tc.desc   : Testing the effectiveness of the Unregister function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiUnregister001, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_motionInterface->Unregister(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdiunregister_0200
 * @tc.name   : testHdiUnregister002
 * @tc.desc   : Testing the effectiveness of the Unregister function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiUnregister002, Function | MediumTest | Level2)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    int32_t ret = g_motionInterface->Register(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_motionInterface->Unregister(g_motionCallback);
    EXPECT_EQ(HDF_SUCCESS, ret);

    ret = g_motionInterface->Unregister(g_motionCallback);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
 * @tc.number : SUB_Driver_Motion_Hdiunregister_0300
 * @tc.name   : testHdiUnregister003
 * @tc.desc   : Testing the effectiveness of the Unregister function
 */
HWTEST_F(HdfMotionTestAdditional, testHdiUnregister003, Function | MediumTest | Level1)
{
    if (g_motionInterface == nullptr) {
        ASSERT_NE(nullptr, g_motionInterface);
        return;
    }

    for (size_t i = 0; i < 1000; i++) {
        int32_t ret = g_motionInterface->Register(g_motionCallback);
        EXPECT_EQ(HDF_SUCCESS, ret);

        ret = g_motionInterface->Unregister(g_motionCallback);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}