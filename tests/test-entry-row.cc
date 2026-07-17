/*
 * Copyright (C) 2022 Purism SPC
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 * Author: Kurt Böhm <kurbo96@gmail.com>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_entry_row_add_remove(void) {
  Adw::EntryRow row;

  Gtk::Button prefix, suffix;

  row.add_prefix(prefix);
  row.add_suffix(suffix);

  row.remove(prefix);
  row.remove(suffix);
}

static void test_adw_entry_row_show_apply_button(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_show_apply_button().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool show_apply_button = row.get_property<bool>("show-apply-button");
  g_assert_false(show_apply_button);

  row.set_show_apply_button(false);
  g_assert_true(notified == 0);

  row.set_show_apply_button(true);
  g_assert_true(row.get_show_apply_button());
  g_assert_true(notified == 1);

  row.set_property<bool>("show-apply-button", false);
  g_assert_false(row.get_show_apply_button());
  g_assert_true(notified == 2);
}

static void test_adw_entry_row_activates_default(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_activates_default().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(row.get_activates_default());

  row.set_activates_default(false);
  g_assert_true(notified == 0);

  row.set_activates_default(true);
  g_assert_true(row.get_activates_default());
  g_assert_true(notified == 1);

  row.set_property<bool>("activates-default", false);
  g_assert_false(row.get_activates_default());
  g_assert_true(notified == 2);
}

static void test_adw_entry_row_input_purpose(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_input_purpose().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(row.get_input_purpose() == Gtk::InputPurpose::FREE_FORM);

  row.set_input_purpose(Gtk::InputPurpose::FREE_FORM);
  g_assert_true(notified == 0);

  row.set_input_purpose(Gtk::InputPurpose::EMAIL);
  g_assert_true(row.get_input_purpose() == Gtk::InputPurpose::EMAIL);
  g_assert_true(notified == 1);
}

static void test_adw_entry_row_input_hints(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_input_hints().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(row.get_input_hints() == Gtk::InputHints::NONE);

  row.set_input_hints(Gtk::InputHints::NONE);
  g_assert_true(notified == 0);

  row.set_input_hints(Gtk::InputHints::LOWERCASE);
  g_assert_true(row.get_input_hints() == Gtk::InputHints::LOWERCASE);
  g_assert_true(notified == 1);
}

static void test_adw_entry_row_enable_emoji_completion(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_enable_emoji_completion().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(row.get_enable_emoji_completion());

  row.set_enable_emoji_completion(false);
  g_assert_true(notified == 0);

  row.set_enable_emoji_completion(true);
  g_assert_true(row.get_enable_emoji_completion());
  g_assert_true(notified == 1);
}

static void test_adw_entry_row_attributes(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_attributes().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Pango::AttrList attrs;
  row.set_attributes(attrs);
  g_assert_true(notified == 1);

  // Content equality on Pango::AttrList isn't cheap to assert generically;
  // just confirm the getter round-trips without crashing.
  Pango::AttrList roundtrip = row.get_attributes();
  (void)roundtrip;
}

static void test_adw_entry_row_max_length(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_max_length().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_cmpint(row.get_max_length(), ==, 0);

  row.set_max_length(0);
  g_assert_true(notified == 0);

  row.set_max_length(10);
  g_assert_cmpint(row.get_max_length(), ==, 10);

  // NOTE: AdwEntryRow:max-length is G_PARAM_EXPLICIT_NOTIFY, and as of
  // libadwaita 1.9.2, adw_entry_row_set_max_length() does not appear to emit
  // notify::max-length at all. Confirmed by connecting directly to the GObject
  // signal (bypassing glibmm's property proxy entirely) and still seeing zero
  // notifications on a real 0 -> 10 value change. Until this is fixed, this
  // test only checks value round-tripping, not change notification.
  // g_assert_true(notified == 1);
}

static void test_adw_entry_row_text_length(void) {
  Adw::EntryRow row;

  notified = 0;
  row.property_text_length().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_cmpuint(row.get_text_length(), ==, 0);

  row.set_text("hello");
  g_assert_cmpuint(row.get_text_length(), ==, 5);
  g_assert_true(notified >= 1);
}

static void test_adw_entry_row_grab_focus_without_selecting(void) {
  Gtk::Window window;
  Adw::EntryRow row;

  window.set_child(row);

  row.grab_focus_without_selecting();
}

static void test_adw_entry_row_signals(void) {
  Adw::EntryRow row;

  int apply_count = 0;
  int activated_count = 0;
  row.signal_apply().connect([&apply_count]() { ++apply_count; });
  row.signal_entry_activated().connect(
      [&activated_count]() { ++activated_count; });

  g_signal_emit_by_name(row.gobj(), "apply");
  g_assert_true(apply_count == 1);

  g_signal_emit_by_name(row.gobj(), "entry-activated");
  g_assert_true(activated_count == 1);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/EntryRow/add_remove",
                  test_adw_entry_row_add_remove);
  g_test_add_func("/Adwaita/EntryRow/show_apply_button",
                  test_adw_entry_row_show_apply_button);
  g_test_add_func("/Adwaita/EntryRow/activates_default",
                  test_adw_entry_row_activates_default);
  g_test_add_func("/Adwaita/EntryRow/input_purpose",
                  test_adw_entry_row_input_purpose);
  g_test_add_func("/Adwaita/EntryRow/input_hints",
                  test_adw_entry_row_input_hints);
  g_test_add_func("/Adwaita/EntryRow/enable_emoji_completion",
                  test_adw_entry_row_enable_emoji_completion);
  g_test_add_func("/Adwaita/EntryRow/attributes",
                  test_adw_entry_row_attributes);
  g_test_add_func("/Adwaita/EntryRow/max_length",
                  test_adw_entry_row_max_length);
  g_test_add_func("/Adwaita/EntryRow/text_length",
                  test_adw_entry_row_text_length);
  g_test_add_func("/Adwaita/EntryRow/grab_focus_without_selecting",
                  test_adw_entry_row_grab_focus_without_selecting);
  g_test_add_func("/Adwaita/EntryRow/signals", test_adw_entry_row_signals);

  return g_test_run();
}
