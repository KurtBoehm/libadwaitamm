/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

static void test_adw_bottom_sheet_content(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_content().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_null(sheet.get_content());

  auto widget = Gtk::make_managed<Gtk::Button>();
  sheet.set_content(widget);
  g_assert_true(sheet.get_content() == widget);
  g_assert_true(notified == 1);

  sheet.set_content(nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_bottom_sheet_sheet(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_sheet().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_null(sheet.get_sheet());

  auto widget = Gtk::make_managed<Gtk::Button>();
  sheet.set_sheet(widget);
  g_assert_true(sheet.get_sheet() == widget);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_bottom_bar(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_bottom_bar().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_null(sheet.get_bottom_bar());

  auto widget = Gtk::make_managed<Gtk::Button>();
  sheet.set_bottom_bar(widget);
  g_assert_true(sheet.get_bottom_bar() == widget);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_open(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_open().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool orig = sheet.get_open();
  sheet.set_open(orig);
  g_assert_true(notified == 0);

  sheet.set_open(!orig);
  g_assert_true(sheet.get_open() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_align(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_align().signal_changed().connect(sigc::ptr_fun(notify_cb));

  float orig = sheet.get_align();
  float other = (orig == 0.0f) ? 1.0f : 0.0f;

  sheet.set_align(orig);
  g_assert_true(notified == 0);

  sheet.set_align(other);
  g_assert_true(sheet.get_align() == other);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_full_width(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_full_width().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool orig = sheet.get_full_width();
  sheet.set_full_width(!orig);
  g_assert_true(sheet.get_full_width() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_show_drag_handle(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_show_drag_handle().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  // There is an overlaid drag handle by default.
  g_assert_true(sheet.get_show_drag_handle());

  sheet.set_show_drag_handle(true);
  g_assert_true(notified == 0);

  sheet.set_show_drag_handle(false);
  g_assert_false(sheet.get_show_drag_handle());
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_modal(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_modal().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool orig = sheet.get_modal();
  sheet.set_modal(!orig);
  g_assert_true(sheet.get_modal() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_can_open(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_can_open().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool orig = sheet.get_can_open();
  sheet.set_can_open(!orig);
  g_assert_true(sheet.get_can_open() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_can_close(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_can_close().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool orig = sheet.get_can_close();
  sheet.set_can_close(!orig);
  g_assert_true(sheet.get_can_close() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_reveal_bottom_bar(void) {
  Adw::BottomSheet sheet;

  notified = 0;
  sheet.property_reveal_bottom_bar().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool orig = sheet.get_reveal_bottom_bar();
  sheet.set_reveal_bottom_bar(!orig);
  g_assert_true(sheet.get_reveal_bottom_bar() == !orig);
  g_assert_true(notified == 1);
}

static void test_adw_bottom_sheet_close_attempt(void) {
  Adw::BottomSheet sheet;

  int close_attempt_count = 0;
  sheet.signal_close_attempt().connect(
      [&close_attempt_count]() { close_attempt_count++; });

  g_signal_emit_by_name(sheet.gobj(), "close-attempt");
  g_assert_true(close_attempt_count == 1);
}

static void test_adw_bottom_sheet_heights(void) {
  Adw::BottomSheet sheet;

  // Read-only, but should be well-defined even with no sheet/bottom bar set.
  g_assert_true(sheet.get_sheet_height() >= 0);
  g_assert_true(sheet.get_bottom_bar_height() >= 0);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/BottomSheet/content",
                  test_adw_bottom_sheet_content);
  g_test_add_func("/Adwaita/BottomSheet/sheet", test_adw_bottom_sheet_sheet);
  g_test_add_func("/Adwaita/BottomSheet/bottom_bar",
                  test_adw_bottom_sheet_bottom_bar);
  g_test_add_func("/Adwaita/BottomSheet/open", test_adw_bottom_sheet_open);
  g_test_add_func("/Adwaita/BottomSheet/align", test_adw_bottom_sheet_align);
  g_test_add_func("/Adwaita/BottomSheet/full_width",
                  test_adw_bottom_sheet_full_width);
  g_test_add_func("/Adwaita/BottomSheet/show_drag_handle",
                  test_adw_bottom_sheet_show_drag_handle);
  g_test_add_func("/Adwaita/BottomSheet/modal", test_adw_bottom_sheet_modal);
  g_test_add_func("/Adwaita/BottomSheet/can_open",
                  test_adw_bottom_sheet_can_open);
  g_test_add_func("/Adwaita/BottomSheet/can_close",
                  test_adw_bottom_sheet_can_close);
  g_test_add_func("/Adwaita/BottomSheet/reveal_bottom_bar",
                  test_adw_bottom_sheet_reveal_bottom_bar);
  g_test_add_func("/Adwaita/BottomSheet/close_attempt",
                  test_adw_bottom_sheet_close_attempt);
  g_test_add_func("/Adwaita/BottomSheet/heights",
                  test_adw_bottom_sheet_heights);

  return g_test_run();
}
