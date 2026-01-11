#include "ucnv_err.hpp"
#include "uenumptr.hpp"
#include "usetptr.hpp"
#include "voidptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/stl.h>

using namespace icu;

void init_ucnv(py::module &m) {
  //
  // enum UConverterPlatform
  //
  py::enum_<UConverterPlatform>(
      m, "UConverterPlatform", py::arithmetic(),
      "Enum for specifying which platform a converter ID refers to.\n\n"
      "The use of platform/CCSID is not recommended. See *ucnv_open_ccsid()*.")
      .value("UCNV_UNKNOWN", UCNV_UNKNOWN)
      .value("UCNV_IBM", UCNV_IBM)
      .export_values();

  //
  // enum UConverterType
  //
  py::enum_<UConverterType>(m, "UConverterType", py::arithmetic(),
                            "Enum for specifying basic types of converters.")
      .value("UCNV_UNSUPPORTED_CONVERTER", UCNV_UNSUPPORTED_CONVERTER)
      .value("UCNV_SBCS", UCNV_SBCS)
      .value("UCNV_DBCS", UCNV_DBCS)
      .value("UCNV_MBCS", UCNV_MBCS)
      .value("UCNV_LATIN_1", UCNV_LATIN_1)
      .value("UCNV_UTF8", UCNV_UTF8)
      .value("UCNV_UTF16_BIG_ENDIAN", UCNV_UTF16_BigEndian)
      .value("UCNV_UTF16_LITTLE_ENDIAN", UCNV_UTF16_LittleEndian)
      .value("UCNV_UTF32_BIG_ENDIAN", UCNV_UTF32_BigEndian)
      .value("UCNV_UTF32_LITTLE_ENDIAN", UCNV_UTF32_LittleEndian)
      .value("UCNV_EBCDIC_STATEFUL", UCNV_EBCDIC_STATEFUL)
      .value("UCNV_ISO_2022", UCNV_ISO_2022)
      .value("UCNV_LMBCS_1", UCNV_LMBCS_1)
      .value("UCNV_LMBCS_2", UCNV_LMBCS_2)
      .value("UCNV_LMBCS_3", UCNV_LMBCS_3)
      .value("UCNV_LMBCS_4", UCNV_LMBCS_4)
      .value("UCNV_LMBCS_5", UCNV_LMBCS_5)
      .value("UCNV_LMBCS_6", UCNV_LMBCS_6)
      .value("UCNV_LMBCS_8", UCNV_LMBCS_8)
      .value("UCNV_LMBCS_11", UCNV_LMBCS_11)
      .value("UCNV_LMBCS_16", UCNV_LMBCS_16)
      .value("UCNV_LMBCS_17", UCNV_LMBCS_17)
      .value("UCNV_LMBCS_18", UCNV_LMBCS_18)
      .value("UCNV_LMBCS_19", UCNV_LMBCS_19)
      .value("UCNV_LMBCS_LAST", UCNV_LMBCS_LAST)
      .value("UCNV_HZ", UCNV_HZ)
      .value("UCNV_SCSU", UCNV_SCSU)
      .value("UCNV_ISCII", UCNV_ISCII)
      .value("UCNV_US_ASCII", UCNV_US_ASCII)
      .value("UCNV_UTF7", UCNV_UTF7)
      .value("UCNV_BOCU1", UCNV_BOCU1)
      .value("UCNV_UTF16", UCNV_UTF16)
      .value("UCNV_UTF32", UCNV_UTF32)
      .value("UCNV_CESU8", UCNV_CESU8)
      .value("UCNV_IMAP_MAILBOX", UCNV_IMAP_MAILBOX)
      .value("UCNV_COMPOUND_TEXT", UCNV_COMPOUND_TEXT)
      .value("UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES",
             UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES)
      .export_values();

  //
  // enum UConverterUnicodeSet
  //
  py::enum_<UConverterUnicodeSet>(m, "UConverterUnicodeSet", py::arithmetic(),
                                  "Selectors for Unicode sets that can be "
                                  "returned by *ucnv_get_unicode_set()*.")
      .value("UCNV_ROUNDTRIP_SET", UCNV_ROUNDTRIP_SET,
             "Select the set of roundtrippable Unicode code points.")
      .value("UCNV_ROUNDTRIP_AND_FALLBACK_SET", UCNV_ROUNDTRIP_AND_FALLBACK_SET,
             "Select the set of Unicode code points with roundtrip or fallback "
             "mappings.")
      .value("UCNV_SET_COUNT", UCNV_SET_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, "
             "see ICU ticket #12420.")
      .export_values();

  //
  // Functions
  //
#if (U_ICU_VERSION_MAJOR_NUM >= 71)
  m.def(
      "ucnv_clone",
      [](_UConverterPtr &cnv) {
        ErrorCode error_code;
        auto p = ucnv_clone(cnv, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("cnv"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)

  m.def(
      "ucnv_close", [](_UConverterPtr &converter) { ucnv_close(converter); },
      py::arg("converter"));

  m.def("ucnv_compare_names", &ucnv_compareNames, py::arg("name1"),
        py::arg("name2"));

  m.def(
      "ucnv_count_aliases",
      [](const std::string &alias) {
        ErrorCode error_code;
        auto result = ucnv_countAliases(alias.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"));

  m.def("ucnv_count_available", &ucnv_countAvailable);

  m.def("ucnv_count_standards", &ucnv_countStandards);

  m.def(
      "ucnv_detect_unicode_signature",
      [](const icupy::CharPtrVariant &source,
         int32_t source_length) -> std::optional<const std::string> {
        const auto source_value = icupy::CharPtr(source);
        if (source_length == -1) {
          source_length = static_cast<int32_t>(source_value.size());
        }
        ErrorCode error_code;
        auto result = ucnv_detectUnicodeSignature(source_value, source_length,
                                                  nullptr, error_code);
        if (result == nullptr) {
          return std::nullopt;
        }
        return result;
      },
      py::arg("source"), py::arg("source_length") = -1);

  m.def(
      "ucnv_fix_file_separator",
      [](const _UConverterPtr &cnv, const std::u16string &source,
         int32_t source_length) {
        auto normalized_source_length = source_length;
        if (normalized_source_length == -1) {
          normalized_source_length = static_cast<int32_t>(source.size());
        }
        std::u16string result(source, 0, std::max(normalized_source_length, 0));
        ucnv_fixFileSeparator(cnv, result.data(), result.size());
        return result;
      },
      py::arg("cnv"), py::arg("source"), py::arg("source_len") = -1);

  m.def("ucnv_flush_cache", &ucnv_flushCache);

  m.def(
      "ucnv_get_alias",
      [](const std::string &alias, int16_t n) {
        ErrorCode error_code;
        auto result = ucnv_getAlias(alias.data(), n, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"), py::arg("n"));

  m.def(
      "ucnv_get_aliases",
      [](const std::string &alias) {
        auto alias_data = alias.data();
        ErrorCode error_code;
        const auto count = ucnv_countAliases(alias_data, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        std::vector<const char *> result(count, nullptr);
        error_code.reset();
        ucnv_getAliases(alias_data, result.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"));

  m.def("ucnv_get_available_name", &ucnv_getAvailableName, py::arg("n"));

  m.def(
      "ucnv_get_canonical_name",
      [](const std::string &alias,
         const std::string &standard) -> std::optional<std::string> {
        ErrorCode error_code;
        auto result =
            ucnv_getCanonicalName(alias.data(), standard.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"), py::arg("standard"));

  m.def(
      "ucnv_get_ccsid",
      [](_UConverterPtr &converter) {
        ErrorCode error_code;
        auto result = ucnv_getCCSID(converter, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"));

  m.def("ucnv_get_default_name", &ucnv_getDefaultName);

  m.def(
      "ucnv_get_display_name",
      [](_UConverterPtr &converter, const std::string &display_locale) {
        auto display_locale_data = display_locale.data();
        ErrorCode error_code;
        const auto display_name_capacity = ucnv_getDisplayName(
            converter, display_locale_data, nullptr, 0, error_code);
        std::u16string result(display_name_capacity, u'\0');
        error_code.reset();
        ucnv_getDisplayName(converter, display_locale_data, result.data(),
                            display_name_capacity, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"), py::arg("display_locale"));

  m.def(
      "ucnv_get_from_ucall_back",
      [](const _UConverterPtr &converter) {
        UConverterFromUCallback callback = nullptr;
        const void *context = nullptr;
        ucnv_getFromUCallBack(converter, &callback, &context);
        if (callback == _UConverterFromUCallbackPtr::callback) {
          // Python callback function and callback data
          auto python_context = reinterpret_cast<icupy::ConstVoidPtr *>(
              const_cast<void *>(context));
          auto action = _UConverterFromUCallbackPtr(python_context->action());
          return py::make_tuple(action, python_context);
        }
        // C callback function and callback data
        return py::make_tuple(_UConverterFromUCallbackPtr(callback),
                              icupy::ConstVoidPtr(context));
      },
      py::return_value_policy::reference, py::arg("converter"));

  m.def(
      "ucnv_get_name",
      [](const _UConverterPtr &converter) {
        ErrorCode error_code;
        auto result = ucnv_getName(converter, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"));

  m.def(
      "ucnv_get_platform",
      [](const _UConverterPtr &converter) {
        ErrorCode error_code;
        auto result = ucnv_getPlatform(converter, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"));

  m.def(
      "ucnv_get_standard",
      [](uint16_t n) {
        ErrorCode error_code;
        auto result = ucnv_getStandard(n, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("n"));

  m.def(
      "ucnv_get_standard_name",
      [](const std::string &name,
         const std::string &standard) -> std::optional<std::string> {
        ErrorCode error_code;
        auto result =
            ucnv_getStandardName(name.data(), standard.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("name"), py::arg("standard"));

  m.def(
      "ucnv_get_subst_chars",
      [](const _UConverterPtr &converter) {
        char sub_chars[UCNV_ERROR_BUFFER_LENGTH];
        int8_t length = sizeof(sub_chars);
        ErrorCode error_code;
        ucnv_getSubstChars(converter, sub_chars, &length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return py::bytes(sub_chars, length);
      },
      py::arg("converter"));

  m.def(
      "ucnv_get_to_ucall_back",
      [](const _UConverterPtr &converter) {
        UConverterToUCallback callback = nullptr;
        const void *context = nullptr;
        ucnv_getToUCallBack(converter, &callback, &context);
        if (callback == _UConverterToUCallbackPtr::callback) {
          // Python callback function and callback data
          auto python_context = reinterpret_cast<icupy::ConstVoidPtr *>(
              const_cast<void *>(context));
          auto action = _UConverterToUCallbackPtr(python_context->action());
          return py::make_tuple(action, python_context);
        }
        // C callback function and callback data
        return py::make_tuple(_UConverterToUCallbackPtr(callback),
                              icupy::ConstVoidPtr(context));
      },
      py::return_value_policy::reference, py::arg("converter"));

  m.def(
      "ucnv_get_type",
      [](const _UConverterPtr &converter) { return ucnv_getType(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_get_unicode_set",
      [](const _UConverterPtr &cnv, _USetPtr &set_fill_in,
         UConverterUnicodeSet which_set) {
        ErrorCode error_code;
        ucnv_getUnicodeSet(cnv, set_fill_in, which_set, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("cnv"), py::arg("set_fill_in"), py::arg("which_set"));

  m.def(
      "ucnv_is_ambiguous",
      [](const _UConverterPtr &cnv) -> py::bool_ {
        return ucnv_isAmbiguous(cnv);
      },
      py::arg("cnv"));

  m.def(
      "ucnv_is_fixed_width",
      [](const _UConverterPtr &cnv) -> py::bool_ {
        ErrorCode error_code;
        auto result = ucnv_isFixedWidth(cnv, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("cnv"));

  m.def(
      "ucnv_open",
      [](const std::string &converter_name) {
        ErrorCode error_code;
        auto p = ucnv_open(converter_name.data(), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("converter_name"));

  m.def("ucnv_open_all_names", []() {
    ErrorCode error_code;
    auto p = ucnv_openAllNames(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::make_unique<_UEnumerationPtr>(p);
  });

  m.def(
      "ucnv_open_ccsid",
      [](int32_t codepage, UConverterPlatform platform) {
        ErrorCode error_code;
        auto p = ucnv_openCCSID(codepage, platform, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("codepage"), py::arg("platform"));

  m.def(
      "ucnv_open_package",
      [](const std::string &package_name, const std::string &converter_name) {
        ErrorCode error_code;
        auto p = ucnv_openPackage(package_name.data(), converter_name.data(),
                                  error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("package_name"), py::arg("converter_name"));

  m.def(
      "ucnv_open_standard_names",
      [](const std::string &conv_name, const std::string &standard) {
        ErrorCode error_code;
        auto p = ucnv_openStandardNames(conv_name.data(), standard.data(),
                                        error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p);
      },
      py::arg("conv_name"), py::arg("standard"));

  m.def(
      "ucnv_reset", [](_UConverterPtr &converter) { ucnv_reset(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_reset_from_unicode",
      [](_UConverterPtr &converter) { ucnv_resetFromUnicode(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_reset_to_unicode",
      [](_UConverterPtr &converter) { ucnv_resetToUnicode(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_set_default_name",
      [](std::optional<const std::string> &name) {
        ucnv_setDefaultName(name ? name->data() : nullptr);
      },
      py::arg("name"));

  m.def(
      "ucnv_set_fallback",
      [](_UConverterPtr &cnv, py::bool_ uses_fallback) {
        ucnv_setFallback(cnv, uses_fallback);
      },
      py::arg("cnv"), py::arg("uses_fallback"));

  m.def(
      "ucnv_set_from_ucall_back",
      [](_UConverterPtr &converter, _UConverterFromUCallbackPtr &new_action,
         icupy::ConstVoidPtr &new_context) {
        auto callback = new_action.get_if<UConverterFromUCallback>();
        const void *context = nullptr;
        if (callback == nullptr) {
          // New Python callback function and callback data
          callback = new_action.callback;
          new_context.set_action(new_action.get<py::function>());
          context = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data
          context = new_context.data();
        }

        UConverterFromUCallback old_action = nullptr;
        const void *old_context = nullptr;
        ErrorCode error_code;
        ucnv_setFromUCallBack(converter, callback, context, &old_action,
                              &old_context, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        if (old_action == new_action.callback) {
          // Old Python callback function and callback data
          auto python_context = reinterpret_cast<icupy::ConstVoidPtr *>(
              const_cast<void *>(old_context));
          auto action = _UConverterFromUCallbackPtr(python_context->action());
          return py::make_tuple(action, python_context);
        }
        // Old C callback function and callback data
        return py::make_tuple(_UConverterFromUCallbackPtr(old_action),
                              icupy::ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(),
      py::keep_alive<3, 1>(), py::arg("converter"), py::arg("new_action"),
      py::arg("new_context"));

  m.def(
      "ucnv_set_subst_chars",
      [](_UConverterPtr &converter, const icupy::CharPtrVariant &sub_chars,
         int8_t length) {
        const auto sub_chars_value = icupy::CharPtr(sub_chars);
        if (length == -1) {
          length = static_cast<int8_t>(sub_chars_value.size());
        }
        ErrorCode error_code;
        ucnv_setSubstChars(converter, sub_chars_value, length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("converter"), py::arg("sub_chars"), py::arg("len_") = -1);

  m.def(
      "ucnv_set_subst_string",
      [](_UConverterPtr &converter, const std::u16string &s, int32_t length) {
        ErrorCode error_code;
        ucnv_setSubstString(converter, s.data(), length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("converter"), py::arg("s"), py::arg("length") = -1);

  m.def(
      "ucnv_set_to_ucall_back",
      [](_UConverterPtr &converter, _UConverterToUCallbackPtr &new_action,
         icupy::ConstVoidPtr &new_context) {
        auto callback = new_action.get_if<UConverterToUCallback>();
        const void *context = nullptr;
        if (callback == nullptr) {
          // New Python callback function and callback data
          callback = new_action.callback;
          new_context.set_action(new_action.get<py::function>());
          context = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data
          context = new_context.data();
        }

        UConverterToUCallback old_action = nullptr;
        const void *old_context = nullptr;
        ErrorCode error_code;
        ucnv_setToUCallBack(converter, callback, context, &old_action,
                            &old_context, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        if (old_action == new_action.callback) {
          // Old Python callback function and callback data
          auto python_context = reinterpret_cast<icupy::ConstVoidPtr *>(
              const_cast<void *>(old_context));
          auto action = _UConverterToUCallbackPtr(python_context->action());
          return py::make_tuple(action, python_context);
        }
        // Old C callback function and callback data
        return py::make_tuple(_UConverterToUCallbackPtr(old_action),
                              icupy::ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(),
      py::keep_alive<3, 1>(), py::arg("converter"), py::arg("new_action"),
      py::arg("new_context"));

  m.def(
      "ucnv_uses_fallback",
      [](const _UConverterPtr &cnv) -> py::bool_ {
        return ucnv_usesFallback(cnv);
      },
      py::arg("cnv"));

  m.attr("UCNV_LOCALE_OPTION_STRING") = UCNV_LOCALE_OPTION_STRING;
  m.attr("UCNV_OPTION_SEP_CHAR") = UCNV_OPTION_SEP_CHAR;
  m.attr("UCNV_OPTION_SEP_STRING") = UCNV_OPTION_SEP_STRING;
  m.attr("UCNV_SI") = UCNV_SI;
  m.attr("UCNV_SO") = UCNV_SO;
  m.attr("UCNV_SWAP_LFNL_OPTION_STRING") = UCNV_SWAP_LFNL_OPTION_STRING;
  m.attr("UCNV_VALUE_SEP_CHAR") = UCNV_VALUE_SEP_CHAR;
  m.attr("UCNV_VALUE_SEP_STRING") = UCNV_VALUE_SEP_STRING;
  m.attr("UCNV_VERSION_OPTION_STRING") = UCNV_VERSION_OPTION_STRING;
}
