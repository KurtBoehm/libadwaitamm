/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_enum_list_model_enum_type() {
  auto model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);

  g_assert_true(model->get_enum_type() == GTK_TYPE_ORIENTATION);
}

static void test_adw_enum_list_model_find_position() {
  auto model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);

  // GtkOrientation: HORIZONTAL = 0, VERTICAL = 1
  g_assert_cmpuint(model->find_position(GTK_ORIENTATION_HORIZONTAL), ==, 0);
  g_assert_cmpuint(model->find_position(GTK_ORIENTATION_VERTICAL), ==, 1);
}

static void test_adw_enum_list_model_n_items() {
  auto model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);

  // Exercises the inherited Gio::ListModel method, present since introduction.
  g_assert_cmpuint(model->get_n_items(), ==, 2);

  // Exercises the AdwEnumListModel:n-items property override, added in 1.9,
  // via generic property access so this doesn't depend on whether a typed
  // property_n_items() accessor exists in giomm's ListModel interface wrap.
  guint n_items = model->get_property<guint>("n-items");
  g_assert_cmpuint(n_items, ==, 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/EnumListModel/enum_type", test_adw_enum_list_model_enum_type);
  g_test_add_func("/Adwaita/EnumListModel/find_position", test_adw_enum_list_model_find_position);
  g_test_add_func("/Adwaita/EnumListModel/n_items", test_adw_enum_list_model_n_items);

  return g_test_run();
}
