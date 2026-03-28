#ifndef ICUPY_UVERSION_HPP
#define ICUPY_UVERSION_HPP

#include <algorithm>
#include <unicode/uversion.h>

namespace icupy {

//
// struct icupy::VersionInfo
//
struct VersionInfo {
  UVersionInfo data{};

  VersionInfo(uint8_t major, uint8_t minor, uint8_t patchlevel,
              uint8_t buildlevel) {
    data[0] = major;
    data[1] = minor;
    data[2] = patchlevel;
    data[3] = buildlevel;
  }

  VersionInfo(const UVersionInfo &info) {
    std::copy(info, info + U_MAX_VERSION_LENGTH, data);
  }
};

} // namespace icupy

#endif // ICUPY_UVERSION_HPP
