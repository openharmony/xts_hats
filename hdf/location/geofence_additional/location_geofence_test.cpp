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
} // namespace

class LocationGeofenceAdditionalTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

int32_t GeofenceCallbackImpl::ReportGeofenceAvailability(bool isAvailable)
{
    (void)isAvailable;
    return HDF_SUCCESS;
}

int32_t GeofenceCallbackImpl::ReportGeofenceEvent(int32_t fenceIndex, const LocationInfo &location, GeofenceEvent event,
                                                  int64_t timestamp)
{
    (void)fenceIndex;
    (void)location;
    (void)event;
    (void)timestamp;
    return HDF_SUCCESS;
}

int32_t GeofenceCallbackImpl::ReportGeofenceOperateResult(int32_t fenceIndex, GeofenceOperateType type,
                                                          GeofenceOperateResult result)
{
    (void)fenceIndex;
    (void)type;
    (void)result;
    return HDF_SUCCESS;
}

void LocationGeofenceAdditionalTest::SetUpTestCase()
{
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return;
    }
    if (devmgr->LoadDevice("gnss_interface_service") != 0) {
        printf("Load gnss service failed!\n");
        return;
    }
    if (devmgr->LoadDevice("agnss_interface_service") != 0) {
        printf("Load agnss service failed!\n");
        return;
    }
    if (devmgr->LoadDevice("geofence_interface_service") != 0) {
        printf("Load geofence service failed!\n");
        return;
    }
    g_igeofenceHci = IGeofenceInterface::Get();
}

void LocationGeofenceAdditionalTest::TearDownTestCase()
{
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return;
    }
    if (devmgr->UnloadDevice("gnss_interface_service") != 0) {
        printf("Load gnss service failed!\n");
        return;
    }
    if (devmgr->UnloadDevice("agnss_interface_service") != 0) {
        printf("Load agnss service failed!\n");
        return;
    }
    if (devmgr->UnloadDevice("geofence_interface_service") != 0) {
        printf("Load geofence service failed!\n");
        return;
    }
}

void LocationGeofenceAdditionalTest::SetUp() {}

void LocationGeofenceAdditionalTest::TearDown() {}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0200
 * @tc.name  : testAddGnssGeofence001
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence001, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0300
 * @tc.name  : testAddGnssGeofence002
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN 100times.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence002, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0400
 * @tc.name  : testAddGnssGeofence003
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence003, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0500
 * @tc.name  : testAddGnssGeofence004
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence004, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0600
 * @tc.name  : testAddGnssGeofence005
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence005, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0700
 * @tc.name  : testAddGnssGeofence006
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence006, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0800
 * @tc.name  : testAddGnssGeofence007
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence007, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_0900
 * @tc.name  : testAddGnssGeofence008
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence008, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1000
 * @tc.name  : testAddGnssGeofence009
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_UNCERTAIN.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence009, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 180.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_UNCERTAIN;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1100
 * @tc.name  : testAddGnssGeofence010
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence010, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1200
 * @tc.name  : testAddGnssGeofence011
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED 100times.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence011, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1300
 * @tc.name  : testAddGnssGeofence012
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence012, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1400
 * @tc.name  : testAddGnssGeofence013
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence013, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1500
 * @tc.name  : testAddGnssGeofence014
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence014, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1600
 * @tc.name  : testAddGnssGeofence015
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence015, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1700
 * @tc.name  : testAddGnssGeofence016
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence016, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1800
 * @tc.name  : testAddGnssGeofence017
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence017, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_1900
 * @tc.name  : testAddGnssGeofence018
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_ENTERED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence018, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 180.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_ENTERED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2000
 * @tc.name  : testAddGnssGeofence019
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence019, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2100
 * @tc.name  : testAddGnssGeofence020
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED 100times.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence020, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2200
 * @tc.name  : testAddGnssGeofence021
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence021, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2300
 * @tc.name  : testAddGnssGeofence022
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence022, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 1.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2400
 * @tc.name  : testAddGnssGeofence023
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence023, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2500
 * @tc.name  : testAddGnssGeofence024
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence024, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 1.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2600
 * @tc.name  : testAddGnssGeofence025
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence025, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2700
 * @tc.name  : testAddGnssGeofence026
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence026, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 1.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_AddGnssGeofence_2800
 * @tc.name  : testAddGnssGeofence027
 * @tc.desc  : Call function AddGnssGeofence with GeofenceEvent as GEOFENCE_EVENT_EXITED.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testAddGnssGeofence027, Function | MediumTest | Level1)
{
    GeofenceInfo fence;
    fence.fenceIndex = 1;
    fence.latitude = 180.00;
    fence.longitude = 180.00;
    fence.radius = 180.00;
    GeofenceEvent geoevent = GeofenceEvent::GEOFENCE_EVENT_EXITED;
    int32_t ret = g_igeofenceHci->AddGnssGeofence(fence, geoevent);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_DeleteGnssGeofence_0200
 * @tc.name  : testDeleteGnssGeofence001
 * @tc.desc  : Call function DeleteGnssGeofence with fenceIndex as 5 100times.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testDeleteGnssGeofence001, Function | MediumTest | Level1)
{
    int fenceIndex = 5;
    int32_t ret = 0;
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->DeleteGnssGeofence(fenceIndex);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}

/**
 * @tc.number: SUB_Location_Geofence_DeleteGnssGeofence_0300
 * @tc.name  : testDeleteGnssGeofence002
 * @tc.desc  : Call function DeleteGnssGeofence with fenceIndex as 0.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testDeleteGnssGeofence002, Function | MediumTest | Level1)
{
    int fenceIndex = 0;
    int32_t ret = g_igeofenceHci->DeleteGnssGeofence(fenceIndex);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
 * @tc.number: SUB_Location_Geofence_SetGeofenceCallback_0200
 * @tc.name  : testSetGeofenceCallback001
 * @tc.desc  : Call function SetGeofenceCallback 100times.
 */
HWTEST_F(LocationGeofenceAdditionalTest, testSetGeofenceCallback001, Function | MediumTest | Level1)
{
    int32_t ret = 0;
    sptr<IGeofenceCallback> geo_callback = new (std::nothrow) GeofenceCallbackImpl();
    if (geo_callback == nullptr) {
        ASSERT_NE(nullptr, geo_callback);
        return;
    }
    for (int i = 0; i < 100; i++) {
        ret = g_igeofenceHci->SetGeofenceCallback(geo_callback);
        EXPECT_EQ(HDF_SUCCESS, ret);
    }
}