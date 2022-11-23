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
#include <benchmark/benchmark.h>
#include <string>
#include <vector>

#include <functional>
#include <gtest/gtest.h>
#include <hdf_io_service_if.h>
#include <hdf_log.h>
#include <base/hdi_smq.h>
#include <idevmgr_hdi.h>
#include <iostream>
#include <ipc_object_stub.h>
#include <iservmgr_hdi.h>
#include <osal_time.h>
#include <string>

#include "sample_hdi.h"

#define HDF_LOG_TAG service_manager_test_cpp

using namespace std;
using namespace testing::ext;
using OHOS::IRemoteObject;
using OHOS::sptr;
using OHOS::HDI::Base::SharedMemQueue;
using OHOS::HDI::Base::SharedMemQueueMeta;
using OHOS::HDI::Base::SmqType;
using OHOS::HDI::DeviceManager::V1_0::IDeviceManager;
using OHOS::HDI::ServiceManager::V1_0::IServiceManager;
using OHOS::HDI::ServiceManager::V1_0::IServStatListener;
using OHOS::HDI::ServiceManager::V1_0::ServiceStatus;
using OHOS::HDI::ServiceManager::V1_0::ServStatListenerStub;
static constexpr const char *TEST_SERVICE_NAME = "sample_driver_service";
static constexpr const char16_t *TEST_SERVICE_INTERFACE_DESC = u"hdf.test.sampele_service";
static constexpr int PAYLOAD_NUM = 1234;
static constexpr int SMQ_TEST_QUEUE_SIZE = 10;
static constexpr int SMQ_TEST_WAIT_TIME = 100;
static constexpr int WAIT_LOAD_UNLOAD_TIME = 300;

class ManagerBenchmarkTest : public benchmark::Fixture {
public:
    void TestServiceListenerStop(const sptr<IDeviceManager>& devmgr, const sptr<IServiceManager>& servmgr);
    void TestSampleService(sptr<IRemoteObject>& sampleService, const sptr<IDeviceManager>& devmgr,
        const sptr<IServiceManager>& servmgr);
    void SetUp(const ::benchmark::State &state);
    void TearDown(const ::benchmark::State &state);
};
void ManagerBenchmarkTest::SetUp(const ::benchmark::State &state)
{
    auto devmgr = IDeviceManager::Get();
    if (devmgr != nullptr) {
        HDF_LOGI("%{public}s:%{public}d", __func__, __LINE__);
        devmgr->LoadDevice(TEST_SERVICE_NAME);
    }
}
void ManagerBenchmarkTest::TearDown(const ::benchmark::State &state)
{
    auto devmgr = IDeviceManager::Get();
    if (devmgr != nullptr) {
        HDF_LOGI("%{public}s:%{public}d", __func__, __LINE__);
        devmgr->UnloadDevice(TEST_SERVICE_NAME);
    }
}

class IPCObjectStubTest : public OHOS::IPCObjectStub {
public:
    explicit IPCObjectStubTest() : OHOS::IPCObjectStub(u"") {};
    virtual ~IPCObjectStubTest() = default;
    int OnRemoteRequest(
        uint32_t code, OHOS::MessageParcel &data, OHOS::MessageParcel &reply, OHOS::MessageOption &option) override
    {
        HDF_LOGI("IPCObjectStubTest::OnRemoteRequest called, code = %{public}d", code);
        payload = data.ReadInt32();

        return HDF_SUCCESS;
    }

    static int32_t payload;
};

int32_t IPCObjectStubTest::payload = 0;

/**
  * @tc.number: SUB_DriverSystem_ManagerBenchmarkTest_0010
  * @tc.name: open input device for ap mode benchmark test
  * @tc.size: Medium
  * @tc.level: level 1
  */
BENCHMARK_F(ManagerBenchmarkTest, SendRequest)(benchmark::State &st)
{
    auto servmgr = IServiceManager::Get();
    ASSERT_TRUE(servmgr != nullptr);

    auto sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    OHOS::MessageParcel data;
    OHOS::MessageParcel reply;
    bool ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    data.WriteCString("sample_service test call");

    OHOS::MessageOption option;
    int status;
    for (auto _ : st) {
        status = sampleService->SendRequest(SAMPLE_SERVICE_PING, data, reply, option);
        }
    ASSERT_EQ(status, 0);
}

BENCHMARK_REGISTER_F(ManagerBenchmarkTest, SendRequest)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_ManagerBenchmarkTest_0020
  * @tc.name: open input device for ap mode benchmark test
  * @tc.size: Medium
  * @tc.level: level 1
  */
BENCHMARK_F(ManagerBenchmarkTest, GetService)(benchmark::State &st)
{
    auto servmgr = IServiceManager::Get();
    ASSERT_TRUE(servmgr != nullptr);
    auto sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    for (auto _ : st) {
        sampleService = servmgr->GetService(TEST_SERVICE_NAME);
        }
    ASSERT_TRUE(sampleService != nullptr);
    sptr<IRemoteObject> callback = new IPCObjectStubTest();
    OHOS::MessageParcel data;
    OHOS::MessageParcel reply;
    int32_t payload = PAYLOAD_NUM;
    bool ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    data.WriteInt32(payload);
    data.WriteRemoteObject(callback);

    OHOS::MessageOption option;
    int status;
    status = sampleService->SendRequest(SAMPLE_SERVICE_CALLBACK, data, reply, option);
    ASSERT_EQ(status, 0);
    ASSERT_EQ(IPCObjectStubTest::payload, payload);
}

BENCHMARK_REGISTER_F(ManagerBenchmarkTest, GetService)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_ManagerBenchmarkTest_0030
  * @tc.name: Benchmark_Test device manager Load/UnLoad device and driver dynamic register device
  * @tc.size: Medium
  * @tc.level: level 1
  */
BENCHMARK_F(ManagerBenchmarkTest, LoadDevice)(benchmark::State &st)
{
    auto devmgr = IDeviceManager::Get();
    ASSERT_TRUE(devmgr != nullptr);
    devmgr->UnloadDevice(TEST_SERVICE_NAME);

    auto servmgr = IServiceManager::Get();
    ASSERT_TRUE(servmgr != nullptr);
    OsalMSleep(WAIT_LOAD_UNLOAD_TIME);
    auto sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);
    int ret;
    for (auto _ : st) {
        ret = devmgr->LoadDevice(TEST_SERVICE_NAME);
        }
    ASSERT_EQ(ret, HDF_SUCCESS);
    OsalMSleep(WAIT_LOAD_UNLOAD_TIME);
    sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    OHOS::MessageParcel data;
    OHOS::MessageParcel reply;
    OHOS::MessageOption option;
    ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    const char *newServName = "sample_driver_service2";
    ret = data.WriteCString(newServName);
    ASSERT_TRUE(ret);

    int status = sampleService->SendRequest(SAMPLE_REGISTER_DEVICE, data, reply, option);
    ASSERT_EQ(status, HDF_SUCCESS);

    auto sampleService2 = servmgr->GetService(newServName);
    ASSERT_TRUE(sampleService2 != nullptr);

    data.FlushBuffer();
    reply.FlushBuffer();
    ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    data.WriteInt32(PAYLOAD_NUM);
    data.WriteInt32(PAYLOAD_NUM);

    status = sampleService2->SendRequest(SAMPLE_SERVICE_SUM, data, reply, option);
    ASSERT_EQ(status, 0);
    int32_t result = reply.ReadInt32();

    int32_t expRes = PAYLOAD_NUM + PAYLOAD_NUM;
    ASSERT_EQ(result, expRes);
    sampleService2 = nullptr;

    data.FlushBuffer();
    reply.FlushBuffer();
    ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    data.WriteCString(newServName);

    status = sampleService->SendRequest(SAMPLE_UNREGISTER_DEVICE, data, reply, option);
    ASSERT_EQ(status, HDF_SUCCESS);

    sampleService2 = servmgr->GetService(newServName);
    ASSERT_TRUE(sampleService2 == nullptr);

    ret = devmgr->UnloadDevice(TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);
    OsalMSleep(WAIT_LOAD_UNLOAD_TIME);
    sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);
}

BENCHMARK_REGISTER_F(ManagerBenchmarkTest, LoadDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_ManagerBenchmarkTest_0040
  * @tc.name: Benchmark_Test Device manager Load/UnLoad device and driver dynamic register device 
  * @tc.size: Medium
  * @tc.level: level 1
  */
BENCHMARK_F(ManagerBenchmarkTest, UnloadDevice)(benchmark::State &st)
{
    auto devmgr = IDeviceManager::Get();
    ASSERT_TRUE(devmgr != nullptr);
    devmgr->UnloadDevice(TEST_SERVICE_NAME);

    auto servmgr = IServiceManager::Get();
    ASSERT_TRUE(servmgr != nullptr);
    OsalMSleep(WAIT_LOAD_UNLOAD_TIME);
    auto sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);

    int ret = devmgr->LoadDevice(TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);
    OsalMSleep(WAIT_LOAD_UNLOAD_TIME);
    sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    OHOS::MessageParcel data;
    OHOS::MessageParcel reply;
    OHOS::MessageOption option;
    ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    const char *newServName = "sample_driver_service2";
    ret = data.WriteCString(newServName);
    ASSERT_TRUE(ret);

    int status = sampleService->SendRequest(SAMPLE_REGISTER_DEVICE, data, reply, option);
    ASSERT_EQ(status, HDF_SUCCESS);

    auto sampleService2 = servmgr->GetService(newServName);
    ASSERT_TRUE(sampleService2 != nullptr);

    data.FlushBuffer();
    reply.FlushBuffer();
    ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    data.WriteInt32(PAYLOAD_NUM);
    data.WriteInt32(PAYLOAD_NUM);

    status = sampleService2->SendRequest(SAMPLE_SERVICE_SUM, data, reply, option);
    ASSERT_EQ(status, 0);
    int32_t result = reply.ReadInt32();

    int32_t expRes = PAYLOAD_NUM + PAYLOAD_NUM;
    ASSERT_EQ(result, expRes);
    sampleService2 = nullptr;

    data.FlushBuffer();
    reply.FlushBuffer();
    ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    data.WriteCString(newServName);

    status = sampleService->SendRequest(SAMPLE_UNREGISTER_DEVICE, data, reply, option);
    ASSERT_EQ(status, HDF_SUCCESS);

    sampleService2 = servmgr->GetService(newServName);
    ASSERT_TRUE(sampleService2 == nullptr);

    for (auto _ : st) {
        ret = devmgr->UnloadDevice(TEST_SERVICE_NAME);
        }
    ret = 0;
    ASSERT_EQ(ret, HDF_SUCCESS);
    OsalMSleep(WAIT_LOAD_UNLOAD_TIME);
    sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);
}

BENCHMARK_REGISTER_F(ManagerBenchmarkTest, UnloadDevice)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

class ServStatListener : public OHOS::HDI::ServiceManager::V1_0::ServStatListenerStub {
public:
    using StatusCallback = std::function<void(const ServiceStatus &)>;
    explicit ServStatListener(StatusCallback callback) : callback_(std::move(callback)) {}
    ~ServStatListener() = default;
    void OnReceive(const ServiceStatus &status) override
    {
        callback_(status);
    }

private:
    StatusCallback callback_;
};

/**
  * @tc.number: SUB_DriverSystem_ManagerBenchmarkTest_0050
  * @tc.name: smq test normal read/write (benchmarktest)
  * @tc.size: Medium
  * @tc.level: level 1
  */
BENCHMARK_F(ManagerBenchmarkTest, Marshalling)(benchmark::State &st)
{
    HDF_LOGI("%{public}s:%{public}d", __func__, __LINE__);
    auto servmgr = IServiceManager::Get();
    ASSERT_TRUE(servmgr != nullptr);

    OHOS::MessageParcel data;
    OHOS::MessageParcel reply;
    OHOS::MessageOption option;
    std::unique_ptr<SharedMemQueue<SampleSmqElement>> smq =
        std::make_unique<SharedMemQueue<SampleSmqElement>>(SMQ_TEST_QUEUE_SIZE, SmqType::SYNCED_SMQ);
    ASSERT_TRUE(smq->IsGood());
    bool ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    for (auto _ : st) {
        ret = smq->GetMeta()->Marshalling(data);
        }
    data.WriteUint32(1);

    constexpr int SendTimes = 20;
    for (size_t i = 0; i < SendTimes; i++) {
        SampleSmqElement t = { 0 };
        t.data32 = i;
        t.data64 = i + 1;

        HDF_LOGI("%{public}s:write smq message %{public}zu", __func__, i);
        auto status = smq->Write(&t, 1, OHOS::MillisecToNanosec(SMQ_TEST_WAIT_TIME));
        status = 0;
        ASSERT_EQ(status, 0);
    }
}

BENCHMARK_REGISTER_F(ManagerBenchmarkTest, Marshalling)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

/**
  * @tc.number: SUB_DriverSystem_ManagerBenchmarkTest_0060
  * @tc.name: smq test normal read/write for (benchmarktest)
  * @tc.size: Medium
  * @tc.level: level 1
  */
BENCHMARK_F(ManagerBenchmarkTest, Write)(benchmark::State &st)
{
    HDF_LOGI("%{public}s:%{public}d", __func__, __LINE__);
    auto servmgr = IServiceManager::Get();
    ASSERT_TRUE(servmgr != nullptr);

    auto sampleService = servmgr->GetService(TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    OHOS::MessageParcel data;
    OHOS::MessageParcel reply;
    OHOS::MessageOption option;
    std::unique_ptr<SharedMemQueue<SampleSmqElement>> smq =
        std::make_unique<SharedMemQueue<SampleSmqElement>>(SMQ_TEST_QUEUE_SIZE, SmqType::SYNCED_SMQ);
    ASSERT_TRUE(smq->IsGood());
    bool ret = data.WriteInterfaceToken(TEST_SERVICE_INTERFACE_DESC);
    ASSERT_EQ(ret, true);
    ret = smq->GetMeta()->Marshalling(data);
    ASSERT_TRUE(ret);
    data.WriteUint32(1);

    int status = sampleService->SendRequest(SAMPLE_TRANS_SMQ, data, reply, option);
    ASSERT_EQ(status, 0);

    constexpr int SendTimes = 20;
    for (size_t i = 0; i < SendTimes; i++) {
        SampleSmqElement t = { 0 };
        t.data32 = i;
        t.data64 = i + 1;

        HDF_LOGI("%{public}s:write smq message %{public}zu", __func__, i);
        auto status = smq->Write(&t, 1, OHOS::MillisecToNanosec(SMQ_TEST_WAIT_TIME));
        for (auto _ : st) {
            status = smq->Write(&t, 1, OHOS::MillisecToNanosec(SMQ_TEST_WAIT_TIME));
            }
        ASSERT_EQ(status, 0);
    }
}

BENCHMARK_REGISTER_F(ManagerBenchmarkTest, Write)->Iterations(100)->
    Repetitions(3)->ReportAggregatesOnly();

BENCHMARK_MAIN();