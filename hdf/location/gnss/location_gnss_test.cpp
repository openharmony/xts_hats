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
#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#ifdef FEATURE_GNSS_SUPPORT
#include <osal_mem.h>
#include "hdf_base.h"
#include "hdf_log.h"
#include "osal_time.h"
#include "hdf_sbuf.h"
#include "v2_0/ignss_interface.h"
#include "gnss_callback_impl.h"

using namespace OHOS::HDI::Location::Gnss::V2_0;
#endif
using namespace std;
using namespace testing::ext;

namespace {
#ifdef FEATURE_GNSS_SUPPORT
    sptr<IGnssInterface> g_ignssHci = nullptr;
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    constexpr const char *AGNSS_SERVICE_NAME = "agnss_interface_service";
#endif
    constexpr const char *GNSS_SERVICE_NAME = "gnss_interface_service";
#ifdef HDF_DRIVERS_INTERFACE_GEOFENCE_ENABLE
    constexpr const char *GEOFENCE_SERVICE_NAME = "geofence_interface_service";
#endif
    constexpr const char *LOCATION_HOST_NAME = "location_host";
#endif
}

class LocationGnssTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

#ifdef FEATURE_GNSS_SUPPORT
bool IsDeviceLoaded(const std::string &servName)
{
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return false;
    }
    std::vector<OHOS::HDI::DeviceManager::V1_0::HdiDevHostInfo> deviceInfos;
    int ret = devmgr->ListAllDevice(deviceInfos);
    if (ret != HDF_SUCCESS) {
        printf("get listAllDevice failed\n");
        return false;
    }
    auto itDevicesInfo = deviceInfos.begin();
    for (;itDevicesInfo != deviceInfos.end(); itDevicesInfo++) {
        if (itDevicesInfo->hostName == LOCATION_HOST_NAME) {
            break;
        }
    }
    if (itDevicesInfo == deviceInfos.end()) {
        printf("The host is not found:LOCATION_HOST_NAME\n");
        return false;
    }
    auto itDevInfo = itDevicesInfo->devInfo.begin();
    for (;itDevInfo != itDevicesInfo->devInfo.end(); itDevInfo++) {
        if (itDevInfo->servName == servName) {
            break;
        }
    }
    if (itDevInfo == itDevicesInfo->devInfo.end()) {
        printf("The devices is not found\n");
        return false;
    }
    printf("already loaded...\n");
    return true;
}

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
    if (status == GnssWorkingStatus::GNSS_WORKING_STATUS_NONE) {
        printf("GNSS_WORKING_STATUS_NONE\n");
        return HDF_SUCCESS;
    } else if (status == GnssWorkingStatus::GNSS_WORKING_STATUS_SESSION_BEGIN) {
        printf("GNSS_WORKING_STATUS_SESSION_BEGIN\n");
        return HDF_SUCCESS;
    } else if (status == GnssWorkingStatus::GNSS_WORKING_STATUS_SESSION_END) {
        printf("GNSS_WORKING_STATUS_SESSION_END\n");
        return HDF_SUCCESS;
    } else if (status == GnssWorkingStatus::GNSS_WORKING_STATUS_ENGINE_ON) {
        printf("GNSS_WORKING_STATUS_ENGINE_ON\n");
        return HDF_SUCCESS;
    } else if (status == GnssWorkingStatus::GNSS_WORKING_STATUS_ENGINE_OFF) {
        printf("GNSS_WORKING_STATUS_ENGINE_OFF\n");
        return HDF_SUCCESS;
    } else {
        printf("Gnss status fail\n");
        return HDF_FAILURE;
    }
}

int32_t GnssCallbackImpl::ReportNmea(int64_t timestamp, const std::string& nmea, int32_t length)
{
    (void)timestamp;
    (void)nmea;
    if (length >= 0) {
        printf("Report nmea success\n");
        return HDF_SUCCESS;
    }
    else{
        printf("Report nmea fail\n");
        return HDF_FAILURE;
    }
}

int32_t GnssCallbackImpl::ReportGnssCapabilities(unsigned int capabilities)
{
    (void)capabilities;
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
    (void)type;
    return HDF_SUCCESS;
}

int32_t GnssCallbackImpl::RequestPredictGnssData()
{
    return HDF_SUCCESS;
}

int32_t GnssCallbackImpl::ReportCachedLocation(const std::vector<LocationInfo>& gnssLocations)
{
    (void)gnssLocations;
    return HDF_SUCCESS;
}

int32_t GnssCallbackImpl::ReportGnssNiNotification(const GnssNiNotificationRequest& notification)
{
    (void)notification;
    return HDF_SUCCESS;
}
#endif

void LocationGnssTest::SetUpTestCase()
{
#ifdef FEATURE_GNSS_SUPPORT
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return;
    }
    if (!IsDeviceLoaded(GNSS_SERVICE_NAME)) {
        if (devmgr->LoadDevice(GNSS_SERVICE_NAME) != 0) {
            printf("Load gnss service failed!\n");
            return;
        }
    }
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    if (!IsDeviceLoaded(AGNSS_SERVICE_NAME)) {
        if (devmgr->LoadDevice(AGNSS_SERVICE_NAME) != 0) {
            printf("Load agnss service failed!\n");
            return;
        }
    }
#endif
#ifdef HDF_DRIVERS_INTERFACE_GEOFENCE_ENABLE
    if (!IsDeviceLoaded(GEOFENCE_SERVICE_NAME)) {
        if (devmgr->LoadDevice(GEOFENCE_SERVICE_NAME) != 0) {
            printf("Load geofence service failed!\n");
            return;
        }
    }
#endif
    g_ignssHci = IGnssInterface::Get();
#endif
}

void LocationGnssTest::TearDownTestCase()
{
#ifdef FEATURE_GNSS_SUPPORT
    auto devmgr = HDI::DeviceManager::V1_0::IDeviceManager::Get();
    if (devmgr == nullptr) {
        printf("fail to get devmgr.\n");
        return;
    }
    if (devmgr->UnloadDevice(GNSS_SERVICE_NAME) != 0) {
        printf("Load gnss service failed!\n");
        return;
    }
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    if (devmgr->UnloadDevice(AGNSS_SERVICE_NAME) != 0) {
        printf("Load agnss service failed!\n");
        return;
    }
#endif
#ifdef HDF_DRIVERS_INTERFACE_GEOFENCE_ENABLE
    if (devmgr->UnloadDevice(GEOFENCE_SERVICE_NAME) != 0) {
        printf("Load geofence service failed!\n");
        return;
    }
#endif
#endif
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
#ifdef FEATURE_GNSS_SUPPORT
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
#endif
}


/**
  * @tc.name: SetGnssConfigPara0100
  * @tc.desc: Setting gnss configuration parameters.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_SetGnssConfigPara_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssConfigPara para;
    para.gnssBasic.minInterval = 10;
    para.gnssBasic.gnssMode = GnssWorkingMode::GNSS_WORKING_MODE_STANDALONE;
    para.gnssCaching.interval = 20;
    para.gnssCaching.fifoFullNotify = true;
    int32_t ret = g_ignssHci->SetGnssConfigPara(para);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}


/**
  * @tc.name: SetGnssReferenceInfo0100
  * @tc.desc: Inject reference information to the GNSS module.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_SetGnssReferenceInfo_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssRefInfo refInfo;
    refInfo.type = GnssRefInfoType::GNSS_REF_INFO_TIME;
    refInfo.time.time = 50;
    refInfo.time.elapsedRealtime = 100;
    refInfo.time.uncertaintyOfTime = 200;
    refInfo.gnssLocation.latitude = 39.56;
    refInfo.gnssLocation.longitude = 116.20;
    refInfo.gnssLocation.horizontalAccuracy = 90;
    refInfo.bestLocation.latitude = 39.58;
    refInfo.bestLocation.longitude = 116.45;
    refInfo.bestLocation.altitude = 110;
    refInfo.bestLocation.horizontalAccuracy = 60;
    refInfo.bestLocation.speed = 60;
    refInfo.bestLocation.bearing = 60;
    refInfo.bestLocation.timeForFix = 60;
    refInfo.bestLocation.timeSinceBoot = 60;
    int32_t ret = g_ignssHci->SetGnssReferenceInfo(refInfo);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: StopGnss0100
  * @tc.desc: Stop the navigation function in normal mode.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_StopGnss_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssStartType stoptype = GnssStartType::GNSS_START_TYPE_NORMAL;
    int32_t ret = g_ignssHci->StopGnss(stoptype);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: DeleteAuxiliaryData0100
  * @tc.desc: Delete the specified auxiliary data.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_DeleteAuxiliaryData_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    GnssAuxiliaryDataType auxdata = GnssAuxiliaryDataType::GNSS_AUXILIARY_DATA_EPHEMERIS;
    int32_t ret = g_ignssHci->DeleteAuxiliaryData(auxdata);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}


/**
  * @tc.name: SetPredictGnssData0100
  * @tc.desc: Ingesting Predict Gnss Data.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_SetPredictGnssData_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    const std::string str1 = "testing";
    int32_t ret = g_ignssHci->SetPredictGnssData(str1);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: GetCachedGnssLocationsSize0100
  * @tc.desc: Obtain the number of locations that can be cached by the GNSS module.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_GetCachedGnssLocationsSize_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    int cach_size = 60;
    int32_t ret = g_ignssHci->GetCachedGnssLocationsSize(cach_size);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: GetCachedGnssLocations0100
  * @tc.desc: Request to obtain all the location information in the GNSS cache at a time and clear the cache buffer.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_GetCachedGnssLocations_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    int32_t ret = g_ignssHci->GetCachedGnssLocations();
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: DisableGnss0100
  * @tc.desc: Disable the GNSS module.
  * @tc.type: FUNC
  */
HWTEST_F(LocationGnssTest, SUB_DriverSystem_DisableGnss_0100, TestSize.Level1)
{
#ifdef FEATURE_GNSS_SUPPORT
    if (g_ignssHci == nullptr) {
        ASSERT_NE(nullptr, g_ignssHci);
        return;
    }
    int32_t ret = g_ignssHci->DisableGnss();
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}
