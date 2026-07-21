/*
 * Copyright (C) 2026 Jamie Murphy <jmurphy@gnome.org>
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_css_class_binding_default(void) {
  Adw::Bin source, target;

  source.set_visible(false);

  Glib::RefPtr<Adw::CssClassBinding> binding =
      Adw::CssClassBinding::bind_property_to_css_class(
          Glib::wrap(source.gobj(), true), "visible", &target, "test-class");

  g_assert_true(binding->get_source()->gobj() == source.gobj());
  g_assert_true(binding->get_target() == &target);
  g_assert_true(binding->get_flags() == Glib::Binding::Flags::DEFAULT);
  g_assert_true(binding->get_source_property() == "visible");
  g_assert_true(binding->get_target_css_class() == "test-class");

  source.set_visible(true);
  g_assert_true(target.has_css_class("test-class"));

  source.set_visible(false);
  g_assert_false(target.has_css_class("test-class"));

  binding->unbind();

  source.set_visible(true);
  g_assert_false(target.has_css_class("test-class"));
}

static void test_adw_css_class_binding_custom_map(void) {
  Adw::Bin source, target;
  bool called = false;

  Glib::RefPtr<Adw::CssClassBinding> binding =
      Adw::CssClassBinding::bind_property_to_css_class_full(
          Glib::wrap(source.gobj(), true), "opacity", &target, "test-class",
          Glib::Binding::Flags::DEFAULT,
          [&called](const Glib::ValueBase& value) {
            called = true;
            return g_value_get_double(value.gobj()) < 0.5;
          });

  source.set_opacity(0.0);
  g_assert_true(called);
  g_assert_true(target.has_css_class("test-class"));

  source.set_opacity(1.0);
  g_assert_false(target.has_css_class("test-class"));

  binding->unbind();
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/CssClassBinding/default",
                  test_adw_css_class_binding_default);
  g_test_add_func("/Adwaita/CssClassBinding/custom_map",
                  test_adw_css_class_binding_custom_map);

  return g_test_run();
}
