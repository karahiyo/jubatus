#pragma once

#include <pficommon/text/json.h>

namespace jubatus {
namespace common {

class config {
 public:
  config(const pfi::text::json::json& js)
      : js_(js), path_("$") {}

  config(const pfi::text::json::json& js,
         const std::string& path)
      : js_(js), path_(path) {}

  template <typename T>
  T as() const {
    return pfi::text::json::json_cast<T>(js_);
  }

  config operator[](const std::string& key) const {
    std::string path = path_ + "." + key;
    return config(js_[key], path);
  }

  config operator[](size_t i) const {
    std::stringstream ss(path_);
    ss << "[" << i << "]";
    return config(js_[i], ss.str());
  }

  const std::string& get_path() const {
    return path_;
  }
  
 private:  
  const pfi::text::json::json& js_;
  std::string path_;
};

}
}
