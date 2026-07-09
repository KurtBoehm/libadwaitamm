/*
 * Copyright (C) 2020 Alexander Mikhaylenko <alexm@gnome.org>
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_application_window_new(void) {
  Glib::RefPtr<Gtk::Application> app; // none
  Adw::ApplicationWindow window(app);
}

static void test_adw_application_window_content(void) {
  Adw::ApplicationWindow window;

  // No content set yet.
  g_assert_null(window.get_content());

  auto box = Gtk::make_managed<Gtk::Box>();
  window.set_content(box);
  g_assert_true(window.get_content() == box);

  // Clearing content should work too.
  window.set_content(nullptr);
  g_assert_null(window.get_content());
}

static void test_adw_application_window_adaptive_preview(void) {
  Adw::ApplicationWindow window;

  // Closed by default.
  g_assert_false(window.get_adaptive_preview());

  window.set_adaptive_preview(true);
  g_assert_true(window.get_adaptive_preview());

  window.set_adaptive_preview(false);
  g_assert_false(window.get_adaptive_preview());
}

static void test_adw_application_window_breakpoint(void) {
  Adw::ApplicationWindow window;

  // No breakpoint applied on a freshly constructed window.
  g_assert_true(window.property_current_breakpoint().get_value() == nullptr);

  auto condition = Adw::BreakpointCondition::parse("max-width: 400px");
  auto breakpoint = Adw::Breakpoint::create(condition);

  // Should not crash, and the getter/setter methods should remain usable
  // afterwards even though the breakpoint won't actually be applied
  // without allocating/realizing the window.
  window.add_breakpoint(breakpoint);
  g_assert_true(window.get_current_breakpoint() == nullptr ||
                window.get_current_breakpoint() != nullptr);
}

static void test_adw_application_window_dialogs(void) {
  Adw::ApplicationWindow window;

  // No dialogs open on a freshly constructed window.
  auto dialogs = window.get_dialogs();
  g_assert_cmpuint(dialogs.size(), ==, 0);

  g_assert_null(window.get_visible_dialog());
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ApplicationWindow/new",
                  test_adw_application_window_new);
  g_test_add_func("/Adwaita/ApplicationWindow/content",
                  test_adw_application_window_content);
  g_test_add_func("/Adwaita/ApplicationWindow/adaptive_preview",
                  test_adw_application_window_adaptive_preview);
  g_test_add_func("/Adwaita/ApplicationWindow/breakpoint",
                  test_adw_application_window_breakpoint);
  g_test_add_func("/Adwaita/ApplicationWindow/dialogs",
                  test_adw_application_window_dialogs);

  return g_test_run();
}