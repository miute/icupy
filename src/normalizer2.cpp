#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/normalizer2.h>

using namespace icu;

void init_normalizer2(py::module &m) {
  py::class_<Normalizer2, UObject> n2(m, "Normalizer2");
  n2.def(
      "append",
      [](const Normalizer2 &self, UnicodeString &first, const _UnicodeStringVariant &second) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.append(first, VARIANT_TO_UNISTR(second), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("first"), py::arg("second"));
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  n2.def("compose_pair", &Normalizer2::composePair, py::arg("a"), py::arg("b"));
  n2.def("get_combining_class", &Normalizer2::getCombiningClass, py::arg("c"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
  n2.def("get_decomposition", &Normalizer2::getDecomposition, py::arg("c"), py::arg("decomposition"));
  n2.def_static(
      "get_instance",
      [](const char *package_name, const char *name, UNormalization2Mode mode) {
        ErrorCode error_code;
        auto result = Normalizer2::getInstance(package_name, name, mode, error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("package_name"), py::arg("name"), py::arg("mode"));
#if (U_ICU_VERSION_MAJOR_NUM >= 49)
  n2.def_static(
      "get_nfc_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFCInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  n2.def_static(
      "get_nfd_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFDInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  n2.def_static(
      "get_nfkc_casefold_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKCCasefoldInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  n2.def_static(
      "get_nfkc_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKCInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  n2.def_static(
      "get_nfkd_instance",
      []() {
        ErrorCode error_code;
        auto result = Normalizer2::getNFKDInstance(error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference);
  n2.def("get_raw_decomposition", &Normalizer2::getRawDecomposition, py::arg("c"), py::arg("decomposition"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 49)
  n2.def("has_boundary_after", &Normalizer2::hasBoundaryAfter, py::arg("c"));
  n2.def("has_boundary_before", &Normalizer2::hasBoundaryBefore, py::arg("c"));
  n2.def("is_inert", &Normalizer2::isInert, py::arg("c"));
  n2.def(
      "is_normalized",
      [](const Normalizer2 &self, const _UnicodeStringVariant &s) {
        ErrorCode error_code;
        auto result = self.isNormalized(VARIANT_TO_UNISTR(s), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("s"));
  n2.def(
        "normalize",
        [](const Normalizer2 &self, const _UnicodeStringVariant &src) {
          ErrorCode error_code;
          auto result = self.normalize(VARIANT_TO_UNISTR(src), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("src"))
      .def(
          "normalize",
          [](const Normalizer2 &self, const _UnicodeStringVariant &src, UnicodeString &dest) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.normalize(VARIANT_TO_UNISTR(src), dest, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("src"), py::arg("dest"));
  n2.def(
      "normalize_second_and_append",
      [](const Normalizer2 &self, UnicodeString &first, const _UnicodeStringVariant &second) -> UnicodeString & {
        ErrorCode error_code;
        auto &result = self.normalizeSecondAndAppend(first, VARIANT_TO_UNISTR(second), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("first"), py::arg("second"));
  n2.def(
      "quick_check",
      [](const Normalizer2 &self, const _UnicodeStringVariant &s) {
        ErrorCode error_code;
        auto result = self.quickCheck(VARIANT_TO_UNISTR(s), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("s"));
  n2.def(
      "span_quick_check_yes",
      [](const Normalizer2 &self, const _UnicodeStringVariant &s) {
        ErrorCode error_code;
        auto result = self.spanQuickCheckYes(VARIANT_TO_UNISTR(s), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("s"));

  py::class_<FilteredNormalizer2, Normalizer2> fn2(m, "FilteredNormalizer2");
  fn2.def(py::init<const Normalizer2 &, const UnicodeSet &>(), py::arg("n2"), py::arg("filter_set"));
}
