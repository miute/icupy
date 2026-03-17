#include "main.hpp"
#include <unicode/ucurr.h>

void init_ucurr(py::module &m) {
  //
  // enum UCurrCurrencyType
  //
  py::enum_<UCurrCurrencyType>(m, "UCurrCurrencyType", py::arithmetic(),
                               R"doc(
Selector constants for ``ucurr_openCurrencies()``.
      )doc")
      .value("UCURR_ALL", UCURR_ALL, R"doc(
             Select all ISO-4217 currency codes.
             )doc")
      .value("UCURR_COMMON", UCURR_COMMON, R"doc(
             Select only ISO-4217 commonly used currency codes.

             These currencies can be found in common use, and they usually have
             bank notes or coins associated with the currency code. This does
             not include fund codes, precious metals and other various ISO-4217
             codes limited to special financial products.
             )doc")
      .value("UCURR_UNCOMMON", UCURR_UNCOMMON, R"doc(
             Select ISO-4217 uncommon currency codes.

             These codes respresent fund codes, precious metals and other
             various ISO-4217 codes limited to special financial products.
             A fund code is a monetary resource associated with a currency.
             )doc")
      .value("UCURR_DEPRECATED", UCURR_DEPRECATED, R"doc(
             Select only deprecated ISO-4217 codes.

             These codes are no longer in general public use.
             )doc")
      .value("UCURR_NON_DEPRECATED", UCURR_NON_DEPRECATED, R"doc(
             Select only non-deprecated ISO-4217 codes.

             These codes are in general public use.
             )doc")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  //
  // enum UCurrencyUsage
  //
  py::enum_<UCurrencyUsage>(m, "UCurrencyUsage", py::arithmetic(), R"doc(
Currency Usage used for Decimal Format.
      )doc")
      .value("UCURR_USAGE_STANDARD", UCURR_USAGE_STANDARD, R"doc(
             A setting to specify currency usage which determines currency
             digit and rounding for standard usage, for example:
             "50.00 NT$" used as DEFAULT value.
             )doc")
      .value("UCURR_USAGE_CASH", UCURR_USAGE_CASH, R"doc(
             A setting to specify currency usage which determines currency
             digit and rounding for cash usage, for example: "50 NT$".
             )doc")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  //
  // enum UCurrNameStyle
  //
  py::enum_<UCurrNameStyle>(m, "UCurrNameStyle", py::arithmetic(), R"doc(
Selector constants for ``ucurr_getName()``.
      )doc")
      .value("UCURR_SYMBOL_NAME", UCURR_SYMBOL_NAME, R"doc(
             Selector for ``ucurr_getName()`` indicating a symbolic name for a
             currency, such as "$" for USD.
             )doc")
      .value("UCURR_LONG_NAME", UCURR_LONG_NAME, R"doc(
             Selector for ``ucurr_getName()`` indicating the long name for a
             currency, such as "US Dollar" for USD.
             )doc")
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("UCURR_NARROW_SYMBOL_NAME", UCURR_NARROW_SYMBOL_NAME, R"doc(
             Selector for ``ucurr_getName()`` indicating the narrow currency
             symbol.

             The narrow currency symbol is similar to the regular currency
             symbol, but it always takes the shortest form: for example, "$"
             instead of "US$" for USD in en-CA.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)
#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UCURR_FORMAL_SYMBOL_NAME", UCURR_FORMAL_SYMBOL_NAME, R"doc(
             Selector for ``ucurr_getName()`` indicating the formal currency
             symbol.

             The formal currency symbol is similar to the regular currency
             symbol, but it always takes the form used in formal settings such
             as banking; for example, "NT$" instead of "$" for TWD in zh-TW.
             )doc")
      .value("UCURR_VARIANT_SYMBOL_NAME", UCURR_VARIANT_SYMBOL_NAME, R"doc(
             Selector for ``ucurr_getName()`` indicating the variant currency
             symbol.

             The variant symbol for a currency is an alternative symbol that is
             not necessarily as widely used as the regular symbol.
             )doc")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .export_values();
}
