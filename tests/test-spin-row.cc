/*
 * Copyright (C) 2022 Christopher Davis <christopherdavis@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <cfloat>

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_spin_row_new_with_range(void) {
  Adw::SpinRow row(0, 100, 1);

  Glib::RefPtr<Gtk::Adjustment> adjustment = row.get_adjustment();
  g_assert_true(adjustment != nullptr);

  g_assert_true(G_APPROX_VALUE(adjustment->get_lower(), 0, DBL_EPSILON));
  g_assert_true(G_APPROX_VALUE(adjustment->get_upper(), 100, DBL_EPSILON));
  g_assert_true(
      G_APPROX_VALUE(adjustment->get_step_increment(), 1, DBL_EPSILON));
  g_assert_true(
      G_APPROX_VALUE(adjustment->get_page_increment(), 10, DBL_EPSILON));
}

static void test_adw_spin_row_configure(void) {
  Adw::SpinRow row(0, 1, 1);
  Glib::RefPtr<Gtk::Adjustment> adjustment =
      Gtk::Adjustment::create(50, 0, 100, 1, 10, 0);

  row.configure(adjustment, 2, 2);

  g_assert_true(G_APPROX_VALUE(row.get_value(), 50, DBL_EPSILON));
  g_assert_true(G_APPROX_VALUE(row.get_climb_rate(), 2, DBL_EPSILON));
  g_assert_true(row.get_digits() == 2);
  g_assert_true(row.get_adjustment() == adjustment);
}

static void test_adw_spin_row_set_range(void) {
  Adw::SpinRow row(1, 2, 1);

  g_assert_true(G_APPROX_VALUE(row.get_value(), 1, DBL_EPSILON));

  row.set_range(2, 3);

  g_assert_true(G_APPROX_VALUE(row.get_value(), 2, DBL_EPSILON));
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/SpinRow/new_with_range",
                  test_adw_spin_row_new_with_range);
  g_test_add_func("/Adwaita/SpinRow/configure", test_adw_spin_row_configure);
  g_test_add_func("/Adwaita/SpinRow/set_range", test_adw_spin_row_set_range);

  return g_test_run();
}
