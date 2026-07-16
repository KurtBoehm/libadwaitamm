/*
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
int responses;
int responses_cancel;
int responses_save;

static void notify_cb() { notified++; }

static void response_cb(const Glib::ustring &response) {
  responses++;
  if (response == "cancel") {
    responses_cancel++;
  } else if (response == "save") {
    responses_save++;
  } else {
    g_assert_not_reached();
  }
}

static void test_adw_alert_dialog_heading(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_heading().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring heading = dialog.get_property<Glib::ustring>("heading");
  g_assert_true(heading == "");

  dialog.set_heading("Heading");
  g_assert_true(dialog.get_heading() == "Heading");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("heading", "Heading 2");
  g_assert_true(dialog.get_heading() == "Heading 2");
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_heading_use_markup(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_heading_use_markup().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool use_markup = dialog.get_property<bool>("heading-use-markup");
  g_assert_false(use_markup);

  dialog.set_heading_use_markup(true);
  g_assert_true(dialog.get_heading_use_markup());
  g_assert_true(notified == 1);

  dialog.set_property<bool>("heading-use-markup", false);
  g_assert_false(dialog.get_heading_use_markup());
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_body(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_body().signal_changed().connect(sigc::ptr_fun(notify_cb));

  Glib::ustring body = dialog.get_property<Glib::ustring>("body");
  g_assert_true(body == "");

  dialog.set_body("Body");
  g_assert_true(dialog.get_body() == "Body");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("body", "Body 2");
  g_assert_true(dialog.get_body() == "Body 2");
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_body_use_markup(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_body_use_markup().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool use_markup = dialog.get_property<bool>("body-use-markup");
  g_assert_false(use_markup);

  dialog.set_body_use_markup(true);
  g_assert_true(dialog.get_body_use_markup());
  g_assert_true(notified == 1);

  dialog.set_property<bool>("body-use-markup", false);
  g_assert_false(dialog.get_body_use_markup());
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_format(void) {
  Adw::AlertDialog dialog("", "");

  // FIXME: We are not currently wrapping the format_* conveniences of
  // Adw.AlertDialog. Emulate here.
  // --
  dialog.set_heading_use_markup(true);
  dialog.set_heading(Glib::ustring::sprintf("Heading <b>%d</b>", 42));
  // --
  g_assert_true(dialog.get_heading() == "Heading <b>42</b>");
  g_assert_true(dialog.get_heading_use_markup());

  // --
  dialog.set_heading_use_markup(false);
  dialog.set_heading(Glib::ustring::sprintf("Heading %d", 42));
  // --
  g_assert_true(dialog.get_heading() == "Heading 42");
  g_assert_false(dialog.get_heading_use_markup());

  // --
  dialog.set_body_use_markup(true);
  dialog.set_body(Glib::ustring::sprintf("Body <b>%d</b>", 42));
  // --
  g_assert_true(dialog.get_body() == "Body <b>42</b>");
  g_assert_true(dialog.get_body_use_markup());

  // --
  dialog.set_body_use_markup(false);
  dialog.set_body(Glib::ustring::sprintf("Body %d", 42));
  // --
  g_assert_true(dialog.get_body() == "Body 42");
  g_assert_false(dialog.get_body_use_markup());
}

static void test_adw_alert_dialog_extra_child(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_extra_child().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::Widget *widget = dialog.get_property<Gtk::Widget *>("extra-child");
  g_assert_true(widget == nullptr);

  dialog.set_extra_child(nullptr);
  g_assert_true(notified == 0);

  widget = Gtk::make_managed<Gtk::Button>();
  dialog.set_extra_child(widget);
  g_assert_true(dialog.get_extra_child() == widget);
  g_assert_true(notified == 1);

  dialog.set_property<Gtk::Widget *>("extra-child", nullptr);
  g_assert_true(dialog.get_extra_child() == nullptr);
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_prefer_wide_layout(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_prefer_wide_layout().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  bool prefer_wide_layout = dialog.get_property<bool>("prefer-wide-layout");
  g_assert_false(prefer_wide_layout);

  dialog.set_prefer_wide_layout(true);
  g_assert_true(dialog.get_prefer_wide_layout());
  g_assert_true(notified == 1);

  dialog.set_property<bool>("prefer-wide-layout", false);
  g_assert_false(dialog.get_prefer_wide_layout());
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_add_response(void) {
  Adw::AlertDialog dialog("", "");

  dialog.add_response("response1", "Response 1");
  dialog.add_response("response2", "Response 2");

  g_assert_true(dialog.get_response_label("response1") == "Response 1");
  g_assert_true(dialog.get_response_enabled("response1"));
  g_assert_true(dialog.get_response_appearance("response1") ==
                Adw::ResponseAppearance::DEFAULT);

  g_assert_true(dialog.get_response_label("response2") == "Response 2");
  g_assert_true(dialog.get_response_enabled("response2"));
  g_assert_true(dialog.get_response_appearance("response2") ==
                Adw::ResponseAppearance::DEFAULT);
}

static void test_adw_alert_dialog_add_responses(void) {
  Adw::AlertDialog dialog("", "");

  // FIXME: The convenience to add many responses at once has not been
  // wrapped. Emulate.
  // --
  dialog.add_response("response1", "Response 1");
  dialog.add_response("response2", "Response 2");
  // --

  g_assert_true(dialog.get_response_label("response1") == "Response 1");
  g_assert_true(dialog.get_response_enabled("response1"));
  g_assert_true(dialog.get_response_appearance("response1") ==
                Adw::ResponseAppearance::DEFAULT);

  g_assert_true(dialog.get_response_label("response2") == "Response 2");
  g_assert_true(dialog.get_response_enabled("response2"));
  g_assert_true(dialog.get_response_appearance("response2") ==
                Adw::ResponseAppearance::DEFAULT);
}

static void test_adw_alert_dialog_response_label(void) {
  Adw::AlertDialog dialog("", "");

  dialog.add_response("response", "Response");
  g_assert_true(dialog.get_response_label("response") == "Response");

  dialog.set_response_label("response", "Label");
  g_assert_true(dialog.get_response_label("response") == "Label");
}

static void test_adw_alert_dialog_response_enabled(void) {
  Adw::AlertDialog dialog("", "");

  dialog.add_response("response", "Response");
  g_assert_true(dialog.get_response_enabled("response"));

  dialog.set_response_enabled("response", false);
  g_assert_false(dialog.get_response_enabled("response"));

  dialog.set_response_enabled("response", true);
  g_assert_true(dialog.get_response_enabled("response"));
}

static void test_adw_alert_dialog_response_appearance(void) {
  Adw::AlertDialog dialog("", "");

  dialog.add_response("response", "Response");
  g_assert_true(dialog.get_response_appearance("response") ==
                Adw::ResponseAppearance::DEFAULT);

  dialog.set_response_appearance("response",
                                 Adw::ResponseAppearance::DESTRUCTIVE);
  g_assert_true(dialog.get_response_appearance("response") ==
                Adw::ResponseAppearance::DESTRUCTIVE);
}

static void test_adw_alert_dialog_response_signal(void) {
  Adw::AlertDialog dialog("", "");

  responses = responses_cancel = responses_save = 0;
  dialog.signal_response().connect(sigc::ptr_fun(response_cb));

  dialog.add_response("cancel", "Cancel");
  dialog.add_response("save", "Save");

  g_signal_emit_by_name(dialog.gobj(), "response", "cancel");
  g_assert_true(responses == 1);
  g_assert_true(responses_cancel == 1);
  g_assert_true(responses_save == 0);

  g_signal_emit_by_name(dialog.gobj(), "response", "save");
  g_assert_true(responses == 2);
  g_assert_true(responses_cancel == 1);
  g_assert_true(responses_save == 1);
}

static void test_adw_alert_dialog_default_response(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_default_response().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring response =
      dialog.get_property<Glib::ustring>("default-response");
  g_assert_true(response == "");

  dialog.set_default_response("save");
  g_assert_true(dialog.get_default_response() == "save");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("default-response", "load");
  g_assert_true(dialog.get_default_response() == "load");
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_close_response(void) {
  Adw::AlertDialog dialog("", "");

  notified = 0;
  dialog.property_close_response().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring response = dialog.get_property<Glib::ustring>("close-response");
  g_assert_true(response == "close");

  dialog.set_close_response("save");
  g_assert_true(dialog.get_close_response() == "save");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("close-response", "cancel");
  g_assert_true(dialog.get_close_response() == "cancel");
  g_assert_true(notified == 2);
}

static void test_adw_alert_dialog_choose(void) {
  Adw::AlertDialog dialog("Heading", "Body");

  dialog.add_response("cancel", "Cancel");
  dialog.add_response("save", "Save");
  dialog.set_close_response("cancel");

  Glib::ustring chosen_response;
  bool got_result = false;

  auto slot = [&](const Glib::RefPtr<Gio::AsyncResult> &result) {
    chosen_response = dialog.choose_finish(result);
    got_result = true;
  };

  dialog.choose(nullptr, Glib::RefPtr<Gio::Cancellable>(), slot);
  g_signal_emit_by_name(dialog.gobj(), "response", "save");

  while (!got_result)
    g_main_context_iteration(nullptr, TRUE);

  g_assert_true(chosen_response == "save");
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/AlertDialog/heading",
                  test_adw_alert_dialog_heading);
  g_test_add_func("/Adwaita/AlertDialog/heading_use_markup",
                  test_adw_alert_dialog_heading_use_markup);
  g_test_add_func("/Adwaita/AlertDialog/body", test_adw_alert_dialog_body);
  g_test_add_func("/Adwaita/AlertDialog/body_use_markup",
                  test_adw_alert_dialog_body_use_markup);
  g_test_add_func("/Adwaita/AlertDialog/format", test_adw_alert_dialog_format);
  g_test_add_func("/Adwaita/AlertDialog/extra_child",
                  test_adw_alert_dialog_extra_child);
  g_test_add_func("/Adwaita/AlertDialog/prefer_wide_layout",
                  test_adw_alert_dialog_prefer_wide_layout);
  g_test_add_func("/Adwaita/AlertDialog/add_response",
                  test_adw_alert_dialog_add_response);
  g_test_add_func("/Adwaita/AlertDialog/add_responses",
                  test_adw_alert_dialog_add_responses);
  g_test_add_func("/Adwaita/AlertDialog/response_label",
                  test_adw_alert_dialog_response_label);
  g_test_add_func("/Adwaita/AlertDialog/response_enabled",
                  test_adw_alert_dialog_response_enabled);
  g_test_add_func("/Adwaita/AlertDialog/response_appearance",
                  test_adw_alert_dialog_response_appearance);
  g_test_add_func("/Adwaita/AlertDialog/response_signal",
                  test_adw_alert_dialog_response_signal);
  g_test_add_func("/Adwaita/AlertDialog/default_response",
                  test_adw_alert_dialog_default_response);
  g_test_add_func("/Adwaita/AlertDialog/close_response",
                  test_adw_alert_dialog_close_response);
  g_test_add_func("/Adwaita/AlertDialog/choose", test_adw_alert_dialog_choose);

  return g_test_run();
}
