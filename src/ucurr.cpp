#include "main.hpp"
#include <unicode/ucurr.h>

void init_ucurr(py::module &m) {
  py::enum_<UCurrCurrencyType>(m, "UCurrCurrencyType", py::arithmetic(),
                               "Selector constants for *ucurr_open_currencies()*.")
      .value("UCURR_ALL", UCURR_ALL, "Select all ISO-4217 currency codes.")
      .value("UCURR_COMMON", UCURR_COMMON,
             "Select only ISO-4217 commonly used currency codes.\n\n  "
             "These currencies can be found in common use, and they usually have bank notes or coins associated with "
             "the currency code. This does not include fund codes, precious metals and other various ISO-4217 codes "
             "limited to special financial products.")
      .value("UCURR_UNCOMMON", UCURR_UNCOMMON,
             "Select ISO-4217 uncommon currency codes.\n\n  "
             "These codes respresent fund codes, precious metals and other various ISO-4217 codes limited to special "
             "financial products. A fund code is a monetary resource associated with a currency.")
      .value("UCURR_DEPRECATED", UCURR_DEPRECATED,
             "Select only deprecated ISO-4217 codes.\n\n  "
             "These codes are no longer in general public use.")
      .value("UCURR_NON_DEPRECATED", UCURR_NON_DEPRECATED,
             "Select only non-deprecated ISO-4217 codes.\n\n  "
             "These codes are in general public use.")
      .export_values();

#if (U_ICU_VERSION_MAJOR_NUM >= 54)
  py::enum_<UCurrencyUsage>(m, "UCurrencyUsage", py::arithmetic(), "Currency Usage used for Decimal Format.")
      .value("UCURR_USAGE_STANDARD", UCURR_USAGE_STANDARD,
             "A setting to specify currency usage which determines currency digit and rounding for standard usage, for "
             "example: \"50.00 NT$\" used as DEFAULT value.")
      .value("UCURR_USAGE_CASH", UCURR_USAGE_CASH,
             "A setting to specify currency usage which determines currency digit and rounding for cash usage, for "
             "example: \"50 NT$\".")
      .export_values();
#endif // (U_ICU_VERSION_MAJOR_NUM >= 54)

  py::enum_<UCurrNameStyle>(m, "UCurrNameStyle", py::arithmetic(), "Selector constants for *ucurr_get_name()*.")
      .value("UCURR_SYMBOL_NAME", UCURR_SYMBOL_NAME,
             "Selector for *ucurr_get_name* indicating a symbolic name for a currency, such as \"$\" for USD.")
      .value("UCURR_LONG_NAME", UCURR_LONG_NAME,
             "Selector for *ucurr_get_name* indicating the long name for a currency, such as \"US Dollar\" for USD.")
#if (U_ICU_VERSION_MAJOR_NUM >= 61)
      .value("UCURR_NARROW_SYMBOL_NAME", UCURR_NARROW_SYMBOL_NAME,
             "Selector for *get_name()* indicating the narrow currency symbol.\n\n  "
             "The narrow currency symbol is similar to the regular currency symbol, but it always takes the shortest "
             "form: for example, \"$\" instead of \"US$\" for USD in en-CA.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 61)

#if (U_ICU_VERSION_MAJOR_NUM >= 68)
      .value("UCURR_FORMAL_SYMBOL_NAME", UCURR_FORMAL_SYMBOL_NAME,
             "Selector for *get_name()* indicating the formal currency symbol.\n\n  "
             "The formal currency symbol is similar to the regular currency symbol, but it always takes the form used "
             "in formal settings such as banking; for example, \"NT$\" instead of \"$\" for TWD in zh-TW.")
      .value("UCURR_VARIANT_SYMBOL_NAME", UCURR_VARIANT_SYMBOL_NAME,
             "Selector for *get_name()* indicating the variant currency symbol.\n\n  "
             "The variant symbol for a currency is an alternative symbol that is not necessarily as widely used as the "
             "regular symbol.")
#endif // (U_ICU_VERSION_MAJOR_NUM >= 68)
      .export_values();
}
