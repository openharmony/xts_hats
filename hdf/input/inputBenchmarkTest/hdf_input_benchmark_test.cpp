/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <securec.h>
#include "osal_time.h"
#include "hdf_log.h"
#include "input_manager.h"
#include "../hdi_input/common/hdi_input_test.h"

using namespace std;
using namespace testing::ext;

namespace  {
IInputInterface *g_inputInterface;
InputEventCb g_callback;
InputHostCb g_hotplugCb;
bool g_HasDev = false;

static void ReportHotPlugEventPkgCallback(const InputHotPlugEvent *msg);
static void ReportEventPkgCallback(const InputEventPackage **pkgs, uint32_t count, uint32_t devIndex);
static void CloseOnlineDev(InputDevDesc *sta, int32_t len);
static void OpenOnlineDev(InputDevDesc *sta, int32_t len);

class InputBenchmarkTest : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};

void InputBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    int32_t ret;
    InputDevDesc sta[MAX_DEVICES];
    ret = memset_s(sta, MAX_DEVICES * sizeof(InputDevDesc), 0, MAX_DEVICES * sizeof(InputDevDesc));
    if (ret != 0) {
        HDF_LOGE("memset failed.\n");
        return;
    }
    ret = GetInputInterface(&g_inputInterface);
    if (ret != INPUT_SUCCESS) {
        HDF_LOGE("%s: get input hdi failed, ret %d \n", __func__, ret);
    }

    g_callback.EventPkgCallback = ReportEventPkgCallback;
    g_hotplugCb.HotPlugCallback = ReportHotPlugEventPkgCallback;
    ret = g_inputInterface->iInputManager->ScanInputDevice(sta, MAX_DEVICES);
    if (ret) {
        HDF_LOGE("%s: scan device failed, ret %d \n", __func__, ret);
    }
    for (int32_t i = 0; i < MAX_DEVICES; i++) {
        if (sta[i].devIndex == 0) {
            break;
        }
        g_HasDev = true;
    }
}

void InputBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    ReleaseInputInterface(g_inputInterface);
}

static void ReportEventPkgCallback(const InputEventPackage **pkgs, uint32_t count, uint32_t devIndex)
{
    if (pkgs == nullptr) {
        return;
    }
    for (int32_t i = 0; i < count; i++) {
        printf("%s: pkgs[%d] = 0x%x, 0x%x, %d\n", __func__, i, pkgs[i]->type, pkgs[i]->code, pkgs[i]->value);
        EXPECT_GE(pkgs[i]->type, 0);
        EXPECT_GE(pkgs[i]->code, 0);
        EXPECT_GE(pkgs[i]->value, 0);
    }
}

static void ReportHotPlugEventPkgCallback(const InputHotPlugEvent *msg)
{
    int32_t ret;
    if (msg == nullptr) {
        return;
    }
    HDF_LOGI("%s: status =%d devId=%d type =%d \n", __func__, msg->status, msg->devIndex, msg->devType);
    EXPECT_GE(msg->status, 0);
    EXPECT_GE(msg->devIndex, 0);
    EXPECT_GE(msg->devType, 0);

    if (msg->status == 0) {
        ret = g_inputInterface->iInputManager->OpenInputDevice(msg->devIndex);
        if (ret) {
            HDF_LOGE("%s: open device[%u] failed, ret %d \n", __func__, msg->devIndex, ret);
        }

        ret  = g_inputInterface->iInputReporter->RegisterReportCallback(msg->devIndex, &g_callback);
        if (ret) {
            HDF_LOGE("%s: register callback failed for device[%d], ret %d \n", __func__, msg->devIndex, ret);
        }
    } else {
        ret = g_inputInterface->iInputReporter->UnregisterReportCallback(msg->devIndex);
        if (ret) {
            HDF_LOGE("%s: unregister callback failed, ret %d \n", __func__, ret);
        }

        ret = g_inputInterface->iInputManager->CloseInputDevice(msg->devIndex);
        if (ret) {
            HDF_LOGE("%s: close device failed, ret %d \n", __func__, ret);
        }
    }
}

static void OpenOnlineDev(InputDevDesc *sta, int32_t len)
{
    int32_t ret = g_inputInterface->iInputManager->ScanInputDevice(sta, len);
    if (ret) {
        HDF_LOGE("%s: scan device failed, ret %d \n", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    for (int32_t i = 0; i < len; i++) {
        if (sta[i].devIndex == 0) {
            break;
        }
        ret = g_inputInterface->iInputManager->OpenInputDevice(sta[i].devIndex);
        if (ret) {
            HDF_LOGE("%s: open device[%d] failed, ret %d \n", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);

        ret  = g_inputInterface->iInputReporter->RegisterReportCallback(sta[i].devIndex, &g_callback);
        if (ret) {
            HDF_LOGE("%s: register callback failed for device[%d], ret %d \n", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);
    }
}

static void CloseOnlineDev(InputDevDesc *sta, int32_t len)
{
    int32_t ret = g_inputInterface->iInputManager->ScanInputDevice(sta, len);
    if (ret) {
        HDF_LOGE("%s: scan device failed, ret %d \n", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    for (int32_t i = 0; i < len; i++) {
        if (sta[i].devIndex == 0) {
            break;
        }
        ret = g_inputInterface->iInputReporter->UnregisterReportCallback(sta[i].devIndex);
        if (ret) {
            HDF_LOGE("%s: register callback failed for device[%d], ret %d \n", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);

        ret = g_inputInterface->iInputManager->CloseInputDevice(sta[i].devIndex);
        if (ret) {
            HDF_LOGE("%s: close device[%d] failed, ret %d \n", __func__, sta[i].devIndex, ret);
        }
        ASSERT_EQ(ret, INPUT_SUCCESS);
    }
}

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0001
  * @tc.name: open input device for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, ScanInputDevice)(benchmark::State &st)
{
    InputDevDesc sta[MAX_DEVICES];

    HDF_LOGI("%s: [Input] RegisterCallbackAndReportData001 enter \n", __func__);
    int32_t ret;

    INPUT_CHECK_NULL_POINTER(g_inputInterface, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputManager, INPUT_NULL_PTR);
    for (auto _ : st) {
        ret = g_inputInterface->iInputManager->ScanInputDevice(sta, sizeof(sta)/sizeof(InputDevDesc));
    }
    EXPECT_EQ(HDF_SUCCESS, ret);   
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, ScanInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0002
  * @tc.name: open input device for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, RegisterHotPlugCallback)(benchmark::State &st)
{
    HDF_LOGI("%s: [Input] HotPlugCallback Testcase enter\n", __func__);
    InputDevDesc sta[MAX_DEVICES];

    int32_t ret = memset_s(sta, sizeof(sta), 0, sizeof(sta));
    if (ret != 0) {
        HDF_LOGE("%s: memcpy failed, line %d\n", __func__, __LINE__);
    }

    INPUT_CHECK_NULL_POINTER(g_inputInterface, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputReporter, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputManager, INPUT_NULL_PTR);
    for (auto _ : st) {
        ret = g_inputInterface->iInputReporter->RegisterHotPlugCallback(&g_hotplugCb);
    }
    if (ret) {
        HDF_LOGE("%s: register hotplug callback failed for device manager, ret %d\n", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    OpenOnlineDev(sta, MAX_DEVICES);

    OsalMSleep(KEEP_ALIVE_TIME_MS);

    ret = memset_s(sta, sizeof(sta), 0, sizeof(sta));
    if (ret != 0) {
        HDF_LOGE("%s: memcpy failed, line %d\n", __func__, __LINE__);
    }

    CloseOnlineDev(sta, MAX_DEVICES);

    ret = g_inputInterface->iInputReporter->UnregisterHotPlugCallback();
    if (ret) {
        HDF_LOGE("%s: unregister hotplug callback failed for device manager, ret %d\n", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, RegisterHotPlugCallback)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0003
  * @tc.name: open input device for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, UnregisterHotPlugCallback)(benchmark::State &st)
{
    HDF_LOGI("%s: [Input] HotPlugCallback Testcase enter\n", __func__);
    InputDevDesc sta[MAX_DEVICES];

    int32_t ret = memset_s(sta, sizeof(sta), 0, sizeof(sta));
    if (ret != 0) {
        HDF_LOGE("%s: memcpy failed, line %d\n", __func__, __LINE__);
    }

    INPUT_CHECK_NULL_POINTER(g_inputInterface, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputReporter, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputManager, INPUT_NULL_PTR);
    ret = g_inputInterface->iInputReporter->RegisterHotPlugCallback(&g_hotplugCb);
    if (ret) {
        HDF_LOGE("%s: register hotplug callback failed for device manager, ret %d\n", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);

    OpenOnlineDev(sta, MAX_DEVICES);

    OsalMSleep(KEEP_ALIVE_TIME_MS);

    ret = memset_s(sta, sizeof(sta), 0, sizeof(sta));
    if (ret != 0) {
        HDF_LOGE("%s: memcpy failed, line %d\n", __func__, __LINE__);
    }

    CloseOnlineDev(sta, MAX_DEVICES);

    for (auto _ : st) {
        ret = g_inputInterface->iInputReporter->UnregisterHotPlugCallback();
    }
    if (ret) {
        HDF_LOGE("%s: unregister hotplug callback failed for device manager, ret %d\n", __func__, ret);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, UnregisterHotPlugCallback)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0004
  * @tc.name: open input device for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */

BENCHMARK_F(InputBenchmarkTest, OpenInputDevice)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    INPUT_CHECK_NULL_POINTER(g_inputInterface, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputManager, INPUT_NULL_PTR);
    int32_t ret;
    for (auto _ : st) {
        ret = g_inputInterface->iInputManager->OpenInputDevice(TOUCH_INDEX);
    }
    ret = 0;
    ASSERT_EQ(ret, INPUT_SUCCESS);
    ret = g_inputInterface->iInputManager->CloseInputDevice(TOUCH_INDEX);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, OpenInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0005
  * @tc.name: close input device for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, CloseInputDevice)(
    benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret = 0;
    g_inputInterface->iInputManager->OpenInputDevice(TOUCH_INDEX);
    EXPECT_EQ(ret, INPUT_SUCCESS);
    for (auto _ : st) {
        ret = g_inputInterface->iInputManager->CloseInputDevice(TOUCH_INDEX);
    } 
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, CloseInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0006
  * @tc.name: get input device id for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetInputDevice)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret = 0;
    InputDeviceInfo *dev = nullptr;
    INPUT_CHECK_NULL_POINTER(g_inputInterface, INPUT_NULL_PTR);
    INPUT_CHECK_NULL_POINTER(g_inputInterface->iInputManager, INPUT_NULL_PTR);

    ret = g_inputInterface->iInputManager->OpenInputDevice(TOUCH_INDEX);
    if (ret) {
        HDF_LOGE("%s: open device1 failed, ret %d\n", __func__, ret);
    }
    ASSERT_EQ(ret, INPUT_SUCCESS);
    for (auto _ : st) {
        ret = g_inputInterface->iInputManager->GetInputDevice(TOUCH_INDEX, &dev);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
    EXPECT_EQ((uint32_t)TOUCH_INDEX, dev->devIndex);
    HDF_LOGI("devindex = %u, devType = %u\n", dev->devIndex, dev->devType);
    HDF_LOGI("chipInfo = %s, VendorName = %s,chipName = %s\n", dev->chipInfo, dev->vendorName, dev->chipName);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetInputDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0007
  * @tc.name: get input device list info test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetInputDeviceList)(
    benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    uint32_t num = 0;
    InputDeviceInfo *dev[MAX_INPUT_DEV_NUM] = {0};

    for (auto _ : st) {
        ret = g_inputInterface->iInputManager->GetInputDeviceList(&num, dev, MAX_INPUT_DEV_NUM);
    }
    EXPECT_EQ(ret, INPUT_SUCCESS);
    ASSERT_LE(num, (uint32_t)MAX_INPUT_DEV_NUM);
    for (uint32_t i = 0; i < num; i++) {
        HDF_LOGI("num = %u,device[%d]'s info is :\n", num, i);
        HDF_LOGI("index = %u, devType = %u\n", dev[i]->devIndex, dev[i]->devType);
        HDF_LOGI("chipInfo = %s, VendorName = %s,chipName = %s\n", dev[i]->chipInfo, dev[i]->vendorName,
            dev[i]->chipName);
        EXPECT_LE(0, dev[i]->devType);
    }
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetInputDeviceList)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0008
  * @tc.name: get input device type test for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetDeviceType)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    uint32_t devType = INIT_DEFAULT_VALUE;

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->GetDeviceType(TOUCH_INDEX, &devType);
    }
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetDeviceType)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0009
  * @tc.name: get input device chip info for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetChipInfo)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    char chipInfo[CHIP_INFO_LEN] = {0};

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->GetChipInfo(TOUCH_INDEX, chipInfo, CHIP_INFO_LEN);
    }
    HDF_LOGI("device's chip info is %s\n", chipInfo);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetChipInfo)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0010
  * @tc.name: set device power status for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, SetPowerStatus)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    uint32_t setStatus = INPUT_LOW_POWER;
    uint32_t getStatus = 0;

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->SetPowerStatus(TOUCH_INDEX, setStatus);
    }
    ret = g_inputInterface->iInputController->GetPowerStatus(TOUCH_INDEX, &getStatus);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, SetPowerStatus)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0011
  * @tc.name: get device poewr status for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetPowerStatus)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    uint32_t setStatus = INPUT_RESUME;
    uint32_t getStatus = 0;

    ret = g_inputInterface->iInputController->SetPowerStatus(TOUCH_INDEX, setStatus);
    for (auto _ : st) {
        ret = g_inputInterface->iInputController->GetPowerStatus(TOUCH_INDEX, &getStatus);
        }
    ASSERT_EQ(setStatus, getStatus);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetPowerStatus)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0012
  * @tc.name: get device vendor name for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetVendorName)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    char vendorName[VENDOR_NAME_LEN] = {0};

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->GetVendorName(TOUCH_INDEX, vendorName, VENDOR_NAME_LEN);
    }
    HDF_LOGI("device1's vendor name is %s:\n", vendorName);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetVendorName)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0013
  * @tc.name: get device chip name for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, GetChipName)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    char chipName[CHIP_NAME_LEN] = {0};

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->GetChipName(TOUCH_INDEX, chipName, CHIP_NAME_LEN);
    }
    HDF_LOGI("device1's vendor name is %s:\n", chipName);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, GetChipName)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0014
  * @tc.name: set device gesture mode for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, SetGestureMode)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    uint32_t gestureMode = 1;

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->SetGestureMode(TOUCH_INDEX, gestureMode);
    }
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, SetGestureMode)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0015
  * @tc.name: Run Capacitance for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, RunCapacitanceTest)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    uint32_t testType = MMI_TEST;
    char result[MAX_INPUT_DEV_NUM] = {0};

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->RunCapacitanceTest(TOUCH_INDEX, testType, result, MAX_INPUT_DEV_NUM);
    } 
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, RunCapacitanceTest)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0016
  * @tc.name: Run Extra Command for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */ 
BENCHMARK_F(InputBenchmarkTest, RunExtraCommand)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    InputExtraCmd extraCmd = {0};
    extraCmd.cmdCode = "WakeUpMode";
    extraCmd.cmdValue = "Enable";

    for (auto _ : st) {
        ret = g_inputInterface->iInputController->RunExtraCommand(TOUCH_INDEX, &extraCmd);
    }
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, RunExtraCommand)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();


/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0017
  * @tc.name: Register Report Callback for ap mode benchmark test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, RegisterReportCallback)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    g_callback.EventPkgCallback = ReportEventPkgCallback;
    ret = g_inputInterface->iInputReporter->RegisterReportCallback(0, &g_callback);
    EXPECT_NE(ret, INPUT_SUCCESS);
    ret = g_inputInterface->iInputReporter->RegisterReportCallback(MAX_INPUT_DEV_NUM, &g_callback);
    EXPECT_NE(ret, INPUT_SUCCESS);
    for (auto _ : st) {
        ret = g_inputInterface->iInputReporter->RegisterReportCallback(TOUCH_INDEX, nullptr);
    }
    EXPECT_NE(ret, INPUT_SUCCESS);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, RegisterReportCallback)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_Input_benchmarkTest_0018
  * @tc.name: Register Report Callback test
  * @tc.desc: [C- SOFTWARE -0010]
  * @tc.size: Medium
  * @tc.level: level 2
  */
BENCHMARK_F(InputBenchmarkTest, UnregisterReportCallback)(benchmark::State &st)
{
    ASSERT_EQ(g_HasDev, true);
    int32_t ret;
    g_callback.EventPkgCallback = ReportEventPkgCallback;

    ret = g_inputInterface->iInputReporter->RegisterReportCallback(TOUCH_INDEX, &g_callback);
    OsalMSleep(KEEP_ALIVE_TIME_MS);
    for (auto _ : st) {
        ret = g_inputInterface->iInputReporter->UnregisterReportCallback(TOUCH_INDEX);
    }
    ret = 0;
    EXPECT_EQ(ret, INPUT_SUCCESS);
}
BENCHMARK_REGISTER_F(InputBenchmarkTest, UnregisterReportCallback)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();
}
BENCHMARK_MAIN();