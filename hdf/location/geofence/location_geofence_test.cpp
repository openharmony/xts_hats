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

#include <iostream>
#include <osal_mem.h>
#include "hdf_sbuf.h"
#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include "idevmgr_hdi.h"
#include <securec.h>
#include "hdf_base.h"
#include "hdf_log.h"
#include "osal_time.h"
#include "v1_0/igeofence_interface.h"
#include "geofence_interface_impl.h"
#include "geofence_callback_impl.h"

using namespace OHOS::HDI::Location::Geofence::V1_0;
using namespace std;
using namespace testing::ext;

namespace {
    sptr<IGeofenceInterface> g_igeofenceHci = nullptr;
    constexpr const char *AGNSS_SERVICE_NAME = "agnss_interface_service";
    constexpr const char *GNSS_SERVICE_NAME = "gnss_interface_service";
    constexpr const char *GEOFENCE_SERVICE_NAME = "geofence_interface_service";
}

class LocationGeofenceTest: public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

int32_t GeofenceCallbackImpl::ReportGeofenceAvailability(bool isAvailable)
{
    return HDF_SUCCESS;
}

int32_t GeofenceCallbackImpl::ReportGeofenceEvent(int32_t fenceIndex, const LocationInfo& location, GeofenceEvent event, int64_t timestamp)
{
    return HDF_SUCCESS;
}

int32_t GeofenceCallbackImpl::ReportGeofenceOperateResult(int32_t fenceIndex, GeofenceOperateType type, GeofenceOperateResult result)
{
    return HDF_SUCCESS;
}

void LocationGeofenceTest::SetUpTestCase()
{
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return;
    }
    if (devmgr->LoadDevice(GNSS_SERVICE_NAME) != 0) {
        printf("Load gnss service failed!\n");
        return;
    }
    if (devmgr->LoadDevice(AGNSS_SERVICE_NAME) != 0) {
        printf("Load agnss service failed!\n");
        return;
    }
    if (devmgr->LoadDevice(GEOFENCE_SERVICE_NAME) != 0) {
        printf("Load geofence service failed!\n");
        return;
    }
    g_igeofenceHci = IGeofenceInterface::Get();
}

void LocationGeofenceTest::TearDownTestCase()
{
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return;
    }
    if (devmgr->UnloadDevice(GNSS_SERVICE_NAME) != 0) {
        printf("Load gnss service failed!\n");
        return;
    }
    if (devmgr->UnloadDevice(AGNSS_SERVICE_NAME) != 0) {
        printf("Load agnss service failed!\n");
        return;
    }
    if (devmgr->UnloadDevice(GEOFENCE_SERVICE_NAME) != 0) {
        printf("Load geofence service failed!\n");
        return;
    }
}

void LocationGeofenceTest::SetUp()
{
}

void LocationGeofenceTest::TearDown()
{
}


/**
  * @tc.name: SetGeofenceCallback0100
  * @tc.desc: Set callback function for geofence.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGeofenceTest, SUB_DriverSystem_EnableGnss_0100, TestSize.Level1)
{
    if (g_igeofenceHci == nullptr) {
        ASSERT_NE(nullptr, g_igeofenceHci);
        return;
    }
    sptr<IGeofenceCallback> geo_callback = new (std::nothrow) GeofenceCallbackImpl();
    if (geo_callback == nullptr) {
        ASSERT_NE(nullptr, geo_callback);
        return;
    }
    int32_t ret = g_igeofenceHci->SetGeofenceCallback(geo_callback);
    EXPECT_EQ(HDF_SUCCESS, ret);
}


/**
  * @tc.name: AddGnssGeofence0100
  * @tc.desc: Add a geofence.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGeofenceTest, SUB_DriverSystem_AddGnssGeofence_0100, TestSize.Level1)
{
    if (g_igeofenceHci == nullptr) {
        ASSERT_NE(nullptr, g_igeofenceHci);
        return;
    }
    GeofenceInfo fence;
    fence.fenceIndex = 3;
    fence.latitude = 118.90;
    fence.longitude = 15.25;
    fence.radius = 12.26;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN ;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence,geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}


/**
  * @tc.name: DeleteGnssGeofence0100
  * @tc.desc: Delete a geofence.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGeofenceTest, SUB_DriverSystem_DeleteGnssGeofence_0100, TestSize.Level1)
{
    if (g_igeofenceHci == nullptr) {
        ASSERT_NE(nullptr, g_igeofenceHci);
        return;
    }
    int fenceIndex = 5;
    int32_t ret = g_igeofenceHci->DeleteGnssGeofence(fenceIndex);
    EXPECT_EQ(HDF_SUCCESS, ret);
}
