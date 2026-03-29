#include "uversion.hpp"
#include "main.hpp"
#include <pybind11/stl.h>
#include <sstream>

void init_uversion(py::module &m) {
  //
  // struct icupy::VersionInfo
  //
  py::class_<icupy::VersionInfo> vi(m, "UVersionInfo", R"doc(
      Array of four unsigned 8-bit integers for the ICU API version.

      See Also:
          :func:`u_version_to_string`
      )doc");

  vi.def_property(
      "buildlevel", [](const icupy::VersionInfo &self) { return self.data[3]; },
      [](icupy::VersionInfo &self, uint8_t value) { self.data[3] = value; },
      R"doc(
      int: Get/Set the build level version number.

      This is equivalent to ``self[3]``.
      )doc");

  vi.def_property(
      "major", [](const icupy::VersionInfo &self) { return self.data[0]; },
      [](icupy::VersionInfo &self, uint8_t value) { self.data[0] = value; },
      R"doc(
      int: Get/Set the major version number.

      This is equivalent to ``self[0]``.
      )doc");

  vi.def_property(
      "minor", [](const icupy::VersionInfo &self) { return self.data[1]; },
      [](icupy::VersionInfo &self, uint8_t value) { self.data[1] = value; },
      R"doc(
      int: Get/Set the minor version number.

      This is equivalent to ``self[1]``.
      )doc");

  vi.def_property(
      "patchlevel", [](const icupy::VersionInfo &self) { return self.data[2]; },
      [](icupy::VersionInfo &self, uint8_t value) { self.data[2] = value; },
      R"doc(
      int: Get/Set the patchlevel version number.

      This is equivalent to ``self[2]``.
      )doc");

  vi.def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), py::arg("major") = 0,
         py::arg("minor") = 0, py::arg("patchlevel") = 0,
         py::arg("buildlevel") = 0, R"doc(
      Initialize a ``UVersionInfo`` instance with the specified *major*,
      *minor*, *patchlevel*, and *buildlevel* values.

      The values must be between 0 and 255.
      )doc");

  vi.def(
        "__eq__",
        [](const icupy::VersionInfo &self,
           const std::array<uint8_t, U_MAX_VERSION_LENGTH> &other) {
          return std::equal(std::begin(self.data), std::end(self.data),
                            std::begin(other));
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` == *other*.
      )doc")
      .def(
          "__eq__",
          [](const icupy::VersionInfo &self, const icupy::VersionInfo &other) {
            return std::equal(std::begin(self.data), std::end(self.data),
                              std::begin(other.data));
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` == *other*.
      )doc");

  vi.def(
      "__getitem__",
      [](const icupy::VersionInfo &self, int32_t index) {
        auto actual_index = index;
        if (actual_index < 0) {
          actual_index += U_MAX_VERSION_LENGTH;
        }
        if (actual_index < 0 || actual_index >= U_MAX_VERSION_LENGTH) {
          throw py::index_error("array index out of range: " +
                                std::to_string(index));
        }
        return self.data[actual_index];
      },
      py::arg("index"), R"doc(
      Return the value at offset *index*.
      )doc");

  vi.def(
        "__ge__",
        [](const icupy::VersionInfo &self,
           const std::array<uint8_t, U_MAX_VERSION_LENGTH> &other) {
          return !std::lexicographical_compare(
              std::begin(self.data), std::end(self.data), std::begin(other),
              std::end(other));
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` >= *other*.
      )doc")
      .def(
          "__ge__",
          [](const icupy::VersionInfo &self, const icupy::VersionInfo &other) {
            return !std::lexicographical_compare(
                std::begin(self.data), std::end(self.data),
                std::begin(other.data), std::end(other.data));
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` >= *other*.
      )doc");

  vi.def(
        "__gt__",
        [](const icupy::VersionInfo &self,
           const std::array<uint8_t, U_MAX_VERSION_LENGTH> &other) {
          return std::lexicographical_compare(
              std::begin(other), std::end(other), std::begin(self.data),
              std::end(self.data));
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` > *other*.
      )doc")
      .def(
          "__gt__",
          [](const icupy::VersionInfo &self, const icupy::VersionInfo &other) {
            return std::lexicographical_compare(
                std::begin(other.data), std::end(other.data),
                std::begin(self.data), std::end(self.data));
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` > *other*.
      )doc");

  vi.def(
      "__int__",
      [](const icupy::VersionInfo &self) -> uint32_t {
        return self.data[0] << 24 | self.data[1] << 16 | self.data[2] << 8 |
               self.data[3];
      },
      R"doc(
      Return ``major`` << 24 | ``minor`` << 16 | ``patchlevel`` << 8 |
      ``buildlevel``.

      Example:
          >>> from icupy import icu
          >>> vi = icu.UVersionInfo(1, 3, 31, 2)
          >>> int(vi)
          16981762
          >>> vi.major << 24 | vi.minor << 16 | vi.patchlevel << 8 | vi.buildlevel
          16981762
      )doc");

  vi.def(
        "__le__",
        [](const icupy::VersionInfo &self,
           const std::array<uint8_t, U_MAX_VERSION_LENGTH> &other) {
          return !std::lexicographical_compare(
              std::begin(other), std::end(other), std::begin(self.data),
              std::end(self.data));
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` <= *other*.
      )doc")
      .def(
          "__le__",
          [](const icupy::VersionInfo &self, const icupy::VersionInfo &other) {
            return !std::lexicographical_compare(
                std::begin(other.data), std::end(other.data),
                std::begin(self.data), std::end(self.data));
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` <= *other*.
      )doc");

  vi.def(
      "__len__",
      [](const icupy::VersionInfo &self) { return U_MAX_VERSION_LENGTH; },
      R"doc(
      Always return :attr:`U_MAX_VERSION_LENGTH`.
      )doc");

  vi.def(
        "__lt__",
        [](const icupy::VersionInfo &self,
           const std::array<uint8_t, U_MAX_VERSION_LENGTH> &other) {
          return std::lexicographical_compare(
              std::begin(self.data), std::end(self.data), std::begin(other),
              std::end(other));
        },
        py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` < *other*.
      )doc")
      .def(
          "__lt__",
          [](const icupy::VersionInfo &self, const icupy::VersionInfo &other) {
            return std::lexicographical_compare(
                std::begin(self.data), std::end(self.data),
                std::begin(other.data), std::end(other.data));
          },
          py::is_operator(), py::arg("other"), R"doc(
      Return ``self`` < *other*.
      )doc");

  vi.def("__repr__", [](const icupy::VersionInfo &self) {
    std::stringstream ss;
    ss << "UVersionInfo(";
    for (int i = 0; i < U_MAX_VERSION_LENGTH; ++i) {
      if (i > 0) {
        ss << ", ";
      }
      ss << static_cast<int>(self.data[i]);
    }
    ss << ")";
    return ss.str();
  });

  vi.def(
      "__setitem__",
      [](icupy::VersionInfo &self, int32_t index, uint8_t value) {
        auto actual_index = index;
        if (actual_index < 0) {
          actual_index += U_MAX_VERSION_LENGTH;
        }
        if (actual_index < 0 || actual_index >= U_MAX_VERSION_LENGTH) {
          throw py::index_error("array index out of range: " +
                                std::to_string(index));
        }
        self.data[actual_index] = value;
      },
      py::arg("index"), py::arg("value"), R"doc(
      Set the *value* at offset *index*.
      )doc");

  vi.def(
      "__str__",
      [](const icupy::VersionInfo &self) {
        std::stringstream ss;
        for (int i = 0; i < U_MAX_VERSION_LENGTH; ++i) {
          if (i > 0) {
            ss << ".";
          }
          ss << static_cast<int>(self.data[i]);
        }
        return ss.str();
      },
      R"doc(
      Return a dot-separated decimal version string.
      )doc");

  //
  // Functions
  //
  m.def(
      "u_get_version",
      []() {
        UVersionInfo version_array{};
        u_getVersion(version_array);
        return icupy::VersionInfo(version_array);
      },
      R"doc(
      Return the ICU release version information.
      )doc");

  m.def(
      "u_version_from_string",
      [](const std::string &version_string) {
        UVersionInfo version_array{};
        u_versionFromString(version_array, version_string.data());
        return icupy::VersionInfo(version_array);
      },
      py::arg("version_string"), R"doc(
      Parse a string with dotted-decimal version information and return the
      result.

      *version_string* is a dot-separated decimal string with up to four
      non-negative numeric fields, where the value of each field is between
      0 and 255.
      )doc");

  m.def(
       "u_version_to_string",
       [](const std::array<uint8_t, U_MAX_VERSION_LENGTH> &version_array) {
         char buf[U_MAX_VERSION_STRING_LENGTH + 1];
         memset(buf, 0, sizeof(buf));
         u_versionToString(version_array.data(), buf);
         return std::string(buf);
       },
       py::arg("version_array"), R"doc(
      Convert a sequence of numbers into a dot-separated decimal string, and
      return the result.
      )doc")
      .def(
          "u_version_to_string",
          [](const icupy::VersionInfo &version_array) {
            char buf[U_MAX_VERSION_STRING_LENGTH + 1];
            memset(buf, 0, sizeof(buf));
            u_versionToString(version_array.data, buf);
            return std::string(buf);
          },
          py::arg("version_array"), R"doc(
      Convert the :class:`UVersionInfo` to a dot-separated decimal string, and
      return the result.
      )doc");

  m.attr("U_MAX_VERSION_LENGTH") = U_MAX_VERSION_LENGTH;
  m.attr("U_VERSION_DELIMITER") = U_VERSION_DELIMITER;

  // <unicode/uvernum.h>
  m.attr("U_COPYRIGHT_STRING") = U_COPYRIGHT_STRING;
  m.attr("U_ICU_VERSION") = U_ICU_VERSION;
  m.attr("U_ICU_VERSION_BUILDLEVEL_NUM") = U_ICU_VERSION_BUILDLEVEL_NUM;
  m.attr("U_ICU_VERSION_MAJOR_NUM") = U_ICU_VERSION_MAJOR_NUM;
  m.attr("U_ICU_VERSION_MINOR_NUM") = U_ICU_VERSION_MINOR_NUM;
  m.attr("U_ICU_VERSION_PATCHLEVEL_NUM") = U_ICU_VERSION_PATCHLEVEL_NUM;
  m.attr("U_ICU_VERSION_SHORT") = U_ICU_VERSION_SHORT;
  m.attr("UCOL_BUILDER_VERSION") = UCOL_BUILDER_VERSION;
  m.attr("UCOL_RUNTIME_VERSION") = UCOL_RUNTIME_VERSION;
}
