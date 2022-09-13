/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "cmath"
#include "condition_variable"
#include "cstdio"
#include "gtest/gtest.h"
#include "hdf_base.h"
#include "hdf_log.h"
#include "map"
#include "mutex"
#include "securec.h"
#include "stdlib.h"
#include "thread"
#include "unistd.h"
#include "v1_0/iril_interface.h"

using namespace OHOS::HDI::Ril::V1_0;
using namespace testing::ext;

enum class HdiId {
    HREQ_NONE = -1,
    HREQ_CALL_BASE = 0,
    HREQ_CALL_GET_CALL_LIST,
    HREQ_CALL_DIAL,
    HREQ_CALL_HANGUP,
    HREQ_CALL_REJECT,
    HREQ_CALL_ANSWER,
    HREQ_CALL_HOLD_CALL, // call hold value 6
    HREQ_CALL_UNHOLD_CALL, // call active value 6
    HREQ_CALL_SWITCH_CALL,
    HREQ_CALL_COMBINE_CONFERENCE,
    HREQ_CALL_SEPARATE_CONFERENCE, // Keep all other calls except the xth call
    HREQ_CALL_CALL_SUPPLEMENT,
    HREQ_CALL_SEND_DTMF,
    HREQ_CALL_START_DTMF,
    HREQ_CALL_STOP_DTMF,
    HREQ_CALL_SET_CLIP,
    HREQ_CALL_GET_CLIP,
    HREQ_CALL_GET_CALL_WAITING,
    HREQ_CALL_SET_CALL_WAITING,
    HREQ_CALL_GET_CALL_RESTRICTION,
    HREQ_CALL_SET_CALL_RESTRICTION,
    HREQ_CALL_GET_CALL_TRANSFER_INFO,
    HREQ_CALL_SET_CALL_TRANSFER_INFO,
    HREQ_CALL_GET_CLIR,
    HREQ_CALL_SET_CLIR,
    HREQ_CALL_GET_CALL_PREFERENCE,
    HREQ_CALL_SET_CALL_PREFERENCE,
    HREQ_CALL_SET_USSD,
    HREQ_CALL_GET_USSD,
    HREQ_CALL_SET_MUTE,
    HREQ_CALL_GET_MUTE,
    HREQ_CALL_GET_EMERGENCY_LIST,
    HREQ_CALL_SET_EMERGENCY_LIST,
    HREQ_CALL_GET_FAIL_REASON,
    HREQ_CALL_SET_BARRING_PASSWORD,

    HREQ_SMS_BASE = 100,
    HREQ_SMS_SEND_GSM_SMS,
    HREQ_SMS_SEND_CDMA_SMS,
    HREQ_SMS_ADD_SIM_MESSAGE,
    HREQ_SMS_DEL_SIM_MESSAGE,
    HREQ_SMS_UPDATE_SIM_MESSAGE,
    HREQ_SMS_SEND_SMS_MORE_MODE,
    HREQ_SMS_SEND_SMS_ACK,
    HREQ_SMS_SET_SMSC_ADDR,
    HREQ_SMS_GET_SMSC_ADDR,
    HREQ_SMS_SET_CB_CONFIG,
    HREQ_SMS_GET_CB_CONFIG,
    HREQ_SMS_GET_CDMA_CB_CONFIG,
    HREQ_SMS_SET_CDMA_CB_CONFIG,
    HREQ_SMS_ADD_CDMA_SIM_MESSAGE,
    HREQ_SMS_DEL_CDMA_SIM_MESSAGE,
    HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE,

    HREQ_SIM_BASE = 200,
    HREQ_SIM_GET_SIM_STATUS,
    HREQ_SIM_GET_IMSI,
    HREQ_SIM_GET_SIM_IO,
    HREQ_SIM_GET_SIM_LOCK_STATUS,
    HREQ_SIM_SET_SIM_LOCK,
    HREQ_SIM_CHANGE_SIM_PASSWORD,
    HREQ_SIM_UNLOCK_PIN,
    HREQ_SIM_UNLOCK_PUK,
    HREQ_SIM_GET_SIM_PIN_INPUT_TIMES,
    HREQ_SIM_UNLOCK_PIN2,
    HREQ_SIM_UNLOCK_PUK2,
    HREQ_SIM_GET_SIM_PIN2_INPUT_TIMES,
    HREQ_SIM_SET_ACTIVE_SIM,
    HREQ_SIM_RADIO_PROTOCOL,
    HREQ_SIM_STK_SEND_TERMINAL_RESPONSE,
    HREQ_SIM_STK_SEND_ENVELOPE,
    HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT,
    HREQ_SIM_STK_IS_READY,
    HREQ_SIM_OPEN_LOGICAL_CHANNEL,
    HREQ_SIM_CLOSE_LOGICAL_CHANNEL,
    HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL,
    HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL,
    HREQ_SIM_AUTHENTICATION,
    HREQ_SIM_UNLOCK_SIM_LOCK,

    HREQ_DATA_BASE = 300,
    HREQ_DATA_SET_INIT_APN_INFO,
    HREQ_DATA_DEACTIVATE_PDP_CONTEXT,
    HREQ_DATA_ACTIVATE_PDP_CONTEXT,
    HREQ_DATA_GET_PDP_CONTEXT_LIST,
    HREQ_DATA_GET_LINK_BANDWIDTH_INFO,
    HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE,
    HREQ_DATA_SET_DATA_PROFILE_INFO,
    HREQ_DATA_SET_DATA_PERMITTED,

    HREQ_NETWORK_BASE = 400,
    HREQ_NETWORK_GET_SIGNAL_STRENGTH,
    HREQ_NETWORK_GET_CS_REG_STATUS,
    HREQ_NETWORK_GET_PS_REG_STATUS,
    HREQ_NETWORK_GET_OPERATOR_INFO,
    HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION,
    HREQ_NETWORK_GET_NETWORK_SELECTION_MODE,
    HREQ_NETWORK_SET_NETWORK_SELECTION_MODE,
    HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST,
    HREQ_NETWORK_GET_CURRENT_CELL_INFO,
    HREQ_NETWORK_SET_PREFERRED_NETWORK,
    HREQ_NETWORK_GET_PREFERRED_NETWORK,
    HREQ_NETWORK_GET_RADIO_CAPABILITY,
    HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG,
    HREQ_NETWORK_SET_LOCATE_UPDATES,
    HREQ_NETWORK_SET_NOTIFICATION_FILTER,
    HREQ_NETWORK_SET_DEVICE_STATE,

    HREQ_COMMON_BASE = 500,
    HREQ_MODEM_SHUT_DOWN,
    HREQ_MODEM_SET_RADIO_STATUS,
    HREQ_MODEM_GET_RADIO_STATUS,
    HREQ_MODEM_GET_IMEI,
    HREQ_MODEM_GET_MEID,
    HREQ_MODEM_GET_BASEBAND_VERSION,
    HREQ_MODEM_GET_VOICE_RADIO,
    HREQ_MODEM_EXIT = 1000
};

enum class DeviceStateType { POWER_SAVE_MODE, CHARGING_STATE, LOW_DATA_STATE };

enum class NotificationFilter {
    NOTIFICATION_FILTER_ALL = -1,
    NOTIFICATION_FILTER_NONE = 0,
    NOTIFICATION_FILTER_SIGNAL_STRENGTH = 1,
    NOTIFICATION_FILTER_NETWORK_STATE = 2,
    NOTIFICATION_FILTER_DATA_CALL = 4,
    NOTIFICATION_FILTER_LINK_CAPACITY = 8,
    NOTIFICATION_FILTER_PHYSICAL_CHANNEL_CONFIG = 16
};

enum class RatType {
    NETWORK_TYPE_UNKNOWN = 0,
    NETWORK_TYPE_GSM,
    NETWORK_TYPE_CDMA,
    NETWORK_TYPE_WCDMA,
    NETWORK_TYPE_TDSCDMA,
    NETWORK_TYPE_LTE,
    NETWORK_TYPE_NR
};

enum class SelectionMode { MODE_TYPE_UNKNOWN = -1, MODE_TYPE_AUTO = 0, MODE_TYPE_MANUAL = 1 };

enum class PreferredNetworkMode {
    CORE_NETWORK_MODE_AUTO = 0,
    CORE_NETWORK_MODE_GSM = 1,
    CORE_NETWORK_MODE_WCDMA = 2,
    CORE_NETWORK_MODE_LTE = 3,
    CORE_NETWORK_MODE_LTE_WCDMA = 4,
    CORE_NETWORK_MODE_LTE_WCDMA_GSM = 5,
    CORE_NETWORK_MODE_WCDMA_GSM = 6,
    CORE_NETWORK_MODE_CDMA = 7,
    CORE_NETWORK_MODE_EVDO = 8,
    CORE_NETWORK_MODE_EVDO_CDMA = 9,
    CORE_NETWORK_MODE_WCDMA_GSM_EVDO_CDMA = 10,
    CORE_NETWORK_MODE_LTE_EVDO_CDMA = 11,
    CORE_NETWORK_MODE_LTE_WCDMA_GSM_EVDO_CDMA = 12,
    CORE_NETWORK_MODE_TDSCDMA = 13,
    CORE_NETWORK_MODE_TDSCDMA_GSM = 14,
    CORE_NETWORK_MODE_TDSCDMA_WCDMA = 15,
    CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM = 16,
    CORE_NETWORK_MODE_LTE_TDSCDMA = 17,
    CORE_NETWORK_MODE_LTE_TDSCDMA_GSM = 18,
    CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA = 19,
    CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM = 20,
    CORE_NETWORK_MODE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 21,
    CORE_NETWORK_MODE_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 22,
    CORE_NETWORK_MODE_NR = 31,
    CORE_NETWORK_MODE_NR_LTE = 32,
    CORE_NETWORK_MODE_NR_LTE_WCDMA = 33,
    CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM = 34,
    CORE_NETWORK_MODE_NR_LTE_EVDO_CDMA = 35,
    CORE_NETWORK_MODE_NR_LTE_WCDMA_GSM_EVDO_CDMA = 36,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA = 37,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_GSM = 38,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA = 39,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM = 40,
    CORE_NETWORK_MODE_NR_LTE_TDSCDMA_WCDMA_GSM_EVDO_CDMA = 41,
    CORE_NETWORK_MODE_MAX_VALUE = 99,
};

enum class SimMessageStatus {
    SIM_MESSAGE_STATUS_UNREAD = 0,
    SIM_MESSAGE_STATUS_READ = 1,
    SIM_MESSAGE_STATUS_UNSENT = 2,
    SIM_MESSAGE_STATUS_SENT = 3,
};

class RilCallback : public IRilCallback {
public:
    void NotifyAll();
    void WaitFor(int32_t timeoutSecond);
    bool GetBoolResult(HdiId hdiId_);
    void Clean();

    // Call
    int32_t CallStateUpdated(int32_t slotId, int32_t type) override;
    int32_t CallRingbackVoiceNotice(int32_t slotId, const IRingbackVoice &ringbackVoice) override;
    int32_t CallSrvccStatusNotice(int32_t slotId, const ISrvccStatus &srvccStatus) override;
    int32_t CallUssdNotice(int32_t slotId, const IUssdNoticeInfo &ussdNoticeInfo) override;
    int32_t CallSsNotice(int32_t slotId, const ISsNoticeInfo &ssNoticeInfo) override;
    int32_t CallEmergencyNotice(int32_t slotId, const IEmergencyInfoList &emergencyInfoList) override;
    int32_t SetEmergencyCallListResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetEmergencyCallListResponse(const IHRilRadioResponseInfo &responseInfo,
        const IEmergencyInfoList &emergencyInfoList) override;
    int32_t GetCallListResponse(const IHRilRadioResponseInfo &responseInfo, const ICallInfoList &callList) override;
    int32_t DialResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t HangupResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t RejectResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t AnswerResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t HoldCallResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t UnHoldCallResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SwitchCallResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetClipResponse(const IHRilRadioResponseInfo &responseInfo,
        const IGetClipResult &getClipResult) override;
    int32_t SetClipResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t CombineConferenceResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SeparateConferenceResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t CallSupplementResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetCallWaitingResponse(const IHRilRadioResponseInfo &responseInfo,
        const ICallWaitResult &callWaitResult) override;
    int32_t SetCallWaitingResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetCallTransferInfoResponse(const IHRilRadioResponseInfo &responseInfo,
        const ICallForwardQueryInfoList &cFQueryList) override;
    int32_t SetCallTransferInfoResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetCallRestrictionResponse(const IHRilRadioResponseInfo &responseInfo,
        const ICallRestrictionResult &result) override;
    int32_t SetCallRestrictionResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetClirResponse(const IHRilRadioResponseInfo &responseInfo,
        const IGetClirResult &getClirResult) override;
    int32_t SetClirResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t StartDtmfResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SendDtmfResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t StopDtmfResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetCallPreferenceModeResponse(const IHRilRadioResponseInfo &responseInfo,
        int32_t mode) override;
    int32_t SetCallPreferenceModeResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetUssdResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetUssdResponse(const IHRilRadioResponseInfo &responseInfo, int32_t cusd) override;
    int32_t SetMuteResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetMuteResponse(const IHRilRadioResponseInfo &responseInfo, int32_t mute) override;
    int32_t GetCallFailReasonResponse(const IHRilRadioResponseInfo &responseInfo, int32_t callFail) override;
    int32_t SetBarringPasswordResponse(const IHRilRadioResponseInfo &responseInfo) override;

    // Data
    int32_t PdpContextListUpdated(int32_t slotId, const IDataCallResultList &dataCallResultList) override
    {
        return 0;
    }
    int32_t ActivatePdpContextResponse(
        const IHRilRadioResponseInfo &responseInfo, const ISetupDataCallResultInfo &setupDataCallResultInfo) override;
    int32_t DeactivatePdpContextResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetPdpContextListResponse(
        const IHRilRadioResponseInfo &responseInfo, const IDataCallResultList &dataCallResultList) override;
    int32_t SetInitApnInfoResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetLinkBandwidthReportingRuleResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetLinkBandwidthInfoResponse(
        const IHRilRadioResponseInfo &responseInfo, const IDataLinkBandwidthInfo &dataLinkBandwidthInfo) override;
    int32_t SetDataPermittedResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetDataProfileInfoResponse(const IHRilRadioResponseInfo &responseInfo);

    // Modem
    int32_t RadioStateUpdated(int32_t slotId, int32_t state) override;
    int32_t VoiceRadioTechUpdated(
        int32_t slotId, const IVoiceRadioTechnology &voiceRadioTechnology) override;
    int32_t ShutDownResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetRadioStateResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetRadioStateResponse(const IHRilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t GetImeiResponse(
        const IHRilRadioResponseInfo &responseInfo, const std::string &imei) override;
    int32_t GetMeidResponse(
        const IHRilRadioResponseInfo &responseInfo, const std::string &meid) override;
    int32_t GetVoiceRadioTechnologyResponse(const IHRilRadioResponseInfo &responseInfo,
        const IVoiceRadioTechnology &voiceRadioTechnology) override;
    int32_t GetBasebandVersionResponse(
        const IHRilRadioResponseInfo &responseInfo, const std::string &basebandVersion) override;

    // Sim notice
    int32_t SimStateUpdated(int32_t slotId) override;
    int32_t SimStkSessionEndNotify(int32_t slotId) override;
    int32_t SimStkProactiveNotify(int32_t slotId, const std::string &response) override;
    int32_t SimStkAlphaNotify(int32_t slotId, const std::string &response) override;
    int32_t SimStkEventNotify(int32_t slotId, const std::string &response) override;
    int32_t SimStkCallSetupNotify(int32_t slotId) override;
    int32_t SimRefreshNotify(int32_t slotId) override;
    // Sim response
    int32_t GetSimIOResponse(const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result) override;
    int32_t GetSimStatusResponse(const IHRilRadioResponseInfo &responseInfo, const ICardStatusInfo &result) override;
    int32_t GetImsiResponse(const IHRilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t GetSimLockStatusResponse(const IHRilRadioResponseInfo &responseInfo, int32_t simLockStatus) override;
    int32_t SetSimLockResponse(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;
    int32_t ChangeSimPasswordResponse(
        const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;
    int32_t UnlockPinResponse(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;
    int32_t UnlockPukResponse(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;
    int32_t UnlockPin2Response(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;
    int32_t UnlockPuk2Response(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;
    int32_t SetActiveSimResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendTerminalResponseResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendEnvelopeResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendCallSetupRequestResultResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SimStkIsReadyResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetRadioProtocolResponse(
        const IHRilRadioResponseInfo &responseInfo, const ISimProtocolResponse &pSimProtocol) override;
    int32_t SimOpenLogicalChannelResponse(const IHRilRadioResponseInfo &responseInfo,
        const IOpenLogicalChannelResponse &pOpenLogicalChannelResponse) override;
    int32_t SimCloseLogicalChannelResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SimTransmitApduLogicalChannelResponse(
        const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result) override;
    int32_t SimTransmitApduBasicChannelResponse(
        const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result) override;
    int32_t SimAuthenticationResponse(
        const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result) override;
    int32_t UnlockSimLockResponse(
        const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus) override;

    // Network
    int32_t NetworkCsRegStatusUpdated(int32_t slotId, const ICsRegStatusInfo &csRegStatusInfo) override;
    int32_t NetworkPsRegStatusUpdated(int32_t slotId, const IPsRegStatusInfo &psRegStatusInfo) override;
    int32_t SignalStrengthUpdated(int32_t slotId, const IRssi &rssi) override;
    int32_t NetworkTimeZoneUpdated(int32_t slotId, const std::string &timeZoneStr) override;
    int32_t NetworkTimeUpdated(int32_t slotId, const std::string &timeStr) override;
    int32_t NetworkPhyChnlCfgUpdated(int32_t slotId, const IChannelConfigInfoList &channelConfigInfoList) override;
    int32_t NetworkCurrentCellUpdated(int32_t slotId, const ICellListCurrentInfo &cellListCurrentInfo) override;
    int32_t GetSignalStrengthResponse(const IHRilRadioResponseInfo &responseInfo, const IRssi &rssi) override;
    int32_t GetCsRegStatusResponse(const IHRilRadioResponseInfo &responseInfo,
        const ICsRegStatusInfo &csRegStatusInfo) override;
    int32_t GetPsRegStatusResponse(const IHRilRadioResponseInfo &responseInfo,
        const IPsRegStatusInfo &psRegStatusInfo) override;
    int32_t GetOperatorInfoResponse(const IHRilRadioResponseInfo &responseInfo,
        const IOperatorInfo &operatorInfo) override;
    int32_t GetNetworkSearchInformationResponse(const IHRilRadioResponseInfo &responseInfo,
        const IAvailableNetworkList &availableNetworkList) override;
    int32_t GetNetworkSelectionModeResponse(const IHRilRadioResponseInfo &responseInfo,
        const ISetNetworkModeInfo &setNetworkModeInfo) override;
    int32_t SetNetworkSelectionModeResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetNeighboringCellInfoListResponse(const IHRilRadioResponseInfo &responseInfo,
        const ICellListNearbyInfo &cellInfoList) override;
    int32_t GetCurrentCellInfoResponse(const IHRilRadioResponseInfo &responseInfo,
        const ICellListCurrentInfo &cellInfoList) override;
    int32_t SetPreferredNetworkResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetPreferredNetworkResponse(const IHRilRadioResponseInfo &responseInfo,
        const IPreferredNetworkTypeInfo &preferredNetworkTypeInfo) override;
    int32_t GetRadioCapabilityResponse(const IHRilRadioResponseInfo &responseInfo,
        const IRadioCapabilityInfo &radioCapabilityInfo) override;
    int32_t GetPhysicalChannelConfigResponse(const IHRilRadioResponseInfo &responseInfo,
        const IChannelConfigInfoList &channelConfigInfoList) override;
    int32_t SetLocateUpdatesResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetNotificationFilterResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetDeviceStateResponse(const IHRilRadioResponseInfo &responseInfo) override;

    // Sms
    int32_t NewSmsNotify(int32_t slotId, const ISmsMessageInfo &smsMessageInfo) override;
    int32_t NewCdmaSmsNotify(int32_t slotId, const ISmsMessageInfo &smsMessageInfo) override;
    int32_t SmsStatusReportNotify(int32_t slotId, const ISmsMessageInfo &smsMessageInfo) override;
    int32_t NewSmsStoredOnSimNotify(int32_t slotId, int32_t recordNumber, int32_t indicationType) override;
    int32_t CBConfigNotify(int32_t slotId, const ICBConfigReportInfo &cellBroadConfigReportInfo) override;
    int32_t SendGsmSmsResponse(
        const IHRilRadioResponseInfo &responseInfo, const ISendSmsResultInfo &sendSmsResultInfo) override;
    int32_t SendCdmaSmsResponse(
        const IHRilRadioResponseInfo &responseInfo, const ISendSmsResultInfo &sendSmsResultInfo) override;
    int32_t AddSimMessageResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t DelSimMessageResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t UpdateSimMessageResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t AddCdmaSimMessageResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t DelCdmaSimMessageResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t UpdateCdmaSimMessageResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t SetSmscAddrResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetSmscAddrResponse(
        const IHRilRadioResponseInfo &responseInfo, const IServiceCenterAddress &serviceCenterAddress) override;
    int32_t SetCBConfigResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetCBConfigResponse(
        const IHRilRadioResponseInfo &responseInfo, const ICBConfigInfo &cellBroadcastInfo) override;
    int32_t SetCdmaCBConfigResponse(const IHRilRadioResponseInfo &responseInfo) override;
    int32_t GetCdmaCBConfigResponse(
        const IHRilRadioResponseInfo &responseInfo, const ICdmaCBConfigInfo &cdmaCBConfigInfo) override;
    int32_t SendSmsMoreModeResponse(
        const IHRilRadioResponseInfo &responseInfo, const ISendSmsResultInfo &sendSmsResultInfo) override;
    int32_t SendSmsAckResponse(const IHRilRadioResponseInfo &responseInfo) override;

private:
    std::mutex callbackMutex_;
    std::condition_variable cv_;
    HdiId hdiId_;
    IHRilRadioResponseInfo resultInfo_;
};

class HdfRilHdiTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

namespace {
sptr<IRilInterface> g_rilInterface = nullptr;
RilCallback callback_;
constexpr static int32_t SLOTID_1 = 0;
constexpr static int32_t SLOTID_2 = 1;
constexpr static int32_t SUCCESS = 0;
constexpr static int32_t WAIT_TIME_SECOND = 5;
constexpr static int32_t WAIT_TIME_SECOND_LONG = 10;
constexpr static int32_t WAIT_TIME_SECOND_BEGIN = 20;
constexpr static int32_t WAIT_TIME_SECOND_30 = 30;
constexpr static int32_t WAIT_TIME_SECOND_60 = 60;
constexpr static int32_t WAIT_TIME_SECOND_180 = 180;
constexpr static int32_t CID = 1;
constexpr static int32_t REASON = 2;
std::map<int32_t, int32_t> simState_;
int32_t currentChannelId_ = 1;
constexpr int32_t TEST_TOSCA = 0;
const std::string TEST_STORAGE_PDU = "1234";
const std::string TEST_SEND_PDU = "A10305810180F6000004F4F29C0E";
const std::string TEST_SMSC_PDU = "00";
const std::string TEST_CDMA_PDU = "pdu";
const std::string TEST_SMSC_ADDR = "00";
const std::string TEST_ID_LIST = "0,1,5,320-478,922";
const std::string TEST_DCS_LIST = "0-3,5";
const int32_t TEST_MODE = 0;
const bool TEST_RESULT = true;
} // namespace

/**
** common fun
**/
void WaitFor(int32_t timeoutSecond)
{
    callback_.WaitFor(timeoutSecond);
}

bool GetBoolResult(HdiId hdiId_)
{
    return callback_.GetBoolResult(hdiId_);
}

int32_t GetSerialId()
{
    return rand() % 10000000000;
}

bool IsReady(int32_t slotId)
{
    if (g_rilInterface == nullptr) {
	HDF_LOGE("IsReady enter 1104,simState_[%{public}d]:%{public}d",slotId, simState_[slotId]);
        return false;
    }
    return simState_[slotId] != 0;
}

void BeginModule(std::string module)
{
    HDF_LOGI("BeginModule %{public}s wait", module.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(20));
    HDF_LOGI("BeginModule %{public}s start", module.c_str());
}

/**
** RilCallback implement
**/

void RilCallback::NotifyAll()
{
    std::unique_lock<std::mutex> callbackLock(callbackMutex_);
    cv_.notify_all();
}

void RilCallback::WaitFor(int32_t timeoutSecond)
{
    Clean();
	HDF_LOGI("WaitFor timeoutSecod:%{public}d 09071637", timeoutSecond);
    std::unique_lock<std::mutex> callbackLock(callbackMutex_);
    cv_.wait_for(callbackLock, std::chrono::seconds(timeoutSecond));
}

void RilCallback::Clean()
{
    hdiId_ = HdiId::HREQ_NONE;
	HDF_LOGI("hdiId_ clean 0");
}

bool RilCallback::GetBoolResult(HdiId hdiId)
{
    HDF_LOGI("GetBoolResult hdiId: %{public}d, error: %{public}d", hdiId, (int32_t)resultInfo_.error);
    bool ret = false;
    if (hdiId_ == HdiId::HREQ_NONE) {
        HDF_LOGE("response timeout, not implemented."
                 "hdiId: %d, current hdiId_: %{public}d",
            (int32_t)hdiId, hdiId_);
        ret = true;
        Clean();
        return ret;
    }
    if (hdiId_ != hdiId) {
        ret = false;
        HDF_LOGE(
            "GetBoolResult hdiId does not match. hdiId: %{public}d, current hdiId: %{public}d", (int32_t)hdiId, hdiId_);
        Clean();
        return ret;
    }
    ret = true;
    Clean();
    return ret;
}

// SIM
int32_t RilCallback::SimStateUpdated(int32_t slotId)
{
    HDF_LOGI("SimStateUpdated notice : slotId = %{public}d", slotId);
    g_rilInterface->GetSimStatus(GetSerialId(), slotId);
    return 0;
}

int32_t RilCallback::SimStkSessionEndNotify(int32_t slotId)
{
    HDF_LOGI("SimStkSessionEndNotify notice : slotId = %{public}d", slotId);
    return 0;
}

int32_t RilCallback::SimStkProactiveNotify(int32_t slotId, const std::string &response)
{
    HDF_LOGI("SimStkProactiveNotify notice : slotId = %{public}d, response = %{public}s", slotId, response.c_str());
    return 0;
}

int32_t RilCallback::SimStkAlphaNotify(int32_t slotId, const std::string &response)
{
    HDF_LOGI("SimStkAlphaNotify notice : slotId = %{public}d, response = %{public}s", slotId, response.c_str());
    return 0;
}

int32_t RilCallback::SimStkEventNotify(int32_t slotId, const std::string &response)
{
    HDF_LOGI("SimStkEventNotify notice : slotId = %{public}d, response = %{public}s", slotId, response.c_str());
    return 0;
}

int32_t RilCallback::SimStkCallSetupNotify(int32_t slotId)
{
    HDF_LOGI("SimStkCallSetupNotify notice : slotId = %{public}d", slotId);
    return 0;
}

int32_t RilCallback::SimRefreshNotify(int32_t slotId)
{
    HDF_LOGI("SimRefreshNotify notice : slotId = %{public}d", slotId);
    return 0;
}

int32_t RilCallback::GetSimStatusResponse(
    const HDI::Ril::V1_0::IHRilRadioResponseInfo &responseInfo, const HDI::Ril::V1_0::ICardStatusInfo &result)
{
    HDF_LOGI("GetBoolResult GetSimStatus result : slotId = %{public}d, simType = %{public}d, simState = %{public}d",
        responseInfo.slotId, result.simType, result.simState);
    simState_[responseInfo.slotId] = result.simState;
    HDF_LOGI("IsReady %{public}d %{public}d", responseInfo.slotId, simState_[responseInfo.slotId]);
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetSimIOResponse(const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result)
{
    HDF_LOGI("GetBoolResult GetSimIO result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s", result.sw1,
        result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_IO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetImsiResponse(const IHRilRadioResponseInfo &responseInfo, const std::string &response)
{
    HDF_LOGI("GetBoolResult GetImsi result : response = %{public}s", response.c_str());
    hdiId_ = HdiId::HREQ_SIM_GET_IMSI;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::GetSimLockStatusResponse(const IHRilRadioResponseInfo &responseInfo, int32_t simLockStatus)
{
    HDF_LOGI("GetBoolResult GetSimLockStatus result : simLockStatus = %{public}d", simLockStatus);
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::SetSimLockResponse(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult SetSimLock result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_SET_SIM_LOCK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::ChangeSimPasswordResponse(
    const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult ChangeSimPassword result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPinResponse(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult UnlockPin result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PIN;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPukResponse(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult UnlockPuk result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PUK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPin2Response(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult UnlockPin2 result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PIN2;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPuk2Response(const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult UnlockPuk2 result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PUK2;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetActiveSimResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetActiveSim result");
    hdiId_ = HdiId::HREQ_SIM_SET_ACTIVE_SIM;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkSendTerminalResponseResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SimStkSendTerminalResponse result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkSendEnvelopeResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SimStkSendEnvelope result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_ENVELOPE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkSendCallSetupRequestResultResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SimStkSendCallSetupRequestResult result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkIsReadyResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SimStkIsReady result");
    hdiId_ = HdiId::HREQ_SIM_STK_IS_READY;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetRadioProtocolResponse(
    const IHRilRadioResponseInfo &responseInfo, const ISimProtocolResponse &pSimProtocol)
{
    HDF_LOGI("GetBoolResult SetRadioProtocol result : phase = %{public}d, result = %{public}d, slotId = %{public}d",
        pSimProtocol.phase, pSimProtocol.result, pSimProtocol.slotId);
    hdiId_ = HdiId::HREQ_SIM_RADIO_PROTOCOL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimOpenLogicalChannelResponse(
    const IHRilRadioResponseInfo &responseInfo, const IOpenLogicalChannelResponse &pOpenLogicalChannelResponse)
{
    HDF_LOGI("GetBoolResult SimOpenLogicalChannel result : sw1 = %{public}d, sw2 = %{public}d, channelId = %{public}d, "
             "response = %{public}s",
        pOpenLogicalChannelResponse.sw1, pOpenLogicalChannelResponse.sw2, pOpenLogicalChannelResponse.channelId,
        pOpenLogicalChannelResponse.response.c_str());
    currentChannelId_ = pOpenLogicalChannelResponse.channelId;
    hdiId_ = HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimCloseLogicalChannelResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SimCloseLogicalChannel result");
    hdiId_ = HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimTransmitApduLogicalChannelResponse(
    const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result)
{
    HDF_LOGI("GetBoolResult SimTransmitApduLogicalChannel result : sw1 = %{public}d, sw2 = %{public}d, response = "
             "%{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimTransmitApduBasicChannelResponse(
    const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result)
{
    HDF_LOGI(
        "GetBoolResult SimTransmitApduBasicChannel result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::SimAuthenticationResponse(
    const IHRilRadioResponseInfo &responseInfo, const IIccIoResultInfo &result)
{
    HDF_LOGI("GetBoolResult SimAuthentication result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_AUTHENTICATION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::UnlockSimLockResponse(
    const IHRilRadioResponseInfo &responseInfo, const ILockStatusResp &lockStatus)
{
    HDF_LOGI("GetBoolResult UnlockSimLock result : result = %{public}d, remain = %{public}d", lockStatus.result,
        lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_SIM_LOCK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Network
int32_t RilCallback::NetworkCsRegStatusUpdated(int32_t slotId, const ICsRegStatusInfo &csRegStatusInfo)
{
    HDF_LOGI("RilCallback::NetworkCsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, "
             "lacCode:%{public}d, cellId:%{public}d, radioTechnology:%{public}d",
        csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
        csRegStatusInfo.radioTechnology);
    return 0;
}

int32_t RilCallback::NetworkPsRegStatusUpdated(int32_t slotId, const IPsRegStatusInfo &psRegStatusInfo)
{
    HDF_LOGI(
        "RilCallback::NetworkPsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, lacCode:%{public}d, "
        "cellId:%{public}d, tech:%{public}d, nrRestricted:%{public}d, nrAvailable:%{public}d, enDcAvailable:%{public}d",
        psRegStatusInfo.notifyType, psRegStatusInfo.regStatus, psRegStatusInfo.lacCode, psRegStatusInfo.cellId,
        psRegStatusInfo.radioTechnology, psRegStatusInfo.isDcNrRestricted, psRegStatusInfo.isNrAvailable,
        psRegStatusInfo.isEnDcAvailable);
    return 0;
}

int32_t RilCallback::SignalStrengthUpdated(int32_t slotId, const IRssi &rssi)
{
    HDF_LOGI("RilCallback::SignalStrengthUpdated rxlev:%{public}d rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
    return 0;
}

int32_t RilCallback::NetworkTimeZoneUpdated(int32_t slotId, const std::string &timeZoneStr)
{
    HDF_LOGI("RilCallback::NetworkTimeZoneUpdated timeZone:%{public}s", timeZoneStr.c_str());
    return 0;
}

int32_t RilCallback::NetworkTimeUpdated(int32_t slotId, const std::string &timeStr)
{
    HDF_LOGI("RilCallback::NetworkTimeZoneUpdated time:%{public}s", timeStr.c_str());
    return 0;
}

int32_t RilCallback::NetworkPhyChnlCfgUpdated(int32_t slotId, const IChannelConfigInfoList &channelConfigInfoList)
{
    HDF_LOGI("RilCallback::NetworkPhyChnlCfgUpdated itemNum:%{public}d", channelConfigInfoList.itemNum);
    for (IPhysicalChannelConfig phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
        HDF_LOGI("RilCallback::NetworkPhyChnlCfgUpdated cellConnStatus:%{public}d, "
                 "cellBandwidthDownlinkKhz:%{public}d, cellBandwidthUplinkKhz:%{public}d, physicalCellId:%{public}d, "
                 "ratType:%{public}d, freqRange:%{public}d, downlinkChannelNum:%{public}d, "
                 "uplinkChannelNum:%{public}d, contextIdNum:%{public}d",
            phyChnlCfg.cellConnStatus, phyChnlCfg.cellBandwidthDownlinkKhz, phyChnlCfg.cellBandwidthUplinkKhz,
            phyChnlCfg.ratType, phyChnlCfg.freqRange, phyChnlCfg.downlinkChannelNum, phyChnlCfg.uplinkChannelNum,
            phyChnlCfg.physicalCellId, phyChnlCfg.contextIdNum);
        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            HDF_LOGI("contextIds[%{public}d]:%{public}d", j, phyChnlCfg.contextIds[j]);
        }
    }
    return 0;
}

int32_t RilCallback::NetworkCurrentCellUpdated(int32_t slotId, const ICellListCurrentInfo &cellListCurrentInfo)
{
    HDF_LOGI("RilCallback::NetworkCurrentCellUpdated itemNum:%{public}d", cellListCurrentInfo.itemNum);
    for (auto info : cellListCurrentInfo.cellCurrentInfo) {
        HDF_LOGI("RilCallback::NetworkCurrentCellUpdated ratType:%{public}d, mcc:%{public}d, mnc:%{public}d",
            info.ratType, info.mcc, info.mnc);
        switch (static_cast<RatType>(info.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                HDF_LOGI("cellId:%{public}d", info.serviceCells.lte.cellId);
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.lte.arfcn);
                HDF_LOGI("pci:%{public}d", info.serviceCells.lte.pci);
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.lte.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.lte.rsrq);
                HDF_LOGI("rxlev:%{public}d", info.serviceCells.lte.rssi);
                break;
            case RatType::NETWORK_TYPE_GSM:
                HDF_LOGI("band:%{public}d", info.serviceCells.gsm.band);
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.gsm.arfcn);
                HDF_LOGI("bsic:%{public}d", info.serviceCells.gsm.bsic);
                HDF_LOGI("cellId:%{public}d", info.serviceCells.gsm.cellId);
                HDF_LOGI("rxlev:%{public}d", info.serviceCells.gsm.rxlev);
                HDF_LOGI("lac:%{public}d", info.serviceCells.gsm.lac);
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.wcdma.arfcn);
                HDF_LOGI("psc:%{public}d", info.serviceCells.wcdma.psc);
                HDF_LOGI("rscp:%{public}d", info.serviceCells.wcdma.rscp);
                HDF_LOGI("ecno:%{public}d", info.serviceCells.wcdma.ecno);
                break;
            case RatType::NETWORK_TYPE_NR:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.nr.nrArfcn);
                HDF_LOGI("psc:%{public}d", info.serviceCells.nr.pci);
                HDF_LOGI("rscp:%{public}d", info.serviceCells.nr.tac);
                HDF_LOGI("ecno:%{public}d", info.serviceCells.nr.nci);
                break;
            default:
                HDF_LOGE("RilCallback::NetworkCurrentCellUpdated invalid ratType");
        }
    }
    return 0;
}

int32_t RilCallback::GetSignalStrengthResponse(
    const IHRilRadioResponseInfo &responseInfo, const IRssi &rssi)
{
    HDF_LOGI("RilCallback::GetSignalStrengthResponse rxlev:%{public}d rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
    hdiId_ = HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCsRegStatusResponse(
    const IHRilRadioResponseInfo &responseInfo, const ICsRegStatusInfo &csRegStatusInfo)
{
    HDF_LOGI("RilCallback::GetCsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, "
             "lacCode:%{public}d, cellId:%{public}d, radioTechnology:%{public}d",
        csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
        csRegStatusInfo.radioTechnology);
    hdiId_ = HdiId::HREQ_NETWORK_GET_CS_REG_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPsRegStatusResponse(
    const IHRilRadioResponseInfo &responseInfo, const IPsRegStatusInfo &psRegStatusInfo)
{
    HDF_LOGI(
        "RilCallback::GetPsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, lacCode:%{public}d, "
        "cellId:%{public}d, tech:%{public}d, nrRestricted:%{public}d, nrAvailable:%{public}d, enDcAvailable:%{public}d",
        psRegStatusInfo.notifyType, psRegStatusInfo.regStatus, psRegStatusInfo.lacCode, psRegStatusInfo.cellId,
        psRegStatusInfo.radioTechnology, psRegStatusInfo.isDcNrRestricted, psRegStatusInfo.isNrAvailable,
        psRegStatusInfo.isEnDcAvailable);
    hdiId_ = HdiId::HREQ_NETWORK_GET_PS_REG_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetOperatorInfoResponse(
    const IHRilRadioResponseInfo &responseInfo, const IOperatorInfo &operatorInfo)
{
    HDF_LOGI("RilCallback::GetOperatorInfoResponse longName:%{public}s, shortName:%{public}s, numeric:%{public}s",
        operatorInfo.longName.c_str(), operatorInfo.shortName.c_str(), operatorInfo.numeric.c_str());
    hdiId_ = HdiId::HREQ_NETWORK_GET_OPERATOR_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetNetworkSearchInformationResponse(const IHRilRadioResponseInfo &responseInfo,
    const IAvailableNetworkList &availableNetworkList)
{
    HDF_LOGI("RilCallback::GetNetworkSearchInformationResponse itemNum:%{public}d", availableNetworkList.itemNum);
    for (auto availableInfo : availableNetworkList.availableNetworkInfo) {
        HDF_LOGI("status:%{public}d", availableInfo.status);
        HDF_LOGI("numeric:%{public}s", availableInfo.numeric.c_str());
        HDF_LOGI("shortName:%{public}s", availableInfo.shortName.c_str());
        HDF_LOGI("longName:%{public}s", availableInfo.longName.c_str());
        HDF_LOGI("rat:%{public}d", availableInfo.rat);
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetNetworkSelectionModeResponse(const IHRilRadioResponseInfo &responseInfo,
    const ISetNetworkModeInfo &setNetworkModeInfo)
{
    HDF_LOGI("RilCallback::GetNetworkSelectionModeResponse selectMode:%{public}d", setNetworkModeInfo.selectMode);
    hdiId_ = HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetNetworkSelectionModeResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetNetworkSelectionModeResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetNeighboringCellInfoListResponse(
    const IHRilRadioResponseInfo &responseInfo, const ICellListNearbyInfo &cellInfoList)
{
    HDF_LOGI("RilCallback::GetNeighboringCellInfoListResponse itemNum:%{public}d", cellInfoList.itemNum);
    for (auto info : cellInfoList.cellNearbyInfo) {
        HDF_LOGI("RilCallback::GetNeighboringCellInfoListResponse ratType:%{public}d", info.ratType);
        switch (static_cast<RatType>(info.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.lte.arfcn);
                HDF_LOGI("pci:%{public}d", info.serviceCells.lte.pci);
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.lte.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.lte.rsrq);
                HDF_LOGI("rxlev:%{public}d", info.serviceCells.lte.rxlev);
                break;
            case RatType::NETWORK_TYPE_GSM:
                HDF_LOGI("band:%{public}d", info.serviceCells.gsm.band);
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.gsm.arfcn);
                HDF_LOGI("bsic:%{public}d", info.serviceCells.gsm.bsic);
                HDF_LOGI("cellId:%{public}d", info.serviceCells.gsm.cellId);
                HDF_LOGI("rxlev:%{public}d", info.serviceCells.gsm.rxlev);
                HDF_LOGI("lac:%{public}d", info.serviceCells.gsm.lac);
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.wcdma.arfcn);
                HDF_LOGI("psc:%{public}d", info.serviceCells.wcdma.psc);
                HDF_LOGI("rscp:%{public}d", info.serviceCells.wcdma.rscp);
                HDF_LOGI("ecno:%{public}d", info.serviceCells.wcdma.ecno);
                break;
            case RatType::NETWORK_TYPE_NR:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.nr.nrArfcn);
                HDF_LOGI("psc:%{public}d", info.serviceCells.nr.pci);
                HDF_LOGI("rscp:%{public}d", info.serviceCells.nr.tac);
                HDF_LOGI("ecno:%{public}d", info.serviceCells.nr.nci);
                break;
            default:
                HDF_LOGE("RilCallback::GetNeighboringCellInfoListResponse invalid ratType");
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCurrentCellInfoResponse(const IHRilRadioResponseInfo &responseInfo,
    const ICellListCurrentInfo &cellInfoList)
{
    HDF_LOGI("RilCallback::GetCurrentCellInfoResponse itemNum:%{public}d", cellInfoList.itemNum);
    for (auto info : cellInfoList.cellCurrentInfo) {
        HDF_LOGI("RilCallback::GetCurrentCellInfoResponse ratType:%{public}d, mcc:%{public}d, mnc:%{public}d",
            info.ratType, info.mcc, info.mnc);
        switch (static_cast<RatType>(info.ratType)) {
            case RatType::NETWORK_TYPE_LTE:
                HDF_LOGI("cellId:%{public}d", info.serviceCells.lte.cellId);
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.lte.arfcn);
                HDF_LOGI("pci:%{public}d", info.serviceCells.lte.pci);
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.lte.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.lte.rsrq);
                HDF_LOGI("rxlev:%{public}d", info.serviceCells.lte.rssi);
                break;
            case RatType::NETWORK_TYPE_GSM:
                HDF_LOGI("band:%{public}d", info.serviceCells.gsm.band);
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.gsm.arfcn);
                HDF_LOGI("bsic:%{public}d", info.serviceCells.gsm.bsic);
                HDF_LOGI("cellId:%{public}d", info.serviceCells.gsm.cellId);
                HDF_LOGI("rxlev:%{public}d", info.serviceCells.gsm.rxlev);
                HDF_LOGI("lac:%{public}d", info.serviceCells.gsm.lac);
                break;
            case RatType::NETWORK_TYPE_WCDMA:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.wcdma.arfcn);
                HDF_LOGI("psc:%{public}d", info.serviceCells.wcdma.psc);
                HDF_LOGI("rscp:%{public}d", info.serviceCells.wcdma.rscp);
                HDF_LOGI("ecno:%{public}d", info.serviceCells.wcdma.ecno);
                break;
            case RatType::NETWORK_TYPE_NR:
                HDF_LOGI("arfcn:%{public}d", info.serviceCells.nr.nrArfcn);
                HDF_LOGI("psc:%{public}d", info.serviceCells.nr.pci);
                HDF_LOGI("rscp:%{public}d", info.serviceCells.nr.tac);
                HDF_LOGI("ecno:%{public}d", info.serviceCells.nr.nci);
                break;
            default:
                HDF_LOGE("RilCallback::GetCurrentCellInfoResponse invalid ratType");
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetPreferredNetworkResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetPreferredNetworkResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPreferredNetworkResponse(const IHRilRadioResponseInfo &responseInfo,
    const IPreferredNetworkTypeInfo &preferredNetworkTypeInfo)
{
    HDF_LOGI("RilCallback::GetPreferredNetworkResponse type:%{public}d", preferredNetworkTypeInfo.preferredNetworkType);
    hdiId_ = HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetRadioCapabilityResponse(const IHRilRadioResponseInfo &responseInfo,
    const IRadioCapabilityInfo &radioCapabilityInfo)
{
    HDF_LOGI("RilCallback::GetRadioCapabilityResponse ratFamily:%{public}d", radioCapabilityInfo.ratFamily);
    hdiId_ = HdiId::HREQ_NETWORK_GET_RADIO_CAPABILITY;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPhysicalChannelConfigResponse(const IHRilRadioResponseInfo &responseInfo,
    const IChannelConfigInfoList &channelConfigInfoList)
{
    HDF_LOGI("RilCallback::GetPhysicalChannelConfigResponse itemNum:%{public}d", channelConfigInfoList.itemNum);
    for (IPhysicalChannelConfig phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
        HDF_LOGI("RilCallback::GetPhysicalChannelConfigResponse cellConnStatus:%{public}d, "
                 "cellBandwidthDownlinkKhz:%{public}d, cellBandwidthUplinkKhz:%{public}d, physicalCellId:%{public}d, "
                 "ratType:%{public}d, freqRange:%{public}d, downlinkChannelNum:%{public}d, "
                 "uplinkChannelNum:%{public}d, contextIdNum:%{public}d",
            phyChnlCfg.cellConnStatus, phyChnlCfg.cellBandwidthDownlinkKhz, phyChnlCfg.cellBandwidthUplinkKhz,
            phyChnlCfg.ratType, phyChnlCfg.freqRange, phyChnlCfg.downlinkChannelNum, phyChnlCfg.uplinkChannelNum,
            phyChnlCfg.physicalCellId, phyChnlCfg.contextIdNum);
        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            HDF_LOGI("contextIds[%{public}d]:%{public}d", j, phyChnlCfg.contextIds[j]);
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetLocateUpdatesResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetLocateUpdatesResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetNotificationFilterResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetNotificationFilterResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetDeviceStateResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetDeviceStateResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_DEVICE_STATE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Call
int32_t RilCallback::CallEmergencyNotice(int32_t slotId, const IEmergencyInfoList &emergencyInfoList)
{
    HDF_LOGI("GetBoolResult CallEmergencyNotice callSize : %{public}d", emergencyInfoList.callSize);
    return 0;
}

int32_t RilCallback::CallStateUpdated(int32_t slotId, int32_t type)
{
    HDF_LOGI("GetBoolResult CallStateUpdated slotId : %{public}d, type: %{public}d", slotId, type);
    return 0;
}

int32_t RilCallback::CallRingbackVoiceNotice(int32_t slotId, const IRingbackVoice &ringbackVoice)
{
    HDF_LOGI("GetBoolResult CallRingbackVoiceNotice slotId : %{public}d, ringbackVoice: %{public}d",
        slotId, ringbackVoice.status);
    return 0;
}

int32_t RilCallback::CallSrvccStatusNotice(int32_t slotId, const ISrvccStatus &srvccStatus)
{
    HDF_LOGI("GetBoolResult CallSrvccStatusNotice slotId : %{public}d, srvccStatus: %{public}d",
        slotId, srvccStatus.status);
    return 0;
}

int32_t RilCallback::CallUssdNotice(int32_t slotId, const IUssdNoticeInfo &ussdNoticeInfo)
{
    HDF_LOGI("GetBoolResult CallUssdNotice slotId : %{public}d, ussdNoticeInfo m: %{public}d, str: %{public}s",
        slotId, ussdNoticeInfo.m, ussdNoticeInfo.str.c_str());
    return 0;
}

int32_t RilCallback::CallSsNotice(int32_t slotId, const ISsNoticeInfo &ssNoticeInfo)
{
    HDF_LOGI("GetBoolResult CallSsNotice slotId : %{public}d, result: %{public}d, serviceType: %{public}d,"
        "requestType: %{public}d, serviceClass: %{public}d", slotId, ssNoticeInfo.result, ssNoticeInfo.serviceType,
        ssNoticeInfo.requestType, ssNoticeInfo.serviceClass);
    return 0;
}

int32_t RilCallback::GetEmergencyCallListResponse(
    const IHRilRadioResponseInfo &responseInfo, const IEmergencyInfoList &emergencyInfoList)
{
    HDF_LOGI("GetBoolResult emergencyInfoList callSize : %{public}d", emergencyInfoList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_EMERGENCY_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetEmergencyCallListResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetEmergencyCallListResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_EMERGENCY_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallListResponse(
    const IHRilRadioResponseInfo &responseInfo, const ICallInfoList &callList)
{
    HDF_LOGI("GetBoolResult CallInfoList callSize : %{public}d", callList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DialResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult DialResponse");
    hdiId_ = HdiId::HREQ_CALL_DIAL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::HangupResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult HangupResponse");
    hdiId_ = HdiId::HREQ_CALL_HANGUP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::RejectResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult RejectResponse");
    hdiId_ = HdiId::HREQ_CALL_REJECT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::AnswerResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult AnswerResponse");
    hdiId_ = HdiId::HREQ_CALL_ANSWER;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::HoldCallResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult HoldCallResponse");
    hdiId_ = HdiId::HREQ_CALL_HOLD_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::UnHoldCallResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult UnHoldCallResponse");
    hdiId_ = HdiId::HREQ_CALL_UNHOLD_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SwitchCallResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SwitchCallResponse");
    hdiId_ = HdiId::HREQ_CALL_SWITCH_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetClipResponse(const IHRilRadioResponseInfo &responseInfo,
    const IGetClipResult &getClipResult)
{
    HDF_LOGI("GetBoolResult result: %{public}d, action: %{public}d, clipStat: %{public}d",
        getClipResult.result, getClipResult.action, getClipResult.clipStat);
    hdiId_ = HdiId::HREQ_CALL_GET_CLIP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetClipResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetClipResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CLIP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CombineConferenceResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult CombineConferenceResponse");
    hdiId_ = HdiId::HREQ_CALL_COMBINE_CONFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SeparateConferenceResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SeparateConferenceResponse");
    hdiId_ = HdiId::HREQ_CALL_SEPARATE_CONFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CallSupplementResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult CallSupplementResponse");
    hdiId_ = HdiId::HREQ_CALL_CALL_SUPPLEMENT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallWaitingResponse(const IHRilRadioResponseInfo &responseInfo,
    const ICallWaitResult &callWaitResult)
{
    HDF_LOGI("GetBoolResult GetCallWaitingResponse result: %{public}d, status: %{public}d, classCw: %{public}d",
        callWaitResult.result, callWaitResult.status , callWaitResult.classCw);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_WAITING;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCallWaitingResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetCallWaitingResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_WAITING;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallTransferInfoResponse(const IHRilRadioResponseInfo &responseInfo,
    const ICallForwardQueryInfoList &cFQueryList)
{
    HDF_LOGI("GetBoolResult GetCallTransferInfoResponse cFQueryList: %{public}d", cFQueryList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCallTransferInfoResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetCallTransferInfoResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallRestrictionResponse(const IHRilRadioResponseInfo &responseInfo,
    const ICallRestrictionResult &result)
{
    HDF_LOGI("GetBoolResult result: %{public}d, status: %{public}d, classCw: %{public}d",
        result.result, result.status , result.classCw);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_RESTRICTION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::SetCallRestrictionResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetCallRestrictionResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_RESTRICTION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::GetClirResponse(const IHRilRadioResponseInfo &responseInfo, const IGetClirResult &getClirResult)
{
    HDF_LOGI("GetBoolResult result: %{public}d, action: %{public}d, clirStat: %{public}d",
        getClirResult.result, getClirResult.action, getClirResult.clirStat);
    hdiId_ = HdiId::HREQ_CALL_GET_CLIR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetClirResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetClirResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CLIR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::StartDtmfResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult StartDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_START_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendDtmfResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SendDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_SEND_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::StopDtmfResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult StopDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_STOP_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallPreferenceModeResponse(const IHRilRadioResponseInfo &responseInfo, int32_t mode)
{
    HDF_LOGI("GetBoolResult GetCallPreferenceModeResponse mode: %{public}d", mode);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_PREFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCallPreferenceModeResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetCallPreferenceModeResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_PREFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetUssdResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetUssdResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_USSD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetUssdResponse(const IHRilRadioResponseInfo &responseInfo, int32_t cusd)
{
    HDF_LOGI("GetBoolResult GetUssdResponse cusd: %{public}d", cusd);
    hdiId_ = HdiId::HREQ_CALL_GET_USSD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetMuteResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetMuteResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_MUTE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetMuteResponse(const IHRilRadioResponseInfo &responseInfo, int32_t mute)
{
    HDF_LOGI("GetBoolResult GetMuteResponse mute: %{public}d", mute);
    hdiId_ = HdiId::HREQ_CALL_GET_MUTE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallFailReasonResponse(const IHRilRadioResponseInfo &responseInfo, int32_t callFail)
{
    HDF_LOGI("GetBoolResult GetCallFailReasonResponse callFail: %{public}d", callFail);
    hdiId_ = HdiId::HREQ_CALL_GET_FAIL_REASON;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetBarringPasswordResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetBarringPasswordResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_BARRING_PASSWORD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

/**
 * modem
 **/
int32_t RilCallback::RadioStateUpdated(int32_t slotId, int32_t state)
{
    HDF_LOGI("RadioStateUpdated state : %{public}d", state);
    return 0;
}

int32_t RilCallback::VoiceRadioTechUpdated(int32_t slotId, const IVoiceRadioTechnology &voiceRadioTechnology)
{
    HDF_LOGI("GetVoiceRadioTechnologyResponse srvStatus : %{public}d srvDomain : %{public}d roamStatus: %{public}d "
             "simStatus : %{public}d lockStatus : %{public}d sysMode : %{public}d actType : %{public}d sysModeName : "
             "%{public}s actName : %{public}s",
        voiceRadioTechnology.srvStatus, voiceRadioTechnology.srvDomain, voiceRadioTechnology.roamStatus,
        voiceRadioTechnology.simStatus, voiceRadioTechnology.lockStatus, voiceRadioTechnology.sysMode,
        voiceRadioTechnology.actType, voiceRadioTechnology.sysModeName.c_str(), voiceRadioTechnology.actName.c_str());
    return 0;
}

int32_t RilCallback::ShutDownResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("ShutDownResponse");
    hdiId_ = HdiId::HREQ_MODEM_SHUT_DOWN;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetRadioStateResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("SetRadioStateResponse");
    hdiId_ = HdiId::HREQ_MODEM_SET_RADIO_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetRadioStateResponse(const IHRilRadioResponseInfo &responseInfo, int32_t state)
{
    HDF_LOGI("GetRadioStateResponse state : %{public}d", state);
    hdiId_ = HdiId::HREQ_MODEM_GET_RADIO_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetImeiResponse(const IHRilRadioResponseInfo &responseInfo, const std::string &imei)
{
    HDF_LOGI("GetImeiResponse imei : %{public}s", imei.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_IMEI;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetMeidResponse(const IHRilRadioResponseInfo &responseInfo, const std::string &meid)
{
    HDF_LOGI("GetMeidResponse meid : %{public}s", meid.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_MEID;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetVoiceRadioTechnologyResponse(
    const IHRilRadioResponseInfo &responseInfo, const IVoiceRadioTechnology &voiceRadioTechnology)
{
    HDF_LOGI("GetVoiceRadioTechnologyResponse srvStatus : %{public}d srvDomain : %{public}d roamStatus: %{public}d "
             "simStatus : %{public}d lockStatus : %{public}d sysMode : %{public}d actType : %{public}d sysModeName : "
             "%{public}s actName : %{public}s",
        voiceRadioTechnology.srvStatus, voiceRadioTechnology.srvDomain, voiceRadioTechnology.roamStatus,
        voiceRadioTechnology.simStatus, voiceRadioTechnology.lockStatus, voiceRadioTechnology.sysMode,
        voiceRadioTechnology.actType, voiceRadioTechnology.sysModeName.c_str(), voiceRadioTechnology.actName.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_VOICE_RADIO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetBasebandVersionResponse(
    const IHRilRadioResponseInfo &responseInfo, const std::string &basebandVersion)
{
    HDF_LOGI("GetBasebandVersionResponse basebandVersion : %{public}s", basebandVersion.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_BASEBAND_VERSION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Data
int32_t RilCallback::ActivatePdpContextResponse(const IHRilRadioResponseInfo &responseInfo,
    const ISetupDataCallResultInfo &setupDataCallResultInfo)
{
    HDF_LOGI("RilCallback::ActivatePdpContextResponse flag:%{public}d reason:%{public}d retryTime:%{public}d "
             "cid:%{public}d active:%{public}d type:%{public}s netPortName:%{public}s address:%{public}s "
             "dns:%{public}s dnsSec:%{public}s gateway:%{public}s maxTransferUnit:%{public}d pCscfPrimAddr:%{public}s "
             "pCscfSecAddr:%{public}s pduSessionId:%{public}d",
        setupDataCallResultInfo.flag, setupDataCallResultInfo.reason, setupDataCallResultInfo.retryTime,
        setupDataCallResultInfo.cid, setupDataCallResultInfo.active, setupDataCallResultInfo.type.c_str(),
        setupDataCallResultInfo.netPortName.c_str(), setupDataCallResultInfo.address.c_str(),
        setupDataCallResultInfo.dns.c_str(), setupDataCallResultInfo.dnsSec.c_str(),
        setupDataCallResultInfo.gateway.c_str(), setupDataCallResultInfo.maxTransferUnit,
        setupDataCallResultInfo.pCscfPrimAddr.c_str(), setupDataCallResultInfo.pCscfSecAddr.c_str(),
        setupDataCallResultInfo.pduSessionId);
    hdiId_ = HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DeactivatePdpContextResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::DeactivatePdpContextResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPdpContextListResponse(
    const IHRilRadioResponseInfo &responseInfo, const IDataCallResultList &dataCallResultList)
{
    HDF_LOGI("RilCallback::GetPdpContextListResponse size:%{public}d", dataCallResultList.size);
    for (auto &setupDataCallResultInfo : dataCallResultList.dcList) {
        HDF_LOGI(
            "RilCallback::GetPdpContextListResponse flag:%{public}d reason:%{public}d retryTime:%{public}d "
            "cid:%{public}d active:%{public}d type:%{public}s netPortName:%{public}s address:%{public}s "
            "dns:%{public}s dnsSec:%{public}s gateway:%{public}s maxTransferUnit:%{public}d pCscfPrimAddr:%{public}s "
            "pCscfSecAddr:%{public}s pduSessionId:%{public}d",
            setupDataCallResultInfo.flag, setupDataCallResultInfo.reason, setupDataCallResultInfo.retryTime,
            setupDataCallResultInfo.cid, setupDataCallResultInfo.active, setupDataCallResultInfo.type.c_str(),
            setupDataCallResultInfo.netPortName.c_str(), setupDataCallResultInfo.address.c_str(),
            setupDataCallResultInfo.dns.c_str(), setupDataCallResultInfo.dnsSec.c_str(),
            setupDataCallResultInfo.gateway.c_str(), setupDataCallResultInfo.maxTransferUnit,
            setupDataCallResultInfo.pCscfPrimAddr.c_str(), setupDataCallResultInfo.pCscfSecAddr.c_str(),
            setupDataCallResultInfo.pduSessionId);
    }
    hdiId_ = HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetInitApnInfoResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetInitApnInfoResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_INIT_APN_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetLinkBandwidthReportingRuleResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetLinkBandwidthReportingRuleResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetLinkBandwidthInfoResponse(const IHRilRadioResponseInfo &responseInfo,
    const IDataLinkBandwidthInfo &dataLinkBandwidthInfo)
{
    HDF_LOGI(
        "RilCallback::GetLinkBandwidthInfoResponse serial:%{public}d cid:%{public}d qi:%{public}d dlGfbr:%{public}d "
        "ulGfbr:%{public}d dlMfbr:%{public}d ulMfbr:%{public}d ulSambr:%{public}d dlSambr:%{public}d "
        "averagingWindow:%{public}d",
        dataLinkBandwidthInfo.serial, dataLinkBandwidthInfo.cid, dataLinkBandwidthInfo.qi, dataLinkBandwidthInfo.dlGfbr,
        dataLinkBandwidthInfo.ulGfbr, dataLinkBandwidthInfo.dlMfbr, dataLinkBandwidthInfo.ulMfbr,
        dataLinkBandwidthInfo.ulSambr, dataLinkBandwidthInfo.dlSambr, dataLinkBandwidthInfo.averagingWindow);
    hdiId_ = HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetDataPermittedResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetDataPermittedResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_DATA_PERMITTED;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetDataProfileInfoResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetDataProfileInfoResponse error:%{public}d",responseInfo.error);
    hdiId_ =HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

// Sms
int32_t RilCallback::NewSmsNotify(int32_t slotId, const ISmsMessageInfo &smsMessageInfo)
{
    HDF_LOGI("RilCallback::NewSmsNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallback::NewCdmaSmsNotify(int32_t slotId, const ISmsMessageInfo &smsMessageInfo)
{
    HDF_LOGI("RilCallback::NewCdmaSmsNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallback::SmsStatusReportNotify(int32_t slotId, const ISmsMessageInfo &smsMessageInfo)
{
    HDF_LOGI("RilCallback::SmsStatusReportNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallback::NewSmsStoredOnSimNotify(int32_t slotId, int32_t recordNumber, int32_t indicationType)
{
    HDF_LOGI("RilCallback::NewSmsStoredOnSimNotify recordNumber : %{public}d, indicationType : %{public}d",
        recordNumber, indicationType);
    return 0;
}

int32_t RilCallback::CBConfigNotify(int32_t slotId, const ICBConfigReportInfo &cellBroadConfigReportInfo)
{
    HDF_LOGI("RilCallback::CBConfigNotify cellBroadConfigReportInfo dcs : %{public}s",
        cellBroadConfigReportInfo.dcs.c_str());
    return 0;
}

int32_t RilCallback::SendGsmSmsResponse(
    const IHRilRadioResponseInfo &responseInfo, const ISendSmsResultInfo &sendSmsResultInfo)
{
    HDF_LOGI("RilCallback::SendGsmSmsResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
        sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_GSM_SMS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendCdmaSmsResponse(
    const IHRilRadioResponseInfo &responseInfo, const ISendSmsResultInfo &sendSmsResultInfo)
{
    HDF_LOGI("RilCallback::SendCdmaSmsResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
        sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_CDMA_SMS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::AddSimMessageResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::AddSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_ADD_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DelSimMessageResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::DelSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_DEL_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UpdateSimMessageResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::UpdateSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::AddCdmaSimMessageResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::AddCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DelCdmaSimMessageResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::DelCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::UpdateCdmaSimMessageResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::UpdateCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetSmscAddrResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetSmscAddrResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_SMSC_ADDR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetSmscAddrResponse(
    const IHRilRadioResponseInfo &responseInfo, const IServiceCenterAddress &serviceCenterAddress)
{
    HDF_LOGI("RilCallback::GetSmscAddrResponse serviceCenterAddress tosca : %{public}d, address : %{public}s",
        serviceCenterAddress.tosca, serviceCenterAddress.address.c_str());
    hdiId_ = HdiId::HREQ_SMS_GET_SMSC_ADDR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCBConfigResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetCBConfigResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCBConfigResponse(
    const IHRilRadioResponseInfo &responseInfo, const ICBConfigInfo &cellBroadcastInfo)
{
    HDF_LOGI("RilCallback::GetCBConfigResponse cellBroadcastInfo mids : %{public}s, dcss: %{public}s",
        cellBroadcastInfo.mids.c_str(), cellBroadcastInfo.dcss.c_str());
    hdiId_ = HdiId::HREQ_SMS_GET_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCdmaCBConfigResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetCdmaCBConfigResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCdmaCBConfigResponse(
    const IHRilRadioResponseInfo &responseInfo, const ICdmaCBConfigInfo &cdmaCBConfigInfo)
{
    HDF_LOGI("RilCallback::GetCdmaCBConfigResponse cdmaCBConfigInfo service : %{public}d, language : %{public}d, "
             "checked: %{public}d",
        cdmaCBConfigInfo.service, cdmaCBConfigInfo.language, cdmaCBConfigInfo.checked);
    hdiId_ = HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendSmsMoreModeResponse(
    const IHRilRadioResponseInfo &responseInfo, const ISendSmsResultInfo &sendSmsResultInfo)
{
    HDF_LOGI("RilCallback::SendSmsMoreModeResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
        sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_SMS_MORE_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendSmsAckResponse(const IHRilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SendSmsAckResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SEND_SMS_ACK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

/**
** HdfRilHdiTest implement
**/
void HdfRilHdiTest::SetUpTestCase() {}

void HdfRilHdiTest::TearDownTestCase() {}

void HdfRilHdiTest::SetUp() {}

void HdfRilHdiTest::TearDown() {}

/**
**  Xts Test
**/

HWTEST_F(HdfRilHdiTest, CheckRilInstanceIsEmpty, Function | MediumTest | Level1)
{
    g_rilInterface = IRilInterface::Get();
    ASSERT_NE(nullptr, g_rilInterface);
    if (g_rilInterface != nullptr) {
        g_rilInterface->SetCallback(&callback_);
    }
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSimStatus_V1_0100, Function | MediumTest | Level3)
{
    if (g_rilInterface == nullptr) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSimStatus_V1_0200, Function | MediumTest | Level3)
{
    if (g_rilInterface == nullptr) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetSimStatus(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_STATUS));
}

// Call
HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetEmergencyCallList_V1_0100, Function | MediumTest | Level3)
{
    BeginModule("Call");
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_EMERGENCY_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetEmergencyCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetEmergencyCallList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_EMERGENCY_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetEmergencyCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IEmergencyInfoList emergencyInfoList;
    IEmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = IEccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = ISimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = IAbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_1, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_EMERGENCY_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetEmergencyCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IEmergencyInfoList emergencyInfoList;
    IEmergencyCall emergencyInfo = {};
    emergencyInfo.index = 1;
    emergencyInfo.total = 1;
    emergencyInfo.eccNum = "120";
    emergencyInfo.eccType = IEccType::TYPE_CATEGORY;
    emergencyInfo.simpresent = ISimpresentType::TYPE_HAS_CARD;
    emergencyInfo.mcc = "460";
    emergencyInfo.abnormalService = IAbnormalServiceType::TYPE_ALL;
    emergencyInfoList.calls.push_back(emergencyInfo);
    int32_t ret = g_rilInterface->SetEmergencyCallList(SLOTID_2, GetSerialId(), emergencyInfoList);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_EMERGENCY_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Dial_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_1, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_DIAL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Dial_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDialInfo dialInfo = {};
    dialInfo.address = "10086";
    dialInfo.clir = 0;
    int32_t ret = g_rilInterface->Dial(SLOTID_2, GetSerialId(), dialInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_DIAL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Reject_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_REJECT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Reject_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->Reject(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_REJECT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Hangup_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HANGUP));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Hangup_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->Hangup(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HANGUP));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Answer_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_ANSWER));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_Answer_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->Answer(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_ANSWER));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_HoldCall_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HOLD_CALL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_HoldCall_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->HoldCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_HOLD_CALL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnHoldCall_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_UNHOLD_CALL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnHoldCall_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->UnHoldCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_UNHOLD_CALL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SwitchCall_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SWITCH_CALL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SwitchCall_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SwitchCall(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SWITCH_CALL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_CombineConference_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_COMBINE_CONFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_CombineConference_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->CombineConference(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_COMBINE_CONFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SeparateConference_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEPARATE_CONFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SeparateConference_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SeparateConference(SLOTID_2, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEPARATE_CONFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallWaiting_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_WAITING));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallWaiting_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallWaiting(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_WAITING));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallWaiting_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_1, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_WAITING));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallWaiting_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetCallWaiting(SLOTID_2, GetSerialId(), 0);
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_WAITING));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallTransferInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND_30);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallTransferInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallTransferInfo(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND_30);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallTransferInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ICallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_1, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallTransferInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ICallForwardSetInfo callForwardSetInfo = {};
    callForwardSetInfo.reason = 1;
    callForwardSetInfo.mode = 1;
    callForwardSetInfo.classx = 1;
    callForwardSetInfo.number = "10086";
    int32_t ret = g_rilInterface->SetCallTransferInfo(SLOTID_2, GetSerialId(), callForwardSetInfo);
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallRestriction_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_1, GetSerialId(), fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_RESTRICTION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallRestriction_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string fac = "AO";
    int32_t ret = g_rilInterface->GetCallRestriction(SLOTID_2, GetSerialId(), fac);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_RESTRICTION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallRestriction_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ICallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_1, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_RESTRICTION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallRestriction_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ICallRestrictionInfo callRestrictionInfo = {};
    callRestrictionInfo.fac = "AO";
    callRestrictionInfo.mode = 1;
    callRestrictionInfo.password = "123456";
    int32_t ret = g_rilInterface->SetCallRestriction(SLOTID_2, GetSerialId(), callRestrictionInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_RESTRICTION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetClip_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIP));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetClip_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetClip(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIP));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetClip_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIP));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetClip_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetClip(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIP));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetClir_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetClir_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetClir(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CLIR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetClir_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND_30);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetClir_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetClir(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND_30);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CLIR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallPreferenceMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_PREFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCallPreferenceMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetCallPreferenceMode(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_CALL_PREFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallPreferenceMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_PREFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallPreferenceMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallPreferenceMode(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_CALL_PREFERENCE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_1, GetSerialId(), "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_USSD));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetUssd(SLOTID_2, GetSerialId(), "12345678");
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_USSD));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetUssd_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_USSD));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetUssd_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetUssd(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_USSD));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetMute_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_MUTE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetMute_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetMute(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_MUTE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetMute_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_MUTE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetMute_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetMute(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_MUTE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallFailReason_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_FAIL_REASON));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCallFailReason_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCallFailReason(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_GET_FAIL_REASON));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_CallSupplement_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_CALL_SUPPLEMENT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_CallSupplement_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->CallSupplement(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_CALL_SUPPLEMENT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 300;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEND_DTMF));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    dtmfInfo.onLength = 1;
    dtmfInfo.offLength = 0;
    dtmfInfo.stringLength = 1;
    int32_t ret = g_rilInterface->SendDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SEND_DTMF));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_StartDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_START_DTMF));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_StartDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StartDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_START_DTMF));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_StopDtmf_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_1, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_STOP_DTMF));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_StopDtmf_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IDtmfInfo dtmfInfo = {};
    dtmfInfo.callId = 0;
    dtmfInfo.dtmfKey = "#";
    int32_t ret = g_rilInterface->StopDtmf(SLOTID_2, GetSerialId(), dtmfInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_STOP_DTMF));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetBarringPassword_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_1, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_BARRING_PASSWORD));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetBarringPassword_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISetBarringInfo setBarringInfo = {};
    setBarringInfo.fac = "AB";
    setBarringInfo.oldPassword = "1234";
    setBarringInfo.newPassword = "2345";
    int32_t ret = g_rilInterface->SetBarringPassword(SLOTID_2, GetSerialId(), setBarringInfo);
    WaitFor(WAIT_TIME_SECOND_60);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_CALL_SET_BARRING_PASSWORD));
}

// Network
HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetRadioCapability_V1_0100, Function | MediumTest | Level3)
{
    BeginModule("Network");
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetRadioCapability(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_RADIO_CAPABILITY));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetRadioCapability_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetRadioCapability(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_RADIO_CAPABILITY));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetOperatorInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_OPERATOR_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetOperatorInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->GetOperatorInfo(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_OPERATOR_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSignalStrength_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSignalStrength_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetSignalStrength(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCsRegStatus_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CS_REG_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCsRegStatus_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCsRegStatus(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CS_REG_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPsRegStatus_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PS_REG_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPsRegStatus_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetPsRegStatus(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PS_REG_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetNetworkSearchInformation_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetNetworkSearchInformation_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSearchInformation(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_180);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SEARCH_INFORMATION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetNetworkSelectionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetNetworkSelectionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetNetworkSelectionMode(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetNetworkSelectionMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(SelectionMode::MODE_TYPE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_1, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetNetworkSelectionMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = static_cast<int32_t>(SelectionMode::MODE_TYPE_AUTO);
    setNetworkModeInfo.oper = "46000";
    int32_t ret = g_rilInterface->SetNetworkSelectionMode(SLOTID_2, GetSerialId(), setNetworkModeInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetPreferredNetwork_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetPreferredNetwork_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetPreferredNetwork(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_LTE));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPreferredNetwork_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPreferredNetwork_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetPreferredNetwork(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCellInfoList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCellInfoList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetNeighboringCellInfoList(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCurrentCellInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCurrentCellInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCurrentCellInfo(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPhysicalChannelConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetPhysicalChannelConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_GET_PHYSICAL_CHANNEL_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetLocateUpdates_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret =
        g_rilInterface->SetLocateUpdates(SLOTID_1, GetSerialId(), IHRilRegNotifyMode::REG_NOTIFY_STAT_LAC_CELLID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetLocateUpdates_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret =
        g_rilInterface->SetLocateUpdates(SLOTID_2, GetSerialId(), IHRilRegNotifyMode::REG_NOTIFY_STAT_LAC_CELLID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetNotificationFilter_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetNotificationFilter_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetNotificationFilter(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(NotificationFilter::NOTIFICATION_FILTER_ALL));
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetDeviceState_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(
        SLOTID_1, GetSerialId(), static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_DEVICE_STATE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetDeviceState_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetDeviceState(
        SLOTID_2, GetSerialId(), static_cast<int32_t>(DeviceStateType::LOW_DATA_STATE), 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_NETWORK_SET_DEVICE_STATE));
}

/**
 * modem Test
 **/
HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_ShutDown_V1_0100, Function | MediumTest | Level3)
{
    BeginModule("modem");
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SHUT_DOWN));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_ShutDown_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->ShutDown(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SHUT_DOWN));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetRadioState_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SET_RADIO_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetRadioState_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetRadioState(SLOTID_1, GetSerialId(), 1, 0);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_SET_RADIO_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetRadioState_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_RADIO_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetRadioState_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetRadioState(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_RADIO_STATUS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetImei_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_IMEI));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetImei_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetImei(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_IMEI));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetMeid_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_MEID));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetMeid_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetMeid(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_MEID));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetBasebandVersion_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_BASEBAND_VERSION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetBasebandVersion_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetBasebandVersion(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_BASEBAND_VERSION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_VOICE_RADIO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetVoiceRadioTechnology_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetVoiceRadioTechnology(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_MODEM_GET_VOICE_RADIO));
}

// Data
HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_ActivatePdpContext_V1_0100, Function | MediumTest | Level3)
{
    BeginModule("Data");
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IDataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.verType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    IDataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = 0;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_1, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_ActivatePdpContext_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IDataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.verType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    IDataCallInfo dataCallInfo;
    dataCallInfo.serial = serialId;
    dataCallInfo.radioTechnology = 0;
    dataCallInfo.dataProfileInfo = dataProfileInfo;
    dataCallInfo.roamingAllowed = true;
    dataCallInfo.isRoaming = false;
    int32_t ret = g_rilInterface->ActivatePdpContext(SLOTID_2, serialId, dataCallInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_ACTIVATE_PDP_CONTEXT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_DeactivatePdpContext_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IUniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = CID;
    uniInfo.arg1 = REASON;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_DeactivatePdpContext_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IUniInfo uniInfo;
    uniInfo.serial = serialId;
    uniInfo.gsmIndex = CID;
    uniInfo.arg1 = REASON;
    int32_t ret = g_rilInterface->DeactivatePdpContext(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPdpContextList_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IUniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_1, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetPdpContextList_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IUniInfo uniInfo;
    uniInfo.serial = serialId;
    int32_t ret = g_rilInterface->GetPdpContextList(SLOTID_2, serialId, uniInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_PDP_CONTEXT_LIST));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetInitApnInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IDataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.verType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    int32_t ret = g_rilInterface->SetInitApnInfo(SLOTID_1, serialId, dataProfileInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_INIT_APN_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetInitApnInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    IDataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.verType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial = serialId;
    int32_t ret = g_rilInterface->SetInitApnInfo(SLOTID_2, serialId, dataProfileInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_INIT_APN_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_1, GetSerialId(), CID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetLinkBandwidthInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetLinkBandwidthInfo(SLOTID_2, GetSerialId(), CID);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_GET_LINK_BANDWIDTH_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    const int32_t BANDWIDTH_HYSTERESIS_MS = 3000;
    const int32_t BANDWIDTH_HYSTERESIS_KBPS = 50;
    const int32_t MAX_UPLINK_LINK_BANDWIDTH[] = { 100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000 };
    uint32_t uplinkKbpsSize = sizeof(MAX_UPLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    const int32_t MAX_DOWNLINK_LINK_BANDWIDTH[] = { 100, // VoIP
        500, // Web
        1000, // SD
        5000, // HD
        10000, // file
        20000, // 4K
        50000, // LTE
        100000,
        200000, // 5G
        500000, 1000000 };
    uint32_t downlinkKbpsSize = sizeof(MAX_DOWNLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    IDataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_LTE);
    dLinkBandwidth.delayMs = BANDWIDTH_HYSTERESIS_MS;
    dLinkBandwidth.delayUplinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
    dLinkBandwidth.delayDownlinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(MAX_UPLINK_LINK_BANDWIDTH[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(MAX_DOWNLINK_LINK_BANDWIDTH[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_1, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetLinkBandwidthReportingRule_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t serialId = GetSerialId();
    const int32_t BANDWIDTH_HYSTERESIS_MS = 3000;
    const int32_t BANDWIDTH_HYSTERESIS_KBPS = 50;
    const int32_t MAX_UPLINK_LINK_BANDWIDTH[] = { 100, 500, 1000, 5000, 10000, 20000, 50000, 100000, 200000 };
    uint32_t uplinkKbpsSize = sizeof(MAX_UPLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    const int32_t MAX_DOWNLINK_LINK_BANDWIDTH[] = { 100, // VoIP
        500, // Web
        1000, // SD
        5000, // HD
        10000, // file
        20000, // 4K
        50000, // LTE
        100000,
        200000, // 5G
        500000, 1000000 };
    uint32_t downlinkKbpsSize = sizeof(MAX_DOWNLINK_LINK_BANDWIDTH) / sizeof(int32_t);
    IDataLinkBandwidthReportingRule dLinkBandwidth;
    dLinkBandwidth.serial = serialId;
    dLinkBandwidth.rat = static_cast<int32_t>(RatType::NETWORK_TYPE_LTE);
    dLinkBandwidth.delayMs = BANDWIDTH_HYSTERESIS_MS;
    dLinkBandwidth.delayUplinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
    dLinkBandwidth.delayDownlinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
    dLinkBandwidth.maximumUplinkKbpsSize = uplinkKbpsSize;
    dLinkBandwidth.maximumDownlinkKbpsSize = downlinkKbpsSize;
    for (uint32_t i = 0; i < uplinkKbpsSize; i++) {
        dLinkBandwidth.maximumUplinkKbps.push_back(MAX_UPLINK_LINK_BANDWIDTH[i]);
    }
    for (uint32_t i = 0; i < downlinkKbpsSize; i++) {
        dLinkBandwidth.maximumDownlinkKbps.push_back(MAX_DOWNLINK_LINK_BANDWIDTH[i]);
    }
    int32_t ret = g_rilInterface->SetLinkBandwidthReportingRule(SLOTID_2, GetSerialId(), dLinkBandwidth);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetDataProfileInfo_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t serialId = GetSerialId();
    IDataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    IDataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.verType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial =serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_1,serialId,dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS,ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetDataProfileInfo_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t serialId = GetSerialId();
    IDataProfilesInfo dataProfilesInfo;
    dataProfilesInfo.serial = serialId;
    dataProfilesInfo.profilesSize = 1;
    IDataProfileDataInfo dataProfileInfo;
    dataProfileInfo.profileId = 0;
    dataProfileInfo.password = "";
    dataProfileInfo.verType = 0;
    dataProfileInfo.userName = "";
    dataProfileInfo.apn = "cmnet";
    dataProfileInfo.protocol = "IPV4V6";
    dataProfileInfo.roamingProtocol = "IPV4V6";
    dataProfileInfo.serial =serialId;
    dataProfilesInfo.profiles.push_back(dataProfileInfo);
    int32_t ret = g_rilInterface->SetDataProfileInfo(SLOTID_2,serialId,dataProfilesInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS,ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetDataPermitted_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_1, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PERMITTED));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetDataPermitted_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SetDataPermitted(SLOTID_2, GetSerialId(), 1);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_DATA_SET_DATA_PERMITTED));
}

// SIM
HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSimIO_V1_0100, Function | MediumTest | Level3)
{
    BeginModule("SIM");
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimIoRequestInfo msg;
    msg.command = 0xc0;
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_IO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSimIO_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimIoRequestInfo msg;
    msg.command = 0xC0;
    // ICCID
    msg.fileId = 0x2FE2;
    msg.p1 = 0;
    msg.p2 = 0;
    msg.p3 = 15;
    msg.data = "";
    msg.path = "3F00";
    msg.pin2 = "";
    int32_t ret = g_rilInterface->GetSimIO(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_IO));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_1, GetSerialId(),true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkSendCallSetupRequestResult_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        return;
    }
    int32_t ret = g_rilInterface->SimStkSendCallSetupRequestResult(SLOTID_2, GetSerialId(),true);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetImsi_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_IMSI));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetImsi_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetImsi(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_IMSI));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetActiveSim_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t enable = 1;
    int32_t index = 1; // ENTITY_CARD
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_1, GetSerialId(), index, enable);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_ACTIVE_SIM));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetActiveSim_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t enable = 1;
    int32_t index = 1; // ENTITY_CARD
    int32_t ret = g_rilInterface->SetActiveSim(SLOTID_2, GetSerialId(), index, enable);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_ACTIVE_SIM));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string cmd = "send terminal response";
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_1, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkSendTerminalResponse_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string cmd = "send terminal response";
    int32_t ret = g_rilInterface->SimStkSendTerminalResponse(SLOTID_2, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkSendEnvelope_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string cmd = "send envelope";
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_1, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_ENVELOPE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkSendEnvelope_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string cmd = "send envelope";
    int32_t ret = g_rilInterface->SimStkSendEnvelope(SLOTID_2, GetSerialId(), cmd);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_SEND_ENVELOPE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkIsReady_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_IS_READY));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimStkIsReady_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SimStkIsReady(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_STK_IS_READY));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetRadioProtocol_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimProtocolRequest protocol;
    protocol.slotId = SLOTID_1;
    protocol.phase = 0; // SET_PROTOCOL
    protocol.protocol = 12; // MAX
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_1, GetSerialId(), protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_RADIO_PROTOCOL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetRadioProtocol_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimProtocolRequest protocol;
    protocol.slotId = SLOTID_2;
    protocol.phase = 0; // SET_PROTOCOL
    protocol.protocol = 12; // MAX
    int32_t ret = g_rilInterface->SetRadioProtocol(SLOTID_2, GetSerialId(), protocol);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_RADIO_PROTOCOL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimOpenLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string appID = "A00000015141434C00"; // ARAM_AID
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_1, GetSerialId(), appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IApduSimIORequestInfo reqInfo;
    reqInfo.channelId = currentChannelId_;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_1, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimCloseLogicalChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_1, GetSerialId(), currentChannelId_);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimOpenLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string appID = "A00000015141434C00"; // ARAM_AID
    int32_t p2 = 0;
    int32_t ret = g_rilInterface->SimOpenLogicalChannel(SLOTID_2, GetSerialId(), appID, p2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimTransmitApduLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IApduSimIORequestInfo reqInfo;
    reqInfo.channelId = currentChannelId_;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduLogicalChannel(SLOTID_2, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimCloseLogicalChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->SimCloseLogicalChannel(SLOTID_2, GetSerialId(), currentChannelId_);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IApduSimIORequestInfo reqInfo;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_1, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimTransmitApduBasicChannel_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IApduSimIORequestInfo reqInfo;
    reqInfo.type = 0x80; // CLA
    reqInfo.instruction = 0xCA; // COMMAND;
    reqInfo.p1 = 0xFF;
    reqInfo.p2 = 0x40;
    reqInfo.p3 = 0x00;
    reqInfo.data = "";
    int32_t ret = g_rilInterface->SimTransmitApduBasicChannel(SLOTID_2, GetSerialId(), reqInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimAuthentication_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = "aid";
    simAuthInfo.authData = "authData";
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_1, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_AUTHENTICATION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SimAuthentication_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimAuthenticationRequestInfo simAuthInfo;
    simAuthInfo.aid = "aid";
    simAuthInfo.authData = "authData";
    int32_t ret = g_rilInterface->SimAuthentication(SLOTID_2, GetSerialId(), simAuthInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_AUTHENTICATION));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSimLockStatus_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);	
        return;
    }
    ISimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    constexpr const char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSimLockStatus_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    constexpr const char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->GetSimLockStatus(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetSimLock_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    simLockInfo.passwd = "1234";
    constexpr const char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->SetSimLock(SLOTID_1, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetSimLock_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimLockInfo simLockInfo;
    simLockInfo.fac = "SC"; // FAC_PIN_LOCK
    simLockInfo.mode = 2; // MODE
    simLockInfo.classx = 0;
    simLockInfo.passwd = "1234";
    constexpr const char *FAC_PIN_LOCK = "SC";
    int32_t ret = g_rilInterface->SetSimLock(SLOTID_2, GetSerialId(), simLockInfo);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_SET_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_ChangeSimPassword_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC"; // FAC_PIN_LOCK
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_1, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_ChangeSimPassword_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);	
        return;
    }
    ISimPasswordInfo simPassword;
    simPassword.fac = "SC"; // FAC_PIN_LOCK
    simPassword.oldPassword = "1234";
    simPassword.newPassword = "1234";
    simPassword.passwordLength = 4;
    int32_t ret = g_rilInterface->ChangeSimPassword(SLOTID_2, GetSerialId(), simPassword);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPin_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);	
        return;
    }
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPin(SLOTID_1, GetSerialId(), pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPin_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPin(SLOTID_2, GetSerialId(), pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPuk_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);	
        return;
    }
    std::string puk = "42014264";
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPuk(SLOTID_1, GetSerialId(), puk, pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPuk_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string puk = "42014264";
    std::string pin = "1234";
    int32_t ret = g_rilInterface->UnlockPuk(SLOTID_2, GetSerialId(), puk, pin);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPin2_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPin2(SLOTID_1, GetSerialId(), pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPin2_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPin2(SLOTID_2, GetSerialId(), pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PIN2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPuk2_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string puk2 = "42014264";
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPuk2(SLOTID_1, GetSerialId(), puk2, pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockPuk2_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    std::string puk2 = "42014264";
    std::string pin2 = "1234";
    int32_t ret = g_rilInterface->UnlockPuk2(SLOTID_2, GetSerialId(), puk2, pin2);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_PUK2));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockSimLock_V1_0100, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_1)) {
		ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t lockType = 0;
    std::string key = "1234"; // PN_PIN_LOCK
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_1, GetSerialId(), lockType, key);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UnlockSimLock_V1_0200, Function | MediumTest | Level3)
{
#ifdef TEL_TEST_PIN_PUK
    if (!IsReady(SLOTID_2)) {
		ASSERT_NE(nullptr, g_rilInterface);	
        return;
    }
    int32_t lockType = 0;
    std::string key = "1234"; // PN_PIN_LOCK
    int32_t ret = g_rilInterface->UnlockSimLock(SLOTID_2, GetSerialId(), lockType, key);
    WaitFor(WAIT_TIME_SECOND);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SIM_UNLOCK_SIM_LOCK));
#endif
    ASSERT_TRUE(true);
}

// Sms
HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_AddSimMessage_V1_0100, Function | MediumTest | Level3)
{
    BeginModule("Sms");
    WaitFor(WAIT_TIME_SECOND_BEGIN);
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_AddSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UpdateSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UpdateSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_DelSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_DelSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_AddCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_AddCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    int32_t ret = g_rilInterface->AddCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_1, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_UpdateCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISmsMessageIOInfo msgIoInfo;
    msgIoInfo.smscPdu = TEST_SMSC_PDU;
    msgIoInfo.pdu = TEST_STORAGE_PDU;
    msgIoInfo.state = static_cast<int32_t>(SimMessageStatus::SIM_MESSAGE_STATUS_SENT);
    msgIoInfo.index = 0;
    int32_t ret = g_rilInterface->UpdateCdmaSimMessage(SLOTID_2, GetSerialId(), msgIoInfo);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_DelCdmaSimMessage_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_1, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_DelCdmaSimMessage_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t index = 0;
    int32_t ret = g_rilInterface->DelCdmaSimMessage(SLOTID_2, GetSerialId(), index);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetSmscAddr_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = TEST_TOSCA;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_1, GetSerialId(), address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_SMSC_ADDR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetSmscAddr_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IServiceCenterAddress address;
    address.address = TEST_SMSC_ADDR;
    address.tosca = TEST_TOSCA;
    int32_t ret = g_rilInterface->SetSmscAddr(SLOTID_2, GetSerialId(), address);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_SMSC_ADDR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSmscAddr_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_SMSC_ADDR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetSmscAddr_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetSmscAddr(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_SMSC_ADDR));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t GSM_TYPE = 1;
    ICBConfigInfo info;
    info.mode = GSM_TYPE;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_1, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t GSM_TYPE = 1;
    ICBConfigInfo info;
    info.mode = GSM_TYPE;
    info.mids = TEST_ID_LIST;
    info.dcss = TEST_DCS_LIST;
    int32_t ret = g_rilInterface->SetCBConfig(SLOTID_2, GetSerialId(), info);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCBConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCdmaCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ICdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_1, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SetCdmaCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ICdmaCBConfigInfoList broadcastInfoList = {};
    int32_t ret = g_rilInterface->SetCdmaCBConfig(SLOTID_2, GetSerialId(), broadcastInfoList);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCdmaCBConfig_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_1, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_GetCdmaCBConfig_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    int32_t ret = g_rilInterface->GetCdmaCBConfig(SLOTID_2, GetSerialId());
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendSmsMoreMode_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IGsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_MORE_MODE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendSmsMoreMode_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IGsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendSmsMoreMode(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_MORE_MODE));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendSmsAck_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IModeData data;
    data.result = TEST_RESULT;
    data.mode = TEST_MODE;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_1, GetSerialId(), data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_ACK));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendSmsAck_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IModeData data;
    data.result = TEST_RESULT;
    data.mode = TEST_MODE;
    int32_t ret = g_rilInterface->SendSmsAck(SLOTID_2, GetSerialId(), data);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_SMS_ACK));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendGsmSms_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IGsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_GSM_SMS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendGsmSms_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    IGsmSmsMessageInfo msg;
    msg.smscPdu = TEST_SMSC_PDU;
    msg.pdu = TEST_SEND_PDU;
    int32_t ret = g_rilInterface->SendGsmSms(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_GSM_SMS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendCdmaSms_V1_0100, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_1)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_1, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_CDMA_SMS));
}

HWTEST_F(HdfRilHdiTest, Telephony_DriverSystem_SendCdmaSms_V1_0200, Function | MediumTest | Level3)
{
    if (!IsReady(SLOTID_2)) {
        ASSERT_NE(nullptr, g_rilInterface);
        return;
    }
    ISendCdmaSmsMessageInfo msg;
    msg.smscPdu = TEST_CDMA_PDU;
    int32_t ret = g_rilInterface->SendCdmaSms(SLOTID_2, GetSerialId(), msg);
    WaitFor(WAIT_TIME_SECOND_LONG);
    EXPECT_EQ(SUCCESS, ret);
    ASSERT_TRUE(GetBoolResult(HdiId::HREQ_SMS_SEND_CDMA_SMS));
}