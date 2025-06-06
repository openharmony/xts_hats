/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_LOCATION_GEOFENCE_V2_0_GEOFENCECALLBACKIMPL_H
#define OHOS_HDI_LOCATION_GEOFENCE_V2_0_GEOFENCECALLBACKIMPL_H

#ifdef HDF_DRIVERS_INTERFACE_GEOFENCE_ENABLE
#include <hdf_base.h>
#include <v2_0/igeofence_callback.h>

namespace OHOS {
namespace HDI {
namespace Location {
namespace Geofence {
namespace V2_0 {
class GeofenceCallbackImpl : public IGeofenceCallback {
public:
    GeofenceCallbackImpl() {}
    virtual ~GeofenceCallbackImpl() {}
    int32_t ReportGeofenceAvailability(bool isAvailable) override;
    int32_t ReportGeofenceEvent(int32_t fenceIndex, const LocationInfo &location, GeofenceEvent event,
                                int64_t timestamp) override;
    int32_t ReportGeofenceOperateResult(int32_t fenceIndex, GeofenceOperateType type,
                                        GeofenceOperateResult result) override;
};
} // namespace V2_0
} // namespace Geofence
} // namespace Location
} // namespace HDI
} // namespace OHOS
#endif
#endif