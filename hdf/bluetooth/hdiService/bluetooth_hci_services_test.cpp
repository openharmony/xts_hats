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

namespace {
    sptr<IHciInterface>  g_iBtHci = nullptr;
    sptr<HciCallbackImpl> g_hciCallbacks = nullptr;
   
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
    if (g_hciCallbacks == nullptr) {
        return;
    }

    int32_t ret = g_iBtHci->Init(g_hciCallbacks);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: HdiBluetoothGet_0200
  * @tc.desc: Get IHciInterface and check whether is empty.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothGet_0200, TestSize.Level2)
{
    ASSERT_NE(nullptr, g_iBtHci);
}

/**
  * @tc.name: HdiBluetoothSend_0300
  * @tc.desc: the HCI ACL_DATA packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0300, TestSize.Level1)
{
    if (g_iBtHci == nullptr) {
        ASSERT_NE(nullptr, g_iBtHci);
        return;
    }
    BtType btType = BtType::ACL_DATA;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0400
  * @tc.desc: the HCI HCI_CMD packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0400, TestSize.Level1)
{
    if (g_iBtHci == nullptr) {
        ASSERT_NE(nullptr, g_iBtHci);
        return;
    }
    BtType btType = BtType::HCI_CMD;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0500
  * @tc.desc: the HCI SCO_DATA packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0500, TestSize.Level1)
{
    if (g_iBtHci == nullptr) {
        ASSERT_NE(nullptr, g_iBtHci);
        return;
    }
    BtType btType = BtType::SCO_DATA;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0600
  * @tc.desc: the HCI HCI_EVENT packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0600, TestSize.Level1)
{
    if (g_iBtHci == nullptr) {
        ASSERT_NE(nullptr, g_iBtHci);
        return;
    }
    BtType btType = BtType::HCI_EVENT;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothSend_0700
  * @tc.desc: the HCI ISO_DATA packets is sent successfully.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothSend_0700, TestSize.Level1)
{
    if (g_iBtHci == nullptr) {
        ASSERT_NE(nullptr, g_iBtHci);
        return;
    }
    BtType btType = BtType::ISO_DATA;
    std::vector<uint8_t> data;
    int32_t ret = g_iBtHci->SendHciPacket(btType, data);
    EXPECT_EQ(HDF_FAILURE, ret);
}

/**
  * @tc.name: HdiBluetoothClose_0800
  * @tc.desc: Disable the HCI interface.
  * @tc.type: FUNC
  */
HWTEST_F(HdfBluetoothHdiTest, SUB_DriverSystem_HdiBluetoothClose_0800, TestSize.Level1)
{
    if (g_iBtHci == nullptr) {
        ASSERT_NE(nullptr, g_iBtHci);
        return;
    }
    int32_t ret = g_iBtHci->Close();
    EXPECT_EQ(HDF_SUCCESS, ret);
}

