/*
 * Copyright (C) 2020 Purism SPC
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

#define TEST_ICON_NAME "adw-avatar-default-symbolic"
#define TEST_STRING "Mario Rossi"
#define TEST_SIZE 128

static void test_adw_avatar_icon_name() {
  Adw::Avatar avatar(128, "", true);
  g_assert_true(avatar.get_icon_name() == "");

  avatar.set_icon_name(TEST_ICON_NAME);
  g_assert_true(avatar.get_icon_name() == TEST_ICON_NAME);
}

static void test_adw_avatar_text() {
  Adw::Avatar avatar(128, "", true);
  g_assert_true(avatar.get_text() == "");

  avatar.set_text(TEST_STRING);
  g_assert_true(avatar.get_text() == TEST_STRING);
}

static void test_adw_avatar_size() {
  Adw::Avatar avatar(TEST_SIZE, "", true);

  g_assert_true(avatar.get_size() == TEST_SIZE);
  avatar.set_size(TEST_SIZE / 2);
  g_assert_true(avatar.get_size() == TEST_SIZE / 2);
}

static void test_adw_avatar_show_initials() {
  Adw::Avatar avatar(128, TEST_STRING, true);
  g_assert_true(avatar.get_show_initials());

  avatar.set_show_initials(false);
  g_assert_false(avatar.get_show_initials());

  avatar.set_show_initials(true);
  g_assert_true(avatar.get_show_initials());
}

static void test_adw_avatar_custom_image() {
  Adw::Avatar avatar(128, TEST_STRING, true);
  g_assert_true(!avatar.get_custom_image());

  // A 1x1 fully-transparent texture is enough to exercise the setter/getter
  // without pulling in file I/O.
  auto bytes = Glib::Bytes::create("\x00\x00\x00\x00", 4);
  auto texture = Gdk::MemoryTexture::create(1, 1, Gdk::MemoryTexture::Format::R8G8B8A8, bytes, 4);

  avatar.set_custom_image(texture);
  g_assert_true(static_cast<bool>(avatar.get_custom_image()));
  g_assert_true(avatar.get_custom_image() == texture);
}

static void test_adw_avatar_draw_to_texture() {
  Gtk::Window window;
  Adw::Avatar avatar(TEST_SIZE, TEST_STRING, true);
  window.set_child(avatar);
  window.set_visible(true);

  // Ensure the window is actually realized and has a native surface
  // (and thus a GskRenderer) before we try to render into a texture.
  gtk_test_widget_wait_for_draw(GTK_WIDGET(window.gobj()));

  auto texture = avatar.draw_to_texture(1);
  g_assert_true(static_cast<bool>(texture));
  g_assert_cmpint(texture->get_width(), >, 0);
  g_assert_cmpint(texture->get_height(), >, 0);

  auto texture_2x = avatar.draw_to_texture(2);
  g_assert_true(static_cast<bool>(texture_2x));
  g_assert_cmpint(texture_2x->get_width(), ==, texture->get_width() * 2);
  g_assert_cmpint(texture_2x->get_height(), ==, texture->get_height() * 2);

  window.set_visible(false);
}

static void test_adw_avatar_default_ctor() {
  Adw::Avatar avatar;
  g_assert_true(avatar.get_text() == "");
  g_assert_true(avatar.get_icon_name() == "");
  g_assert_true(!avatar.get_custom_image());
}

static void test_adw_avatar_properties() {
  Adw::Avatar avatar(TEST_SIZE, "", true);

  avatar.property_text() = TEST_STRING;
  g_assert_true(avatar.get_text() == TEST_STRING);
  g_assert_true(avatar.property_text().get_value() == TEST_STRING);

  avatar.property_icon_name() = TEST_ICON_NAME;
  g_assert_true(avatar.get_icon_name() == TEST_ICON_NAME);

  avatar.property_show_initials() = false;
  g_assert_false(avatar.get_show_initials());

  avatar.property_size() = TEST_SIZE / 4;
  g_assert_cmpint(avatar.property_size().get_value(), ==, TEST_SIZE / 4);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Avatar/icon_name", test_adw_avatar_icon_name);
  g_test_add_func("/Adwaita/Avatar/text", test_adw_avatar_text);
  g_test_add_func("/Adwaita/Avatar/size", test_adw_avatar_size);
  g_test_add_func("/Adwaita/Avatar/show_initials", test_adw_avatar_show_initials);
  g_test_add_func("/Adwaita/Avatar/custom_image", test_adw_avatar_custom_image);
  g_test_add_func("/Adwaita/Avatar/draw_to_texture", test_adw_avatar_draw_to_texture);
  g_test_add_func("/Adwaita/Avatar/default_ctor", test_adw_avatar_default_ctor);
  g_test_add_func("/Adwaita/Avatar/properties", test_adw_avatar_properties);

  return g_test_run();
}
