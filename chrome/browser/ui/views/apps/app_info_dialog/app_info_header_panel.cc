// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/views/apps/app_info_dialog/app_info_header_panel.h"

#include "base/bind.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/extension_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/extensions/extension_constants.h"
#include "chrome/grit/generated_resources.h"
#include "extensions/browser/extension_system.h"
#include "extensions/browser/image_loader.h"
#include "extensions/common/constants.h"
#include "extensions/common/extension.h"
#include "extensions/common/extension_icon_set.h"
#include "extensions/common/extension_resource.h"
#include "extensions/common/manifest_handlers/icons_handler.h"
#include "extensions/common/manifest_url_handlers.h"
#include "net/base/url_util.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_rep.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/link.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/layout/layout_constants.h"
#include "ui/views/view.h"

namespace {

// Size of extension icon in top left of dialog.
const int kIconSize = 64;

}  // namespace

AppInfoHeaderPanel::AppInfoHeaderPanel(Profile* profile,
                                       const extensions::Extension* app)
    : AppInfoPanel(profile, app),
      app_icon_(NULL),
      view_in_store_link_(NULL),
      weak_ptr_factory_(this) {
  SetLayoutManager(
      new views::BoxLayout(views::BoxLayout::kHorizontal,
                           views::kButtonHEdgeMargin,
                           views::kButtonVEdgeMargin,
                           views::kRelatedControlHorizontalSpacing));

  CreateControls();
}

AppInfoHeaderPanel::~AppInfoHeaderPanel() {
}

void AppInfoHeaderPanel::CreateControls() {
  app_icon_ = new views::ImageView();
  app_icon_->SetImageSize(gfx::Size(kIconSize, kIconSize));
  AddChildView(app_icon_);
  LoadAppImageAsync();

  // Create a vertical container to store the app's name and link.
  views::View* vertical_info_container = new views::View();
  views::BoxLayout* vertical_container_layout =
      new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0);
  vertical_container_layout->set_main_axis_alignment(
      views::BoxLayout::MAIN_AXIS_ALIGNMENT_CENTER);
  vertical_info_container->SetLayoutManager(vertical_container_layout);
  AddChildView(vertical_info_container);

  views::Label* app_name_label =
      new views::Label(base::UTF8ToUTF16(app_->name()),
                       ui::ResourceBundle::GetSharedInstance().GetFontList(
                           ui::ResourceBundle::MediumFont));
  app_name_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  vertical_info_container->AddChildView(app_name_label);

  if (CanShowAppInWebStore()) {
    view_in_store_link_ = new views::Link(
        l10n_util::GetStringUTF16(IDS_APPLICATION_INFO_WEB_STORE_LINK));
    view_in_store_link_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    view_in_store_link_->set_listener(this);
    vertical_info_container->AddChildView(view_in_store_link_);
  } else {
    // If there's no link, allow the app's name to take up multiple lines.
    // TODO(sashab): Limit the number of lines to 2.
    app_name_label->SetMultiLine(true);
  }
}

void AppInfoHeaderPanel::LinkClicked(views::Link* source, int event_flags) {
  DCHECK_EQ(source, view_in_store_link_);
  ShowAppInWebStore();
}

void AppInfoHeaderPanel::LoadAppImageAsync() {
  extensions::ExtensionResource image = extensions::IconsInfo::GetIconResource(
      app_,
      extension_misc::EXTENSION_ICON_LARGE,
      ExtensionIconSet::MATCH_BIGGER);
  int pixel_size =
      static_cast<int>(kIconSize * gfx::ImageSkia::GetMaxSupportedScale());
  extensions::ImageLoader::Get(profile_)->LoadImageAsync(
      app_,
      image,
      gfx::Size(pixel_size, pixel_size),
      base::Bind(&AppInfoHeaderPanel::OnAppImageLoaded, AsWeakPtr()));
}

void AppInfoHeaderPanel::OnAppImageLoaded(const gfx::Image& image) {
  const SkBitmap* bitmap;
  if (image.IsEmpty()) {
    bitmap = &extensions::util::GetDefaultAppIcon()
                  .GetRepresentation(gfx::ImageSkia::GetMaxSupportedScale())
                  .sk_bitmap();
  } else {
    bitmap = image.ToSkBitmap();
  }

  app_icon_->SetImage(gfx::ImageSkia::CreateFrom1xBitmap(*bitmap));
}

void AppInfoHeaderPanel::ShowAppInWebStore() {
  DCHECK(CanShowAppInWebStore());
  Close();
  OpenLink(net::AppendQueryParameter(
      extensions::ManifestURL::GetDetailsURL(app_),
      extension_urls::kWebstoreSourceField,
      extension_urls::kLaunchSourceAppListInfoDialog));
}

bool AppInfoHeaderPanel::CanShowAppInWebStore() const {
  // Hide the webstore link for apps which were installed by default,
  // since this could leak user counts for OEM-specific apps.
  // Also hide Shared Modules because they are automatically installed
  // by Chrome when dependent Apps are installed.
  return app_->from_webstore() && !app_->was_installed_by_default() &&
      !app_->is_shared_module();
}
