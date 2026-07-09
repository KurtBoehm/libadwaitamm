/*
 * Copyright (C) 2026 Kurt Böhm
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <libadwaitamm.h>
#include <libadwaitamm/init.h> // Adw::init

int notified;
int activated_links;
Glib::ustring last_activated_uri;

static void notify_cb() { notified++; }

static bool activate_link_cb(const Glib::ustring &uri) {
  activated_links++;
  last_activated_uri = uri;
  return true;
}

static void test_adw_about_dialog_application_name(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_application_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring name = dialog.get_property<Glib::ustring>("application-name");
  g_assert_true(name == "");

  dialog.set_application_name("Test App");
  g_assert_true(dialog.get_application_name() == "Test App");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("application-name", "Test App 2");
  g_assert_true(dialog.get_application_name() == "Test App 2");
  g_assert_true(notified == 2);
}

static void test_adw_about_dialog_application_icon(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_application_icon().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_application_icon("test-icon-symbolic");
  g_assert_true(dialog.get_application_icon() == "test-icon-symbolic");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("application-icon", "other-icon-symbolic");
  g_assert_true(dialog.get_application_icon() == "other-icon-symbolic");
  g_assert_true(notified == 2);
}

static void test_adw_about_dialog_developer_name(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_developer_name().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_developer_name("A Developer");
  g_assert_true(dialog.get_developer_name() == "A Developer");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("developer-name", "Another Developer");
  g_assert_true(dialog.get_developer_name() == "Another Developer");
  g_assert_true(notified == 2);
}

static void test_adw_about_dialog_version(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_version().signal_changed().connect(sigc::ptr_fun(notify_cb));

  dialog.set_version("1.0.0");
  g_assert_true(dialog.get_version() == "1.0.0");
  g_assert_true(notified == 1);

  dialog.set_property<Glib::ustring>("version", "2.0.0");
  g_assert_true(dialog.get_version() == "2.0.0");
  g_assert_true(notified == 2);
}

static void test_adw_about_dialog_release_notes(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_release_notes().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Glib::ustring notes = dialog.get_property<Glib::ustring>("release-notes");
  g_assert_true(notes == "");

  dialog.set_release_notes("<p>Initial release</p>");
  g_assert_true(dialog.get_release_notes() == "<p>Initial release</p>");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_release_notes_version(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_release_notes_version().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_release_notes_version("1.5.0");
  g_assert_true(dialog.get_release_notes_version() == "1.5.0");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_comments(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_comments().signal_changed().connect(sigc::ptr_fun(notify_cb));

  dialog.set_comments("Some comments");
  g_assert_true(dialog.get_comments() == "Some comments");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_website(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_website().signal_changed().connect(sigc::ptr_fun(notify_cb));

  dialog.set_website("https://example.com");
  g_assert_true(dialog.get_website() == "https://example.com");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_support_url(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_support_url().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_support_url("https://example.com/support");
  g_assert_true(dialog.get_support_url() == "https://example.com/support");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_issue_url(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_issue_url().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_issue_url("https://example.com/issues");
  g_assert_true(dialog.get_issue_url() == "https://example.com/issues");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_debug_info(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_debug_info().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_debug_info("debug: ok");
  g_assert_true(dialog.get_debug_info() == "debug: ok");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_debug_info_filename(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_debug_info_filename().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_debug_info_filename("debug-info.txt");
  g_assert_true(dialog.get_debug_info_filename() == "debug-info.txt");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_copyright(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_copyright().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_copyright("© 2026 Someone");
  g_assert_true(dialog.get_copyright() == "© 2026 Someone");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_license(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_license().signal_changed().connect(sigc::ptr_fun(notify_cb));

  dialog.set_license("Custom license text");
  g_assert_true(dialog.get_license() == "Custom license text");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_license_type(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_license_type().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  Gtk::License license_type = dialog.get_property<Gtk::License>("license-type");
  g_assert_true(license_type == Gtk::License::UNKNOWN);

  dialog.set_license_type(Gtk::License::GPL_3_0);
  g_assert_true(dialog.get_license_type() == Gtk::License::GPL_3_0);
  g_assert_true(notified == 1);

  dialog.set_property<Gtk::License>("license-type", Gtk::License::MIT_X11);
  g_assert_true(dialog.get_license_type() == Gtk::License::MIT_X11);
  g_assert_true(notified == 2);
}

static void test_adw_about_dialog_translator_credits(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_translator_credits().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_translator_credits("Translator One");
  g_assert_true(dialog.get_translator_credits() == "Translator One");
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_developers(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_developers().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  std::vector<Glib::ustring> devs = dialog.get_developers();
  g_assert_true(devs.empty());

  std::vector<Glib::ustring> new_devs = {"Dev One", "Dev Two"};
  dialog.set_developers(new_devs);
  g_assert_true(dialog.get_developers() == new_devs);
  g_assert_true(notified == 1);
}

static void test_adw_about_dialog_designers(void) {
  Adw::AboutDialog dialog;

  std::vector<Glib::ustring> designers = {"Designer One"};
  dialog.set_designers(designers);
  g_assert_true(dialog.get_designers() == designers);
}

static void test_adw_about_dialog_artists(void) {
  Adw::AboutDialog dialog;

  std::vector<Glib::ustring> artists = {"Artist One"};
  dialog.set_artists(artists);
  g_assert_true(dialog.get_artists() == artists);
}

static void test_adw_about_dialog_documenters(void) {
  Adw::AboutDialog dialog;

  std::vector<Glib::ustring> documenters = {"Documenter One"};
  dialog.set_documenters(documenters);
  g_assert_true(dialog.get_documenters() == documenters);
}

#if ADW_CHECK_VERSION(1, 10, 0)
static void test_adw_about_dialog_other_apps_title(void) {
  Adw::AboutDialog dialog;

  notified = 0;
  dialog.property_other_apps_title().signal_changed().connect(
      sigc::ptr_fun(notify_cb));

  dialog.set_other_apps_title("Other Apps");
  g_assert_true(dialog.get_other_apps_title() == "Other Apps");
  g_assert_true(notified == 1);
}
#endif

static void test_adw_about_dialog_add_link(void) {
  Adw::AboutDialog dialog;

  // No getter is exposed for links; just ensure this doesn't crash.
  dialog.add_link("Donate", "https://example.com/donate");
}

static void test_adw_about_dialog_add_credit_section(void) {
  Adw::AboutDialog dialog;

  std::vector<Glib::ustring> people = {"Contributor One"};
  dialog.add_credit_section("Contributors", people);
}

static void test_adw_about_dialog_add_acknowledgement_section(void) {
  Adw::AboutDialog dialog;

  std::vector<Glib::ustring> people = {"Sponsor One"};
  dialog.add_acknowledgement_section("Sponsors", people);
}

static void test_adw_about_dialog_add_legal_section(void) {
  Adw::AboutDialog dialog;

  dialog.add_legal_section("Third-Party Library", "© 2026 Library Author",
                           Gtk::License::MIT_X11, "");
}

static void test_adw_about_dialog_add_other_app(void) {
  Adw::AboutDialog dialog;

  dialog.add_other_app("org.example.OtherApp", "Other App",
                       "A companion application");
}

static void test_adw_about_dialog_activate_link_signal(void) {
  Adw::AboutDialog dialog;

  activated_links = 0;
  last_activated_uri = "";
  dialog.signal_activate_link().connect(sigc::ptr_fun(activate_link_cb), false);

  bool handled = false;
  g_signal_emit_by_name(dialog.gobj(), "activate-link", "https://example.com",
                        &handled);

  g_assert_true(activated_links == 1);
  g_assert_true(last_activated_uri == "https://example.com");
  g_assert_true(handled);
}

int main(int argc, char *argv[]) {
  gtk_test_init(&argc, &argv, NULL);
  Adw::init();

  g_test_add_func("/Adwaita/AboutDialog/application_name",
                  test_adw_about_dialog_application_name);
  g_test_add_func("/Adwaita/AboutDialog/application_icon",
                  test_adw_about_dialog_application_icon);
  g_test_add_func("/Adwaita/AboutDialog/developer_name",
                  test_adw_about_dialog_developer_name);
  g_test_add_func("/Adwaita/AboutDialog/version",
                  test_adw_about_dialog_version);
  g_test_add_func("/Adwaita/AboutDialog/release_notes",
                  test_adw_about_dialog_release_notes);
  g_test_add_func("/Adwaita/AboutDialog/release_notes_version",
                  test_adw_about_dialog_release_notes_version);
  g_test_add_func("/Adwaita/AboutDialog/comments",
                  test_adw_about_dialog_comments);
  g_test_add_func("/Adwaita/AboutDialog/website",
                  test_adw_about_dialog_website);
  g_test_add_func("/Adwaita/AboutDialog/support_url",
                  test_adw_about_dialog_support_url);
  g_test_add_func("/Adwaita/AboutDialog/issue_url",
                  test_adw_about_dialog_issue_url);
  g_test_add_func("/Adwaita/AboutDialog/debug_info",
                  test_adw_about_dialog_debug_info);
  g_test_add_func("/Adwaita/AboutDialog/debug_info_filename",
                  test_adw_about_dialog_debug_info_filename);
  g_test_add_func("/Adwaita/AboutDialog/copyright",
                  test_adw_about_dialog_copyright);
  g_test_add_func("/Adwaita/AboutDialog/license",
                  test_adw_about_dialog_license);
  g_test_add_func("/Adwaita/AboutDialog/license_type",
                  test_adw_about_dialog_license_type);
  g_test_add_func("/Adwaita/AboutDialog/translator_credits",
                  test_adw_about_dialog_translator_credits);
  g_test_add_func("/Adwaita/AboutDialog/developers",
                  test_adw_about_dialog_developers);
  g_test_add_func("/Adwaita/AboutDialog/designers",
                  test_adw_about_dialog_designers);
  g_test_add_func("/Adwaita/AboutDialog/artists",
                  test_adw_about_dialog_artists);
  g_test_add_func("/Adwaita/AboutDialog/documenters",
                  test_adw_about_dialog_documenters);
#if ADW_CHECK_VERSION(1, 10, 0)
  g_test_add_func("/Adwaita/AboutDialog/other_apps_title",
                  test_adw_about_dialog_other_apps_title);
#endif
  g_test_add_func("/Adwaita/AboutDialog/add_link",
                  test_adw_about_dialog_add_link);
  g_test_add_func("/Adwaita/AboutDialog/add_credit_section",
                  test_adw_about_dialog_add_credit_section);
  g_test_add_func("/Adwaita/AboutDialog/add_acknowledgement_section",
                  test_adw_about_dialog_add_acknowledgement_section);
  g_test_add_func("/Adwaita/AboutDialog/add_legal_section",
                  test_adw_about_dialog_add_legal_section);
  g_test_add_func("/Adwaita/AboutDialog/add_other_app",
                  test_adw_about_dialog_add_other_app);
  g_test_add_func("/Adwaita/AboutDialog/activate_link_signal",
                  test_adw_about_dialog_activate_link_signal);

  return g_test_run();
}