// Copyright (c) 2012 The Tangram Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/local/local_view.h"

#include "chrome/browser/ui/view_ids.h"

LocalView::LocalView(content::BrowserContext* browser_context)
    : views::WebView(browser_context) {
  SetID(VIEW_ID_NONE);

  LoadInitialURL(GURL("http://127.0.0.1:3000/sigma"));
  // SetBackground(views::CreateSolidBackground(gfx::kGoogleRedDark500));
}

LocalView::~LocalView() {}

gfx::Size LocalView::CalculatePreferredSize() const {
  gfx::Size prefsize;
  int preferred_height = 40;
  prefsize.set_height(preferred_height);
  return prefsize;
}

gfx::Size LocalView::GetMinimumSize() const {
  int width = 8;
  int height = 40;
  return gfx::Size(width, height);
}