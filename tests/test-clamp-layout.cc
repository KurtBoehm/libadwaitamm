/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

static void test_adw_clamp_layout_maximum_size(void) {
  auto layout = Adw::ClampLayout::create();

  notified = 0;
  layout->property_maximum_size().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  layout->set_maximum_size(100);
  g_assert_true(layout->get_maximum_size() == 100);
  g_assert_true(notified == 1);
}

static void test_adw_clamp_layout_tightening_threshold(void) {
  auto layout = Adw::ClampLayout::create();

  notified = 0;
  layout->property_tightening_threshold().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  layout->set_tightening_threshold(50);
  g_assert_true(layout->get_tightening_threshold() == 50);
  g_assert_true(notified == 1);
}

static void test_adw_clamp_layout_unit(void) {
  auto layout = Adw::ClampLayout::create();

  notified = 0;
  layout->property_unit().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(layout->get_unit() == Adw::LengthUnit::SP);
  layout->set_unit(Adw::LengthUnit::PX);
  g_assert_true(layout->get_unit() == Adw::LengthUnit::PX);
  g_assert_true(notified == 1);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ClampLayout/maximum_size",
                  test_adw_clamp_layout_maximum_size);
  g_test_add_func("/Adwaita/ClampLayout/tightening_threshold",
                  test_adw_clamp_layout_tightening_threshold);
  g_test_add_func("/Adwaita/ClampLayout/unit", test_adw_clamp_layout_unit);
  return g_test_run();
}
