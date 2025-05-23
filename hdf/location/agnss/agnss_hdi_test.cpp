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
#include <iostream>
#include <cmath>
#include <cstdio>
#include <unistd.h>
#include <gtest/gtest.h>
#include <securec.h>
#ifdef FEATURE_GNSS_SUPPORT
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
#include <osal_mem.h>
#include "osal_time.h"
#include "hdf_base.h"
#include "hdf_log.h"
#include "hdf_sbuf.h"
#include "v2_0/ia_gnss_interface.h"
#include "v2_0/ignss_interface.h"
#include "agnss_callback_impl.h"
#include "cellular_data_client.h"
#include "gnss_callback_impl.h"
#endif
#endif

#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
using namespace OHOS::HDI::Location::Agnss::V2_0;
using namespace OHOS::HDI::Location::Gnss::V2_0;
#endif
using namespace std;
using namespace testing::ext;

namespace {
    #ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    using OHOS::Telephony::CellInformation;
    sptr<IAGnssInterface> g_iagnssHci = nullptr; 
    sptr<IGnssInterface> g_ignssHci = nullptr;
    constexpr const char *AGNSS_SERVICE_NAME = "agnss_interface_service";
    constexpr const char *GNSS_SERVICE_NAME = "gnss_interface_service";
    constexpr const char *GEOFENCE_SERVICE_NAME = "geofence_interface_service";
    constexpr const char *LOCATION_HOST_NAME = "location_host";
    const char DEFAULT_STRING[] = "error";
    const std::wstring DEFAULT_WSTRING = L"error";
    const std::u16string DEFAULT_USTRING = u"error";
    #endif
}

class LocationAgnssTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
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

int32_t AgnssCallbackImpl::RequestSetUpAgnssDataLink(const AGnssDataLinkRequest& request)
{
    printf("AGnssEventCallback::RequestSetUpAgnssDataLink. agnsstype:%d, setUpType:%d\n",
        static_cast<int>(request.agnssType), static_cast<int>(request.setUpType));
    return HDF_SUCCESS;
}

int32_t AgnssCallbackImpl::RequestSubscriberSetId(SubscriberSetIdType type)
{
    printf("AGnssEventCallback::RequestSubscriberSetId. type:%d\n", static_cast<int>(type));
    int slotId = Telephony::CellularDataClient::GetInstance().GetDefaultCellularDataSlotId();
    std::u16string imsi;
    DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance().GetIMSI(slotId, imsi);
    SubscriberSetId setId;
    setId.type = HDI::Location::Agnss::V2_0::AGNSS_SETID_TYPE_IMSI;
    setId.id = Str16ToStr8(imsi);
    if (g_iagnssHci == nullptr) {
        printf("g_iagnssHci is null!\n");
        return HDF_FAILURE;
    }
    g_iagnssHci->SetSubscriberSetId(setId);
    return HDF_SUCCESS;
}

int32_t AgnssCallbackImpl::RequestAgnssRefInfo(AGnssRefInfoType type)
{
    if (g_iagnssHci == nullptr) {
        printf("g_iagnssHci is null!\n");
        return HDF_FAILURE;
    }
    int slotId = Telephony::CellularDataClient::GetInstance().GetDefaultCellularDataSlotId();
    std::vector<sptr<CellInformation>> cellInformations;
    DelayedRefSingleton<Telephony::CoreServiceClient>::GetInstance().GetCellInfoList(slotId, cellInformations);
    printf("RequestAgnssRefInfo,cellInformations.\n");
    for (sptr<CellInformation> infoItem : cellInformations) {
        if (!infoItem->GetIsCamped()) {
            printf("GetIsCamped return false\n");
            continue;
        }
        AGnssRefInfo refInfo;
        CellInformation::CellType cellType = infoItem->GetNetworkType();
        refInfo.type = type;
        switch (cellType) {
            case CellInformation::CellType::CELL_TYPE_GSM: {
                JudgmentDataGsm(refInfo, infoItem);
                break;
            }
            case CellInformation::CellType::CELL_TYPE_LTE: {
                JudgmentDataLte(refInfo, infoItem);
                break;
            }
            case CellInformation::CellType::CELL_TYPE_CDMA:
            case CellInformation::CellType::CELL_TYPE_WCDMA:
            case CellInformation::CellType::CELL_TYPE_TDSCDMA: {
                JudgmentDataUmts(refInfo, infoItem);
                break;
            }
            case CellInformation::CellType::CELL_TYPE_NR: {
                JudgmentDataNr(refInfo, infoItem);
                break;
            }
            default:
                break;
        }
        g_iagnssHci->SetAgnssRefInfo(refInfo);
        break;
    }
    return HDF_SUCCESS;
}

void AgnssCallbackImpl::JudgmentDataGsm(AGnssRefInfo& refInfo, sptr<CellInformation> infoItem)
{
    auto gsmCellInfo = static_cast<Telephony::GsmCellInformation *>(infoItem.GetRefPtr());
    if (gsmCellInfo != nullptr) {
        refInfo.cellId.type = HDI::Location::Agnss::V2_0::CELLID_TYPE_GSM;
        refInfo.cellId.mcc = static_cast<unsigned short>(std::stoi(gsmCellInfo->GetMcc()));
        refInfo.cellId.mnc = static_cast<unsigned short>(std::stoi(gsmCellInfo->GetMnc()));
        refInfo.cellId.lac = static_cast<unsigned short>(gsmCellInfo->GetLac());
        refInfo.cellId.cid = static_cast<unsigned int>(gsmCellInfo->GetCellId());
    }
}

void AgnssCallbackImpl::JudgmentDataLte(AGnssRefInfo& refInfo, sptr<CellInformation> infoItem)
{
    auto lteCellInfo = static_cast<Telephony::LteCellInformation *>(infoItem.GetRefPtr());
    if (lteCellInfo != nullptr) {
        refInfo.cellId.type = HDI::Location::Agnss::V2_0::CELLID_TYPE_LTE;
        refInfo.cellId.mcc = static_cast<unsigned short>(std::stoi(lteCellInfo->GetMcc()));
        refInfo.cellId.mnc = static_cast<unsigned short>(std::stoi(lteCellInfo->GetMnc()));
        refInfo.cellId.tac = static_cast<unsigned short>(lteCellInfo->GetTac());
        refInfo.cellId.cid = static_cast<unsigned int>(lteCellInfo->GetCellId());
        refInfo.cellId.pcid = static_cast<unsigned short>(lteCellInfo->GetPci());
    }
}

void AgnssCallbackImpl::JudgmentDataNr(AGnssRefInfo& refInfo, sptr<CellInformation> infoItem)
{
    auto nrCellInfo = static_cast<Telephony::NrCellInformation *>(infoItem.GetRefPtr());
    if (nrCellInfo != nullptr) {
        refInfo.cellId.type = HDI::Location::Agnss::V2_0::CELLID_TYPE_NR;
        refInfo.cellId.mcc = static_cast<unsigned short>(std::stoi(nrCellInfo->GetMcc()));
        refInfo.cellId.mnc = static_cast<unsigned short>(std::stoi(nrCellInfo->GetMnc()));
        refInfo.cellId.tac = static_cast<unsigned short>(nrCellInfo->GetTac());
        refInfo.cellId.cid = static_cast<unsigned int>(nrCellInfo->GetCellId());
        refInfo.cellId.pcid = static_cast<unsigned short>(nrCellInfo->GetPci());
        refInfo.cellId.nci = static_cast<unsigned int>(nrCellInfo->GetNci());
    }
}

void AgnssCallbackImpl::JudgmentDataUmts(AGnssRefInfo& refInfo, sptr<CellInformation> infoItem)
{
    auto wcdmaCellInfo = static_cast<Telephony::WcdmaCellInformation *>(infoItem.GetRefPtr());
    if (wcdmaCellInfo != nullptr) {
        refInfo.cellId.type = HDI::Location::Agnss::V2_0::CELLID_TYPE_UMTS;
        refInfo.cellId.mcc = static_cast<unsigned short>(std::stoi(wcdmaCellInfo->GetMcc()));
        refInfo.cellId.mnc = static_cast<unsigned short>(std::stoi(wcdmaCellInfo->GetMnc()));
        refInfo.cellId.lac = static_cast<unsigned short>(wcdmaCellInfo->GetLac());
        refInfo.cellId.cid = static_cast<unsigned int>(wcdmaCellInfo->GetCellId());
    }
}

std::string AgnssCallbackImpl::Str16ToStr8(std::u16string str)
{
    if (str == DEFAULT_USTRING) {
        return DEFAULT_STRING;
    }
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert(DEFAULT_STRING);
    std::string result = convert.to_bytes(str);
    return result == DEFAULT_STRING ? "" : result;
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
    (void)nmea;
    (void)timestamp;
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

void LocationAgnssTest::SetUpTestCase()
{
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
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
    if (!IsDeviceLoaded(AGNSS_SERVICE_NAME)) {
        if (devmgr->LoadDevice(AGNSS_SERVICE_NAME) != 0) {
            printf("Load agnss service failed!\n");
            return;
        }
    }
    if (!IsDeviceLoaded(GEOFENCE_SERVICE_NAME)) {
        if (devmgr->LoadDevice(GEOFENCE_SERVICE_NAME) != 0) {
            printf("Load geofence service failed!\n");
            return;
        }
    }
    g_iagnssHci = IAGnssInterface::Get();
    g_ignssHci = IGnssInterface::Get();
    sptr<IGnssCallback> gnss_callback = new (std::nothrow) GnssCallbackImpl();
    if (gnss_callback == nullptr) {
        ASSERT_NE(nullptr, gnss_callback);
        return;
    }
    g_ignssHci->EnableGnss(gnss_callback);
#endif
}

void LocationAgnssTest::TearDownTestCase()
{
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
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
    g_ignssHci->DisableGnss();
#endif
}

void LocationAgnssTest::SetUp()
{
}

void LocationAgnssTest::TearDown()
{
}


/**
  * @tc.name: SetAgnssServer0001
  * @tc.desc: Set the Agnss Server Information.
  * @tc.type: FUNC
  */
HWTEST_F(LocationAgnssTest, SUB_DriverSystem_SetAgnssServer_0100, TestSize.Level1)
{
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    if (g_iagnssHci == nullptr) {
        ASSERT_NE(nullptr, g_iagnssHci);
        return;
    }
    AGnssServerInfo server;
    server.type = AGnssUserPlaneProtocol::AGNSS_TYPE_SUPL;
    server.server = "test";
    server.port = 80001;
    int32_t ret = g_iagnssHci->SetAgnssServer(server);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: SetSubscriberSetId0001
  * @tc.desc: Set Subscriber Identity.
  * @tc.type: FUNC
  */
HWTEST_F(LocationAgnssTest, SUB_DriverSystem_SetSubscriberSetId_0100, TestSize.Level1)
{
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    if (g_iagnssHci == nullptr) {
        ASSERT_NE(nullptr, g_iagnssHci);
        return;
    }
    SubscriberSetId id;
    id.type = SubscriberSetIdType::AGNSS_SETID_TYPE_NULL;
    id.id = "111";
    int32_t ret = g_iagnssHci->SetSubscriberSetId(id);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: SetAgnssRefInfo0001
  * @tc.desc: Ingesting Reference Information.
  * @tc.type: FUNC
  */
HWTEST_F(LocationAgnssTest, SUB_DriverSystem_SetAgnssRefInfo_0100, TestSize.Level1)
{
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    if (g_iagnssHci == nullptr) {
        ASSERT_NE(nullptr, g_iagnssHci);
        return;
    }
    AGnssRefInfo refInfo;
    refInfo.type = AGnssRefInfoType::ANSS_REF_INFO_TYPE_CELLID;
    refInfo.cellId.type = CellIdType::CELLID_TYPE_GSM;
    refInfo.cellId.mcc = 100;
    refInfo.cellId.mnc = 8;
    refInfo.cellId.lac = 20;
    refInfo.cellId.cid = 50;
    refInfo.cellId.tac = 60;
    refInfo.cellId.pcid = 80;
    refInfo.cellId.nci = 90;
    int32_t ret = g_iagnssHci->SetAgnssRefInfo(refInfo);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}

/**
  * @tc.name: SetAgnssCallback0001
  * @tc.desc: Set the agnss callback function.
  * @tc.type: FUNC
  */
HWTEST_F(LocationAgnssTest, SUB_DriverSystem_SetAgnssCallback_0100, TestSize.Level1)
{
#ifdef HDF_DRIVERS_INTERFACE_AGNSS_ENABLE
    if (g_iagnssHci == nullptr) {
        ASSERT_NE(nullptr, g_iagnssHci);
        return;
    }
    sptr<IAGnssCallback> agnss_callback = new (std::nothrow) AgnssCallbackImpl();
    if (agnss_callback == nullptr) {
        ASSERT_NE(nullptr, agnss_callback);
        return;
    }
    int32_t ret = g_iagnssHci->SetAgnssCallback(agnss_callback);
    EXPECT_EQ(HDF_SUCCESS, ret);
#endif
}
