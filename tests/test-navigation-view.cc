/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init
#include <vector>

int notified;

static void notify_cb() { notified++; }

static void check_navigation_stack(Adw::NavigationView &view,
                                   const std::vector<Glib::ustring> &tags) {
  std::vector<Adw::NavigationPage *> stack = view.get_navigation_stack();

  g_assert_cmpint((int)stack.size(), ==, (int)tags.size());

  for (size_t i = 0; i < tags.size(); i++)
    g_assert_true(stack[i]->get_tag() == tags[i]);
}

static void test_adw_navigation_view_add_remove(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(),
                             "Page 2 again", "page-2");
  int local_notified = 0, notified_tag = 0, pushed = 0, popped = 0;

  view.signal_pushed().connect([&pushed]() { pushed++; });
  view.signal_popped().connect(
      [&popped](Adw::NavigationPage *) { popped++; });
  view.property_visible_page().signal_changed().connect(
      [&local_notified]() { local_notified++; });
  view.property_visible_page_tag().signal_changed().connect(
      [&notified_tag]() { notified_tag++; });

  g_assert_true(view.get_visible_page() == nullptr);
  g_assert_true(view.get_visible_page_tag() == "");
  check_navigation_stack(view, {});
  g_assert_true(pushed == 0);
  g_assert_true(popped == 0);
  g_assert_true(local_notified == 0);
  g_assert_true(notified_tag == 0);

  view.add(&page_1);
  view.add(&page_2);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*Duplicate page tag*");
  view.add(&page_3);
  g_test_assert_expected_messages();

  g_assert_true(view.get_visible_page() == &page_1);
  g_assert_true(view.get_visible_page_tag() == "page-1");
  check_navigation_stack(view, {"page-1"});
  g_assert_true(pushed == 1);
  g_assert_true(local_notified == 1);
  g_assert_true(notified_tag == 1);

  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(view.find_page("page-2") == &page_2);

  view.remove(&page_1);
  view.remove(&page_2);

  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(view.find_page("page-2") == nullptr);

  g_assert_true(view.get_visible_page() == &page_1);
  g_assert_true(view.get_visible_page_tag() == "page-1");
  check_navigation_stack(view, {"page-1"});

  g_assert_true(pushed == 1);
  g_assert_true(popped == 0);
  g_assert_true(local_notified == 1);
  g_assert_true(notified_tag == 1);
}

static void test_adw_navigation_view_push_pop(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(), "Page 3");
  Adw::NavigationPage page_4(Gtk::make_managed<Gtk::Button>(), "Page 4");
  Adw::NavigationPage page_5(Gtk::make_managed<Gtk::Button>(),
                             "Page 2 again", "page-2");
  int local_notified = 0, notified_tag = 0, pushed = 0, popped = 0;

  view.signal_pushed().connect([&pushed]() { pushed++; });
  view.signal_popped().connect(
      [&popped](Adw::NavigationPage *) { popped++; });
  view.property_visible_page().signal_changed().connect(
      [&local_notified]() { local_notified++; });
  view.property_visible_page_tag().signal_changed().connect(
      [&notified_tag]() { notified_tag++; });

  g_assert_true(pushed == 0);
  g_assert_true(popped == 0);
  g_assert_true(local_notified == 0);

  /* Will be autoremoved after pop */
  view.push(&page_1);
  g_assert_true(view.get_visible_page() == &page_1);
  g_assert_true(view.get_visible_page_tag() == "page-1");
  check_navigation_stack(view, {"page-1"});
  g_assert_true(pushed == 1);
  g_assert_true(local_notified == 1);
  g_assert_true(notified_tag == 1);

  /* Explicitly added page - will persist after pop */
  view.add(&page_2);
  g_assert_true(view.get_visible_page() == &page_1);
  check_navigation_stack(view, {"page-1"});
  g_assert_true(pushed == 1);
  g_assert_true(local_notified == 1);

  view.push(&page_2);
  g_assert_true(view.get_visible_page() == &page_2);
  g_assert_true(view.get_visible_page_tag() == "page-2");
  check_navigation_stack(view, {"page-1", "page-2"});
  g_assert_true(pushed == 2);
  g_assert_true(local_notified == 2);
  g_assert_true(notified_tag == 2);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already in navigation stack*");
  view.push(&page_2);
  g_test_assert_expected_messages();
  check_navigation_stack(view, {"page-1", "page-2"});
  g_assert_true(pushed == 2);

  view.add(&page_3);
  view.push(&page_3);
  g_assert_true(view.get_visible_page() == &page_3);
  g_assert_true(view.get_visible_page_tag() == "");
  check_navigation_stack(view, {"page-1", "page-2", ""});
  g_assert_true(pushed == 3);

  /* Removing while in navigation stack - no effect until it's popped */
  view.remove(&page_3);
  g_assert_true(view.get_visible_page() == &page_3);
  check_navigation_stack(view, {"page-1", "page-2", ""});
  g_assert_true(pushed == 3);

  view.push(&page_4);
  g_assert_true(view.get_visible_page() == &page_4);
  check_navigation_stack(view, {"page-1", "page-2", "", ""});
  g_assert_true(pushed == 4);
  g_assert_true(popped == 0);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*Duplicate page tag*");
  view.push(&page_5);
  g_test_assert_expected_messages();
  g_assert_true(view.get_visible_page() == &page_4);
  check_navigation_stack(view, {"page-1", "page-2", "", ""});
  g_assert_true(pushed == 4);
  g_assert_true(popped == 0);

  g_assert_true(view.pop());
  g_assert_true(view.get_visible_page() == &page_3);
  check_navigation_stack(view, {"page-1", "page-2", ""});
  g_assert_true(popped == 1);

  g_assert_true(view.pop());
  g_assert_true(view.get_visible_page() == &page_2);
  g_assert_true(view.get_visible_page_tag() == "page-2");
  check_navigation_stack(view, {"page-1", "page-2"});
  g_assert_true(view.find_page("page-3") == nullptr);
  g_assert_true(popped == 2);

  g_assert_true(view.pop());
  g_assert_true(view.get_visible_page() == &page_1);
  g_assert_true(view.get_visible_page_tag() == "page-1");
  check_navigation_stack(view, {"page-1"});
  g_assert_true(view.find_page("page-2") == &page_2);
  g_assert_true(popped == 3);

  /* Last page - not allowed to pop */
  g_assert_false(view.pop());
  g_assert_true(view.get_visible_page() == &page_1);
  check_navigation_stack(view, {"page-1"});
  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(popped == 3);

  g_assert_true(pushed == 4);
}

static void test_adw_navigation_view_push_pop_by_tag(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(), "Page 3",
                             "page-3");
  Adw::NavigationPage page_4(Gtk::make_managed<Gtk::Button>(), "Page 4",
                             "page-4");
  int pushed = 0, popped = 0;

  view.signal_pushed().connect([&pushed]() { pushed++; });
  view.signal_popped().connect(
      [&popped](Adw::NavigationPage *) { popped++; });

  view.add(&page_1);
  view.add(&page_2);
  view.add(&page_3);
  view.add(&page_4);

  check_navigation_stack(view, {"page-1"});
  g_assert_true(pushed == 1);
  g_assert_true(popped == 0);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, "*with the tag*");
  view.push_by_tag("page-0");
  g_test_assert_expected_messages();
  g_assert_true(pushed == 1);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*already in navigation stack*");
  view.push_by_tag("page-1");
  g_test_assert_expected_messages();
  g_assert_true(pushed == 1);

  view.push_by_tag("page-2");
  g_assert_true(view.get_visible_page() == &page_2);
  check_navigation_stack(view, {"page-1", "page-2"});
  g_assert_true(pushed == 2);

  view.push_by_tag("page-3");
  g_assert_true(view.get_visible_page() == &page_3);
  check_navigation_stack(view, {"page-1", "page-2", "page-3"});
  g_assert_true(pushed == 3);

  view.push_by_tag("page-4");
  g_assert_true(view.get_visible_page() == &page_4);
  check_navigation_stack(view, {"page-1", "page-2", "page-3", "page-4"});
  g_assert_true(pushed == 4);

  g_assert_true(popped == 0);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL, "*with the tag*");
  g_assert_false(view.pop_to_tag("page-5"));
  g_test_assert_expected_messages();

  g_assert_false(view.pop_to_tag("page-4"));
  g_assert_true(view.get_visible_page() == &page_4);
  check_navigation_stack(view, {"page-1", "page-2", "page-3", "page-4"});
  g_assert_true(popped == 0);

  g_assert_true(view.pop_to_tag("page-2"));
  g_assert_true(view.get_visible_page() == &page_2);
  check_navigation_stack(view, {"page-1", "page-2"});
  g_assert_true(popped == 2);

  g_assert_true(view.pop_to_tag("page-1"));
  g_assert_true(view.get_visible_page() == &page_1);
  check_navigation_stack(view, {"page-1"});
  g_assert_true(popped == 3);
}

static void test_adw_navigation_view_pop_to_page(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(), "Page 3",
                             "page-3");
  Adw::NavigationPage page_4(Gtk::make_managed<Gtk::Button>(), "Page 4",
                             "page-4");
  Adw::NavigationPage page_5(Gtk::make_managed<Gtk::Button>(), "Page 5",
                             "page-5");
  int popped = 0;

  view.signal_popped().connect(
      [&popped](Adw::NavigationPage *) { popped++; });

  view.add(&page_1);
  view.add(&page_3);
  view.push(&page_2);
  view.push(&page_3);
  view.push(&page_4);

  g_assert_true(view.get_visible_page() == &page_4);
  check_navigation_stack(view, {"page-1", "page-2", "page-3", "page-4"});

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*not in the navigation stack*");
  g_assert_false(view.pop_to_page(&page_5));
  g_test_assert_expected_messages();
  g_assert_true(popped == 0);

  g_assert_false(view.pop_to_page(&page_4));
  g_assert_true(popped == 0);

  g_assert_true(view.pop_to_page(&page_3));
  g_assert_true(view.get_visible_page() == &page_3);
  check_navigation_stack(view, {"page-1", "page-2", "page-3"});
  g_assert_true(popped == 1);
  g_assert_true(view.find_page("page-4") == nullptr);

  g_assert_true(view.pop_to_page(&page_1));
  g_assert_true(view.get_visible_page() == &page_1);
  check_navigation_stack(view, {"page-1"});
  g_assert_true(popped == 3);
  g_assert_true(view.find_page("page-2") == nullptr);
  g_assert_true(view.find_page("page-3") == &page_3);
}

static void test_adw_navigation_view_replace(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(), "Page 3",
                             "page-3");
  int pushed = 0, popped = 0, replaced = 0;

  view.signal_pushed().connect([&pushed]() { pushed++; });
  view.signal_popped().connect(
      [&popped](Adw::NavigationPage *) { popped++; });
  view.signal_replaced().connect([&replaced]() { replaced++; });

  check_navigation_stack(view, {});

  view.replace({});

  check_navigation_stack(view, {});
  g_assert_true(replaced == 1);

  view.replace({&page_1});

  check_navigation_stack(view, {"page-1"});
  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(view.find_page("page-2") == nullptr);
  g_assert_true(replaced == 2);

  view.replace({&page_2, &page_1});

  check_navigation_stack(view, {"page-2", "page-1"});
  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(view.find_page("page-2") == &page_2);
  g_assert_true(replaced == 3);

  view.replace({&page_1, &page_2});

  check_navigation_stack(view, {"page-1", "page-2"});
  g_assert_true(replaced == 4);

  view.add(&page_2);
  view.add(&page_3);

  view.replace_with_tags({"page-2", "page-3"});
  g_assert_true(view.find_page("page-1") == nullptr);
  g_assert_true(view.find_page("page-2") == &page_2);
  g_assert_true(view.find_page("page-3") == &page_3);
  g_assert_true(replaced == 5);

  view.remove(&page_3);

  view.replace_with_tags({});
  g_assert_true(view.find_page("page-2") == &page_2);
  g_assert_true(view.find_page("page-3") == nullptr);
  g_assert_true(replaced == 6);

  g_assert_true(pushed == 0);
  g_assert_true(popped == 0);
}

static void test_adw_navigation_view_previous_page(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(), "Page 3",
                             "page-3");
  Adw::NavigationPage page_4(Gtk::make_managed<Gtk::Button>(), "Page 4",
                             "page-4");

  view.add(&page_1);
  view.add(&page_2);
  view.add(&page_4);

  view.push(&page_2);
  view.push(&page_3);

  g_assert_true(view.get_previous_page(&page_1) == nullptr);
  g_assert_true(view.get_previous_page(&page_2) == &page_1);
  g_assert_true(view.get_previous_page(&page_3) == &page_2);
  g_assert_true(view.get_previous_page(&page_4) == nullptr);
}

static void test_adw_navigation_view_find_page(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page_1(Gtk::make_managed<Gtk::Button>(), "Page 1",
                             "page-1");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Page 2",
                             "page-2");
  g_assert_true(view.find_page("page-1") == nullptr);
  g_assert_true(view.find_page("page-2") == nullptr);

  view.add(&page_1);
  view.add(&page_2);
  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(view.find_page("page-2") == &page_2);

  g_test_expect_message(ADW_LOG_DOMAIN, G_LOG_LEVEL_CRITICAL,
                        "*Duplicate page tag*");
  page_1.set_tag("page-2");
  g_test_assert_expected_messages();
  g_assert_true(view.find_page("page-1") == &page_1);
  g_assert_true(view.find_page("page-2") == &page_2);

  page_1.set_tag("page-3");
  g_assert_true(view.find_page("page-1") == nullptr);
  g_assert_true(view.find_page("page-2") == &page_2);
  g_assert_true(view.find_page("page-3") == &page_1);

  view.replace({});
  view.remove(&page_1);
  view.remove(&page_2);
  g_assert_true(view.find_page("page-1") == nullptr);
  g_assert_true(view.find_page("page-2") == nullptr);
  g_assert_true(view.find_page("page-3") == nullptr);
}

static void test_adw_navigation_view_animate_transitions(void) {
  Adw::NavigationView view;

  notified = 0;
  view.property_animate_transitions().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(view.get_animate_transitions());

  view.set_animate_transitions(true);
  g_assert_true(notified == 0);

  view.set_animate_transitions(false);
  g_assert_false(view.get_animate_transitions());
  g_assert_true(notified == 1);

  view.set_property<bool>("animate-transitions", true);
  g_assert_true(view.get_animate_transitions());
  g_assert_true(notified == 2);
}

static void test_adw_navigation_view_pop_on_escape(void) {
  Adw::NavigationView view;

  notified = 0;
  view.property_pop_on_escape().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  g_assert_true(view.get_pop_on_escape());

  view.set_pop_on_escape(true);
  g_assert_true(notified == 0);

  view.set_pop_on_escape(false);
  g_assert_false(view.get_pop_on_escape());
  g_assert_true(notified == 1);

  view.set_property<bool>("pop-on-escape", true);
  g_assert_true(view.get_pop_on_escape());
  g_assert_true(notified == 2);
}

static void test_adw_navigation_page_child(void) {
  Gtk::Widget *button = Gtk::make_managed<Gtk::Button>();
  Adw::NavigationPage page(button, "Title");
  Gtk::Widget *widget;

  notified = 0;
  page.property_child().signal_changed().connect(sigc::ptr_fun(notify_cb));

  widget = page.get_property<Gtk::Widget *>("child");
  g_assert_true(widget == button);
  g_assert_true(notified == 0);

  page.set_child(nullptr);
  g_assert_null(page.get_child());
  g_assert_true(notified == 1);

  Gtk::Widget *button2 = Gtk::make_managed<Gtk::Button>();
  page.set_property<Gtk::Widget *>("child", button2);
  g_assert_true(page.get_child() == button2);
  g_assert_true(notified == 2);
}

static void test_adw_navigation_page_title(void) {
  Adw::NavigationPage page(Gtk::make_managed<Gtk::Button>(), "Title");

  notified = 0;
  page.property_title().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring title = page.get_property<Glib::ustring>("title");
  g_assert_true(title == "Title");
  g_assert_true(notified == 0);

  page.set_title("Some title");
  g_assert_true(page.get_title() == "Some title");
  g_assert_true(notified == 1);

  page.set_property<Glib::ustring>("title", "Some other title");
  g_assert_true(page.get_title() == "Some other title");
  g_assert_true(notified == 2);
}

static void test_adw_navigation_page_tag(void) {
  Adw::NavigationPage page(Gtk::make_managed<Gtk::Button>(), "Title");

  notified = 0;
  page.property_tag().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring tag = page.get_property<Glib::ustring>("tag");
  g_assert_true(tag == "");
  g_assert_true(notified == 0);

  page.set_tag("tag");
  g_assert_true(page.get_tag() == "tag");
  g_assert_true(notified == 1);

  page.set_property<Glib::ustring>("tag", "other-tag");
  g_assert_true(page.get_tag() == "other-tag");
  g_assert_true(notified == 2);
}

static void test_adw_navigation_page_can_pop(void) {
  Adw::NavigationPage page(Gtk::make_managed<Gtk::Button>(), "Title");

  notified = 0;
  page.property_can_pop().signal_changed().connect(sigc::ptr_fun(notify_cb));

  g_assert_true(page.get_can_pop());

  page.set_can_pop(true);
  g_assert_true(notified == 0);

  page.set_can_pop(false);
  g_assert_false(page.get_can_pop());
  g_assert_true(notified == 1);

  page.set_property<bool>("can-pop", true);
  g_assert_true(page.get_can_pop());
  g_assert_true(notified == 2);
}

static void test_adw_navigation_page_signals(void) {
  Adw::NavigationView view;
  Adw::NavigationPage page(Gtk::make_managed<Gtk::Button>(), "Title");
  Adw::NavigationPage page_2(Gtk::make_managed<Gtk::Button>(), "Title");
  Adw::NavigationPage page_3(Gtk::make_managed<Gtk::Button>(), "Title");
  int showing = 0, shown = 0, hiding = 0, hidden = 0;

  page.signal_showing().connect([&showing]() { showing++; });
  page.signal_shown().connect([&shown]() { shown++; });
  page.signal_hiding().connect([&hiding]() { hiding++; });
  page.signal_hidden().connect([&hidden]() { hidden++; });

  g_assert_true(showing == 0);
  g_assert_true(shown == 0);
  g_assert_true(hiding == 0);
  g_assert_true(hidden == 0);

  view.add(&page);
  g_assert_true(showing == 1);
  g_assert_true(shown == 1);
  g_assert_true(hiding == 0);
  g_assert_true(hidden == 0);

  view.push(&page_2);
  g_assert_true(showing == 1);
  g_assert_true(shown == 1);
  g_assert_true(hiding == 1);
  g_assert_true(hidden == 1);

  view.pop();
  g_assert_true(showing == 2);
  g_assert_true(shown == 2);
  g_assert_true(hiding == 1);
  g_assert_true(hidden == 1);

  view.replace({&page_2});
  g_assert_true(showing == 2);
  g_assert_true(shown == 2);
  g_assert_true(hiding == 2);
  g_assert_true(hidden == 2);

  view.push(&page);
  g_assert_true(showing == 3);
  g_assert_true(shown == 3);
  g_assert_true(hiding == 2);
  g_assert_true(hidden == 2);

  view.pop();
  g_assert_true(showing == 3);
  g_assert_true(shown == 3);
  g_assert_true(hiding == 3);
  g_assert_true(hidden == 3);

  view.replace({&page_2, &page, &page_3});
  g_assert_true(showing == 3);
  g_assert_true(shown == 3);
  g_assert_true(hiding == 3);
  g_assert_true(hidden == 3);

  view.pop_to_page(&page_2);
  g_assert_true(showing == 3);
  g_assert_true(shown == 3);
  g_assert_true(hiding == 3);
  g_assert_true(hidden == 3);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/NavigationView/add_remove",
                  test_adw_navigation_view_add_remove);
  g_test_add_func("/Adwaita/NavigationView/push_pop",
                  test_adw_navigation_view_push_pop);
  g_test_add_func("/Adwaita/NavigationView/push_pop_by_tag",
                  test_adw_navigation_view_push_pop_by_tag);
  g_test_add_func("/Adwaita/NavigationView/pop_to_page",
                  test_adw_navigation_view_pop_to_page);
  g_test_add_func("/Adwaita/NavigationView/replace",
                  test_adw_navigation_view_replace);
  g_test_add_func("/Adwaita/NavigationView/previous_page",
                  test_adw_navigation_view_previous_page);
  g_test_add_func("/Adwaita/NavigationView/find_page",
                  test_adw_navigation_view_find_page);
  g_test_add_func("/Adwaita/NavigationView/animate_transitions",
                  test_adw_navigation_view_animate_transitions);
  g_test_add_func("/Adwaita/NavigationView/pop_on_escape",
                  test_adw_navigation_view_pop_on_escape);
  g_test_add_func("/Adwaita/NavigationPage/child",
                  test_adw_navigation_page_child);
  g_test_add_func("/Adwaita/NavigationPage/title",
                  test_adw_navigation_page_title);
  g_test_add_func("/Adwaita/NavigationPage/tag", test_adw_navigation_page_tag);
  g_test_add_func("/Adwaita/NavigationPage/can_pop",
                  test_adw_navigation_page_can_pop);
  g_test_add_func("/Adwaita/NavigationPage/signals",
                  test_adw_navigation_page_signals);

  return g_test_run();
}
