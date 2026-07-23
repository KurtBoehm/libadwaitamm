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

static void test_adw_tab_overview_view() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_view().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::TabView* view = overview.get_property<Adw::TabView*>("view");
  g_assert_true(view == nullptr);

  overview.set_view(nullptr);
  g_assert_true(notified == 0);

  view = Gtk::make_managed<Adw::TabView>();
  overview.set_view(view);
  g_assert_true(overview.get_view() == view);
  g_assert_true(notified == 1);

  overview.set_property<Adw::TabView*>("view", nullptr);
  g_assert_true(overview.get_view() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_child() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_child().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* widget = overview.get_property<Gtk::Widget*>("child");
  g_assert_true(widget == nullptr);

  overview.set_child(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  overview.set_child(widget);
  g_assert_true(overview.get_child() == widget);
  g_assert_true(notified == 1);

  overview.set_property<Gtk::Widget*>("child", nullptr);
  g_assert_true(overview.get_child() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_open() {
  Adw::TabOverview overview;
  Adw::TabView* view = Gtk::make_managed<Adw::TabView>();

  view->add_page(*Gtk::make_managed<Gtk::Button>());

  overview.set_child(view);
  overview.set_view(view);

  notified = 0;
  overview.property_open().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool open = overview.get_property<bool>("open");
  g_assert_false(open);

  overview.set_open(false);
  g_assert_true(notified == 0);

  overview.set_open(true);
  g_assert_true(overview.get_open());
  g_assert_true(notified == 1);

  overview.set_property<bool>("open", false);
  g_assert_false(overview.get_open());
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_inverted() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_inverted().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool inverted = overview.get_property<bool>("inverted");
  g_assert_false(inverted);

  overview.set_inverted(false);
  g_assert_true(notified == 0);

  overview.set_inverted(true);
  g_assert_true(overview.get_inverted());
  g_assert_true(notified == 1);

  overview.set_property<bool>("inverted", false);
  g_assert_false(overview.get_inverted());
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_enable_search() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_enable_search().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool enable_search = overview.get_property<bool>("enable-search");
  g_assert_true(enable_search);

  overview.set_enable_search(true);
  g_assert_true(notified == 0);

  overview.set_enable_search(false);
  g_assert_false(overview.get_enable_search());
  g_assert_true(notified == 1);

  overview.set_property<bool>("enable-search", true);
  g_assert_true(overview.get_enable_search());
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_enable_new_tab() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_enable_new_tab().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool enable_new_tab = overview.get_property<bool>("enable-new-tab");
  g_assert_false(enable_new_tab);

  overview.set_enable_new_tab(false);
  g_assert_true(notified == 0);

  overview.set_enable_new_tab(true);
  g_assert_true(overview.get_enable_new_tab());
  g_assert_true(notified == 1);

  overview.set_property<bool>("enable-new-tab", false);
  g_assert_false(overview.get_enable_new_tab());
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_show_start_title_buttons() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_show_start_title_buttons().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool show_start_title_buttons = overview.get_property<bool>("show-start-title-buttons");
  g_assert_true(show_start_title_buttons);

  overview.set_show_start_title_buttons(true);
  g_assert_true(notified == 0);

  overview.set_show_start_title_buttons(false);
  g_assert_false(overview.get_show_start_title_buttons());
  g_assert_true(notified == 1);

  overview.set_property<bool>("show-start-title-buttons", true);
  g_assert_true(overview.get_show_start_title_buttons());
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_show_end_title_buttons() {
  Adw::TabOverview overview;

  notified = 0;
  overview.property_show_end_title_buttons().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool show_end_title_buttons = overview.get_property<bool>("show-end-title-buttons");
  g_assert_true(show_end_title_buttons);

  overview.set_show_end_title_buttons(true);
  g_assert_true(notified == 0);

  overview.set_show_end_title_buttons(false);
  g_assert_false(overview.get_show_end_title_buttons());
  g_assert_true(notified == 1);

  overview.set_property<bool>("show-end-title-buttons", true);
  g_assert_true(overview.get_show_end_title_buttons());
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_secondary_menu() {
  Adw::TabOverview overview;
  Glib::RefPtr<Gio::Menu> model1 = Gio::Menu::create();
  Glib::RefPtr<Gio::Menu> model2 = Gio::Menu::create();

  notified = 0;
  overview.property_secondary_menu().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::MenuModel> model =
    overview.get_property<Glib::RefPtr<Gio::MenuModel>>("secondary-menu");
  g_assert_true(model == nullptr);
  g_assert_true(notified == 0);

  overview.set_secondary_menu(model1);
  g_assert_true(overview.get_secondary_menu() == model1);
  g_assert_true(notified == 1);

  overview.set_property<Glib::RefPtr<Gio::MenuModel>>("secondary-menu", model2);
  g_assert_true(overview.get_secondary_menu() == model2);
  g_assert_true(notified == 2);
}

static void test_adw_tab_overview_actions() {
  Adw::TabOverview overview;
  Adw::TabView* view = Gtk::make_managed<Adw::TabView>();

  view->add_page(*Gtk::make_managed<Gtk::Button>());

  overview.set_child(view);
  overview.set_view(view);

  overview.activate_action("overview.open");

  g_assert_true(overview.get_open());

  overview.activate_action("overview.close");

  g_assert_false(overview.get_open());
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/TabOverview/view", test_adw_tab_overview_view);
  g_test_add_func("/Adwaita/TabOverview/child", test_adw_tab_overview_child);
  g_test_add_func("/Adwaita/TabOverview/open", test_adw_tab_overview_open);
  g_test_add_func("/Adwaita/TabOverview/inverted", test_adw_tab_overview_inverted);
  g_test_add_func("/Adwaita/TabOverview/enable_search", test_adw_tab_overview_enable_search);
  g_test_add_func("/Adwaita/TabOverview/enable_new_tab", test_adw_tab_overview_enable_new_tab);
  g_test_add_func("/Adwaita/TabOverview/secondary_menu", test_adw_tab_overview_secondary_menu);
  g_test_add_func("/Adwaita/TabOverview/show_start_title_buttons",
                  test_adw_tab_overview_show_start_title_buttons);
  g_test_add_func("/Adwaita/TabOverview/show_end_title_buttons",
                  test_adw_tab_overview_show_end_title_buttons);
  g_test_add_func("/Adwaita/TabOverview/actions", test_adw_tab_overview_actions);

  return g_test_run();
}
