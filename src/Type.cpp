/***************************************************************************
 *   Copyright (C) 2016 by pgRouting developers                            *
 *   project@pgrouting.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License t &or more details.                        *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <boost/lexical_cast.hpp>
#include <string>
#include <map>
#include "./Class.h"
#include "./Type.h"

namespace osm2pgr {

void Type::AddClass(const Class &pClass) {
    m_Classes[pClass.name()] = pClass;
}


Type::Type(const char **atts) {
    auto **attribut = atts;
    while (*attribut != NULL) {
        std::string name = *attribut++;
        std::string value = *attribut++;
        if (name == "id") {
            m_id = boost::lexical_cast<int64_t>(value);
        } else if (name == "name") {
            m_name = value;
        } else {
            auto tag_key = boost::lexical_cast<std::string>(name);
            auto tag_value = boost::lexical_cast<std::string>(value);
            m_tags[tag_key] = tag_value;
        }
    }
}

void
Type::add_class(const char **atts) {
    AddClass(Class(atts));
}


}  // end namespace osm2pgr
