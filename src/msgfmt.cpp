#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/msgfmt.h>
#include <unicode/strenum.h>

using namespace icu;

void init_msgfmt(py::module &m) {
  // icu::MessageFormat
  py::class_<MessageFormat, Format> mf(m, "MessageFormat");
  mf.def(
        // [1] MessageFormat::MessageFormat
        py::init([](const _UnicodeStringVariant &pattern) {
          ErrorCode error_code;
          auto result = std::make_unique<MessageFormat>(VARIANT_TO_UNISTR(pattern), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        }),
        py::arg("pattern"))
      .def(
          // [2] MessageFormat::MessageFormat
          py::init([](const _UnicodeStringVariant &pattern, const _LocaleVariant &new_locale) {
            ErrorCode error_code;
            auto result =
                std::make_unique<MessageFormat>(VARIANT_TO_UNISTR(pattern), VARIANT_TO_LOCALE(new_locale), error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("pattern"), py::arg("new_locale"))
      .def(
          // [3] MessageFormat::MessageFormat
          py::init(
              [](const _UnicodeStringVariant &pattern, const _LocaleVariant &new_locale, UParseError &parse_error) {
                ErrorCode error_code;
                auto result = std::make_unique<MessageFormat>(VARIANT_TO_UNISTR(pattern), VARIANT_TO_LOCALE(new_locale),
                                                              parse_error, error_code);
                if (error_code.isFailure()) {
                  throw ICUError(error_code);
                }
                return result;
              }),
          py::arg("pattern"), py::arg("new_locale"), py::arg("parse_error"))
      .def(
          // [4] MessageFormat::MessageFormat
          py::init<const MessageFormat &>(), py::arg("other"));
  mf.def("__copy__", &MessageFormat::clone)
      .def(
          "__deepcopy__", [](const MessageFormat &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def(
          "__eq__", [](const MessageFormat &self, const Format &other) { return self == other; }, py::is_operator(),
          py::arg("other"));
  // FIXME: Implement "void icu::MessageFormat::adoptFormat(const UnicodeString &formatName, Format *formatToAdopt,
  // UErrorCode &status)".
  // FIXME: Implement "void icu::MessageFormat::adoptFormat(int32_t formatNumber, Format *formatToAdopt)".
  // FIXME: Implement "void icu::MessageFormat::adoptFormats(Format **formatsToAdopt, int32_t count)".
  mf.def(
        // [1] MessageFormat::applyPattern
        "apply_pattern",
        [](MessageFormat &self, const _UnicodeStringVariant &pattern) {
          ErrorCode error_code;
          self.applyPattern(VARIANT_TO_UNISTR(pattern), error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
        },
        py::arg("pattern"))
      .def(
          // [2] MessageFormat::applyPattern
          "apply_pattern",
          [](MessageFormat &self, const _UnicodeStringVariant &pattern, UMessagePatternApostropheMode apos_mode,
             UParseError *parse_error) {
            ErrorCode error_code;
            self.applyPattern(VARIANT_TO_UNISTR(pattern), apos_mode, parse_error, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("apos_mode"), py::arg("parse_error"))
      .def(
          // [3] MessageFormat::applyPattern
          "apply_pattern",
          [](MessageFormat &self, const _UnicodeStringVariant &pattern, UParseError &parse_error) {
            ErrorCode error_code;
            self.applyPattern(VARIANT_TO_UNISTR(pattern), parse_error, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("pattern"), py::arg("parse_error"));
  mf.def_static(
      "auto_quote_apostrophe",
      [](const _UnicodeStringVariant &pattern) {
        ErrorCode error_code;
        auto result = MessageFormat::autoQuoteApostrophe(VARIANT_TO_UNISTR(pattern), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("pattern"));
  mf.def("clone", &MessageFormat::clone);
  mf.def(
        // [1] MessageFormat::format
        // [2] Format::format
        "format",
        [](const MessageFormat &self, const Formattable &obj, UnicodeString &append_to,
           FieldPosition &pos) -> UnicodeString & {
          ErrorCode error_code;
          auto &result = self.format(obj, append_to, pos, error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("obj"), py::arg("append_to"), py::arg("pos"))
      .def(
          // [3] Format::format
          "format",
          [](const MessageFormat &self, const Formattable &obj, UnicodeString &append_to,
             FieldPositionIterator *pos_iter) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, pos_iter, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"), py::arg("pos_iter"))
      .def(
          // [4] Format::format
          "format",
          [](const MessageFormat &self, const Formattable &obj, UnicodeString &append_to) -> UnicodeString & {
            ErrorCode error_code;
            auto &result = self.format(obj, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("obj"), py::arg("append_to"))
      .def(
          // [5] MessageFormat::format
          "format",
          [](const MessageFormat &self, const std::vector<Formattable> &source, int32_t count, UnicodeString &append_to,
             FieldPosition &ignore) -> UnicodeString & {
            if (count == -1) {
              count = static_cast<int32_t>(source.size());
            }
            ErrorCode error_code;
            auto &result = self.format(source.data(), count, append_to, ignore, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("source"), py::arg("count"), py::arg("append_to"), py::arg("ignore"))
      // FIXME: Implement "static UnicodeString& icu::MessageFormat::format(const UnicodeString &pattern,
      // const Formattable *arguments, int32_t count, UnicodeString &appendTo, UErrorCode &status)".
      /*
      .def_static(
          // [6] MessageFormat::format
          "format",
          [](const UnicodeString &pattern, const std::vector<Formattable> &arguments, int32_t count,
             UnicodeString &append_to) -> UnicodeString & {
            if (count == -1) {
              count = static_cast<int32_t>(arguments.size());
            }
            ErrorCode error_code;
            auto &result = MessageFormat::format(pattern, arguments.data(), count, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("arguments"), py::arg("count"), py::arg("append_to"))
      .def_static(
          // const char16_t *pattern -> const UnicodeString &pattern
          "format",
          [](const char16_t *pattern, const std::vector<Formattable> &arguments, int32_t count,
             UnicodeString &append_to) -> UnicodeString & {
            if (count == -1) {
              count = static_cast<int32_t>(arguments.size());
            }
            ErrorCode error_code;
            auto &result = MessageFormat::format(pattern, arguments.data(), count, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("pattern"), py::arg("arguments"), py::arg("count"), py::arg("append_to"))
      */
      .def(
          // [7] MessageFormat::format
          "format",
          [](const MessageFormat &self, const std::list<UnicodeString> &argument_names,
             const std::vector<Formattable> &arguments, int32_t count, UnicodeString &append_to) -> UnicodeString & {
            if (count == -1) {
              count = static_cast<int32_t>(argument_names.size());
            }
            std::vector<UnicodeString> _argument_names(std::begin(argument_names), std::end(argument_names));
            ErrorCode error_code;
            auto &result = self.format(_argument_names.data(), arguments.data(), count, append_to, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("argument_names"), py::arg("arguments"), py::arg("count"), py::arg("append_to"));
  mf.def("get_apostrophe_mode", &MessageFormat::getApostropheMode);
  mf.def(
      "get_format",
      [](MessageFormat &self, const _UnicodeStringVariant &format_name) {
        ErrorCode error_code;
        auto result = self.getFormat(VARIANT_TO_UNISTR(format_name), error_code);
        if (error_code.isFailure()) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("format_name"));
  mf.def("get_format_names", [](MessageFormat &self) {
    ErrorCode error_code;
    auto result = self.getFormatNames(error_code);
    if (error_code.isFailure()) {
      throw ICUError(error_code);
    }
    return result;
  });
  mf.def(
      "get_formats",
      [](const MessageFormat &self) {
        int32_t count;
        auto p = self.getFormats(count);
        std::vector<const Format *> result(count);
        for (int32_t i = 0; i < count; ++i) {
          result[i] = p[i];
        }
        return result;
      },
      py::return_value_policy::reference);
  mf.def("get_locale", &MessageFormat::getLocale);
  mf.def(
        // [1] MessageFormat::parse
        "parse",
        [](const MessageFormat &self, const _UnicodeStringVariant &source) {
          int32_t count;
          ErrorCode error_code;
          auto p = self.parse(VARIANT_TO_UNISTR(source), count, error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
          std::vector<Formattable *> result(count);
          for (int32_t i = 0; i < count; ++i) {
            result[i] = p + i;
          }
          return result;
        },
        py::return_value_policy::reference, py::arg("source"))
      .def(
          // [2] MessageFormat::parse
          "parse",
          [](const MessageFormat &self, const _UnicodeStringVariant &source, ParsePosition &pos) {
            int32_t count = 0;
            auto p = self.parse(VARIANT_TO_UNISTR(source), pos, count);
            std::vector<Formattable *> result(count);
            for (int32_t i = 0; i < count; ++i) {
              result[i] = p + i;
            }
            return result;
          },
          py::return_value_policy::reference, py::arg("source"), py::arg("pos"));
  mf.def(
        "parse_object",
        [](const MessageFormat &self, const _UnicodeStringVariant &source, Formattable &result, ParsePosition &pos) {
          self.parseObject(VARIANT_TO_UNISTR(source), result, pos);
        },
        py::arg("source"), py::arg("result"), py::arg("pos"))
      .def(
          "parse_object",
          [](const Format &self, const _UnicodeStringVariant &source, Formattable &result) {
            ErrorCode error_code;
            self.parseObject(VARIANT_TO_UNISTR(source), result, error_code);
            if (error_code.isFailure()) {
              throw ICUError(error_code);
            }
          },
          py::arg("source"), py::arg("result"));
  mf.def(
        // [1] MessageFormat::setFormat
        "set_format",
        [](MessageFormat &self, const _UnicodeStringVariant &format_name, const Format &format) {
          ErrorCode error_code;
          self.setFormat(VARIANT_TO_UNISTR(format_name), format, error_code);
          if (error_code.isFailure()) {
            throw ICUError(error_code);
          }
        },
        py::arg("format_name"), py::arg("format_"))
      .def(
          // [2] MessageFormat::setFormat
          "set_format", py::overload_cast<int32_t, const Format &>(&MessageFormat::setFormat), py::arg("format_number"),
          py::arg("format_"));
  mf.def(
      "set_formats",
      [](MessageFormat &self, std::vector<const Format *> &new_formats, int32_t count) {
        if (count == -1) {
          count = static_cast<int32_t>(new_formats.size());
        }
        self.setFormats(new_formats.data(), count);
      },
      py::arg("new_formats"), py::arg("count") = -1);
  mf.def(
      "set_locale",
      [](MessageFormat &self, const _LocaleVariant &locale) { self.setLocale(VARIANT_TO_LOCALE(locale)); },
      py::arg("locale"));
  mf.def("to_pattern", &MessageFormat::toPattern, py::arg("append_to"));
  mf.def("uses_named_arguments", &MessageFormat::usesNamedArguments);
}
