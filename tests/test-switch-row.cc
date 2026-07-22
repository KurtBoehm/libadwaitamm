/*
 * Copyright (C) 2023 Joshua Lee <lee.son.wai@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-late
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() {
  ++notified;
}

static void test_adw_switch_row_active() {
  Adw::SwitchRow row;

  notified = 0;
  row.property_active().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_false(row.get_active());

  row.set_active(!row.get_active());
  g_assert_true(notified == 1);

  g_assert_true(row.get_active());
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/SwitchRow/active", test_adw_switch_row_active);

  return g_test_run();
}
