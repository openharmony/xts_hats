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

#include "gtest/gtest.h"
#include "iostream"
#include "string_ex.h"
#include "iservice_registry.h"
#include "accesstoken_kit.h"
#include "token_setproc.h"
#include "nativetoken_kit.h"
#include "hilog_wrapper.h"
#include "driver_extension_controller.h"

namespace OHOS {
namespace ExternalDeviceManager {
using namespace OHOS;
using namespace std;
using namespace testing::ext;
class DrvExtCtrlTest : public testing::Test {
public:
    void SetUp() override
    {
        cout << "DrvExtCtrlTest SetUp" << endl;
    }
    void TearDown() override
    {
        cout << "DrvExtCtrlTest TearDown" << endl;
    }
    class TestConCb : public IDriverExtensionConnectCallback {
    public:
        TestConCb() { };
        int32_t OnConnectDone(const sptr<IRemoteObject> &remote, int resultCode) override
        {
            cout << "ConCb OnConnectDone, "<<  remote.GetRefPtr() << ", " << resultCode << endl;
            connectCount_++;
            return 0;
        };
        int32_t OnDisconnectDone(int resultCode) override
        {
            cout << "ConCb OnDisconnectDone, " << resultCode << endl;
            disconnectCount_++;
            return 0;
        }
        int connectCount_ = 0;
        int disconnectCount_ = 0;
    };
};

constexpr const char *TEST_BUNDLE_NAME = "com.usb.right";
constexpr const char *TEST_ABILITY_NAME = "UsbServiceExtAbility";
constexpr const char *TEST_ABILITY_NAME_ERR = "XXX";
constexpr const int MAX_WAIT_FOR_CONNECT = 10;

using namespace OHOS::Security::AccessToken;

TokenInfoParams g_normalInfoInstance = {
    .dcapsNum = 0,
    .permsNum = 0,
    .aclsNum = 0,
    .dcaps = nullptr,
    .perms = nullptr,
    .acls = nullptr,
    .processName = "usb_manager",
    .aplStr = "normal",
};

TokenInfoParams g_sysInfoInstance = {
    .dcapsNum = 0,
    .permsNum = 0,
    .aclsNum = 0,
    .dcaps = nullptr,
    .perms = nullptr,
    .acls = nullptr,
    .processName = "usb_manager",
    .aplStr = "system_basic",
};

class AccessTokenTest {
public:
    static void SetTestCaseNative(TokenInfoParams *infoInstance)
    {
        uint64_t tokenId = GetAccessTokenId(infoInstance);
        int ret = SetSelfTokenID(tokenId);
        if (ret == 0) {
            EDM_LOGI(MODULE_EA_MGR, "SetSelfTokenID success");
        } else {
            EDM_LOGE(MODULE_EA_MGR, "SetSelfTokenID fail");
        }
        AccessTokenKit::ReloadNativeTokenInfo();
    }
};

/**
 * @tc.number: SUB_Driver_Ext_LifeCycle_0500
 * @tc.name: DrvExtCtrlWithSATest
 * @tc.desc: Successfully called the Start Stop interface when testing with SA permission
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DrvExtCtrlTest, SUB_Driver_Ext_LifeCycle_0500, TestSize.Level1)
{
    AccessTokenTest::SetTestCaseNative(&g_sysInfoInstance);
    int ret = 0;
    auto &ctrl = DriverExtensionController::GetInstance();
    ret = ctrl.StartDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME);
    ASSERT_EQ(ret, 0);
    ret = ctrl.StopDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME);
    ASSERT_EQ(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Ext_LifeCycle_0600
 * @tc.name: DrvExtCtrlWithoutSATest
 * @tc.desc: Call to Start Stop interface failed when testing without SA permission
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DrvExtCtrlTest, SUB_Driver_Ext_LifeCycle_0600, TestSize.Level1)
{
    AccessTokenTest::SetTestCaseNative(&g_normalInfoInstance);
    int ret = 0;
    auto &ctrl = DriverExtensionController::GetInstance();
    ret = ctrl.StartDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME_ERR);
    ASSERT_NE(ret, 0);
    ret = ctrl.StopDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME_ERR);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Ext_LifeCycle_0700
 * @tc.name: DrvExtCtrlConnectTest0
 * @tc.desc: When testing with SA permissions, Connect&Disconnect successfully called
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DrvExtCtrlTest, SUB_Driver_Ext_LifeCycle_0700, TestSize.Level1)
{
    AccessTokenTest::SetTestCaseNative(&g_sysInfoInstance);
    int ret = 0;
    bool isTimeout = true;
    auto &ctrl = DriverExtensionController::GetInstance();
    auto con = make_shared<TestConCb>();
    ret = ctrl.ConnectDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME, con);
    ASSERT_EQ(ret, 0);
    isTimeout = true;
    for (int i = 0; i < MAX_WAIT_FOR_CONNECT; i++) {
        sleep(1);
        if (con->IsConnectDone()) {
            isTimeout = false;
            break;
        }
    }
    ASSERT_EQ(con->connectCount_, 1);
    ASSERT_EQ(isTimeout, false);
    ret = ctrl.DisconnectDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME, con);
    ASSERT_EQ(ret, 0);
    isTimeout = true;
    for (int i = 0; i < MAX_WAIT_FOR_CONNECT; i++) {
        sleep(1);
        if (!con->IsConnectDone()) {
            isTimeout = false;
            break;
        }
    }
    ASSERT_EQ(isTimeout, false);
    ASSERT_EQ(con->disconnectCount_, 1);
}

/**
 * @tc.number: SUB_Driver_Ext_LifeCycle_0800
 * @tc.name: DrvExtCtrlConnectTest1
 * @tc.desc: When testing for SA permissions, an error was entered and the registration for
 * Connect&Disconnect failed to call
 * @tc.size: MediumTest
 * @tc.type: Function
 */
HWTEST_F(DrvExtCtrlTest, SUB_Driver_Ext_LifeCycle_0800, TestSize.Level1)
{
    AccessTokenTest::SetTestCaseNative(&g_sysInfoInstance);
    int ret = 0;
    bool isTimeout = true;
    auto &ctrl = DriverExtensionController::GetInstance();
    auto conErr = make_shared<TestConCb>();
    ret = ctrl.ConnectDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME_ERR, conErr);
    ASSERT_NE(ret, 0);

    auto con = make_shared<TestConCb>();
    ret = ctrl.ConnectDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME, con);
    ASSERT_EQ(ret, 0);

    isTimeout = true;
    for (int i = 0; i < MAX_WAIT_FOR_CONNECT; i++) {
        sleep(1);
        if (con->IsConnectDone()) {
            isTimeout = false;
            break;
        }
    }
    ASSERT_EQ(isTimeout, false);
    ret = ctrl.DisconnectDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME_ERR, con);
    ASSERT_NE(ret, 0);
    ret = ctrl.DisconnectDriverExtension(TEST_BUNDLE_NAME, TEST_ABILITY_NAME, con);
    ASSERT_EQ(ret, 0);
    isTimeout = true;
    for (int i = 0; i < MAX_WAIT_FOR_CONNECT; i++) {
        sleep(1);
        if (!con->IsConnectDone()) {
            isTimeout = false;
            break;
        }
    }
    ASSERT_EQ(isTimeout, false);
}
}
}