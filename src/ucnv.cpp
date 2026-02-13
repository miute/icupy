#include "ucnv_err.hpp"
#include "uenumptr.hpp"
#include "usetptr.hpp"
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
      [](icupy::UConverterPtr &cnv) {
        ErrorCode error_code;
        auto p = ucnv_clone(cnv, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UConverterPtr>(p);
      },
      py::arg("cnv"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)

  m.def(
      "ucnv_close",
      [](icupy::UConverterPtr &converter) { ucnv_close(converter); },
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
      [](const icupy::UConverterPtr &cnv, const std::u16string &source,
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
      [](icupy::UConverterPtr &converter) {
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
      [](icupy::UConverterPtr &converter, const std::string &display_locale) {
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
      [](const icupy::UConverterPtr &converter) {
        UConverterFromUCallback action = nullptr;
        const void *context = nullptr;
        ucnv_getFromUCallBack(converter, &action, &context);
        if (icupy::UConverterFromUCallbackPtr::is_cpp_function(action)) {
          // C++ (Python) callback function
          auto pair = reinterpret_cast<icupy::FromUCallbackAndContextPair *>(
              const_cast<void *>(context));
          return icupy::UConverterFromUCallbackPtr(pair);
        }
        // Predefined callback function
        return icupy::UConverterFromUCallbackPtr(action, context);
      },
      py::arg("converter"),
      R"doc(
      Get the current From Unicode callback function.

      See Also:
          :func:`ucnv_set_from_ucall_back`
      )doc");

  m.def(
      "ucnv_get_name",
      [](const icupy::UConverterPtr &converter) {
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
      [](const icupy::UConverterPtr &converter) {
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
      [](const icupy::UConverterPtr &converter) {
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
      [](const icupy::UConverterPtr &converter) {
        UConverterToUCallback action = nullptr;
        const void *context = nullptr;
        ucnv_getToUCallBack(converter, &action, &context);
        if (icupy::UConverterToUCallbackPtr::is_cpp_function(action)) {
          // C++ (Python) callback function
          auto pair = reinterpret_cast<icupy::ToUCallbackAndContextPair *>(
              const_cast<void *>(context));
          return icupy::UConverterToUCallbackPtr(pair);
        }
        // Predefined callback function
        return icupy::UConverterToUCallbackPtr(action, context);
      },
      py::arg("converter"),
      R"doc(
      Get the current To Unicode callback function.

      See Also:
          :func:`ucnv_set_to_ucall_back`
      )doc");

  m.def(
      "ucnv_get_type",
      [](const icupy::UConverterPtr &converter) {
        return ucnv_getType(converter);
      },
      py::arg("converter"));

  m.def(
      "ucnv_get_unicode_set",
      [](const icupy::UConverterPtr &cnv, icupy::USetPtr &set_fill_in,
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
      [](const icupy::UConverterPtr &cnv) -> py::bool_ {
        return ucnv_isAmbiguous(cnv);
      },
      py::arg("cnv"));

  m.def(
      "ucnv_is_fixed_width",
      [](const icupy::UConverterPtr &cnv) -> py::bool_ {
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
        return std::make_unique<icupy::UConverterPtr>(p);
      },
      py::arg("converter_name"));

  m.def("ucnv_open_all_names", []() {
    ErrorCode error_code;
    auto p = ucnv_openAllNames(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::make_unique<icupy::UEnumerationPtr>(p);
  });

  m.def(
      "ucnv_open_ccsid",
      [](int32_t codepage, UConverterPlatform platform) {
        ErrorCode error_code;
        auto p = ucnv_openCCSID(codepage, platform, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UConverterPtr>(p);
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
        return std::make_unique<icupy::UConverterPtr>(p);
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
        return std::make_unique<icupy::UEnumerationPtr>(p);
      },
      py::arg("conv_name"), py::arg("standard"));

  m.def(
      "ucnv_reset",
      [](icupy::UConverterPtr &converter) { ucnv_reset(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_reset_from_unicode",
      [](icupy::UConverterPtr &converter) { ucnv_resetFromUnicode(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_reset_to_unicode",
      [](icupy::UConverterPtr &converter) { ucnv_resetToUnicode(converter); },
      py::arg("converter"));

  m.def(
      "ucnv_set_default_name",
      [](std::optional<const std::string> &name) {
        ucnv_setDefaultName(name ? name->data() : nullptr);
      },
      py::arg("name"));

  m.def(
      "ucnv_set_fallback",
      [](icupy::UConverterPtr &cnv, py::bool_ uses_fallback) {
        ucnv_setFallback(cnv, uses_fallback);
      },
      py::arg("cnv"), py::arg("uses_fallback"));

  m.def(
      "ucnv_set_from_ucall_back",
      [](icupy::UConverterPtr &converter,
         icupy::UConverterFromUCallbackPtr &new_action) {
        auto action = new_action.get_native_callback();
        auto new_pair = new_action.context();
        const void *context = nullptr;
        if (new_action.is_cpp_function()) {
          // New C++ (Python) callback function
          context = new_pair;
        } else if (new_pair) {
          // Predefined callback function
          context = new_pair->second;
        }
        UConverterFromUCallback old_action = nullptr;
        const void *old_context = nullptr;
        ErrorCode error_code;
        ucnv_setFromUCallBack(converter, action, context, &old_action,
                              &old_context, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        if (icupy::UConverterFromUCallbackPtr::is_cpp_function(old_action)) {
          // Old C++ (Python) callback function
          auto pair = reinterpret_cast<icupy::FromUCallbackAndContextPair *>(
              const_cast<void *>(old_context));
          return icupy::UConverterFromUCallbackPtr(pair);
        }
        // Predefined callback function
        return icupy::UConverterFromUCallbackPtr(old_action, old_context);
      },
      py::arg("converter"), py::arg("new_action"), R"doc(
      Set the From Unicode callback function used when this ``UConverter``
      founds an illegal or invalid Unicode sequence.

      `new_action` must outlive the ``UConverter`` object.

      See Also:
          :func:`ucnv_get_from_ucall_back`
          :meth:`UnicodeString.extract`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> def from_unicode_cb(
          ...     options: object,
          ...     args: icu.UConverterFromUnicodeArgs,
          ...     code_units: str,
          ...     length: int,
          ...     code_point: int,
          ...     reason: icu.UConverterCallbackReason,
          ...     error_code: icu.ErrorCode,
          ... ) -> None:
          ...     _ = options, length, code_point  # unused
          ...     if reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
          ...         error_code.set(icu.U_ZERO_ERROR)
          ...         source = "".join(f"\\u{ord(c):04X}" for c in code_units)
          ...         icu.ucnv_cb_from_uwrite_bytes(args, source, len(source), 0)
          ...
          >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
          ...     action = icu.UConverterFromUCallback(from_unicode_cb)
          ...     old_action = icu.ucnv_set_from_ucall_back(cnv, action)
          ...     s = icu.UnicodeString("A€B")
          ...     s.extract(cnv)
          ...
          b'A\\u20ACB'
      )doc");

  m.def(
      "ucnv_set_subst_chars",
      [](icupy::UConverterPtr &converter,
         const icupy::CharPtrVariant &sub_chars, int8_t length) {
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
      py::arg("converter"), py::arg("sub_chars"), py::arg("length") = -1);

  m.def(
      "ucnv_set_subst_string",
      [](icupy::UConverterPtr &converter, const std::u16string &s,
         int32_t length) {
        ErrorCode error_code;
        ucnv_setSubstString(converter, s.data(), length, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("converter"), py::arg("s"), py::arg("length") = -1);

  m.def(
      "ucnv_set_to_ucall_back",
      [](icupy::UConverterPtr &converter,
         icupy::UConverterToUCallbackPtr &new_action) {
        auto action = new_action.get_native_callback();
        auto new_pair = new_action.context();
        const void *context = nullptr;
        if (new_action.is_cpp_function()) {
          // New C++ (Python) callback function
          context = new_pair;
        } else if (new_pair) {
          // Predefined callback function
          context = new_pair->second;
        }
        UConverterToUCallback old_action = nullptr;
        const void *old_context = nullptr;
        ErrorCode error_code;
        ucnv_setToUCallBack(converter, action, context, &old_action,
                            &old_context, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        if (icupy::UConverterToUCallbackPtr::is_cpp_function(old_action)) {
          // Old C++ (Python) callback function
          auto pair = reinterpret_cast<icupy::ToUCallbackAndContextPair *>(
              const_cast<void *>(old_context));
          return icupy::UConverterToUCallbackPtr(pair);
        }
        // Predefined callback function
        return icupy::UConverterToUCallbackPtr(old_action, old_context);
      },
      py::arg("converter"), py::arg("new_action"), R"doc(
      Set the To Unicode callback function used when this ``UConverter``
      founds an illegal or invalid codepage sequence.

      `new_action` must outlive the ``UConverter`` object.

      See Also:
          :func:`ucnv_get_to_ucall_back`
          :meth:`UnicodeString.__init__`

      Example:
          >>> from icupy import icu
          >>> from icupy.utils import gc
          >>> def to_unicode_cb(
          ...     options: object,
          ...     args: icu.UConverterToUnicodeArgs,
          ...     code_units: bytes,
          ...     length: int,
          ...     reason: icu.UConverterCallbackReason,
          ...     error_code: icu.ErrorCode,
          ... ) -> None:
          ...     _= options, length  # unused
          ...     if reason in [icu.UCNV_UNASSIGNED, icu.UCNV_ILLEGAL, icu.UCNV_IRREGULAR]:
          ...         error_code.set(icu.U_ZERO_ERROR)
          ...         source = "".join(f"%{b:02X}" for b in code_units)
          ...         icu.ucnv_cb_to_uwrite_uchars(args, source, len(source), 0)
          ...
          >>> with gc(icu.ucnv_open("Shift-JIS"), icu.ucnv_close) as cnv:
          ...     action = icu.UConverterToUCallback(to_unicode_cb)
          ...     old_action = icu.ucnv_set_to_ucall_back(cnv, action)
          ...     src = b"\x61\xeb\x40\x62"  # 0xeb 0x40: UCNV_UNASSIGNED
          ...     s = icu.UnicodeString(src, -1, cnv)
          ...     str(s)
          ...
          'a%EB%40b'
      )doc");

  m.def(
      "ucnv_uses_fallback",
      [](const icupy::UConverterPtr &cnv) -> py::bool_ {
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
