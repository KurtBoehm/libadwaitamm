/*
 * Copyright (C) 2024 Alice Mikhaylenko <alicem@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void test_adw_spinner_paintable_new() {
  Glib::RefPtr<Adw::SpinnerPaintable> paintable = Adw::SpinnerPaintable::create();
  g_assert_true(paintable != nullptr);
}

static void test_adw_spinner_paintable_new_with_widget() {
  Gtk::Image* image = new Gtk::Image();

  Glib::RefPtr<Adw::SpinnerPaintable> paintable = Adw::SpinnerPaintable::create(image);
  g_assert_true(paintable != nullptr);

  image->set(paintable);

  delete image;
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/SpinnerPaintable/new", test_adw_spinner_paintable_new);
  g_test_add_func("/Adwaita/SpinnerPaintable/new_with_widget",
                  test_adw_spinner_paintable_new_with_widget);

  return g_test_run();
}
