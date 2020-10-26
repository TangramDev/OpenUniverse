// Copyright (c) 2020 The Tangram Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_LOCAL_VIEW_H_
#define CHROME_BROWSER_UI_VIEWS_LOCAL_VIEW_H_

#include "ui/views/controls/webview/webview.h"

class LocalView : public views::WebView {
 public:
  LocalView(content::BrowserContext* browser_context);
  ~LocalView() override;

  gfx::Size CalculatePreferredSize() const override;
  gfx::Size GetMinimumSize() const override;
};

#endif  // CHROME_BROWSER_UI_VIEWS_LOCAL_VIEW_H_
