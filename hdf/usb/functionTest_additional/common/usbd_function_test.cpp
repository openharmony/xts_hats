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

#include <iostream>

#include "hdf_log.h"
#include "if_system_ability_manager.h"
#include "system_ability_definition.h"
#include "usbd_function_test.h"
#include "v1_0/iusb_interface.h"
#include "v1_0/usb_types.h"

constexpr int32_t SLEEP_TIME = 3;
constexpr int32_t USB_PORT_ID_INVALID = 2;
constexpr int32_t USB_POWER_ROLE_INVALID = 4;
constexpr int32_t USB_DATA_ROLE_INVALID = 5;
using namespace testing::ext;
using namespace OHOS;
using namespace std;
using namespace OHOS::HDI::Usb::V1_0;

namespace {
sptr<IUsbInterface> g_usbInterface = nullptr;

int32_t SwitchErrCode(int32_t ret)
{
    return ret == HDF_ERR_NOT_SUPPORT ? HDF_SUCCESS : ret;
}

void UsbdFunctionTestAdditional::SetUpTestCase(void)
{
    g_usbInterface = IUsbInterface::Get();
    if (g_usbInterface == nullptr) {
        HDF_LOGE("%{public}s:IUsbInterface::Get() failed.", __func__);
        exit(0);
    }
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    sleep(SLEEP_TIME);
    HDF_LOGI("UsbdFunctionTestAdditional::[Device] %{public}d SetPortRole=%{public}d", __LINE__, ret);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    if (ret != 0) {
        exit(0);
    }
}

void UsbdFunctionTestAdditional::TearDownTestCase(void) {}

void UsbdFunctionTestAdditional::SetUp(void) {}

void UsbdFunctionTestAdditional::TearDown(void) {}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3100
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions012
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_RNDIS | USB_FUNCTION_STORAGE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions012, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_RNDIS | USB_FUNCTION_STORAGE;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2000
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions001
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ACM | USB_FUNCTION_MTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions001, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ACM | USB_FUNCTION_MTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2100
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions002
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ACM | USB_FUNCTION_PTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions002, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ACM | USB_FUNCTION_PTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2200
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions003
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ACM | USB_FUNCTION_RNDIS.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions003, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ACM | USB_FUNCTION_RNDIS;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2300
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions004
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ACM | USB_FUNCTION_STORAGE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions004, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ACM | USB_FUNCTION_STORAGE;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2400
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions005
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ECM | USB_FUNCTION_MTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions005, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ECM | USB_FUNCTION_MTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2500
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions006
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ECM | USB_FUNCTION_PTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions006, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ECM | USB_FUNCTION_PTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2600
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions007
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ECM | USB_FUNCTION_RNDIS.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions007, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ECM | USB_FUNCTION_RNDIS;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2700
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions008
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_ECM | USB_FUNCTION_STORAGE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions008, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_ECM | USB_FUNCTION_STORAGE;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2800
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions009
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_MTP | USB_FUNCTION_PTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions009, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_MTP | USB_FUNCTION_PTP;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_2900
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions010
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_MTP | USB_FUNCTION_STORAGE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions010, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_MTP | USB_FUNCTION_STORAGE;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3000
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions011
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * funcs = USB_FUNCTION_PTP | USB_FUNCTION_STORAGE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions011, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_PTP | USB_FUNCTION_STORAGE;
    auto ret = g_usbInterface->SetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3200
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions013
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * USB_FUNCTION_NONE,USB_FUNCTION_ACM.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions013, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ACM);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3300
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions014
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * USB_FUNCTION_NONE,USB_FUNCTION_ECM.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions014, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ECM);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3400
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions015
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * USB_FUNCTION_NONE,USB_FUNCTION_MTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions015, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_MTP);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3500
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions016
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * USB_FUNCTION_NONE,USB_FUNCTION_PTP.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions016, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_PTP);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3600
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions017
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * USB_FUNCTION_NONE,USB_FUNCTION_RNDIS.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions017, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_RNDIS);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetCurrentFunctions_3700
 * @tc.name: testHdiUsbFunctionTestSetCurrentFunctions018
 * @tc.desc: Sets the list of functions (represented by bit field) supported by the current device.
 * USB_FUNCTION_NONE,USB_FUNCTION_STORAGE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetCurrentFunctions018, Function | MediumTest | Level1)
{
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_NONE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_STORAGE);
    ASSERT_EQ(0, ret);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_0400
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions001
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_ACM Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions001, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ACM);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_ACM, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_0500
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions002
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_ECM Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions002, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_ECM);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_ECM, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_0600
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions003
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_HDC Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions003, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_HDC);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_HDC, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_0700
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions004
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_MTP Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions004, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_MTP);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_MTP, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_0800
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions005
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_PTP Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions005, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_PTP);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_PTP, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_0900
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions006
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_RNDIS Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions006, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_RNDIS);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_RNDIS, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_GetCurrentFunctions_1000
 * @tc.name: testHdiUsbFunctionTestGetCurrentFunctions007
 * @tc.desc: Obtains the list of functions (represented by bit field) supported by the current device.
 * Set USB_FUNCTION_STORAGE Check if the Get is correct.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestGetCurrentFunctions007, Function | MediumTest | Level1)
{
    int32_t funcs = USB_FUNCTION_NONE;
    auto ret = g_usbInterface->SetCurrentFunctions(USB_FUNCTION_STORAGE);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->GetCurrentFunctions(funcs);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(USB_FUNCTION_STORAGE, funcs);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1000
 * @tc.name: testHdiUsbFunctionTestSetPortRole001
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are
 * (USB_PORT_ID_INVALID, POWER_ROLE_SINK, DATA_ROLE_DEVICE).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole001, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1100
 * @tc.name: testHdiUsbFunctionTestSetPortRole002
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are (DEFAULT_PORT_ID,
 * USB_POWER_ROLE_INVALID, DATA_ROLE_NONE).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole002, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, USB_POWER_ROLE_INVALID, DATA_ROLE_NONE);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1200
 * @tc.name: testHdiUsbFunctionTestSetPortRole003
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are (DEFAULT_PORT_ID,
 * POWER_ROLE_SINK, USB_DATA_ROLE_INVALID).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole003, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, USB_DATA_ROLE_INVALID);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1300
 * @tc.name: testHdiUsbFunctionTestSetPortRole004
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are (DEFAULT_PORT_ID,
 * POWER_ROLE_NONE, USB_DATA_ROLE_INVALID).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole004, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_NONE, USB_DATA_ROLE_INVALID);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1400
 * @tc.name: testHdiUsbFunctionTestSetPortRole005
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are
 * (USB_PORT_ID_INVALID, POWER_ROLE_SINK, USB_DATA_ROLE_INVALID).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole005, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, POWER_ROLE_SINK, USB_DATA_ROLE_INVALID);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1500
 * @tc.name: testHdiUsbFunctionTestSetPortRole006
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are
 * (USB_PORT_ID_INVALID, POWER_ROLE_NONE, USB_DATA_ROLE_INVALID).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole006, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, POWER_ROLE_NONE, USB_DATA_ROLE_INVALID);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1600
 * @tc.name: testHdiUsbFunctionTestSetPortRole007
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are
 * (USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, DATA_ROLE_NONE).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole007, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, DATA_ROLE_NONE);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}
/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1700
 * @tc.name: testHdiUsbFunctionTestSetPortRole008
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are
 * (USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, DATA_ROLE_HOST).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole008, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, DATA_ROLE_HOST);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_SetPortRole_1800
 * @tc.name: testHdiUsbFunctionTestSetPortRole009
 * @tc.desc: Closes a USB device to release all system resources related to the device.The entries are
 * (USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, DATA_ROLE_DEVICE).
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestSetPortRole009, Function | MediumTest | Level2)
{
    auto ret = g_usbInterface->SetPortRole(USB_PORT_ID_INVALID, USB_POWER_ROLE_INVALID, DATA_ROLE_DEVICE);
    ret = SwitchErrCode(ret);
    ASSERT_NE(ret, 0);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_QueryPort_0200
 * @tc.name: testHdiUsbFunctionTestQueryPort001
 * @tc.desc: Queries the current settings of a port.To see if portId is equal to DEFAULT_PORT_ID.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestQueryPort001, Function | MediumTest | Level1)
{
    int32_t portId = 0;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(DEFAULT_PORT_ID, portId);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_QueryPort_0300
 * @tc.name: testHdiUsbFunctionTestQueryPort002
 * @tc.desc: Queries the current settings of a port.To see if powerRole is equal to POWER_ROLE_SINK.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestQueryPort002, Function | MediumTest | Level1)
{
    int32_t portId = DEFAULT_PORT_ID;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(POWER_ROLE_SINK, powerRole);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_QueryPort_0400
 * @tc.name: testHdiUsbFunctionTestQueryPort003
 * @tc.desc: Queries the current settings of a port.To see if dataRole is equal to DATA_ROLE_DEVICE.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestQueryPort003, Function | MediumTest | Level1)
{
    int32_t portId = DEFAULT_PORT_ID;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SINK, DATA_ROLE_DEVICE);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(DATA_ROLE_DEVICE, dataRole);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_QueryPort_0500
 * @tc.name: testHdiUsbFunctionTestQueryPort004
 * @tc.desc: Queries the current settings of a port.To see if portId is equal to DEFAULT_PORT_ID.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestQueryPort004, Function | MediumTest | Level1)
{
    int32_t portId = 0;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    ret = SwitchErrCode(ret);
    ASSERT_EQ(0, ret);
    ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
    ASSERT_EQ(0, ret);
    ASSERT_EQ(DEFAULT_PORT_ID, portId);
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_QueryPort_0600
 * @tc.name: testHdiUsbFunctionTestQueryPort005
 * @tc.desc: Queries the current settings of a port.To see if powerRole is equal to DATA_ROLE_HOST.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestQueryPort005, Function | MediumTest | Level1)
{
    int32_t portId = DEFAULT_PORT_ID;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    if (ret == HDF_SUCCESS) {
        ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
        ASSERT_EQ(0, ret);
        ASSERT_EQ(POWER_ROLE_SOURCE, powerRole);
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
        ASSERT_EQ(0, ret);
        ASSERT_EQ(POWER_ROLE_SINK, powerRole);
    }
}

/**
 * @tc.number: SUB_Driver_Usb_FunctionTest_QueryPort_0700
 * @tc.name: testHdiUsbFunctionTestQueryPort006
 * @tc.desc: Queries the current settings of a port.To see if dataRole is equal to DATA_ROLE_HOST.
 */
HWTEST_F(UsbdFunctionTestAdditional, testHdiUsbFunctionTestQueryPort006, Function | MediumTest | Level1)
{
    int32_t portId = DEFAULT_PORT_ID;
    int32_t powerRole = POWER_ROLE_NONE;
    int32_t dataRole = DATA_ROLE_NONE;
    int32_t mode = PORT_MODE_NONE;
    auto ret = g_usbInterface->SetPortRole(DEFAULT_PORT_ID, POWER_ROLE_SOURCE, DATA_ROLE_HOST);
    if (ret == HDF_SUCCESS) {
        ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
        ASSERT_EQ(0, ret);
        ASSERT_EQ(DATA_ROLE_HOST, dataRole);
    } else if (ret == HDF_ERR_NOT_SUPPORT) {
        ret = g_usbInterface->QueryPort(portId, powerRole, dataRole, mode);
        ASSERT_EQ(0, ret);
        ASSERT_EQ(DATA_ROLE_DEVICE, dataRole);
    }
}

} // namespace
