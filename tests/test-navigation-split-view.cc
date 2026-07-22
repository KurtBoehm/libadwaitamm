/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_navigation_split_view_sidebar(void) {
  Adw::NavigationSplitView split_view;
  int local_notified = 0, showing = 0, hiding = 0, shown = 0, hidden = 0;

  split_view.property_sidebar().signal_changed().connect(
      [&local_notified]() { local_notified++; });

  g_assert_true(split_view.get_sidebar() == nullptr);

  split_view.set_sidebar(nullptr);
  g_assert_true(local_notified == 0);

  Adw::NavigationPage sidebar(Gtk::make_managed<Gtk::Button>(), "Sidebar");
  sidebar.signal_showing().connect([&showing]() { showing++; });
  sidebar.signal_hiding().connect([&hiding]() { hiding++; });
  sidebar.signal_shown().connect([&shown]() { shown++; });
  sidebar.signal_hidden().connect([&hidden]() { hidden++; });

  split_view.set_sidebar(&sidebar);
  g_assert_true(split_view.get_sidebar() == &sidebar);
  g_assert_true(local_notified == 1);
  g_assert_true(showing == 1);
  g_assert_true(shown == 1);
  g_assert_true(hiding == 0);
  g_assert_true(hidden == 0);

  split_view.set_property<Adw::NavigationPage *>("sidebar", nullptr);
  g_assert_true(split_view.get_sidebar() == nullptr);
  g_assert_true(local_notified == 2);
  g_assert_true(showing == 1);
  g_assert_true(shown == 1);
  g_assert_true(hiding == 1);
  g_assert_true(hidden == 1);
}

static void test_adw_navigation_split_view_content(void) {
  Adw::NavigationSplitView split_view;
  int local_notified = 0, showing = 0, hiding = 0, shown = 0, hidden = 0;

  split_view.property_content().signal_changed().connect(
      [&local_notified]() { local_notified++; });

  g_assert_true(split_view.get_content() == nullptr);

  split_view.set_content(nullptr);
  g_assert_true(local_notified == 0);

  Adw::NavigationPage content(Gtk::make_managed<Gtk::Button>(), "Content");
  content.signal_showing().connect([&showing]() { showing++; });
  content.signal_hiding().connect([&hiding]() { hiding++; });
  content.signal_shown().connect([&shown]() { shown++; });
  content.signal_hidden().connect([&hidden]() { hidden++; });

  split_view.set_content(&content);
  g_assert_true(split_view.get_content() == &content);
  g_assert_true(local_notified == 1);
  g_assert_true(showing == 1);
  g_assert_true(shown == 1);
  g_assert_true(hiding == 0);
  g_assert_true(hidden == 0);

  split_view.set_property<Adw::NavigationPage *>("content", nullptr);
  g_assert_true(split_view.get_content() == nullptr);
  g_assert_true(local_notified == 2);
  g_assert_true(showing == 1);
  g_assert_true(shown == 1);
  g_assert_true(hiding == 1);
  g_assert_true(hidden == 1);
}

static void test_adw_navigation_split_view_sidebar_position(void) {
  Adw::NavigationSplitView split_view;

  notified = 0;
  split_view.property_sidebar_position().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_sidebar_position() == Gtk::PackType::START);

  split_view.set_sidebar_position(Gtk::PackType::START);
  g_assert_true(notified == 0);

  split_view.set_sidebar_position(Gtk::PackType::END);
  g_assert_true(split_view.get_sidebar_position() == Gtk::PackType::END);
  g_assert_true(notified == 1);

  split_view.set_property<Gtk::PackType>("sidebar-position",
                                         Gtk::PackType::START);
  g_assert_true(split_view.get_sidebar_position() == Gtk::PackType::START);
  g_assert_true(notified == 2);
}

static void test_adw_navigation_split_view_collapsed(void) {
  Adw::NavigationSplitView split_view;
  int local_notified = 0;
  int sidebar_showing = 0, sidebar_hiding = 0, sidebar_shown = 0,
      sidebar_hidden = 0;
  int content_showing = 0, content_hiding = 0, content_shown = 0,
      content_hidden = 0;

  split_view.property_collapsed().signal_changed().connect(
      [&local_notified]() { local_notified++; });

  g_assert_false(split_view.get_collapsed());

  split_view.set_collapsed(false);
  g_assert_true(local_notified == 0);

  split_view.set_collapsed(true);
  g_assert_true(split_view.get_collapsed());
  g_assert_true(local_notified == 1);

  split_view.set_property<bool>("collapsed", false);
  g_assert_false(split_view.get_collapsed());
  g_assert_true(local_notified == 2);

  Adw::NavigationPage sidebar(Gtk::make_managed<Gtk::Button>(), "Sidebar");
  sidebar.signal_showing().connect([&sidebar_showing]() { sidebar_showing++; });
  sidebar.signal_hiding().connect([&sidebar_hiding]() { sidebar_hiding++; });
  sidebar.signal_shown().connect([&sidebar_shown]() { sidebar_shown++; });
  sidebar.signal_hidden().connect([&sidebar_hidden]() { sidebar_hidden++; });

  Adw::NavigationPage content(Gtk::make_managed<Gtk::Button>(), "Content");
  content.signal_showing().connect([&content_showing]() { content_showing++; });
  content.signal_hiding().connect([&content_hiding]() { content_hiding++; });
  content.signal_shown().connect([&content_shown]() { content_shown++; });
  content.signal_hidden().connect([&content_hidden]() { content_hidden++; });

  split_view.set_sidebar(&sidebar);
  g_assert_true(sidebar_showing == 1);
  g_assert_true(sidebar_shown == 1);
  g_assert_true(sidebar_hiding == 0);
  g_assert_true(sidebar_hidden == 0);

  split_view.set_collapsed(true);
  g_assert_true(split_view.get_collapsed());
  g_assert_true(local_notified == 3);

  split_view.set_collapsed(false);
  g_assert_false(split_view.get_collapsed());
  g_assert_true(local_notified == 4);

  split_view.set_sidebar(nullptr);
  split_view.set_content(&content);
  g_assert_true(sidebar_showing == 1);
  g_assert_true(sidebar_shown == 1);
  g_assert_true(sidebar_hiding == 1);
  g_assert_true(sidebar_hidden == 1);
  g_assert_true(content_showing == 1);
  g_assert_true(content_shown == 1);
  g_assert_true(content_hiding == 0);
  g_assert_true(content_hidden == 0);

  split_view.set_collapsed(true);
  g_assert_true(split_view.get_collapsed());
  g_assert_true(local_notified == 5);

  split_view.set_collapsed(false);
  g_assert_false(split_view.get_collapsed());
  g_assert_true(local_notified == 6);

  split_view.set_sidebar(&sidebar);
  g_assert_true(sidebar_showing == 2);
  g_assert_true(sidebar_shown == 2);
  g_assert_true(sidebar_hiding == 1);
  g_assert_true(sidebar_hidden == 1);

  split_view.set_show_content(true);

  split_view.set_collapsed(true);
  g_assert_true(split_view.get_collapsed());
  g_assert_true(local_notified == 7);
  g_assert_true(sidebar_showing == 2);
  g_assert_true(sidebar_shown == 2);
  g_assert_true(sidebar_hiding == 2);
  g_assert_true(sidebar_hidden == 2);
  g_assert_true(content_showing == 1);
  g_assert_true(content_shown == 1);
  g_assert_true(content_hiding == 0);
  g_assert_true(content_hidden == 0);

  split_view.set_collapsed(false);
  g_assert_false(split_view.get_collapsed());
  g_assert_true(local_notified == 8);
  g_assert_true(sidebar_showing == 3);
  g_assert_true(sidebar_shown == 3);
  g_assert_true(sidebar_hiding == 2);
  g_assert_true(sidebar_hidden == 2);
  g_assert_true(content_showing == 1);
  g_assert_true(content_shown == 1);
  g_assert_true(content_hiding == 0);
  g_assert_true(content_hidden == 0);

  split_view.set_show_content(false);

  split_view.set_collapsed(true);
  g_assert_true(split_view.get_collapsed());
  g_assert_true(local_notified == 9);
  g_assert_true(sidebar_showing == 3);
  g_assert_true(sidebar_shown == 3);
  g_assert_true(sidebar_hiding == 2);
  g_assert_true(sidebar_hidden == 2);
  g_assert_true(content_showing == 1);
  g_assert_true(content_shown == 1);
  g_assert_true(content_hiding == 1);
  g_assert_true(content_hidden == 1);

  split_view.set_collapsed(false);
  g_assert_false(split_view.get_collapsed());
  g_assert_true(local_notified == 10);
  g_assert_true(sidebar_showing == 3);
  g_assert_true(sidebar_shown == 3);
  g_assert_true(sidebar_hiding == 2);
  g_assert_true(sidebar_hidden == 2);
  g_assert_true(content_showing == 2);
  g_assert_true(content_shown == 2);
  g_assert_true(content_hiding == 1);
  g_assert_true(content_hidden == 1);
}

static void test_adw_navigation_split_view_show_content(void) {
  Adw::NavigationSplitView split_view;

  notified = 0;
  split_view.property_show_content().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(split_view.get_show_content());

  split_view.set_show_content(false);
  g_assert_true(notified == 0);

  split_view.set_show_content(true);
  g_assert_true(split_view.get_show_content());
  g_assert_true(notified == 1);

  split_view.set_property<bool>("show-content", false);
  g_assert_false(split_view.get_show_content());
  g_assert_true(notified == 2);

  Adw::NavigationPage sidebar(Gtk::make_managed<Gtk::Button>(), "Sidebar");
  split_view.set_sidebar(&sidebar);

  split_view.set_show_content(true);
  g_assert_true(split_view.get_show_content());
  g_assert_true(notified == 3);

  split_view.set_property<bool>("show-content", false);
  g_assert_false(split_view.get_show_content());
  g_assert_true(notified == 4);
}

static void test_adw_navigation_split_view_min_sidebar_width(void) {
  Adw::NavigationSplitView split_view;

  notified = 0;
  split_view.property_min_sidebar_width().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_cmpfloat_with_epsilon(split_view.get_min_sidebar_width(), 180, 1e-9);

  split_view.set_min_sidebar_width(180);
  g_assert_true(notified == 0);

  split_view.set_min_sidebar_width(200);
  g_assert_cmpfloat_with_epsilon(split_view.get_min_sidebar_width(), 200, 1e-9);
  g_assert_true(notified == 1);

  split_view.set_property<double>("min-sidebar-width", 180.0);
  g_assert_cmpfloat_with_epsilon(split_view.get_min_sidebar_width(), 180, 1e-9);
  g_assert_true(notified == 2);
}

static void test_adw_navigation_split_view_max_sidebar_width(void) {
  Adw::NavigationSplitView split_view;

  notified = 0;
  split_view.property_max_sidebar_width().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_cmpfloat_with_epsilon(split_view.get_max_sidebar_width(), 280, 1e-9);

  split_view.set_max_sidebar_width(280);
  g_assert_true(notified == 0);

  split_view.set_max_sidebar_width(200);
  g_assert_cmpfloat_with_epsilon(split_view.get_max_sidebar_width(), 200, 1e-9);
  g_assert_true(notified == 1);

  split_view.set_property<double>("max-sidebar-width", 280.0);
  g_assert_cmpfloat_with_epsilon(split_view.get_max_sidebar_width(), 280, 1e-9);
  g_assert_true(notified == 2);
}

static void test_adw_navigation_split_view_sidebar_width_fraction(void) {
  Adw::NavigationSplitView split_view;

  notified = 0;
  split_view.property_sidebar_width_fraction().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_cmpfloat_with_epsilon(split_view.get_sidebar_width_fraction(),
                                 0.25, 1e-9);

  split_view.set_sidebar_width_fraction(0.25);
  g_assert_true(notified == 0);

  split_view.set_sidebar_width_fraction(0.2);
  g_assert_cmpfloat_with_epsilon(split_view.get_sidebar_width_fraction(), 0.2,
                                 1e-9);
  g_assert_true(notified == 1);

  split_view.set_property<double>("sidebar-width-fraction", 0.25);
  g_assert_cmpfloat_with_epsilon(split_view.get_sidebar_width_fraction(),
                                 0.25, 1e-9);
  g_assert_true(notified == 2);
}

static void test_adw_navigation_split_view_sidebar_width_unit(void) {
  Adw::NavigationSplitView split_view;

  notified = 0;
  split_view.property_sidebar_width_unit().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(split_view.get_sidebar_width_unit() == Adw::LengthUnit::SP);

  split_view.set_sidebar_width_unit(Adw::LengthUnit::SP);
  g_assert_true(notified == 0);

  split_view.set_sidebar_width_unit(Adw::LengthUnit::PX);
  g_assert_true(split_view.get_sidebar_width_unit() == Adw::LengthUnit::PX);
  g_assert_true(notified == 1);

  split_view.set_property<Adw::LengthUnit>("sidebar-width-unit",
                                           Adw::LengthUnit::SP);
  g_assert_true(split_view.get_sidebar_width_unit() == Adw::LengthUnit::SP);
  g_assert_true(notified == 2);
}

static void test_adw_navigation_split_view_page_tags(void) {
  Adw::NavigationSplitView split_view;
  Adw::NavigationPage sidebar(Gtk::make_managed<Gtk::Button>(), "Sidebar",
                              "sidebar");
  Adw::NavigationPage content(Gtk::make_managed<Gtk::Button>(), "Content",
                              "sidebar");
  Adw::NavigationPage content2(Gtk::make_managed<Gtk::Button>(), "Content",
                               "content");

  split_view.set_sidebar(&sidebar);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already has the same tag*");
  split_view.set_content(&content);
  g_test_assert_expected_messages();

  split_view.set_content(&content2);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already has the same tag*");
  split_view.get_sidebar()->set_tag("content");
  g_test_assert_expected_messages();

  split_view.get_sidebar()->set_tag("sidebar");

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already has the same tag*");
  split_view.get_content()->set_tag("sidebar");
  g_test_assert_expected_messages();
}

static void test_adw_navigation_split_view_actions(void) {
  Adw::NavigationSplitView split_view;
  Adw::NavigationPage sidebar(Gtk::make_managed<Gtk::Button>(), "Sidebar",
                              "sidebar");
  Adw::NavigationPage content(Gtk::make_managed<Gtk::Button>(), "Content",
                              "content");

  notified = 0;
  split_view.property_show_content().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  split_view.set_sidebar(&sidebar);
  split_view.set_content(&content);

  g_assert_false(split_view.get_show_content());
  g_assert_true(notified == 0);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already in the navigation stack*");
  split_view.activate_action("navigation.push",
                             Glib::Variant<Glib::ustring>::create("sidebar"));
  g_test_assert_expected_messages();

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*No page with the tag*");
  split_view.activate_action(
      "navigation.push", Glib::Variant<Glib::ustring>::create("something"));
  g_test_assert_expected_messages();

  split_view.activate_action("navigation.push",
                             Glib::Variant<Glib::ustring>::create("content"));

  g_assert_true(split_view.get_show_content());
  g_assert_true(notified == 1);

  split_view.activate_action("navigation.push",
                             Glib::Variant<Glib::ustring>::create("content"));
  split_view.activate_action("navigation.push",
                             Glib::Variant<Glib::ustring>::create("content"));

  g_assert_true(split_view.get_show_content());
  g_assert_true(notified == 1);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already in the navigation stack*");
  split_view.activate_action("navigation.push",
                             Glib::Variant<Glib::ustring>::create("sidebar"));
  g_test_assert_expected_messages();

  split_view.activate_action("navigation.pop");

  g_assert_false(split_view.get_show_content());
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/NavigationSplitView/sidebar",
                  test_adw_navigation_split_view_sidebar);
  g_test_add_func("/Adwaita/NavigationSplitView/content",
                  test_adw_navigation_split_view_content);
  g_test_add_func("/Adwaita/NavigationSplitView/sidebar_position",
                  test_adw_navigation_split_view_sidebar_position);
  g_test_add_func("/Adwaita/NavigationSplitView/collapsed",
                  test_adw_navigation_split_view_collapsed);
  g_test_add_func("/Adwaita/NavigationSplitView/show_content",
                  test_adw_navigation_split_view_show_content);
  g_test_add_func("/Adwaita/NavigationSplitView/min_sidebar_width",
                  test_adw_navigation_split_view_min_sidebar_width);
  g_test_add_func("/Adwaita/NavigationSplitView/max_sidebar_width",
                  test_adw_navigation_split_view_max_sidebar_width);
  g_test_add_func("/Adwaita/NavigationSplitView/sidebar_width_fraction",
                  test_adw_navigation_split_view_sidebar_width_fraction);
  g_test_add_func("/Adwaita/NavigationSplitView/sidebar_width_unit",
                  test_adw_navigation_split_view_sidebar_width_unit);
  g_test_add_func("/Adwaita/NavigationSplitView/page_tags",
                  test_adw_navigation_split_view_page_tags);
  g_test_add_func("/Adwaita/NavigationSplitView/actions",
                  test_adw_navigation_split_view_actions);

  return g_test_run();
}
