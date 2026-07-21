/*
 * Copyright (C) 2018 Purism SPC
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

static void test_adw_expander_row_add_remove(void) {
  Adw::ExpanderRow row;
  Gtk::ListBoxRow child;

  row.add_row(child);
  row.remove(child);
}

static void test_adw_expander_row_add_prefix_suffix(void) {
  Adw::ExpanderRow row;
  Gtk::Button prefix, suffix;

  row.add_prefix(prefix);
  row.add_suffix(suffix);

  row.remove(prefix);
  row.remove(suffix);
}

static void test_adw_expander_row_add_action(void) {
  Adw::ExpanderRow row;
  Gtk::Button action;

  // Deprecated since 1.4 in favor of add_suffix(), but still part of the
  // public API and should keep working.
  row.add_action(action);
  row.remove(action);
}

static void test_adw_expander_row_subtitle(void) {
  Adw::ExpanderRow row;
  g_assert_true(row.get_subtitle() == "");

  row.set_subtitle("Dummy subtitle");
  g_assert_true(row.get_subtitle() == "Dummy subtitle");

  row.set_use_markup(false);
  row.set_subtitle("Invalid <b>markup");
  g_assert_true(row.get_subtitle() == "Invalid <b>markup");
}

static void test_adw_expander_row_icon_name(void) {
  Adw::ExpanderRow row;
  g_assert_true(row.get_icon_name() == "");

  row.set_icon_name("dummy-icon-name");
  g_assert_true(row.get_icon_name() == "dummy-icon-name");
}

static void test_adw_expander_row_expanded(void) {
  Adw::ExpanderRow row;
  g_assert_false(row.get_expanded());

  row.set_expanded(true);
  g_assert_true(row.get_expanded());

  row.set_expanded(false);
  g_assert_false(row.get_expanded());
}

static void test_adw_expander_row_enable_expansion(void) {
  Adw::ExpanderRow row;

  g_assert_true(row.get_enable_expansion());
  g_assert_false(row.get_expanded());

  row.set_expanded(true);
  g_assert_true(row.get_expanded());

  row.set_enable_expansion(false);
  g_assert_false(row.get_enable_expansion());
  g_assert_false(row.get_expanded());

  row.set_expanded(true);
  g_assert_false(row.get_expanded());

  row.set_enable_expansion(true);
  g_assert_true(row.get_enable_expansion());
  g_assert_true(row.get_expanded());
}

static void test_adw_expander_row_title_lines(void) {
  Adw::ExpanderRow row;

  notified = 0;
  row.property_title_lines().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_cmpint(row.get_title_lines(), ==, 0);

  g_test_expect_message(
      ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
      "adw_action_row_set_title_lines: assertion 'title_lines >= 0' failed");
  row.set_title_lines(-1);
  g_test_assert_expected_messages();

  g_assert_cmpint(row.get_title_lines(), ==, 0);

  row.set_title_lines(0);
  g_assert_true(notified == 0);

  row.set_title_lines(3);
  g_assert_cmpint(row.get_title_lines(), ==, 3);
  g_assert_true(notified == 1);
}

static void test_adw_expander_row_subtitle_lines(void) {
  Adw::ExpanderRow row;

  notified = 0;
  row.property_subtitle_lines().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_cmpint(row.get_subtitle_lines(), ==, 0);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "adw_action_row_set_subtitle_lines: assertion "
                        "'subtitle_lines >= 0' failed");
  row.set_subtitle_lines(-1);
  g_test_assert_expected_messages();

  g_assert_cmpint(row.get_subtitle_lines(), ==, 0);

  row.set_subtitle_lines(0);
  g_assert_true(notified == 0);

  row.set_subtitle_lines(2);
  g_assert_cmpint(row.get_subtitle_lines(), ==, 2);
  g_assert_true(notified == 1);
}

static void test_adw_expander_row_show_enable_switch(void) {
  Adw::ExpanderRow row;

  g_assert_false(row.get_show_enable_switch());

  row.set_show_enable_switch(true);
  g_assert_true(row.get_show_enable_switch());

  row.set_show_enable_switch(false);
  g_assert_false(row.get_show_enable_switch());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ExpanderRow/add_remove",
                  test_adw_expander_row_add_remove);
  g_test_add_func("/Adwaita/ExpanderRow/add_prefix_suffix",
                  test_adw_expander_row_add_prefix_suffix);
  g_test_add_func("/Adwaita/ExpanderRow/add_action",
                  test_adw_expander_row_add_action);
  g_test_add_func("/Adwaita/ExpanderRow/subtitle",
                  test_adw_expander_row_subtitle);
  g_test_add_func("/Adwaita/ExpanderRow/icon_name",
                  test_adw_expander_row_icon_name);
  g_test_add_func("/Adwaita/ExpanderRow/expanded",
                  test_adw_expander_row_expanded);
  g_test_add_func("/Adwaita/ExpanderRow/enable_expansion",
                  test_adw_expander_row_enable_expansion);
  g_test_add_func("/Adwaita/ExpanderRow/title_lines",
                  test_adw_expander_row_title_lines);
  g_test_add_func("/Adwaita/ExpanderRow/subtitle_lines",
                  test_adw_expander_row_subtitle_lines);
  g_test_add_func("/Adwaita/ExpanderRow/show_enable_switch",
                  test_adw_expander_row_show_enable_switch);

  return g_test_run();
}
