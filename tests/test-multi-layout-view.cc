/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;

static void notify_cb() { notified++; }

static Glib::RefPtr<Adw::Layout> make_layout(Gtk::Widget* content) {
  return Glib::make_refptr_for_instance<Adw::Layout>(new Adw::Layout(content));
}

// AdwMultiLayoutView::add_layout() takes ownership of the layout it is
// given, so callers that want to keep their own reference (as these tests
// do, to keep comparing against it) need to give it an extra one first.
static void add_layout(Adw::MultiLayoutView& view,
                       const Glib::RefPtr<Adw::Layout>& layout) {
  layout->reference();
  view.add_layout(layout.get());
}

static void test_adw_multi_layout_view_add_remove(void) {
  Adw::MultiLayoutView view;
  Gtk::Widget* content1 = Gtk::make_managed<Adw::Bin>();
  Gtk::Widget* content2 = Gtk::make_managed<Adw::Bin>();
  Glib::RefPtr<Adw::Layout> layout1 = make_layout(content1);
  Glib::RefPtr<Adw::Layout> layout2 = make_layout(content2);

  g_assert_true(view.get_layout() == nullptr);

  add_layout(view, layout1);
  g_assert_true(view.get_layout() == layout1);

  add_layout(view, layout2);
  g_assert_true(view.get_layout() == layout1);

  view.remove_layout(layout1.get());
  g_assert_true(view.get_layout() == layout2);
}

static void test_adw_multi_layout_view_layout(void) {
  Adw::MultiLayoutView view;
  Gtk::Widget* content1 = Gtk::make_managed<Adw::Bin>();
  Gtk::Widget* content2 = Gtk::make_managed<Adw::Bin>();
  Glib::RefPtr<Adw::Layout> layout1 = make_layout(content1);
  Glib::RefPtr<Adw::Layout> layout2 = make_layout(content2);

  notified = 0;
  view.property_layout().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(view.get_layout() == nullptr);
  g_assert_true(notified == 0);

  add_layout(view, layout1);
  g_assert_true(view.get_layout() == layout1);
  g_assert_true(notified == 1);

  add_layout(view, layout2);
  g_assert_true(notified == 1);

  view.set_layout(layout2.get());
  g_assert_true(view.get_layout() == layout2);
  g_assert_true(notified == 2);

  view.set_property<Adw::Layout*>("layout", layout1.get());
  g_assert_true(view.get_layout() == layout1);
  g_assert_true(notified == 3);
}

static void test_adw_multi_layout_view_layout_name(void) {
  Adw::MultiLayoutView view;
  Gtk::Widget* content1 = Gtk::make_managed<Adw::Bin>();
  Gtk::Widget* content2 = Gtk::make_managed<Adw::Bin>();
  Glib::RefPtr<Adw::Layout> layout1 = make_layout(content1);
  Glib::RefPtr<Adw::Layout> layout2 = make_layout(content2);

  layout1->set_name("layout1");
  layout2->set_name("layout2");

  notified = 0;
  view.property_layout_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(view.get_layout_name() == "");
  g_assert_true(notified == 0);

  add_layout(view, layout1);
  g_assert_true(view.get_layout() == layout1);
  g_assert_true(view.get_layout_name() == "layout1");
  g_assert_true(notified == 1);

  add_layout(view, layout2);
  g_assert_true(notified == 1);

  view.set_layout_name("layout2");
  g_assert_true(view.get_layout() == layout2);
  g_assert_true(view.get_layout_name() == "layout2");
  g_assert_true(notified == 2);

  view.set_property<Glib::ustring>("layout-name", "layout1");
  g_assert_true(view.get_layout() == layout1);
  g_assert_true(view.get_layout_name() == "layout1");
  g_assert_true(notified == 3);

  layout1->set_name("layout11");
  g_assert_true(view.get_layout_name() == "layout11");
  g_assert_true(notified == 4);

  layout2->set_name("layout22");
  g_assert_true(notified == 4);

  g_assert_true(view.get_layout_by_name("layout11") == layout1);
  g_assert_true(view.get_layout_by_name("layout22") == layout2);
  g_assert_true(view.get_layout_by_name("layout1") == nullptr);
  g_assert_true(view.get_layout_by_name("layout2") == nullptr);
  g_assert_true(view.get_layout_by_name("layout3") == nullptr);
}

static void test_adw_multi_layout_view_children(void) {
  Adw::Window window;
  Adw::MultiLayoutView* view = Gtk::make_managed<Adw::MultiLayoutView>();
  Gtk::Widget* slot11 = Gtk::make_managed<Adw::LayoutSlot>("slot1");
  Gtk::Widget* slot12 = Gtk::make_managed<Adw::LayoutSlot>("slot1");
  Gtk::Widget* slot21 = Gtk::make_managed<Adw::LayoutSlot>("slot2");
  Gtk::Widget* slot22 = Gtk::make_managed<Adw::LayoutSlot>("slot2");
  Gtk::Box* content1 =
      Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
  Gtk::Box* content2 = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  Gtk::Widget* child1 = Gtk::make_managed<Adw::Bin>();
  Gtk::Widget* child2 = Gtk::make_managed<Adw::Bin>();

  content1->append(*slot11);
  content1->append(*slot21);
  content2->append(*slot12);
  content2->append(*slot22);

  Glib::RefPtr<Adw::Layout> layout1 = make_layout(content1);
  Glib::RefPtr<Adw::Layout> layout2 = make_layout(content2);

  window.set_content(*view);
  view->set_child("slot1", child1);

  g_assert_true(view->get_child("slot1") == child1);
  g_assert_null(child1->get_parent());
  g_assert_null(view->get_child("slot2"));

  add_layout(*view, layout1);
  add_layout(*view, layout2);
  g_assert_true(child1->get_parent() == slot11);

  view->set_child("slot2", child2);
  g_assert_true(child2->get_parent() == slot21);

  view->set_layout(layout2.get());
  g_assert_true(child1->get_parent() == slot12);
  g_assert_true(child2->get_parent() == slot22);

  g_assert_true(view->get_child("slot1") == child1);
  g_assert_true(view->get_child("slot2") == child2);
}

int main(int argc, char* argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/MultiLayoutView/add_remove",
                  test_adw_multi_layout_view_add_remove);
  g_test_add_func("/Adwaita/MultiLayoutView/layout",
                  test_adw_multi_layout_view_layout);
  g_test_add_func("/Adwaita/MultiLayoutView/layout_name",
                  test_adw_multi_layout_view_layout_name);
  g_test_add_func("/Adwaita/MultiLayoutView/children",
                  test_adw_multi_layout_view_children);

  return g_test_run();
}
