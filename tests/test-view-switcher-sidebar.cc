/*
 * Copyright (C) 2025 GNOME Foundation Inc.
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alice Mikhaylenko <alicem@gnome.org>
 * Author: Kurt Böhm <kurbo96@gmail.com>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() {
  ++notified;
}

static void test_adw_view_switcher_sidebar_stack() {
  Adw::ViewSwitcherSidebar sidebar;

  notified = 0;
  sidebar.property_stack().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::ViewStack* stack = sidebar.get_property<Adw::ViewStack*>("stack");
  g_assert_true(stack == nullptr);

  stack = Gtk::make_managed<Adw::ViewStack>();
  sidebar.set_stack(stack);
  g_assert_true(sidebar.get_stack() == stack);
  g_assert_true(notified == 1);

  sidebar.set_property<Adw::ViewStack*>("stack", nullptr);
  g_assert_true(sidebar.get_stack() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_view_switcher_sidebar_mode() {
  Adw::ViewSwitcherSidebar sidebar;

  notified = 0;
  sidebar.property_mode().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::Sidebar::Mode mode = sidebar.get_property<Adw::Sidebar::Mode>("mode");
  g_assert_true(mode == Adw::Sidebar::Mode::SIDEBAR);

  sidebar.set_mode(Adw::Sidebar::Mode::PAGE);
  g_assert_true(sidebar.get_mode() == Adw::Sidebar::Mode::PAGE);
  g_assert_true(notified == 1);

  sidebar.set_property<Adw::Sidebar::Mode>("mode", Adw::Sidebar::Mode::SIDEBAR);
  g_assert_true(sidebar.get_mode() == Adw::Sidebar::Mode::SIDEBAR);
  g_assert_true(notified == 2);
}

static void test_adw_view_switcher_sidebar_filter() {
  Adw::ViewSwitcherSidebar sidebar;

  notified = 0;
  sidebar.property_filter().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gtk::Filter> filter = sidebar.get_property<Glib::RefPtr<Gtk::Filter>>("filter");
  g_assert_true(filter == nullptr);

  sidebar.set_filter(Glib::RefPtr<Gtk::Filter>());
  g_assert_true(notified == 0);

  filter = Gtk::BoolFilter::create(Glib::RefPtr<Gtk::Expression<bool>>());
  sidebar.set_filter(filter);
  g_assert_true(sidebar.get_filter() == filter);
  g_assert_true(notified == 1);

  sidebar.set_property<Glib::RefPtr<Gtk::Filter>>("filter", Glib::RefPtr<Gtk::Filter>());
  g_assert_true(sidebar.get_filter() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_view_switcher_sidebar_placeholder() {
  Adw::ViewSwitcherSidebar sidebar;

  notified = 0;
  sidebar.property_placeholder().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* placeholder = sidebar.get_property<Gtk::Widget*>("placeholder");
  g_assert_true(placeholder == nullptr);

  sidebar.set_placeholder(nullptr);
  g_assert_true(notified == 0);

  placeholder = Gtk::make_managed<Gtk::Button>();
  sidebar.set_placeholder(placeholder);
  g_assert_true(sidebar.get_placeholder() == placeholder);
  g_assert_true(notified == 1);

  sidebar.set_property<Gtk::Widget*>("placeholder", nullptr);
  g_assert_true(sidebar.get_placeholder() == nullptr);
  g_assert_true(notified == 2);
}

#if ADW_CHECK_VERSION(1, 10, 0)
static void test_adw_view_switcher_sidebar_prefix() {
  Adw::ViewSwitcherSidebar sidebar;
  auto* prefix1 = Gtk::make_managed<Gtk::Button>();
  auto* prefix2 = Gtk::make_managed<Gtk::Button>();

  notified = 0;
  sidebar.property_prefix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* prefix = sidebar.get_property<Gtk::Widget*>("prefix");
  g_assert_true(prefix == nullptr);

  sidebar.set_prefix(nullptr);
  g_assert_true(notified == 0);

  sidebar.set_prefix(prefix1);
  g_assert_true(sidebar.get_prefix() == prefix1);
  g_assert_true(notified == 1);

  sidebar.set_mode(Adw::Sidebar::Mode::PAGE);

  sidebar.set_property<Gtk::Widget*>("prefix", nullptr);
  g_assert_true(sidebar.get_prefix() == nullptr);
  g_assert_true(notified == 2);

  sidebar.set_prefix(prefix2);
  g_assert_true(sidebar.get_prefix() == prefix2);
  g_assert_true(notified == 3);

  sidebar.set_mode(Adw::Sidebar::Mode::SIDEBAR);
}

static void test_adw_view_switcher_sidebar_suffix() {
  Adw::ViewSwitcherSidebar sidebar;
  auto* suffix1 = Gtk::make_managed<Gtk::Button>();
  auto* suffix2 = Gtk::make_managed<Gtk::Button>();

  notified = 0;
  sidebar.property_suffix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* suffix = sidebar.get_property<Gtk::Widget*>("suffix");
  g_assert_true(suffix == nullptr);

  sidebar.set_suffix(nullptr);
  g_assert_true(notified == 0);

  sidebar.set_suffix(suffix1);
  g_assert_true(sidebar.get_suffix() == suffix1);
  g_assert_true(notified == 1);

  sidebar.set_mode(Adw::Sidebar::Mode::PAGE);

  sidebar.set_property<Gtk::Widget*>("suffix", nullptr);
  g_assert_true(sidebar.get_suffix() == nullptr);
  g_assert_true(notified == 2);

  sidebar.set_suffix(suffix2);
  g_assert_true(sidebar.get_suffix() == suffix2);
  g_assert_true(notified == 3);

  sidebar.set_mode(Adw::Sidebar::Mode::SIDEBAR);
}
#endif

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ViewSwitcherSidebar/stack", test_adw_view_switcher_sidebar_stack);
  g_test_add_func("/Adwaita/ViewSwitcherSidebar/mode", test_adw_view_switcher_sidebar_mode);
  g_test_add_func("/Adwaita/ViewSwitcherSidebar/filter", test_adw_view_switcher_sidebar_filter);
  g_test_add_func("/Adwaita/ViewSwitcherSidebar/placeholder",
                  test_adw_view_switcher_sidebar_placeholder);
#if ADW_CHECK_VERSION(1, 10, 0)
  g_test_add_func("/Adwaita/ViewSwitcherSidebar/prefix", test_adw_view_switcher_sidebar_prefix);
  g_test_add_func("/Adwaita/ViewSwitcherSidebar/suffix", test_adw_view_switcher_sidebar_suffix);
#endif

  return g_test_run();
}
