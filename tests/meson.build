test_env = [
  'G_TEST_SRCDIR=@0@'.format(meson.current_source_dir()),
  'G_TEST_BUILDDIR=@0@'.format(meson.current_build_dir()),
  'G_DEBUG=gc-friendly,fatal-warnings',
  'GSETTINGS_BACKEND=memory',
  'PYTHONDONTWRITEBYTECODE=yes',
  'MALLOC_CHECK_=2',
  'GTK_A11Y=none',
]

test_cppflags = [
  '-DADW_LOG_DOMAIN="Adwaita"',
  '-DTEST_DATA_DIR="@0@/data"'.format(meson.current_source_dir()),
]

test_link_args = []
use_pie = false

cpp = meson.get_compiler('cpp')
if cpp.get_argument_syntax() != 'msvc'
  test_link_args += '-fPIC'
  use_pie = true
endif

# Same GResource bundle the C test suite uses, so tests that construct
# widgets from AppStream metadata (e.g. AboutDialog/AboutWindow) can find it.
test_resources = gnome.compile_resources(
  'adwaita-test-resources',
  'tests.gresources.xml',
  source_dir: join_paths(meson.current_source_dir()),
  c_name: 'test',
)

# Tests that need the compiled-in AppStream metadata resource.
tests_needing_resources = [
  'test-about-dialog',
  'test-about-window',
]

test_names = [
  'test-about-dialog',
  'test-about-window',
  'test-accent-color',
  'test-action-row',
  'test-alert-dialog',
  'test-animation',
  'test-animation-target',
  'test-application-window',
  'test-avatar',
  'test-banner',
  'test-bin',
  'test-bottom-sheet',
  'test-button-content',
  'test-button-row',
  'test-breakpoint',
  'test-breakpoint-bin',
  'test-carousel',
  'test-carousel-indicator-dots',
  'test-carousel-indicator-lines',
  'test-clamp',
  'test-clamp-layout',
  'test-clamp-scrollable',
  'test-dialog',
  'test-combo-row',
  'test-easing',
  'test-entry-row',
  'test-enum-list-model',
  'test-expander-row',
  'test-flap',
  'test-header-bar',
  'test-inline-view-switcher',
  'test-leaflet',
  'test-message-dialog',
  'test-multi-layout-view',
  'test-navigation-split-view',
  'test-navigation-view',
  'test-overlay-split-view',
  'test-password-entry-row',
  'test-preferences-group',
  'test-preferences-page',
  'test-preferences-row',
  'test-preferences-window',
  'test-split-button',
  'test-squeezer',
  'test-status-page',
  'test-style-manager',
  'test-tab-bar',
  'test-tab-view',
  'test-timed-animation',
  'test-toast',
  'test-toast-overlay',
  'test-view-switcher',
  'test-view-switcher-bar',
  'test-window',
  'test-window-title',
]

if libadwaita_has_1_10
  test_names += 'test-css-class-binding'
endif

foreach test_name : test_names
  test_sources = [test_name + '.cc']
  if tests_needing_resources.contains(test_name)
    test_sources += test_resources
  endif

  t = executable(test_name, test_sources,
                     cpp_args: test_cppflags,
                    link_args: test_link_args,
                 dependencies: libadwaitamm_own_dep,
                          pie: use_pie)
  test(test_name, t, env: test_env)
endforeach
