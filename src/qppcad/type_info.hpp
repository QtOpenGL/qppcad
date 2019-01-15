#ifndef QPP_CAD_TYPE_INFO_H
#define QPP_CAD_TYPE_INFO_H
#include <string>

namespace qpp {

  namespace cad {

    //inspired by Urho3d https://github.com/urho3d
    class type_info {

      private:

        size_t p_type_hash;
        std::string p_type_name;
        const type_info* p_base_type_info;

      public:

        type_info(std::string type_name, const type_info* base_type_info);
        ~type_info();

        bool is_type_of(size_t type) const;

        bool is_type_of(const type_info *_typeInfo) const;

        template<typename T> bool is_type_of() const {
          return is_type_of(T::get_type_info_static());
        }

        size_t get_type() const {
          return p_type_hash;
        }

        const std::string& get_type_name() const {
          return p_type_name;
        }

        const type_info* get_base_type_info() const {
          return p_base_type_info;
        }

    };

  }

}

#endif
