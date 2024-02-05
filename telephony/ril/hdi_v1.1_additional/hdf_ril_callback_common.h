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
#include "hdf_base.h"
#include "hdf_log.h"
#include "map"
#include "mutex"
#include "securec.h"
#include "cstdlib"
#include "unistd.h"
#include "v1_2/iril.h"
#include "gtest/gtest.h"

#define RAND_MAXVALUE 10000000000

using OHOS::HDI::Ril::V1_2::RilRadioResponseInfo;
using OHOS::HDI::Ril::V1_2::RingbackVoice;
using OHOS::HDI::Ril::V1_2::SrvccStatus;
using OHOS::HDI::Ril::V1_2::UssdNoticeInfo;
using OHOS::HDI::Ril::V1_2::SsNoticeInfo;
using OHOS::HDI::Ril::V1_2::EmergencyInfoList;
using OHOS::HDI::Ril::V1_2::CallInfoList;
using OHOS::HDI::Ril::V1_2::GetClipResult;
using OHOS::HDI::Ril::V1_2::CallWaitResult;
using OHOS::HDI::Ril::V1_2::CallForwardQueryInfoList;
using OHOS::HDI::Ril::V1_2::CallRestrictionResult;
using OHOS::HDI::Ril::V1_2::GetClirResult;
using OHOS::HDI::Ril::V1_2::DataCallResultList;
using OHOS::HDI::Ril::V1_2::DataLinkCapability;
using OHOS::HDI::Ril::V1_2::SetupDataCallResultInfo;
using OHOS::HDI::Ril::V1_2::DataLinkBandwidthInfo;
using OHOS::HDI::Ril::V1_2::VoiceRadioTechnology;
using OHOS::HDI::Ril::V1_2::RadioProtocol;
using OHOS::HDI::Ril::V1_2::IccIoResultInfo;
using OHOS::HDI::Ril::V1_2::CardStatusInfo;
using OHOS::HDI::Ril::V1_2::LockStatusResp;
using OHOS::HDI::Ril::V1_2::OpenLogicalChannelResponse;
using OHOS::HDI::Ril::V1_2::CsRegStatusInfo;
using OHOS::HDI::Ril::V1_2::PsRegStatusInfo;
using OHOS::HDI::Ril::V1_2::Rssi;
using OHOS::HDI::Ril::V1_2::ChannelConfigInfoList;
using OHOS::HDI::Ril::V1_2::CellListCurrentInfo_1_1;
using OHOS::HDI::Ril::V1_2::CellListCurrentInfo;
using OHOS::HDI::Ril::V1_2::PreferredNetworkTypeInfo;
using OHOS::HDI::Ril::V1_2::OperatorInfo;
using OHOS::HDI::Ril::V1_2::AvailableNetworkList;
using OHOS::HDI::Ril::V1_2::SetNetworkModeInfo;
using OHOS::HDI::Ril::V1_2::CellListNearbyInfo;
using OHOS::HDI::Ril::V1_2::NrCellSsbIds;
using OHOS::HDI::Ril::V1_2::SmsMessageInfo;
using OHOS::HDI::Ril::V1_2::CBConfigReportInfo;
using OHOS::HDI::Ril::V1_2::SendSmsResultInfo;
using OHOS::HDI::Ril::V1_2::ServiceCenterAddress;
using OHOS::HDI::Ril::V1_2::CBConfigInfo;
using OHOS::HDI::Ril::V1_2::CdmaCBConfigInfo;
using OHOS::HDI::Ril::V1_2::PhysicalChannelConfig;
using OHOS::HDI::Ril::V1_2::CellListCurrentInfo_1_2;
using OHOS::HDI::Ril::V1_2::CellListNearbyInfo_1_2;

enum class HdiId {
    HREQ_NONE = -1,
    HREQ_CALL_BASE = 0,
    HREQ_CALL_GET_CALL_LIST,
    HREQ_CALL_DIAL,
    HREQ_CALL_HANGUP,
    HREQ_CALL_REJECT,
    HREQ_CALL_ANSWER,
    HREQ_CALL_HOLD_CALL,
    HREQ_CALL_UNHOLD_CALL,
    HREQ_CALL_SWITCH_CALL,
    HREQ_CALL_COMBINE_CONFERENCE,
    HREQ_CALL_SEPARATE_CONFERENCE,
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
    HREQ_CALL_CLOSE_UNFINISHED_USSD,
    HREQ_SET_VONR_SWITCH,

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
    HREQ_SIM_SEND_NCFG_OPER_INFO,

    HREQ_DATA_BASE = 300,
    HREQ_DATA_SET_INIT_APN_INFO,
    HREQ_DATA_DEACTIVATE_PDP_CONTEXT,
    HREQ_DATA_ACTIVATE_PDP_CONTEXT,
    HREQ_DATA_GET_PDP_CONTEXT_LIST,
    HREQ_DATA_GET_LINK_BANDWIDTH_INFO,
    HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE,
    HREQ_DATA_SET_DATA_PROFILE_INFO,
    HREQ_DATA_SEND_DATA_PERFORMANCE_MODE,
    HREQ_DATA_SEND_DATA_SLEEP_MODE,
    HREQ_DATA_SET_DATA_PERMITTED,
    HREQ_DATA_GET_LINK_CAPABILITY,
    HREQ_DATA_CLEAN_ALL_CONNECTIONS,

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
    HREQ_NETWORK_GET_RRC_CONNECTION_STATE,
    HREQ_NETWORK_SET_NR_OPTION_MODE,
    HREQ_NETWORK_GET_NR_OPTION_MODE,
    HREQ_NETWORK_GET_NR_SSBID_INFO,

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

enum class NrMode { NR_MODE_UNKNOWN, NR_MODE_NSA_ONLY, NR_MODE_SA_ONLY, NR_MODE_NSA_AND_SA };

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

class RilCallback : public OHOS::HDI::Ril::V1_2::IRilCallback {
public:
    void NotifyAll();
    void WaitFor(int32_t timeoutSecond);
    bool GetBoolResult(HdiId hdiId);
    void Clean();

    int32_t CallStateUpdated(const RilRadioResponseInfo &responseInfo) override;
    int32_t CallRingbackVoiceNotice(const RilRadioResponseInfo &responseInfo,
                                    const RingbackVoice &ringbackVoice) override;
    int32_t CallSrvccStatusNotice(const RilRadioResponseInfo &responseInfo, const SrvccStatus &srvccStatus) override;
    int32_t CallUssdNotice(const RilRadioResponseInfo &responseInfo, const UssdNoticeInfo &ussdNoticeInfo) override;
    int32_t CallSsNotice(const RilRadioResponseInfo &responseInfo, const SsNoticeInfo &ssNoticeInfo) override;
    int32_t CallEmergencyNotice(const RilRadioResponseInfo &responseInfo,
                                const EmergencyInfoList &emergencyInfoList) override;
    int32_t CallRsrvccStatusNotify(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetEmergencyCallListResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetEmergencyCallListResponse(const RilRadioResponseInfo &responseInfo,
                                         const EmergencyInfoList &emergencyInfoList) override;
    int32_t GetCallListResponse(const RilRadioResponseInfo &responseInfo, const CallInfoList &callList) override;
    int32_t DialResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t HangupResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t RejectResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t AnswerResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t HoldCallResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t UnHoldCallResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SwitchCallResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetClipResponse(const RilRadioResponseInfo &responseInfo, const GetClipResult &getClipResult) override;
    int32_t SetClipResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t CombineConferenceResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SeparateConferenceResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t CallSupplementResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallWaitingResponse(const RilRadioResponseInfo &responseInfo,
                                   const CallWaitResult &callWaitResult) override;
    int32_t SetCallWaitingResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallTransferInfoResponse(const RilRadioResponseInfo &responseInfo,
                                        const CallForwardQueryInfoList &cFQueryList) override;
    int32_t SetCallTransferInfoResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallRestrictionResponse(const RilRadioResponseInfo &responseInfo,
                                       const CallRestrictionResult &result) override;
    int32_t SetCallRestrictionResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetClirResponse(const RilRadioResponseInfo &responseInfo, const GetClirResult &getClirResult) override;
    int32_t SetClirResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t StartDtmfResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SendDtmfResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t StopDtmfResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo, int32_t mode) override;
    int32_t SetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetUssdResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetUssdResponse(const RilRadioResponseInfo &responseInfo, int32_t cusd) override;
    int32_t SetMuteResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetMuteResponse(const RilRadioResponseInfo &responseInfo, int32_t mute) override;
    int32_t GetCallFailReasonResponse(const RilRadioResponseInfo &responseInfo, int32_t callFail) override;
    int32_t SetBarringPasswordResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t CloseUnFinishedUssdResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetVonrSwitchResponse(const RilRadioResponseInfo &responseInfo) override;

    int32_t PdpContextListUpdated(const RilRadioResponseInfo &responseInfo,
                                  const DataCallResultList &dataCallResultList) override;
    int32_t DataLinkCapabilityUpdated(const RilRadioResponseInfo &responseInfo,
                                      const DataLinkCapability &dataLinkCapability) override;
    int32_t ActivatePdpContextResponse(const RilRadioResponseInfo &responseInfo,
                                       const SetupDataCallResultInfo &setupDataCallResultInfo) override;
    int32_t DeactivatePdpContextResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetPdpContextListResponse(const RilRadioResponseInfo &responseInfo,
                                      const DataCallResultList &dataCallResultList) override;
    int32_t SetInitApnInfoResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetLinkBandwidthReportingRuleResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetLinkBandwidthInfoResponse(const RilRadioResponseInfo &responseInfo,
                                         const DataLinkBandwidthInfo &dataLinkBandwidthInfo) override;
    int32_t SetDataPermittedResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetDataProfileInfoResponse(const RilRadioResponseInfo &responseInfo);
    int32_t GetLinkCapabilityResponse(const RilRadioResponseInfo &responseInfo,
                                      const DataLinkCapability &dataLinkCapability) override;
    int32_t CleanAllConnectionsResponse(const RilRadioResponseInfo &responseInfo) override;

    int32_t RadioStateUpdated(const RilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t VoiceRadioTechUpdated(const RilRadioResponseInfo &responseInfo,
                                  const VoiceRadioTechnology &voiceRadioTechnology) override;
    int32_t DsdsModeUpdated(const RilRadioResponseInfo &responseInfo, int32_t mode) override;
    int32_t ShutDownResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetRadioStateResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetRadioStateResponse(const RilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t GetImeiResponse(const RilRadioResponseInfo &responseInfo, const std::string &imei) override;
    int32_t GetMeidResponse(const RilRadioResponseInfo &responseInfo, const std::string &meid) override;
    int32_t GetVoiceRadioTechnologyResponse(const RilRadioResponseInfo &responseInfo,
                                            const VoiceRadioTechnology &voiceRadioTechnology) override;
    int32_t GetBasebandVersionResponse(const RilRadioResponseInfo &responseInfo,
                                       const std::string &basebandVersion) override;

    int32_t SimStateUpdated(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSessionEndNotify(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkProactiveNotify(const RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t SimStkAlphaNotify(const RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t SimStkEventNotify(const RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t SimStkCallSetupNotify(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimRefreshNotify(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimRadioProtocolUpdated(const RilRadioResponseInfo &responseInfo,
                                    const RadioProtocol &radioProtocol) override
    {
        return 0;
    }
    int32_t GetSimIOResponse(const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result) override;
    int32_t GetSimStatusResponse(const RilRadioResponseInfo &responseInfo, const CardStatusInfo &result) override;
    int32_t GetImsiResponse(const RilRadioResponseInfo &responseInfo, const std::string &response) override;
    int32_t GetSimLockStatusResponse(const RilRadioResponseInfo &responseInfo, int32_t simLockStatus) override;
    int32_t SetSimLockResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus) override;
    int32_t ChangeSimPasswordResponse(const RilRadioResponseInfo &responseInfo,
                                      const LockStatusResp &lockStatus) override;
    int32_t UnlockPinResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus) override;
    int32_t UnlockPukResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus) override;
    int32_t UnlockPin2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus) override;
    int32_t UnlockPuk2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus) override;
    int32_t SetActiveSimResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendTerminalResponseResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendEnvelopeResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkSendCallSetupRequestResultResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimStkIsReadyResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetRadioProtocolResponse(const RilRadioResponseInfo &responseInfo,
                                     const RadioProtocol &radioProtocol) override;
    int32_t GetRadioProtocolResponse(const RilRadioResponseInfo &responseInfo,
                                     const RadioProtocol &radioProtocol) override;
    int32_t SimOpenLogicalChannelResponse(const RilRadioResponseInfo &responseInfo,
                                          const OpenLogicalChannelResponse &pOpenLogicalChannelResponse) override;
    int32_t SimCloseLogicalChannelResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SimTransmitApduLogicalChannelResponse(const RilRadioResponseInfo &responseInfo,
                                                  const IccIoResultInfo &result) override;
    int32_t SimTransmitApduBasicChannelResponse(const RilRadioResponseInfo &responseInfo,
                                                const IccIoResultInfo &result) override;
    int32_t SimAuthenticationResponse(const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result) override;
    int32_t UnlockSimLockResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus) override;
    int32_t SendSimMatchedOperatorInfoResponse(const RilRadioResponseInfo &responseInfo) override;

    int32_t NetworkCsRegStatusUpdated(const RilRadioResponseInfo &responseInfo,
                                      const CsRegStatusInfo &csRegStatusInfo) override;
    int32_t NetworkPsRegStatusUpdated(const RilRadioResponseInfo &responseInfo,
                                      const PsRegStatusInfo &psRegStatusInfo) override;
    int32_t SignalStrengthUpdated(const RilRadioResponseInfo &responseInfo, const Rssi &rssi) override;
    int32_t NetworkTimeZoneUpdated(const RilRadioResponseInfo &responseInfo, const std::string &timeZoneStr) override;
    int32_t NetworkTimeUpdated(const RilRadioResponseInfo &responseInfo, const std::string &timeStr) override;
    int32_t NetworkPhyChnlCfgUpdated(const RilRadioResponseInfo &responseInfo,
                                     const ChannelConfigInfoList &channelConfigInfoList) override;
    int32_t NetworkCurrentCellUpdated(const RilRadioResponseInfo &responseInfo,
                                      const CellListCurrentInfo &cellListCurrentInfo) override;
    int32_t NetworkCurrentCellUpdated_1_1(const RilRadioResponseInfo &responseInfo,
                                          const CellListCurrentInfo_1_1 &cellListCurrentInfo) override;
    int32_t NetworkCurrentCellUpdated_1_2(const RilRadioResponseInfo &responseInfo,
                                          const CellListCurrentInfo_1_2 &cellListCurrentInfo) override;
    int32_t ResidentNetworkUpdated(const RilRadioResponseInfo &responseInfo, const std::string &plmn) override;
    int32_t GetSignalStrengthResponse(const RilRadioResponseInfo &responseInfo, const Rssi &rssi) override;
    int32_t GetCsRegStatusResponse(const RilRadioResponseInfo &responseInfo,
                                   const CsRegStatusInfo &csRegStatusInfo) override;
    int32_t GetPsRegStatusResponse(const RilRadioResponseInfo &responseInfo,
                                   const PsRegStatusInfo &psRegStatusInfo) override;
    int32_t GetOperatorInfoResponse(const RilRadioResponseInfo &responseInfo,
                                    const OperatorInfo &operatorInfo) override;
    int32_t GetNetworkSearchInformationResponse(const RilRadioResponseInfo &responseInfo,
                                                const AvailableNetworkList &availableNetworkList) override;
    int32_t GetNetworkSelectionModeResponse(const RilRadioResponseInfo &responseInfo,
                                            const SetNetworkModeInfo &setNetworkModeInfo) override;
    int32_t SetNetworkSelectionModeResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetNeighboringCellInfoListResponse(const RilRadioResponseInfo &responseInfo,
                                               const CellListNearbyInfo &cellInfoList) override;
    int32_t GetNeighboringCellInfoListResponse_1_2(const RilRadioResponseInfo &responseInfo,
                                                   const CellListNearbyInfo_1_2 &cellInfoList) override;
    int32_t GetCurrentCellInfoResponse(const RilRadioResponseInfo &responseInfo,
                                       const CellListCurrentInfo &cellInfoList) override;
    int32_t GetCurrentCellInfoResponse_1_1(const RilRadioResponseInfo &responseInfo,
                                           const CellListCurrentInfo_1_1 &cellListCurrentInfo) override;
    int32_t GetCurrentCellInfoResponse_1_2(const RilRadioResponseInfo &responseInfo,
                                           const CellListCurrentInfo_1_2 &cellListCurrentInfo) override;
    int32_t SetPreferredNetworkResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetPreferredNetworkResponse(const RilRadioResponseInfo &responseInfo,
                                        const PreferredNetworkTypeInfo &preferredNetworkTypeInfo) override;
    int32_t GetPhysicalChannelConfigResponse(const RilRadioResponseInfo &responseInfo,
                                             const ChannelConfigInfoList &channelConfigInfoList) override;
    int32_t SetLocateUpdatesResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetNotificationFilterResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetDeviceStateResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetRrcConnectionStateResponse(const RilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t SetNrOptionModeResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetNrOptionModeResponse(const RilRadioResponseInfo &responseInfo, int32_t mode) override;
    int32_t GetRrcConnectionStateUpdated(const RilRadioResponseInfo &responseInfo, int32_t state) override;
    int32_t GetNrSsbIdResponse(const RilRadioResponseInfo &responseInfo, const NrCellSsbIds &nrCellSsbIds) override;

    int32_t NewSmsNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                         const SmsMessageInfo &smsMessageInfo) override;
    int32_t NewCdmaSmsNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                             const SmsMessageInfo &smsMessageInfo) override;
    int32_t SmsStatusReportNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                  const SmsMessageInfo &smsMessageInfo) override;
    int32_t NewSmsStoredOnSimNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                    int32_t recordNumber, int32_t indicationType) override;
    int32_t CBConfigNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                           const CBConfigReportInfo &cellBroadConfigReportInfo) override;
    int32_t SendGsmSmsResponse(const RilRadioResponseInfo &responseInfo,
                               const SendSmsResultInfo &sendSmsResultInfo) override;
    int32_t SendCdmaSmsResponse(const RilRadioResponseInfo &responseInfo,
                                const SendSmsResultInfo &sendSmsResultInfo) override;
    int32_t AddSimMessageResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t DelSimMessageResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t UpdateSimMessageResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t AddCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t DelCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t UpdateCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t SetSmscAddrResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetSmscAddrResponse(const RilRadioResponseInfo &responseInfo,
                                const ServiceCenterAddress &serviceCenterAddress) override;
    int32_t SetCBConfigResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetCBConfigResponse(const RilRadioResponseInfo &responseInfo,
                                const CBConfigInfo &cellBroadcastInfo) override;
    int32_t SetCdmaCBConfigResponse(const RilRadioResponseInfo &responseInfo) override;
    int32_t GetCdmaCBConfigResponse(const RilRadioResponseInfo &responseInfo,
                                    const CdmaCBConfigInfo &cdmaCBConfigInfo) override;
    int32_t SendSmsMoreModeResponse(const RilRadioResponseInfo &responseInfo,
                                    const SendSmsResultInfo &sendSmsResultInfo) override;
    int32_t SendSmsAckResponse(const RilRadioResponseInfo &responseInfo) override;

    int32_t CommonErrorResponse(const RilRadioResponseInfo &responseInfo) override;

private:
    std::mutex callbackMutex_;
    std::condition_variable cv_;
    HdiId hdiId_;
    RilRadioResponseInfo resultInfo_;
};

extern OHOS::sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface;
extern RilCallback g_callback;

constexpr int32_t SLOTID_1 = 0;
constexpr int32_t SLOTID_2 = 1;
constexpr int32_t SUCCESS = 0;
constexpr int32_t WAIT_TIME_SECOND = 20;
constexpr int32_t WAIT_TIME_SECOND_LONG = 40;
extern std::map<int32_t, int32_t> g_simState;
extern int32_t g_currentChannelld;
const std::string TEST_STORAGE_PDU = "1234";
const std::string TEST_SEND_PDU = "A10305810180F6000004F4F29C0E";
const std::string TEST_SMSC_PDU = "00";
const std::string TEST_CDMA_PDU = "pdu";
const std::string TEST_SMSC_ADDR = "00";
const std::string TEST_ID_LIST = "0,1,5,320-478,922";
const std::string TEST_DCS_LIST = "0-3,5";
extern int32_t g_currentSerialId;
extern bool g_hangupResponseFlag;
extern bool g_rejectResponseFlag;
extern bool g_answerResponseFlag;
extern bool g_holdCallResponseFlag;
extern bool g_unHoldCallResponseFlag;
extern bool g_switchCallResponseFlag;
extern bool g_getClipResponseFlag;
extern bool g_setClipResponseFlag;
extern bool g_simStkSendTerminalResponseResponseFlag;
extern bool g_simStkSendEnvelopeResponseFlag;
extern bool g_simStkSendCallSetupRequestResultResponseFlag;
extern bool g_simStkIsReadyResponseFlag;
extern bool g_getRadioProtocolResponseFlag;
extern bool g_setRadioProtocolResponseFlag;
extern bool g_simOpenLogicalChannelResponseFlag;
extern bool g_simCloseLogicalChannelResponseFlag;
extern bool g_simTransmitApduLogicalChannelResponseFlag;
extern bool g_simTransmitApduBasicChannelResponseFlag;
extern bool g_simAuthenticationResponseFlag;
extern bool g_unlockSimLockResponseFlag;
extern bool g_getSignalStrengthResponseFlag;
extern bool g_getCsRegStatusResponseFlag;
extern bool g_getPsRegStatusResponseFlag;
extern bool g_getOperatorInfoResponseFlag;
extern bool g_getNetworkSearchInformationResponseFlag;
extern bool g_getNetworkSelectionModeResponseFlag;
extern bool g_getNeighboringCellInfoListResponseFlag;
extern bool g_getCurrentCellInfoResponseFlag;
extern bool g_getCurrentCellInfoResponse11Flag;
extern bool g_setNetworkSelectionModeResponseFlag;
extern bool g_setLocateUpdatesResponseFlag;
extern bool g_setNotificationFilterResponseFlag;
extern bool g_setDeviceStateResponseFlag;
extern bool g_sendGsmSmsResponseFlag;
extern bool g_sendCdmaSmsResponseFlag;
extern bool g_addSimMessageResponseFlag;
extern bool g_delSimMessageResponseFlag;
extern bool g_updateSimMessageResponseFlag;
extern bool g_addCdmaSimMessageResponseFlag;
extern bool g_delCdmaSimMessageResponseFlag;
extern bool g_setPreferredNetworkResponseFlag;
extern bool g_getPreferredNetworkResponseFlag;
extern bool g_getPhysicalChannelConfigResponseFlag;
extern bool g_separateConferenceResponseFlag;
extern bool g_callSupplementResponseFlag;
extern bool g_getCallWaitingResponseFlag;
extern bool g_setCallWaitingResponseFlag;
extern bool g_getUssdResponseFlag;
extern bool g_setMuteResponseFlag;
extern bool g_getMuteResponseFlag;
extern bool g_setDataPermittedResponseFlag;
extern bool g_shutDownResponseFlag;
extern bool g_setRadioStateResponseFlag;
extern bool g_getRadioStateResponseFlag;
extern bool g_getImeiResponseFlag;
extern bool g_getMeidResponseFlag;
extern bool g_getBasebandVersionResponseFlag;
extern bool g_getImsiResponseFlag;
extern bool g_setActiveSimResponseFlag;
extern bool g_setNrOptionModeResponseFlag;
extern bool g_getNrOptionModeResponseFlag;
extern bool g_dialResponseFlag;
extern bool g_setCdmaCBConfigResponseFlag;
extern bool g_getCdmaCBConfigResponseFlag;
extern bool g_sendSmsMoreModeResponseFlag;
extern bool g_sendSmsAckResponseFlag;
extern bool g_updateCdmaSimMessageResponseFlag;
extern bool g_setSmscAddrResponseFlag;
extern bool g_getSmscAddrResponseFlag;
extern bool g_setCBConfigResponseFlag;
extern bool g_getCBConfigResponseFlag;
extern bool g_signalStrengthUpdatedFlag;
extern bool g_networkPhyChnlCfgUpdatedFlag;
extern bool g_networkCurrentCellUpdatedFlag;
extern bool g_setCallTransferInfoResponseFlag;
extern bool g_getCallRestrictionResponseFlag;
extern bool g_setCallRestrictionResponseFlag;
extern bool g_getClirResponseFlag;
extern bool g_setClirResponseFlag;
extern bool g_startDtmfResponseFlag;
extern bool g_sendDtmfResponseFlag;
extern bool g_stopDtmfResponseFlag;
extern bool g_getCallPreferenceModeResponseFlag;
extern bool g_setCallPreferenceModeResponseFlag;
extern bool g_setUssdResponseFlag;
extern bool g_closeUnFinishedUssdResponseFlag;
extern bool g_getCallFailReasonResponseFlag;
extern bool g_setBarringPasswordResponseFlag;
extern bool g_deactivatePdpContextResponseFlag;
extern bool g_getPdpContextListResponseFlag;
extern bool g_setInitApnInfoResponseFlag;
extern bool g_getLinkCapabilityResponseFlag;
extern bool g_getVoiceRadioTechnologyResponseFlag;
extern bool g_getSimIOResponseFlag;
extern bool g_getSimStatusResponseFlag;
extern bool g_setSimLockResponseFlag;
extern bool g_changeSimPasswordResponseFlag;
extern bool g_unlockPinResponseFlag;
extern bool g_unlockPukResponseFlag;
extern bool g_unlockPin2ResponseFlag;
extern bool g_unlockPuk2ResponseFlag;
extern bool g_setVonrSwitchResponseFlag;
extern bool g_getSimLockStatusResponseFlag;
extern bool g_getCallTransferInfoResponseFlag;
extern bool g_combineConferenceResponseFlag;
extern bool g_getCallListResponseFlag;
extern bool g_getEmergencyCallListResponseFlag;
extern bool g_setEmergencyCallListResponse;
extern bool g_getLinkBandwidthInfoResponseFlag;
/**
** common fun
**/
void WaitFor(int32_t timeoutSecond);
bool GetBoolResult(HdiId hdiId);
int32_t GetSerialId();
bool IsReady(int32_t slotId);