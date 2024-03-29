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

#include "bluetooth_hci_callback_impl.h"
#include "hci_interface_impl.h"
#include "hdf_base.h"
#include "osal_time.h"
#include "v1_0/ihci_interface.h"
#include "vendor_interface.h"
#include <cmath>
#include <cstdio>
#include <gtest/gtest.h>
#include <securec.h>
#include <unistd.h>

using namespace OHOS::HDI::Bluetooth::Hci::V1_0;
using namespace testing::ext;
#define COMMAND_HCI_SHOULD_BE_UNKNOWN                                                                                  \
    {                                                                                                                  \
        0xff, 0x3B, 0x08, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07                                               \
    }
#define COMMAND_HCI_READ_LOCAL_VERSION_INFORMATION                                                                     \
    {                                                                                                                  \
        0x01, 0x10, 0x00                                                                                               \
    }
#define COMMAND_HCI_READ_BUFFER_SIZE                                                                                   \
    {                                                                                                                  \
        0x05, 0x10, 0x00                                                                                               \
    }
#define COMMAND_HCI_RESET                                                                                              \
    {                                                                                                                  \
        0x03, 0x0c, 0x00                                                                                               \
    }
#define COMMAND_HCI_WRITE_LOCAL_NAME                                                                                   \
    {                                                                                                                  \
        0x13, 0x0c, 0xf8                                                                                               \
    }
#define ACL_BROADCAST_POINT_TO_POINT                                                                                   \
    {                                                                                                                  \
        0x13, 0x0c, 0xf8                                                                                               \
    }
#define ACL_PACKET_BOUNDARY_FIRST_AUTO_FLUSHABLE                                                                       \
    {                                                                                                                  \
        0x13, 0x0c, 0xf8                                                                                               \
    }
namespace {
sptr<IHciInterface> g_iBtHci = nullptr;
sptr<HciCallbackImpl> g_hciCallbacks = nullptr;
constexpr int32_t SENSOR_WAIT_TIME = 1000;

} // namespace
class HdfBluetoothHdiTestAdditional : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void HdfBluetoothHdiTestAdditional::SetUpTestCase()
{
    g_iBtHci = IHciInterface::Get();
    std::cout << "g_iBtHci = " << g_iBtHci << std::endl;
    ASSERT_NE(nullptr, g_iBtHci);
}
void HdfBluetoothHdiTestAdditional::TearDownTestCase() {}
void HdfBluetoothHdiTestAdditional::SetUp() {}
void HdfBluetoothHdiTestAdditional::TearDown() {}

/**
 * @tc.number: SUB_Communication_Bluetooth_SendHciPacket_0100
 * @tc.name: testSendHciPacket001
 * @tc.desc: SendHciPacket, stability test
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testSendHciPacket001, Function | MediumTest | Level1)
{
    g_hciCallbacks = new (std::nothrow) HciCallbackImpl();
    ASSERT_NE(nullptr, g_hciCallbacks);
    int32_t ret = g_iBtHci->Init(g_hciCallbacks);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalMSleep(SENSOR_WAIT_TIME);

    int n = 0;
    std::vector<uint8_t> acl_vector;
    acl_vector.push_back(static_cast<uint8_t>(0xff));
    acl_vector.push_back(static_cast<uint8_t>((0x0f00) >> 8));
    acl_vector.push_back(static_cast<uint8_t>(0xff));
    acl_vector.push_back(static_cast<uint8_t>((0xff00) >> 8));
    for (size_t i = 0; i < 10; i++) {
        acl_vector.push_back(static_cast<uint8_t>(i + n));
    }
    BtType btType = BtType::ACL_DATA;
    ret = -1;
    for (int i = 0; i < 100; i++) {
        ret = g_iBtHci->SendHciPacket(btType, acl_vector);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }

    ret = g_iBtHci->Close();
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Communication_Bluetooth_Init_0100
 * @tc.name: testInit001
 * @tc.desc: Init, callback is nullptr
 */
HWTEST_F(HdfBluetoothHdiTestAdditional, testInit001, Function | MediumTest | Level2)
{
    int32_t ret = g_iBtHci->Init(nullptr);
    EXPECT_NE(HDF_SUCCESS, ret);

    ret = g_iBtHci->Close();
    EXPECT_EQ(HDF_SUCCESS, ret);
}
