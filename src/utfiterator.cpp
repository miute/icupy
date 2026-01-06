#include "main.hpp"

#if (U_ICU_VERSION_MAJOR_NUM >= 78)
#include <pybind11/stl.h>
#include <unicode/utfiterator.h>

namespace header = U_HEADER_ONLY_NAMESPACE;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)

void init_utfiterator(py::module &m, py::module &h) {
#if (U_ICU_VERSION_MAJOR_NUM >= 78)
  //
  // enum UTFIllFormedBehavior
  //
  py::enum_<UTFIllFormedBehavior>(
      m, "UTFIllFormedBehavior", py::arithmetic(),
      "Some defined behaviors for handling ill-formed Unicode strings.\n\n  "
      "This is a template parameter for UTFIterator and related classes.\n\n  "
      "When a validating UTFIterator encounters an ill-formed code unit "
      "sequence, then "
      "CodeUnits.codePoint() is a value according to this parameter.")
      .value("UTF_BEHAVIOR_NEGATIVE", UTF_BEHAVIOR_NEGATIVE,
             "Returns a negative value (-1=U_SENTINEL) instead of a code "
             "point.\n\n  "
             "If the CP32 template parameter for the relevant classes is an "
             "unsigned type, then the negative value "
             "becomes 0xffffffff=UINT32_MAX.")
      .value("UTF_BEHAVIOR_FFFD", UTF_BEHAVIOR_FFFD,
             "Returns U+FFFD Replacement Character.")
      .value("UTF_BEHAVIOR_SURROGATE", UTF_BEHAVIOR_SURROGATE,
             "UTF-8: Not allowed; UTF-16: returns the unpaired surrogate; "
             "UTF-32: returns the surrogate code point, or "
             "U+FFFD if out of range.")
      .export_values();

  //
  // template<typename CP32>
  // class U_HEADER_ONLY_NAMESPACE::AllCodePoints< CP32 >
  //
  using AllCodePoints = header::AllCodePoints<int32_t>;

  py::class_<AllCodePoints> acp(h, "AllCodePoints");

  acp.def(py::init<>());

  acp.def(
      "__iter__",
      [](AllCodePoints &self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>());

  //
  // template<typename CP32>
  // class U_HEADER_ONLY_NAMESPACE::AllScalarValues< CP32 >
  //
  using AllScalarValues = header::AllScalarValues<int32_t>;

  py::class_<AllScalarValues> asv(h, "AllScalarValues");

  asv.def(py::init<>());

  asv.def(
      "__iter__",
      [](AllScalarValues &self) {
        return py::make_iterator(self.begin(), self.end());
      },
      py::keep_alive<0, 1>());

  //
  // template<typename CP32, typename UnitIter, typename = void>
  // class U_HEADER_ONLY_NAMESPACE::UnsafeCodeUnits< CP32, UnitIter, typename >
  //
  using UnsafeCodeUnits =
      header::UnsafeCodeUnits<int32_t, std::u16string::iterator>;
  using CodeUnits = header::CodeUnits<int32_t, std::u16string::iterator>;

  py::class_<UnsafeCodeUnits>(h, "UnsafeCodeUnits")
      .def(
          "__getitem__",
          [](const UnsafeCodeUnits &self, int32_t index) -> int32_t {
            auto normalized_index = index;
            const auto length = self.length();
            if (normalized_index < 0) {
              normalized_index += length;
            }
            if (normalized_index < 0 || normalized_index >= length) {
              throw py::index_error("index out of range: " +
                                    std::to_string(index));
            }
            return *(self.begin() + normalized_index);
          },
          py::arg("index"))
      .def("__len__", [](const UnsafeCodeUnits &self) { return self.length(); })
      .def("__str__",
           [](const UnsafeCodeUnits &self) { return self.stringView(); })
      .def("code_point",
           [](const UnsafeCodeUnits &self) { return self.codePoint(); })
      .def("length", [](const UnsafeCodeUnits &self) { return self.length(); })
      .def("string_view",
           [](const UnsafeCodeUnits &self) { return self.stringView(); });

  //
  // template<typename CP32, typename UnitIter, typename = void>
  // class U_HEADER_ONLY_NAMESPACE::CodeUnits< CP32, UnitIter, typename >
  //
  py::class_<CodeUnits, UnsafeCodeUnits>(h, "CodeUnits")
      .def("well_formed",
           [](const CodeUnits &self) { return self.wellFormed(); });

  //
  // template<typename CP32, typename Range>
  // class U_HEADER_ONLY_NAMESPACE::UnsafeUTFStringCodePoints< CP32, Range >
  //
  using UnsafeUTFStringCodePoints =
      header::UnsafeUTFStringCodePoints<int32_t, std::u16string>;

  py::class_<UnsafeUTFStringCodePoints>(h, "UnsafeUTFStringCodePoints")
      .def(py::init<std::u16string>(), py::arg("unit_range"))
      .def(
          "__iter__",
          [](UnsafeUTFStringCodePoints &self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>());

  //
  // template<typename CP32, UTFIllFormedBehavior behavior, typename Range>
  // class U_HEADER_ONLY_NAMESPACE::UTFStringCodePoints< CP32, behavior, Range >
  //
  using UTFStringCodePointsNegative =
      header::UTFStringCodePoints<int32_t, UTF_BEHAVIOR_NEGATIVE,
                                  std::u16string>;
  using UTFStringCodePointsFFFD =
      header::UTFStringCodePoints<int32_t, UTF_BEHAVIOR_FFFD, std::u16string>;
  using UTFStringCodePointsSurrogate =
      header::UTFStringCodePoints<int32_t, UTF_BEHAVIOR_SURROGATE,
                                  std::u16string>;

  py::class_<UTFStringCodePointsNegative>(h, "UTFStringCodePointsNegative")
      .def(py::init<std::u16string>(), py::arg("unit_range"))
      .def(
          "__iter__",
          [](UTFStringCodePointsNegative &self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>());

  py::class_<UTFStringCodePointsFFFD>(h, "UTFStringCodePointsFFFD")
      .def(py::init<std::u16string>(), py::arg("unit_range"))
      .def(
          "__iter__",
          [](UTFStringCodePointsFFFD &self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>());

  py::class_<UTFStringCodePointsSurrogate>(h, "UTFStringCodePointsSurrogate")
      .def(py::init<std::u16string>(), py::arg("unit_range"))
      .def(
          "__iter__",
          [](UTFStringCodePointsSurrogate &self) {
            return py::make_iterator(self.begin(), self.end());
          },
          py::keep_alive<0, 1>());
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)
}
