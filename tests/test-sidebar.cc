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

static void notify_cb() {
  ++notified;
}

static Glib::RefPtr<Adw::SidebarSection>
create_section(const Glib::ustring& title, std::initializer_list<Glib::ustring> item_titles) {
  Glib::RefPtr<Adw::SidebarSection> section = Adw::SidebarSection::create();

  section->set_title(title);

  for (const Glib::ustring& item_title : item_titles) {
    section->append(Adw::SidebarItem::create(item_title));
  }

  return section;
}

static void check_items(const Adw::Sidebar& sidebar, guint selected,
                        std::initializer_list<Glib::ustring> titles) {
  Glib::RefPtr<Gtk::SelectionModel> items = sidebar.get_items();
  Glib::RefPtr<Gio::ListModel> items_model = std::dynamic_pointer_cast<Gio::ListModel>(items);

  g_assert_true(sidebar.get_selected() == selected);
  g_assert_true(items_model->get_n_items() == titles.size());

  guint i = 0;
  for (const Glib::ustring& title : titles) {
    Glib::RefPtr<Adw::SidebarItem> item =
      std::dynamic_pointer_cast<Adw::SidebarItem>(items_model->get_object(i));

    g_assert_true(item == sidebar.get_item(i));

    g_assert_true(item->get_title() == title);
    g_assert_true(item->get_index() == i);

    if (i == selected) {
      g_assert_true(sidebar.get_selected_item() == item);
      g_assert_true(items->is_selected(i));
    } else {
      g_assert_false(items->is_selected(i));
    }

    i++;
  }
}

static void check_sections(const Adw::Sidebar& sidebar,
                           std::initializer_list<Glib::ustring> titles) {
  std::vector<Glib::RefPtr<Adw::SidebarSection>> sections = sidebar.get_sections();
  Glib::RefPtr<Gtk::SelectionModel> items = sidebar.get_items();
  Glib::RefPtr<Gio::ListModel> items_model = std::dynamic_pointer_cast<Gio::ListModel>(items);
  Glib::RefPtr<Gtk::SectionModel> items_sections =
    std::dynamic_pointer_cast<Gtk::SectionModel>(items);

  g_assert_true(sections.size() == titles.size());

  guint i = 0;
  guint section_start = 0;
  for (const Glib::ustring& title : titles) {
    Glib::RefPtr<Adw::SidebarSection> section = sections[i];
    Glib::RefPtr<Gio::ListModel> section_items = section->get_items();

    g_assert_true(section == sidebar.get_section(i));

    g_assert_true(section->get_title() == title);
    g_assert_true(section->get_sidebar() == &sidebar);

    guint n_items = section_items->get_n_items();
    for (guint j = 0; j < n_items; j++) {
      Glib::RefPtr<Adw::SidebarItem> item1 =
        std::dynamic_pointer_cast<Adw::SidebarItem>(section_items->get_object(j));
      Glib::RefPtr<Adw::SidebarItem> item2 =
        std::dynamic_pointer_cast<Adw::SidebarItem>(items_model->get_object(section_start + j));

      g_assert_true(item1 == item2);
      g_assert_true(item2->get_section() == section);
      g_assert_true(item2->get_section_index() == j);

      std::pair<guint, guint> range = items_sections->get_section(section_start + j);

      g_assert_true(range.first == section_start);
      g_assert_true(range.second == section_start + n_items);
    }

    section_start += n_items;
    i++;
  }
}

static void test_adw_sidebar_mode() {
  Adw::Sidebar sidebar;

  notified = 0;
  sidebar.property_mode().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Adw::Sidebar::Mode mode = sidebar.get_property<Adw::Sidebar::Mode>("mode");
  g_assert_true(mode == Adw::Sidebar::Mode::SIDEBAR);

  sidebar.set_mode(Adw::Sidebar::Mode::PAGE);
  g_assert_true(sidebar.get_mode() == Adw::Sidebar::Mode::PAGE);
  g_assert_true(notified == 1);

  sidebar.set_property<Adw::Sidebar::Mode>("mode", Adw::Sidebar::Mode::SIDEBAR);
  g_assert_true(sidebar.get_mode() == Adw::Sidebar::Mode::SIDEBAR);
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_filter() {
  Adw::Sidebar sidebar;

  notified = 0;
  sidebar.property_filter().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gtk::Filter> filter = sidebar.get_property<Glib::RefPtr<Gtk::Filter>>("filter");
  g_assert_true(filter == nullptr);

  sidebar.set_filter(Glib::RefPtr<Gtk::Filter>());
  g_assert_true(notified == 0);

  filter = Gtk::BoolFilter::create(Glib::RefPtr<Gtk::Expression<bool>>());
  sidebar.set_filter(filter);
  g_assert_true(sidebar.get_filter() == filter);
  g_assert_true(notified == 1);

  sidebar.set_property<Glib::RefPtr<Gtk::Filter>>("filter", Glib::RefPtr<Gtk::Filter>());
  g_assert_true(sidebar.get_filter() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_sidebar_placeholder() {
  Adw::Sidebar sidebar;

  notified = 0;
  sidebar.property_placeholder().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* placeholder = sidebar.get_property<Gtk::Widget*>("placeholder");
  g_assert_true(placeholder == nullptr);

  sidebar.set_placeholder(nullptr);
  g_assert_true(notified == 0);

  Gtk::Button* widget = Gtk::make_managed<Gtk::Button>();
  sidebar.set_placeholder(widget);
  g_assert_true(sidebar.get_placeholder() == widget);
  g_assert_true(notified == 1);

  sidebar.set_property<Gtk::Widget*>("placeholder", nullptr);
  g_assert_true(sidebar.get_placeholder() == nullptr);
  g_assert_true(notified == 2);
}

#if ADW_CHECK_VERSION(1, 10, 0)
static void test_adw_sidebar_prefix() {
  Adw::Sidebar sidebar;
  Gtk::Button* prefix1 = new Gtk::Button();
  Gtk::Button* prefix2 = new Gtk::Button();

  notified = 0;
  sidebar.property_prefix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* prefix = sidebar.get_property<Gtk::Widget*>("prefix");
  g_assert_true(prefix == nullptr);

  sidebar.set_prefix(nullptr);
  g_assert_true(notified == 0);

  sidebar.set_prefix(prefix1);
  g_assert_true(sidebar.get_prefix() == prefix1);
  g_assert_true(notified == 1);

  sidebar.set_mode(Adw::Sidebar::Mode::PAGE);

  sidebar.set_property<Gtk::Widget*>("prefix", nullptr);
  g_assert_true(sidebar.get_prefix() == nullptr);
  g_assert_true(notified == 2);

  sidebar.set_prefix(prefix2);
  g_assert_true(sidebar.get_prefix() == prefix2);
  g_assert_true(notified == 3);

  sidebar.set_mode(Adw::Sidebar::Mode::SIDEBAR);

  delete prefix1;
  delete prefix2;
}

static void test_adw_sidebar_suffix() {
  Adw::Sidebar sidebar;
  Gtk::Button* suffix1 = new Gtk::Button();
  Gtk::Button* suffix2 = new Gtk::Button();

  notified = 0;
  sidebar.property_suffix().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* suffix = sidebar.get_property<Gtk::Widget*>("suffix");
  g_assert_true(suffix == nullptr);

  sidebar.set_suffix(nullptr);
  g_assert_true(notified == 0);

  sidebar.set_suffix(suffix1);
  g_assert_true(sidebar.get_suffix() == suffix1);
  g_assert_true(notified == 1);

  sidebar.set_mode(Adw::Sidebar::Mode::PAGE);

  sidebar.set_property<Gtk::Widget*>("suffix", nullptr);
  g_assert_true(sidebar.get_suffix() == nullptr);
  g_assert_true(notified == 2);

  sidebar.set_suffix(suffix2);
  g_assert_true(sidebar.get_suffix() == suffix2);
  g_assert_true(notified == 3);

  sidebar.set_mode(Adw::Sidebar::Mode::SIDEBAR);

  delete suffix1;
  delete suffix2;
}
#endif

static void test_adw_sidebar_add_remove() {
  Adw::Sidebar sidebar;
  Glib::RefPtr<Adw::SidebarSection> section1 = create_section("Section 1", {"1"});
  Glib::RefPtr<Adw::SidebarSection> section2 = create_section("Section 2", {"2a", "2b"});
  Glib::RefPtr<Adw::SidebarSection> section3 = create_section("Section 3", {"3a", "3b", "3c"});

  check_items(sidebar, GTK_INVALID_LIST_POSITION, {});
  check_sections(sidebar, {});

  sidebar.append(section1);
  check_items(sidebar, 0, {"1"});
  check_sections(sidebar, {"Section 1"});

  sidebar.append(section2);
  check_items(sidebar, 0, {"1", "2a", "2b"});
  check_sections(sidebar, {"Section 1", "Section 2"});

  sidebar.append(section3);
  check_items(sidebar, 0, {"1", "2a", "2b", "3a", "3b", "3c"});
  check_sections(sidebar, {"Section 1", "Section 2", "Section 3"});

  sidebar.remove_all();
  check_items(sidebar, GTK_INVALID_LIST_POSITION, {});
  check_sections(sidebar, {});

  sidebar.prepend(section1);
  check_items(sidebar, 0, {"1"});
  check_sections(sidebar, {"Section 1"});

  sidebar.prepend(section2);
  check_items(sidebar, 2, {"2a", "2b", "1"});
  check_sections(sidebar, {"Section 2", "Section 1"});

  sidebar.prepend(section3);
  check_items(sidebar, 5, {"3a", "3b", "3c", "2a", "2b", "1"});
  check_sections(sidebar, {"Section 3", "Section 2", "Section 1"});

  sidebar.remove(section2);
  check_items(sidebar, 3, {"3a", "3b", "3c", "1"});
  check_sections(sidebar, {"Section 3", "Section 1"});

  sidebar.remove(section1);
  check_items(sidebar, GTK_INVALID_LIST_POSITION, {"3a", "3b", "3c"});
  check_sections(sidebar, {"Section 3"});

  sidebar.remove_all();
  check_items(sidebar, GTK_INVALID_LIST_POSITION, {});
  check_sections(sidebar, {});

  sidebar.insert(section1, 1);
  check_items(sidebar, 0, {"1"});
  check_sections(sidebar, {"Section 1"});

  sidebar.insert(section2, 1);
  check_items(sidebar, 0, {"1", "2a", "2b"});
  check_sections(sidebar, {"Section 1", "Section 2"});

  sidebar.insert(section3, 1);
  check_items(sidebar, 0, {"1", "3a", "3b", "3c", "2a", "2b"});
  check_sections(sidebar, {"Section 1", "Section 3", "Section 2"});

  sidebar.remove_all();
  check_items(sidebar, GTK_INVALID_LIST_POSITION, {});
  check_sections(sidebar, {});

  sidebar.insert(section1, -1);
  check_items(sidebar, 0, {"1"});
  check_sections(sidebar, {"Section 1"});

  sidebar.insert(section2, -1);
  check_items(sidebar, 0, {"1", "2a", "2b"});
  check_sections(sidebar, {"Section 1", "Section 2"});

  sidebar.insert(section3, -1);
  check_items(sidebar, 0, {"1", "2a", "2b", "3a", "3b", "3c"});
  check_sections(sidebar, {"Section 1", "Section 2", "Section 3"});
}

static void test_adw_sidebar_menu_model() {
  Adw::Sidebar sidebar;

  Glib::RefPtr<Gio::MenuModel> model1 = Gio::Menu::create();
  Glib::RefPtr<Gio::MenuModel> model2 = Gio::Menu::create();

  notified = 0;
  sidebar.property_menu_model().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::RefPtr<Gio::MenuModel> model =
    sidebar.get_property<Glib::RefPtr<Gio::MenuModel>>("menu-model");
  g_assert_true(model == nullptr);
  g_assert_true(notified == 0);

  sidebar.set_menu_model(model1);
  g_assert_true(sidebar.get_menu_model() == model1);
  g_assert_true(notified == 1);

  sidebar.set_property<Glib::RefPtr<Gio::MenuModel>>("menu-model", model2);
  g_assert_true(sidebar.get_menu_model() == model2);
  g_assert_true(notified == 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Sidebar/mode", test_adw_sidebar_mode);
  g_test_add_func("/Adwaita/Sidebar/filter", test_adw_sidebar_filter);
  g_test_add_func("/Adwaita/Sidebar/placeholder", test_adw_sidebar_placeholder);
#if ADW_CHECK_VERSION(1, 10, 0)
  g_test_add_func("/Adwaita/Sidebar/prefix", test_adw_sidebar_prefix);
  g_test_add_func("/Adwaita/Sidebar/suffix", test_adw_sidebar_suffix);
#endif
  g_test_add_func("/Adwaita/Sidebar/add_remove", test_adw_sidebar_add_remove);
  g_test_add_func("/Adwaita/Sidebar/menu_model", test_adw_sidebar_menu_model);

  return g_test_run();
}
