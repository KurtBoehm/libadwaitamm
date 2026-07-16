/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

static void test_adw_breakpoint_bin_child(void) {
  Adw::BreakpointBin bin;

  notified = 0;
  bin.property_child().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget *widget = bin.get_property<Gtk::Widget *>("child");
  g_assert_null(widget);

  bin.set_child(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  bin.set_child(widget);
  g_assert_true(bin.get_child() == widget);
  g_assert_true(notified == 1);

  bin.set_property<Gtk::Widget *>("child", nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_breakpoint_bin_add_remove_breakpoint(void) {
  Adw::BreakpointBin bin;

  g_assert_true(bin.get_current_breakpoint() == nullptr);

  auto condition = Adw::BreakpointCondition::parse("max-width: 400px");
  auto breakpoint = Adw::Breakpoint::create(condition);

  bin.add_breakpoint(breakpoint.get());

  // Without a real size allocation matching the condition, no breakpoint
  // becomes current.
  g_assert_true(bin.get_current_breakpoint() == nullptr);

  bin.remove_breakpoint(breakpoint.get());
  g_assert_true(bin.get_current_breakpoint() == nullptr);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/BreakpointBin/child",
                  test_adw_breakpoint_bin_child);
  g_test_add_func("/Adwaita/BreakpointBin/add_remove_breakpoint",
                  test_adw_breakpoint_bin_add_remove_breakpoint);

  return g_test_run();
}
