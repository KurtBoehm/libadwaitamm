/*
 * Copyright (C) 2023 Maximiliano Sandoval <msandova@gnome.org>
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() {
  ++notified;
}

static void check_toggles(const Adw::ToggleGroup& group, guint active,
                          std::initializer_list<Glib::ustring> names) {
  Glib::RefPtr<Gtk::SelectionModel> toggles = group.get_toggles();
  Glib::RefPtr<Gio::ListModel> toggles_model = std::dynamic_pointer_cast<Gio::ListModel>(toggles);

  g_assert_true(group.get_n_toggles() == names.size());
  g_assert_true(toggles_model->get_n_items() == names.size());

  guint i = 0;
  for (const Glib::ustring& name : names) {
    Glib::RefPtr<Adw::Toggle> toggle =
      std::dynamic_pointer_cast<Adw::Toggle>(toggles_model->get_object(i));

    g_assert_true(toggle->get_name() == name);

    if (i == active) {
      g_assert_true(toggles->is_selected(i));
    } else {
      g_assert_false(toggles->is_selected(i));
    }

    i++;
  }
}

static void test_adw_toggle_group_add() {
  Adw::ToggleGroup group;
  int index_notified = 0;
  int name_notified = 0;
  int n_toggles_notified = 0;

  group.property_active().signal_changed().connect([&index_notified]() { index_notified++; });
  group.property_active_name().signal_changed().connect([&name_notified]() { name_notified++; });
  group.property_n_toggles().signal_changed().connect(
    [&n_toggles_notified]() { n_toggles_notified++; });

  check_toggles(group, GTK_INVALID_LIST_POSITION, {});

  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 0);
  g_assert_true(name_notified == 0);
  g_assert_true(n_toggles_notified == 0);

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->reference();
  group.add(toggle1.get());

  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 0);
  g_assert_true(n_toggles_notified == 1);

  check_toggles(group, 0, {""});

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->set_name("toggle2");

  // Active doesn't change again.
  toggle2->reference();
  group.add(toggle2.get());
  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 0);
  g_assert_true(n_toggles_notified == 2);

  check_toggles(group, 0, {"", "toggle2"});

  group.remove_all();
  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 0);
  g_assert_true(n_toggles_notified == 3);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {});

  // Toggle is disabled, so will not be selected.
  Glib::RefPtr<Adw::Toggle> toggle3 = Adw::Toggle::create();
  toggle3->set_name("toggle3");
  toggle3->set_enabled(false);
  toggle3->reference();
  group.add(toggle3.get());

  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 0);
  g_assert_true(n_toggles_notified == 4);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {"toggle3"});

  // This one is enabled, so we jump to it.
  Glib::RefPtr<Adw::Toggle> toggle4 = Adw::Toggle::create();
  toggle4->set_name("toggle4");
  toggle4->reference();
  group.add(toggle4.get());

  g_assert_true(group.get_active() == 1);
  g_assert_true(group.get_active_name() == "toggle4");
  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 1);
  g_assert_true(n_toggles_notified == 5);

  check_toggles(group, 1, {"toggle3", "toggle4"});
}

static void test_adw_toggle_group_remove() {
  Adw::ToggleGroup group;
  int index_notified = 0;
  int name_notified = 0;
  int n_toggles_notified = 0;

  group.property_active().signal_changed().connect([&index_notified]() { index_notified++; });
  group.property_active_name().signal_changed().connect([&name_notified]() { name_notified++; });
  group.property_n_toggles().signal_changed().connect(
    [&n_toggles_notified]() { n_toggles_notified++; });

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->reference();
  group.add(toggle1.get());
  g_assert_true(n_toggles_notified == 1);

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->reference();
  group.add(toggle2.get());
  g_assert_true(n_toggles_notified == 2);

  Glib::RefPtr<Adw::Toggle> toggle3 = Adw::Toggle::create();
  toggle3->set_name("toggle3");
  toggle3->reference();
  group.add(toggle3.get());
  g_assert_true(n_toggles_notified == 3);

  Glib::RefPtr<Adw::Toggle> toggle4 = Adw::Toggle::create();
  toggle4->reference();
  group.add(toggle4.get());
  g_assert_true(n_toggles_notified == 4);

  check_toggles(group, 0, {"", "", "toggle3", ""});

  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 0);

  group.set_active(2);

  g_assert_true(group.get_active() == 2);
  g_assert_true(group.get_active_name() == "toggle3");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 1);

  check_toggles(group, 2, {"", "", "toggle3", ""});

  group.remove(group.get_toggle(1).get());

  g_assert_true(group.get_active() == 1);
  g_assert_true(group.get_active_name() == "toggle3");
  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 1);
  g_assert_true(n_toggles_notified == 5);

  check_toggles(group, 1, {"", "toggle3", ""});

  group.remove(group.get_toggle(2).get());

  g_assert_true(group.get_active() == 1);
  g_assert_true(group.get_active_name() == "toggle3");
  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 1);
  g_assert_true(n_toggles_notified == 6);

  check_toggles(group, 1, {"", "toggle3"});

  group.remove(group.get_toggle(1).get());

  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 4);
  g_assert_true(name_notified == 2);
  g_assert_true(n_toggles_notified == 7);

  check_toggles(group, 1, {""});

  group.remove(group.get_toggle(0).get());

  g_assert_true(index_notified == 4);
  g_assert_true(name_notified == 2);
  g_assert_true(n_toggles_notified == 8);

  check_toggles(group, 1, {});
}

static void test_adw_toggle_group_remove_all() {
  Adw::ToggleGroup group;
  int index_notified = 0;
  int name_notified = 0;
  int n_toggles_notified = 0;

  group.property_active().signal_changed().connect([&index_notified]() { index_notified++; });
  group.property_active_name().signal_changed().connect([&name_notified]() { name_notified++; });
  group.property_n_toggles().signal_changed().connect(
    [&n_toggles_notified]() { n_toggles_notified++; });

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->reference();
  group.add(toggle1.get());
  g_assert_true(n_toggles_notified == 1);

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->set_name("toggle2");
  toggle2->reference();
  group.add(toggle2.get());
  g_assert_true(n_toggles_notified == 2);

  check_toggles(group, 0, {"", "toggle2"});

  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 0);

  group.remove_all();

  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 0);
  g_assert_true(n_toggles_notified == 3);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {});

  Glib::RefPtr<Adw::Toggle> toggle3 = Adw::Toggle::create();
  toggle3->set_name("toggle3");
  toggle3->reference();
  group.add(toggle3.get());
  g_assert_true(n_toggles_notified == 4);

  Glib::RefPtr<Adw::Toggle> toggle4 = Adw::Toggle::create();
  toggle4->reference();
  group.add(toggle4.get());

  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 1);
  g_assert_true(n_toggles_notified == 5);

  check_toggles(group, 0, {"toggle3", ""});

  group.remove_all();

  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 4);
  g_assert_true(name_notified == 2);
  g_assert_true(n_toggles_notified == 6);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {});
}

static void test_adw_toggle_group_get_toggle() {
  Adw::ToggleGroup group;

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->reference();
  group.add(toggle1.get());

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->reference();
  group.add(toggle2.get());

  g_assert_true(group.get_toggle(0) == toggle1);
  g_assert_true(group.get_toggle(1) == toggle2);
  g_assert_true(group.get_toggle(2) == nullptr);
  g_assert_true(group.get_toggle(GTK_INVALID_LIST_POSITION) == nullptr);
}

static void test_adw_toggle_group_get_toggle_by_name() {
  Adw::ToggleGroup group;

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->set_name("toggle1");
  toggle1->reference();
  group.add(toggle1.get());

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->set_name("toggle2");
  toggle2->reference();
  group.add(toggle2.get());

  g_assert_true(group.get_toggle_by_name("toggle1") == toggle1);
  g_assert_true(group.get_toggle_by_name("toggle2") == toggle2);
  g_assert_true(group.get_toggle_by_name("toggle3") == nullptr);
}

static void test_adw_toggle_group_active() {
  Adw::ToggleGroup group;
  int index_notified = 0;
  int name_notified = 0;

  group.property_active().signal_changed().connect([&index_notified]() { index_notified++; });
  group.property_active_name().signal_changed().connect([&name_notified]() { name_notified++; });

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->set_name("toggle1");
  toggle1->reference();
  group.add(toggle1.get());

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->set_name("toggle2");
  toggle2->reference();
  group.add(toggle2.get());

  Glib::RefPtr<Adw::Toggle> toggle3 = Adw::Toggle::create();
  toggle3->reference();
  group.add(toggle3.get());

  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 1);

  check_toggles(group, 0, {"toggle1", "toggle2", ""});

  group.set_active(0);
  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 1);

  check_toggles(group, 0, {"toggle1", "toggle2", ""});

  group.set_active(1);
  g_assert_true(group.get_active() == 1);
  g_assert_true(group.get_active_name() == "toggle2");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 2);

  check_toggles(group, 1, {"toggle1", "toggle2", ""});

  group.set_active(3);
  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 3);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {"toggle1", "toggle2", ""});

  group.set_active(GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 3);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {"toggle1", "toggle2", ""});

  group.set_active(2);
  g_assert_true(group.get_active() == 2);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 4);
  g_assert_true(name_notified == 3);

  check_toggles(group, 2, {"toggle1", "toggle2", ""});
}

static void test_adw_toggle_group_active_name() {
  Adw::ToggleGroup group;
  int index_notified = 0;
  int name_notified = 0;

  group.property_active().signal_changed().connect([&index_notified]() { index_notified++; });
  group.property_active_name().signal_changed().connect([&name_notified]() { name_notified++; });

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->set_name("toggle1");
  toggle1->reference();
  group.add(toggle1.get());

  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 1);

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->set_name("toggle2");
  toggle2->reference();
  group.add(toggle2.get());

  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 1);

  Glib::RefPtr<Adw::Toggle> toggle3 = Adw::Toggle::create();
  toggle3->reference();
  group.add(toggle3.get());
  group.set_active(2);
  g_assert_true(group.get_active() == 2);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 2);

  check_toggles(group, 2, {"toggle1", "toggle2", ""});

  group.set_active_name("toggle1");
  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 3);
  g_assert_true(name_notified == 3);

  check_toggles(group, 0, {"toggle1", "toggle2", ""});

  group.set_active_name("toggle2");
  g_assert_true(group.get_active() == 1);
  g_assert_true(group.get_active_name() == "toggle2");
  g_assert_true(index_notified == 4);
  g_assert_true(name_notified == 4);

  check_toggles(group, 1, {"toggle1", "toggle2", ""});

  // set_active_name() can’t express AdwToggleGroup:active-name’s nullable “unset” semantics through
  // Glib::ustring, so clear via set_active() instead, which clears active-name too.
  group.set_active(GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 5);
  g_assert_true(name_notified == 5);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {"toggle1", "toggle2", ""});

  group.set_active(2);
  g_assert_true(group.get_active() == 2);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 6);
  g_assert_true(name_notified == 5);

  check_toggles(group, 2, {"toggle1", "toggle2", ""});
}

static void test_adw_toggle_group_active_enabled() {
  Adw::ToggleGroup group;
  int index_notified = 0;
  int name_notified = 0;

  group.property_active().signal_changed().connect([&index_notified]() { index_notified++; });
  group.property_active_name().signal_changed().connect([&name_notified]() { name_notified++; });

  Glib::RefPtr<Adw::Toggle> toggle1 = Adw::Toggle::create();
  toggle1->set_name("toggle1");
  toggle1->reference();
  group.add(toggle1.get());

  Glib::RefPtr<Adw::Toggle> toggle2 = Adw::Toggle::create();
  toggle2->set_name("toggle2");
  toggle2->reference();
  group.add(toggle2.get());

  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 1);

  check_toggles(group, 0, {"toggle1", "toggle2"});

  // Nothing happens when disabling inactive toggles.
  toggle2->set_enabled(false);
  g_assert_true(group.get_active() == 0);
  g_assert_true(group.get_active_name() == "toggle1");
  g_assert_true(index_notified == 1);
  g_assert_true(name_notified == 1);

  check_toggles(group, 0, {"toggle1", "toggle2"});

  // Selection clears when disabling the active one.
  toggle1->set_enabled(false);
  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 2);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {"toggle1", "toggle2"});

  // Enabling it back does nothing.
  toggle1->set_enabled(true);
  g_assert_true(group.get_active() == GTK_INVALID_LIST_POSITION);
  g_assert_true(group.get_active_name() == "");
  g_assert_true(index_notified == 2);
  g_assert_true(name_notified == 2);

  check_toggles(group, GTK_INVALID_LIST_POSITION, {"toggle1", "toggle2"});
}

static void test_adw_toggle_group_homogeneous() {
  Adw::ToggleGroup group;

  notified = 0;
  group.property_homogeneous().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool homogeneous = group.get_property<bool>("homogeneous");
  g_assert_false(homogeneous);

  group.set_homogeneous(false);
  g_assert_false(group.get_homogeneous());
  g_assert_true(notified == 0);

  group.set_property<bool>("homogeneous", true);
  g_assert_true(group.get_homogeneous());
  g_assert_true(notified == 1);

  group.set_homogeneous(false);
  g_assert_false(group.get_property<bool>("homogeneous"));
  g_assert_true(notified == 2);
}

static void test_adw_toggle_group_can_shrink() {
  Adw::ToggleGroup group;

  notified = 0;
  group.property_can_shrink().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool can_shrink = group.get_property<bool>("can-shrink");
  g_assert_true(can_shrink);

  group.set_can_shrink(true);
  g_assert_true(group.get_can_shrink());
  g_assert_true(notified == 0);

  group.set_property<bool>("can-shrink", false);
  g_assert_false(group.get_can_shrink());
  g_assert_true(notified == 1);

  group.set_can_shrink(true);
  g_assert_true(group.get_property<bool>("can-shrink"));
  g_assert_true(notified == 2);
}

static void test_adw_toggle_name() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_name().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring name = toggle->get_property<Glib::ustring>("name");
  g_assert_true(name == "");

  toggle->set_name("toggle");
  g_assert_true(toggle->get_name() == "toggle");
  g_assert_true(notified == 1);

  toggle->set_property<Glib::ustring>("name", "toggle2");
  g_assert_true(toggle->get_name() == "toggle2");
  g_assert_true(notified == 2);
}

static void test_adw_toggle_label() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_label().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring label = toggle->get_property<Glib::ustring>("label");
  g_assert_true(label == "");

  toggle->set_label("Toggle");
  g_assert_true(toggle->get_label() == "Toggle");
  g_assert_true(notified == 1);

  toggle->set_property<Glib::ustring>("label", "Toggle 2");
  g_assert_true(toggle->get_label() == "Toggle 2");
  g_assert_true(notified == 2);
}

static void test_adw_toggle_use_underline() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_use_underline().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool use_underline = toggle->get_property<bool>("use-underline");
  g_assert_false(use_underline);

  toggle->set_use_underline(false);
  g_assert_false(toggle->get_use_underline());
  g_assert_true(notified == 0);

  toggle->set_property<bool>("use-underline", true);
  g_assert_true(toggle->get_use_underline());
  g_assert_true(notified == 1);

  toggle->set_use_underline(false);
  g_assert_false(toggle->get_property<bool>("use-underline"));
  g_assert_true(notified == 2);
}

static void test_adw_toggle_icon_name() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_icon_name().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring icon_name = toggle->get_property<Glib::ustring>("icon-name");
  g_assert_true(icon_name == "");

  toggle->set_icon_name("go-previous-symbolic");
  g_assert_true(toggle->get_icon_name() == "go-previous-symbolic");
  g_assert_true(notified == 1);

  toggle->set_property<Glib::ustring>("icon-name", "go-next-symbolic");
  g_assert_true(toggle->get_icon_name() == "go-next-symbolic");
  g_assert_true(notified == 2);
}

static void test_adw_toggle_tooltip() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_tooltip().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring tooltip = toggle->get_property<Glib::ustring>("tooltip");
  g_assert_true(tooltip == "");

  toggle->set_tooltip("Tooltip");
  g_assert_true(toggle->get_tooltip() == "Tooltip");
  g_assert_true(notified == 1);

  toggle->set_property<Glib::ustring>("tooltip", "");
  g_assert_true(toggle->get_tooltip() == "");
  g_assert_true(notified == 2);
}

static void test_adw_toggle_description() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_description().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring description = toggle->get_property<Glib::ustring>("description");
  g_assert_true(description == "");

  toggle->set_description("Description");
  g_assert_true(toggle->get_description() == "Description");
  g_assert_true(notified == 1);

  toggle->set_property<Glib::ustring>("description", "");
  g_assert_true(toggle->get_description() == "");
  g_assert_true(notified == 2);
}

static void test_adw_toggle_child() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_child().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Gtk::Widget* widget = toggle->get_property<Gtk::Widget*>("child");
  g_assert_true(widget == nullptr);

  toggle->set_child(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  toggle->set_child(widget);
  g_assert_true(toggle->get_child() == widget);
  g_assert_true(notified == 1);

  toggle->set_property<Gtk::Widget*>("child", nullptr);
  g_assert_true(toggle->get_child() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_toggle_enabled() {
  Glib::RefPtr<Adw::Toggle> toggle = Adw::Toggle::create();

  notified = 0;
  toggle->property_enabled().signal_changed().connect(sigc::ptr_fun(notify_cb));

  bool enabled = toggle->get_property<bool>("enabled");
  g_assert_true(enabled);

  toggle->set_enabled(true);
  g_assert_true(toggle->get_enabled());
  g_assert_true(notified == 0);

  toggle->set_property<bool>("enabled", false);
  g_assert_false(toggle->get_enabled());
  g_assert_true(notified == 1);

  toggle->set_enabled(true);
  g_assert_true(toggle->get_property<bool>("enabled"));
  g_assert_true(notified == 2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/ToggleGroup/add", test_adw_toggle_group_add);
  g_test_add_func("/Adwaita/ToggleGroup/remove", test_adw_toggle_group_remove);
  g_test_add_func("/Adwaita/ToggleGroup/remove_all", test_adw_toggle_group_remove_all);
  g_test_add_func("/Adwaita/ToggleGroup/get_toggle", test_adw_toggle_group_get_toggle);
  g_test_add_func("/Adwaita/ToggleGroup/get_toggle_by_name",
                  test_adw_toggle_group_get_toggle_by_name);
  g_test_add_func("/Adwaita/ToggleGroup/active", test_adw_toggle_group_active);
  g_test_add_func("/Adwaita/ToggleGroup/active_name", test_adw_toggle_group_active_name);
  g_test_add_func("/Adwaita/ToggleGroup/active_enabled", test_adw_toggle_group_active_enabled);
  g_test_add_func("/Adwaita/ToggleGroup/homogeneous", test_adw_toggle_group_homogeneous);
  g_test_add_func("/Adwaita/ToggleGroup/can_shrink", test_adw_toggle_group_can_shrink);
  g_test_add_func("/Adwaita/Toggle/name", test_adw_toggle_name);
  g_test_add_func("/Adwaita/Toggle/label", test_adw_toggle_label);
  g_test_add_func("/Adwaita/Toggle/use_underline", test_adw_toggle_use_underline);
  g_test_add_func("/Adwaita/Toggle/icon_name", test_adw_toggle_icon_name);
  g_test_add_func("/Adwaita/Toggle/tooltip", test_adw_toggle_tooltip);
  g_test_add_func("/Adwaita/Toggle/description", test_adw_toggle_description);
  g_test_add_func("/Adwaita/Toggle/child", test_adw_toggle_child);
  g_test_add_func("/Adwaita/Toggle/enabled", test_adw_toggle_enabled);

  return g_test_run();
}
