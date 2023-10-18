/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include <iostream>

#include "hdf_log.h"
#include "if_system_ability_manager.h"
#include "system_ability_definition.h"
#include "usbd_function_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

constexpr int32_t SLEEP_TIME = 3;
constexpr int32_t USB_FUNCTION_INVALID = -1;
constexpr int32_t USB_PORT_ID_INVALID = 2;
constexpr int32_t USB_POWER_ROLE_INVALID = 4;
constexpr int32_t USB_DATA_ROLE_INVALID = 5;
constexpr int32_t USB_FUNCTION_UNSUPPORTED = 128;

using namespace testing::ext;
using namespace OHOS;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

void UsbdFunctionTest::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdFunctionTest::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }
}

void UsbdFunctionTest::TearDownTestCase(void) {}

void UsbdFunctionTest::SetUp(void) {}

void UsbdFunctionTest::TearDown(void) {}

/**
 * @tc.name: SUB_USB_HDI_1250
 * @tc.desc: Test functions to GetCurrentFunctions
 * @tc.desc: int32_t GetCurrentFunctions(int32_t &funcs);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1250, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->GetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1250 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1260
 * @tc.desc: Test functions to GetCurrentFunctions
 * @tc.desc: int32_t GetCurrentFunctions(int32_t &funcs);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1260, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ACM);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1270 %{public}d SetCurrentFunctions=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
    int32_t funcs = USB_FUNCTION_NONE;
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1260 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**********************************************************************************************************/

/**
 * @tc.name: SUB_USB_HDI_1270
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1270, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ACM);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1270 %{public}d SetCurrentFunctions=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1280
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Negative test: parameters exception, Funcs error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1280, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_INVALID);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1280 %{public}d, ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}
/**
 * @tc.name: SUB_USB_HDI_1290
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1290, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ECM);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1290 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1300
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1300, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ACM | USB_FUNCTION_ECM;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1300 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1310
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1310, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_HDC);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1310 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1320
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1320, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ACM | USB_FUNCTION_HDC;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1320 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1330
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1330, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ECM | USB_FUNCTION_HDC;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1330 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1340
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1340, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_RNDIS);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1340 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2340
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2340, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_STORAGE);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2340 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2350
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2350, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_RNDIS | USB_FUNCTION_HDC;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2350 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2360
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2360, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_STORAGE | USB_FUNCTION_HDC;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2360 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2700
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2700, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_MTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2700 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2710
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2710, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_PTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2710 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2720
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2720, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_MTP | USB_FUNCTION_HDC;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2720 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2730
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2730, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_PTP | USB_FUNCTION_HDC;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2730 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2740
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2740, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_MTP | USB_FUNCTION_RNDIS;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2740 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2750
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2750, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_PTP | USB_FUNCTION_RNDIS;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2750 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2370
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Negative test: parameters exception, funcs error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2370, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_UNSUPPORTED);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2370 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_2380
 * @tc.desc: Test functions to SetCurrentFunctions
 * @tc.desc: int32_t SetCurrentFunctions(int32_t funcs)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_2380, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_2380 ret=%{public}d", ret);
    ASSERT_EQ(0, ret);
    HDF_LOGI("UsbdFunctionTest::the function was set to none successfully");
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_HDC);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1350
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1350, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1350 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1360
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, portId error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1360, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1360 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1370
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, powerRole error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1370, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, USB_POWER_ROLE_INVALID, DATA_ROLE_DEVICE);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1370 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1380
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, dataRole error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1380, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, USB_DATA_ROLE_INVALID);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1380 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1390
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, powerRole error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1390, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, USB_POWER_ROLE_INVALID, DATA_ROLE_HOST);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1390 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1400
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, portId && dataRole error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1400, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, POWER_ROLE_SOURCE, USB_DATA_ROLE_INVALID);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1400 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1410
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, powerRole && dataRole error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1410, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, USB_POWER_ROLE_INVALID, USB_DATA_ROLE_INVALID);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1410 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1420
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Negative test: parameters exception, portId && powerRole && dataRole error
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1420, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, USB_DATA_ROLE_INVALID);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1420 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.name: SUB_USB_HDI_1430
 * @tc.desc: Test functions to SetPortRole
 * @tc.desc: int32_t SetPortRole(int32_t portId,int32_t powerRole,int32_t dataRole)
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1430, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1430 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.name: SUB_USB_HDI_1700
 * @tc.desc: Test functions to QueryPort
 * @tc.desc: int32_t QueryPort(int32_t &portId, int32_t &powerRole, int32_t &dataRole, int32_t &mode);
 * @tc.desc: Positive test: parameters correctly
 * @tc.type: FUNC
 */
HWTEST_F(UsbdFunctionTest, SUB_USB_HDI_1700, Function | MediumTest | Level1)
{
    int32_t portId = DEFAULT_PORT_ID;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    HDF_LOGI("UsbdFunctionTest::SUB_USB_HDI_1700 %{public}d ret=%{public}d", __LINE__, ret);
    ASSERT_EQ(0, ret);
}
} // namespace
