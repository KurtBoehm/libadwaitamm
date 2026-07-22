#ifndef _ADW_SELECTIONLISTMODELIMPL_H
#define _ADW_SELECTIONLISTMODELIMPL_H

/* Copyright (C) 2021 The gtkmm Development Team
 * Copyright (C) 2026 Kurt Böhm <kurbo96@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>

namespace Adw {
/** %Adw::SelectionListModelImpl is an object that implements the
 * Gio::ListModel, Gtk::SelectionModel and Gtk::SectionModel interfaces.
 *
 * gtk_stack_get_pages() and adw_sidebar_get_items() return an object of a
 * class that implements GListModel, GtkSelectionModel and GtkSectionModel.
 * Since that C class is not public, it's not wrapped in a C++ class. A C
 * object of such a class can be wrapped in a %Adw::SelectionListModelImpl
 * object. %Adw::SelectionListModelImpl does not directly correspond to any
 * GTK class.
 *
 * This class is intended only for wrapping C objects returned from GTK
 * functions.
 *
 * @see Gtk::Stack::get_pages()
 * @see Adw::Sidebar::get_items()
 * @newin{4,2}
 */

class SelectionListModelImpl : public Gio::ListModel,
                               public Gtk::SelectionModel,
                               public Gtk::SectionModel,
                               public Glib::Object {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
  // noncopyable
  SelectionListModelImpl(const SelectionListModelImpl&) = delete;
  SelectionListModelImpl& operator=(const SelectionListModelImpl&) = delete;

protected:
  explicit SelectionListModelImpl(GObject* castitem);
  friend class Leaflet;
  friend class Sidebar;
  friend class Squeezer;
  friend class TabView;
  friend class ViewStack;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:
  SelectionListModelImpl(SelectionListModelImpl&& src) noexcept;
  SelectionListModelImpl& operator=(SelectionListModelImpl&& src) noexcept;

  ~SelectionListModelImpl() noexcept override;
};
} // namespace Adw

#endif /* _ADW_SELECTIONLISTMODELIMPL_H */
