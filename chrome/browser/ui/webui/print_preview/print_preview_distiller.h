// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_PRINT_PREVIEW_DISTILLER_H_
#define CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_PRINT_PREVIEW_DISTILLER_H_

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "content/public/browser/web_contents_observer.h"

namespace net {
class URLRequestContextGetter;
}

// This class controls the rendering of the distilled contents
// generated by a source web contents
class PrintPreviewDistiller {
 public:
  PrintPreviewDistiller(content::WebContents* source_web_contents,
                        base::Closure on_failed_callback,
                        scoped_ptr<base::DictionaryValue> settings);
  ~PrintPreviewDistiller();

 private:
  class WebContentsDelegateImpl;

  // Create the web contents with a default
  // size. |session_storage_namespace| indicates the namespace that
  // the distiller content renderer's page should be part of.
  void CreateDestinationWebContents(
      content::SessionStorageNamespace* session_storage_namespace,
      content::WebContents* source_web_contents,
      scoped_ptr<base::DictionaryValue> settings,
      base::Closure on_failed_callback);

  content::WebContents* CreateWebContents(
      content::SessionStorageNamespace* session_storage_namespace,
      content::WebContents* source_web_contents);

  // The distilled rendered WebContents; may be null.
  scoped_ptr<content::WebContents> web_contents_;

  scoped_ptr<WebContentsDelegateImpl> web_contents_delegate_;

  DISALLOW_COPY_AND_ASSIGN(PrintPreviewDistiller);
};

#endif  // CHROME_BROWSER_UI_WEBUI_PRINT_PREVIEW_PRINT_PREVIEW_DISTILLER_H_
