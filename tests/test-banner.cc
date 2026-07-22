/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() {
  ++notified;
}

int button_clicked_count;
static void on_button_clicked() {
  button_clicked_count++;
}

static void test_adw_banner_title() {
  Adw::Banner banner;

  notified = 0;
  banner.property_title().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring title = banner.get_property<Glib::ustring>("title");
  g_assert_true(title == "");

  banner.set_title("");
  g_assert_true(notified == 0);

  banner.set_title("Update available");
  g_assert_true(banner.get_title() == "Update available");
  g_assert_true(notified == 1);

  banner.set_property<Glib::ustring>("title", "");
  g_assert_true(banner.get_title() == "");
  g_assert_true(notified == 2);
}

static void test_adw_banner_button_label() {
  Adw::Banner banner;

  notified = 0;
  banner.property_button_label().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring label = banner.get_property<Glib::ustring>("button-label");
  g_assert_true(label == "");

  banner.set_button_label("");
  g_assert_true(notified == 1);

  banner.set_button_label("Restart");
  g_assert_true(banner.get_button_label() == "Restart");
  g_assert_true(notified == 2);
}

static void test_adw_banner_revealed() {
  Adw::Banner banner;

  notified = 0;
  banner.property_revealed().signal_changed().connect(sigc::ptr_fun(notify_cb));

  // Banners are hidden by default.
  g_assert_false(banner.get_revealed());

  banner.set_revealed(false);
  g_assert_true(notified == 0);

  banner.set_revealed(true);
  g_assert_true(banner.get_revealed());
  g_assert_true(notified == 1);
}

static void test_adw_banner_use_markup() {
  Adw::Banner banner;

  notified = 0;
  banner.property_use_markup().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool orig = banner.get_use_markup();

  banner.set_use_markup(orig);
  g_assert_true(notified == 0);

  banner.set_use_markup(!orig);
  g_assert_true(banner.get_use_markup() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_banner_button_style() {
  Adw::Banner banner;

  notified = 0;
  banner.property_button_style().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(banner.get_button_style() == Adw::Banner::ButtonStyle::DEFAULT);

  banner.set_button_style(Adw::Banner::ButtonStyle::DEFAULT);
  g_assert_true(notified == 0);

  banner.set_button_style(Adw::Banner::ButtonStyle::SUGGESTED);
  g_assert_true(banner.get_button_style() == Adw::Banner::ButtonStyle::SUGGESTED);
  g_assert_true(notified == 1);
}

static void test_adw_banner_button_clicked() {
  Adw::Banner banner;

  button_clicked_count = 0;
  banner.signal_button_clicked().connect(sigc::ptr_fun(on_button_clicked));

  g_signal_emit_by_name(banner.gobj(), "button-clicked");
  g_assert_true(button_clicked_count == 1);

  g_signal_emit_by_name(banner.gobj(), "button-clicked");
  g_assert_true(button_clicked_count == 2);
}

static void test_adw_banner_new() {
  Adw::Banner banner("Hello");
  g_assert_true(banner.get_title() == "Hello");
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Banner/title", test_adw_banner_title);
  g_test_add_func("/Adwaita/Banner/button_label", test_adw_banner_button_label);
  g_test_add_func("/Adwaita/Banner/revealed", test_adw_banner_revealed);
  g_test_add_func("/Adwaita/Banner/use_markup", test_adw_banner_use_markup);
  g_test_add_func("/Adwaita/Banner/button_style", test_adw_banner_button_style);
  g_test_add_func("/Adwaita/Banner/button_clicked", test_adw_banner_button_clicked);
  g_test_add_func("/Adwaita/Banner/new", test_adw_banner_new);

  return g_test_run();
}
