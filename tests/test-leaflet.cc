/*
 * Copyright (C) 2020 Alexander Mikhaylenko <alexm@gnome.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

static void assert_page_position(Glib::RefPtr<Gtk::SelectionModel> pages, Gtk::Widget* widget,
                                 int position) {
  Glib::RefPtr<Gio::ListModel> list_model = std::dynamic_pointer_cast<Gio::ListModel>(pages);
  g_assert_true(list_model != nullptr);

  Glib::RefPtr<Adw::LeafletPage> page =
    std::dynamic_pointer_cast<Adw::LeafletPage>(list_model->get_object(position));
  g_assert_true(page != nullptr);

  g_assert_true(widget->gobj() == page->get_child()->gobj());
}

static void test_adw_leaflet_adjacent_child() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 3> children;

  for (int i = 0; i < 3; i++) {
    children[i] = Gtk::make_managed<Gtk::Button>();

    Glib::RefPtr<Adw::LeafletPage> page = leaflet.append(*children[i]);

    if (i == 1) {
      page->set_navigatable(false);
    }
  }

  leaflet.set_visible_child(*children[0]);

  Gtk::Widget* result = leaflet.get_adjacent_child(Adw::NavigationDirection::BACK);
  g_assert_null(result);

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::FORWARD);
  g_assert_true(result->gobj() == children[2]->gobj());

  leaflet.set_visible_child(*children[1]);

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::BACK);
  g_assert_true(result->gobj() == children[0]->gobj());

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::FORWARD);
  g_assert_true(result->gobj() == children[2]->gobj());

  leaflet.set_visible_child(*children[2]);

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::BACK);
  g_assert_true(result->gobj() == children[0]->gobj());

  result = leaflet.get_adjacent_child(Adw::NavigationDirection::FORWARD);
  g_assert_null(result);
}

static void test_adw_leaflet_navigate() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 3> children;

  bool result = leaflet.navigate(Adw::NavigationDirection::BACK);
  g_assert_false(result);

  result = leaflet.navigate(Adw::NavigationDirection::FORWARD);
  g_assert_false(result);

  for (int i = 0; i < 3; i++) {
    children[i] = Gtk::make_managed<Gtk::Label>("");

    Glib::RefPtr<Adw::LeafletPage> page = leaflet.append(*children[i]);

    if (i == 1) {
      page->set_navigatable(false);
    }
  }

  leaflet.set_visible_child(*children[0]);

  result = leaflet.navigate(Adw::NavigationDirection::BACK);
  g_assert_false(result);

  result = leaflet.navigate(Adw::NavigationDirection::FORWARD);
  g_assert_true(result);
  g_assert_true(leaflet.get_visible_child()->gobj() == children[2]->gobj());

  result = leaflet.navigate(Adw::NavigationDirection::FORWARD);
  g_assert_false(result);

  result = leaflet.navigate(Adw::NavigationDirection::BACK);
  g_assert_true(result);
  g_assert_true(leaflet.get_visible_child()->gobj() == children[0]->gobj());
}

static void test_adw_leaflet_prepend() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 2> labels;

  for (int i = 0; i < 2; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");
  }

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  leaflet.prepend(*labels[1]);
  assert_page_position(pages, labels[1], 0);

  leaflet.prepend(*labels[0]);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[1], 1);
}

static void test_adw_leaflet_insert_child_after() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 3> labels;

  for (int i = 0; i < 3; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");
  }

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  leaflet.append(*labels[2]);

  assert_page_position(pages, labels[2], 0);

  leaflet.insert_child_after(*labels[0], nullptr);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[2], 1);

  leaflet.insert_child_after(*labels[1], labels[0]);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[1], 1);
  assert_page_position(pages, labels[2], 2);
}

static void test_adw_leaflet_reorder_child_after() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 3> labels;

  for (int i = 0; i < 3; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");

    leaflet.append(*labels[i]);
  }

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[1], 1);
  assert_page_position(pages, labels[2], 2);

  leaflet.reorder_child_after(*labels[2], nullptr);
  assert_page_position(pages, labels[2], 0);
  assert_page_position(pages, labels[0], 1);
  assert_page_position(pages, labels[1], 2);

  leaflet.reorder_child_after(*labels[0], labels[1]);
  assert_page_position(pages, labels[2], 0);
  assert_page_position(pages, labels[1], 1);
  assert_page_position(pages, labels[0], 2);
}

// Regression test: adw_leaflet_get_pages() is (transfer full) in C and
// always returns a new reference, even when the AdwLeafletPages object
// already exists. Calling Leaflet::get_pages() repeatedly must keep
// returning the same C++ wrapper around the same GObject, without
// leaking the extra reference obtained on subsequent calls.
static void test_adw_leaflet_get_pages_returns_same_object() {
  Adw::Leaflet leaflet;
  Gtk::Widget* label = Gtk::make_managed<Gtk::Label>("");
  leaflet.append(*label);

  Glib::RefPtr<Gtk::SelectionModel> pages1 = leaflet.get_pages();
  Glib::RefPtr<Gtk::SelectionModel> pages2 = leaflet.get_pages();

  g_assert_true(pages1->gobj() == pages2->gobj());
  g_assert_true(pages1 == pages2);

  // A third call, after dropping one of the RefPtrs, should still resolve
  // to the same underlying object rather than constructing a new wrapper.
  pages1.reset();
  Glib::RefPtr<Gtk::SelectionModel> pages3 = leaflet.get_pages();
  g_assert_true(pages2->gobj() == pages3->gobj());
}

static void test_adw_leaflet_pages_n_items() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 3> labels;

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();
  Glib::RefPtr<Gio::ListModel> list_model = std::dynamic_pointer_cast<Gio::ListModel>(pages);
  g_assert_true(list_model != nullptr);

  g_assert_cmpuint(list_model->get_n_items(), ==, 0);

  for (int i = 0; i < 3; i++) {
    labels[i] = Gtk::make_managed<Gtk::Label>("");
    leaflet.append(*labels[i]);
  }

  g_assert_cmpuint(list_model->get_n_items(), ==, 3);

  leaflet.remove(*labels[1]);

  g_assert_cmpuint(list_model->get_n_items(), ==, 2);
  assert_page_position(pages, labels[0], 0);
  assert_page_position(pages, labels[2], 1);
}

static void test_adw_leaflet_pages_selection() {
  Adw::Leaflet leaflet;
  std::array<Gtk::Widget*, 3> children;

  for (int i = 0; i < 3; i++) {
    children[i] = Gtk::make_managed<Gtk::Button>();
    leaflet.append(*children[i]);
  }

  leaflet.set_visible_child(*children[0]);

  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();

  g_assert_true(pages->is_selected(0));
  g_assert_false(pages->is_selected(1));
  g_assert_false(pages->is_selected(2));

  // Selecting a page through the selection model should update the
  // leaflet's visible child, and vice versa (tested via is_selected above).
  pages->select_item(2, true);

  g_assert_true(leaflet.get_visible_child()->gobj() == children[2]->gobj());
  g_assert_false(pages->is_selected(0));
  g_assert_true(pages->is_selected(2));
}

static void test_adw_leaflet_pages_items_changed_signal() {
  Adw::Leaflet leaflet;
  Glib::RefPtr<Gtk::SelectionModel> pages = leaflet.get_pages();
  Glib::RefPtr<Gio::ListModel> list_model = std::dynamic_pointer_cast<Gio::ListModel>(pages);
  g_assert_true(list_model != nullptr);

  int signal_count = 0;
  guint last_position = 0, last_removed = 0, last_added = 0;

  list_model->signal_items_changed().connect([&](guint position, guint removed, guint added) {
    signal_count++;
    last_position = position;
    last_removed = removed;
    last_added = added;
  });

  Gtk::Widget* label = Gtk::make_managed<Gtk::Label>("");
  leaflet.append(*label);

  g_assert_cmpint(signal_count, ==, 1);
  g_assert_cmpuint(last_position, ==, 0);
  g_assert_cmpuint(last_removed, ==, 0);
  g_assert_cmpuint(last_added, ==, 1);

  leaflet.remove(*label);

  g_assert_cmpint(signal_count, ==, 2);
  g_assert_cmpuint(last_position, ==, 0);
  g_assert_cmpuint(last_removed, ==, 1);
  g_assert_cmpuint(last_added, ==, 0);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/Leaflet/adjacent_child", test_adw_leaflet_adjacent_child);
  g_test_add_func("/Adwaita/Leaflet/navigate", test_adw_leaflet_navigate);
  g_test_add_func("/Adwaita/Leaflet/prepend", test_adw_leaflet_prepend);
  g_test_add_func("/Adwaita/Leaflet/insert_child_after", test_adw_leaflet_insert_child_after);
  g_test_add_func("/Adwaita/Leaflet/reorder_child_after", test_adw_leaflet_reorder_child_after);
  g_test_add_func("/Adwaita/Leaflet/get_pages_returns_same_object",
                  test_adw_leaflet_get_pages_returns_same_object);
  g_test_add_func("/Adwaita/Leaflet/pages_n_items", test_adw_leaflet_pages_n_items);
  g_test_add_func("/Adwaita/Leaflet/pages_selection", test_adw_leaflet_pages_selection);
  g_test_add_func("/Adwaita/Leaflet/pages_items_changed_signal",
                  test_adw_leaflet_pages_items_changed_signal);

  return g_test_run();
}
