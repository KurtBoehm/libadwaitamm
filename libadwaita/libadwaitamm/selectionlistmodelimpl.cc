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
#include <libadwaitamm/selectionlistmodelimpl.h>
#include <utility>

namespace Adw {
SelectionListModelImpl::SelectionListModelImpl(GObject* castitem)
    : Glib::ObjectBase(nullptr), Glib::Object(castitem) {}

SelectionListModelImpl::SelectionListModelImpl(SelectionListModelImpl&& src) noexcept
    : Gio::ListModel(std::move(src)), Gtk::SelectionModel(std::move(src)),
      Gtk::SectionModel(std::move(src)), Glib::Object(std::move(src)) {}

SelectionListModelImpl& SelectionListModelImpl::operator=(SelectionListModelImpl&& src) noexcept {
  Gio::ListModel::operator=(std::move(src));
  Gtk::SelectionModel::operator=(std::move(src));
  Gtk::SectionModel::operator=(std::move(src));
  Glib::Object::operator=(std::move(src));
  return *this;
}

SelectionListModelImpl::~SelectionListModelImpl() noexcept {}
} // namespace Adw
