/*
 * Copyright (C) 2019 Purism SPC
 * Copyright (C) 2024 GNOME Foundation Inc
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_preferences_dialog_add_remove(void) {
  Adw::PreferencesDialog dialog;
  Adw::PreferencesPage page;

  dialog.add(&page);
  dialog.remove(&page);
}

static void test_adw_preferences_dialog_add_toast(void) {
  Adw::PreferencesDialog dialog;
  Adw::Toast* toast = new Adw::Toast("Test Notification");
  dialog.add_toast(toast);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PreferencesDialog/add_remove",
                  test_adw_preferences_dialog_add_remove);
  g_test_add_func("/Adwaita/PreferencesDialog/add_toast",
                  test_adw_preferences_dialog_add_toast);

  return g_test_run();
}
