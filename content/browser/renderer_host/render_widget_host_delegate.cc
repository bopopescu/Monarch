// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "content/browser/renderer_host/render_widget_host_delegate.h"

namespace content {

bool RenderWidgetHostDelegate::PreHandleKeyboardEvent(
    const NativeWebKeyboardEvent& event,
    bool* is_keyboard_shortcut) {
  return false;
}

bool RenderWidgetHostDelegate::HandleWheelEvent(
    const blink::WebMouseWheelEvent& event) {
  return false;
}

bool RenderWidgetHostDelegate::PreHandleGestureEvent(
    const blink::WebGestureEvent& event) {
  return false;
}

BrowserAccessibilityManager*
    RenderWidgetHostDelegate::GetRootBrowserAccessibilityManager() {
  return NULL;
}

BrowserAccessibilityManager*
    RenderWidgetHostDelegate::GetOrCreateRootBrowserAccessibilityManager() {
  return NULL;
}

#if defined(OS_WIN)
gfx::NativeViewAccessible
    RenderWidgetHostDelegate::GetParentNativeViewAccessible() {
  return NULL;
}
#endif

// If a delegate does not override this, the RenderWidgetHostView will
// assume it is the sole platform event consumer.
RenderWidgetHostInputEventRouter*
RenderWidgetHostDelegate::GetInputEventRouter() {
  return nullptr;
}

}  // namespace content
