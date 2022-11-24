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
#include "v1_0/ihci_interface.h"
#include "bluetooth_hci_callback_impl.h"
#include "hci_interface_impl.h"
#include "vendor_interface.h"

using namespace OHOS::HDI::Bluetooth::Hci::V1_0;
using namespace testing::ext;
#define COMMAND_HCI_SHOULD_BE_UNKNOWN \
    { 0xff, 0x3B, 0x08, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }
#define COMMAND_HCI_READ_LOCAL_VERSION_INFORMATION \
    { 0x01, 0x10, 0x00 }
#define COMMAND_HCI_READ_BUFFER_SIZE \
    { 0x05, 0x10, 0x00 }
#define COMMAND_HCI_RESET \
    { 0x03, 0x0c, 0x00 }
#define COMMAND_HCI_WRITE_LOCAL_NAME \
    { 0x13, 0x0c, 0xf8 }
#define ACL_BROADCAST_POINT_TO_POINT \
    { 0x13, 0x0c, 0xf8 }
#define ACL_PACKET_BOUNDARY_FIRST_AUTO_FLUSHABLE \
    { 0x13, 0x0c, 0xf8 }
namespace {
    sptr<IHciInterface>  g_iBtHci = nullptr;
    sptr<HciCallbackImpl> g_hciCallbacks = nullptr;
    constexpr int32_t SENSOR_WAIT_TIME = 1000;
   
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
    g_iBtHci = IHciInterface::Get();
    std::cout << "g_iBtHci = " << g_iBtHci << std::endl;
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
  * @tc.name: HdiBluetoothInit_0100
  * @tc.desc: Returns 0 if the callback is successfully Init.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothInit_0100, TestSize.Level1)
{
    g_hciCallbacks = new (std::nothrow) HciCallbackImpl();
    ASSERT_NE(nullptr, g_hciCallbacks);
    std::cout << "g_hciCallbacks = " << g_hciCallbacks << std::endl;
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci0100 = " << g_iBtHci << std::endl;
    int32_t ret = g_iBtHci->Init(g_hciCallbacks);
    EXPECT_EQ(HDF_SUCCESS, ret);
    OsalMSleep(SENSOR_WAIT_TIME);
}

/**
  * @tc.name: HdiBluetoothSend_0100
  * @tc.desc: the HCI ACL_DATA packets is sent success.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0100, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci002 = " << g_iBtHci << std::endl;
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
    int32_t ret = g_iBtHci->SendHciPacket(btType, acl_vector);
    std::cout << "HdiBluetoothSend_0100_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0200
  * @tc.desc: the HCI null ACL_DATA packets is sent.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0200, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci003 = " << g_iBtHci << std::endl;
    BtType btType = BtType::ACL_DATA;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    std::cout << "HdiBluetoothSend_0200_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0300
  * @tc.desc: the reset HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0300, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci004 = " << g_iBtHci << std::endl;
    std::vector<uint8_t> cmd = COMMAND_HCI_RESET;
    BtType btType = BtType::HCI_CMD;
    int32_t ret = g_iBtHci->SendHciPacket(btType, cmd);
    std::cout << "HdiBluetoothSend_0300_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0400
  * @tc.desc: the version HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0400, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci005 = " << g_iBtHci << std::endl;
    std::vector<uint8_t> cmd = COMMAND_HCI_READ_LOCAL_VERSION_INFORMATION;
    BtType btType = BtType::HCI_CMD;
    int32_t ret = g_iBtHci->SendHciPacket(btType, cmd);
    std::cout << "HdiBluetoothSend_0400_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0500
  * @tc.desc: the unkonwn HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0500, TestSize.Level2)
{
   ASSERT_NE(nullptr, g_iBtHci);
   std::cout << "g_iBtHci006 = " << g_iBtHci << std::endl;
    std::vector<uint8_t> cmd = COMMAND_HCI_SHOULD_BE_UNKNOWN;
    BtType btType = BtType::HCI_CMD;
    int32_t ret = g_iBtHci->SendHciPacket(btType, cmd);
    std::cout << "HdiBluetoothSend_0500_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0600
  * @tc.desc: the buffer HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0600, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci007 = " << g_iBtHci << std::endl;
    std::vector<uint8_t> cmd = COMMAND_HCI_READ_BUFFER_SIZE;
    BtType btType = BtType::HCI_CMD;
    int32_t ret = g_iBtHci->SendHciPacket(btType, cmd);
    std::cout << "HdiBluetoothSend_0600_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0700
  * @tc.desc: the name HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0700, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci008 = " << g_iBtHci << std::endl;
     // Send an HCI packet
    std::vector<uint8_t> writeName = COMMAND_HCI_WRITE_LOCAL_NAME;
    // With a name
    char newName[] = "John Jacob Jingleheimer Schmidt ___________________0";
    size_t newNameLength = strlen(newName);
    for (size_t i = 0; i < newNameLength; i++)
        writeName.push_back(static_cast<uint8_t>(newName[i]));
    // And the packet number
    size_t i = newNameLength - 1;
    size_t n = newNameLength;
    for (int digits = n; digits > 0; digits = digits / 10, i--)
        writeName[i] = static_cast<uint8_t>('0' + digits % 10);
    // And padding
    for (size_t i = 0; i < 248 - newNameLength; i++)
        writeName.push_back(static_cast<uint8_t>(0));
    std::vector<uint8_t> cmd = writeName;
    BtType btType = BtType::HCI_CMD;
    int32_t ret = g_iBtHci->SendHciPacket(btType, cmd);
    std::cout << "HdiBluetoothSend_0700_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0800
  * uint16_t handle;
  * size_t size;@tc.desc: the HCI null HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0800, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci009 = " << g_iBtHci << std::endl;
    BtType btType = BtType::HCI_CMD;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    std::cout << "HdiBluetoothSend_0800_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0900
  * @tc.desc: the HCI SCO_DATA packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0900, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci100 = " << g_iBtHci << std::endl;
    int n = 0;
    std::vector<uint8_t> sco_vector;
    sco_vector.push_back(static_cast<uint8_t>(0xff));
    sco_vector.push_back(static_cast<uint8_t>((0x0f00) >> 8));
    sco_vector.push_back(static_cast<uint8_t>(0xff));
    sco_vector.push_back(static_cast<uint8_t>((0xff00) >> 8));
    for (size_t i = 0; i < 10; i++) {
        sco_vector.push_back(static_cast<uint8_t>(i + n));
    }
    BtType btType = BtType::SCO_DATA;
    int32_t ret = g_iBtHci->SendHciPacket(btType, sco_vector);
    std::cout << "HdiBluetoothSend_0900_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothSend_1000
  * @tc.desc: the HCI SCO_DATA packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_1000, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci110 = " << g_iBtHci << std::endl;
    BtType btType = BtType::SCO_DATA;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    std::cout << "HdiBluetoothSend_1000_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_1100
  * @tc.desc: the HCI HCI_EVENT packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_1100, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci120 = " << g_iBtHci << std::endl;
    BtType btType = BtType::HCI_EVENT;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    std::cout << "HdiBluetoothSend_1100_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_1200
  * @tc.desc: the HCI ISO_DATA packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_1200, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci130 = " << g_iBtHci << std::endl;
    BtType btType = BtType::ISO_DATA;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    std::cout << "HdiBluetoothSend_1200_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothClose_0800
  * @tc.desc: Disable the HCI interface.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothClose_0100, TestSize.Level3)
{
    ASSERT_NE(nullptr, g_iBtHci);
    std::cout << "g_iBtHci140 = " << g_iBtHci << std::endl;
    int32_t ret = g_iBtHci->Close();
    std::cout << "diBluetoothClose_0100_ret = " << ret << std::endl;
    EXPECT_EQ(HDF_SUCCESS, ret);
}

