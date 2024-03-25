/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "bluetooth_audio_session_service.h"
#include "bluetooth_hci_callback_impl.h"
#include "hdf_base.h"
#include "osal_time.h"
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include <unistd.h>

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

} // namespace

class HdfBluetoothHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfBluetoothHdiTestAdditional::SetUpTestCase() { g_iBtHci = IBluetoothAudioSession::Get(true); }
void HdfBluetoothHdiTestAdditional::TearDownTestCase() {}
void HdfBluetoothHdiTestAdditional::SetUp() {}
void HdfBluetoothHdiTestAdditional::TearDown() {}

/**
 * @tc.number: SUB_Communication_Bluetooth_StartSession_0100
 * @tc.name: testStartSession001
 * @tc.desc: StartSession, sessionType is UNKNOWN_TYPE, callback is normal
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStartSession001, Function | MediumTest | Level1)
{
    g_hciCallbacks = new (std::nothrow) SeClientCallback();
    ASSERT_NE(nullptr, g_hciCallbacks);
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::UNKNOWN_TYPE;
    std::shared_ptr<SharedMemQueue<uint8_t>> queue = nullptr;
    int32_t ret = g_iBtHci->StartSession(sessionType, g_hciCallbacks, queue);
    EXPECT_NE(HDF_SUCCESS, ret);
    g_iBtHci->StopSession(sessionType);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StartSession_0300
 * @tc.name: testStartSession003
 * @tc.desc: StartSession, sessionType is UNKNOWN_TYPE, callback is nullptr
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStartSession003, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::UNKNOWN_TYPE;
    std::shared_ptr<SharedMemQueue<uint8_t>> queue = nullptr;
    int32_t ret = g_iBtHci->StartSession(sessionType, nullptr, queue);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StartSession_0400
 * @tc.name: testStartSession004
 * @tc.desc: StartSession, sessionType is SOFTWARE_ENCODING, callback is nullptr
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStartSession004, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::SOFTWARE_ENCODING;
    std::shared_ptr<SharedMemQueue<uint8_t>> queue = nullptr;
    int32_t ret = g_iBtHci->StartSession(sessionType, nullptr, queue);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StartSession_0500
 * @tc.name: testStartSession005
 * @tc.desc: StartSession, sessionType is HARDWARE_ENCODING, callback is nullptr
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStartSession005, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::HARDWARE_ENCODING;
    std::shared_ptr<SharedMemQueue<uint8_t>> queue = nullptr;
    int32_t ret = g_iBtHci->StartSession(sessionType, nullptr, queue);
    EXPECT_NE(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StartSession_0600
 * @tc.name: testStartSession006
 * @tc.desc: StartSession, stability test
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStartSession006, Function | MediumTest | Level1)
{
    g_hciCallbacks = new (std::nothrow) SeClientCallback();
    ASSERT_NE(nullptr, g_hciCallbacks);
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::SOFTWARE_ENCODING;
    std::shared_ptr<SharedMemQueue<uint8_t>> queue = nullptr;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_iBtHci->StartSession(sessionType, g_hciCallbacks, queue);
        EXPECT_EQ(HDF_SUCCESS, ret);
        g_iBtHci->StopSession(sessionType);
    }
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StopSession_0100
 * @tc.name: testStopSession001
 * @tc.desc: StopSession, sessionType is SOFTWARE_ENCODING
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStopSession001, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::SOFTWARE_ENCODING;
    int32_t ret = g_iBtHci->StopSession(sessionType);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StopSession_0200
 * @tc.name: testStopSession002
 * @tc.desc: StopSession, sessionType is HARDWARE_ENCODING
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStopSession002, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::HARDWARE_ENCODING;
    int32_t ret = g_iBtHci->StopSession(sessionType);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_StopSession_0300
 * @tc.name: testStopSession003
 * @tc.desc: StopSession, stability test
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testStopSession003, Function | MediumTest | Level1)
{
    ASSERT_NE(nullptr, g_iBtHci);
    SessionType sessionType = SessionType::HARDWARE_ENCODING;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_iBtHci->StopSession(sessionType);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Communication_Bluetooth_RenderOperationResult_0200
 * @tc.name: testRenderOperationResult002
 * @tc.desc: RenderOperationResult, Operation is START_RENDER, status is FAILURE
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testRenderOperationResult002, Function | MediumTest | Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    Operation operation_render = Operation::START_RENDER;
    Status status = Status::FAILURE;
    int32_t ret = g_iBtHci->RenderOperationResult(operation_render, status);
    EXPECT_NE(HDF_SUCCESS, ret);
}