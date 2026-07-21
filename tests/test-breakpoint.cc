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
  g_assert_true(Adw::BreakpointCondition::create_length(
                    Adw::BreakpointCondition::LengthType::MAX_WIDTH, 400,
                    Adw::LengthUnit::PX)
                    .to_string() == "max-width: 400px");

  g_assert_true(Adw::BreakpointCondition::create_length(
                    Adw::BreakpointCondition::LengthType::MIN_HEIGHT, 200,
                    Adw::LengthUnit::PT)
                    .to_string() == "min-height: 200pt");

  g_assert_true(Adw::BreakpointCondition::create_length(
                    Adw::BreakpointCondition::LengthType::MIN_HEIGHT, 200.5,
                    Adw::LengthUnit::PT)
                    .to_string() == "min-height: 200.5pt");
}

static void test_adw_breakpoint_condition_ratio(void) {
  g_assert_true(Adw::BreakpointCondition::create_ratio(
                    Adw::BreakpointCondition::RatioType::MIN_ASPECT_RATIO, 4,
                    3)
                    .to_string() == "min-aspect-ratio: 4/3");

  // Ratios are simplified.
  g_assert_true(Adw::BreakpointCondition::create_ratio(
                    Adw::BreakpointCondition::RatioType::MIN_ASPECT_RATIO, 2,
                    1)
                    .to_string() == "min-aspect-ratio: 2");
  g_assert_true(Adw::BreakpointCondition::create_ratio(
                    Adw::BreakpointCondition::RatioType::MIN_ASPECT_RATIO, 0,
                    2)
                    .to_string() == "min-aspect-ratio: 0");
}

static void test_adw_breakpoint_condition_and_or(void) {
  auto condition_1 = Adw::BreakpointCondition::create_length(
      Adw::BreakpointCondition::LengthType::MAX_WIDTH, 400,
      Adw::LengthUnit::PX);
  auto condition_2 = Adw::BreakpointCondition::create_ratio(
      Adw::BreakpointCondition::RatioType::MIN_ASPECT_RATIO, 4, 3);
  auto condition_3 = Adw::BreakpointCondition::create_ratio(
      Adw::BreakpointCondition::RatioType::MAX_ASPECT_RATIO, 2, 1);

  g_assert_true(
      Adw::BreakpointCondition::create_and(condition_1, condition_2)
          .to_string() == "max-width: 400px and min-aspect-ratio: 4/3");

  g_assert_true(
      Adw::BreakpointCondition::create_or(condition_1, condition_2)
          .to_string() == "max-width: 400px or min-aspect-ratio: 4/3");

  // Nested conditions of the same kind don't get parentheses...
  g_assert_true(
      Adw::BreakpointCondition::create_and(
          condition_1,
          Adw::BreakpointCondition::create_and(condition_2, condition_3))
          .to_string() ==
      "max-width: 400px and min-aspect-ratio: 4/3 and max-aspect-ratio: 2");

  // ...but nested conditions of a different kind do.
  g_assert_true(
      Adw::BreakpointCondition::create_and(
          condition_1,
          Adw::BreakpointCondition::create_or(condition_2, condition_3))
          .to_string() ==
      "max-width: 400px and (min-aspect-ratio: 4/3 or max-aspect-ratio: 2)");

  g_assert_true(
      Adw::BreakpointCondition::create_or(
          Adw::BreakpointCondition::create_and(condition_1, condition_2),
          condition_3)
          .to_string() ==
      "(max-width: 400px and min-aspect-ratio: 4/3) or max-aspect-ratio: 2");
}

static void test_adw_breakpoint_condition_parse(void) {
  auto check = [](const char *input, const char *expected) {
    g_assert_true(Adw::BreakpointCondition::parse(input).to_string() ==
                  expected);
  };

  // Length: unit defaults to px, and trailing zeroes are dropped.
  check("max-width: 400px", "max-width: 400px");
  check("max-width: 400", "max-width: 400px");
  check("max-width: 400pt", "max-width: 400pt");
  check("max-width:400pt", "max-width: 400pt");
  check("max-width: 400.0px", "max-width: 400px");
  check("max-width: 400.5px", "max-width: 400.5px");
  check("      max-width        :        400     pt       ",
        "max-width: 400pt");

  // Ratio: height defaults to 1, and the ratio is simplified.
  check("max-aspect-ratio: 4/3", "max-aspect-ratio: 4/3");
  check("max-aspect-ratio: 2", "max-aspect-ratio: 2");
  check("max-aspect-ratio: 2/1", "max-aspect-ratio: 2");
  check("max-aspect-ratio: 0/3", "max-aspect-ratio: 0");
  check("max-aspect-ratio:4/3", "max-aspect-ratio: 4/3");
  check("       max-aspect-ratio   :         4/3       ",
        "max-aspect-ratio: 4/3");

  // Parentheses can be used to group a single condition.
  check("(max-width: 100px)", "max-width: 100px");
  check("(((max-width: 100px)))", "max-width: 100px");
  check("   (   max-width   :   100px   )   ", "max-width: 100px");

  // Multiple conditions, with and without parentheses.
  check("max-width: 100px and max-height: 200px",
        "max-width: 100px and max-height: 200px");
  check("max-width: 100px or max-height: 200px",
        "max-width: 100px or max-height: 200px");
  check("(max-width: 100px) and max-height: 200px",
        "max-width: 100px and max-height: 200px");
  check("max-width: 100px and (max-height: 200px)",
        "max-width: 100px and max-height: 200px");
  check("(max-width: 100px and max-height: 200px)",
        "max-width: 100px and max-height: 200px");

  // Without parentheses, the first operator takes priority.
  check("max-width: 100px and max-height: 200px or max-aspect-ratio: 3/2 and "
        "min-aspect-ratio: 1/2",
        "((max-width: 100px and max-height: 200px) or max-aspect-ratio: "
        "3/2) and min-aspect-ratio: 1/2");
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
