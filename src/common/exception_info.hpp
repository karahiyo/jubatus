// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011,2012 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <cstring>
#include <typeinfo>
#include <pficommon/lang/shared_ptr.h>
#include <pficommon/lang/cast.h>

namespace jubatus {
namespace exception {

namespace detail {
std::string demangle_symbol(const char *symbol);
}

class error_info_base {
public:
  virtual bool splitter() const = 0;
  virtual std::string tag_typeid_name() const = 0;
  virtual std::string as_string() const = 0;

  virtual ~error_info_base() throw()
  {}
};

template <class Tag, class V>
class error_info;

template<>
class error_info<struct error_splitter_, void> : public error_info_base {
public:
  bool splitter() const
  {
    return true;
  }

  std::string tag_typeid_name() const
  {
    return jubatus::exception::detail::demangle_symbol(typeid(struct error_splitter_*).name());
  }

  std::string as_string() const
  {
    // USE splitter or tag_typeid_name
    return "-splitter-";
  }
};

template<>
class error_info<struct error_errno_, int> : public error_info_base {
public:
  error_info(int err)
    : value_(err)
  {
  }

  bool splitter() const
  {
    return false;
  }

  std::string tag_typeid_name() const
  {
    return jubatus::exception::detail::demangle_symbol(typeid(struct error_errno_*).name());
  }

  std::string as_string() const
  {
    std::string msg(strerror(value_));
    msg += " (" + pfi::lang::lexical_cast<std::string>(value_) + ")";
    return msg;
  }
private:
  int value_;
};

template <class Tag, class V>
class error_info : public error_info_base {
public:
  typedef V value_type;
  error_info(value_type v);
  ~error_info() throw();

  bool splitter() const;
  std::string tag_typeid_name() const;
  std::string as_string() const;

  value_type value() const
  {
    return value_;
  }

private:
  value_type value_;
};

template <class Tag, class V>
inline error_info<Tag, V>::error_info(value_type v)
  : value_(v)
{
}

template <class Tag, class V>
inline error_info<Tag, V>::~error_info() throw()
{
}

template <class Tag, class V>
inline bool error_info<Tag, V>::splitter() const
{
  return false;
}

template <class Tag, class V>
inline std::string error_info<Tag, V>::tag_typeid_name() const
{
  return jubatus::exception::detail::demangle_symbol(typeid(Tag*).name());
}

template <class Tag, class V>
inline std::string error_info<Tag, V>::as_string() const
{
  // TODO: implement generic and user defined converter to std::string
  return pfi::lang::lexical_cast<std::string>(value_);
}

} // exception
} // jubatus
