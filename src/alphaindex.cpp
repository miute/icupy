#include "main.hpp"
#include "voidptr.hpp"
#include <unicode/alphaindex.h>
#include <unicode/tblcoll.h>
#include <unicode/uniset.h>

using namespace icu;

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
using Bucket = AlphabeticIndex::Bucket;
using ImmutableIndex = AlphabeticIndex::ImmutableIndex;
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

void init_alphaindex(py::module &m) {
  py::enum_<UAlphabeticIndexLabelType>(m, "UAlphabeticIndexLabelType", py::arithmetic(),
                                       "Constants for Alphabetic Index Label Types.\n\n"
                                       "The form of these enum constants anticipates having a plain C API for "
                                       "Alphabetic Indexes that will also use them.")
      .value("U_ALPHAINDEX_NORMAL", U_ALPHAINDEX_NORMAL,
             "Normal Label, typically the starting letter of the names in the bucket with this label.")
      .value("U_ALPHAINDEX_UNDERFLOW", U_ALPHAINDEX_UNDERFLOW,
             "Undeflow Label.\n\n  "
             "The bucket with this label contains names in scripts that sort before any of the bucket labels in this "
             "index.")
      .value("U_ALPHAINDEX_INFLOW", U_ALPHAINDEX_INFLOW,
             "Inflow Label.\n\n  "
             "The bucket with this label contains names in scripts that sort between two of the bucket labels in this "
             "index. Inflow labels are created when an index contains normal labels for multiple scripts, and skips "
             "other scripts that sort between some of the included scripts.")
      .value("U_ALPHAINDEX_OVERFLOW", U_ALPHAINDEX_OVERFLOW,
             "Overflow Label.\n\n  "
             "Te bucket with this label contains names in scripts that sort after all of the bucket labels in this "
             "index.")
      .export_values();

  // icu::AlphabeticIndex
  py::class_<AlphabeticIndex, UObject> ai(m, "AlphabeticIndex");

#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  // icu::AlphabeticIndex::Bucket
  py::class_<AlphabeticIndex::Bucket, UObject> bkt(ai, "Bucket");
  bkt.def("get_label", &Bucket::getLabel);
  bkt.def("get_label_type", &Bucket::getLabelType);

  // icu::AlphabeticIndex::ImmutableIndex
  py::class_<AlphabeticIndex::ImmutableIndex, UObject> ii(ai, "ImmutableIndex");
  ii.def("get_bucket", &ImmutableIndex::getBucket, py::return_value_policy::reference, py::arg("index"));
  ii.def("get_bucket_count", &ImmutableIndex::getBucketCount);
  ii.def(
        "get_bucket_index",
        [](const ImmutableIndex &self, const UnicodeString &name) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.getBucketIndex(name, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("name"))
      .def(
          // const char16_t *name -> const UnicodeString &name
          "get_bucket_index",
          [](const ImmutableIndex &self, const char16_t *name) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBucketIndex(name, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)

  // icu::AlphabeticIndex
  ai.def(py::init([](const Locale &locale) {
           UErrorCode error_code = U_ZERO_ERROR;
           auto result = std::make_unique<AlphabeticIndex>(locale, error_code);
           if (U_FAILURE(error_code)) {
             throw ICUError(error_code);
           }
           return result;
         }),
         py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          py::init([](const char *locale) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<AlphabeticIndex>(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("locale"))
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
      .def(py::init([](RuleBasedCollator *collator) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = std::make_unique<AlphabeticIndex>(
                 reinterpret_cast<RuleBasedCollator *>(collator ? collator->clone() : nullptr), error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           }),
           py::arg("collator"))
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
      ;
  ai.def(
        // [1] AlphabeticIndex::addLabels
        "add_labels",
        [](AlphabeticIndex &self, const Locale &locale) -> AlphabeticIndex & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.addLabels(locale, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("locale"))
      .def(
          // const char *locale -> const Locale &locale
          "add_labels",
          [](AlphabeticIndex &self, const char *locale) -> AlphabeticIndex & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.addLabels(locale, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("locale"))
      .def(
          // [2] AlphabeticIndex::addLabels
          "add_labels",
          [](AlphabeticIndex &self, const UnicodeSet &additions) -> AlphabeticIndex & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.addLabels(additions, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("additions"));
  ai.def(
        "add_record",
        [](AlphabeticIndex &self, const UnicodeString &name, _ConstVoidPtr *data) -> AlphabeticIndex & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.addRecord(name, data, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("name"), py::arg("data"))
      .def(
          // const char16_t *name -> const UnicodeString &name
          "add_record",
          [](AlphabeticIndex &self, const char16_t *name, _ConstVoidPtr *data) -> AlphabeticIndex & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.addRecord(name, data, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("name"), py::arg("data"));
#if (U_ICU_VERSION_MAJOR_NUM >= 51)
  ai.def("build_immutable_index", [](AlphabeticIndex &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.buildImmutableIndex(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
#endif // (U_ICU_VERSION_MAJOR_NUM >= 51)
  ai.def("clear_records", [](AlphabeticIndex &self) -> AlphabeticIndex & {
    UErrorCode error_code = U_ZERO_ERROR;
    auto &result = self.clearRecords(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ai.def("get_bucket_count", [](AlphabeticIndex &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getBucketCount(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ai.def("get_bucket_index", py::overload_cast<>(&AlphabeticIndex::getBucketIndex, py::const_))
      .def(
          "get_bucket_index",
          [](AlphabeticIndex &self, const UnicodeString &item_name) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBucketIndex(item_name, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("item_name"))
      .def(
          // const char16_t *item_name -> const UnicodeString &item_name
          "get_bucket_index",
          [](AlphabeticIndex &self, const char16_t *item_name) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.getBucketIndex(item_name, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("item_name"));
  ai.def("get_bucket_label", &AlphabeticIndex::getBucketLabel);
  ai.def("get_bucket_label_type", &AlphabeticIndex::getBucketLabelType);
  ai.def("get_bucket_record_count", &AlphabeticIndex::getBucketRecordCount);
  ai.def("get_collator", &AlphabeticIndex::getCollator, py::return_value_policy::reference);
  ai.def("get_inflow_label", &AlphabeticIndex::getInflowLabel);
  ai.def("get_max_label_count", &AlphabeticIndex::getMaxLabelCount);
  ai.def("get_overflow_label", &AlphabeticIndex::getOverflowLabel);
  ai.def("get_record_count", [](AlphabeticIndex &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getRecordCount(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ai.def("get_record_data", [](const AlphabeticIndex &self) {
    return reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(self.getRecordData()));
  });
  ai.def("get_record_name", &AlphabeticIndex::getRecordName);
  ai.def("get_underflow_label", &AlphabeticIndex::getUnderflowLabel);
  ai.def("next_bucket", [](AlphabeticIndex &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.nextBucket(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ai.def("next_record", [](AlphabeticIndex &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.nextRecord(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ai.def("reset_bucket_iterator", [](AlphabeticIndex &self) -> AlphabeticIndex & {
    UErrorCode error_code = U_ZERO_ERROR;
    auto &result = self.resetBucketIterator(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  ai.def("reset_record_iterator", &AlphabeticIndex::resetRecordIterator);
  ai.def(
        "set_inflow_label",
        [](AlphabeticIndex &self, const UnicodeString &inflow_label) -> AlphabeticIndex & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.setInflowLabel(inflow_label, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("inflow_label"))
      .def(
          // const char16_t *inflow_label -> const UnicodeString &inflow_label
          "set_inflow_label",
          [](AlphabeticIndex &self, const char16_t *inflow_label) -> AlphabeticIndex & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.setInflowLabel(inflow_label, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("inflow_label"));
  ai.def(
      "set_max_label_count",
      [](AlphabeticIndex &self, int32_t max_label_count) -> AlphabeticIndex & {
        UErrorCode error_code = U_ZERO_ERROR;
        auto &result = self.setMaxLabelCount(max_label_count, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("max_label_count"));
  ai.def(
        "set_overflow_label",
        [](AlphabeticIndex &self, const UnicodeString &overflow_label) -> AlphabeticIndex & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.setOverflowLabel(overflow_label, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("overflow_label"))
      .def(
          // const char16_t *overflow_label -> const UnicodeString &overflow_label
          "set_overflow_label",
          [](AlphabeticIndex &self, const char16_t *overflow_label) -> AlphabeticIndex & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.setOverflowLabel(overflow_label, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("overflow_label"));
  ai.def(
        "set_underflow_label",
        [](AlphabeticIndex &self, const UnicodeString &underflow_label) -> AlphabeticIndex & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.setUnderflowLabel(underflow_label, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("underflow_label"))
      .def(
          // const char16_t *underflow_label -> const UnicodeString &underflow_label
          "set_underflow_label",
          [](AlphabeticIndex &self, const char16_t *underflow_label) -> AlphabeticIndex & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.setUnderflowLabel(underflow_label, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("underflow_label"));
}
