#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/basictz.h>
#include <unicode/calendar.h>
#include <unicode/curramt.h>
#include <unicode/dtintrv.h>
#include <unicode/fmtable.h>
#include <unicode/tmutamt.h>

using namespace icu;

void init_fmtable(py::module &m, py::class_<Formattable, UObject> &fmt) {
  // icu::Formattable
  py::enum_<Formattable::ISDATE>(
      fmt, "ISDATE", py::arithmetic(),
      "This enum is only used to let callers distinguish between the *Formattable(UDate)* constructor and the "
      "*Formattable(double)* constructor; the compiler cannot distinguish the signatures, since UDate is currently "
      "typedefed to be either double or long.\n\n"
      "If UDate is changed later to be a bonafide class or struct, then we no longer need this enum.")
      .value("IS_DATE", Formattable::ISDATE::kIsDate)
      .export_values();

  py::enum_<Formattable::Type>(
      fmt, "Type", py::arithmetic(),
      "Selector for flavor of data type contained within a *Formattable* object.\n\n"
      "*Formattable* is a union of several different types, and at any time contains exactly one type.")
      .value("DATE", Formattable::Type::kDate,
             "Selector indicating a UDate value.\n\n  "
             "Use *get_date* to retrieve the value.")
      .value("DOUBLE", Formattable::Type::kDouble,
             "Selector indicating a double value.\n\n  "
             "Use *get_double* to retrieve the value.")
      .value("LONG", Formattable::Type::kLong,
             "Selector indicating a 32-bit integer value.\n\n  "
             "Use *get_long* to retrieve the value.")
      .value("STRING", Formattable::Type::kString,
             "Selector indicating a UnicodeString value.\n\n  "
             "Use *get_string* to retrieve the value.")
      .value("ARRAY", Formattable::Type::kArray,
             "Selector indicating an array of Formattables.\n\n  "
             "Use *get_array* to retrieve the value.")
      .value("INT64", Formattable::Type::kInt64,
             "Selector indicating a 64-bit integer value.\n\n  "
             "Use *get_int64* to retrieve the value.")
      .value("OBJECT", Formattable::Type::kObject,
             "Selector indicating a UObject value.\n\n  "
             "Use *get_object* to retrieve the value.")
      .export_values();

  fmt.def(py::init<>())
      .def(py::init<UDate, Formattable::ISDATE>(), py::arg("d"), py::arg("flag"))
      .def(py::init<double>(), py::arg("d").noconvert())
      .def(py::init<int32_t>(), py::arg("l"))
      .def(py::init<int64_t>(), py::arg("ll"))
      .def(
          // [7] Formattable::Formattable(StringPiece number, UErrorCode &status)
          py::init([](const char *number) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<Formattable>(number, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("number"))
      .def(py::init<const UnicodeString &>(), py::arg("str_to_copy"))
      .def(py::init<const Formattable &>(), py::arg("source"))
      .def(py::init([](const std::vector<Formattable> &array_to_copy, int32_t count) {
             if (count == -1) {
               count = static_cast<int32_t>(array_to_copy.size());
             }
             return std::make_unique<Formattable>(array_to_copy.data(), count);
           }),
           py::arg("array_to_copy"), py::arg("count") = -1)
      // FIXME: Implement "icu::Formattable::Formattable(UObject *objectToAdopt)".
      .def(py::init(
               [](const Calendar *object_to_adopt) { return std::make_unique<Formattable>(object_to_adopt->clone()); }),
           py::arg("object_to_adopt").none(false))
      .def(py::init([](const DateInterval *object_to_adopt) {
             return std::make_unique<Formattable>(object_to_adopt->clone());
           }),
           py::arg("object_to_adopt").none(false))
      .def(py::init(
               [](const Measure *object_to_adopt) { return std::make_unique<Formattable>(object_to_adopt->clone()); }),
           py::arg("object_to_adopt").none(false))
      .def(py::init(
               [](const TimeZone *object_to_adopt) { return std::make_unique<Formattable>(object_to_adopt->clone()); }),
           py::arg("object_to_adopt").none(false))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  fmt.def("__copy__", &Formattable::clone)
      .def(
          "__deepcopy__", [](const Formattable &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__getitem__",
          [](Formattable &self, int32_t index) -> Formattable & {
            UErrorCode error_code = U_ZERO_ERROR;
            int32_t count;
            self.getArray(count, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            if (index < 0) {
              index += count;
            }
            if (index < 0 || index >= count) {
              throw py::index_error(std::to_string(index));
            }
            return self[index];
          },
          py::arg("index"));
  // FIXME: Implement "void icu::Formattable::adoptArray(Formattable *array, int32_t count)".
  // FIXME: Implement "void icu::Formattable::adoptObject(UObject *objectToAdopt)".
  // FIXME: Implement "void icu::Formattable::adoptString(UnicodeString *stringToAdopt)".
  fmt.def("clone", &Formattable::clone);
  // TODO: Implement "const Formattable *icu::Formattable::fromUFormattable(const UFormattable *fmt)".
  // TODO: Implement "Formattable *icu::Formattable::fromUFormattable(UFormattable *fmt)".
  fmt.def("get_array", [](const Formattable &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    int32_t count;
    auto array = self.getArray(count, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    std::vector<Formattable> result(count);
    for (int32_t i = 0; i < count; ++i) {
      result[i] = array[i];
    }
    return result;
  });
  fmt.def("get_date", [](const Formattable &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getDate(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  fmt.def("get_decimal_number", [](Formattable &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto str = self.getDecimalNumber(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return py::str(str.data());
  });
  fmt.def("get_double", [](const Formattable &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getDouble(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  fmt.def("get_int64", [](const Formattable &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getInt64(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  fmt.def("get_long", [](const Formattable &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto result = self.getLong(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return result;
  });
  fmt.def(
      "get_object",
      [](const Formattable &self)
          -> std::variant<const BasicTimeZone *, const Calendar *, const CurrencyAmount *, const DateInterval *,
                          const TimeUnitAmount *, const TimeZone *, const UObject *> {
        auto obj = self.getObject();
        if (obj) {
          auto cal = dynamic_cast<const Calendar *>(obj);
          if (cal) {
            return cal;
          }
          auto camt = dynamic_cast<const CurrencyAmount *>(obj);
          if (camt) {
            return camt;
          }
          auto dtitv = dynamic_cast<const DateInterval *>(obj);
          if (dtitv) {
            return dtitv;
          }
          auto tuamt = dynamic_cast<const TimeUnitAmount *>(obj);
          if (tuamt) {
            return tuamt;
          }
          auto btz = dynamic_cast<const BasicTimeZone *>(obj);
          if (btz) {
            return btz;
          }
          auto tz = dynamic_cast<const TimeZone *>(obj);
          if (tz) {
            return tz;
          }
        }
        return obj;
      },
      py::return_value_policy::reference);
  fmt.def("get_string",
          [](const Formattable &self) -> const UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.getString(error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          })
      .def(
          "get_string",
          [](const Formattable &self, UnicodeString &result) -> UnicodeString & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &value = self.getString(result, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return value;
          },
          py::arg("result"));
  fmt.def("get_type", &Formattable::getType);
  fmt.def("is_numeric", &Formattable::isNumeric);
  fmt.def(
      "set_array",
      [](Formattable &self, const std::vector<Formattable> &array, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(array.size());
        }
        self.setArray(array.data(), count);
      },
      py::arg("array"), py::arg("count") = -1);
  fmt.def("set_date", &Formattable::setDate, py::arg("d"));
  fmt.def(
      "set_decimal_number",
      [](Formattable &self, const char *number_string) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setDecimalNumber(number_string, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("number_string"));
  fmt.def("set_double", &Formattable::setDouble, py::arg("d"));
  fmt.def("set_int64", &Formattable::setInt64, py::arg("ll"));
  fmt.def("set_long", &Formattable::setLong, py::arg("l"));
  fmt.def("set_string", &Formattable::setString, py::arg("string_to_copy"))
      .def(
          // const char16_t *string_to_copy -> const UnicodeString &string_to_copy
          "set_string", [](Formattable &self, const char16_t *string_to_copy) { self.setString(string_to_copy); },
          py::arg("string_to_copy"));
  // TODO: Implement "UFormattable *icu::Formattable::toUFormattable()".
  // TODO: Implement "const UFormattable *icu::Formattable::toUFormattable() const".
}
