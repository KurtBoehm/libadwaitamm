/*
 * Copyright (C) 2021 Purism SPC
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alexander Mikhaylenko <alexander.mikhaylenko@puri.sm>
 * Author: Kurt Böhm <kurbo96@gmail.com>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static double last_value;
static int done_count;

static void value_cb(double value) {
  last_value = value;
}

static void done_cb() {
  done_count++;
}

static void test_adw_animation_general() {
  Gtk::Button widget;
  auto target = Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  auto target2 = Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));

  auto animation = Adw::TimedAnimation::create(&widget, 10, 20, 100, target);

  last_value = 0;
  done_count = 0;

  animation->signal_done().connect(sigc::ptr_fun(done_cb));

  g_assert_true(animation->get_widget() == &widget);
  g_assert_true(animation->get_target()->gobj() == (AdwAnimationTarget*)target->gobj());

  g_assert_true(animation->get_state() == Adw::Animation::State::IDLE);
  g_assert_true(animation->get_value() == 10);
  g_assert_true(last_value == 0);
  g_assert_true(done_count == 0);

  animation->play();

  /* Since the widget is not mapped, the animation will immediately finish */
  g_assert_true(animation->get_state() == Adw::Animation::State::FINISHED);
  g_assert_true(animation->get_value() == 20);
  g_assert_true(last_value == 20);
  g_assert_true(done_count == 1);

  animation->reset();

  g_assert_true(animation->get_value() == 10);
  g_assert_true(last_value == 10);
  g_assert_true(done_count == 1);

  animation->skip();

  g_assert_true(animation->get_state() == Adw::Animation::State::FINISHED);
  g_assert_true(animation->get_value() == 20);
  g_assert_true(last_value == 20);
  g_assert_true(done_count == 2);

  animation->set_target(target2);
  g_assert_true(animation->get_target()->gobj() == (AdwAnimationTarget*)target2->gobj());

  g_assert_true(last_value == 20);
  g_assert_true(done_count == 2);
}

static void test_adw_animation_play_pause() {
  /* A mapped widget is required so the animation actually advances across
   * frames instead of finishing synchronously on play(), which is what lets
   * us observe PLAYING/PAUSED states in between. */
  Gtk::Window window;
  Gtk::Button widget;
  window.set_child(widget);
  window.show();

  auto target = Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  auto animation = Adw::TimedAnimation::create(&widget, 0, 1, 1000, target);

  last_value = -1;
  done_count = 0;
  animation->signal_done().connect(sigc::ptr_fun(done_cb));

  g_assert_true(animation->get_state() == Adw::Animation::State::IDLE);

  animation->play();
  g_assert_true(animation->get_state() == Adw::Animation::State::PLAYING);

  animation->pause();
  g_assert_true(animation->get_state() == Adw::Animation::State::PAUSED);

  animation->resume();
  g_assert_true(animation->get_state() == Adw::Animation::State::PLAYING);

  animation->skip();
  g_assert_true(animation->get_state() == Adw::Animation::State::FINISHED);
  g_assert_true(animation->get_value() == 1);
  g_assert_true(last_value == 1);
  g_assert_true(done_count == 1);
}

static void test_adw_animation_follow_enable_animations_setting() {
  Gtk::Button widget;
  auto target = Adw::CallbackAnimationTarget::create(sigc::ptr_fun(value_cb));
  auto animation = Adw::TimedAnimation::create(&widget, 0, 1, 100, target);

  /* Default is true */
  g_assert_true(animation->get_follow_enable_animations_setting());

  animation->set_follow_enable_animations_setting(false);
  g_assert_false(animation->get_follow_enable_animations_setting());

  animation->set_follow_enable_animations_setting(true);
  g_assert_true(animation->get_follow_enable_animations_setting());
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Animation/general", test_adw_animation_general);
  g_test_add_func("/Adwaita/Animation/play_pause", test_adw_animation_play_pause);
  g_test_add_func("/Adwaita/Animation/follow_enable_animations_setting",
                  test_adw_animation_follow_enable_animations_setting);

  return g_test_run();
}
