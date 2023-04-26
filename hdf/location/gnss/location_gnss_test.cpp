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


#include "idevmgr_hdi.h"
#include <memory>
#include <iostream>
#include <osal_mem.h>
#include "hdf_sbuf.h"
#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#include "hdf_base.h"
#include "hdf_log.h"
#include "osal_time.h"
#include "v1_0/ignss_interface.h"
#include "gnss_interface_impl.h"
#include "gnss_callback_impl.h"


using namespace OHOS::HDI::Location::Gnss::V1_0;
using namespace std;
using namespace testing::ext;

namespace {
    sptr<IGnssInterface> g_ignssHci = nullptr;
    constexpr const char *AGNSS_SERVICE_NAME = "agnss_interface_service";
    constexpr const char *GNSS_SERVICE_NAME = "gnss_interface_service";
    constexpr const char *GEOFENCE_SERVICE_NAME = "geofence_interface_service";
}

class LocationGnssTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};


int32_t GnssCallbackImpl::ReportLocation(const LocationInfo& location)
{
    if (location.timeSinceBoot != 0) {
        printf("Location success!!\n");
        return HDF_SUCCESS;
    }
    else{
        printf("Location fail!!\n");
        return HDF_FAILURE;
    }
}


int32_t GnssCallbackImpl::ReportGnssWorkingStatus(GnssWorkingStatus status)
{
    if (status == GnssWorkingStatus::GNSS_STATUS_NONE) {
        printf("GNSS_STATUS_NONE\n");
        return HDF_SUCCESS;
    }
    else if(status == GnssWorkingStatus::GNSS_STATUS_SESSION_BEGIN){
        printf("GNSS_STATUS_SESSION_BEGIN\n");
        return HDF_SUCCESS;
    }
    else if(status == GnssWorkingStatus::GNSS_STATUS_SESSION_END){
        printf("GNSS_STATUS_SESSION_END\n");
        return HDF_SUCCESS;
    }
    else if(status == GnssWorkingStatus::GNSS_STATUS_ENGINE_ON){
        printf("GNSS_STATUS_ENGINE_ON\n");
        return HDF_SUCCESS;
    }
    else if(status == GnssWorkingStatus::GNSS_STATUS_ENGINE_OFF){
        printf("GNSS_STATUS_ENGINE_OFF\n");
        return HDF_SUCCESS;
    }
    else{
        printf("Gnss status fail\n");
        return HDF_FAILURE;
    }
}

int32_t GnssCallbackImpl::ReportNmea(int64_t timestamp, const std::string& nmea, int32_t length)
{
    if (length >= 0) {
        printf("Report nmea success\n");
        return HDF_SUCCESS;
    }
    else{
        printf("Report nmea fail\n");
        return HDF_FAILURE;
    }
}

int32_t GnssCallbackImpl::ReportGnssCapabilities(GnssCapabilities capabilities)
{
    return HDF_SUCCESS;
}

int32_t GnssCallbackImpl::ReportSatelliteStatusInfo(const SatelliteStatusInfo& info)
{
    if (info.satellitesNumber <= 0) {
        printf("SvStatusCallback, satellites_num <= 0!\n");
        return HDF_ERR_INVALID_PARAM;
    }
    if (((info.carrierFrequencies).size()) > 0) {
        printf("Get satellite info success!!\n");
        return HDF_SUCCESS;
    }
    else{
        printf("Get satellite info fail!!\n");
        return HDF_FAILURE;
    }
}

int32_t GnssCallbackImpl::RequestGnssReferenceInfo(GnssRefInfoType type)
{
    return HDF_SUCCESS;
}

int32_t GnssCallbackImpl::RequestPredictGnssData()
{
    return HDF_SUCCESS;
}

int32_t GnssCallbackImpl::ReportCachedLocation(const std::vector<LocationInfo>& gnssLocations)
{
    return HDF_SUCCESS;
}



void LocationGnssTest::SetUpTestCase()
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
    g_ignssHci = IGnssInterface::Get();
}

void LocationGnssTest::TearDownTestCase()
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

void LocationGnssTest::SetUp()
{
}

void LocationGnssTest::TearDown()
{
}


/**
  * @tc.name: EnableGnss0100
  * @tc.desc: Enable the GNSS module and transmit the callback of the upper layer to the GNSS module..
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_EnableGnss_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    sptr<IGnssCallback> gnss_callback = new (std::nothrow) GnssCallbackImpl();
    if (gnss_callback == nullptr) {
        ASSERT_NE(nullptr, gnss_callback);
        return;
    }
    int32_t ret = g_ignssHci->EnableGnss(gnss_callback);
    EXPECT_EQ(HDF_SUCCESS, ret);
    GnssStartType starttype = GnssStartType::GNSS_START_TYPE_NORMAL;
    int32_t ret1 = g_ignssHci->StartGnss(starttype);
    EXPECT_EQ(HDF_SUCCESS, ret1);
}


/**
  * @tc.name: SetGnssConfigPara0100
  * @tc.desc: Setting gnss configuration parameters.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_SetGnssConfigPara_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssConfigPara para;
    para.gnssBasic.minInterval = 10;
    para.gnssBasic.accuracy = 50;
    para.gnssBasic.firstFixTime = 300;
    para.gnssBasic.isPeriodicPositioning = true;
    para.gnssBasic.gnssMode = GnssWorkingMode::GNSS_WORKING_MODE_STANDALONE;
    para.gnssCaching.interval = 20;
    para.gnssCaching.fifoFullNotify = true;
    int32_t ret = g_ignssHci->SetGnssConfigPara(para);
    EXPECT_EQ(HDF_SUCCESS, ret);
}


/**
  * @tc.name: SetGnssReferenceInfo0100
  * @tc.desc: Inject reference information to the GNSS module.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_SetGnssReferenceInfo_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssRefInfo refInfo;
    refInfo.type = GnssRefInfoType::GNSS_REF_INFO_TIME;
    refInfo.time.timeMs = 50;
    refInfo.time.timeReferenceMs = 100;
    refInfo.time.uncertainty = 200;
    refInfo.location.latitude = 39.56;
    refInfo.location.longitude = 116.20;
    refInfo.location.accuracy = 90;
    refInfo.best_location.latitude = 39.58;
    refInfo.best_location.longitude = 116.45;
    refInfo.best_location.altitude = 110;
    refInfo.best_location.accuracy = 60;
    refInfo.best_location.speed = 60;
    refInfo.best_location.direction = 60;
    refInfo.best_location.timeStamp = 60;
    refInfo.best_location.timeSinceBoot = 60;
    int32_t ret = g_ignssHci->SetGnssReferenceInfo(refInfo);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: StopGnss0100
  * @tc.desc: Stop the navigation function in normal mode.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_StopGnss_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssStartType stoptype = GnssStartType::GNSS_START_TYPE_NORMAL;
    int32_t ret = g_ignssHci->StopGnss(stoptype);
    EXPECT_EQ(HDF_SUCCESS, ret);  
}

/**
  * @tc.name: DeleteAuxiliaryData0100
  * @tc.desc: Delete the specified auxiliary data.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_DeleteAuxiliaryData_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssAuxiliaryData auxdata = GnssAuxiliaryData::GNSS_AUXILIARY_DATA_EPHEMERIS;
    int32_t ret = g_ignssHci->DeleteAuxiliaryData(auxdata);
    EXPECT_EQ(HDF_SUCCESS, ret);
}


/**
  * @tc.name: SetPredictGnssData0100
  * @tc.desc: Ingesting Predict Gnss Data.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_SetPredictGnssData_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    const std::string str1 = "testing";
    int32_t ret = g_ignssHci->SetPredictGnssData(str1);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: GetCachedGnssLocationsSize0100
  * @tc.desc: Obtain the number of locations that can be cached by the GNSS module.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_GetCachedGnssLocationsSize_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    int cach_size = 60;
    int32_t ret = g_ignssHci->GetCachedGnssLocationsSize(cach_size);
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: GetCachedGnssLocations0100
  * @tc.desc: Request to obtain all the location information in the GNSS cache at a time and clear the cache buffer.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_GetCachedGnssLocations_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    int32_t ret = g_ignssHci->GetCachedGnssLocations();
    EXPECT_EQ(HDF_SUCCESS, ret);
}

/**
  * @tc.name: DisableGnss0100
  * @tc.desc: Disable the GNSS module.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_DisableGnss_0100, TestSize.Level1)
{
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    int32_t ret = g_ignssHci->DisableGnss();
    EXPECT_EQ(HDF_SUCCESS, ret);
}
