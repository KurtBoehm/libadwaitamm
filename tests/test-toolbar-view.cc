/*
 * Copyright (C) 2023 Purism SPC
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

static void test_adw_toolbar_view_content() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_content().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* widget = view.get_property<Gtk::Widget*>("content");
  g_assert_true(widget == nullptr);

  view.set_property<Gtk::Widget*>("content", nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  view.set_content(*widget);
  g_assert_true(view.get_content() == widget);
  g_assert_true(notified == 1);
}

static void test_adw_toolbar_view_add_remove() {
  Adw::ToolbarView view;
  auto* header_bar = Gtk::make_managed<Gtk::HeaderBar>();
  auto* search_bar = Gtk::make_managed<Gtk::SearchBar>();
  auto* action_bar = Gtk::make_managed<Gtk::ActionBar>();
  auto* content = Gtk::make_managed<Gtk::Button>();

  view.add_top_bar(*header_bar);
  view.add_top_bar(*search_bar);
  view.add_bottom_bar(*action_bar);
  view.set_content(*content);

  view.remove(*header_bar);
  view.remove(*search_bar);
  view.remove(*action_bar);
  view.remove(*content);
}

static void test_adw_toolbar_view_top_bar_style() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_top_bar_style().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::ToolbarView::Style style = view.get_property<Adw::ToolbarView::Style>("top-bar-style");
  g_assert_true(style == Adw::ToolbarView::Style::FLAT);

  view.set_top_bar_style(Adw::ToolbarView::Style::FLAT);
  g_assert_true(notified == 0);

  view.set_top_bar_style(Adw::ToolbarView::Style::RAISED);
  g_assert_true(view.get_top_bar_style() == Adw::ToolbarView::Style::RAISED);
  g_assert_true(notified == 1);

  view.set_property<Adw::ToolbarView::Style>("top-bar-style", Adw::ToolbarView::Style::FLAT);
  g_assert_true(view.get_top_bar_style() == Adw::ToolbarView::Style::FLAT);
  g_assert_true(notified == 2);
}

static void test_adw_toolbar_view_bottom_bar_style() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_bottom_bar_style().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::ToolbarView::Style style = view.get_property<Adw::ToolbarView::Style>("bottom-bar-style");
  g_assert_true(style == Adw::ToolbarView::Style::FLAT);

  view.set_bottom_bar_style(Adw::ToolbarView::Style::FLAT);
  g_assert_true(notified == 0);

  view.set_bottom_bar_style(Adw::ToolbarView::Style::RAISED);
  g_assert_true(view.get_bottom_bar_style() == Adw::ToolbarView::Style::RAISED);
  g_assert_true(notified == 1);

  view.set_property<Adw::ToolbarView::Style>("bottom-bar-style", Adw::ToolbarView::Style::FLAT);
  g_assert_true(view.get_bottom_bar_style() == Adw::ToolbarView::Style::FLAT);
  g_assert_true(notified == 2);
}

static void test_adw_toolbar_view_reveal_top_bars() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_reveal_top_bars().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool reveal = view.get_property<bool>("reveal-top-bars");
  g_assert_true(reveal);

  view.set_reveal_top_bars(true);
  g_assert_true(notified == 0);

  view.set_reveal_top_bars(false);
  g_assert_false(view.get_reveal_top_bars());
  g_assert_true(notified == 1);

  view.set_property<bool>("reveal-top-bars", true);
  g_assert_true(view.get_reveal_top_bars());
  g_assert_true(notified == 2);
}

static void test_adw_toolbar_view_reveal_bottom_bars() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_reveal_bottom_bars().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool reveal = view.get_property<bool>("reveal-bottom-bars");
  g_assert_true(reveal);

  view.set_reveal_bottom_bars(true);
  g_assert_true(notified == 0);

  view.set_reveal_bottom_bars(false);
  g_assert_false(view.get_reveal_bottom_bars());
  g_assert_true(notified == 1);

  view.set_property<bool>("reveal-bottom-bars", true);
  g_assert_true(view.get_reveal_bottom_bars());
  g_assert_true(notified == 2);
}

static void test_adw_toolbar_view_extend_content_to_top_edge() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_extend_content_to_top_edge().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool extend = view.get_property<bool>("extend-content-to-top-edge");
  g_assert_false(extend);

  view.set_extend_content_to_top_edge(false);
  g_assert_true(notified == 0);

  view.set_extend_content_to_top_edge(true);
  g_assert_true(view.get_extend_content_to_top_edge());
  g_assert_true(notified == 1);

  view.set_property<bool>("extend-content-to-top-edge", false);
  g_assert_false(view.get_extend_content_to_top_edge());
  g_assert_true(notified == 2);
}

static void test_adw_toolbar_view_extend_content_to_bottom_edge() {
  Adw::ToolbarView view;

  notified = 0;
  view.property_extend_content_to_bottom_edge().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool extend = view.get_property<bool>("extend-content-to-bottom-edge");
  g_assert_false(extend);

  view.set_extend_content_to_bottom_edge(false);
  g_assert_true(notified == 0);

  view.set_extend_content_to_bottom_edge(true);
  g_assert_true(view.get_extend_content_to_bottom_edge());
  g_assert_true(notified == 1);

  view.set_property<bool>("extend-content-to-bottom-edge", false);
  g_assert_false(view.get_extend_content_to_bottom_edge());
  g_assert_true(notified == 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ToolbarView/content", test_adw_toolbar_view_content);
  g_test_add_func("/Adwaita/ToolbarView/add_remove", test_adw_toolbar_view_add_remove);
  g_test_add_func("/Adwaita/ToolbarView/top_bar_style", test_adw_toolbar_view_top_bar_style);
  g_test_add_func("/Adwaita/ToolbarView/bottom_bar_style", test_adw_toolbar_view_bottom_bar_style);
  g_test_add_func("/Adwaita/ToolbarView/reveal_top_bars", test_adw_toolbar_view_reveal_top_bars);
  g_test_add_func("/Adwaita/ToolbarView/reveal_bottom_bars",
                  test_adw_toolbar_view_reveal_bottom_bars);
  g_test_add_func("/Adwaita/ToolbarView/extend_content_to_top_edge",
                  test_adw_toolbar_view_extend_content_to_top_edge);
  g_test_add_func("/Adwaita/ToolbarView/extend_content_to_bottom_edge",
                  test_adw_toolbar_view_extend_content_to_bottom_edge);

  return g_test_run();
}
