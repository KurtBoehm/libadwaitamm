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

static void test_adw_combo_row_set_for_enum(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gtk::Expression<Glib::ustring>> expr;
  Glib::RefPtr<Gio::ListModel> model;
  Glib::RefPtr<Adw::EnumListItem> item;

  g_assert_true(row.get_model() == nullptr);

  expr = Gtk::PropertyExpression<Glib::ustring>::create(
      Adw::EnumListItem::get_type(), "nick");
  row.set_expression(expr);

  // Enums don't have a way to retrieve its GType in gtkmm.
  model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);
  row.set_model(model);

  model = row.get_model();
  g_assert_true(model != nullptr);

  g_assert_true(model->get_n_items() == 2);

  item = std::dynamic_pointer_cast<Adw::EnumListItem>(model->get_object(0));
  g_assert_true(item != nullptr);
  g_assert_true(item->get_nick() == "horizontal");

  item = std::dynamic_pointer_cast<Adw::EnumListItem>(model->get_object(1));
  g_assert_true(item != nullptr);
  g_assert_true(item->get_nick() == "vertical");
}

static void test_adw_combo_row_model(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gio::ListModel> model;

  notified = 0;
  row.property_model().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(row.get_model() == nullptr);

  model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);
  row.set_model(model);
  g_assert_true(row.get_model() != nullptr);
  g_assert_true(notified == 1);

  row.set_model(nullptr);
  g_assert_true(row.get_model() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_combo_row_selected(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gio::ListModel> model;
  int selected = 0;

  notified = 0;
  row.property_selected().signal_changed().connect(sigc::ptr_fun(notify_cb));

  selected = row.get_property<int>("selected");
  g_assert_true(selected == -1);

  row.set_selected(-1);
  g_assert_true(notified == 0);

  // Enums don't have a way to retrieve its GType in gtkmm.
  model = Adw::EnumListModel::create(GTK_TYPE_SELECTION_MODE);
  row.set_model(model);

  g_assert_true(row.get_selected() == 0);
  g_assert_true(notified == 1);

  row.set_selected(3);
  g_assert_true(row.get_selected() == 3);
  g_assert_true(notified == 2);

  row.set_property<int>("selected", 1);
  g_assert_true(row.get_selected() == 1);
  g_assert_true(notified == 3);
}

static void test_adw_combo_row_selected_item(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gio::ListModel> model;

  model = Adw::EnumListModel::create(GTK_TYPE_ORIENTATION);
  row.set_model(model);

  g_assert_true(row.get_selected() == 0);
  auto item = row.get_selected_item();
  g_assert_true(item != nullptr);

  row.set_selected(1);
  auto item2 = row.get_selected_item();
  g_assert_true(item2 != nullptr);
  g_assert_true(item->gobj() != item2->gobj());
}

static void test_adw_combo_row_factory(void) {
  Adw::ComboRow row;
  Glib::RefPtr<Gtk::ListItemFactory> initial = row.get_factory();

  notified = 0;
  row.property_factory().signal_changed().connect(sigc::ptr_fun(notify_cb));

  auto factory = Gtk::SignalListItemFactory::create();
  row.set_factory(factory);
  g_assert_true(row.get_factory()->gobj() ==
                (GtkListItemFactory *)factory->gobj());
  g_assert_true(notified == 1);

  row.set_factory(nullptr);
  g_assert_true(row.get_factory() != factory);
  g_assert_true(notified == 2);
}

static void test_adw_combo_row_header_factory(void) {
  Adw::ComboRow row;

  notified = 0;
  row.property_header_factory().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(row.get_header_factory() == nullptr);

  auto factory = Gtk::SignalListItemFactory::create();
  row.set_header_factory(factory);
  g_assert_true(row.get_header_factory()->gobj() ==
                (GtkListItemFactory *)factory->gobj());
  g_assert_true(notified == 1);
}

static void test_adw_combo_row_list_factory(void) {
  Adw::ComboRow row;

  notified = 0;
  row.property_list_factory().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(row.get_list_factory() == nullptr);

  auto factory = Gtk::SignalListItemFactory::create();
  row.set_list_factory(factory);
  g_assert_true(row.get_list_factory()->gobj() ==
                (GtkListItemFactory *)factory->gobj());
  g_assert_true(notified == 1);
}

static void test_adw_combo_row_use_subtitle(void) {
  Adw::ComboRow row;

  notified = 0;
  row.property_use_subtitle().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(row.get_use_subtitle());

  row.set_use_subtitle(false);
  g_assert_true(notified == 0);

  row.set_use_subtitle(true);
  g_assert_true(row.get_use_subtitle());
  g_assert_true(notified == 1);

  row.set_property<bool>("use-subtitle", false);
  bool use_subtitle = row.get_property<bool>("use-subtitle");
  g_assert_false(use_subtitle);
  g_assert_true(notified == 2);
}

static void test_adw_combo_row_enable_search(void) {
  Adw::ComboRow row;

  notified = 0;
  row.property_enable_search().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(row.get_enable_search());
  row.set_enable_search(true);
  g_assert_true(row.get_enable_search());
  g_assert_true(notified == 1);

  row.set_property<bool>("enable-search", false);
  g_assert_false(row.get_property<bool>("enable-search"));
  g_assert_true(notified == 2);

  row.set_enable_search(false);
  g_assert_true(notified == 2);
}

static void test_adw_combo_row_search_match_mode(void) {
  Adw::ComboRow row;

  notified = 0;
  row.property_search_match_mode().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(row.get_search_match_mode() ==
                Gtk::StringFilter::MatchMode::PREFIX);
  row.set_search_match_mode(Gtk::StringFilter::MatchMode::SUBSTRING);
  g_assert_true(row.get_search_match_mode() ==
                Gtk::StringFilter::MatchMode::SUBSTRING);
  g_assert_true(notified == 1);

  row.set_search_match_mode(Gtk::StringFilter::MatchMode::SUBSTRING);
  g_assert_true(notified == 1);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ComboRow/set_for_enum",
                  test_adw_combo_row_set_for_enum);
  g_test_add_func("/Adwaita/ComboRow/model", test_adw_combo_row_model);
  g_test_add_func("/Adwaita/ComboRow/selected", test_adw_combo_row_selected);
  g_test_add_func("/Adwaita/ComboRow/selected_item",
                  test_adw_combo_row_selected_item);
  g_test_add_func("/Adwaita/ComboRow/factory", test_adw_combo_row_factory);
  g_test_add_func("/Adwaita/ComboRow/header_factory",
                  test_adw_combo_row_header_factory);
  g_test_add_func("/Adwaita/ComboRow/list_factory",
                  test_adw_combo_row_list_factory);
  g_test_add_func("/Adwaita/ComboRow/use_subtitle",
                  test_adw_combo_row_use_subtitle);
  g_test_add_func("/Adwaita/ComboRow/enable_search",
                  test_adw_combo_row_enable_search);
  g_test_add_func("/Adwaita/ComboRow/search_match_mode",
                  test_adw_combo_row_search_match_mode);

  return g_test_run();
}
