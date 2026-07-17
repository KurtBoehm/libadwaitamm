/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <cmath>
#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static inline int rgba_to_hex(const Gdk::RGBA &rgba) {
  return (int)(std::round(rgba.get_red() * 0xff)) << 16 |
         (int)(std::round(rgba.get_green() * 0xff)) << 8 |
         (int)(std::round(rgba.get_blue() * 0xff));
}

static inline Gdk::RGBA hex_to_rgba(int hex) {
  Gdk::RGBA rgba;
  rgba.set_red((float)((hex >> 16) & 0xff) / 0xff);
  rgba.set_green((float)((hex >> 8) & 0xff) / 0xff);
  rgba.set_blue((float)(hex & 0xff) / 0xff);
  rgba.set_alpha(1);
  return rgba;
}

static void test_adw_accent_color_to_rgba(void) {
  Gdk::RGBA rgba;

  Adw::accent_color_to_rgba(Adw::AccentColor::BLUE, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x3584e4);

  Adw::accent_color_to_rgba(Adw::AccentColor::TEAL, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x2190a4);

  Adw::accent_color_to_rgba(Adw::AccentColor::GREEN, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x3a944a);

  Adw::accent_color_to_rgba(Adw::AccentColor::YELLOW, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0xc88800);

  Adw::accent_color_to_rgba(Adw::AccentColor::ORANGE, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0xed5b00);

  Adw::accent_color_to_rgba(Adw::AccentColor::RED, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0xe62d42);

  Adw::accent_color_to_rgba(Adw::AccentColor::PINK, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0xd56199);

  Adw::accent_color_to_rgba(Adw::AccentColor::PURPLE, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x9141ac);

  Adw::accent_color_to_rgba(Adw::AccentColor::SLATE, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x6f8396);
}

static void test_adw_accent_color_to_standalone_rgba(void) {
  Gdk::RGBA rgba;

  Adw::accent_color_to_standalone_rgba(Adw::AccentColor::BLUE, false, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x0461be);

  Adw::accent_color_to_standalone_rgba(Adw::AccentColor::BLUE, true, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x81d0ff);

  Adw::accent_color_to_standalone_rgba(Adw::AccentColor::SLATE, false, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0x526678);

  Adw::accent_color_to_standalone_rgba(Adw::AccentColor::SLATE, true, rgba);
  g_assert_cmphex(rgba_to_hex(rgba), ==, 0xbbd1e5);
}

static void test_adw_accent_color_rgba_to_standalone(void) {
  Gdk::RGBA rgba = hex_to_rgba(0x3584e4);
  Gdk::RGBA standalone;

  Adw::rgba_to_standalone(rgba, false, standalone);
  g_assert_cmphex(rgba_to_hex(standalone), ==, 0x0461be);

  Adw::rgba_to_standalone(rgba, true, standalone);
  g_assert_cmphex(rgba_to_hex(standalone), ==, 0x81d0ff);

  rgba = hex_to_rgba(0xffffff);
  Adw::rgba_to_standalone(rgba, false, standalone);
  g_assert_cmphex(rgba_to_hex(standalone), ==, 0x636363);
  Adw::rgba_to_standalone(rgba, true, standalone);
  g_assert_cmphex(rgba_to_hex(standalone), ==, 0xffffff);

  rgba = hex_to_rgba(0x000000);
  Adw::rgba_to_standalone(rgba, false, standalone);
  g_assert_cmphex(rgba_to_hex(standalone), ==, 0x000000);
  Adw::rgba_to_standalone(rgba, true, standalone);
  g_assert_cmphex(rgba_to_hex(standalone), ==, 0xcecece);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/AccentColor/to_rgba",
                  test_adw_accent_color_to_rgba);
  g_test_add_func("/Adwaita/AccentColor/to_standalone_rgba",
                  test_adw_accent_color_to_standalone_rgba);
  g_test_add_func("/Adwaita/AccentColor/rgba_to_standalone",
                  test_adw_accent_color_rgba_to_standalone);

  return g_test_run();
}
