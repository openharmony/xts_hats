/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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
#ifndef AUDIO_INFO_H
#define AUDIO_INFO_H

#ifdef __MUSL__
#include <stdint.h>
#endif // __MUSL__

#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <array>
#include <unistd.h>
#include <unordered_map>
#include <parcel.h>

namespace OHOS {
namespace AudioStandard {

// For common stream
constexpr uint32_t STREAM_ID_INVALID = 0;

//#ifdef FEATURE_DTMF_TONE  cant contain this macro due to idl dependency
// Maximun number of sine waves in a tone segment
constexpr uint32_t TONEINFO_MAX_WAVES = 3;
//Maximun number of SupportedTones
constexpr uint32_t MAX_SUPPORTED_TONEINFO_SIZE = 65535;
// Maximun number of segments in a tone descriptor
constexpr uint32_t TONEINFO_MAX_SEGMENTS = 12;
constexpr uint32_t TONEINFO_INF = 0xFFFFFFFF;

class ToneSegment : public Parcelable {
public:
    uint32_t duration;
    uint16_t waveFreq[TONEINFO_MAX_WAVES + 1];
    uint16_t loopCnt;
    uint16_t loopIndx;

    ToneSegment() = default;
    bool Marshalling(Parcel &parcel) const override
    {
        parcel.WriteUint32(duration);
        parcel.WriteUint16(loopCnt);
        parcel.WriteUint16(loopIndx);
        for (uint32_t i = 0; i < TONEINFO_MAX_WAVES + 1; i++) {
            parcel.WriteUint16(waveFreq[i]);
        }
        return true;
    }

    void UnmarshallingSelf(Parcel &parcel)
    {
        duration = parcel.ReadUint32();
        loopCnt = parcel.ReadUint16();
        loopIndx = parcel.ReadUint16();
        for (uint32_t i = 0; i < TONEINFO_MAX_WAVES + 1; i++) {
            waveFreq[i] = parcel.ReadUint16();
        }
    }

    static ToneSegment *Unmarshalling(Parcel &parcel)
    {
        auto info = new(std::nothrow) ToneSegment();
        if (info == nullptr) {
            return nullptr;
        }

        info->UnmarshallingSelf(parcel);
        return info;
    }
};

class ToneInfo : public Parcelable {
public:
    ToneSegment segments[TONEINFO_MAX_SEGMENTS + 1];
    uint32_t segmentCnt;
    uint32_t repeatCnt;
    uint32_t repeatSegment;
    ToneInfo() = default;
    bool Marshalling(Parcel &parcel) const override
    {
        parcel.WriteUint32(segmentCnt);
        parcel.WriteUint32(repeatCnt);
        parcel.WriteUint32(repeatSegment);
        if (!(segmentCnt >= 0 && segmentCnt <= TONEINFO_MAX_SEGMENTS + 1)) {
            return false;
        }
        for (uint32_t i = 0; i < segmentCnt; i++) {
            segments[i].Marshalling(parcel);
        }
        return true;
    }
    static ToneInfo *Unmarshalling(Parcel &parcel)
    {
        auto info = new(std::nothrow) ToneInfo();
        if (info == nullptr) {
            return nullptr;
        }
        info->segmentCnt = parcel.ReadUint32();
        info->repeatCnt = parcel.ReadUint32();
        info->repeatSegment = parcel.ReadUint32();
        if (!(info->segmentCnt >= 0 && info->segmentCnt <= TONEINFO_MAX_SEGMENTS + 1)) {
            delete info;
            return nullptr;
        }
        for (uint32_t i = 0; i < info->segmentCnt; i++) {
            info->segments[i].UnmarshallingSelf(parcel);
        }
        return info;
    }
};
//#endif

enum VolumeAdjustType {
    /**
     * Adjust volume up
     */
    VOLUME_UP = 0,
    /**
     * Adjust volume down
     */
    VOLUME_DOWN = 1,
};

enum ChannelBlendMode {
    /**
     * No channel process.
     */
    MODE_DEFAULT = 0,
    /**
     * Blend left and right channel.
     */
    MODE_BLEND_LR = 1,
    /**
     * Replicate left to right channel.
     */
    MODE_ALL_LEFT = 2,
    /**
     * Replicate right to left channel.
     */
    MODE_ALL_RIGHT = 3,
};

enum ConnectType {
    /**
     * Group connect type of local device
     */
    CONNECT_TYPE_LOCAL = 0,
    /**
     * Group connect type of distributed device
     */
    CONNECT_TYPE_DISTRIBUTED
};

// typedef AudioStreamType AudioVolumeType;

enum VolumeFlag {
    /**
     * Show system volume bar
     */
    FLAG_SHOW_SYSTEM_UI = 1,
};

enum AudioOffloadType {
    /**
     * Indicates audio offload state default.
     */
    OFFLOAD_DEFAULT = -1,
    /**
     * Indicates audio offload state : screen is active & app is foreground.
     */
    OFFLOAD_ACTIVE_FOREGROUND = 0,
    /**
     * Indicates audio offload state : screen is active & app is background.
     */
    OFFLOAD_ACTIVE_BACKGROUND = 1,
    /**
     * Indicates audio offload state : screen is inactive & app is background.
     */
    OFFLOAD_INACTIVE_BACKGROUND = 3,
};

enum FocusType {
    /**
     * Recording type.
     */
    FOCUS_TYPE_RECORDING = 0,
};

enum AudioErrors {
    /**
     * Common errors.
     */
    ERROR_INVALID_PARAM = 6800101,
    ERROR_NO_MEMORY     = 6800102,
    ERROR_ILLEGAL_STATE = 6800103,
    ERROR_UNSUPPORTED   = 6800104,
    ERROR_TIMEOUT       = 6800105,
    ERROR_UNSUPPORTED_FORMAT = 6800106,
    /**
     * Audio specific errors.
     */
    ERROR_STREAM_LIMIT  = 6800201,
    /**
     * Default error.
     */
    ERROR_SYSTEM        = 6800301
};

// Ringer Mode
enum AudioRingerMode {
    RINGER_MODE_SILENT = 0,
    RINGER_MODE_VIBRATE = 1,
    RINGER_MODE_NORMAL = 2
};

/**
 * Enumerates audio stream privacy type for playback capture.
 */
enum AudioPrivacyType {
    PRIVACY_TYPE_PUBLIC = 0,
    PRIVACY_TYPE_PRIVATE = 1,
    PRIVACY_TYPE_SHARED = 2
};

/**
* media safe volume status
*/
enum SafeStatus : int32_t {
    SAFE_UNKNOWN = -1,
    SAFE_INACTIVE = 0,
    SAFE_ACTIVE = 1,
};

enum CallbackChange : int32_t {
    CALLBACK_UNKNOWN = 0,
    CALLBACK_FOCUS_INFO_CHANGE,
    CALLBACK_RENDERER_STATE_CHANGE,
    CALLBACK_CAPTURER_STATE_CHANGE,
    CALLBACK_MICMUTE_STATE_CHANGE,
    CALLBACK_AUDIO_SESSION,
    CALLBACK_PREFERRED_OUTPUT_DEVICE_CHANGE,
    CALLBACK_PREFERRED_INPUT_DEVICE_CHANGE,
    CALLBACK_SET_VOLUME_KEY_EVENT,
    CALLBACK_SET_DEVICE_CHANGE,
    CALLBACK_SET_RINGER_MODE,
    CALLBACK_APP_VOLUME_CHANGE,
    CALLBACK_SELF_APP_VOLUME_CHANGE,
    CALLBACK_ACTIVE_VOLUME_TYPE_CHANGE,
    CALLBACK_SET_MIC_STATE_CHANGE,
    CALLBACK_SPATIALIZATION_ENABLED_CHANGE,
    CALLBACK_HEAD_TRACKING_ENABLED_CHANGE,
    CALLBACK_SET_MICROPHONE_BLOCKED,
    CALLBACK_DEVICE_CHANGE_WITH_INFO,
    CALLBACK_HEAD_TRACKING_DATA_REQUESTED_CHANGE,
    CALLBACK_NN_STATE_CHANGE,
    CALLBACK_SET_AUDIO_SCENE_CHANGE,
    CALLBACK_SPATIALIZATION_ENABLED_CHANGE_FOR_CURRENT_DEVICE,
    CALLBACK_DISTRIBUTED_OUTPUT_CHANGE,
    CALLBACK_FORMAT_UNSUPPORTED_ERROR,
    CALLBACK_STREAM_VOLUME_CHANGE,
    CALLBACK_SYSTEM_VOLUME_CHANGE,
    CALLBACK_AUDIO_SESSION_STATE,
    CALLBACK_AUDIO_SESSION_DEVICE,
    CALLBACK_AUDIO_SESSION_INPUT_DEVICE,
    CALLBACK_SET_DEVICE_INFO_UPDATE,
    CALLBACK_COLLABORATION_ENABLED_CHANGE_FOR_CURRENT_DEVICE,
    CALLBACK_SET_VOLUME_DEGREE_CHANGE,
    CALLBACK_PREFERRED_DEVICE_SET,
    CALLBACK_MAX,
};

enum AdjustStreamVolume {
    STREAM_VOLUME_INFO = 0,
    LOW_POWER_VOLUME_INFO,
    DUCK_VOLUME_INFO,
};

struct AdjustStreamVolumeInfo {
    float volume;
    uint32_t sessionId;
    std::string invocationTime;
};

constexpr CallbackChange CALLBACK_ENUMS[] = {
    CALLBACK_UNKNOWN,
    CALLBACK_FOCUS_INFO_CHANGE,
    CALLBACK_RENDERER_STATE_CHANGE,
    CALLBACK_CAPTURER_STATE_CHANGE,
    CALLBACK_MICMUTE_STATE_CHANGE,
    CALLBACK_AUDIO_SESSION,
    CALLBACK_PREFERRED_OUTPUT_DEVICE_CHANGE,
    CALLBACK_PREFERRED_INPUT_DEVICE_CHANGE,
    CALLBACK_SET_VOLUME_KEY_EVENT,
    CALLBACK_SET_DEVICE_CHANGE,
    CALLBACK_SET_VOLUME_KEY_EVENT,
    CALLBACK_ACTIVE_VOLUME_TYPE_CHANGE,
    CALLBACK_SET_DEVICE_CHANGE,
    CALLBACK_SET_RINGER_MODE,
    CALLBACK_SET_MIC_STATE_CHANGE,
    CALLBACK_SPATIALIZATION_ENABLED_CHANGE,
    CALLBACK_HEAD_TRACKING_ENABLED_CHANGE,
    CALLBACK_SET_MICROPHONE_BLOCKED,
    CALLBACK_DEVICE_CHANGE_WITH_INFO,
    CALLBACK_HEAD_TRACKING_DATA_REQUESTED_CHANGE,
    CALLBACK_NN_STATE_CHANGE,
    CALLBACK_SET_AUDIO_SCENE_CHANGE,
    CALLBACK_SPATIALIZATION_ENABLED_CHANGE_FOR_CURRENT_DEVICE,
    CALLBACK_DISTRIBUTED_OUTPUT_CHANGE,
    CALLBACK_FORMAT_UNSUPPORTED_ERROR,
    CALLBACK_STREAM_VOLUME_CHANGE,
    CALLBACK_SYSTEM_VOLUME_CHANGE,
    CALLBACK_AUDIO_SESSION_STATE,
    CALLBACK_AUDIO_SESSION_DEVICE,
    CALLBACK_AUDIO_SESSION_INPUT_DEVICE,
    CALLBACK_SET_DEVICE_INFO_UPDATE,
    CALLBACK_COLLABORATION_ENABLED_CHANGE_FOR_CURRENT_DEVICE,
    CALLBACK_SET_VOLUME_DEGREE_CHANGE,
    CALLBACK_PREFERRED_DEVICE_SET,
};

static_assert((sizeof(CALLBACK_ENUMS) / sizeof(CallbackChange)) == static_cast<size_t>(CALLBACK_MAX),
    "check CALLBACK_ENUMS");

enum PlayerType : int32_t {
    PLAYER_TYPE_DEFAULT = 0,

    // AudioFramework internal type.
    PLAYER_TYPE_OH_AUDIO_RENDERER = 100,
    PLAYER_TYPE_ARKTS_AUDIO_RENDERER = 101,
    PLAYER_TYPE_CJ_AUDIO_RENDERER = 102,
    PLAYER_TYPE_OPENSL_ES = 103,

    // Indicates a type from the system internals, but not from the AudioFramework.
    PLAYER_TYPE_SOUND_POOL = 1000,
    PLAYER_TYPE_AV_PLAYER = 1001,
    PLAYER_TYPE_SYSTEM_WEBVIEW = 1002,
    PLAYER_TYPE_TONE_PLAYER = 1003,
};

enum RecorderType : int32_t {
    RECORDER_TYPE_DEFAULT = 0,

    // AudioFramework internal type.
    RECORDER_TYPE_ARKTS_AUDIO_RECORDER = 100,
    RECORDER_TYPE_OPENSL_ES = 101,

    // Indicates a type from the system internals, but not from the AudioFramework.
    RECORDER_TYPE_AV_RECORDER = 1000,
};

enum AudioLoopbackMode {
    /** The hardware mode of audio loopback.*/
    LOOPBACK_HARDWARE = 0,
};

enum AudioLoopbackStatus {
    /** Audio loopback unavailable by the output or input device. For example, the device change.*/
    LOOPBACK_UNAVAILABLE_DEVICE = -2,
    /** Audio loopback unavailable by the audio scene. For example, the audio interrupt.*/
    LOOPBACK_UNAVAILABLE_SCENE = -1,
    /** Audio loopback available and idle.*/
    LOOPBACK_AVAILABLE_IDLE = 0,
    /** Audio loopback available and running.*/
    LOOPBACK_AVAILABLE_RUNNING = 1,
};

enum AudioLoopbackState {
    LOOPBACK_STATE_IDLE,
    LOOPBACK_STATE_PREPARED,
    LOOPBACK_STATE_RUNNING,
    LOOPBACK_STATE_DESTROYING,
    LOOPBACK_STATE_DESTROYED,
};

enum AudioLoopbackReverbPreset {
    /**
     * A preset that keep the original reverberation without any enhancement.
     */
    REVERB_PRESET_ORIGINAL = 1,
    /**
     * A preset representing a reverberation effect with karaoke-like acoustic characteristics.
     */
    REVERB_PRESET_KTV = 2,
    /**
     * A preset representing a reverberation effect with theater-like acoustic characteristics.
     */
    REVERB_PRESET_THEATER = 3,
    /**
     * A preset representing a reverberation effect with concert-like acoustic characteristics.
     */
    REVERB_PRESET_CONCERT = 4,
};

enum AudioLoopbackEqualizerPreset {
    /**
     * A preset that keep the original frequency response without any enhancement.
     */
    EQUALIZER_PRESET_FLAT = 1,
    /**
     * A preset representing a equalizer that can enhance the fullness of the vocie
     */
    EQUALIZER_PRESET_FULL = 2,
    /**
     * A preset representing a equalizer that can enhance the brightness of the vocie
     */
    EQUALIZER_PRESET_BRIGHT = 3,
};

struct MicStateChangeEvent : public Parcelable {
    bool mute;

    bool Marshalling(Parcel &parcel) const override
    {
        return parcel.WriteBool(mute);
    }

    static MicStateChangeEvent *Unmarshalling(Parcel &parcel)
    {
        auto event = new(std::nothrow) MicStateChangeEvent();
        if (event == nullptr) {
            return nullptr;
        }
        event->mute = parcel.ReadBool();
        return event;
    }
};

enum AudioScene : int32_t {
    /**
     * Invalid
     */
    AUDIO_SCENE_INVALID = -1,
    /**
     * Default audio scene
     */
    AUDIO_SCENE_DEFAULT,
    /**
     * Ringing audio scene
     * Only available for system api.
     */
    AUDIO_SCENE_RINGING,
    /**
     * Phone call audio scene
     * Only available for system api.
     */
    AUDIO_SCENE_PHONE_CALL,
    /**
     * Voice chat audio scene
     */
    AUDIO_SCENE_PHONE_CHAT,
    /**
     * AvSession set call start flag
     */
    AUDIO_SCENE_CALL_START,
    /**
     * AvSession set call end flag
     */
    AUDIO_SCENE_CALL_END,
    /**
     * Voice ringing audio scene
     * Only available for system api.
     */
    AUDIO_SCENE_VOICE_RINGING,
    /**
     * Max
     */
    AUDIO_SCENE_MAX,
};

enum AudioDeviceUsage : uint32_t {
    /**
     * Media output devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    MEDIA_OUTPUT_DEVICES = 1,
    /**
     * Media input devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    MEDIA_INPUT_DEVICES = 2,
    /**
     * All media devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    ALL_MEDIA_DEVICES = 3,
    /**
     * Call output devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    CALL_OUTPUT_DEVICES = 4,
    /**
     * Call input devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    CALL_INPUT_DEVICES = 8,
    /**
     * All call devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    ALL_CALL_DEVICES = 12,
    /**
     * All devices.
     * @syscap SystemCapability.Multimedia.Audio.Device
     * @systemapi
     * @since 11
     */
    D_ALL_DEVICES = 15,
};

enum BluetoothAndNearlinkPreferredRecordCategory : uint32_t {
    /**
     * @brief Not prefer to use bluetooth and nearlink record.
     */
    PREFERRED_NONE = 0,

    /**
     * @brief Prefer to use bluetooth and nearlink record.
     * However, whether to use low latency or high quality recording
     * dpends on system.
     */
    PREFERRED_DEFAULT = 1,

    /**
     * @brief Prefer to use bluetooth and nearlink low latency mode to record.
     */
    PREFERRED_LOW_LATENCY = 2,

    /**
     * @brief Prefer to use bluetooth and nearlink high quality mode to record.
     */
    PREFERRED_HIGH_QUALITY = 3,
};

enum FilterMode : uint32_t {
    INCLUDE = 0,
    EXCLUDE,
    MAX_FILTER_MODE
};

inline constexpr uint32_t MAX_VALID_USAGE_SIZE = 30; // 128 for pids
inline constexpr uint32_t MAX_VALID_PIDS_SIZE = 128; // 128 for pids

struct BufferQueueState {
    uint32_t numBuffers;
    uint32_t currentIndex;
};

enum AudioRenderMode {
    RENDER_MODE_NORMAL,
    RENDER_MODE_CALLBACK,
    RENDER_MODE_STATIC
};

struct SinkInfo {
    uint32_t sinkId; // sink id
    std::string sinkName;
    std::string adapterName;
};

typedef uint32_t AudioIOHandle;

enum AudioServiceIndex {
    HDI_SERVICE_INDEX = 0,
    AUDIO_SERVICE_INDEX
};

/**
 * @brief Enumerates the rendering states of the current device.
 */
enum RendererState {
    /** INVALID state */
    RENDERER_INVALID = -1,
    /** Create New Renderer instance */
    RENDERER_NEW,
    /** Reneder Prepared state */
    RENDERER_PREPARED,
    /** Rendere Running state */
    RENDERER_RUNNING,
    /** Renderer Stopped state */
    RENDERER_STOPPED,
    /** Renderer Released state */
    RENDERER_RELEASED,
    /** Renderer Paused state */
    RENDERER_PAUSED
};

/**
 * @brief Enumerates the capturing states of the current device.
 */
enum CapturerState {
    /** Capturer INVALID state */
    CAPTURER_INVALID = -1,
    /** Create new capturer instance */
    CAPTURER_NEW,
    /** Capturer Prepared state */
    CAPTURER_PREPARED,
    /** Capturer Running state */
    CAPTURER_RUNNING,
    /** Capturer Stopped state */
    CAPTURER_STOPPED,
    /** Capturer Released state */
    CAPTURER_RELEASED,
    /** Capturer Paused state */
    CAPTURER_PAUSED
};

enum State {
    /** INVALID */
    INVALID = -1,
    /** New */
    NEW,
    /** Prepared */
    PREPARED,
    /** Running */
    RUNNING,
    /** Stopped */
    STOPPED,
    /** Released */
    RELEASED,
    /** Paused */
    PAUSED,
    /** Stopping */
    STOPPING
};

/**
 * @brief Defines the fast status.
 */
enum FastStatus {
    /** Invalid status */
    FASTSTATUS_INVALID = -1,
    /** Normal status */
    FASTSTATUS_NORMAL,
    /** Fast status */
    FASTSTATUS_FAST
};

enum PlaybackCaptureStartState {
    /* Internal recording started successfully. */
    START_STATE_SUCCESS = 0,
    /* Start playback capture failed */
    START_STATE_FAILED = 1,
    /* Start playback capture but user not authorized state. */
    START_STATE_NOT_AUTHORIZED = 2
};

enum StateChangeCmdType {
    CMD_FROM_CLIENT = 0,
    CMD_FROM_SYSTEM = 1
};

enum AudioMode {
    AUDIO_MODE_PLAYBACK,
    AUDIO_MODE_RECORD
};

enum StopAudioType {
    STOP_ALL,
    STOP_RENDER,
    STOP_RECORD
};

enum RecordErrorCode {
    RECORD_ERROR_GET_FOCUS_FAIL,
    RECORD_ERROR_NO_FOCUS_CFG,
};

enum InnerCapMode : uint32_t {
    LEGACY_INNER_CAP = 0,
    LEGACY_MUTE_CAP,
    MODERN_INNER_CAP,
    INVALID_CAP_MODE
};

struct Volume {
    bool isMute = false;
    float volumeFloat = 1.0f;
    uint32_t volumeInt = 0;
    uint32_t volumeDegree = 0;
};

enum AppIsBackState {
    STATE_UNKNOWN = -1,
    STATE_FOREGROUND,
    STATE_BACKGROUND,
    STATE_END,
};

enum StreamSetState {
    STREAM_PAUSE,
    STREAM_RESUME,
    STREAM_MUTE,
    STREAM_UNMUTE
};

enum SwitchState {
    SWITCH_STATE_WAITING,
    SWITCH_STATE_TIMEOUT,
    SWITCH_STATE_CREATED,
    SWITCH_STATE_STARTED,
    SWITCH_STATE_FINISHED
};

enum AudioPin {
    AUDIO_PIN_NONE = 0, // Invalid pin
    AUDIO_PIN_OUT_SPEAKER = 1 << 0, // Speaker output pin
    AUDIO_PIN_OUT_HEADSET = 1 << 1, // Wired headset pin for output
    AUDIO_PIN_OUT_LINEOUT = 1 << 2, // Line-out pin
    AUDIO_PIN_OUT_HDMI = 1 << 3, // HDMI output pin
    AUDIO_PIN_OUT_USB = 1 << 4, // USB output pin
    AUDIO_PIN_OUT_USB_EXT = 1 << 5, // Extended USB output pin
    AUDIO_PIN_OUT_EARPIECE = 1 << 5 | 1 << 4, // Earpiece output pin
    AUDIO_PIN_OUT_BLUETOOTH_SCO = 1 << 6, // Bluetooth SCO output pin
    AUDIO_PIN_OUT_DAUDIO_DEFAULT = 1 << 7, // Daudio default output pin
    AUDIO_PIN_OUT_HEADPHONE = 1 << 8, // Wired headphone output pin
    AUDIO_PIN_OUT_USB_HEADSET = 1 << 9,  // Arm usb output pin
    AUDIO_PIN_OUT_BLUETOOTH_A2DP = 1 << 10,  // Bluetooth A2dp output pin
    AUDIO_PIN_OUT_DP = 1 << 11,
    AUDIO_PIN_OUT_NEARLINK = 1 << 12, // Nearlink output pin
    AUDIO_PIN_OUT_HEARING_AID = 1 << 13, // HearingAid output pin
    AUDIO_PIN_IN_MIC = 1 << 27 | 1 << 0, // Microphone input pin
    AUDIO_PIN_IN_HS_MIC = 1 << 27 | 1 << 1, // Wired headset microphone pin for input
    AUDIO_PIN_IN_LINEIN = 1 << 27 | 1 << 2, // Line-in pin
    AUDIO_PIN_IN_USB_EXT = 1 << 27 | 1 << 3, // Extended USB input pin
    AUDIO_PIN_IN_BLUETOOTH_SCO_HEADSET = 1 << 27 | 1 << 4, // Bluetooth SCO headset input pin
    AUDIO_PIN_IN_DAUDIO_DEFAULT = 1 << 27 | 1 << 5, // Daudio default input pin
    AUDIO_PIN_IN_USB_HEADSET = 1 << 27 | 1 << 6,  // Arm usb input pin
    AUDIO_PIN_IN_PENCIL = 1 << 27 | 1 << 7,  // Pencil input pin
    AUDIO_PIN_IN_UWB = 1 << 27 | 1 << 8,  // Remote control input pin
    AUDIO_PIN_IN_NEARLINK = 1 << 27 | 1 << 9,  // Nearlink input pin
};

enum AudioParamKey {
    NONE = 0,
    VOLUME = 1,
    INTERRUPT = 2,
    PARAM_KEY_STATE = 5,
    A2DP_SUSPEND_STATE = 6,  // for bluetooth sink
    BT_HEADSET_NREC = 7,
    BT_WBS = 8,
    A2DP_OFFLOAD_STATE = 9, // for a2dp offload
    GET_DP_DEVICE_INFO = 10, // for dp sink
    GET_PENCIL_INFO = 11, // for pencil source
    GET_UWB_INFO = 12, // for remote control source
    USB_DEVICE = 101, // Check USB device type ARM or HIFI
    PERF_INFO = 201,
    MMI = 301,
    PARAM_KEY_LOWPOWER = 1000,
    AUDIO_EXT_PARAM_KEY_CUSTOM = 1001,
};

struct HWDecodingInfo {
    uint64_t pts = 0;
    uint32_t size = 0;
    int32_t optCode = 0;
};

struct AudioRendererDataInfo {
    uint8_t *buffer;
    size_t flag;
};

enum AudioPermissionState {
    AUDIO_PERMISSION_START = 0,
    AUDIO_PERMISSION_STOP = 1,
};

class AudioRendererPolicyServiceDiedCallback {
public:
    virtual ~AudioRendererPolicyServiceDiedCallback() = default;

    /**
     * Called when audio policy service died.
     * @since 10
     */
    virtual void OnAudioPolicyServiceDied() = 0;
};

class AudioCapturerPolicyServiceDiedCallback {
public:
    virtual ~AudioCapturerPolicyServiceDiedCallback() = default;

    /**
     * Called when audio policy service died.
     * @since 10
     */
    virtual void OnAudioPolicyServiceDied() = 0;
};

class AudioStreamPolicyServiceDiedCallback {
public:
    virtual ~AudioStreamPolicyServiceDiedCallback() = default;

    /**
     * Called when audio policy service died.
     * @since 11
     */
    virtual void OnAudioPolicyServiceDied() = 0;
};

class AudioSessionManagerPolicyServiceDiedCallback {
public:
    virtual ~AudioSessionManagerPolicyServiceDiedCallback() = default;

    /**
     * Called when audio policy service died.
     * @since 20
     */
    virtual void OnAudioPolicyServiceDied() = 0;
};

/**
 * Describes three-dimensional value.
 * @since 11
 */
struct Vector3D {
    /**
     * X-axis value.
     * @since 11
     */
    float x;
    /**
     * Y-axis value.
     * @since 11
     */
    float y;
    /**
     * Z-axis value.
     * @since 11
     */
    float z;
};

enum CastType {
    CAST_TYPE_NULL = 0,
    CAST_TYPE_ALL,
    CAST_TYPE_PROJECTION,
    CAST_TYPE_COOPERATION,
};

class AudioPnpDeviceChangeCallback {
public:
    virtual ~AudioPnpDeviceChangeCallback() = default;
    virtual void OnPnpDeviceStatusChanged(const std::string &info) = 0;
    virtual void OnMicrophoneBlocked(const std::string &info) = 0;
};

/**
 * @brief Device group used by set/get volume.
 */
enum DeviceGroup {
    /** Invalid device group */
    DEVICE_GROUP_INVALID = -1,
    /** Built in device */
    DEVICE_GROUP_BUILT_IN,
    /** Wired device */
    DEVICE_GROUP_WIRED,
    /** Wireless device */
    DEVICE_GROUP_WIRELESS,
    /** Remote cast device */
    DEVICE_GROUP_REMOTE_CAST,
    /* earpiece device*/
    DEVICE_GROUP_EARPIECE,
    /** Dp device */
    DEVICE_GROUP_DP,
};

enum RouterType {
    /**
     * None router.
     * @since 12
     */
    ROUTER_TYPE_NONE = 0,
    /**
     * Default router.
     * @since 12
     */
    ROUTER_TYPE_DEFAULT,
    /**
     * Stream filter router.
     * @since 12
     */
    ROUTER_TYPE_STREAM_FILTER,
    /**
     * Package filter router.
     * @since 12
     */
    ROUTER_TYPE_PACKAGE_FILTER,
    /**
     * Cockpit phone router.
     * @since 12
     */
    ROUTER_TYPE_COCKPIT_PHONE,
    /**
     * Privacy priority router.
     * @since 12
     */
    ROUTER_TYPE_PRIVACY_PRIORITY,
    /**
     * Public priority router.
     * @since 12
     */
    ROUTER_TYPE_PUBLIC_PRIORITY,
    /**
     * Pair device router.
     * @since 12
     */
    ROUTER_TYPE_PAIR_DEVICE,
    /**
     * User select router.
     * @since 12
     */
    ROUTER_TYPE_USER_SELECT,

    /**
     * App select router.
     * @since 12
     */
    ROUTER_TYPE_APP_SELECT,
};

enum RenderMode {
    /**
     * Primary render mode.
     * @since 12
     */
    PRIMARY,
    /**
     * VOIP render mode.
     * @since 12
     */
    VOIP,
    /**
     * Offload render mode.
     * @since 12
     */
    OFFLOAD,
    /**
     * Low latency render mode.
     * @since 12
     */
    LOW_LATENCY,
};

enum WriteDataCallbackType {
    /**
     * Use OH_AudioRenderer_Callbacks.OH_AudioRenderer_OnWriteData
     */
    WRITE_DATA_CALLBACK_WITHOUT_RESULT = 0,
    /**
     * Use OH_AudioRenderer_OnWriteDataCallback
     */
    WRITE_DATA_CALLBACK_WITH_RESULT = 1,
    /**
     * Use OH_AudioRenderer_OnWriteDataCallbackAdvanced
    */
    WRITE_DATA_CALLBACK_ADVANCED = 2,
};

enum ReadDataCallbackType {
    /**
     * Use OH_AudioCapturer_Callbacks.OH_AudioCapturer_OnReadData
     */
    READ_DATA_CALLBACK_WITHOUT_RESULT = 0,
    /**
     * Use OH_AudioCapturer_OnReadDataCallback
     */
    READ_DATA_CALLBACK_WITH_RESULT = 1
};

enum StreamEventCallbackType {
    /**
     * Use OH_AudioCapturer_Callbacks.OH_AudioCapturer_OnStreamEvent
     */
    STREAM_EVENT_CALLBACK_COMBINED = 0,
    /**
     * Use OH_AudioCapturer_OnStreamEventCallback
     */
    STREAM_EVENT_CALLBACK_SEPERATED = 1
};

enum ErrorCallbackType {
    /**
     * Use OH_AudioRenderer_Callbacks.OH_AudioRenderer_OnError
     */
    ERROR_CALLBACK_COMBINED = 0,
    /**
     * Use OH_AudioRenderer_OnErrorCallback
     */
    ERROR_CALLBACK_SEPERATED = 1
};

enum PolicyType {
    EDM_POLICY_TYPE = 0,
    PRIVACY_POLCIY_TYPE = 1,
    TEMPORARY_POLCIY_TYPE = 2,
};

enum SuscribeResultCode {
    SUCCESS_SUBSCRIBE = 0,
    /**
     * Volume button input error
     */
    ERR_SUBSCRIBE_INVALID_PARAM,
     /**
     * The keyOption creation failed
     */
    ERR_SUBSCRIBE_KEY_OPTION_NULL,
     /**
     * The im pointer creation failed
     */
    ERR_SUBSCRIBE_MMI_NULL,
    /**
     * Volume key multimode subscription results
     */
    ERR_MODE_SUBSCRIBE,
};

enum RestoreStatus : int32_t {
    NO_NEED_FOR_RESTORE = 0,
    NEED_RESTORE,
    RESTORING,
    RESTORE_ERROR,
    NEED_RESTORE_TO_NORMAL,
};

enum RestoreReason : int32_t {
    DEFAULT_REASON = 0,
    DEVICE_CHANGED,
    STREAM_CONCEDED,
    STREAM_SPLIT,
    SERVER_DIED,
};

enum CheckPosTimeRes : int32_t {
    CHECK_SUCCESS = 0,
    CHECK_FAILED,
    NEED_MODIFY,
};

enum AudioProcessStage {
    AUDIO_PROC_STAGE_STOP,
    AUDIO_PROC_STAGE_STOP_BY_RELEASE,
};

enum RendererStage {
    RENDERER_STAGE_UNKNOWN = 0,
    RENDERER_STAGE_START_OK = 0x10,
    RENDERER_STAGE_START_FAIL = 0x11,
    RENDERER_STAGE_PAUSE_OK = 0x20,
    RENDERER_STAGE_STOP_OK = 0x30,
    RENDERER_STAGE_STOP_BY_RELEASE = 0x31,
    RENDERER_STAGE_STANDBY_BEGIN = 0x40,
    RENDERER_STAGE_STANDBY_END = 0x41,
    RENDERER_STAGE_SET_VOLUME_ZERO = 0x50,
    RENDERER_STAGE_SET_VOLUME_NONZERO = 0x51,
};

enum CapturerStage {
    CAPTURER_STAGE_START_OK = 0x10,
    CAPTURER_STAGE_START_FAIL = 0x11,
    CAPTURER_STAGE_PAUSE_OK = 0x20,
    CAPTURER_STAGE_STOP_OK = 0x30,
    CAPTURER_STAGE_STOP_BY_RELEASE = 0x31,
};

/**
 * Enumerates the method sources that trigger priority boosting.
 * Used to distinguish between different triggering entry points.
 */
enum BoostTriggerMethod : uint32_t {
    METHOD_START = 0,
    METHOD_WRITE_OR_READ,
    METHOD_MAX
};

enum XperfEventId : int32_t {
    XPERF_EVENT_START = 0,
    XPERF_EVENT_STOP = 1,
    XPERF_EVENT_RELEASE = 2,
    XPERF_EVENT_FAULT = 3,
    XPERF_EVENT_MAX = 4,
};

/**
 * Enumerates the audio playback target
 */
enum RenderTarget {
    NORMAL_PLAYBACK = 0,
    INJECT_TO_VOICE_COMMUNICATION_CAPTURE = 1
};

/**
 * remote device splite stream enum
 */
enum SplitStreamType {
    STREAM_TYPE_DEFAULT = 0,
    STREAM_TYPE_MEDIA = 1,
    STREAM_TYPE_COMMUNICATION = 2,
    STREAM_TYPE_NAVIGATION = 13
};

/**
 * static audiorenderer callback event enum
 */
enum StaticBufferEventId : int32_t {
    BUFFER_END_EVENT = 0,
    LOOP_END_EVENT = 1
};

class StaticBufferEventCallback {
public:
    virtual ~StaticBufferEventCallback() = default;

    virtual void OnStaticBufferEvent(StaticBufferEventId eventId) = 0;
};

enum LatencyFlag : uint32_t {
    LATENCY_FLAG_SHARED_BUFFER = 1U << 0,
    LATENCY_FLAG_ENGINE = 1U << 1,
    LATENCY_FLAG_SOFTWARE = 0X0FFFFFFF,
    LATENCY_FLAG_HARDWARE = 1U << 31,
    LATENCY_FLAG_ALL = 0XFFFFFFFF
};
} // namespace AudioStandard
} // namespace OHOS
#endif // AUDIO_INFO_H
