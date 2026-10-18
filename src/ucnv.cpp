#include "ucnv_err.hpp"
#include "uenumptr.hpp"
#include "usetptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/native_enum.h>
#include <pybind11/stl.h>

using namespace icu;

void init_ucnv(py::module &m) {
  //
  // enum UConverterPlatform
  //
  py::native_enum<UConverterPlatform>(m, "UConverterPlatform", "enum.IntEnum",
                                      R"doc(
Enum for specifying which platform a converter ID refers to.

.. important::

   The use of platform/CCSID is not recommended. See :func:`ucnv_open_ccsid`.

.. seealso::

   :func:`ucnv_get_ccsid`
   :func:`ucnv_get_platform`
   :func:`ucnv_open_ccsid`
      )doc")
      .value("UCNV_UNKNOWN", UCNV_UNKNOWN, "")
      .value("UCNV_IBM", UCNV_IBM, "")
      .export_values()
      .finalize();

  //
  // enum UConverterType
  //
  py::native_enum<UConverterType>(m, "UConverterType", "enum.IntEnum", R"doc(
Enum for specifying basic types of converters.

.. seealso::

   :func:`ucnv_get_type`
      )doc")
      .value("UCNV_UNSUPPORTED_CONVERTER", UCNV_UNSUPPORTED_CONVERTER, "")
      .value("UCNV_SBCS", UCNV_SBCS, "")
      .value("UCNV_DBCS", UCNV_DBCS, "")
      .value("UCNV_MBCS", UCNV_MBCS, "")
      .value("UCNV_LATIN_1", UCNV_LATIN_1, "")
      .value("UCNV_UTF8", UCNV_UTF8, "")
      .value("UCNV_UTF16_BIG_ENDIAN", UCNV_UTF16_BigEndian, "")
      .value("UCNV_UTF16_LITTLE_ENDIAN", UCNV_UTF16_LittleEndian, "")
      .value("UCNV_UTF32_BIG_ENDIAN", UCNV_UTF32_BigEndian, "")
      .value("UCNV_UTF32_LITTLE_ENDIAN", UCNV_UTF32_LittleEndian, "")
      .value("UCNV_EBCDIC_STATEFUL", UCNV_EBCDIC_STATEFUL, "")
      .value("UCNV_ISO_2022", UCNV_ISO_2022, "")
      .value("UCNV_LMBCS_1", UCNV_LMBCS_1, "")
      .value("UCNV_LMBCS_2", UCNV_LMBCS_2, "")
      .value("UCNV_LMBCS_3", UCNV_LMBCS_3, "")
      .value("UCNV_LMBCS_4", UCNV_LMBCS_4, "")
      .value("UCNV_LMBCS_5", UCNV_LMBCS_5, "")
      .value("UCNV_LMBCS_6", UCNV_LMBCS_6, "")
      .value("UCNV_LMBCS_8", UCNV_LMBCS_8, "")
      .value("UCNV_LMBCS_11", UCNV_LMBCS_11, "")
      .value("UCNV_LMBCS_16", UCNV_LMBCS_16, "")
      .value("UCNV_LMBCS_17", UCNV_LMBCS_17, "")
      .value("UCNV_LMBCS_18", UCNV_LMBCS_18, "")
      .value("UCNV_LMBCS_19", UCNV_LMBCS_19, "")
      .value("UCNV_LMBCS_LAST", UCNV_LMBCS_LAST, "")
      .value("UCNV_HZ", UCNV_HZ, "")
      .value("UCNV_SCSU", UCNV_SCSU, "")
      .value("UCNV_ISCII", UCNV_ISCII, "")
      .value("UCNV_US_ASCII", UCNV_US_ASCII, "")
      .value("UCNV_UTF7", UCNV_UTF7, "")
      .value("UCNV_BOCU1", UCNV_BOCU1, "")
      .value("UCNV_UTF16", UCNV_UTF16, "")
      .value("UCNV_UTF32", UCNV_UTF32, "")
      .value("UCNV_CESU8", UCNV_CESU8, "")
      .value("UCNV_IMAP_MAILBOX", UCNV_IMAP_MAILBOX, "")
      .value("UCNV_COMPOUND_TEXT", UCNV_COMPOUND_TEXT, "")
      .value("UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES",
             UCNV_NUMBER_OF_SUPPORTED_CONVERTER_TYPES, "")
      .export_values()
      .finalize();

  //
  // enum UConverterUnicodeSet
  //
  py::native_enum<UConverterUnicodeSet>(m, "UConverterUnicodeSet",
                                        "enum.IntEnum", R"doc(
Selectors for Unicode sets that can be returned by
:func:`ucnv_get_unicode_set`.
      )doc")
      .value("UCNV_ROUNDTRIP_SET", UCNV_ROUNDTRIP_SET, R"doc(
             Select the set of round-trippable Unicode code points.
             )doc")
      .value("UCNV_ROUNDTRIP_AND_FALLBACK_SET", UCNV_ROUNDTRIP_AND_FALLBACK_SET,
             R"doc(
             Select the set of Unicode code points with round-trip or fallback
             mappings.
             )doc")
      .value("UCNV_SET_COUNT", UCNV_SET_COUNT, R"doc(
             Number of :class:`UConverterUnicodeSet` selectors.

             .. version-deprecated:: ICU58

                The numeric value may change over time, see ICU ticket #12420.
             )doc")
      .export_values()
      .finalize();

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
      py::arg("cnv"), R"doc(
      Return a copy of the Unicode converter.

      .. seealso::

         :func:`ucnv_close`
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 71)

  m.def(
      "ucnv_close",
      [](icupy::UConverterPtr &converter) { ucnv_close(converter); },
      py::arg("converter"), R"doc(
      Delete the Unicode converter and release the resources associated only
      with this converter.

      .. seealso::

         :func:`ucnv_clone`
         :func:`ucnv_open_ccsid`
         :func:`ucnv_open_package`
         :func:`ucnv_open`
      )doc");

  m.def("ucnv_compare_names", &ucnv_compareNames, py::arg("name1"),
        py::arg("name2"), R"doc(
      Do a fuzzy compare of two converter/alias names;
      return 0 if the names match, a negative value if the *name1* lexically
      precedes *name2*, or a positive value if the *name1* lexically follows
      *name2*.

      *name1* is the converter name or alias.

      *name2* is the converter name or alias.

      The comparison is case-insensitive, ignores leading zeroes if they are
      not followed by further digits, and ignores all but letters and digits.
      Thus the strings "UTF-8", "utf_8", "u*T\@f08" and "Utf 8" are exactly
      equivalent. See section 1.4, Charset Alias Matching in Unicode Technical
      Standard #22 at http://www.unicode.org/reports/tr22/.
      )doc");

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
      py::arg("alias"), R"doc(
      Return the number of aliases for the specified converter or alias name.

      *alias* is the name of the converter or alias.

      .. seealso::

         :func:`ucnv_get_alias`
      )doc");

  m.def("ucnv_count_available", &ucnv_countAvailable, R"doc(
      Return the number of available converters.

      .. seealso::

         :func:`ucnv_get_available_name`
      )doc");

  m.def("ucnv_count_standards", &ucnv_countStandards, R"doc(
      Return the number of standards associated to the converter names.

      .. seealso::

         :func:`ucnv_get_standard`
      )doc");

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
      py::arg("source"), py::arg("source_length") = -1, R"doc(
      Detect the Unicode signature byte sequence at the start of the byte
      stream; return the name of the encoding detected, or ``None`` if the
      encoding is not detected.

      *source* is the source string in which the signature should be detected.

      *source_length* is the length of *source*;
      can be set to -1 for a NUL-terminated string.
      )doc");

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
      py::arg("cnv"), py::arg("source"), py::arg("source_len") = -1, R"doc(
      Fix mapping errors for backslash characters in specific encodings such as
      Shift-JIS.

      *cnv* is the Unicode converter.

      *source* is the source string to be fixed.

      *source_len* is the length of *source*;
      can be set to -1 for a NUL-terminated string.

      .. seealso::

         :func:`ucnv_is_ambiguous`
      )doc");

  m.def("ucnv_flush_cache", &ucnv_flushCache, R"doc(
      Free up memory occupied by unused, cached converter shared data.
      )doc");

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
      py::arg("alias"), py::arg("n"), R"doc(
      Return the name of the alias at the specified index of alias list.

      *alias* is the name of the alias.

      *n* is the index in the list of aliases.

      .. seealso::

         :func:`ucnv_count_aliases`
      )doc");

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
      py::arg("alias"), R"doc(
      Return a list of alias names for the specified alias.

      *alias* is the name of the alias.
      )doc");

  m.def("ucnv_get_available_name", &ucnv_getAvailableName, py::arg("n"), R"doc(
      Return the canonical converter name of the specified converter from the
      list of all available converters.

      *n* is the index in the list of all available converters.

      .. seealso::

         :func:`ucnv_count_available`
      )doc");

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
      py::arg("alias"), py::arg("standard"), R"doc(
      Return the internal canonical converter name of the tagged alias, or
      ``None`` if the standard or alias name cannot be determined.

      *alias* is the name of the alias.

      *standard* is the name of the standard.

      .. seealso::

         :func:`ucnv_get_standard_name`
      )doc");

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
      py::arg("converter"), R"doc(
      Return the codepage number associated with the converter, or -1 if any
      error occurs.

      .. important::

         The use of CCSIDs is not recommended because it is limited to only
         two platforms in principle and only one
         (:attr:`~UConverterPlatform.UCNV_IBM`) in the current ICU converter
         API. Also, CCSIDs are insufficient to identify IBM Unicode conversion
         tables precisely. For more details see :func:`ucnv_open_ccsid`.

      .. seealso::

         :func:`ucnv_get_platform`
         :func:`ucnv_open_ccsid`
      )doc");

  m.def("ucnv_get_default_name", &ucnv_getDefaultName, R"doc(
      Return the current default converter name.

      .. seealso::

         :func:`ucnv_set_default_name`
      )doc");

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
      py::arg("converter"), py::arg("display_locale"), R"doc(
      Return the name for the converter in a format suitable for display in the
      locale specified by *display_locale*.

      If the locale does not contain a display name, return the internal ASCII
      name.

      *converter* is the Unicode converter.

      *display_locale* is the locale to be used to display the name.

      .. seealso::

         :func:`ucnv_get_name`
      )doc");

  m.def(
       "ucnv_get_from_u_call_back",
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
      Return the current from-Unicode callback function used by the converter.

      .. seealso::

         :func:`ucnv_set_from_u_call_back`
      )doc")
      .def(
          "ucnv_get_from_ucall_back",
          [](const icupy::UConverterPtr &converter) {
            UConverterFromUCallback action = nullptr;
            const void *context = nullptr;
            ucnv_getFromUCallBack(converter, &action, &context);
            if (icupy::UConverterFromUCallbackPtr::is_cpp_function(action)) {
              // C++ (Python) callback function
              auto pair =
                  reinterpret_cast<icupy::FromUCallbackAndContextPair *>(
                      const_cast<void *>(context));
              return icupy::UConverterFromUCallbackPtr(pair);
            }
            // Predefined callback function
            return icupy::UConverterFromUCallbackPtr(action, context);
          },
          py::arg("converter"),
          R"doc(
      Return the current from-Unicode callback function used by the converter.

      .. version-deprecated:: 0.25

         Use :func:`ucnv_get_from_u_call_back` instead.
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
      py::arg("converter"), R"doc(
      Return the internal, canonical name of the converter.

      .. seealso::

         :func:`ucnv_get_display_name`
      )doc");

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
      py::arg("converter"), R"doc(
      Return the codepage platform associated with the converter.
      )doc");

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
      py::arg("n"), R"doc(
      Return the name of the standard at the specified index of standard list.

      *n* is the index in the list of standards.

      .. seealso::

         :func:`ucnv_count_standards`
      )doc");

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
      py::arg("name"), py::arg("standard"), R"doc(
      Return the standard name for the specified converter name, or ``None`` if
      not found.

      *name* is the name of the converter.

      *standard* is the name of the standard.
      )doc");

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
      py::arg("converter"), R"doc(
      Return the substitution character.

      If :func:`ucnv_set_subst_string` set a Unicode string,
      :func:`ucnv_get_subst_chars` will return an empty string.

      .. seealso::

         :func:`ucnv_set_subst_chars`
         :func:`ucnv_set_subst_string`
      )doc");

  m.def(
       "ucnv_get_to_u_call_back",
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
      Return the current to-Unicode callback function used by the converter.

      .. seealso::

         :func:`ucnv_set_to_u_call_back`
      )doc")
      .def(
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
      Return the current to-Unicode callback function used by the converter.

      .. version-deprecated:: 0.25

         Use :func:`ucnv_get_to_u_call_back` instead.
      )doc");

  m.def(
      "ucnv_get_type",
      [](const icupy::UConverterPtr &converter) {
        return ucnv_getType(converter);
      },
      py::arg("converter"), R"doc(
      Return the type of the converter.
      )doc");

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
      py::arg("cnv"), py::arg("set_fill_in"), py::arg("which_set"), R"doc(
      Retrieve the set of Unicode code points that can be converted by the
      converter.

      *cnv* is the Unicode converter.

      *set_fill_in* is a :class:`USet` object to store a specific set of
      converters. The previous contents of *set_fill_in* are lost.

      *which_set* is a :class:`UConverterUnicodeSet` selector. Currently,
      only :attr:`~UConverterUnicodeSet.UCNV_ROUNDTRIP_SET` is supported.

      .. seealso::

         :meth:`UnicodeSet.to_uset`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> from icupy.utils import gc
         >>> with gc(icu.ucnv_open("iso8859-1"), icu.ucnv_close) as cnv:
         ...     us = icu.UnicodeSet()
         ...     icu.ucnv_get_unicode_set(cnv, us.to_uset(), icu.UCNV_ROUNDTRIP_SET)
         ...
         >>> us
         <UnicodeSet('[\u0000-\u00FF]')>
      )doc");

  m.def(
      "ucnv_is_ambiguous",
      [](const icupy::UConverterPtr &cnv) -> bool {
        return ucnv_isAmbiguous(cnv);
      },
      py::arg("cnv"), R"doc(
      Return ``True`` if the converter contains ambiguous mappings for the same
      character, ``False`` otherwise.
      )doc");

  m.def(
      "ucnv_is_fixed_width",
      [](const icupy::UConverterPtr &cnv) -> bool {
        ErrorCode error_code;
        auto result = ucnv_isFixedWidth(cnv, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("cnv"), R"doc(
      Return ``True`` if the converter is fixed-width, ``False`` otherwise.
      )doc");

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
      py::arg("converter_name"), R"doc(
      Create a new ``UConverter`` object with the name of a coded character
      set.

      *converter_name* is the name of the coded character set table. This may
      have options appended to the string. IANA alias character set names,
      IBM CCSIDs starting with "ibm-", Windows codepage numbers starting with
      "windows-" are frequently used for this parameter.
      See :func:`ucnv_get_available_name` and :func:`ucnv_get_alias` for a
      complete list that is available.

      .. seealso::

         :func:`ucnv_close`
         :func:`ucnv_get_alias`
         :func:`ucnv_get_available_name`
         :func:`ucnv_open_ccsid`
      )doc");

  m.def(
      "ucnv_open_all_names",
      []() {
        ErrorCode error_code;
        auto p = ucnv_openAllNames(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<icupy::UEnumerationPtr>(p);
      },
      R"doc(
      Create a new :class:`UEnumeration` object to enumerate all of the
      canonical converter names, as per the alias file, regardless of the
      ability to open each converter.
      )doc");

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
      py::arg("codepage"), py::arg("platform"), R"doc(
      Create a new ``UConverter`` object from a CCSID number and platform
      pair.

      *codepage* is the codepage number used to create a converter.

      *platform* is the platform in which that codepage number exists.
      Currently, only :attr:`~UConverterPlatform.UCNV_IBM` is supported.

      .. important::

         The use of CCSIDs and associated API functions is not recommended.

      .. note::

         The usefulness of this function is limited to platforms with numeric
         encoding IDs. Only IBM and Microsoft platforms use numeric (16-bit)
         identifiers for encoding.

         In addition, IBM CCSIDs and Unicode conversion tables are not 1:1
         related. For many IBM CCSIDs, there are multiple (up to six) Unicode
         conversion tables, and for some Unicode conversion tables, there are
         multiple CCSIDs. Some "alternative" Unicode conversion tables are
         provided by the IBM CDRA conversion table registry. The most prominent
         example of a systematic changes to conversion tables that is not
         provided in the form of conversion table files in the repository is
         that S/390 Unix System Services swaps the codes for Line Feed and
         New Line in all EBCDIC codepages, which requires such a swap in the
         Unicode conversion tables as well.

      .. seealso::

         :func:`ucnv_close`
         :func:`ucnv_get_ccsid`
         :func:`ucnv_get_platform`
         :func:`ucnv_open`
      )doc");

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
      py::arg("package_name"), py::arg("converter_name"), R"doc(
      Create a new ``UConverter`` object from the package name and converter
      name.

      *package_name* is the name of the package.

      *converter_name* is the name of the data item to be used, without the
      suffix.

      .. seealso::

         :func:`ucnv_close`
         :func:`ucnv_open`
      )doc");

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
      py::arg("conv_name"), py::arg("standard"), R"doc(
      Create a new :class:`UEnumeration` object for enumerating all alias names
      associated with the specified converter that are recognized by the
      standard, or ``None`` if any of the parameters are invalid.

      *conv_name* is the name of the converter.

      *standard* is the name of the standard.

      .. seealso::

         :func:`ucnv_get_standard_name`
      )doc");

  m.def(
      "ucnv_reset",
      [](icupy::UConverterPtr &converter) { ucnv_reset(converter); },
      py::arg("converter"), R"doc(
      Reset the state of a converter to the default state.

      This is used in the case of an error, to restart a conversion from a
      known default state. It will also empty the internal output buffers.
      )doc");

  m.def(
      "ucnv_reset_from_unicode",
      [](icupy::UConverterPtr &converter) { ucnv_resetFromUnicode(converter); },
      py::arg("converter"), R"doc(
      Reset the from-Unicode part of a converter state to the default state.

      This is used in the case of an error to restart a conversion from Unicode
      to a known default state. It will also empty the internal output buffers
      used for the conversion from Unicode codepoints.
      )doc");

  m.def(
      "ucnv_reset_to_unicode",
      [](icupy::UConverterPtr &converter) { ucnv_resetToUnicode(converter); },
      py::arg("converter"), R"doc(
      Reset the to-Unicode part of a converter state to the default state.

      This is used in the case of an error to restart a conversion to Unicode
      to a known default state. It will also empty the internal output buffers
      used for the conversion to Unicode codepoints.
      )doc");

  m.def(
      "ucnv_set_default_name",
      [](std::optional<const std::string> &name) {
        ucnv_setDefaultName(name ? name->data() : nullptr);
      },
      py::arg("name"), R"doc(
      Set the default converter name.

      *name* is the name of the converter to be set as the default. If *name*
      is ``None``, the default codepage is used.

      .. important::

         This function is not thread safe.
         DO NOT call this function when ANY ICU function is being used from
         more than one thread.

      .. seealso::

         :func:`ucnv_get_default_name`
      )doc");

  m.def(
      "ucnv_set_fallback",
      [](icupy::UConverterPtr &cnv, bool uses_fallback) {
        ucnv_setFallback(cnv, uses_fallback);
      },
      py::arg("cnv"), py::arg("uses_fallback"), R"doc(
      Set the converter to use fallback mapping or not.

      *cnv* is the Unicode converter.

      If *uses_fallback* is ``True``, fallback mapping will be enabled;
      otherwise, it will be disabled.

      Regardless of this flag, the converter will always use fallbacks from
      Unicode Private Use code points, as well as reverse fallbacks (to
      Unicode). For details, see the
      `ICU User Guide, Conversion Data: .ucm File Format
      <https://unicode-org.github.io/icu/userguide/conversion/data.html#ucm-file-format>`__.

      .. seealso::

         :func:`ucnv_uses_fallback`
      )doc");

  m.def(
       "ucnv_set_from_u_call_back",
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
      Change the current from-Unicode callback function to be used when the
      converter finds an illegal or invalid Unicode sequence, and return
      the old callback function.

      *converter* is the Unicode converter.

      *new_action* is the new callback function.

      .. important::

         *new_action* must outlive the Unicode converter object.

      .. seealso::

         :func:`ucnv_get_from_u_call_back`
         :meth:`UnicodeString.extract`
      )doc")
      .def(
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
            if (icupy::UConverterFromUCallbackPtr::is_cpp_function(
                    old_action)) {
              // Old C++ (Python) callback function
              auto pair =
                  reinterpret_cast<icupy::FromUCallbackAndContextPair *>(
                      const_cast<void *>(old_context));
              return icupy::UConverterFromUCallbackPtr(pair);
            }
            // Predefined callback function
            return icupy::UConverterFromUCallbackPtr(old_action, old_context);
          },
          py::arg("converter"), py::arg("new_action"), R"doc(
      Change the current from-Unicode callback function to be used when the
      converter finds an illegal or invalid Unicode sequence, and return
      the old callback function.

      .. version-deprecated:: 0.25

         Use :func:`ucnv_set_from_u_call_back` instead.
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
      py::arg("converter"), py::arg("sub_chars"), py::arg("length") = -1,
      R"doc(
      Set the substitution character used when converting from Unicode to a
      codepage.

      *converter* is the Unicode converter.

      *sub_chars* is a substitute character to be used when a character cannot
      be converted to a codepage. A substitute character must be a single
      character of 1 to 4 bytes.

      *length* is the length of *sub_chars*;
      can be set to -1 for a NUL-terminated string.

      The caller needs to know the byte sequence of a valid character in the
      converter's charset. For some converters, for example some ISO 2022
      variants, only single-byte substitution characters may be supported.
      The newer :func:`ucnv_set_subst_string` relaxes these limitations.

      .. seealso::

         :func:`ucnv_get_subst_chars`
         :func:`ucnv_set_subst_string`
      )doc");

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
      py::arg("converter"), py::arg("s"), py::arg("length") = -1, R"doc(
      Set the substitution string used when converting from Unicode to the
      charset.

      *converter* is the Unicode converter.

      *s* is the substitution string to be used when a character cannot be
      converted to the charset.

      *length* is the length of *s*;
      can be set to -1 for a NUL-terminated string.

      The caller need not know the charset byte sequence for each charset.

      Unlike :func:`ucnv_set_subst_chars` which is designed to set a charset
      byte sequence for a single character, this function takes a Unicode
      string with zero, one or more characters, and immediately verifies that
      the string can be converted to the charset. If not, or if the result is
      too long (more than 32 bytes as of ICU 3.6), then the function returns
      with an error accordingly.

      Also unlike :func:`ucnv_set_subst_chars`, this function works for
      stateful charsets by converting on the fly at the point of substitution
      rather than setting a fixed byte sequence.

      .. seealso::

         :func:`ucnv_get_subst_chars`
         :func:`ucnv_set_subst_chars`
      )doc");

  m.def(
       "ucnv_set_to_u_call_back",
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
      Change the current to-Unicode callback function to be used when the
      converter finds an illegal or invalid codepage sequence, and return
      the old callback function.

      *converter* is the Unicode converter.

      *new_action* is the new callback function.

      .. important::

         *new_action* must outlive the Unicode converter object.

      .. seealso::

         :func:`ucnv_get_to_u_call_back`
         :meth:`UnicodeString.__init__`
      )doc")
      .def(
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
      Change the current to-Unicode callback function to be used when the
      converter finds an illegal or invalid codepage sequence, and return
      the old callback function.

      .. version-deprecated:: 0.25

         Use :func:`ucnv_set_to_u_call_back` instead.
      )doc");

  m.def(
      "ucnv_uses_fallback",
      [](const icupy::UConverterPtr &cnv) -> bool {
        return ucnv_usesFallback(cnv);
      },
      py::arg("cnv"), R"doc(
      Return ``True`` if the converter uses fallback mapping, ``False``
      otherwise.

      .. seealso::

         :func:`ucnv_set_fallback`
      )doc");

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
