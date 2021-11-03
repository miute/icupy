#include "main.hpp"
#include "uregex.hpp"
#include "utextvec.hpp"
#include "voidptr.hpp"
#include <memory>
#include <optional>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include <unicode/regex.h>

using namespace icu;

void init_regex(py::module &m) {
  // icu::RegexMatcher
  py::class_<RegexMatcher, UObject> rm(m, "RegexMatcher");

  // icu::RegexPattern
  py::class_<RegexPattern, UObject> rp(m, "RegexPattern");

  // icu::RegexMatcher
  rm.def(
        // [1] RegexMatcher::RegexMatcher
        py::init([](const UnicodeString &regexp, uint32_t flags) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = std::make_unique<RegexMatcher>(regexp, flags, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        }),
        py::arg("regexp"), py::arg("flags"))
      .def(
          // const char16_t *regexp -> const UnicodeString &regexp
          py::init([](const char16_t *regexp, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RegexMatcher>(regexp, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("flags"))
      .def(
          // [2] RegexMatcher::RegexMatcher
          py::init([](_UTextPtr &regexp, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RegexMatcher>(regexp, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("flags"))
      .def(
          // [3] RegexMatcher::RegexMatcher
          py::init([](const UnicodeString &regexp, const UnicodeString &input, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RegexMatcher>(regexp, input, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("input_"), py::arg("flags"))
      .def(
          // const char16_t *regexp -> const UnicodeString &regexp
          py::init([](const char16_t *regexp, const UnicodeString &input, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RegexMatcher>(regexp, input, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("input_"), py::arg("flags"))
      .def(
          // [4] RegexMatcher::RegexMatcher
          py::init([](_UTextPtr &regexp, _UTextPtr &input, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = std::make_unique<RegexMatcher>(regexp, input, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("input_"), py::arg("flags"));
  rm.def(
        "append_replacement",
        [](RegexMatcher &self, UnicodeString &dest, const UnicodeString &replacement) -> RegexMatcher & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.appendReplacement(dest, replacement, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("dest"), py::arg("replacement"))
      .def(
          // const char16_t *replacement -> const UnicodeString &replacement
          "append_replacement",
          [](RegexMatcher &self, UnicodeString &dest, const char16_t *replacement) -> RegexMatcher & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.appendReplacement(dest, replacement, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("dest"), py::arg("replacement"))
      .def(
          "append_replacement",
          [](RegexMatcher &self, _UTextPtr &dest, _UTextPtr &replacement) -> RegexMatcher & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.appendReplacement(dest, replacement, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("dest"), py::arg("replacement"));
  rm.def("append_tail", py::overload_cast<UnicodeString &>(&RegexMatcher::appendTail), py::arg("dest"))
      .def(
          "append_tail",
          [](RegexMatcher &self, _UTextPtr &dest) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto p = self.appendTail(dest, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return std::make_unique<_UTextPtr>(p);
          },
          py::arg("dest"));
  rm.def(
        "end",
        [](const RegexMatcher &self, int32_t group) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.end(group, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("end", [](const RegexMatcher &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.end(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  rm.def(
        "end64",
        [](const RegexMatcher &self, int32_t group) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.end64(group, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("end64", [](const RegexMatcher &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.end64(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
#if (U_ICU_VERSION_MAJOR_NUM < 55)
  rm.def("find", py::overload_cast<>(&RegexMatcher::find));
#endif // (U_ICU_VERSION_MAJOR_NUM < 55)
  rm.def(
        "find",
        [](RegexMatcher &self, int64_t start) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.find(start, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("start"))
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
      .def("find",
           [](RegexMatcher &self) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = self.find(error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           })
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
      ;
  rm.def("get_find_progress_callback", [](RegexMatcher &self) {
    URegexFindProgressCallback *callback;
    const void *context;
    UErrorCode error_code = U_ZERO_ERROR;
    self.getFindProgressCallback(callback, context, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    if (callback == _URegexFindProgressCallbackPtr::callback) {
      // Python callback function and callback data
      auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
      auto result1 = _URegexFindProgressCallbackPtr(result2->get_action());
      return py::make_tuple(result1, result2);
    }
    // C callback function and callback data
    return py::make_tuple(_URegexFindProgressCallbackPtr(callback), _ConstVoidPtr(context));
  });
  rm.def(
      "get_input",
      [](const RegexMatcher &self, std::optional<_UTextPtr> &dest) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto p = self.getInput(dest.value_or(nullptr), error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("dest"));
  rm.def("get_match_callback", [](RegexMatcher &self) {
    URegexMatchCallback *callback;
    const void *context;
    UErrorCode error_code = U_ZERO_ERROR;
    self.getMatchCallback(callback, context, error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    if (callback == _URegexMatchCallbackPtr::callback) {
      // Python callback function and callback data
      auto result2 = reinterpret_cast<_ConstVoidPtr *>(const_cast<void *>(context));
      auto result1 = _URegexMatchCallbackPtr(result2->get_action());
      return py::make_tuple(result1, result2);
    }
    // C callback function and callback data
    return py::make_tuple(_URegexMatchCallbackPtr(callback), _ConstVoidPtr(context));
  });
  rm.def("get_stack_limit", &RegexMatcher::getStackLimit);
  rm.def("get_time_limit", &RegexMatcher::getTimeLimit);
  rm.def(
        "group",
        [](const RegexMatcher &self, int32_t group_num) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.group(group_num, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("group_num"))
      .def(
          "group",
          [](const RegexMatcher &self, int32_t group_num, std::optional<_UTextPtr> &dest) {
            UErrorCode error_code = U_ZERO_ERROR;
            int64_t group_len;
            auto p = self.group(group_num, dest.value_or(nullptr), group_len, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return py::make_tuple(std::make_unique<_UTextPtr>(p), group_len);
          },
          py::arg("group_num"), py::arg("dest"))
      .def("group",
           [](const RegexMatcher &self) {
             UErrorCode error_code = U_ZERO_ERROR;
             auto result = self.group(error_code);
             if (U_FAILURE(error_code)) {
               throw ICUError(error_code);
             }
             return result;
           })
      .def(
          "group",
          [](const RegexMatcher &self, std::optional<_UTextPtr> &dest) {
            UErrorCode error_code = U_ZERO_ERROR;
            int64_t group_len;
            auto p = self.group(dest.value_or(nullptr), group_len, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return py::make_tuple(std::make_unique<_UTextPtr>(p), group_len);
          },
          py::arg("dest"));
  rm.def("group_count", &RegexMatcher::groupCount);
  rm.def("has_anchoring_bounds", &RegexMatcher::hasAnchoringBounds);
  rm.def("has_transparent_bounds", &RegexMatcher::hasTransparentBounds);
  rm.def("hit_end", &RegexMatcher::hitEnd);
  rm.def("input", &RegexMatcher::input);
  rm.def("input_text", [](const RegexMatcher &self) {
    auto p = self.inputText();
    return std::make_unique<_UTextPtr>(p);
  });
  rm.def(
        "looking_at",
        [](RegexMatcher &self, int64_t start_index) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.lookingAt(start_index, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("start_index"))
      .def("looking_at", [](RegexMatcher &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.lookingAt(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  rm.def(
        "matches",
        [](RegexMatcher &self, int64_t start_index) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.matches(start_index, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("start_index"))
      .def("matches", [](RegexMatcher &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.matches(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  rm.def("pattern", &RegexMatcher::pattern);
  // FIXME: Implement "RegexMatcher& RegexMatcher::refreshInputText(UText *input, UErrorCode &status)".
  rm.def(
        "region",
        [](RegexMatcher &self, int64_t region_start, int64_t region_limit, int64_t start_index) -> RegexMatcher & {
          UErrorCode error_code = U_ZERO_ERROR;
          auto &result = self.region(region_start, region_limit, start_index, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("region_start"), py::arg("region_limit"), py::arg("start_index"))
      .def(
          "region",
          [](RegexMatcher &self, int64_t start, int64_t limit) -> RegexMatcher & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.region(start, limit, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("start"), py::arg("limit"));
  rm.def("region_end", &RegexMatcher::regionEnd);
  rm.def("region_end64", &RegexMatcher::regionEnd64);
  rm.def("region_start", &RegexMatcher::regionStart);
  rm.def("region_start64", &RegexMatcher::regionStart64);
  rm.def(
        "replace_all",
        [](RegexMatcher &self, const UnicodeString &replacement) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.replaceAll(replacement, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("replacement"))
      .def(
          // const char16_t *replacement -> const UnicodeString &replacement
          "replace_all",
          [](RegexMatcher &self, const char16_t *replacement) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceAll(replacement, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("replacement"))
      .def(
          "replace_all",
          [](RegexMatcher &self, _UTextPtr &replacement, std::optional<_UTextPtr> &dest) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto p = self.replaceAll(replacement, dest.value_or(nullptr), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return std::make_unique<_UTextPtr>(p);
          },
          py::arg("replacement"), py::arg("dest"));
  rm.def(
        "replace_first",
        [](RegexMatcher &self, const UnicodeString &replacement) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.replaceFirst(replacement, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("replacement"))
      .def(
          // const char16_t *replacement -> const UnicodeString &replacement
          "replace_first",
          [](RegexMatcher &self, const char16_t *replacement) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.replaceFirst(replacement, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("replacement"))
      .def(
          "replace_first",
          [](RegexMatcher &self, _UTextPtr &replacement, std::optional<_UTextPtr> &dest) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto p = self.replaceFirst(replacement, dest.value_or(nullptr), error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return std::make_unique<_UTextPtr>(p);
          },
          py::arg("replacement"), py::arg("dest"));
  rm.def("require_end", &RegexMatcher::requireEnd);
  rm.def("reset", py::overload_cast<>(&RegexMatcher::reset))
      .def("reset", py::overload_cast<const UnicodeString &>(&RegexMatcher::reset), py::arg("input_"))
      .def(
          "reset",
          [](RegexMatcher &self, int64_t index) -> RegexMatcher & {
            UErrorCode error_code = U_ZERO_ERROR;
            auto &result = self.reset(index, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("index"))
      .def(
          "reset", [](RegexMatcher &self, _UTextPtr &input) -> RegexMatcher & { return self.reset(input); },
          py::arg("input_"));
  rm.def(
      "set_find_progress_callback",
      [](RegexMatcher &self, _URegexFindProgressCallbackPtr &callback, _ConstVoidPtr &context) {
        URegexFindProgressCallback *fp = callback.get_if<URegexFindProgressCallback *>();
        const void *cp = nullptr;
        if (fp == nullptr && callback.has_value()) {
          // New Python callback function and callback data
          fp = callback.callback;
          context.set_action(callback.get<py::function>());
          cp = &context;
        } else if (context.has_value()) {
          // New C callback data
          cp = context.to_c_str();
        }
        UErrorCode error_code = U_ZERO_ERROR;
        self.setFindProgressCallback(fp, cp, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("callback"), py::arg("context"));
  rm.def(
      "set_match_callback",
      [](RegexMatcher &self, _URegexMatchCallbackPtr &callback, _ConstVoidPtr &context) {
        URegexMatchCallback *fp = callback.get_if<URegexMatchCallback *>();
        const void *cp = nullptr;
        if (fp == nullptr && callback.has_value()) {
          // New Python callback function and callback data
          fp = callback.callback;
          context.set_action(callback.get<py::function>());
          cp = &context;
        } else if (context.has_value()) {
          // New C callback data
          cp = context.to_c_str();
        }
        UErrorCode error_code = U_ZERO_ERROR;
        self.setMatchCallback(fp, cp, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("callback"), py::arg("context"));
  rm.def(
      "set_stack_limit",
      [](RegexMatcher &self, int32_t limit) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setStackLimit(limit, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("limit"));
  rm.def(
      "set_time_limit",
      [](RegexMatcher &self, int32_t limit) {
        UErrorCode error_code = U_ZERO_ERROR;
        self.setTimeLimit(limit, error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
      },
      py::arg("limit"));
  rm.def(
        "split",
        [](RegexMatcher &self, const UnicodeString &input, _UnicodeStringVector &dest, int32_t dest_capacity) {
          if (dest_capacity == -1) {
            dest_capacity = static_cast<int32_t>(dest.size());
          }
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.split(input, dest.data(), dest_capacity, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("input_"), py::arg("dest"), py::arg("dest_capacity") = -1)
      .def(
          "split",
          [](RegexMatcher &self, _UTextPtr &input, _UTextVector &dest, int32_t dest_capacity) {
            if (dest_capacity == -1) {
              dest_capacity = static_cast<int32_t>(dest.size());
            }
            UErrorCode error_code = U_ZERO_ERROR;
            std::vector<UText *> output(std::max(0, dest_capacity));
            std::copy(dest.begin(), dest.begin() + output.size(), output.begin());
            auto result = self.split(input, output.data(), dest_capacity, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("input_"), py::arg("dest"), py::arg("dest_capacity") = -1);
  rm.def(
        "start",
        [](const RegexMatcher &self, int32_t group) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.start(group, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("start", [](const RegexMatcher &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.start(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  rm.def(
        "start64",
        [](const RegexMatcher &self, int32_t group) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.start64(group, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("start64", [](const RegexMatcher &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.start64(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  rm.def("use_anchoring_bounds", &RegexMatcher::useAnchoringBounds, py::arg("b"));
  rm.def("use_transparent_bounds", &RegexMatcher::useTransparentBounds, py::arg("b"));

  // icu::RegexPattern
  rp.def(py::init<>())
      .def(py::init<const RegexPattern &>(), py::arg("source"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  rp.def("__copy__", &RegexPattern::clone)
      .def(
          "__deepcopy__", [](const RegexPattern &self, py::dict) { return self.clone(); }, py::arg("memo"));
  rp.def("clone", &RegexPattern::clone);
  rp.def_static(
        "compile",
        [](const UnicodeString &regex, uint32_t flags) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = RegexPattern::compile(regex, flags, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("regex"), py::arg("flags"))
      .def_static(
          // const char16_t *regex -> const UnicodeString &regex
          "compile",
          [](const char16_t *regex, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"))
      .def_static(
          "compile",
          [](const UnicodeString &regex, uint32_t flags, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, flags, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"), py::arg("pe"))
      .def_static(
          // const char16_t *regex -> const UnicodeString &regex
          "compile",
          [](const char16_t *regex, uint32_t flags, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, flags, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"), py::arg("pe"))
      .def_static(
          "compile",
          [](const UnicodeString &regex, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("pe"))
      .def_static(
          // const char16_t *regex -> const UnicodeString &regex
          "compile",
          [](const char16_t *regex, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("pe"))
      .def_static(
          "compile",
          [](_UTextPtr &regex, uint32_t flags) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, flags, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"))
      .def_static(
          "compile",
          [](_UTextPtr &regex, uint32_t flags, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, flags, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"), py::arg("pe"))
      .def_static(
          "compile",
          [](_UTextPtr &regex, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::compile(regex, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("pe"));
  rp.def("flags", &RegexPattern::flags);
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  rp.def(
        "group_number_from_name",
        [](const RegexPattern &self, const char *group_name, int32_t name_length) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.groupNumberFromName(group_name, name_length, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("group_name"), py::arg("name_length"))
      .def(
          "group_number_from_name",
          [](const RegexPattern &self, const UnicodeString &group_name) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.groupNumberFromName(group_name, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("group_name"))
      .def(
          // const char16_t *group_name -> const UnicodeString &group_name
          "group_number_from_name",
          [](const RegexPattern &self, const char16_t *group_name) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = self.groupNumberFromName(group_name, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("group_name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
  rp.def(
        "matcher",
        [](const RegexPattern &self, const UnicodeString &input) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.matcher(input, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("input_"))
      .def("matcher", [](const RegexPattern &self) {
        UErrorCode error_code = U_ZERO_ERROR;
        auto result = self.matcher(error_code);
        if (U_FAILURE(error_code)) {
          throw ICUError(error_code);
        }
        return result;
      });
  rp.def_static(
        "matches",
        [](const UnicodeString &regex, const UnicodeString &input, UParseError &pe) {
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = RegexPattern::matches(regex, input, pe, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("regex"), py::arg("input_"), py::arg("pe"))
      .def_static(
          // const char16_t *regex -> const UnicodeString &regex
          "matches",
          [](const char16_t *regex, const UnicodeString &input, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::matches(regex, input, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("input_"), py::arg("pe"))
      .def_static(
          "matches",
          [](_UTextPtr &regex, _UTextPtr &input, UParseError &pe) {
            UErrorCode error_code = U_ZERO_ERROR;
            auto result = RegexPattern::matches(regex, input, pe, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("input_"), py::arg("pe"));
  rp.def("pattern", &RegexPattern::pattern);
  rp.def("pattern_text", [](const RegexPattern &self) {
    UErrorCode error_code = U_ZERO_ERROR;
    auto p = self.patternText(error_code);
    if (U_FAILURE(error_code)) {
      throw ICUError(error_code);
    }
    return std::make_unique<_UTextPtr>(p);
  });
  rp.def(
        "split",
        [](const RegexPattern &self, const UnicodeString &input, _UnicodeStringVector &dest, int32_t dest_capacity) {
          if (dest_capacity == -1) {
            dest_capacity = static_cast<int32_t>(dest.size());
          }
          UErrorCode error_code = U_ZERO_ERROR;
          auto result = self.split(input, dest.data(), dest_capacity, error_code);
          if (U_FAILURE(error_code)) {
            throw ICUError(error_code);
          }
          return result;
        },
        py::arg("input_"), py::arg("dest"), py::arg("dest_capacity") = -1)
      .def(
          "split",
          [](const RegexPattern &self, _UTextPtr &input, _UTextVector &dest, int32_t dest_capacity) {
            if (dest_capacity == -1) {
              dest_capacity = static_cast<int32_t>(dest.size());
            }
            UErrorCode error_code = U_ZERO_ERROR;
            std::vector<UText *> output(std::max(0, dest_capacity));
            std::copy(dest.begin(), dest.begin() + output.size(), output.begin());
            auto result = self.split(input, output.data(), dest_capacity, error_code);
            if (U_FAILURE(error_code)) {
              throw ICUError(error_code);
            }
            return result;
          },
          py::arg("input_"), py::arg("dest"), py::arg("dest_capacity") = -1);
}
