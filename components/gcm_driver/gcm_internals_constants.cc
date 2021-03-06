// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/gcm_driver/gcm_internals_constants.h"

namespace gcm_driver {

// Resource paths.
const char kGcmInternalsCSS[] = "gcm_internals.css";
const char kGcmInternalsJS[] = "gcm_internals.js";

// Message handlers.
const char kGetGcmInternalsInfo[] = "getGcmInternalsInfo";
const char kSetGcmInternalsInfo[] = "gcmInternals.setGcmInternalsInfo";
const char kSetGcmInternalsRecording[] = "setGcmInternalsRecording";

// GCM internal info.
const char kAndroidId[] = "androidId";
const char kCheckinInfo[] = "checkinInfo";
const char kConnectionClientCreated[] = "connectionClientCreated";
const char kConnectionInfo[] = "connectionInfo";
const char kConnectionState[] = "connectionState";
const char kDeviceInfo[] = "deviceInfo";
const char kGcmClientCreated[] = "gcmClientCreated";
const char kGcmClientState[] = "gcmClientState";
const char kGcmEnabled[] = "gcmEnabled";
const char kIsRecording[] = "isRecording";
const char kProfileServiceCreated[] = "profileServiceCreated";
const char kReceiveInfo[] = "receiveInfo";
const char kRegisteredAppIds[] = "registeredAppIds";
const char kRegistrationInfo[] = "registrationInfo";
const char kResendQueueSize[] = "resendQueueSize";
const char kSendInfo[] = "sendInfo";
const char kSendQueueSize[] = "sendQueueSize";

}  // namespace gcm_driver
