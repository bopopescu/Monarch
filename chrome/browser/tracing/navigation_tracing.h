// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TRACING_NAVIGATION_TRACING_H_
#define CHROME_BROWSER_TRACING_NAVIGATION_TRACING_H_

#include "content/public/browser/background_tracing_manager.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

namespace tracing {

void SetupNavigationTracing();

class NavigationTracingObserver
    : public content::WebContentsObserver,
      public content::WebContentsUserData<NavigationTracingObserver> {
 public:
  static bool IsEnabled();

 private:
  friend class content::WebContentsUserData<NavigationTracingObserver>;

  explicit NavigationTracingObserver(content::WebContents* web_contents);

  ~NavigationTracingObserver() override;

  // content::WebContentsObserver implementation.
  void DidStartProvisionalLoadForFrame(
      content::RenderFrameHost* render_frame_host,
      const GURL& validated_url,
      bool is_error_page,
      bool is_iframe_srcdoc) override;

  static content::BackgroundTracingManager::TriggerHandle navigation_handle;

  DISALLOW_COPY_AND_ASSIGN(NavigationTracingObserver);
};

}  // namespace tracing

#endif  // CHROME_BROWSER_TRACING_NAVIGATION_TRACING_H_