/*
 * Copyright (C) 2025 GNOME Foundation Inc.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Alice Mikhaylenko <alicem@gnome.org>
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static void test_adw_sidebar_item_title(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("");

  notified = 0;
  item->property_title().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring title = item->get_property<Glib::ustring>("title");
  g_assert_true(title == "");
  g_assert_true(notified == 0);

  item->set_title("Some title");
  g_assert_true(item->get_title() == "Some title");
  g_assert_true(notified == 1);

  item->set_property<Glib::ustring>("title", "Some other title");
  g_assert_true(item->get_title() == "Some other title");
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_subtitle(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_subtitle().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring subtitle = item->get_property<Glib::ustring>("subtitle");
  g_assert_true(subtitle == "");
  g_assert_true(notified == 0);

  item->set_subtitle("Some subtitle");
  g_assert_true(item->get_subtitle() == "Some subtitle");
  g_assert_true(notified == 1);

  item->set_property<Glib::ustring>("subtitle", "Some other subtitle");
  g_assert_true(item->get_subtitle() == "Some other subtitle");
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_use_underline(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_use_underline().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool use_underline = item->get_property<bool>("use-underline");
  g_assert_false(use_underline);
  g_assert_true(notified == 0);

  item->set_use_underline(true);
  g_assert_true(item->get_use_underline());
  g_assert_true(notified == 1);

  item->set_property<bool>("use-underline", false);
  g_assert_false(item->get_use_underline());
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_icon(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");
  int notified_name = 0;
  int notified_paintable = 0;

  auto notify_name_cb = [&notified_name]() { notified_name++; };
  auto notify_paintable_cb = [&notified_paintable]() { notified_paintable++; };

  item->property_icon_name().signal_changed().connect(notify_name_cb);
  item->property_icon_paintable().signal_changed().connect(notify_paintable_cb);

  Glib::ustring icon_name = item->get_property<Glib::ustring>("icon-name");
  Glib::RefPtr<Gdk::Paintable> icon_paintable =
      item->get_property<Glib::RefPtr<Gdk::Paintable>>("icon-paintable");

  g_assert_true(icon_name == "");
  g_assert_true(icon_paintable == nullptr);

  g_assert_true(notified_name == 0);
  g_assert_true(notified_paintable == 0);

  item->set_icon_name("something-symbolic");
  g_assert_true(item->get_icon_name() == "something-symbolic");
  g_assert_true(item->get_icon_paintable() == nullptr);
  g_assert_true(notified_name == 1);
  g_assert_true(notified_paintable == 0);

  item->set_property<Glib::ustring>("icon-name", "something-else-symbolic");
  g_assert_true(item->get_icon_name() == "something-else-symbolic");
  g_assert_true(item->get_icon_paintable() == nullptr);
  g_assert_true(notified_name == 2);
  g_assert_true(notified_paintable == 0);

  Glib::RefPtr<Adw::SpinnerPaintable> paintable =
      Adw::SpinnerPaintable::create();
  item->set_icon_paintable(paintable);
  g_assert_true(item->get_icon_name() == "");
  g_assert_true(item->get_icon_paintable() == paintable);
  g_assert_true(notified_name == 3);
  g_assert_true(notified_paintable == 1);

  item->set_icon_paintable(Glib::RefPtr<Gdk::Paintable>());
  g_assert_true(item->get_icon_name() == "");
  g_assert_true(item->get_icon_paintable() == nullptr);
  g_assert_true(notified_name == 3);
  g_assert_true(notified_paintable == 2);
}

#if ADW_CHECK_VERSION(1, 10, 0)
static void test_adw_sidebar_item_prefix(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_prefix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* prefix = item->get_property<Gtk::Widget*>("prefix");
  g_assert_true(prefix == nullptr);
  g_assert_true(notified == 0);

  Gtk::Switch* widget = Gtk::make_managed<Gtk::Switch>();
  item->set_prefix(widget);
  g_assert_true(item->get_prefix() == widget);
  g_assert_true(notified == 1);

  item->set_property<Gtk::Widget*>("prefix", nullptr);
  g_assert_true(item->get_prefix() == nullptr);
  g_assert_true(notified == 2);
}
#endif

static void test_adw_sidebar_item_suffix(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_suffix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* suffix = item->get_property<Gtk::Widget*>("suffix");
  g_assert_true(suffix == nullptr);
  g_assert_true(notified == 0);

  Gtk::Switch* widget = Gtk::make_managed<Gtk::Switch>();
  item->set_suffix(widget);
  g_assert_true(item->get_suffix() == widget);
  g_assert_true(notified == 1);

  item->set_property<Gtk::Widget*>("suffix", nullptr);
  g_assert_true(item->get_suffix() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_visible(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_visible().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool visible = item->get_property<bool>("visible");
  g_assert_true(visible);
  g_assert_true(notified == 0);

  item->set_visible(false);
  g_assert_false(item->get_visible());
  g_assert_true(notified == 1);

  item->set_property<bool>("visible", true);
  g_assert_true(item->get_visible());
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_enabled(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_enabled().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool enabled = item->get_property<bool>("enabled");
  g_assert_true(enabled);
  g_assert_true(notified == 0);

  item->set_enabled(false);
  g_assert_false(item->get_enabled());
  g_assert_true(notified == 1);

  item->set_property<bool>("enabled", true);
  g_assert_true(item->get_enabled());
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_drag_motion_activate(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");

  notified = 0;
  item->property_drag_motion_activate().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool drag_motion_activate = item->get_property<bool>("drag-motion-activate");
  g_assert_true(drag_motion_activate);
  g_assert_true(notified == 0);

  item->set_drag_motion_activate(false);
  g_assert_false(item->get_drag_motion_activate());
  g_assert_true(notified == 1);

  item->set_property<bool>("drag-motion-activate", true);
  g_assert_true(item->get_drag_motion_activate());
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_item_get_section(void) {
  Glib::RefPtr<Adw::SidebarItem> item = Adw::SidebarItem::create("Item");
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();

  notified = 0;
  item->property_section().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(item->get_section() == nullptr);
  g_assert_true(notified == 0);

  section->append(item);
  g_assert_true(item->get_section() == section);
  g_assert_true(notified == 1);

  section->remove(item);
  g_assert_true(item->get_section() == nullptr);
  g_assert_true(notified == 2);

  section->append(item);
  g_assert_true(item->get_section() == section);
  g_assert_true(notified == 3);

  section->remove_all();
  g_assert_true(item->get_section() == nullptr);
  g_assert_true(notified == 4);
}

static void test_adw_sidebar_item_get_section_index(void) {
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();
  Glib::RefPtr<Adw::SidebarItem> item1 = Adw::SidebarItem::create("Item 1");
  Glib::RefPtr<Adw::SidebarItem> item2 = Adw::SidebarItem::create("Item 2");
  Glib::RefPtr<Adw::SidebarItem> item3 = Adw::SidebarItem::create("Item 3");

  g_assert_true(item1->get_section_index() == 0);
  g_assert_true(item2->get_section_index() == 0);
  g_assert_true(item3->get_section_index() == 0);

  section->prepend(item1);
  g_assert_true(item1->get_section_index() == 0);
  g_assert_true(item2->get_section_index() == 0);
  g_assert_true(item3->get_section_index() == 0);

  section->prepend(item2);
  g_assert_true(item1->get_section_index() == 1);
  g_assert_true(item2->get_section_index() == 0);
  g_assert_true(item3->get_section_index() == 0);

  section->append(item3);
  g_assert_true(item1->get_section_index() == 1);
  g_assert_true(item2->get_section_index() == 0);
  g_assert_true(item3->get_section_index() == 2);

  section->remove_all();
  g_assert_true(item1->get_section_index() == 0);
  g_assert_true(item2->get_section_index() == 0);
  g_assert_true(item3->get_section_index() == 0);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/SidebarItem/title", test_adw_sidebar_item_title);
  g_test_add_func("/Adwaita/SidebarItem/subtitle",
                  test_adw_sidebar_item_subtitle);
  g_test_add_func("/Adwaita/SidebarItem/use_underline",
                  test_adw_sidebar_item_use_underline);
  g_test_add_func("/Adwaita/SidebarItem/icon", test_adw_sidebar_item_icon);
#if ADW_CHECK_VERSION(1, 10, 0)
  g_test_add_func("/Adwaita/SidebarItem/prefix", test_adw_sidebar_item_prefix);
#endif
  g_test_add_func("/Adwaita/SidebarItem/suffix", test_adw_sidebar_item_suffix);
  g_test_add_func("/Adwaita/SidebarItem/visible",
                  test_adw_sidebar_item_visible);
  g_test_add_func("/Adwaita/SidebarItem/enabled",
                  test_adw_sidebar_item_enabled);
  g_test_add_func("/Adwaita/SidebarItem/drag_motion_activate",
                  test_adw_sidebar_item_drag_motion_activate);
  g_test_add_func("/Adwaita/SidebarItem/get_section",
                  test_adw_sidebar_item_get_section);
  g_test_add_func("/Adwaita/SidebarItem/get_section_index",
                  test_adw_sidebar_item_get_section_index);

  return g_test_run();
}
