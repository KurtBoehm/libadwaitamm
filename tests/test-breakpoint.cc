/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
static void notify_cb() { notified++; }

int apply_count;
static void on_apply() { apply_count++; }

int unapply_count;
static void on_unapply() { unapply_count++; }

static void test_adw_breakpoint_condition_length(void) {
  auto condition = Adw::BreakpointCondition::create_length(
      Adw::BreakpointCondition::LengthType::MAX_WIDTH, 400,
      Adw::LengthUnit::PX);
  Glib::ustring str = condition.to_string();
  g_assert_true(str.find("max-width") != Glib::ustring::npos);
}

static void test_adw_breakpoint_condition_ratio(void) {
  auto condition = Adw::BreakpointCondition::create_ratio(
      Adw::BreakpointCondition::RatioType::MAX_ASPECT_RATIO, 4, 3);
  Glib::ustring str = condition.to_string();
  g_assert_true(str.find("aspect-ratio") != Glib::ustring::npos);
}

static void test_adw_breakpoint_condition_and_or(void) {
  auto and_condition = Adw::BreakpointCondition::create_and(
      Adw::BreakpointCondition::parse("min-width: 400px"),
      Adw::BreakpointCondition::parse("max-aspect-ratio: 4/3"));
  g_assert_true(and_condition.to_string().find(" and ") != Glib::ustring::npos);

  auto or_condition = Adw::BreakpointCondition::create_or(
      Adw::BreakpointCondition::parse("max-width: 360sp"),
      Adw::BreakpointCondition::parse("max-width: 360px"));
  g_assert_true(or_condition.to_string().find(" or ") != Glib::ustring::npos);
}

static void test_adw_breakpoint_condition_parse(void) {
  auto condition = Adw::BreakpointCondition::parse("min-width: 400px");
  Glib::ustring str = condition.to_string();
  g_assert_true(str.find("min-width") != Glib::ustring::npos);
}

static void test_adw_breakpoint_condition_property(void) {
  auto condition = Adw::BreakpointCondition::parse("max-width: 400px");
  auto breakpoint = Adw::Breakpoint::create(condition);

  notified = 0;
  breakpoint->property_condition().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(breakpoint->get_condition().to_string() ==
                condition.to_string());

  auto new_condition = Adw::BreakpointCondition::parse("max-width: 200px");
  breakpoint->set_condition(new_condition);
  g_assert_true(notified == 1);
  g_assert_true(breakpoint->get_condition().to_string() ==
                new_condition.to_string());
}

static void test_adw_breakpoint_add_setter(void) {
  auto condition = Adw::BreakpointCondition::parse("max-width: 400px");
  auto breakpoint = Adw::Breakpoint::create(condition);
  Gtk::Label label;

  Glib::Value<bool> value;
  value.init(Glib::Value<bool>::value_type());
  value.set(false);

  // Just verify this doesn't crash; the setter only takes effect once the
  // breakpoint is added to an AdwBreakpointBin/window and actually applies.
  breakpoint->add_setter(label, "visible", value);
}

static void test_adw_breakpoint_signals(void) {
  auto condition = Adw::BreakpointCondition::parse("max-width: 400px");
  auto breakpoint = Adw::Breakpoint::create(condition);

  apply_count = 0;
  unapply_count = 0;
  breakpoint->signal_apply().connect(sigc::ptr_fun(on_apply));
  breakpoint->signal_unapply().connect(sigc::ptr_fun(on_unapply));

  g_signal_emit_by_name(breakpoint->gobj(), "apply");
  g_assert_true(apply_count == 1);
  g_assert_true(unapply_count == 0);

  g_signal_emit_by_name(breakpoint->gobj(), "unapply");
  g_assert_true(unapply_count == 1);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Breakpoint/condition_length",
                  test_adw_breakpoint_condition_length);
  g_test_add_func("/Adwaita/Breakpoint/condition_ratio",
                  test_adw_breakpoint_condition_ratio);
  g_test_add_func("/Adwaita/Breakpoint/condition_and_or",
                  test_adw_breakpoint_condition_and_or);
  g_test_add_func("/Adwaita/Breakpoint/condition_parse",
                  test_adw_breakpoint_condition_parse);
  g_test_add_func("/Adwaita/Breakpoint/condition_property",
                  test_adw_breakpoint_condition_property);
  g_test_add_func("/Adwaita/Breakpoint/add_setter",
                  test_adw_breakpoint_add_setter);
  g_test_add_func("/Adwaita/Breakpoint/signals", test_adw_breakpoint_signals);

  return g_test_run();
}
