/*
 * Copyright (C) 2022 Purism SPC
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alice Mikhaylenko <alice.mikhaylenko@puri.sm>
 * Author: Kurt Böhm <kurbo96@gmail.com>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() {
  ++notified;
}

static void test_adw_tab_button_view() {
  Adw::TabButton button;

  notified = 0;
  button.property_view().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::TabView* view = button.get_property<Adw::TabView*>("view");
  g_assert_true(view == nullptr);

  button.set_view(nullptr);
  g_assert_true(notified == 0);

  view = Gtk::make_managed<Adw::TabView>();
  button.set_view(view);
  g_assert_true(button.get_view() == view);
  g_assert_true(notified == 1);

  button.set_property<Adw::TabView*>("view", nullptr);
  g_assert_true(button.get_view() == nullptr);
  g_assert_true(notified == 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/TabButton/view", test_adw_tab_button_view);

  return g_test_run();
}
