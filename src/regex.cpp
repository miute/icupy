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
  //
  // icu::RegexMatcher
  //
  py::class_<RegexMatcher, UObject> rm(m, "RegexMatcher");

  //
  // icu::RegexPattern
  //
  py::class_<RegexPattern, UObject> rp(m, "RegexPattern");

  //
  // icu::RegexMatcher
  //
  rm.def(
        // [1] RegexMatcher::RegexMatcher
        py::init([](const icupy::UnicodeStringVariant &regexp, uint32_t flags) {
          ErrorCode error_code;
          auto result = std::make_unique<RegexMatcher>(icupy::to_unistr(regexp), flags, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        }),
        py::arg("regexp"), py::arg("flags"))
      .def(
          // [2] RegexMatcher::RegexMatcher
          py::init([](_UTextPtr &regexp, uint32_t flags) {
            ErrorCode error_code;
            auto result = std::make_unique<RegexMatcher>(regexp, flags, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("flags"))
      .def(
          // [3] RegexMatcher::RegexMatcher
          py::init([](const icupy::UnicodeStringVariant &regexp, const UnicodeString &input, uint32_t flags) {
            ErrorCode error_code;
            auto result = std::make_unique<RegexMatcher>(icupy::to_unistr(regexp), input, flags, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("input_"), py::arg("flags"))
      .def(
          // [4] RegexMatcher::RegexMatcher
          py::init([](_UTextPtr &regexp, _UTextPtr &input, uint32_t flags) {
            ErrorCode error_code;
            auto result = std::make_unique<RegexMatcher>(regexp, input, flags, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          }),
          py::arg("regexp"), py::arg("input_"), py::arg("flags"));

  rm.def(
        "append_replacement",
        [](RegexMatcher &self, UnicodeString &dest, const icupy::UnicodeStringVariant &replacement) -> RegexMatcher & {
          ErrorCode error_code;
          auto &result = self.appendReplacement(dest, icupy::to_unistr(replacement), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("dest"), py::arg("replacement"))
      .def(
          "append_replacement",
          [](RegexMatcher &self, _UTextPtr &dest, _UTextPtr &replacement) -> RegexMatcher & {
            ErrorCode error_code;
            auto &result = self.appendReplacement(dest, replacement, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("dest"), py::arg("replacement"));

  rm.def("append_tail", py::overload_cast<UnicodeString &>(&RegexMatcher::appendTail), py::arg("dest"))
      .def(
          "append_tail",
          [](RegexMatcher &self, _UTextPtr &dest) {
            ErrorCode error_code;
            auto p = self.appendTail(dest, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return std::make_unique<_UTextPtr>(p);
          },
          py::arg("dest"));

  rm.def(
        "end",
        [](const RegexMatcher &self, int32_t group) {
          ErrorCode error_code;
          auto result = self.end(group, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("end", [](const RegexMatcher &self) {
        ErrorCode error_code;
        auto result = self.end(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  rm.def(
        "end64",
        [](const RegexMatcher &self, int32_t group) {
          ErrorCode error_code;
          auto result = self.end64(group, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("end64", [](const RegexMatcher &self) {
        ErrorCode error_code;
        auto result = self.end64(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

#if (U_ICU_VERSION_MAJOR_NUM < 55)
  rm.def("find", py::overload_cast<>(&RegexMatcher::find));
#endif // (U_ICU_VERSION_MAJOR_NUM < 55)
  rm.def(
        "find",
        [](RegexMatcher &self, int64_t start) -> py::bool_ {
          ErrorCode error_code;
          auto result = self.find(start, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("start"))
#if (U_ICU_VERSION_MAJOR_NUM >= 55)
      .def("find",
           [](RegexMatcher &self) -> py::bool_ {
             ErrorCode error_code;
             auto result = self.find(error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           })
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)
      ;

  rm.def("get_find_progress_callback", [](RegexMatcher &self) {
    URegexFindProgressCallback *callback;
    const void *context;
    ErrorCode error_code;
    self.getFindProgressCallback(callback, context, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
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
        ErrorCode error_code;
        auto p = self.getInput(dest.value_or(nullptr), error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return std::make_unique<_UTextPtr>(p);
      },
      py::arg("dest"));

  rm.def("get_match_callback", [](RegexMatcher &self) {
    URegexMatchCallback *callback;
    const void *context;
    ErrorCode error_code;
    self.getMatchCallback(callback, context, error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
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
          ErrorCode error_code;
          auto result = self.group(group_num, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("group_num"))
      .def(
          "group",
          [](const RegexMatcher &self, int32_t group_num, std::optional<_UTextPtr> &dest) {
            ErrorCode error_code;
            int64_t group_len;
            auto p = self.group(group_num, dest.value_or(nullptr), group_len, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return py::make_tuple(std::make_unique<_UTextPtr>(p), group_len);
          },
          py::arg("group_num"), py::arg("dest"))
      .def("group",
           [](const RegexMatcher &self) {
             ErrorCode error_code;
             auto result = self.group(error_code);
             if (error_code.isFailure()) {
               throw icupy::ICUError(error_code);
             }
             return result;
           })
      .def(
          "group",
          [](const RegexMatcher &self, std::optional<_UTextPtr> &dest) {
            ErrorCode error_code;
            int64_t group_len;
            auto p = self.group(dest.value_or(nullptr), group_len, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return py::make_tuple(std::make_unique<_UTextPtr>(p), group_len);
          },
          py::arg("dest"));

  rm.def("group_count", &RegexMatcher::groupCount);

  rm.def("has_anchoring_bounds", [](const RegexMatcher &self) -> py::bool_ { return self.hasAnchoringBounds(); });

  rm.def("has_transparent_bounds", [](const RegexMatcher &self) -> py::bool_ { return self.hasTransparentBounds(); });

  rm.def("hit_end", [](const RegexMatcher &self) -> py::bool_ { return self.hitEnd(); });

  rm.def("input", &RegexMatcher::input);

  rm.def("input_text", [](const RegexMatcher &self) {
    auto p = self.inputText();
    return std::make_unique<_UTextPtr>(p);
  });

  rm.def(
        "looking_at",
        [](RegexMatcher &self, int64_t start_index) -> py::bool_ {
          ErrorCode error_code;
          auto result = self.lookingAt(start_index, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("start_index"))
      .def("looking_at", [](RegexMatcher &self) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.lookingAt(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  rm.def(
        "matches",
        [](RegexMatcher &self, int64_t start_index) -> py::bool_ {
          ErrorCode error_code;
          auto result = self.matches(start_index, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("start_index"))
      .def("matches", [](RegexMatcher &self) -> py::bool_ {
        ErrorCode error_code;
        auto result = self.matches(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  rm.def("pattern", &RegexMatcher::pattern);

  // FIXME: Implement "RegexMatcher& RegexMatcher::refreshInputText(UText *input, UErrorCode &status)".

  rm.def(
        "region",
        [](RegexMatcher &self, int64_t region_start, int64_t region_limit, int64_t start_index) -> RegexMatcher & {
          ErrorCode error_code;
          auto &result = self.region(region_start, region_limit, start_index, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("region_start"), py::arg("region_limit"), py::arg("start_index"))
      .def(
          "region",
          [](RegexMatcher &self, int64_t start, int64_t limit) -> RegexMatcher & {
            ErrorCode error_code;
            auto &result = self.region(start, limit, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
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
        [](RegexMatcher &self, const icupy::UnicodeStringVariant &replacement) {
          ErrorCode error_code;
          auto result = self.replaceAll(icupy::to_unistr(replacement), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("replacement"))
      .def(
          "replace_all",
          [](RegexMatcher &self, _UTextPtr &replacement, std::optional<_UTextPtr> &dest) {
            ErrorCode error_code;
            auto p = self.replaceAll(replacement, dest.value_or(nullptr), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return std::make_unique<_UTextPtr>(p);
          },
          py::arg("replacement"), py::arg("dest"));

  rm.def(
        "replace_first",
        [](RegexMatcher &self, const icupy::UnicodeStringVariant &replacement) {
          ErrorCode error_code;
          auto result = self.replaceFirst(icupy::to_unistr(replacement), error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("replacement"))
      .def(
          "replace_first",
          [](RegexMatcher &self, _UTextPtr &replacement, std::optional<_UTextPtr> &dest) {
            ErrorCode error_code;
            auto p = self.replaceFirst(replacement, dest.value_or(nullptr), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return std::make_unique<_UTextPtr>(p);
          },
          py::arg("replacement"), py::arg("dest"));

  rm.def("require_end", [](const RegexMatcher &self) -> py::bool_ { return self.requireEnd(); });

  rm.def("reset", py::overload_cast<>(&RegexMatcher::reset))
      .def("reset", py::overload_cast<const UnicodeString &>(&RegexMatcher::reset), py::arg("input_"))
      .def(
          "reset",
          [](RegexMatcher &self, int64_t index) -> RegexMatcher & {
            ErrorCode error_code;
            auto &result = self.reset(index, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
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
        ErrorCode error_code;
        self.setFindProgressCallback(fp, cp, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
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
        ErrorCode error_code;
        self.setMatchCallback(fp, cp, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("callback"), py::arg("context"));

  rm.def(
      "set_stack_limit",
      [](RegexMatcher &self, int32_t limit) {
        ErrorCode error_code;
        self.setStackLimit(limit, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("limit"));

  rm.def(
      "set_time_limit",
      [](RegexMatcher &self, int32_t limit) {
        ErrorCode error_code;
        self.setTimeLimit(limit, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
      },
      py::arg("limit"));

  rm.def(
        "split",
        [](RegexMatcher &self, const UnicodeString &input, icupy::UnicodeStringVector &dest, int32_t dest_capacity) {
          if (dest_capacity == -1) {
            dest_capacity = static_cast<int32_t>(dest.size());
          }
          ErrorCode error_code;
          auto result = self.split(input, dest.data(), dest_capacity, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
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
            ErrorCode error_code;
            std::vector<UText *> output(std::max(0, dest_capacity), nullptr);
            std::copy(dest.begin(), dest.begin() + output.size(), output.begin());
            auto result = self.split(input, output.data(), dest_capacity, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("input_"), py::arg("dest"), py::arg("dest_capacity") = -1);

  rm.def(
        "start",
        [](const RegexMatcher &self, int32_t group) {
          ErrorCode error_code;
          auto result = self.start(group, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("start", [](const RegexMatcher &self) {
        ErrorCode error_code;
        auto result = self.start(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  rm.def(
        "start64",
        [](const RegexMatcher &self, int32_t group) {
          ErrorCode error_code;
          auto result = self.start64(group, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("group"))
      .def("start64", [](const RegexMatcher &self) {
        ErrorCode error_code;
        auto result = self.start64(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  rm.def(
      "use_anchoring_bounds",
      [](RegexMatcher &self, py::bool_ b) -> RegexMatcher & { return self.useAnchoringBounds(b); }, py::arg("b"));

  rm.def(
      "use_transparent_bounds",
      [](RegexMatcher &self, py::bool_ b) -> RegexMatcher & { return self.useTransparentBounds(b); }, py::arg("b"));

  //
  // icu::RegexPattern
  //
  rp.def(py::init<>()).def(py::init<const RegexPattern &>(), py::arg("other"));

  rp.def("__copy__", &RegexPattern::clone);

  rp.def(
      "__deepcopy__", [](const RegexPattern &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  rp.def(
      "__eq__", [](const RegexPattern &self, const RegexPattern &other) { return self == other; }, py::is_operator(),
      py::arg("other"));

  rp.def(
      "__ne__", [](const RegexPattern &self, const RegexPattern &other) { return self != other; }, py::is_operator(),
      py::arg("other"));

  rp.def("clone", &RegexPattern::clone);

  rp.def_static(
        "compile",
        [](const icupy::UnicodeStringVariant &regex, uint32_t flags) {
          ErrorCode error_code;
          auto result = RegexPattern::compile(icupy::to_unistr(regex), flags, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("regex"), py::arg("flags"))
      .def_static(
          "compile",
          [](const icupy::UnicodeStringVariant &regex, uint32_t flags, UParseError &pe) {
            ErrorCode error_code;
            auto result = RegexPattern::compile(icupy::to_unistr(regex), flags, pe, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"), py::arg("pe"))
      .def_static(
          "compile",
          [](const icupy::UnicodeStringVariant &regex, UParseError &pe) {
            ErrorCode error_code;
            auto result = RegexPattern::compile(icupy::to_unistr(regex), pe, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("pe"))
      .def_static(
          "compile",
          [](_UTextPtr &regex, uint32_t flags) {
            ErrorCode error_code;
            auto result = RegexPattern::compile(regex, flags, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"))
      .def_static(
          "compile",
          [](_UTextPtr &regex, uint32_t flags, UParseError &pe) {
            ErrorCode error_code;
            auto result = RegexPattern::compile(regex, flags, pe, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("flags"), py::arg("pe"))
      .def_static(
          "compile",
          [](_UTextPtr &regex, UParseError &pe) {
            ErrorCode error_code;
            auto result = RegexPattern::compile(regex, pe, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("pe"));

  rp.def("flags", &RegexPattern::flags);

#if (U_ICU_VERSION_MAJOR_NUM >= 55)
  rp.def(
        "group_number_from_name",
        [](const RegexPattern &self, const char *group_name, int32_t name_length) {
          ErrorCode error_code;
          auto result = self.groupNumberFromName(group_name, name_length, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("group_name"), py::arg("name_length"))
      .def(
          "group_number_from_name",
          [](const RegexPattern &self, const icupy::UnicodeStringVariant &group_name) {
            ErrorCode error_code;
            auto result = self.groupNumberFromName(icupy::to_unistr(group_name), error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("group_name"));
#endif // (U_ICU_VERSION_MAJOR_NUM >= 55)

  rp.def(
        "matcher",
        [](const RegexPattern &self, const UnicodeString &input) {
          ErrorCode error_code;
          auto result = self.matcher(input, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("input_"))
      .def("matcher", [](const RegexPattern &self) {
        ErrorCode error_code;
        auto result = self.matcher(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      });

  rp.def_static(
        "matches",
        [](const icupy::UnicodeStringVariant &regex, const UnicodeString &input, UParseError &pe) -> py::bool_ {
          ErrorCode error_code;
          auto result = RegexPattern::matches(icupy::to_unistr(regex), input, pe, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("regex"), py::arg("input_"), py::arg("pe"))
      .def_static(
          "matches",
          [](_UTextPtr &regex, _UTextPtr &input, UParseError &pe) -> py::bool_ {
            ErrorCode error_code;
            auto result = RegexPattern::matches(regex, input, pe, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("regex"), py::arg("input_"), py::arg("pe"));

  rp.def("pattern", &RegexPattern::pattern);

  rp.def("pattern_text", [](const RegexPattern &self) {
    ErrorCode error_code;
    auto p = self.patternText(error_code);
    if (error_code.isFailure()) {
      throw icupy::ICUError(error_code);
    }
    return std::make_unique<_UTextPtr>(p);
  });

  rp.def(
        "split",
        [](const RegexPattern &self, const UnicodeString &input, icupy::UnicodeStringVector &dest,
           int32_t dest_capacity) {
          if (dest_capacity == -1) {
            dest_capacity = static_cast<int32_t>(dest.size());
          }
          ErrorCode error_code;
          auto result = self.split(input, dest.data(), dest_capacity, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
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
            ErrorCode error_code;
            std::vector<UText *> output(std::max(0, dest_capacity), nullptr);
            std::copy(dest.begin(), dest.begin() + output.size(), output.begin());
            auto result = self.split(input, output.data(), dest_capacity, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("input_"), py::arg("dest"), py::arg("dest_capacity") = -1);
}
