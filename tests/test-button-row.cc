/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

int activated_count;
static void on_activated() { activated_count++; }

static void test_adw_button_row_start_icon_name(void) {
  Adw::ButtonRow row;

  notified = 0;
  row.property_start_icon_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring name = row.get_property<Glib::ustring>("start-icon-name");
  g_assert_true(name == "");

  row.set_start_icon_name("");
  g_assert_true(notified == 0);

  row.set_start_icon_name("document-open-symbolic");
  g_assert_true(row.get_start_icon_name() == "document-open-symbolic");
  g_assert_true(notified == 1);
}

static void test_adw_button_row_end_icon_name(void) {
  Adw::ButtonRow row;

  notified = 0;
  row.property_end_icon_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring name = row.get_property<Glib::ustring>("end-icon-name");
  g_assert_true(name == "");

  row.set_end_icon_name("");
  g_assert_true(notified == 0);

  row.set_end_icon_name("go-next-symbolic");
  g_assert_true(row.get_end_icon_name() == "go-next-symbolic");
  g_assert_true(notified == 1);
}

static void test_adw_button_row_activated(void) {
  Adw::ButtonRow row;

  activated_count = 0;
  row.signal_activated().connect(sigc::ptr_fun(on_activated));

  g_signal_emit_by_name(row.gobj(), "activated");
  g_assert_true(activated_count == 1);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ButtonRow/start_icon_name",
                  test_adw_button_row_start_icon_name);
  g_test_add_func("/Adwaita/ButtonRow/end_icon_name",
                  test_adw_button_row_end_icon_name);
  g_test_add_func("/Adwaita/ButtonRow/activated",
                  test_adw_button_row_activated);

  return g_test_run();
}
