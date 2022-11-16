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

#ifndef OHOS_HDI_LOCATION_GNSS_V1_0_GNSSCALLBACKIMPL_H
#define OHOS_HDI_LOCATION_GNSS_V1_0_GNSSCALLBACKIMPL_H

#include <hdf_base.h>
#include <string>
#include <v1_0/ignss_callback.h>


namespace OHOS {
namespace HDI {
namespace Location {
namespace Gnss {
namespace V1_0 {
class GnssCallbackImpl : public IGnssCallback {
public:
    GnssCallbackImpl()
    {}
    virtual ~GnssCallbackImpl()
    {}
    int32_t ReportLocation(const LocationInfo& location) override;
    int32_t ReportGnssWorkingStatus(GnssWorkingStatus status) override;
    int32_t ReportNmea(int64_t timestamp, const std::string& nmea, int32_t length) override;
    int32_t ReportGnssCapabilities(GnssCapabilities capabilities) override;
    int32_t ReportSatelliteStatusInfo(const SatelliteStatusInfo& info) override;
    int32_t RequestGnssReferenceInfo(GnssRefInfoType type) override;
    int32_t RequestPredictGnssData() override;
    int32_t ReportCachedLocation(const std::vector<LocationInfo>& gnssLocations) override;
};
} // V1_0
} // Gnss
} // Location
} // HDI
} // OHOS

#endif // OHOS_HDI_LOCATION_GNSS_V1_0_GNSSCALLBACKIMPL_H

