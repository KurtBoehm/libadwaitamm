/*
 * Copyright (C) 2022 George Barrett <bob@bob131.so>
 * Copyright (C) 2026 Kurt Böhm <kurbo64@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_property_animation_target_construct() {
  auto widget = Glib::RefPtr<Gtk::Button>(new Gtk::Button());

  auto named_target = Adw::PropertyAnimationTarget::create(widget, "opacity");

  GParamSpec* target_pspec = named_target->get_pspec();
  g_assert_true(target_pspec);
  g_assert_true(Glib::ustring(target_pspec->name) == "opacity");
  g_assert_true(named_target->get_object() == widget);

  auto pspec_target = Adw::PropertyAnimationTarget::create(widget, target_pspec);
  g_assert_true(pspec_target->get_pspec() == target_pspec);
  g_assert_true(pspec_target->get_object() == widget);

  target_pspec = pspec_target->get_pspec();
  g_assert_true(target_pspec);
  g_assert_true(Glib::ustring(target_pspec->name) == "opacity");
}

static void test_adw_property_animation_target_basic() {
  auto widget = Glib::RefPtr<Gtk::Button>(new Gtk::Button());

  auto target = Adw::PropertyAnimationTarget::create(widget, "opacity");
  auto animation = Adw::TimedAnimation::create(widget.get(), 1, 0, 0, target);

  g_assert_true(widget->get_opacity() == 1);

  animation->play();
  g_assert_true(widget->get_opacity() == 0);
}

static void test_adw_none_animation_target_basic() {
  auto widget = Glib::RefPtr<Gtk::Button>(new Gtk::Button());

  auto target = Adw::NoneAnimationTarget::create();
  g_assert_true(target);

  // A "none" target should be a no-op: it can be attached and played
  // without affecting the object it's nominally associated with.
  auto animation = Adw::TimedAnimation::create(widget.get(), 1, 0, 0, target);

  g_assert_true(widget->get_opacity() == 1);

  animation->play();
  g_assert_true(widget->get_opacity() == 1);
}

static void test_adw_callback_animation_target_basic() {
  auto widget = Glib::RefPtr<Gtk::Button>(new Gtk::Button());

  double received_value = -1;
  int call_count = 0;

  auto target = Adw::CallbackAnimationTarget::create([&received_value, &call_count](double value) {
    received_value = value;
    ++call_count;
  });
  g_assert_true(target);

  auto animation = Adw::TimedAnimation::create(widget.get(), 1, 0, 0, target);

  animation->play();

  g_assert_true(call_count > 0);
  g_assert_true(received_value == 0);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/PropertyAnimationTarget/construct",
                  test_adw_property_animation_target_construct);
  g_test_add_func("/Adwaita/PropertyAnimationTarget/basic",
                  test_adw_property_animation_target_basic);
  g_test_add_func("/Adwaita/NoneAnimationTarget/basic", test_adw_none_animation_target_basic);
  g_test_add_func("/Adwaita/CallbackAnimationTarget/basic",
                  test_adw_callback_animation_target_basic);

  return g_test_run();
}
