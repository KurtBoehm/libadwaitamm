/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

static void test_adw_clamp_child(void) {
  Adw::Clamp clamp;
  Gtk::Label child;

  g_assert_true(clamp.get_child() == nullptr);

  clamp.set_child(&child);
  g_assert_true(clamp.get_child()->gobj() == (GtkWidget *)child.gobj());

  clamp.set_child(nullptr);
  g_assert_true(clamp.get_child() == nullptr);
}

static void test_adw_clamp_maximum_size(void) {
  Adw::Clamp clamp;

  notified = 0;
  clamp.property_maximum_size().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  clamp.set_maximum_size(100);
  g_assert_true(clamp.get_maximum_size() == 100);
  g_assert_true(notified == 1);

  clamp.set_property<int>("maximum-size", 200);
  g_assert_true(clamp.get_property<int>("maximum-size") == 200);
  g_assert_true(notified == 2);

  clamp.set_maximum_size(200);
  g_assert_true(notified == 2);
}

static void test_adw_clamp_tightening_threshold(void) {
  Adw::Clamp clamp;

  notified = 0;
  clamp.property_tightening_threshold().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  clamp.set_tightening_threshold(50);
  g_assert_true(clamp.get_tightening_threshold() == 50);
  g_assert_true(notified == 1);

  clamp.set_property<int>("tightening-threshold", 150);
  g_assert_true(clamp.get_property<int>("tightening-threshold") == 150);
  g_assert_true(notified == 2);

  clamp.set_tightening_threshold(150);
  g_assert_true(notified == 2);
}

static void test_adw_clamp_unit(void) {
  Adw::Clamp clamp;

  notified = 0;
  clamp.property_unit().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(clamp.get_unit() == Adw::LengthUnit::SP);

  clamp.set_unit(Adw::LengthUnit::PX);
  g_assert_true(clamp.get_unit() == Adw::LengthUnit::PX);
  g_assert_true(notified == 1);

  clamp.set_unit(Adw::LengthUnit::PX);
  g_assert_true(notified == 1);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Clamp/child", test_adw_clamp_child);
  g_test_add_func("/Adwaita/Clamp/maximum_size", test_adw_clamp_maximum_size);
  g_test_add_func("/Adwaita/Clamp/tightening_threshold",
                  test_adw_clamp_tightening_threshold);
  g_test_add_func("/Adwaita/Clamp/unit", test_adw_clamp_unit);
  return g_test_run();
}
