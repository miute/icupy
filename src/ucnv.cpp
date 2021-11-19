#include "ucnv_err.hpp"
#include "uenumptr.hpp"
#include "usetptr.hpp"
#include "voidptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/stl.h>

void init_ucnv(py::module &m) {
  py::enum_<UConverterPlatform>(m, "UConverterPlatform", py::arithmetic(),
                                "Enum for specifying which platform a converter ID refers to.\n\n"
                                "The use of platform/CCSID is not recommended. See *ucnv_open_ccsid()*.")
      .value("UCNV_UNKNOWN", UCNV_UNKNOWN)
      .value("UCNV_IBM", UCNV_IBM)
      .export_values();

  py::enum_<UConverterType>(m, "UConverterType", py::arithmetic(), "Enum for specifying basic types of converters.")
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
      .value("UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES", UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES)
      .export_values();

  py::enum_<UConverterUnicodeSet>(m, "UConverterUnicodeSet", py::arithmetic(),
                                  "Selectors for Unicode sets that can be returned by *ucnv_get_unicode_set()*.")
      .value("UCNV_ROUNDTRIP_SET", UCNV_ROUNDTRIP_SET, "Select the set of roundtrippable Unicode code points.")
      .value("UCNV_ROUNDTRIP_AND_FALLBACK_SET", UCNV_ROUNDTRIP_AND_FALLBACK_SET,
             "Select the set of Unicode code points with roundtrip or fallback mappings.")
#ifndef U_HIDE_DEPRECATED_API
      .value("UCNV_SET_COUNT", UCNV_SET_COUNT,
             "**Deprecated:** ICU 58 The numeric value may change over time, see ICU ticket #12420.")
#endif // U_HIDE_DEPRECATED_API
      .export_values();

  m.def(
      "ucnv_close", [](_UConverterPtr &converter) { ucnv_close(converter); }, py::arg("converter"));
  m.def("ucnv_compare_names", &ucnv_compareNames, py::arg("name1"), py::arg("name2"));

  m.def(
      "ucnv_count_aliases",
      [](const char *alias) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_countAliases(alias, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"));
  m.def("ucnv_count_available", &ucnv_countAvailable);
  m.def("ucnv_count_standards", &ucnv_countStandards);
  m.def(
      "ucnv_detect_unicode_signature",
      [](const char *source, int32_t source_length) -> std::optional<const char *> {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_detectUnicodeSignature(source, source_length, nullptr, &error_code);
        if (result == nullptr) {
          return std::nullopt;
        }
        return result;
      },
      py::arg("source"), py::arg("source_length"));
  m.def(
      "ucnv_fix_file_separator",
      [](const _UConverterPtr &cnv, const UChar *source, int32_t source_len) {
        std::u16string result(source, source_len);
        ucnv_fixFileSeparator(cnv, result.data(), static_cast<int32_t>(result.size()));
        return result;
      },
      py::arg("cnv"), py::arg("source"), py::arg("source_len"));
  m.def("ucnv_flush_cache", &ucnv_flushCache);

  m.def(
      "ucnv_get_alias",
      [](const char *alias, int16_t n) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getAlias(alias, n, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"), py::arg("n"));
  m.def(
      "ucnv_get_aliases",
      [](const char *alias) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto count = ucnv_countAliases(alias, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        std::vector<const char *> result(count);
        error_code = U_ZERO_ERROR;
        ucnv_getAliases(alias, result.data(), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"));
  m.def("ucnv_get_available_name", &ucnv_getAvailableName, py::arg("n"));
  m.def(
      "ucnv_get_canonical_name",
      [](const char *alias, const char *standard) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getCanonicalName(alias, standard, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("alias"), py::arg("standard"));
  m.def(
      "ucnv_get_ccsid",
      [](_UConverterPtr &converter) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getCCSID(converter, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"));
  m.def("ucnv_get_default_name", &ucnv_getDefaultName);
  m.def(
      "ucnv_get_display_name",
      [](_UConverterPtr &converter, const char *display_locale) {
        UErrorCode error_code = U_ZERO_ERROR;
        const auto length = ucnv_getDisplayName(converter, display_locale, nullptr, 0, &error_code);
        std::u16string result(length, u'\0');
        error_code = U_ZERO_ERROR;
        ucnv_getDisplayName(converter, display_locale, result.data(), static_cast<int32_t>(result.size()), &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"), py::arg("display_locale"));
  m.def(
      "ucnv_get_from_u_call_back",
      [](const _UConverterPtr &converter) {
        PyErr_WarnEx(nullptr,
                     "ucnv_get_from_u_call_back() is deprecated and will be removed in a future release. "
                     "Use ucnv_get_from_ucall_back() instead.",
                     1);
        UConverterFromUCallback action;
        const void *context;
        ucnv_getFromUCallBack(converter, &action, &context);
        if (action == _UConverterFromUCallbackPtr::callback) {
          // Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
          auto result1 = _UConverterFromUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // C callback function and callback data
        return py::make_tuple(_UConverterFromUCallbackPtr(action), _ConstVoidPtr(context));
      },
      py::return_value_policy::reference, py::arg("converter"));
  m.def(
      "ucnv_get_from_ucall_back",
      [](const _UConverterPtr &converter) {
        UConverterFromUCallback action;
        const void *context;
        ucnv_getFromUCallBack(converter, &action, &context);
        if (action == _UConverterFromUCallbackPtr::callback) {
          // Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
          auto result1 = _UConverterFromUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // C callback function and callback data
        return py::make_tuple(_UConverterFromUCallbackPtr(action), _ConstVoidPtr(context));
      },
      py::return_value_policy::reference, py::arg("converter"));
  m.def(
      "ucnv_get_name",
      [](const _UConverterPtr &converter) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getName(converter, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"));
  m.def(
      "ucnv_get_platform",
      [](const _UConverterPtr &converter) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getPlatform(converter, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("converter"));
  m.def(
      "ucnv_get_standard",
      [](uint16_t n) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getStandard(n, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("n"));
  m.def(
      "ucnv_get_standard_name",
      [](const char *name, const char *standard) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_getStandardName(name, standard, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("name"), py::arg("standard"));
  m.def(
      "ucnv_get_subst_chars",
      [](const _UConverterPtr &converter) {
        UErrorCode error_code = U_ZERO_ERROR;
        char sub_chars[UCNV_ERROR_BUFFER_LENGTH];
        int8_t len = sizeof(sub_chars);
        ucnv_getSubstChars(converter, sub_chars, &len, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return py::bytes(sub_chars, len);
      },
      py::arg("converter"));
  m.def(
      "ucnv_get_to_u_call_back",
      [](const _UConverterPtr &converter) {
        PyErr_WarnEx(nullptr,
                     "ucnv_get_to_u_call_back() is deprecated and will be removed in a future release. "
                     "Use ucnv_get_to_ucall_back() instead.",
                     1);
        UConverterToUCallback action;
        const void *context;
        ucnv_getToUCallBack(converter, &action, &context);
        if (action == _UConverterToUCallbackPtr::callback) {
          // Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
          auto result1 = _UConverterToUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // C callback function and callback data
        return py::make_tuple(_UConverterToUCallbackPtr(action), _ConstVoidPtr(context));
      },
      py::return_value_policy::reference, py::arg("converter"));
  m.def(
      "ucnv_get_to_ucall_back",
      [](const _UConverterPtr &converter) {
        UConverterToUCallback action;
        const void *context;
        ucnv_getToUCallBack(converter, &action, &context);
        if (action == _UConverterToUCallbackPtr::callback) {
          // Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
          auto result1 = _UConverterToUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // C callback function and callback data
        return py::make_tuple(_UConverterToUCallbackPtr(action), _ConstVoidPtr(context));
      },
      py::return_value_policy::reference, py::arg("converter"));
  m.def(
      "ucnv_get_type", [](const _UConverterPtr &converter) { return ucnv_getType(converter); }, py::arg("converter"));
  m.def(
      "ucnv_get_unicode_set",
      [](const _UConverterPtr &cnv, _USetPtr &set_fill_in, UConverterUnicodeSet which_set) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_getUnicodeSet(cnv, set_fill_in, which_set, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("cnv"), py::arg("set_fill_in"), py::arg("which_set"));
  m.def(
      "ucnv_is_ambiguous", [](const _UConverterPtr &cnv) { return ucnv_isAmbiguous(cnv); }, py::arg("cnv"));
  m.def(
      "ucnv_is_fixed_width",
      [](const _UConverterPtr &cnv) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = ucnv_isFixedWidth(cnv, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      },
      py::arg("cnv"));
  m.def(
      "ucnv_open",
      [](const char *converter_name) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ucnv_open(converter_name, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("converter_name"));
  m.def("ucnv_open_all_names", []() {
    UErrorCode error_code = U_ZERO_ERROR;
    auto p = ucnv_openAllNames(&error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_UEnumerationPtr>(p);
  });
  m.def(
      "ucnv_open_ccsid",
      [](int32_t codepage, UConverterPlatform platform) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ucnv_openCCSID(codepage, platform, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("codepage"), py::arg("platform"));
  m.def(
      "ucnv_open_package",
      [](const char *package_name, const char *converter_name) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ucnv_openPackage(package_name, converter_name, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UConverterPtr>(p);
      },
      py::arg("package_name"), py::arg("converter_name"));
  m.def(
      "ucnv_open_standard_names",
      [](const char *conv_name, const char *standard) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = ucnv_openStandardNames(conv_name, standard, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UEnumerationPtr>(p);
      },
      py::arg("conv_name"), py::arg("standard"));
  m.def(
      "ucnv_reset", [](_UConverterPtr &converter) { ucnv_reset(converter); }, py::arg("converter"));
  m.def(
      "ucnv_reset_from_unicode", [](_UConverterPtr &converter) { ucnv_resetFromUnicode(converter); },
      py::arg("converter"));
  m.def(
      "ucnv_reset_to_unicode", [](_UConverterPtr &converter) { ucnv_resetToUnicode(converter); }, py::arg("converter"));

  m.def("ucnv_set_default_name", &ucnv_setDefaultName, py::arg("name"));
  m.def(
      "ucnv_set_fallback", [](_UConverterPtr &cnv, UBool uses_fallback) { ucnv_setFallback(cnv, uses_fallback); },
      py::arg("cnv"), py::arg("uses_fallback"));
  m.def(
      "ucnv_set_from_u_call_back",
      [](_UConverterPtr &converter, _UConverterFromUCallbackPtr &new_action, _ConstVoidPtr &new_context) {
        PyErr_WarnEx(nullptr,
                     "ucnv_set_from_u_call_back() is deprecated and will be removed in a future release. "
                     "Use ucnv_set_from_ucall_back() instead.",
                     1);
        UConverterFromUCallback fp = new_action.get_if<UConverterFromUCallback>();
        const void *cp = nullptr;
        if (fp == nullptr) {
          // New Python callback function and callback data
          fp = new_action.callback;
          new_context.set_action(new_action.get<py::function>());
          cp = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data
          cp = new_context.to_c_str();
        }

        UConverterFromUCallback old_action;
        const void *old_context;
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_setFromUCallBack(converter, fp, cp, &old_action, &old_context, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        if (old_action == new_action.callback) {
          // Old Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(old_context));
          auto result1 = _UConverterFromUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // Old C callback function and callback data
        return py::make_tuple(_UConverterFromUCallbackPtr(old_action), _ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(), py::keep_alive<3, 1>(), py::arg("converter"),
      py::arg("new_action"), py::arg("new_context"));
  m.def(
      "ucnv_set_from_ucall_back",
      [](_UConverterPtr &converter, _UConverterFromUCallbackPtr &new_action, _ConstVoidPtr &new_context) {
        UConverterFromUCallback fp = new_action.get_if<UConverterFromUCallback>();
        const void *cp = nullptr;
        if (fp == nullptr) {
          // New Python callback function and callback data
          fp = new_action.callback;
          new_context.set_action(new_action.get<py::function>());
          cp = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data
          cp = new_context.to_c_str();
        }

        UConverterFromUCallback old_action;
        const void *old_context;
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_setFromUCallBack(converter, fp, cp, &old_action, &old_context, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        if (old_action == new_action.callback) {
          // Old Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(old_context));
          auto result1 = _UConverterFromUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // Old C callback function and callback data
        return py::make_tuple(_UConverterFromUCallbackPtr(old_action), _ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(), py::keep_alive<3, 1>(), py::arg("converter"),
      py::arg("new_action"), py::arg("new_context"));

  m.def(
      "ucnv_set_subst_chars",
      [](_UConverterPtr &converter, const char *sub_chars, int8_t len) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_setSubstChars(converter, sub_chars, len, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("converter"), py::arg("sub_chars"), py::arg("len_"));
  m.def(
      "ucnv_set_subst_string",
      [](_UConverterPtr &converter, const UChar *s, int32_t length) {
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_setSubstString(converter, s, length, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("converter"), py::arg("s"), py::arg("length"));
  m.def(
      "ucnv_set_to_u_call_back",
      [](_UConverterPtr &converter, _UConverterToUCallbackPtr &new_action, _ConstVoidPtr &new_context) {
        PyErr_WarnEx(nullptr,
                     "ucnv_set_to_u_call_back() is deprecated and will be removed in a future release. "
                     "Use ucnv_set_to_ucall_back() instead.",
                     1);
        UConverterToUCallback fp = new_action.get_if<UConverterToUCallback>();
        const void *cp = nullptr;
        if (fp == nullptr) {
          // New Python callback function and callback data
          fp = new_action.callback;
          new_context.set_action(new_action.get<py::function>());
          cp = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data
          cp = new_context.to_c_str();
        }

        UConverterToUCallback old_action;
        const void *old_context;
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_setToUCallBack(converter, fp, cp, &old_action, &old_context, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        if (old_action == new_action.callback) {
          // Old Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(old_context));
          auto result1 = _UConverterToUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // Old C callback function and callback data
        return py::make_tuple(_UConverterToUCallbackPtr(old_action), _ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(), py::keep_alive<3, 1>(), py::arg("converter"),
      py::arg("new_action"), py::arg("new_context"));
  m.def(
      "ucnv_set_to_ucall_back",
      [](_UConverterPtr &converter, _UConverterToUCallbackPtr &new_action, _ConstVoidPtr &new_context) {
        UConverterToUCallback fp = new_action.get_if<UConverterToUCallback>();
        const void *cp = nullptr;
        if (fp == nullptr) {
          // New Python callback function and callback data
          fp = new_action.callback;
          new_context.set_action(new_action.get<py::function>());
          cp = &new_context;
        } else if (new_context.has_value()) {
          // New C callback data
          cp = new_context.to_c_str();
        }

        UConverterToUCallback old_action;
        const void *old_context;
        UErrorCode error_code = U_ZERO_ERROR;
        ucnv_setToUCallBack(converter, fp, cp, &old_action, &old_context, &error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        if (old_action == new_action.callback) {
          // Old Python callback function and callback data
          auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(old_context));
          auto result1 = _UConverterToUCallbackPtr(result2->get_action());
          return py::make_tuple(result1, result2);
        }
        // Old C callback function and callback data
        return py::make_tuple(_UConverterToUCallbackPtr(old_action), _ConstVoidPtr(old_context));
      },
      py::return_value_policy::reference, py::keep_alive<2, 1>(), py::keep_alive<3, 1>(), py::arg("converter"),
      py::arg("new_action"), py::arg("new_context"));

  m.def(
      "ucnv_uses_fallback", [](const _UConverterPtr &cnv) { return ucnv_usesFallback(cnv); }, py::arg("cnv"));

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
