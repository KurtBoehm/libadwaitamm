/*
 * Copyright (C) 2025 GNOME Foundation Inc.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alice Mikhaylenko <alicem@gnome.org>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() {
  ++notified;
}

static void test_adw_shortcut_label_accelerator() {
  Adw::ShortcutLabel label("<Control>C");

  notified = 0;
  label.property_accelerator().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring accel = label.get_property<Glib::ustring>("accelerator");
  g_assert_true(accel == "<Control>C");
  g_assert_true(notified == 0);

  label.set_accelerator("<Control>X");
  g_assert_true(label.get_accelerator() == "<Control>X");
  g_assert_true(notified == 1);

  label.set_property<Glib::ustring>("accelerator", "<Control>C");
  g_assert_true(label.get_accelerator() == "<Control>C");
  g_assert_true(notified == 2);
}

static void test_adw_shortcut_label_disabled_text() {
  Adw::ShortcutLabel label("<Control>C");

  notified = 0;
  label.property_disabled_text().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring text = label.get_property<Glib::ustring>("disabled-text");
  g_assert_true(text == "");
  g_assert_true(notified == 0);

  label.set_disabled_text("No Shortcut");
  g_assert_true(label.get_disabled_text() == "No Shortcut");
  g_assert_true(notified == 1);

  label.set_property<Glib::ustring>("disabled-text", "Disabled");
  g_assert_true(label.get_disabled_text() == "Disabled");
  g_assert_true(notified == 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ShortcutLabel/accelerator", test_adw_shortcut_label_accelerator);
  g_test_add_func("/Adwaita/ShortcutLabel/disabled_text", test_adw_shortcut_label_disabled_text);

  return g_test_run();
}
