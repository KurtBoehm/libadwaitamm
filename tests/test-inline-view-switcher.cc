/*
 * Copyright (C) 2023 Maximiliano Sandoval <msandova@gnome.org>
 * Copyright (C) 2024 GNOME Foundation Inc.
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_inline_view_switcher_stack(void) {
  Adw::InlineViewSwitcher switcher;
  Adw::ViewStack *stack = Gtk::make_managed<Adw::ViewStack>();

  notified = 0;
  switcher.property_stack().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::Bin first, second, third;
  stack->add_titled(first, "first", "First");
  stack->add_titled(second, "second", "Second");
  stack->add_titled(third, "third", "Third");

  stack->set_visible_child_name("first");

  switcher.set_stack(nullptr);
  g_assert_null(switcher.get_stack());
  g_assert_true(notified == 0);

  switcher.property_stack() = stack;
  g_assert_true(switcher.get_stack() == stack);
  g_assert_true(notified == 1);

  switcher.property_stack() = nullptr;
  g_assert_null(switcher.get_stack());
  g_assert_true(notified == 2);

  switcher.set_stack(stack);
  g_assert_true(switcher.get_stack() == stack);
  g_assert_true(notified == 3);
}

static void test_adw_inline_view_switcher_display_mode(void) {
  Adw::InlineViewSwitcher switcher;

  notified = 0;
  switcher.property_display_mode().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(switcher.get_display_mode() ==
                Adw::InlineViewSwitcher::DisplayMode::LABELS);

  switcher.set_display_mode(Adw::InlineViewSwitcher::DisplayMode::LABELS);
  g_assert_true(switcher.get_display_mode() ==
                Adw::InlineViewSwitcher::DisplayMode::LABELS);
  g_assert_true(notified == 0);

  switcher.property_display_mode() =
      Adw::InlineViewSwitcher::DisplayMode::ICONS;
  g_assert_true(switcher.get_display_mode() ==
                Adw::InlineViewSwitcher::DisplayMode::ICONS);
  g_assert_true(notified == 1);

  switcher.set_display_mode(Adw::InlineViewSwitcher::DisplayMode::BOTH);
  g_assert_true(switcher.get_display_mode() ==
                Adw::InlineViewSwitcher::DisplayMode::BOTH);
  g_assert_true(notified == 2);
}

static void test_adw_inline_view_switcher_homogeneous(void) {
  Adw::InlineViewSwitcher switcher;

  notified = 0;
  switcher.property_homogeneous().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_false(switcher.get_homogeneous());

  switcher.set_homogeneous(false);
  g_assert_false(switcher.get_homogeneous());
  g_assert_true(notified == 0);

  switcher.property_homogeneous() = true;
  g_assert_true(switcher.get_homogeneous());
  g_assert_true(notified == 1);

  switcher.set_homogeneous(false);
  g_assert_false(switcher.get_homogeneous());
  g_assert_true(notified == 2);
}

static void test_adw_inline_view_switcher_can_shrink(void) {
  Adw::InlineViewSwitcher switcher;

  notified = 0;
  switcher.property_can_shrink().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(switcher.get_can_shrink());

  switcher.set_can_shrink(true);
  g_assert_true(switcher.get_can_shrink());
  g_assert_true(notified == 0);

  switcher.property_can_shrink() = false;
  g_assert_false(switcher.get_can_shrink());
  g_assert_true(notified == 1);

  switcher.set_can_shrink(true);
  g_assert_true(switcher.get_can_shrink());
  g_assert_true(notified == 2);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/InlineViewSwitcher/stack",
                  test_adw_inline_view_switcher_stack);
  g_test_add_func("/Adwaita/InlineViewSwitcher/display_mode",
                  test_adw_inline_view_switcher_display_mode);
  g_test_add_func("/Adwaita/InlineViewSwitcher/homogeneous",
                  test_adw_inline_view_switcher_homogeneous);
  g_test_add_func("/Adwaita/InlineViewSwitcher/can_shrink",
                  test_adw_inline_view_switcher_can_shrink);

  return g_test_run();
}
