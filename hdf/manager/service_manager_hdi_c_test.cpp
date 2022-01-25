/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <devmgr_hdi.h>
#include <gtest/gtest.h>
#include <hdf_io_service_if.h>
#include <hdf_log.h>
#include <hdf_remote_service.h>
#include <hdf_sbuf.h>
#include <hdf_service_status.h>
#include <osal_time.h>
#include <servmgr_hdi.h>
#include <string>
#include <shared_mem.h>
#include <sys/mman.h>
#include "sample_hdi.h"

#define HDF_LOG_TAG service_manager_test

using namespace testing::ext;

constexpr const char *TEST_SERVICE_NAME = "sample_driver_service";
constexpr int PAYLOAD_NUM = 1234;

class HdfServiceMangerHdiCTest : public testing::Test {
public:
    static void SetUpTestCase()
    {
        struct HDIDeviceManager *devmgr = HDIDeviceManagerGet();
        if (devmgr != nullptr) {
            devmgr->LoadDevice(devmgr, TEST_SERVICE_NAME);
        }
    }
    static void TearDownTestCase()
    {
        struct HDIDeviceManager *devmgr = HDIDeviceManagerGet();
        if (devmgr != nullptr) {
            devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);
        }
    }
    void SetUp() {};
    void TearDown() {};
};

/**
  * @tc.number: SUB_DriverSystem_Manager_0010
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest001, Function | MediumTest | Level1)
{
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);
    HDIServiceManagerRelease(servmgr);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0020
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest002, Function | MediumTest | Level1)
{
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    HDIServiceManagerRelease(servmgr);
    ASSERT_TRUE(sampleService != nullptr);

    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);

    bool ret = HdfSbufWriteString(data, "sample_service test call");
    ASSERT_EQ(ret, true);

    int status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_SERVICE_PING, data, reply);
    ASSERT_EQ(status, 0);

    HdfSBufRecycle(data);
    HdfSBufRecycle(reply);
}

static int32_t g_callbackPayload = 0;

static int ServiceManagerTestCallbackDispatch(
    struct HdfRemoteService *service, int code, struct HdfSBuf *data, struct HdfSBuf *reply)
{
    HDF_LOGI("ServiceManagerTestCallbackDispatch called, code = %{public}d", code);
    HdfSbufReadInt32(data, &g_callbackPayload);
    return HDF_SUCCESS;
}

static struct HdfRemoteDispatcher g_callbackDispatcher {
    .Dispatch = ServiceManagerTestCallbackDispatch,
};

/**
  * @tc.number: SUB_DriverSystem_Manager_0030
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest003, Function | MediumTest | Level1)
{
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    HDIServiceManagerRelease(servmgr);
    ASSERT_TRUE(sampleService != nullptr);

    struct HdfRemoteService *callback = HdfRemoteServiceObtain(nullptr, &g_callbackDispatcher);
    ASSERT_NE(callback, nullptr);
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);

    int32_t payload = PAYLOAD_NUM;
    HdfSbufWriteInt32(data, payload);
    HdfSBufWriteRemoteService(data, callback);

    int status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_SERVICE_CALLBACK, data, reply);
    ASSERT_EQ(status, 0);
    ASSERT_EQ(g_callbackPayload, payload);

    HdfSBufRecycle(data);
    HdfSBufRecycle(reply);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0040
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest004, Function | MediumTest | Level1)
{
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    HDIServiceManagerRelease(servmgr);
    ASSERT_TRUE(sampleService != nullptr);

    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);
    HdfSbufWriteInt32(data, PAYLOAD_NUM);
    HdfSbufWriteInt32(data, PAYLOAD_NUM);

    int status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_SERVICE_SUM, data, reply);
    ASSERT_EQ(status, 0);
    int32_t result;
    bool ret = HdfSbufReadInt32(reply, &result);
    ASSERT_TRUE(ret);

    int32_t expRes = PAYLOAD_NUM + PAYLOAD_NUM;
    ASSERT_EQ(result, expRes);

    HdfSBufRecycle(data);
    HdfSBufRecycle(reply);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0050
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest005, Function | MediumTest | Level1)
{
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    HDIServiceManagerRelease(servmgr);
    ASSERT_TRUE(sampleService != nullptr);

    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);

    struct DataBlock dataBlock = { 1, 2, "dataBolck", 3 };
    bool ret = DataBlockBlockMarshalling(&dataBlock, data);
    ASSERT_TRUE(ret);

    int status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_STRUCT_TRANS, data, reply);
    ASSERT_EQ(status, 0);

    struct DataBlock *dataBlock_ = DataBlockBlockUnmarshalling(reply);
    ASSERT_TRUE(dataBlock_ != nullptr);

    ASSERT_EQ(dataBlock_->a, dataBlock.a);
    ASSERT_EQ(dataBlock_->b, dataBlock.b);
    ASSERT_EQ(dataBlock_->c, dataBlock.c);
    ASSERT_TRUE(!strcmp(dataBlock_->str, dataBlock.str));
    DataBlockFree(dataBlock_);

    HdfSBufRecycle(data);
    HdfSBufRecycle(reply);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0060
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest006, Function | MediumTest | Level1)
{
    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    HDIServiceManagerRelease(servmgr);
    ASSERT_TRUE(sampleService != nullptr);

    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);

    constexpr int buffersize = 10;
    uint8_t dataBuffer[buffersize];
    for (int i = 0; i < buffersize; i++) {
        dataBuffer[i] = i;
    }

    bool ret = HdfSbufWriteUnpadBuffer(data, dataBuffer, sizeof(dataBuffer));
    ASSERT_TRUE(ret);

    int status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_BUFFER_TRANS, data, reply);
    ASSERT_EQ(status, 0);

    const uint8_t *retBuffer = HdfSbufReadUnpadBuffer(reply, buffersize);
    ASSERT_TRUE(retBuffer != nullptr);

    for (int i = 0; i < buffersize; i++) {
        ASSERT_EQ(retBuffer[i], i);
    }

    HdfSBufRecycle(data);
    HdfSBufRecycle(reply);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0070
  * @tc.name: open input device test
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest007, Function | MediumTest | Level1)
{
    struct HDIDeviceManager *devmgr = HDIDeviceManagerGet();
    ASSERT_TRUE(devmgr != nullptr);
    devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);

    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);

    int ret = devmgr->LoadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);

    const char *newServName = "sample_driver_service2";
    ret = HdfSbufWriteString(data, newServName);
    ASSERT_TRUE(ret);

    int status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_REGISTER_DEVICE, data, reply);
    ASSERT_EQ(status, HDF_SUCCESS);

    struct HdfRemoteService *sampleService2 = servmgr->GetService(servmgr, newServName);
    ASSERT_TRUE(sampleService != nullptr);

    HdfSbufFlush(data);
    HdfSbufFlush(reply);
    HdfSbufWriteInt32(data, PAYLOAD_NUM);
    HdfSbufWriteInt32(data, PAYLOAD_NUM);

    status = sampleService2->dispatcher->Dispatch(sampleService2, SAMPLE_SERVICE_SUM, data, reply);
    ASSERT_EQ(status, 0);
    int32_t result;
    ret = HdfSbufReadInt32(reply, &result);
    ASSERT_TRUE(ret);

    int32_t expRes = PAYLOAD_NUM + PAYLOAD_NUM;
    ASSERT_EQ(result, expRes);
    HdfRemoteServiceRecycle(sampleService2);

    HdfSbufFlush(data);
    ret = HdfSbufWriteString(data, newServName);
    ASSERT_TRUE(ret);

    status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_UNREGISTER_DEVICE, data, reply);
    ASSERT_EQ(status, HDF_SUCCESS);

    sampleService2 = servmgr->GetService(servmgr, newServName);
    ASSERT_TRUE(sampleService2 == nullptr);

    ret = devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    HdfRemoteServiceRecycle(sampleService);
    sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);

    HdfSBufRecycle(data);
    HdfSBufRecycle(reply);
}

struct ServiceStatusData {
    ServiceStatusData() : devClass(0), servStatus(0), callbacked(false) {}
    ~ServiceStatusData() = default;
    std::string servInfo;
    uint16_t devClass;
    uint16_t servStatus;
    bool callbacked;
};

static void TestOnServiceStatusReceived(struct ServiceStatusListener *listener, struct ServiceStatus *servstat)
{
    struct ServiceStatusData *ssd = (struct ServiceStatusData *)listener->priv;
    if (ssd == nullptr) {
        return;
    }
    if (strcmp(servstat->serviceName, TEST_SERVICE_NAME) == 0) {
        ssd->servInfo = servstat->info != nullptr ? servstat->info : "";
        ssd->devClass = servstat->deviceClass;
        ssd->servStatus = servstat->status;
        ssd->callbacked = true;
    }

    HDF_LOGI("service status listener callback: %{public}s, %{public}d", servstat->serviceName, servstat->status);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0080
  * @tc.name: Test service start status listener
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest008, Function | MediumTest | Level1)
{
    struct HDIDeviceManager *devmgr = HDIDeviceManagerGet();
    ASSERT_TRUE(devmgr != nullptr);
    devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);

    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);

    struct ServiceStatusData ssd;
    struct ServiceStatusListener *listener = HdiServiceStatusListenerNewInstance();
    listener->callback = TestOnServiceStatusReceived;
    listener->priv = (void *)&ssd;

    int status = servmgr->RegisterServiceStatusListener(servmgr, listener, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(status, HDF_SUCCESS);

    int ret = devmgr->LoadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    constexpr int WAIT_COUNT = 10;
    int count = WAIT_COUNT;
    while (!ssd.callbacked && count > 0) {
        OsalMSleep(1);
        count--;
    }

    ASSERT_TRUE(ssd.callbacked);
    ASSERT_EQ(ssd.devClass, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(ssd.servInfo, std::string(TEST_SERVICE_NAME));
    ASSERT_EQ(ssd.servStatus, SERVIE_STATUS_START);

    ssd.callbacked = false;
    ret = devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    count = WAIT_COUNT;
    while (!ssd.callbacked && count > 0) {
        OsalMSleep(1);
        count--;
    }
    ASSERT_TRUE(ssd.callbacked);
    ASSERT_EQ(ssd.devClass, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(ssd.servInfo, std::string(TEST_SERVICE_NAME));
    ASSERT_EQ(ssd.servStatus, SERVIE_STATUS_STOP);

    status = servmgr->UnregisterServiceStatusListener(servmgr, listener);
    ASSERT_EQ(status, HDF_SUCCESS);

    HdiServiceStatusListenerFree(listener);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0090
  * @tc.name: Test service status listener update service info
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest009, Function | MediumTest | Level1)
{
    struct HDIDeviceManager *devmgr = HDIDeviceManagerGet();
    ASSERT_TRUE(devmgr != nullptr);
    devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);

    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);

    int ret = devmgr->LoadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    struct ServiceStatusData ssd;
    struct ServiceStatusListener *listener = HdiServiceStatusListenerNewInstance();
    listener->callback = TestOnServiceStatusReceived;
    listener->priv = (void *)&ssd;

    int status = servmgr->RegisterServiceStatusListener(servmgr, listener, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(status, HDF_SUCCESS);
    constexpr int FIRST_WAIT = 20;
    OsalMSleep(FIRST_WAIT); // skip callback on register

    std::string info = "foo";
    struct HdfSBuf *data = HdfSBufTypedObtain(SBUF_IPC);
    struct HdfSBuf *reply = HdfSBufTypedObtain(SBUF_IPC);
    ASSERT_TRUE(data != nullptr);
    ASSERT_TRUE(reply != nullptr);

    ret = HdfSbufWriteString(data, info.data());
    ASSERT_TRUE(ret);

    ssd.callbacked = false;
    status = sampleService->dispatcher->Dispatch(sampleService, SAMPLE_UPDATE_SERVIE, data, reply);
    ASSERT_EQ(status, HDF_SUCCESS);

    constexpr int WAIT_COUNT = 10;
    int count = WAIT_COUNT;
    while (!ssd.callbacked && count > 0) {
        OsalMSleep(1);
        count--;
    }
    ASSERT_TRUE(ssd.callbacked);
    ASSERT_EQ(ssd.devClass, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(ssd.servInfo, info);
    ASSERT_EQ(ssd.servStatus, SERVIE_STATUS_CHANGE);

    ret = devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    status = servmgr->UnregisterServiceStatusListener(servmgr, listener);
    ASSERT_EQ(status, HDF_SUCCESS);

    HdiServiceStatusListenerFree(listener);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0100
  * @tc.name: Test service status listener unregister
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest010, Function | MediumTest | Level1)
{
    struct HDIDeviceManager *devmgr = HDIDeviceManagerGet();
    ASSERT_TRUE(devmgr != nullptr);
    devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);

    struct HDIServiceManager *servmgr = HDIServiceManagerGet();
    ASSERT_TRUE(servmgr != nullptr);

    struct HdfRemoteService *sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService == nullptr);

    struct ServiceStatusData ssd;
    struct ServiceStatusListener *listener = HdiServiceStatusListenerNewInstance();
    listener->callback = TestOnServiceStatusReceived;
    listener->priv = (void *)&ssd;

    int status = servmgr->RegisterServiceStatusListener(servmgr, listener, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(status, HDF_SUCCESS);

    int ret = devmgr->LoadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    constexpr int WAIT_COUNT = 10;
    int count = WAIT_COUNT;
    while (!ssd.callbacked && count > 0) {
        OsalMSleep(1);
        count--;
    }
    ASSERT_TRUE(ssd.callbacked);
    ASSERT_EQ(ssd.devClass, DEVICE_CLASS_DEFAULT);
    ASSERT_EQ(ssd.servInfo, std::string(TEST_SERVICE_NAME));
    ASSERT_EQ(ssd.servStatus, SERVIE_STATUS_START);

    sampleService = servmgr->GetService(servmgr, TEST_SERVICE_NAME);
    ASSERT_TRUE(sampleService != nullptr);

    status = servmgr->UnregisterServiceStatusListener(servmgr, listener);
    ASSERT_EQ(status, HDF_SUCCESS);
    HdiServiceStatusListenerFree(listener);

    ret = devmgr->UnloadDevice(devmgr, TEST_SERVICE_NAME);
    ASSERT_EQ(ret, HDF_SUCCESS);

    ssd.callbacked = false;
    OsalMSleep(WAIT_COUNT);
    ASSERT_FALSE(ssd.callbacked);
}

/**
  * @tc.number: SUB_DriverSystem_Manager_0110
  * @tc.name: Test shared mem interfac
  * @tc.size: Medium
  * @tc.level: level 1
  */
HWTEST_F(HdfServiceMangerHdiCTest, ServMgrTest011, Function | MediumTest | Level1)
{
    constexpr int mapSize = 128;
    int memFd = SharedMemCreate("shared_mem_test", mapSize);
    ASSERT_TRUE(memFd >= 0);

    void *ptr = mmap(nullptr, mapSize, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);
    ASSERT_NE(ptr, MAP_FAILED);

    uint8_t *data = reinterpret_cast<uint8_t *>(ptr);
    for (int i = 0; i < mapSize; i++) {
        data[i] = i;
    }

    for (int i = 0; i < mapSize; i++) {
        ASSERT_EQ(data[i], i);
    }

    auto ret = munmap(ptr, mapSize);
    ASSERT_EQ(ret, 0);

    close(memFd);
}