// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_AUTOFILL_SAVE_CARD_BUBBLE_VIEW_H_
#define CHROME_BROWSER_UI_AUTOFILL_SAVE_CARD_BUBBLE_VIEW_H_

#include "base/macros.h"

namespace autofill {

// The cross-platform UI interface which displays the "Save credit card?"
// bubble. This object is responsible for its own lifetime.
class SaveCardBubbleView {
 public:
  virtual void Show() = 0;
  virtual void Close() = 0;
  virtual void ControllerGone() = 0;

 protected:
  SaveCardBubbleView() {}
  virtual ~SaveCardBubbleView() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(SaveCardBubbleView);
};

}  // namespace autofill

#endif  // CHROME_BROWSER_UI_AUTOFILL_SAVE_CARD_BUBBLE_VIEW_H_