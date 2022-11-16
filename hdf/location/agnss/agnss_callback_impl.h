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

#ifndef OHOS_HDI_LOCATION_AGNSS_V1_0_AGNSSCALLBACKIMPL_H
#define OHOS_HDI_LOCATION_AGNSS_V1_0_AGNSSCALLBACKIMPL_H

#include <hdf_base.h>
#include <v1_0/ia_gnss_callback.h>
#include "core_service_client.h"
#include "cell_information.h"

namespace OHOS {
namespace HDI {
namespace Location {
namespace Agnss {
namespace V1_0 {
class AgnssCallbackImpl : public IAGnssCallback {
public:
    AgnssCallbackImpl()
    {}
    virtual ~AgnssCallbackImpl()
    {}
    int32_t RequestSetUpAgnssDataLink(const AGnssDataLinkRequest& request) override;
    int32_t RequestSubscriberSetId(SubscriberSetIdType type) override;
    int32_t RequestAgnssRefInfo() override;
private:
    void JudgmentDataGsm(AGnssRefInfo& refInfo, sptr<Telephony::CellInformation> infoItem);
    void JudgmentDataUmts(AGnssRefInfo& refInfo, sptr<Telephony::CellInformation> infoItem);
    void JudgmentDataLte(AGnssRefInfo& refInfo, sptr<Telephony::CellInformation> infoItem);
    void JudgmentDataNr(AGnssRefInfo& refInfo, sptr<Telephony::CellInformation> infoItem);
    std::string Str16ToStr8(std::u16string str);
};
} // V1_0
} // Agnss
} // Location
} // HDI
} // OHOS

#endif // OHOS_HDI_LOCATION_AGNSS_V1_0_AGNSSCALLBACKIMPL_H

