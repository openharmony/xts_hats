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

#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "osal_time.h"
#include "bluetooth_audio_session_service.h"
#include "bluetooth_hci_callback_impl.h"

using namespace OHOS::HDI::Bluetooth::A2dp::V1_0;
using IHciInterfaceSession = OHOS::HDI::Bluetooth::A2dp::V1_0::IBluetoothAudioSession;
using IHciInterfaceType = OHOS::HDI::Bluetooth::A2dp::V1_0::BluetoothAudioStatus;
using OHOS::HDI::Bluetooth::A2dp::V1_0::IBluetoothAudioCallback;
using namespace testing::ext;

extern "C" IBluetoothAudioSession *BluetoothAudioSessionImplGetInstance(void)
{
    return new (std::nothrow) BluetoothAudioSessionService();
}

namespace {
    OHOS::sptr<IHciInterfaceSession> g_iBtHci = nullptr;
    // OHOS::sptr<IBluetoothAudioCallback> g_hciCallbacks = nullptr;
    sptr<SeClientCallback> g_hciCallbacks = nullptr;
    
}

class HdfBluetoothHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfBluetoothHdiTest::SetUpTestCase()
{
    g_iBtHci = IBluetoothAudioSession::Get(true);
}
void HdfBluetoothHdiTest::TearDownTestCase()
{
}
void HdfBluetoothHdiTest::SetUp()
{
}
void HdfBluetoothHdiTest::TearDown()
{
}

/**
  * @tc.name: A2dpBluetoothSession_0100
  * @tc.desc: the bluetooth a2dp api StartSession test
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_A2dpBluetoothSession_0100, TestSize.Level2)
{
    g_hciCallbacks = new (std::nothrow) SeClientCallback();
    ASSERT_NE(nullptr, g_hciCallbacks);
    std::cout << "g_hciCallbacks = " << g_hciCallbacks << std::endl;
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci0100 = " << g_iBtHci << std::endl;
    SessionType sessionType = SessionType::SOFTWARE_ENCODING;
    std::shared_ptr<SharedMemQueue<uint8_t>> queue = nullptr;
    int32_t ret = g_iBtHci->StartSession(sessionType, g_hciCallbacks, queue);
    std::cout << "A2dpBluetoothSession_0100_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: A2dpBluetoothSession_0600
  * @tc.desc: the bluetooth a2dp api StopSession test
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_A2dpBluetoothSession_0600, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::UNKNOWN_TYPE;
    int32_t ret = g_iBtHci->StopSession(sessionType);
    std::cout << "A2dpBluetoothSession_0600_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: A2dpBluetoothSession_1100
  * @tc.desc: the bluetooth a2dp api RenderOperationResult test
  * @tc.type: FUNC
  */

HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_A2dpBluetoothSession_1100, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    Operation operation_render = Operation::SUSPEND_RENDER;
    Status statusSuss = Status::SUCCESS;
    int32_t ret = g_iBtHci->RenderOperationResult(operation_render, statusSuss);
    std::cout << "A2dpBluetoothSession_1100_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: A2dpBluetoothSession_1600
  * @tc.desc: the bluetooth a2dp enum types test
  * @tc.type: FUNC
  */

HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_A2dpBluetoothSession_1600, TestSize.Level2)
{
    Operation operation_suspend = Operation::SUSPEND_RENDER;
    std::cout << "Operation SUSPEND_RENDER = " << operation_suspend << std::endl;
    EXPECT_EQ(operation_suspend, 0);

    Operation operation_start = Operation::START_RENDER;
    std::cout << "Operation START_RENDER = " << operation_start << std::endl;
    EXPECT_EQ(operation_start, 1);

    Status staus_success = Status::SUCCESS;
    std::cout << "Status SUCCESS = " << staus_success << std::endl;
    EXPECT_EQ(staus_success, 0);

    Status staus_failure = Status::FAILURE;
    std::cout << "Status FAILURE = " << staus_failure << std::endl;
    EXPECT_EQ(staus_failure, 1);

    SessionType sessionType_unknownType = SessionType::UNKNOWN_TYPE;
    std::cout << "SessionType UNKNOWN_TYPE = " << sessionType_unknownType << std::endl;

    SessionType sessionType_softwareEncoding = SessionType::SOFTWARE_ENCODING;
    std::cout << "SessionType SOFTWARE_ENCODING = " << sessionType_softwareEncoding << std::endl;

    SessionType sessionType_hardwareEncoding = SessionType::HARDWARE_ENCODING;
    std::cout << "SessionType HARDWARE_ENCODING = " << sessionType_hardwareEncoding << std::endl;
}