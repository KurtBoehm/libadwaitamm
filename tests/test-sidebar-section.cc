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

static void check_items(const Glib::RefPtr<Adw::SidebarSection>& section,
                        std::initializer_list<Glib::ustring> titles) {
  Glib::RefPtr<Gio::ListModel> items = section->get_items();

  g_assert_true(items->get_n_items() == titles.size());

  guint i = 0;
  for (const Glib::ustring& title : titles) {
    Glib::RefPtr<Adw::SidebarItem> item =
        std::dynamic_pointer_cast<Adw::SidebarItem>(items->get_object(i));

    g_assert_true(item == section->get_item(i));

    g_assert_true(item->get_title() == title);
    g_assert_true(item->get_section_index() == i);
    g_assert_true(item->get_section() == section);

    i++;
  }
}

static void test_adw_sidebar_section_title(void) {
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();

  notified = 0;
  section->property_title().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring title = section->get_property<Glib::ustring>("title");
  g_assert_true(title == "");
  g_assert_true(notified == 0);

  section->set_title("Some title");
  g_assert_true(section->get_title() == "Some title");
  g_assert_true(notified == 1);

  section->set_property<Glib::ustring>("title", "Some other title");
  g_assert_true(section->get_title() == "Some other title");
  g_assert_true(notified == 2);
}

#if ADW_CHECK_VERSION(1, 10, 0)
static void test_adw_sidebar_section_suffix(void) {
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();

  notified = 0;
  section->property_suffix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* suffix = section->get_property<Gtk::Widget*>("suffix");
  g_assert_true(suffix == nullptr);
  g_assert_true(notified == 0);

  Gtk::Switch* widget = Gtk::make_managed<Gtk::Switch>();
  section->set_suffix(widget);
  g_assert_true(section->get_suffix() == widget);
  g_assert_true(notified == 1);

  section->set_property<Gtk::Widget*>("suffix", nullptr);
  g_assert_true(section->get_suffix() == nullptr);
  g_assert_true(notified == 2);
}
#endif

static void test_adw_sidebar_section_menu_model(void) {
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();

  Glib::RefPtr<Gio::MenuModel> model1 = Gio::Menu::create();
  Glib::RefPtr<Gio::MenuModel> model2 = Gio::Menu::create();

  notified = 0;
  section->property_menu_model().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::MenuModel> model =
      section->get_property<Glib::RefPtr<Gio::MenuModel>>("menu-model");
  g_assert_true(model == nullptr);
  g_assert_true(notified == 0);

  section->set_menu_model(model1);
  g_assert_true(section->get_menu_model() == model1);
  g_assert_true(notified == 1);

  section->set_property<Glib::RefPtr<Gio::MenuModel>>("menu-model", model2);
  g_assert_true(section->get_menu_model() == model2);
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_section_add_remove(void) {
  Adw::Sidebar sidebar;
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();
  Glib::RefPtr<Adw::SidebarItem> item1 = Adw::SidebarItem::create("Item 1");
  Glib::RefPtr<Adw::SidebarItem> item2 = Adw::SidebarItem::create("Item 2");
  Glib::RefPtr<Adw::SidebarItem> item3 = Adw::SidebarItem::create("Item 3");

  sidebar.append(section);

  check_items(section, {});

  section->append(item1);
  check_items(section, {"Item 1"});
  section->append(item2);
  check_items(section, {"Item 1", "Item 2"});
  section->append(item3);
  check_items(section, {"Item 1", "Item 2", "Item 3"});

  section->remove_all();
  check_items(section, {});

  section->prepend(item1);
  check_items(section, {"Item 1"});
  section->prepend(item2);
  check_items(section, {"Item 2", "Item 1"});
  section->prepend(item3);
  check_items(section, {"Item 3", "Item 2", "Item 1"});

  section->remove(item2);
  check_items(section, {"Item 3", "Item 1"});

  section->remove_all();
  check_items(section, {});

  section->insert(item1, 1);
  check_items(section, {"Item 1"});
  section->insert(item2, 1);
  check_items(section, {"Item 1", "Item 2"});
  section->insert(item3, 1);
  check_items(section, {"Item 1", "Item 3", "Item 2"});

  section->remove_all();
  check_items(section, {});

  section->insert(item1, -1);
  check_items(section, {"Item 1"});
  section->insert(item2, -1);
  check_items(section, {"Item 1", "Item 2"});
  section->insert(item3, -1);
  check_items(section, {"Item 1", "Item 2", "Item 3"});
}

static void test_adw_sidebar_section_bind_model(void) {
  Adw::Sidebar sidebar;
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();
  Glib::RefPtr<Gtk::StringList> list = Gtk::StringList::create();

  list->append("Item 1");
  list->append("Item 2");
  list->append("Item 3");

  sidebar.append(section);

  section->append(Adw::SidebarItem::create("Item"));
  check_items(section, {"Item"});

  section->bind_model(list,
                      [](const Glib::RefPtr<Glib::Object>& item)
                          -> Glib::RefPtr<Adw::SidebarItem> {
                        Glib::RefPtr<Gtk::StringObject> string_object =
                            std::dynamic_pointer_cast<Gtk::StringObject>(item);
                        return Adw::SidebarItem::create(
                            string_object->get_string());
                      });
  check_items(section, {"Item 1", "Item 2", "Item 3"});

  list->append("Item 4");
  check_items(section, {"Item 1", "Item 2", "Item 3", "Item 4"});

  list->remove(2);
  check_items(section, {"Item 1", "Item 2", "Item 4"});

  list->splice(1, 2, {});
  check_items(section, {"Item 1"});

  section->bind_model(Glib::RefPtr<Gio::ListModel>(),
                      sigc::slot<Glib::RefPtr<Adw::SidebarItem>(
                          const Glib::RefPtr<Glib::Object>&)>());
  check_items(section, {});

  section->append(Adw::SidebarItem::create("Item"));
  check_items(section, {"Item"});
}

static void test_adw_sidebar_section_get_sidebar(void) {
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();
  Adw::Sidebar sidebar;

  notified = 0;
  section->property_sidebar().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(section->get_sidebar() == nullptr);
  g_assert_true(notified == 0);

  sidebar.append(section);
  g_assert_true(section->get_sidebar() == &sidebar);
  g_assert_true(notified == 1);

  sidebar.remove(section);
  g_assert_true(section->get_sidebar() == nullptr);
  g_assert_true(notified == 2);

  sidebar.append(section);
  g_assert_true(section->get_sidebar() == &sidebar);
  g_assert_true(notified == 3);

  sidebar.remove_all();
  g_assert_true(section->get_sidebar() == nullptr);
  g_assert_true(notified == 4);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/SidebarSection/title",
                  test_adw_sidebar_section_title);
#if ADW_CHECK_VERSION(1, 10, 0)
  g_test_add_func("/Adwaita/SidebarSection/suffix",
                  test_adw_sidebar_section_suffix);
#endif
  g_test_add_func("/Adwaita/SidebarSection/menu_model",
                  test_adw_sidebar_section_menu_model);
  g_test_add_func("/Adwaita/SidebarSection/add_remove",
                  test_adw_sidebar_section_add_remove);
  g_test_add_func("/Adwaita/SidebarSection/bind_model",
                  test_adw_sidebar_section_bind_model);
  g_test_add_func("/Adwaita/SidebarSection/get_sidebar",
                  test_adw_sidebar_section_get_sidebar);

  return g_test_run();
}
