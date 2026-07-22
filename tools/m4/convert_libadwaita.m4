dnl Copyright (c) 2019 Roger Ferrer <rofirrim@gmail.com>,
dnl               2026 Kurt Böhm  <kurbo96@gmail.com>
dnl This file is part of libadwaitamm.


dnl ============================================================================
dnl Generic base conversions
dnl ============================================================================

_CONVERSION(`Glib::Object&', `GObject*', `($3).gobj()')

_CONVERSION(`gpointer', `Glib::RefPtr<Glib::ObjectBase>', `Glib::make_refptr_for_instance<Glib::ObjectBase>(Glib::wrap_auto(G_OBJECT($3)))')

_CONVERSION(`const Glib::RefPtr<Gtk::Application>&', `GtkApplication*', __CONVERT_REFPTR_TO_P($3))


dnl ============================================================================
dnl Gtk expression/factory/model/widgets
dnl ============================================================================

_CONVERSION(`const Glib::RefPtr<Gtk::Expression<Glib::ustring>>&', `GtkExpression*', `(($3) ? ($3)->gobj() : nullptr)')
_CONVERSION(`GtkExpression*', `Glib::RefPtr<Gtk::Expression<Glib::ustring>>', `Glib::wrap<Glib::ustring>($3)')

_CONVERSION(`const Glib::RefPtr<Gtk::ListItemFactory>&', `GtkListItemFactory*', __CONVERT_REFPTR_TO_P)
_CONVERSION(`GtkListItemFactory*', `Glib::RefPtr<Gtk::ListItemFactory>', `Glib::wrap($3)')

_CONVERSION(`GtkPopover*', `Gtk::Popover*', __RP2P)
_CONVERSION(`Gtk::Popover*', `GtkPopover*', __FP2P)

_CONVERSION(`GtkSelectionModel*', `Glib::RefPtr<Gtk::SelectionModel>', __RP2P)
_CONVERSION(`const Glib::RefPtr<Gtk::SelectionModel>&', `GtkSelectionModel*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`GtkWindow*', `Gtk::Window*', __RP2P)
_CONVERSION(`Gtk::Window*', `GtkWindow*', __FP2P)


dnl ============================================================================
dnl Adw core objects & basic wrappers
dnl ============================================================================

_CONVERSION(`AdwAnimationTarget*', `Glib::RefPtr<AnimationTarget>', __RP2P)
_CONVERSION(`Glib::RefPtr<AnimationTarget>', `AdwAnimationTarget*', __FP2P)
_CONVERSION(`const Glib::RefPtr<AnimationTarget>&', `AdwAnimationTarget*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwBreakpointCondition*', `BreakpointCondition*', __RP2P)
_CONVERSION(`BreakpointCondition*', `AdwBreakpointCondition*', __FP2P)
_CONVERSION(`BreakpointCondition', `AdwBreakpointCondition*', `($3).steal()')
_CONVERSION(`AdwBreakpointCondition*', `BreakpointCondition', `Glib::wrap($3, true)')

_CONVERSION(`AdwBreakpoint*', `Glib::RefPtr<Adw::Breakpoint>', __RP2P)
_CONVERSION(`const Glib::RefPtr<Adw::Breakpoint>&', `AdwBreakpoint*', __CONVERT_REFPTR_TO_P($3))
_CONVERSION(`Adw::Breakpoint*', `AdwBreakpoint*', __FP2P)
_CONVERSION(`Adw::Breakpoint&', `AdwBreakpoint*', `($3).gobj()')


dnl ============================================================================
dnl Adw widgets and pages
dnl ============================================================================

_CONVERSION(`AdwCarousel*', `Adw::Carousel*', __RP2P)
_CONVERSION(`Adw::Carousel*', `AdwCarousel*', __FP2P)

_CONVERSION(`AdwDialog*', `Adw::Dialog*', __RP2P)
_CONVERSION(`Adw::Dialog*', `AdwDialog*', __FP2P)

_CONVERSION(`AdwLayout*', `Layout*', __RP2P)
_CONVERSION(`Layout*', `AdwLayout*', __FP2P)
_CONVERSION(`AdwLayout*', `Glib::RefPtr<Layout>', __RP2P)
_CONVERSION(`const Glib::RefPtr<Layout>&', `AdwLayout*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwLeafletPage*', `Glib::RefPtr<LeafletPage>', __RP2P)
_CONVERSION(`const Glib::RefPtr<LeafletPage>&', `AdwLeafletPage*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwNavigationPage*', `NavigationPage*', __RP2P)
_CONVERSION(`NavigationPage*', `AdwNavigationPage*', __FP2P)

_CONVERSION(`AdwPreferencesGroup*', `PreferencesGroup*', __RP2P)
_CONVERSION(`PreferencesGroup*', `AdwPreferencesGroup*', __FP2P)

_CONVERSION(`AdwPreferencesPage*', `PreferencesPage*', __RP2P)
_CONVERSION(`PreferencesPage*', `AdwPreferencesPage*', __FP2P)

_CONVERSION(`AdwSpringParams*', `Glib::RefPtr<SpringParams>', __RP2P)
_CONVERSION(`const Glib::RefPtr<SpringParams>&', `AdwSpringParams*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwSqueezerPage*', `Glib::RefPtr<SqueezerPage>', __RP2P)
_CONVERSION(`const Glib::RefPtr<SqueezerPage>&', `AdwSqueezerPage*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwStyleManager*', `Glib::RefPtr<StyleManager>', __RP2P)

_CONVERSION(`AdwSwipeable*', `Glib::RefPtr<Swipeable>', __RP2P)
_CONVERSION(`const Glib::RefPtr<Swipeable>&', `AdwSwipeable*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwTabPage*', `Glib::RefPtr<TabPage>', __RP2P)
_CONVERSION(`AdwTabPage*', `const Glib::RefPtr<TabPage>&', __RP2P)
_CONVERSION(`const Glib::RefPtr<TabPage>&', `AdwTabPage*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwTabView*', `TabView*', __RP2P)
_CONVERSION(`TabView*', `AdwTabView*', __FP2P)
_CONVERSION(`const TabView*', `AdwTabView*', __FP2P)

_CONVERSION(`AdwToast*', `Toast*', __RP2P)
_CONVERSION(`Toast*', `AdwToast*', __FP2P)

_CONVERSION(`AdwViewStack*', `ViewStack*', __RP2P)
_CONVERSION(`ViewStack*', `AdwViewStack*', __FP2P)
_CONVERSION(`AdwViewStack*', `Glib::RefPtr<ViewStack>', __RP2P)
_CONVERSION(`const Glib::RefPtr<ViewStack>&', `AdwViewStack*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwViewStackPage*', `Glib::RefPtr<ViewStackPage>', __RP2P)
_CONVERSION(`const Glib::RefPtr<ViewStackPage>&', `AdwViewStackPage*', __CONVERT_REFPTR_TO_P($3))

_CONVERSION(`AdwWindow*', `Glib::RefPtr<Window>', __RP2P)


dnl ============================================================================
dnl Enum conversions
dnl ============================================================================

_CONV_ENUM(Adw, ColorScheme)
_CONV_ENUM(Adw, CenteringPolicy)
_CONV_ENUM(Adw, Easing)
_CONV_ENUM(Adw, FoldThresholdPolicy)
_CONV_ENUM(Adw, LengthUnit)
_CONV_ENUM(Adw, NavigationDirection)
_CONV_ENUM(Adw, ResponseAppearance)

_CONV_INCLASS_ENUM(Adw, Animation, State)
_CONV_INCLASS_ENUM(Adw, AlertDialog, ResponseAppearance, AdwResponseAppearance)
_CONV_INCLASS_ENUM(Adw, Banner, ButtonStyle, AdwBannerButtonStyle)
_CONV_INCLASS_ENUM(Adw, BreakpointCondition, LengthType)
_CONV_INCLASS_ENUM(Adw, BreakpointCondition, RatioType)
_CONV_INCLASS_ENUM(Adw, Dialog, PresentationMode)
_CONV_INCLASS_ENUM(Adw, Flap, FoldPolicy)
_CONV_INCLASS_ENUM(Adw, Flap, TransitionType)
_CONV_INCLASS_ENUM(Adw, InlineViewSwitcher, DisplayMode)
_CONV_INCLASS_ENUM(Adw, Leaflet, TransitionType)
_CONV_INCLASS_ENUM(Adw, Squeezer, TransitionType)
_CONV_INCLASS_ENUM(Adw, TabView, Shortcuts)
_CONV_INCLASS_ENUM(Adw, Toast, Priority)
_CONV_INCLASS_ENUM(Adw, ToolbarView, Style, AdwToolbarStyle)
_CONV_INCLASS_ENUM(Adw, ViewSwitcher, Policy)
