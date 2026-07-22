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

static void test_adw_overlay_split_view_sidebar() {
  Adw::OverlaySplitView split_view;
  Gtk::Widget* widget = nullptr;

  notified = 0;
  split_view.property_sidebar().signal_changed().connect(sigc::ptr_fun(notify_cb));

  widget = split_view.get_property<Gtk::Widget*>("sidebar");
  g_assert_null(widget);

  split_view.set_sidebar(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  split_view.set_sidebar(widget);
  g_assert_true(split_view.get_sidebar()->gobj() == widget->gobj());
  g_assert_true(notified == 1);

  split_view.set_property<Gtk::Widget*>("sidebar", nullptr);
  g_assert_null(split_view.get_sidebar());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_content() {
  Adw::OverlaySplitView split_view;
  Gtk::Widget* widget = nullptr;

  notified = 0;
  split_view.property_content().signal_changed().connect(sigc::ptr_fun(notify_cb));

  widget = split_view.get_property<Gtk::Widget*>("content");
  g_assert_null(widget);

  split_view.set_content(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  split_view.set_content(widget);
  g_assert_true(split_view.get_content()->gobj() == widget->gobj());
  g_assert_true(notified == 1);

  split_view.set_property<Gtk::Widget*>("content", nullptr);
  g_assert_null(split_view.get_content());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_collapsed() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_collapsed().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_false(split_view.get_collapsed());

  split_view.set_collapsed(false);
  g_assert_true(notified == 0);

  split_view.set_collapsed(true);
  g_assert_true(split_view.get_collapsed());
  g_assert_true(notified == 1);

  split_view.set_property<bool>("collapsed", false);
  g_assert_false(split_view.get_collapsed());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_sidebar_position() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_sidebar_position().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_sidebar_position() == Gtk::PackType::START);

  split_view.set_sidebar_position(Gtk::PackType::START);
  g_assert_true(notified == 0);

  split_view.set_sidebar_position(Gtk::PackType::END);
  g_assert_true(split_view.get_sidebar_position() == Gtk::PackType::END);
  g_assert_true(notified == 1);

  split_view.set_property<Gtk::PackType>("sidebar-position", Gtk::PackType::START);
  g_assert_true(split_view.get_sidebar_position() == Gtk::PackType::START);
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_show_sidebar() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_show_sidebar().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_show_sidebar());

  split_view.set_show_sidebar(true);
  g_assert_true(notified == 0);

  split_view.set_show_sidebar(false);
  g_assert_false(split_view.get_show_sidebar());
  g_assert_true(notified == 1);

  split_view.set_property<bool>("show-sidebar", true);
  g_assert_true(split_view.get_show_sidebar());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_pin_sidebar() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_pin_sidebar().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_false(split_view.get_pin_sidebar());

  split_view.set_pin_sidebar(false);
  g_assert_true(notified == 0);

  split_view.set_pin_sidebar(true);
  g_assert_true(split_view.get_pin_sidebar());
  g_assert_true(notified == 1);

  split_view.set_property<bool>("pin-sidebar", false);
  g_assert_false(split_view.get_pin_sidebar());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_enable_show_gesture() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_enable_show_gesture().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_enable_show_gesture());

  split_view.set_enable_show_gesture(true);
  g_assert_true(notified == 0);

  split_view.set_enable_show_gesture(false);
  g_assert_false(split_view.get_enable_show_gesture());
  g_assert_true(notified == 1);

  split_view.set_property<bool>("enable-show-gesture", true);
  g_assert_true(split_view.get_enable_show_gesture());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_enable_hide_gesture() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_enable_hide_gesture().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_enable_hide_gesture());

  split_view.set_enable_hide_gesture(true);
  g_assert_true(notified == 0);

  split_view.set_enable_hide_gesture(false);
  g_assert_false(split_view.get_enable_hide_gesture());
  g_assert_true(notified == 1);

  split_view.set_property<bool>("enable-hide-gesture", true);
  g_assert_true(split_view.get_enable_hide_gesture());
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_min_sidebar_width() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_min_sidebar_width().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_cmpfloat_with_epsilon(split_view.get_min_sidebar_width(), 180, 1e-9);

  split_view.set_min_sidebar_width(180);
  g_assert_true(notified == 0);

  split_view.set_min_sidebar_width(200);
  g_assert_cmpfloat_with_epsilon(split_view.get_min_sidebar_width(), 200, 1e-9);
  g_assert_true(notified == 1);

  split_view.set_property<double>("min-sidebar-width", 180.0);
  g_assert_cmpfloat_with_epsilon(split_view.get_min_sidebar_width(), 180, 1e-9);
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_max_sidebar_width() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_max_sidebar_width().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_cmpfloat_with_epsilon(split_view.get_max_sidebar_width(), 280, 1e-9);

  split_view.set_max_sidebar_width(280);
  g_assert_true(notified == 0);

  split_view.set_max_sidebar_width(200);
  g_assert_cmpfloat_with_epsilon(split_view.get_max_sidebar_width(), 200, 1e-9);
  g_assert_true(notified == 1);

  split_view.set_property<double>("max-sidebar-width", 280.0);
  g_assert_cmpfloat_with_epsilon(split_view.get_max_sidebar_width(), 280, 1e-9);
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_sidebar_width_fraction() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_sidebar_width_fraction().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_cmpfloat_with_epsilon(split_view.get_sidebar_width_fraction(), 0.25, 1e-9);

  split_view.set_sidebar_width_fraction(0.25);
  g_assert_true(notified == 0);

  split_view.set_sidebar_width_fraction(0.2);
  g_assert_cmpfloat_with_epsilon(split_view.get_sidebar_width_fraction(), 0.2, 1e-9);
  g_assert_true(notified == 1);

  split_view.set_property<double>("sidebar-width-fraction", 0.25);
  g_assert_cmpfloat_with_epsilon(split_view.get_sidebar_width_fraction(), 0.25, 1e-9);
  g_assert_true(notified == 2);
}

static void test_adw_overlay_split_view_sidebar_width_unit() {
  Adw::OverlaySplitView split_view;

  notified = 0;
  split_view.property_sidebar_width_unit().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_sidebar_width_unit() == Adw::LengthUnit::SP);

  split_view.set_sidebar_width_unit(Adw::LengthUnit::SP);
  g_assert_true(notified == 0);

  split_view.set_sidebar_width_unit(Adw::LengthUnit::PX);
  g_assert_true(split_view.get_sidebar_width_unit() == Adw::LengthUnit::PX);
  g_assert_true(notified == 1);

  split_view.set_property<Adw::LengthUnit>("sidebar-width-unit", Adw::LengthUnit::SP);
  g_assert_true(split_view.get_sidebar_width_unit() == Adw::LengthUnit::SP);
  g_assert_true(notified == 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/OverlaySplitView/sidebar", test_adw_overlay_split_view_sidebar);
  g_test_add_func("/Adwaita/OverlaySplitView/content", test_adw_overlay_split_view_content);
  g_test_add_func("/Adwaita/OverlaySplitView/collapsed", test_adw_overlay_split_view_collapsed);
  g_test_add_func("/Adwaita/OverlaySplitView/sidebar_position",
                  test_adw_overlay_split_view_sidebar_position);
  g_test_add_func("/Adwaita/OverlaySplitView/show_sidebar",
                  test_adw_overlay_split_view_show_sidebar);
  g_test_add_func("/Adwaita/OverlaySplitView/pin_sidebar", test_adw_overlay_split_view_pin_sidebar);
  g_test_add_func("/Adwaita/OverlaySplitView/enable_show_gesture",
                  test_adw_overlay_split_view_enable_show_gesture);
  g_test_add_func("/Adwaita/OverlaySplitView/enable_hide_gesture",
                  test_adw_overlay_split_view_enable_hide_gesture);
  g_test_add_func("/Adwaita/OverlaySplitView/min_sidebar_width",
                  test_adw_overlay_split_view_min_sidebar_width);
  g_test_add_func("/Adwaita/OverlaySplitView/max_sidebar_width",
                  test_adw_overlay_split_view_max_sidebar_width);
  g_test_add_func("/Adwaita/OverlaySplitView/sidebar_width_fraction",
                  test_adw_overlay_split_view_sidebar_width_fraction);
  g_test_add_func("/Adwaita/OverlaySplitView/sidebar_width_unit",
                  test_adw_overlay_split_view_sidebar_width_unit);

  return g_test_run();
}
