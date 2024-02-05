#include "hdf_ril_callback_common.h"

OHOS::sptr<OHOS::HDI::Ril::V1_2::IRil> g_rilInterface = nullptr;
RilCallback g_callback;

std::map<int32_t, int32_t> g_simState;
int32_t g_currentChannelld = 1;
int32_t g_currentSerialId = 0;
bool g_hangupResponseFlag = false;
bool g_rejectResponseFlag = false;
bool g_answerResponseFlag = false;
bool g_holdCallResponseFlag = false;
bool g_unHoldCallResponseFlag = false;
bool g_switchCallResponseFlag = false;
bool g_getClipResponseFlag = false;
bool g_setClipResponseFlag = false;
bool g_simStkSendTerminalResponseResponseFlag = false;
bool g_simStkSendEnvelopeResponseFlag = false;
bool g_simStkSendCallSetupRequestResultResponseFlag = false;
bool g_simStkIsReadyResponseFlag = false;
bool g_getRadioProtocolResponseFlag = false;
bool g_setRadioProtocolResponseFlag = false;
bool g_simOpenLogicalChannelResponseFlag = false;
bool g_simCloseLogicalChannelResponseFlag = false;
bool g_simTransmitApduLogicalChannelResponseFlag = false;
bool g_simTransmitApduBasicChannelResponseFlag = false;
bool g_simAuthenticationResponseFlag = false;
bool g_unlockSimLockResponseFlag = false;
bool g_getSignalStrengthResponseFlag = false;
bool g_getCsRegStatusResponseFlag = false;
bool g_getPsRegStatusResponseFlag = false;
bool g_getOperatorInfoResponseFlag = false;
bool g_getNetworkSearchInformationResponseFlag = false;
bool g_getNetworkSelectionModeResponseFlag = false;
bool g_getNeighboringCellInfoListResponseFlag = false;
bool g_getCurrentCellInfoResponseFlag = false;
bool g_getCurrentCellInfoResponse11Flag = false;
bool g_setNetworkSelectionModeResponseFlag = false;
bool g_setLocateUpdatesResponseFlag = false;
bool g_setNotificationFilterResponseFlag = false;
bool g_setDeviceStateResponseFlag = false;
bool g_sendGsmSmsResponseFlag = false;
bool g_sendCdmaSmsResponseFlag = false;
bool g_addSimMessageResponseFlag = false;
bool g_delSimMessageResponseFlag = false;
bool g_updateSimMessageResponseFlag = false;
bool g_addCdmaSimMessageResponseFlag = false;
bool g_delCdmaSimMessageResponseFlag = false;
bool g_setPreferredNetworkResponseFlag = false;
bool g_getPreferredNetworkResponseFlag = false;
bool g_getPhysicalChannelConfigResponseFlag = false;
bool g_separateConferenceResponseFlag = false;
bool g_callSupplementResponseFlag = false;
bool g_getCallWaitingResponseFlag = false;
bool g_setCallWaitingResponseFlag = false;
bool g_getUssdResponseFlag = false;
bool g_setMuteResponseFlag = false;
bool g_getMuteResponseFlag = false;
bool g_setDataPermittedResponseFlag = false;
bool g_shutDownResponseFlag = false;
bool g_setRadioStateResponseFlag = false;
bool g_getRadioStateResponseFlag = false;
bool g_getImeiResponseFlag = false;
bool g_getMeidResponseFlag = false;
bool g_getBasebandVersionResponseFlag = false;
bool g_getImsiResponseFlag = false;
bool g_setActiveSimResponseFlag = false;
bool g_setNrOptionModeResponseFlag = false;
bool g_getNrOptionModeResponseFlag = false;
bool g_dialResponseFlag = false;
bool g_setCdmaCBConfigResponseFlag = false;
bool g_getCdmaCBConfigResponseFlag = false;
bool g_sendSmsMoreModeResponseFlag = false;
bool g_sendSmsAckResponseFlag = false;
bool g_updateCdmaSimMessageResponseFlag = false;
bool g_setSmscAddrResponseFlag = false;
bool g_getSmscAddrResponseFlag = false;
bool g_setCBConfigResponseFlag = false;
bool g_getCBConfigResponseFlag = false;
bool g_signalStrengthUpdatedFlag = false;
bool g_networkPhyChnlCfgUpdatedFlag = false;
bool g_networkCurrentCellUpdatedFlag = false;
bool g_setCallTransferInfoResponseFlag = false;
bool g_getCallRestrictionResponseFlag = false;
bool g_setCallRestrictionResponseFlag = false;
bool g_getClirResponseFlag = false;
bool g_setClirResponseFlag = false;
bool g_startDtmfResponseFlag = false;
bool g_sendDtmfResponseFlag = false;
bool g_stopDtmfResponseFlag = false;
bool g_getCallPreferenceModeResponseFlag = false;
bool g_setCallPreferenceModeResponseFlag = false;
bool g_setUssdResponseFlag = false;
bool g_closeUnFinishedUssdResponseFlag = false;
bool g_getCallFailReasonResponseFlag = false;
bool g_setBarringPasswordResponseFlag = false;
bool g_deactivatePdpContextResponseFlag = false;
bool g_getPdpContextListResponseFlag = false;
bool g_setInitApnInfoResponseFlag = false;
bool g_getLinkBandwidthInfoResponseFlag = false;
bool g_getLinkCapabilityResponseFlag = false;
bool g_getVoiceRadioTechnologyResponseFlag = false;
bool g_getSimIOResponseFlag = false;
bool g_getSimStatusResponseFlag = false;
bool g_setSimLockResponseFlag = false;
bool g_changeSimPasswordResponseFlag = false;
bool g_unlockPinResponseFlag = false;
bool g_unlockPukResponseFlag = false;
bool g_unlockPin2ResponseFlag = false;
bool g_unlockPuk2ResponseFlag = false;
bool g_setVonrSwitchResponseFlag = false;
bool g_getSimLockStatusResponseFlag = false;
bool g_getCallTransferInfoResponseFlag = false;
bool g_combineConferenceResponseFlag = false;
bool g_getCallListResponseFlag = false;
bool g_getEmergencyCallListResponseFlag = false;
bool g_setEmergencyCallListResponse = false;
/**
** RilCallback implement
**/
void RilCallback::NotifyAll()
{
    std::unique_lock<std::mutex> callbackLock(callbackMutex_);
    if (resultInfo_.serial != g_currentSerialId) {
        hdiId_ = HdiId::HREQ_NONE;
        HDF_LOGI("NotifyAll g_currentSerialId : %{public}d, serial: %{public}d not equal", g_currentSerialId,
                 resultInfo_.serial);
        return;
    }
    cv_.notify_all();
}

void RilCallback::WaitFor(int32_t timeoutSecond)
{
    Clean();
    std::unique_lock<std::mutex> callbackLock(callbackMutex_);
    cv_.wait_for(callbackLock, std::chrono::seconds(timeoutSecond));
}

void RilCallback::Clean() { hdiId_ = HdiId::HREQ_NONE; }

bool RilCallback::GetBoolResult(HdiId hdiId)
{
    HDF_LOGI("GetBoolResult hdiId: %{public}d, error: %{public}d", hdiId, static_cast<int32_t>(resultInfo_.error));
    bool ret = false;
    if (hdiId_ == HdiId::HREQ_NONE) {
        HDF_LOGE("response timeout, not implemented."
                 "hdiId: %d, current hdiId_: %{public}d",
                 static_cast<int32_t>(hdiId), hdiId_);
        ret = true;
        Clean();
        return ret;
    }
    if (hdiId_ != hdiId) {
        ret = false;
        HDF_LOGE("GetBoolResult hdiId does not match. hdiId: %{public}d, current hdiId: %{public}d",
                 static_cast<int32_t>(hdiId), hdiId_);
        Clean();
        return ret;
    }
    ret = true;
    Clean();
    return ret;
}

int32_t RilCallback::SimStateUpdated(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("SimStateUpdated notice : slotId = %{public}d", responseInfo.slotId);
    g_rilInterface->GetSimStatus(GetSerialId(), responseInfo.slotId);
    return 0;
}

int32_t RilCallback::SimStkSessionEndNotify(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("SimStkSessionEndNotify notice : slotId = %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallback::SimStkProactiveNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    HDF_LOGI("SimStkProactiveNotify notice : slotId = %{public}d, response = %{public}s", responseInfo.slotId,
             response.c_str());
    return 0;
}

int32_t RilCallback::SimStkAlphaNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    HDF_LOGI("SimStkAlphaNotify notice : slotId = %{public}d, response = %{public}s", responseInfo.slotId,
             response.c_str());
    return 0;
}

int32_t RilCallback::SimStkEventNotify(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    HDF_LOGI("SimStkEventNotify notice : slotId = %{public}d, response = %{public}s", responseInfo.slotId,
             response.c_str());
    return 0;
}

int32_t RilCallback::SimStkCallSetupNotify(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("SimStkCallSetupNotify notice : slotId = %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallback::SimRefreshNotify(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("SimRefreshNotify notice : slotId = %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallback::GetSimStatusResponse(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                          const OHOS::HDI::Ril::V1_1::CardStatusInfo &result)
{
    g_getSimStatusResponseFlag = true;
    HDF_LOGI("GetBoolResult GetSimStatus result : slotId = %{public}d, simType = %{public}d, g_simState = %{public}d",
             responseInfo.slotId, result.simType, result.simState);
    g_simState[responseInfo.slotId] = result.simState;
    HDF_LOGI("IsReady %{public}d %{public}d", responseInfo.slotId, g_simState[responseInfo.slotId]);
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetSimIOResponse(const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    g_getSimIOResponseFlag = true;
    HDF_LOGI("GetBoolResult GetSimIO result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s", result.sw1,
             result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_IO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetImsiResponse(const RilRadioResponseInfo &responseInfo, const std::string &response)
{
    HDF_LOGI("GetBoolResult GetImsi result : response = %{public}s", response.c_str());
    hdiId_ = HdiId::HREQ_SIM_GET_IMSI;
    resultInfo_ = responseInfo;
    g_getImsiResponseFlag = true;
    NotifyAll();
    return 0;
}
int32_t RilCallback::GetSimLockStatusResponse(const RilRadioResponseInfo &responseInfo, int32_t simLockStatus)
{
    HDF_LOGI("GetBoolResult GetSimLockStatus result : simLockStatus = %{public}d", simLockStatus);
    hdiId_ = HdiId::HREQ_SIM_GET_SIM_LOCK_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::SetSimLockResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    g_setSimLockResponseFlag = true;
    HDF_LOGI("GetBoolResult SetSimLock result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_SET_SIM_LOCK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::ChangeSimPasswordResponse(const RilRadioResponseInfo &responseInfo,
                                               const LockStatusResp &lockStatus)
{
    g_changeSimPasswordResponseFlag = true;
    HDF_LOGI("GetBoolResult ChangeSimPassword result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_CHANGE_SIM_PASSWORD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPinResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    g_unlockPinResponseFlag = true;
    HDF_LOGI("GetBoolResult UnlockPin result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PIN;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPukResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    g_unlockPukResponseFlag = true;
    HDF_LOGI("GetBoolResult UnlockPuk result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PUK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPin2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    g_unlockPin2ResponseFlag = true;
    HDF_LOGI("GetBoolResult UnlockPin2 result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PIN2;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UnlockPuk2Response(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    g_unlockPuk2ResponseFlag = true;
    HDF_LOGI("GetBoolResult UnlockPuk2 result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_PUK2;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetActiveSimResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetActiveSim result");
    hdiId_ = HdiId::HREQ_SIM_SET_ACTIVE_SIM;
    resultInfo_ = responseInfo;
    g_setActiveSimResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkSendTerminalResponseResponse(const RilRadioResponseInfo &responseInfo)
{
    g_simStkSendTerminalResponseResponseFlag = true;
    HDF_LOGI("GetBoolResult SimStkSendTerminalResponse result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_TERMINAL_RESPONSE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkSendEnvelopeResponse(const RilRadioResponseInfo &responseInfo)
{
    g_simStkSendEnvelopeResponseFlag = true;
    HDF_LOGI("GetBoolResult SimStkSendEnvelope result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_ENVELOPE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkSendCallSetupRequestResultResponse(const RilRadioResponseInfo &responseInfo)
{
    g_simStkSendCallSetupRequestResultResponseFlag = true;
    HDF_LOGI("GetBoolResult SimStkSendCallSetupRequestResult result");
    hdiId_ = HdiId::HREQ_SIM_STK_SEND_CALL_SETUP_REQUEST_RESULT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimStkIsReadyResponse(const RilRadioResponseInfo &responseInfo)
{
    g_simStkIsReadyResponseFlag = true;
    HDF_LOGI("GetBoolResult SimStkIsReady result");
    hdiId_ = HdiId::HREQ_SIM_STK_IS_READY;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetRadioProtocolResponse(const RilRadioResponseInfo &responseInfo,
                                              const RadioProtocol &radioProtocol)
{
    g_setRadioProtocolResponseFlag = true;
    HDF_LOGI("GetBoolResult SetRadioProtocol result : phase = %{public}d, slotId = %{public}d", radioProtocol.phase,
             radioProtocol.slotId);
    hdiId_ = HdiId::HREQ_SIM_RADIO_PROTOCOL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetRadioProtocolResponse(const RilRadioResponseInfo &responseInfo,
                                              const RadioProtocol &radioProtocol)
{
    g_getRadioProtocolResponseFlag = true;
    return 0;
}

int32_t RilCallback::SimOpenLogicalChannelResponse(const RilRadioResponseInfo &responseInfo,
                                                   const OpenLogicalChannelResponse &pOpenLogicalChannelResponse)
{
    g_simOpenLogicalChannelResponseFlag = true;
    HDF_LOGI("GetBoolResult SimOpenLogicalChannel result : sw1 = %{public}d, sw2 = %{public}d, channelId = %{public}d, "
             "response = %{public}s",
             pOpenLogicalChannelResponse.sw1, pOpenLogicalChannelResponse.sw2, pOpenLogicalChannelResponse.channelId,
             pOpenLogicalChannelResponse.response.c_str());
    g_currentChannelld = pOpenLogicalChannelResponse.channelId;
    hdiId_ = HdiId::HREQ_SIM_OPEN_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimCloseLogicalChannelResponse(const RilRadioResponseInfo &responseInfo)
{
    g_simCloseLogicalChannelResponseFlag = true;
    HDF_LOGI("GetBoolResult SimCloseLogicalChannel result");
    hdiId_ = HdiId::HREQ_SIM_CLOSE_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimTransmitApduLogicalChannelResponse(const RilRadioResponseInfo &responseInfo,
                                                           const IccIoResultInfo &result)
{
    g_simTransmitApduLogicalChannelResponseFlag = true;
    HDF_LOGI("GetBoolResult SimTransmitApduLogicalChannel result : sw1 = %{public}d, sw2 = %{public}d, response = "
             "%{public}s",
             result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_TRANSMIT_APDU_LOGICAL_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SimTransmitApduBasicChannelResponse(const RilRadioResponseInfo &responseInfo,
                                                         const IccIoResultInfo &result)
{
    g_simTransmitApduBasicChannelResponseFlag = true;
    HDF_LOGI(
        "GetBoolResult SimTransmitApduBasicChannel result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
        result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_TRANSMIT_APDU_BASIC_CHANNEL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::SimAuthenticationResponse(const RilRadioResponseInfo &responseInfo, const IccIoResultInfo &result)
{
    g_simAuthenticationResponseFlag = true;
    HDF_LOGI("GetBoolResult SimAuthentication result : sw1 = %{public}d, sw2 = %{public}d, response = %{public}s",
             result.sw1, result.sw2, result.response.c_str());
    hdiId_ = HdiId::HREQ_SIM_AUTHENTICATION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::UnlockSimLockResponse(const RilRadioResponseInfo &responseInfo, const LockStatusResp &lockStatus)
{
    g_unlockSimLockResponseFlag = true;
    HDF_LOGI("GetBoolResult UnlockSimLock result : result = %{public}d, remain = %{public}d", lockStatus.result,
             lockStatus.remain);
    hdiId_ = HdiId::HREQ_SIM_UNLOCK_SIM_LOCK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendSimMatchedOperatorInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SendSimMatchedOperatorInfo result");
    hdiId_ = HdiId::HREQ_SIM_SEND_NCFG_OPER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::NetworkCsRegStatusUpdated(const RilRadioResponseInfo &responseInfo,
                                               const CsRegStatusInfo &csRegStatusInfo)
{
    HDF_LOGI("RilCallback::NetworkCsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, "
             "lacCode:%{public}d, cellId:%{public}d, radioTechnology:%{public}d",
             csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
             csRegStatusInfo.radioTechnology);
    return 0;
}

int32_t RilCallback::NetworkPsRegStatusUpdated(const RilRadioResponseInfo &responseInfo,
                                               const PsRegStatusInfo &psRegStatusInfo)
{
    HDF_LOGI(
        "RilCallback::NetworkPsRegStatusUpdated notifyType:%{public}d, regStatus:%{public}d, lacCode:%{public}d, "
        "cellId:%{public}d, tech:%{public}d, nrRestricted:%{public}d, nrAvailable:%{public}d, enDcAvailable:%{public}d",
        psRegStatusInfo.notifyType, psRegStatusInfo.regStatus, psRegStatusInfo.lacCode, psRegStatusInfo.cellId,
        psRegStatusInfo.radioTechnology, psRegStatusInfo.isDcNrRestricted, psRegStatusInfo.isNrAvailable,
        psRegStatusInfo.isEnDcAvailable);
    return 0;
}

int32_t RilCallback::SignalStrengthUpdated(const RilRadioResponseInfo &responseInfo, const Rssi &rssi)
{
    g_signalStrengthUpdatedFlag = true;
    HDF_LOGI("RilCallback::SignalStrengthUpdated rxlev:%{public}d rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
    return 0;
}

int32_t RilCallback::NetworkTimeZoneUpdated(const RilRadioResponseInfo &responseInfo, const std::string &timeZoneStr)
{
    HDF_LOGI("RilCallback::NetworkTimeZoneUpdated timeZone:%{public}s", timeZoneStr.c_str());
    return 0;
}

int32_t RilCallback::NetworkTimeUpdated(const RilRadioResponseInfo &responseInfo, const std::string &timeStr)
{
    HDF_LOGI("RilCallback::NetworkTimeZoneUpdated time:%{public}s", timeStr.c_str());
    return 0;
}

int32_t RilCallback::NetworkPhyChnlCfgUpdated(const RilRadioResponseInfo &responseInfo,
                                              const ChannelConfigInfoList &channelConfigInfoList)
{
    g_networkPhyChnlCfgUpdatedFlag = true;
    HDF_LOGI("RilCallback::NetworkPhyChnlCfgUpdated itemNum:%{public}d", channelConfigInfoList.itemNum);
    for (PhysicalChannelConfig phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
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

int32_t RilCallback::NetworkCurrentCellUpdated(const RilRadioResponseInfo &responseInfo,
                                               const CellListCurrentInfo &cellListCurrentInfo)
{
    g_networkCurrentCellUpdatedFlag = true;
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

int32_t RilCallback::NetworkCurrentCellUpdated_1_1(const RilRadioResponseInfo &responseInfo,
                                                   const CellListCurrentInfo_1_1 &cellListCurrentInfo)
{
    HDF_LOGI("RilCallback::NetworkCurrentCellUpdated_1_1 itemNum:%{public}d", cellListCurrentInfo.itemNum);
    for (auto info : cellListCurrentInfo.cellCurrentInfo) {
        HDF_LOGI("RilCallback::NetworkCurrentCellUpdated_1_1 ratType:%{public}d, mcc:%{public}d, mnc:%{public}d",
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
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.nr.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.nr.rsrq);
                break;
            default:
                HDF_LOGE("RilCallback::NetworkCurrentCellUpdated_1_1 invalid ratType");
        }
    }
    return 0;
}

int32_t RilCallback::NetworkCurrentCellUpdated_1_2(const RilRadioResponseInfo &responseInfo,
                                                   const CellListCurrentInfo_1_2 &cellListCurrentInfo)
{
    HDF_LOGI("itemNum:%{public}d", cellListCurrentInfo.itemNum);
    for (auto info : cellListCurrentInfo.cellCurrentInfo) {
        HDF_LOGI("ratType:%{public}d, mcc:%{public}d, mnc:%{public}d", info.ratType, info.mcc, info.mnc);
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
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.nr.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.nr.rsrq);
                break;
            default:
                HDF_LOGE("invalid ratType");
                break;
        }
    }
    return 0;
}

int32_t RilCallback::ResidentNetworkUpdated(const RilRadioResponseInfo &responseInfo, const std::string &plmn)
{
    HDF_LOGI("RilCallback::ResidentNetworkUpdated plmn:%{public}s", plmn.c_str());
    return 0;
}

int32_t RilCallback::GetSignalStrengthResponse(const RilRadioResponseInfo &responseInfo, const Rssi &rssi)
{
    g_getSignalStrengthResponseFlag = true;
    HDF_LOGI("RilCallback::GetSignalStrengthResponse rxlev:%{public}d rsrp:%{public}d", rssi.lte.rxlev, rssi.lte.rsrp);
    hdiId_ = HdiId::HREQ_NETWORK_GET_SIGNAL_STRENGTH;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCsRegStatusResponse(const RilRadioResponseInfo &responseInfo,
                                            const CsRegStatusInfo &csRegStatusInfo)
{
    g_getCsRegStatusResponseFlag = true;
    HDF_LOGI("RilCallback::GetCsRegStatusResponse notifyType:%{public}d, regStatus:%{public}d, "
             "lacCode:%{public}d, cellId:%{public}d, radioTechnology:%{public}d",
             csRegStatusInfo.notifyType, csRegStatusInfo.regStatus, csRegStatusInfo.lacCode, csRegStatusInfo.cellId,
             csRegStatusInfo.radioTechnology);
    hdiId_ = HdiId::HREQ_NETWORK_GET_CS_REG_STATUS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPsRegStatusResponse(const RilRadioResponseInfo &responseInfo,
                                            const PsRegStatusInfo &psRegStatusInfo)
{
    g_getPsRegStatusResponseFlag = true;
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

int32_t RilCallback::GetOperatorInfoResponse(const RilRadioResponseInfo &responseInfo, const OperatorInfo &operatorInfo)
{
    g_getOperatorInfoResponseFlag = true;
    HDF_LOGI("RilCallback::GetOperatorInfoResponse longName:%{public}s, shortName:%{public}s, numeric:%{public}s",
             operatorInfo.longName.c_str(), operatorInfo.shortName.c_str(), operatorInfo.numeric.c_str());
    hdiId_ = HdiId::HREQ_NETWORK_GET_OPERATOR_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetNetworkSearchInformationResponse(const RilRadioResponseInfo &responseInfo,
                                                         const AvailableNetworkList &availableNetworkList)
{
    g_getNetworkSearchInformationResponseFlag = true;
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

int32_t RilCallback::GetNetworkSelectionModeResponse(const RilRadioResponseInfo &responseInfo,
                                                     const SetNetworkModeInfo &setNetworkModeInfo)
{
    g_getNetworkSelectionModeResponseFlag = true;
    HDF_LOGI("RilCallback::GetNetworkSelectionModeResponse selectMode:%{public}d", setNetworkModeInfo.selectMode);
    hdiId_ = HdiId::HREQ_NETWORK_GET_NETWORK_SELECTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetNetworkSelectionModeResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setNetworkSelectionModeResponseFlag = true;
    HDF_LOGI("RilCallback::SetNetworkSelectionModeResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_NETWORK_SELECTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetNeighboringCellInfoListResponse(const RilRadioResponseInfo &responseInfo,
                                                        const CellListNearbyInfo &cellInfoList)
{
    g_getNeighboringCellInfoListResponseFlag = true;
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

int32_t RilCallback::GetNeighboringCellInfoListResponse_1_2(const RilRadioResponseInfo &responseInfo,
                                                            const CellListNearbyInfo_1_2 &cellInfoList)
{
    HDF_LOGI("itemNum:%{public}d", cellInfoList.itemNum);
    for (auto info : cellInfoList.cellNearbyInfo) {
        HDF_LOGI("ratType:%{public}d", info.ratType);
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
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.nr.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.nr.rsrq);
                break;
            default:
                HDF_LOGE("invalid ratType");
                break;
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_NEIGHBORING_CELLINFO_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCurrentCellInfoResponse(const RilRadioResponseInfo &responseInfo,
                                                const CellListCurrentInfo &cellInfoList)
{
    g_getCurrentCellInfoResponseFlag = true;
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

int32_t RilCallback::GetCurrentCellInfoResponse_1_1(const RilRadioResponseInfo &responseInfo,
                                                    const CellListCurrentInfo_1_1 &cellListCurrentInfo)
{
    g_getCurrentCellInfoResponse11Flag = true;
    HDF_LOGI("RilCallback::GetCurrentCellInfoResponse_1_1 itemNum:%{public}d", cellListCurrentInfo.itemNum);
    for (auto info : cellListCurrentInfo.cellCurrentInfo) {
        HDF_LOGI("RilCallback::GetCurrentCellInfoResponse_1_1 ratType:%{public}d, mcc:%{public}d, mnc:%{public}d",
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
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.nr.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.nr.rsrq);
                break;
            default:
                HDF_LOGE("RilCallback::GetCurrentCellInfoResponse_1_1 invalid ratType");
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCurrentCellInfoResponse_1_2(const RilRadioResponseInfo &responseInfo,
                                                    const CellListCurrentInfo_1_2 &cellListCurrentInfo)
{
    HDF_LOGI("itemNum:%{public}d", cellListCurrentInfo.itemNum);
    for (auto info : cellListCurrentInfo.cellCurrentInfo) {
        HDF_LOGI("ratType:%{public}d, mcc:%{public}d, mnc:%{public}d", info.ratType, info.mcc, info.mnc);
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
                HDF_LOGI("rsrp:%{public}d", info.serviceCells.nr.rsrp);
                HDF_LOGI("rsrq:%{public}d", info.serviceCells.nr.rsrq);
                break;
            default:
                HDF_LOGE("invalid ratType");
                break;
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_CURRENT_CELL_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetPreferredNetworkResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetPreferredNetworkResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_PREFERRED_NETWORK;
    resultInfo_ = responseInfo;
    g_setPreferredNetworkResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPreferredNetworkResponse(const RilRadioResponseInfo &responseInfo,
                                                 const PreferredNetworkTypeInfo &preferredNetworkTypeInfo)
{
    HDF_LOGI("RilCallback::GetPreferredNetworkResponse type:%{public}d", preferredNetworkTypeInfo.preferredNetworkType);
    hdiId_ = HdiId::HREQ_NETWORK_GET_PREFERRED_NETWORK;
    resultInfo_ = responseInfo;
    g_getPreferredNetworkResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPhysicalChannelConfigResponse(const RilRadioResponseInfo &responseInfo,
                                                      const ChannelConfigInfoList &channelConfigInfoList)
{
    HDF_LOGI("RilCallback::GetPhysicalChannelConfigResponse itemNum:%{public}d", channelConfigInfoList.itemNum);
    for (PhysicalChannelConfig phyChnlCfg : channelConfigInfoList.channelConfigInfos) {
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
    g_getPhysicalChannelConfigResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetLocateUpdatesResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setLocateUpdatesResponseFlag = true;
    HDF_LOGI("RilCallback::SetLocateUpdatesResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_LOCATE_UPDATES;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetNotificationFilterResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setNotificationFilterResponseFlag = true;
    HDF_LOGI("RilCallback::SetNotificationFilterResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_NOTIFICATION_FILTER;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetDeviceStateResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setDeviceStateResponseFlag = true;
    HDF_LOGI("RilCallback::SetDeviceStateResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_NETWORK_SET_DEVICE_STATE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetRrcConnectionStateResponse(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    HDF_LOGI("RilCallback::GetRrcConnectionStateResponse state:%{public}d", state);
    hdiId_ = HdiId::HREQ_NETWORK_GET_RRC_CONNECTION_STATE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetNrOptionModeResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetDeviceStateResponse error:%{public}d", responseInfo.error);
    g_setNrOptionModeResponseFlag = true;
    hdiId_ = HdiId::HREQ_NETWORK_SET_NR_OPTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetNrOptionModeResponse(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    HDF_LOGI("RilCallback::GetNrOptionModeResponse state:%{public}d", state);
    g_getNrOptionModeResponseFlag = true;
    hdiId_ = HdiId::HREQ_NETWORK_GET_NR_OPTION_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetRrcConnectionStateUpdated(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    HDF_LOGI("RilCallback::GetRrcConnectionStateUpdated state:%{public}d", state);
    return 0;
}

int32_t RilCallback::GetNrSsbIdResponse(const RilRadioResponseInfo &responseInfo, const NrCellSsbIds &nrCellSsbIds)
{
    int32_t ssbListNum = 0;
    int32_t nbCellNum = 0;
    HDF_LOGE("rsrp:%{public}d", nrCellSsbIds.rsrp);
    HDF_LOGE("sinr:%{public}d", nrCellSsbIds.sinr);
    for (auto info : nrCellSsbIds.sCellSsbList) {
        ssbListNum = ssbListNum + 1;
        HDF_LOGE("sCellSsbNum:%{public}d, rsrp:%{public}d", ssbListNum, info.rsrp);
    }
    HDF_LOGE("nbCellCount:%{public}d", nrCellSsbIds.nbCellCount);
    for (auto info : nrCellSsbIds.nbCellSsbList) {
        nbCellNum = nbCellNum + 1;
        HDF_LOGE("nbCellNum:%{public}d, rsrp:%{public}d, sinr:%{public}d", nbCellNum, info.rsrp, info.sinr);
        ssbListNum = 0;
        for (auto infoNbCell : info.ssbIdList) {
            ssbListNum = ssbListNum + 1;
            HDF_LOGE("nbCellSsbNum:%{public}d, rsrp:%{public}d", ssbListNum, infoNbCell.rsrp);
        }
    }
    hdiId_ = HdiId::HREQ_NETWORK_GET_NR_SSBID_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CallEmergencyNotice(const RilRadioResponseInfo &responseInfo,
                                         const EmergencyInfoList &emergencyInfoList)
{
    HDF_LOGI("GetBoolResult CallEmergencyNotice callSize : %{public}d", emergencyInfoList.callSize);
    return 0;
}

int32_t RilCallback::CallStateUpdated(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult CallStateUpdated slotId : %{public}d, type: %{public}d", responseInfo.slotId,
             responseInfo.type);
    return 0;
}

int32_t RilCallback::CallRingbackVoiceNotice(const RilRadioResponseInfo &responseInfo,
                                             const RingbackVoice &ringbackVoice)
{
    HDF_LOGI("GetBoolResult CallRingbackVoiceNotice slotId : %{public}d, ringbackVoice: %{public}d",
             responseInfo.slotId, ringbackVoice.status);
    return 0;
}

int32_t RilCallback::CallSrvccStatusNotice(const RilRadioResponseInfo &responseInfo, const SrvccStatus &srvccStatus)
{
    HDF_LOGI("GetBoolResult CallSrvccStatusNotice slotId : %{public}d, srvccStatus: %{public}d", responseInfo.slotId,
             srvccStatus.status);
    return 0;
}

int32_t RilCallback::CallUssdNotice(const RilRadioResponseInfo &responseInfo, const UssdNoticeInfo &ussdNoticeInfo)
{
    HDF_LOGI("GetBoolResult CallUssdNotice slotId : %{public}d, ussdNoticeInfo m: %{public}d, str: %{public}s",
             responseInfo.slotId, ussdNoticeInfo.type, ussdNoticeInfo.message.c_str());
    return 0;
}

int32_t RilCallback::CallSsNotice(const RilRadioResponseInfo &responseInfo, const SsNoticeInfo &ssNoticeInfo)
{
    HDF_LOGI("GetBoolResult CallSsNotice slotId : %{public}d, result: %{public}d, serviceType: %{public}d,"
             "requestType: %{public}d, serviceClass: %{public}d",
             responseInfo.slotId, ssNoticeInfo.result, ssNoticeInfo.serviceType, ssNoticeInfo.requestType,
             ssNoticeInfo.serviceClass);
    return 0;
}

int32_t RilCallback::CallRsrvccStatusNotify(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult CallRsrvccStatusNotify slotId : %{public}d", responseInfo.slotId);
    return 0;
}

int32_t RilCallback::GetEmergencyCallListResponse(const RilRadioResponseInfo &responseInfo,
                                                  const EmergencyInfoList &emergencyInfoList)
{
    HDF_LOGI("GetBoolResult emergencyInfoList callSize : %{public}d", emergencyInfoList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_EMERGENCY_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetEmergencyCallListResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetEmergencyCallListResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_EMERGENCY_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallListResponse(const RilRadioResponseInfo &responseInfo, const CallInfoList &callList)
{
    HDF_LOGI("GetBoolResult CallInfoList callSize : %{public}d", callList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_LIST;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DialResponse(const RilRadioResponseInfo &responseInfo)
{
    g_dialResponseFlag = true;
    HDF_LOGI("GetBoolResult DialResponse");
    hdiId_ = HdiId::HREQ_CALL_DIAL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::HangupResponse(const RilRadioResponseInfo &responseInfo)
{
    g_hangupResponseFlag = true;
    HDF_LOGI("GetBoolResult HangupResponse");
    hdiId_ = HdiId::HREQ_CALL_HANGUP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::RejectResponse(const RilRadioResponseInfo &responseInfo)
{
    g_rejectResponseFlag = true;
    HDF_LOGI("GetBoolResult RejectResponse");
    hdiId_ = HdiId::HREQ_CALL_REJECT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::AnswerResponse(const RilRadioResponseInfo &responseInfo)
{
    g_answerResponseFlag = true;
    HDF_LOGI("GetBoolResult AnswerResponse");
    hdiId_ = HdiId::HREQ_CALL_ANSWER;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::HoldCallResponse(const RilRadioResponseInfo &responseInfo)
{
    g_holdCallResponseFlag = true;
    HDF_LOGI("GetBoolResult HoldCallResponse");
    hdiId_ = HdiId::HREQ_CALL_HOLD_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::UnHoldCallResponse(const RilRadioResponseInfo &responseInfo)
{
    g_unHoldCallResponseFlag = true;
    HDF_LOGI("GetBoolResult UnHoldCallResponse");
    hdiId_ = HdiId::HREQ_CALL_UNHOLD_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SwitchCallResponse(const RilRadioResponseInfo &responseInfo)
{
    g_switchCallResponseFlag = true;
    HDF_LOGI("GetBoolResult SwitchCallResponse");
    hdiId_ = HdiId::HREQ_CALL_SWITCH_CALL;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetClipResponse(const RilRadioResponseInfo &responseInfo, const GetClipResult &getClipResult)
{
    g_getClipResponseFlag = true;
    HDF_LOGI("GetBoolResult result: %{public}d, action: %{public}d, clipStat: %{public}d", getClipResult.result,
             getClipResult.action, getClipResult.clipStat);
    hdiId_ = HdiId::HREQ_CALL_GET_CLIP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetClipResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setClipResponseFlag = true;
    HDF_LOGI("GetBoolResult SetClipResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CLIP;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CombineConferenceResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult CombineConferenceResponse");
    hdiId_ = HdiId::HREQ_CALL_COMBINE_CONFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SeparateConferenceResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SeparateConferenceResponse");
    hdiId_ = HdiId::HREQ_CALL_SEPARATE_CONFERENCE;
    resultInfo_ = responseInfo;
    g_separateConferenceResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CallSupplementResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult CallSupplementResponse");
    hdiId_ = HdiId::HREQ_CALL_CALL_SUPPLEMENT;
    resultInfo_ = responseInfo;
    g_callSupplementResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallWaitingResponse(const RilRadioResponseInfo &responseInfo,
                                            const CallWaitResult &callWaitResult)
{
    HDF_LOGI("GetBoolResult GetCallWaitingResponse result: %{public}d, status: %{public}d, classCw: %{public}d",
             callWaitResult.result, callWaitResult.status, callWaitResult.classCw);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_WAITING;
    resultInfo_ = responseInfo;
    g_getCallWaitingResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCallWaitingResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetCallWaitingResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_WAITING;
    resultInfo_ = responseInfo;
    g_setCallWaitingResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallTransferInfoResponse(const RilRadioResponseInfo &responseInfo,
                                                 const CallForwardQueryInfoList &cFQueryList)
{
    HDF_LOGI("GetBoolResult GetCallTransferInfoResponse cFQueryList: %{public}d", cFQueryList.callSize);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_TRANSFER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCallTransferInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setCallTransferInfoResponseFlag = true;
    HDF_LOGI("GetBoolResult SetCallTransferInfoResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_TRANSFER_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallRestrictionResponse(const RilRadioResponseInfo &responseInfo,
                                                const CallRestrictionResult &result)
{
    g_getCallRestrictionResponseFlag = true;
    HDF_LOGI("GetBoolResult result: %{public}d, status: %{public}d, classCw: %{public}d", result.result, result.status,
             result.classCw);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_RESTRICTION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::SetCallRestrictionResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setCallRestrictionResponseFlag = true;
    HDF_LOGI("GetBoolResult SetCallRestrictionResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_RESTRICTION;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::GetClirResponse(const RilRadioResponseInfo &responseInfo, const GetClirResult &getClirResult)
{
    g_getClirResponseFlag = true;
    HDF_LOGI("GetBoolResult result: %{public}d, action: %{public}d, clirStat: %{public}d", getClirResult.result,
             getClirResult.action, getClirResult.clirStat);
    hdiId_ = HdiId::HREQ_CALL_GET_CLIR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetClirResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setClirResponseFlag = true;
    HDF_LOGI("GetBoolResult SetClirResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CLIR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::StartDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    g_startDtmfResponseFlag = true;
    HDF_LOGI("GetBoolResult StartDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_START_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    g_sendDtmfResponseFlag = true;
    HDF_LOGI("GetBoolResult SendDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_SEND_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::StopDtmfResponse(const RilRadioResponseInfo &responseInfo)
{
    g_stopDtmfResponseFlag = true;
    HDF_LOGI("GetBoolResult StopDtmfResponse");
    hdiId_ = HdiId::HREQ_CALL_STOP_DTMF;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo, int32_t mode)
{
    g_getCallPreferenceModeResponseFlag = true;
    HDF_LOGI("GetBoolResult GetCallPreferenceModeResponse mode: %{public}d", mode);
    hdiId_ = HdiId::HREQ_CALL_GET_CALL_PREFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCallPreferenceModeResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setCallPreferenceModeResponseFlag = true;
    HDF_LOGI("GetBoolResult SetCallPreferenceModeResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_CALL_PREFERENCE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetUssdResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setUssdResponseFlag = true;
    HDF_LOGI("GetBoolResult SetUssdResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_USSD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetUssdResponse(const RilRadioResponseInfo &responseInfo, int32_t cusd)
{
    HDF_LOGI("GetBoolResult GetUssdResponse cusd: %{public}d", cusd);
    hdiId_ = HdiId::HREQ_CALL_GET_USSD;
    resultInfo_ = responseInfo;
    g_getUssdResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetMuteResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("GetBoolResult SetMuteResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_MUTE;
    resultInfo_ = responseInfo;
    g_setMuteResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetMuteResponse(const RilRadioResponseInfo &responseInfo, int32_t mute)
{
    HDF_LOGI("GetBoolResult GetMuteResponse mute: %{public}d", mute);
    hdiId_ = HdiId::HREQ_CALL_GET_MUTE;
    resultInfo_ = responseInfo;
    g_getMuteResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCallFailReasonResponse(const RilRadioResponseInfo &responseInfo, int32_t callFail)
{
    g_getCallFailReasonResponseFlag = true;
    HDF_LOGI("GetBoolResult GetCallFailReasonResponse callFail: %{public}d", callFail);
    hdiId_ = HdiId::HREQ_CALL_GET_FAIL_REASON;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetBarringPasswordResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setBarringPasswordResponseFlag = true;
    HDF_LOGI("GetBoolResult SetBarringPasswordResponse");
    hdiId_ = HdiId::HREQ_CALL_SET_BARRING_PASSWORD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CloseUnFinishedUssdResponse(const RilRadioResponseInfo &responseInfo)
{
    g_closeUnFinishedUssdResponseFlag = true;
    HDF_LOGI("GetBoolResult CloseUnFinishedUssdResponse");
    hdiId_ = HdiId::HREQ_CALL_CLOSE_UNFINISHED_USSD;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetVonrSwitchResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setVonrSwitchResponseFlag = true;
    HDF_LOGI("GetBoolResult SetVonrSwitchResponse");
    hdiId_ = HdiId::HREQ_SET_VONR_SWITCH;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

/**
 * modem
 **/
int32_t RilCallback::RadioStateUpdated(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    HDF_LOGI("RadioStateUpdated state : %{public}d", state);
    return 0;
}

int32_t RilCallback::VoiceRadioTechUpdated(const RilRadioResponseInfo &responseInfo,
                                           const VoiceRadioTechnology &voiceRadioTechnology)
{
    HDF_LOGI("GetVoiceRadioTechnologyResponse srvStatus : %{public}d srvDomain : %{public}d roamStatus: %{public}d "
             "simStatus : %{public}d lockStatus : %{public}d sysMode : %{public}d actType : %{public}d sysModeName : "
             "%{public}s actName : %{public}s",
             voiceRadioTechnology.srvStatus, voiceRadioTechnology.srvDomain, voiceRadioTechnology.roamStatus,
             voiceRadioTechnology.simStatus, voiceRadioTechnology.lockStatus, voiceRadioTechnology.sysMode,
             voiceRadioTechnology.actType, voiceRadioTechnology.sysModeName.c_str(),
             voiceRadioTechnology.actName.c_str());
    return 0;
}

int32_t RilCallback::DsdsModeUpdated(const RilRadioResponseInfo &responseInfo, int32_t mode)
{
    HDF_LOGI("DsdsModeUpdated mode : %{public}d", mode);
    return 0;
}

int32_t RilCallback::ShutDownResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("ShutDownResponse");
    hdiId_ = HdiId::HREQ_MODEM_SHUT_DOWN;
    resultInfo_ = responseInfo;
    g_shutDownResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetRadioStateResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("SetRadioStateResponse");
    hdiId_ = HdiId::HREQ_MODEM_SET_RADIO_STATUS;
    resultInfo_ = responseInfo;
    g_setRadioStateResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetRadioStateResponse(const RilRadioResponseInfo &responseInfo, int32_t state)
{
    HDF_LOGI("GetRadioStateResponse state : %{public}d", state);
    hdiId_ = HdiId::HREQ_MODEM_GET_RADIO_STATUS;
    resultInfo_ = responseInfo;
    g_getRadioStateResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetImeiResponse(const RilRadioResponseInfo &responseInfo, const std::string &imei)
{
    HDF_LOGI("GetImeiResponse imei : %{public}s", imei.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_IMEI;
    resultInfo_ = responseInfo;
    g_getImeiResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetMeidResponse(const RilRadioResponseInfo &responseInfo, const std::string &meid)
{
    HDF_LOGI("GetMeidResponse meid : %{public}s", meid.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_MEID;
    resultInfo_ = responseInfo;
    g_getMeidResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetVoiceRadioTechnologyResponse(const RilRadioResponseInfo &responseInfo,
                                                     const VoiceRadioTechnology &voiceRadioTechnology)
{
    g_getVoiceRadioTechnologyResponseFlag = true;
    HDF_LOGI("GetVoiceRadioTechnologyResponse srvStatus : %{public}d srvDomain : %{public}d roamStatus: %{public}d "
             "simStatus : %{public}d lockStatus : %{public}d sysMode : %{public}d actType : %{public}d sysModeName : "
             "%{public}s actName : %{public}s",
             voiceRadioTechnology.srvStatus, voiceRadioTechnology.srvDomain, voiceRadioTechnology.roamStatus,
             voiceRadioTechnology.simStatus, voiceRadioTechnology.lockStatus, voiceRadioTechnology.sysMode,
             voiceRadioTechnology.actType, voiceRadioTechnology.sysModeName.c_str(),
             voiceRadioTechnology.actName.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_VOICE_RADIO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetBasebandVersionResponse(const RilRadioResponseInfo &responseInfo,
                                                const std::string &basebandVersion)
{
    HDF_LOGI("GetBasebandVersionResponse basebandVersion : %{public}s", basebandVersion.c_str());
    hdiId_ = HdiId::HREQ_MODEM_GET_BASEBAND_VERSION;
    resultInfo_ = responseInfo;
    g_getBasebandVersionResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::PdpContextListUpdated(const RilRadioResponseInfo &responseInfo,
                                           const DataCallResultList &dataCallResultList)
{
    HDF_LOGI("RilCallback::PdpContextListUpdated size:%{public}d", dataCallResultList.size);
    for (auto &setupDataCallResultInfo : dataCallResultList.dcList) {
        HDF_LOGI(
            "RilCallback::PdpContextListUpdated flag:%{public}d reason:%{public}d retryTime:%{public}d "
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
    return 0;
}

int32_t RilCallback::DataLinkCapabilityUpdated(const RilRadioResponseInfo &responseInfo,
                                               const DataLinkCapability &dataLinkCapability)
{
    HDF_LOGI("RilCallback::DataLinkCapabilityUpdated primaryDownlinkKbps:%{public}d primaryUplinkKbps:%{public}d "
             "secondaryDownlinkKbps:%{public}d secondaryUplinkKbps:%{public}d",
             dataLinkCapability.primaryDownlinkKbps, dataLinkCapability.primaryUplinkKbps,
             dataLinkCapability.secondaryDownlinkKbps, dataLinkCapability.secondaryUplinkKbps);
    return 0;
}

int32_t RilCallback::ActivatePdpContextResponse(const RilRadioResponseInfo &responseInfo,
                                                const SetupDataCallResultInfo &setupDataCallResultInfo)
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

int32_t RilCallback::DeactivatePdpContextResponse(const RilRadioResponseInfo &responseInfo)
{
    g_deactivatePdpContextResponseFlag = true;
    HDF_LOGI("RilCallback::DeactivatePdpContextResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_DEACTIVATE_PDP_CONTEXT;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetPdpContextListResponse(const RilRadioResponseInfo &responseInfo,
                                               const DataCallResultList &dataCallResultList)
{
    g_getPdpContextListResponseFlag = true;
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

int32_t RilCallback::SetInitApnInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setInitApnInfoResponseFlag = true;
    HDF_LOGI("RilCallback::SetInitApnInfoResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_INIT_APN_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetLinkBandwidthReportingRuleResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetLinkBandwidthReportingRuleResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_LINK_BANDWIDTH_REPORTING_RULE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetLinkBandwidthInfoResponse(const RilRadioResponseInfo &responseInfo,
                                                  const DataLinkBandwidthInfo &dataLinkBandwidthInfo)
{
    g_getLinkBandwidthInfoResponseFlag = true;
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

int32_t RilCallback::SetDataPermittedResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetDataPermittedResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_DATA_PERMITTED;
    resultInfo_ = responseInfo;
    g_setDataPermittedResponseFlag = true;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetDataProfileInfoResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetDataProfileInfoResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_SET_DATA_PROFILE_INFO;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetLinkCapabilityResponse(const RilRadioResponseInfo &responseInfo,
                                               const DataLinkCapability &dataLinkCapability)
{
    g_getLinkCapabilityResponseFlag = true;
    HDF_LOGI("RilCallbackTest::GetLinkCapabilityResponse primaryDownlinkKbps:%{public}d "
             "primaryUplinkKbps:%{public}d secondaryDownlinkKbps:%{public}d secondaryUplinkKbps:%{public}d",
             dataLinkCapability.primaryDownlinkKbps, dataLinkCapability.primaryUplinkKbps,
             dataLinkCapability.secondaryDownlinkKbps, dataLinkCapability.secondaryUplinkKbps);
    hdiId_ = HdiId::HREQ_DATA_GET_LINK_CAPABILITY;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CleanAllConnectionsResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::CleanAllConnectionsResponse error:%{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_DATA_CLEAN_ALL_CONNECTIONS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::NewSmsNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                  const SmsMessageInfo &smsMessageInfo)
{
    HDF_LOGI("RilCallback::NewSmsNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallback::NewCdmaSmsNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                      const SmsMessageInfo &smsMessageInfo)
{
    HDF_LOGI("RilCallback::NewCdmaSmsNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallback::SmsStatusReportNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                           const SmsMessageInfo &smsMessageInfo)
{
    HDF_LOGI("RilCallback::SmsStatusReportNotify smsMessageInfo size : %{public}d", smsMessageInfo.size);
    return 0;
}

int32_t RilCallback::NewSmsStoredOnSimNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                             int32_t recordNumber, int32_t indicationType)
{
    HDF_LOGI("RilCallback::NewSmsStoredOnSimNotify recordNumber : %{public}d, indicationType : %{public}d",
             recordNumber, indicationType);
    return 0;
}

int32_t RilCallback::CBConfigNotify(const OHOS::HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
                                    const CBConfigReportInfo &cellBroadConfigReportInfo)
{
    HDF_LOGI("RilCallback::CBConfigNotify cellBroadConfigReportInfo dcs : %{public}s",
             cellBroadConfigReportInfo.dcs.c_str());
    return 0;
}

int32_t RilCallback::SendGsmSmsResponse(const RilRadioResponseInfo &responseInfo,
                                        const SendSmsResultInfo &sendSmsResultInfo)
{
    g_sendGsmSmsResponseFlag = true;
    HDF_LOGI("RilCallback::SendGsmSmsResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
             sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_GSM_SMS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendCdmaSmsResponse(const RilRadioResponseInfo &responseInfo,
                                         const SendSmsResultInfo &sendSmsResultInfo)
{
    g_sendCdmaSmsResponseFlag = true;
    HDF_LOGI("RilCallback::SendCdmaSmsResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
             sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    hdiId_ = HdiId::HREQ_SMS_SEND_CDMA_SMS;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::AddSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    g_addSimMessageResponseFlag = true;
    HDF_LOGI("RilCallback::AddSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_ADD_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DelSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    g_delSimMessageResponseFlag = true;
    HDF_LOGI("RilCallback::DelSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_DEL_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}
int32_t RilCallback::UpdateSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    g_updateSimMessageResponseFlag = true;
    HDF_LOGI("RilCallback::UpdateSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_UPDATE_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::AddCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    g_addCdmaSimMessageResponseFlag = true;
    HDF_LOGI("RilCallback::AddCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_ADD_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::DelCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    g_delCdmaSimMessageResponseFlag = true;
    HDF_LOGI("RilCallback::DelCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_DEL_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::UpdateCdmaSimMessageResponse(const RilRadioResponseInfo &responseInfo)
{
    g_updateCdmaSimMessageResponseFlag = true;
    HDF_LOGI("RilCallback::UpdateCdmaSimMessageResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_UPDATE_CDMA_SIM_MESSAGE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetSmscAddrResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setSmscAddrResponseFlag = true;
    HDF_LOGI("RilCallback::SetSmscAddrResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_SMSC_ADDR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetSmscAddrResponse(const RilRadioResponseInfo &responseInfo,
                                         const ServiceCenterAddress &serviceCenterAddress)
{
    g_getSmscAddrResponseFlag = true;
    HDF_LOGI("RilCallback::GetSmscAddrResponse serviceCenterAddress tosca : %{public}d, address : %{public}s",
             serviceCenterAddress.tosca, serviceCenterAddress.address.c_str());
    hdiId_ = HdiId::HREQ_SMS_GET_SMSC_ADDR;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCBConfigResponse(const RilRadioResponseInfo &responseInfo)
{
    g_setCBConfigResponseFlag = true;
    HDF_LOGI("RilCallback::SetCBConfigResponse error : %{public}d", responseInfo.error);
    hdiId_ = HdiId::HREQ_SMS_SET_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCBConfigResponse(const RilRadioResponseInfo &responseInfo,
                                         const CBConfigInfo &cellBroadcastInfo)
{
    g_getCBConfigResponseFlag = true;
    HDF_LOGI("RilCallback::GetCBConfigResponse cellBroadcastInfo mids : %{public}s, dcss: %{public}s",
             cellBroadcastInfo.mids.c_str(), cellBroadcastInfo.dcss.c_str());
    hdiId_ = HdiId::HREQ_SMS_GET_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SetCdmaCBConfigResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SetCdmaCBConfigResponse error : %{public}d", responseInfo.error);
    g_setCdmaCBConfigResponseFlag = true;
    hdiId_ = HdiId::HREQ_SMS_SET_CDMA_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::GetCdmaCBConfigResponse(const RilRadioResponseInfo &responseInfo,
                                             const CdmaCBConfigInfo &cdmaCBConfigInfo)
{
    HDF_LOGI("RilCallback::GetCdmaCBConfigResponse cdmaCBConfigInfo service : %{public}d, language : %{public}d, "
             "checked: %{public}d",
             cdmaCBConfigInfo.service, cdmaCBConfigInfo.language, cdmaCBConfigInfo.checked);
    g_getCdmaCBConfigResponseFlag = true;
    hdiId_ = HdiId::HREQ_SMS_GET_CDMA_CB_CONFIG;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendSmsMoreModeResponse(const RilRadioResponseInfo &responseInfo,
                                             const SendSmsResultInfo &sendSmsResultInfo)
{
    HDF_LOGI("RilCallback::SendSmsMoreModeResponse sendSmsResultInfo pdu : %{public}s, error : %{public}d",
             sendSmsResultInfo.pdu.c_str(), sendSmsResultInfo.errCode);
    g_sendSmsMoreModeResponseFlag = true;
    hdiId_ = HdiId::HREQ_SMS_SEND_SMS_MORE_MODE;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::SendSmsAckResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::SendSmsAckResponse error:%{public}d", responseInfo.error);
    g_sendSmsAckResponseFlag = true;
    hdiId_ = HdiId::HREQ_SMS_SEND_SMS_ACK;
    resultInfo_ = responseInfo;
    NotifyAll();
    return 0;
}

int32_t RilCallback::CommonErrorResponse(const RilRadioResponseInfo &responseInfo)
{
    HDF_LOGI("RilCallback::CommonErrorResponse type:%{public}d error:%{public}d", responseInfo.type,
             responseInfo.error);
    return 0;
}

/**
** common fun
**/
void WaitFor(int32_t timeoutSecond) { g_callback.WaitFor(WAIT_TIME_SECOND); }

bool GetBoolResult(HdiId hdiId) { return g_callback.GetBoolResult(hdiId); }

int32_t GetSerialId()
{
    g_currentSerialId = rand() % RAND_MAXVALUE;
    return g_currentSerialId;
}

bool IsReady(int32_t slotId)
{
    if (g_rilInterface == nullptr) {
        return false;
    }
    return g_simState[slotId] != 0 && g_simState[slotId] != -1;
}