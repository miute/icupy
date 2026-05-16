#include "main.hpp"
#include <optional>
#include <pybind11/stl.h>
#include <unicode/translit.h>
#include <unicode/uniset.h>

using namespace icu;

class PyTransliterator : public Transliterator {
public:
  PyTransliterator(const UnicodeString &id, UnicodeFilter *adopted_filter)
      : Transliterator(id, adopted_filter) {}

  PyTransliterator(const Transliterator &other) : Transliterator(other) {}

  virtual Transliterator *clone() const override {
    return new PyTransliterator(*this);
  }

  virtual UnicodeSet &getTargetSet(UnicodeSet &result) const override {
    PYBIND11_OVERRIDE_NAME(UnicodeSet &, Transliterator, "get_target_set",
                           getTargetSet, result);
  }

  virtual void handleGetSourceSet(UnicodeSet &result) const override {
    PYBIND11_OVERRIDE_NAME(void, Transliterator, "handle_get_source_set",
                           handleGetSourceSet, result);
  }

  virtual void handleTransliterate(Replaceable &text, UTransPosition &pos,
                                   UBool incremental) const override {
    PYBIND11_OVERRIDE_PURE_NAME(void, Transliterator, "_handle_transliterate",
                                handleTransliterate, text, std::ref(pos),
                                py::bool_(incremental));
  }

  virtual UClassID getDynamicClassID() const override;

  static UClassID U_EXPORT2 getStaticClassID();

  using Transliterator::createBasicInstance;
  using Transliterator::setID;
  using Transliterator::setMaximumContextLength;
};

UOBJECT_DEFINE_RTTI_IMPLEMENTATION(PyTransliterator);

void init_translit(py::module &m) {
  //
  // class icu::Transliterator
  //
  py::class_<Transliterator, UObject, PyTransliterator> tl(m, "Transliterator",
                                                           R"doc(
      Abstract class that transliterates text from one format to another.

      - Subclassing Transliterators

        Subclasses must implement the abstract method
        :meth:`._handle_transliterate`.
        Also, override other methods as needed.

        .. rubric:: Example

        .. code-block:: python

           # Recreating the compound transliterator in Python
           from icupy import icu
           class CompoundTransliterator(icu.Transliterator):
               def __init__(self, compound_id: str, direction: icu.UTransDirection) -> None:
                   icu.Transliterator.__init__(self, "")
                   self._trans: list[icu.Transliterator] = []
                   global_filter = self._parse_compound_id(compound_id, direction)
                   self.adopt_filter(global_filter)
                   new_id = ";".join(str(x.get_id()) for x in self._trans)
                   self._set_id(new_id)
                   new_context_length = max(x.get_maximum_context_length() for x in self._trans)
                   self._set_maximum_context_length(new_context_length)
               def _handle_transliterate(
                   self,
                   text: icu.Replaceable,
                   pos: icu.UTransPosition,
                   incremental: bool,
               ) -> None:
                   compound_limit = pos.limit
                   compound_start = pos.start
                   delta = 0
                   for tl in self._trans:
                       pos.start = compound_start
                       limit = pos.limit
                       if pos.start == pos.limit:
                           break
                       tl.filtered_transliterate(text, pos, incremental)
                       if not incremental and pos.start != pos.limit:
                           pos.start = pos.limit
                       delta += pos.limit - limit
                       if incremental:
                           pos.limit = pos.start
                   compound_limit += delta
                   pos.limit = compound_limit
               def _parse_compound_id(
                   self,
                   compound_id: str,
                   direction: icu.UTransDirection,
               ) -> icu.UnicodeSet | None:
                   # Split IDs in the "ID;ID;..." format into single IDs, and
                   # create a list of transliterator objects from the single IDs.
                   global_filter: icu.UnicodeSet | None = None
                   for i, single in enumerate(filter(None, map(str.strip, compound_id.split(";")))):
                       if i == 0 and single.startswith("[") and single.endswith("]"):
                           # Use the initial single ID as a global filter.
                           pos = icu.ParsePosition()
                           global_filter = icu.UnicodeSet(single, pos, icu.USET_IGNORE_SPACE)
                       else:
                           tl = self._create_basic_instance(single)
                           if tl is None:
                               continue
                           self._trans.append(tl)
                   if len(self._trans) == 0:
                       tl = self._create_basic_instance("Any-Null")
                       self._trans.append(tl)
                   if direction == icu.UTRANS_REVERSE:
                       self._trans.reverse()
                   return global_filter
               def count_elements(self) -> int:
                   return len(self._trans)
               def get_element(self, index: int) -> icu.Transliterator:
                   return self._trans[index]
               def to_rules(
                   self,
                   result: icu.UnicodeString,
                   escape_unprintable: bool,
               ) -> icu.UnicodeString:
                   result.truncate(0)
                   for tl in self._trans:
                       if len(result) > 0:
                           result.append("\n")
                       result.append("::")
                       tid = tl.get_id()
                       if not escape_unprintable:
                           result.append(tid)
                       else:
                           tmp = str(tid)
                           result.append(tmp.encode("unicode_escape").decode())
                       result.append(";")
                   return result

           tl = CompoundTransliterator("[:Latin:]; NFKD; Lower; Latin-Katakana;", icu.UTRANS_FORWARD)
           text = icu.UnicodeString("Natsume Sōseki")
           tl.transliterate(text)

      For more information, see the ICU User Guide: `General Transforms
      <https://unicode-org.github.io/icu/userguide/transforms/general/>`__ and
      the C++ API reference: `icu::Transliterator
      <https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1Transliterator.html#details>`__.
      )doc");

  //
  // class icu::Transliterator::Token
  //
  // py::class_<Transliterator::Token>(tl, "Token");

  //
  // class icu::Transliterator
  //
  tl.def(py::init([](const icupy::UnicodeStringVariant &id,
                     std::optional<UnicodeFilter *> &adopted_filter) {
           return std::make_unique<PyTransliterator>(
               icupy::to_unistr(id),
               adopted_filter ? (*adopted_filter)->clone() : nullptr);
         }),
         py::arg("id"), py::arg("adopted_filter") = std::nullopt, R"doc(
      Initialize a ``Transliterator`` instance with the specified ID and
      filter.

      Any character for which ``adopted_filter.contains()`` returns ``False``
      will not be altered by this transliterator.

      If *adopted_filter* is ``None``, no filtering is applied.

      .. note::

         An instance of :class:`UnicodeFilter` will be copied: changes made to
         the filter object after passing it to ``Transliterator`` will not be
         reflected.

      .. seealso::

         :meth:`.adopt_filter`
      )doc")
      .def(py::init([](const PyTransliterator &other) {
             return std::make_unique<PyTransliterator>(other);
           }),
           py::arg("other"), R"doc(
      Initialize a ``Transliterator`` instance from another ``Transliterator``.
      )doc");

  tl.def("__copy__", &Transliterator::clone,
         R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  tl.def(
      "__deepcopy__",
      [](const Transliterator &self, py::dict & /* memo */) {
        return self.clone();
      },
      py::arg("memo"),
      R"doc(
      Return a copy of this instance.

      This is equivalent to calling :meth:`.clone`.
      )doc");

  tl.def(
      "adopt_filter",
      [](Transliterator &self, std::optional<UnicodeFilter *> &adopted_filter) {
        self.adoptFilter(adopted_filter ? (*adopted_filter)->clone() : nullptr);
      },
      py::arg("adopted_filter"), R"doc(
      Change the filter used by this transliterator.

      Any character for which ``adopted_filter.contains()`` returns ``False``
      will not be altered by this transliterator.

      If *adopted_filter* is ``None``, no filtering is applied.

      .. note::

         An instance of :class:`UnicodeFilter` will be copied: changes made to
         the filter object after passing it to ``Transliterator`` will not be
         reflected.

      .. seealso::

         :meth:`.get_filter`
         :meth:`.orphan_filter`
      )doc");

  tl.def("clone", &Transliterator::clone,
         R"doc(
      Return a copy of this instance.

      .. seealso::

         :meth:`.__copy__`
         :meth:`.__deepcopy__`
      )doc");

  tl.def_static("count_available_sources",
                &Transliterator::countAvailableSources, R"doc(
      Return the number of registered source specifiers.

      .. seealso::

         :meth:`.get_available_source`
      )doc");

  tl.def_static(
      "count_available_targets",
      [](const icupy::UnicodeStringVariant &source) {
        return Transliterator::countAvailableTargets(icupy::to_unistr(source));
      },
      py::arg("source"), R"doc(
      Return the number of registered target specifiers for the specified
      source specifier.

      .. seealso::

         :meth:`.get_available_target`
      )doc");

  tl.def_static(
      "count_available_variants",
      [](const icupy::UnicodeStringVariant &source,
         const icupy::UnicodeStringVariant &target) {
        return Transliterator::countAvailableVariants(icupy::to_unistr(source),
                                                      icupy::to_unistr(target));
      },
      py::arg("source"), py::arg("target"), R"doc(
      Return the number of registered variant specifiers for the specified
      source-target pair specifiers.

      .. seealso::

         :meth:`.get_available_variant`
      )doc");

  tl.def("count_elements", &Transliterator::countElements, R"doc(
      Return the number of elements that make up this transliterator.

      .. seealso::

         :meth:`.get_element`

      .. rubric:: Example

      .. code-block:: python

         >>> from icupy import icu
         >>> tl = icu.Transliterator.create_instance("NFD;Jamo-Latin;Latin-Greek", icu.UTRANS_FORWARD)
         >>> tl.count_elements()
         3
      )doc");

  tl.def_static(
      "_create_basic_instance",
      [](const icupy::UnicodeStringVariant &id,
         std::optional<const UnicodeString *> &canon)
          -> std::optional<Transliterator *> {
        return PyTransliterator::createBasicInstance(icupy::to_unistr(id),
                                                     canon.value_or(nullptr));
      },
      py::arg("id"), py::arg("canon") = std::nullopt, R"doc(
      Create a new transliterator instance with the specified basic ID and
      canonical ID.

      *id* must contain only the forward direction source, target, and variant.

      .. important::

         *canon* must outlive the returned transliterator object.
      )doc");

  tl.def_static(
      "create_from_rules",
      [](const icupy::UnicodeStringVariant &id,
         const icupy::UnicodeStringVariant &rules, UTransDirection dir,
         UParseError &parse_error) {
        ErrorCode error_code;
        auto result = Transliterator::createFromRules(
            icupy::to_unistr(id), icupy::to_unistr(rules), dir, parse_error,
            error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::arg("id"), py::arg("rules"), py::arg("dir"), py::arg("parse_error"),
      R"doc(
      Create a new transliterator instance with the specified rule string.

      If the rule string contains only rules, it is a rule-based
      transliterator;
      if it contains an ID block, it is a composite transliterator; and
      if it contains an ID block that is parsed as empty for the specified
      direction, it is a null transliterator.

      .. seealso::

         :meth:`.to_rules`
      )doc");

  tl.def_static(
        "create_instance",
        [](const icupy::UnicodeStringVariant &id, UTransDirection dir) {
          ErrorCode error_code;
          auto result = Transliterator::createInstance(icupy::to_unistr(id),
                                                       dir, error_code);
          if (error_code.isFailure()) {
            throw icupy::ICUError(error_code);
          }
          return result;
        },
        py::arg("id"), py::arg("dir"), R"doc(
      Create a new transliterator instance with the specified ID.

      .. seealso::

          :meth:`.get_available_ids`
      )doc")
      .def_static(
          "create_instance",
          [](const icupy::UnicodeStringVariant &id, UTransDirection dir,
             UParseError &parse_error) {
            ErrorCode error_code;
            auto result = Transliterator::createInstance(
                icupy::to_unistr(id), dir, parse_error, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
            return result;
          },
          py::arg("id"), py::arg("dir"), py::arg("parse_error"), R"doc(
      Create a new transliterator instance with the specified ID.

      If an error occurs while parsing the rules string, the offset into the
      rules string at which the error occurred will be saved into the
      :class:`UParseError`.

      .. seealso::

          :meth:`.get_available_ids`
      )doc");

  tl.def(
      "create_inverse",
      [](const Transliterator &self) {
        ErrorCode error_code;
        auto result = self.createInverse(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return the inverse of this transliterator.
      )doc");

  tl.def(
      "filtered_transliterate",
      [](const Transliterator &self, Replaceable &text, UTransPosition &index,
         py::bool_ incremental) {
        self.filteredTransliterate(text, index, incremental);
      },
      py::arg("text"), py::arg("index"), py::arg("incremental"), R"doc(
      Translate a text substring based on the specified index, taking the
      filter into account.

      This method is intended for subclasses that require the task to be
      delegated to another translator.

      .. seealso::

         :meth:`._handle_transliterate`
         :meth:`.transliterate`
      )doc");

  tl.def("finish_transliteration", &Transliterator::finishTransliteration,
         py::arg("text"), py::arg("index"), R"doc(
      Finish any pending transliterations that were waiting for more
      characters.

      The client should call this method last, after calling the
      :meth:`.transliterate` method one or more times.

      .. seealso::

         :meth:`.transliterate`
      )doc");

  tl.def_static(
      "get_available_ids",
      []() {
        ErrorCode error_code;
        auto result = Transliterator::getAvailableIDs(error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      R"doc(
      Return all available IDs, including user-registered IDs.
      )doc");

  tl.def_static("get_available_source", &Transliterator::getAvailableSource,
                py::arg("index"), py::arg("result"), R"doc(
      Return the registered source specifier for the specified index.

      .. seealso::

         :meth:`.count_available_sources`
      )doc");

  tl.def_static(
      "get_available_target",
      [](int32_t index, const icupy::UnicodeStringVariant &source,
         UnicodeString &result) -> UnicodeString & {
        return Transliterator::getAvailableTarget(
            index, icupy::to_unistr(source), result);
      },
      py::arg("index"), py::arg("source"), py::arg("result"), R"doc(
      Return the registered target specifier for the specified index and source
      specifier.

      .. seealso::

         :meth:`.count_available_targets`
      )doc");

  tl.def_static(
      "get_available_variant",
      [](int32_t index, const icupy::UnicodeStringVariant &source,
         const icupy::UnicodeStringVariant &target,
         UnicodeString &result) -> UnicodeString & {
        return Transliterator::getAvailableVariant(
            index, icupy::to_unistr(source), icupy::to_unistr(target), result);
      },
      py::arg("index"), py::arg("source"), py::arg("target"), py::arg("result"),
      R"doc(
      Return the registered variant specifier for the specified index, source
      specifier, and target specifier.

      .. seealso::

         :meth:`.count_available_variants`
      )doc");

  tl.def_static(
        "get_display_name",
        [](const icupy::UnicodeStringVariant &id,
           const icupy::LocaleVariant &in_locale,
           UnicodeString &result) -> UnicodeString & {
          return Transliterator::getDisplayName(
              icupy::to_unistr(id), icupy::to_locale(in_locale), result);
        },
        py::arg("id"), py::arg("in_locale"), py::arg("result"), R"doc(
      Copy *id* to *result* in a format suitable for display in the locale
      specified by *in_locale*.
      )doc")
      .def_static(
          "get_display_name",
          [](const icupy::UnicodeStringVariant &id,
             UnicodeString &result) -> UnicodeString & {
            return Transliterator::getDisplayName(icupy::to_unistr(id), result);
          },
          py::arg("id"), py::arg("result"), R"doc(
      Copy *id* to *result* in a format suitable for display in the default
      locale.
      )doc");

  tl.def(
      "get_element",
      [](const Transliterator &self, int32_t index) -> const Transliterator & {
        ErrorCode error_code;
        auto &result = self.getElement(index, error_code);
        if (error_code.isFailure()) {
          throw icupy::ICUError(error_code);
        }
        return result;
      },
      py::return_value_policy::reference, py::arg("index"), R"doc(
      Return the element at the specified index.

      .. seealso::

         :meth:`.count_elements`
      )doc");

  tl.def(
      "get_filter",
      [](const Transliterator &self) -> std::optional<const UnicodeFilter *> {
        return self.getFilter();
      },
      py::return_value_policy::reference, R"doc(
      Return the filter used by this transliterator, or ``None`` if no filter
      is applied.

      .. seealso::

         :meth:`.adopt_filter`
         :meth:`.orphan_filter`
      )doc");

  tl.def("get_id", &Transliterator::getID, R"doc(
      Return a programmatic identifier for this transliterator.

      .. seealso::

         :meth:`._set_id`
      )doc");

  tl.def("get_maximum_context_length", &Transliterator::getMaximumContextLength,
         R"doc(
      Return the maximum context length required by this transliterator.

      .. note::

         The default implementation provided by transliterator returns 0.
         Subclasses that use the preceding context should override this method
         to return the correct value.

      .. seealso::

         :meth:`._set_maximum_context_length`
      )doc");

  tl.def("get_source_set", &Transliterator::getSourceSet, py::arg("result"),
         R"doc(
      Return the set of all characters in the input text that may be modified
      by this transliterator.

      .. note::

         The default implementation returns the empty set.
         Subclasses can override :meth:`.handle_get_source_set` as needed to
         return a more precise result.
         In any case, the returned results is an approximation and is intended
         for use in tests, tools, or utilities.

      .. seealso::

         :meth:`.get_target_set`
         :meth:`.handle_get_source_set`
      )doc");

  tl.def("get_target_set", &Transliterator::getTargetSet, py::arg("result"),
         R"doc(
      Return the set of all characters that this transliterator may generate as
      replacement text.

      .. note::

         The default implementation returns the empty set.
         Subclasses can override this method as needed to return a more
         precise results.
         In any case, the returned results are approximate values and are
         intended for use in tests, tools, or utilities that require such meta
         information.

      .. seealso::

         :meth:`.get_source_set`
      )doc");

  tl.def("handle_get_source_set", &Transliterator::handleGetSourceSet,
         py::arg("result"), R"doc(
      Framework method that returns the set of all characters that may be
      modified in the input text by this transliterator, ignoring the effect of
      this object's filter.

      .. note::

         In the default implementation, this method is called from
         :meth:`.get_source_set` and returns the empty set. Subclasses that
         wish to implement this method should override it.

      .. seealso::

         :meth:`.get_source_set`
      )doc");

  tl.def(
      "_handle_transliterate",
      [](const PyTransliterator &self, Replaceable &text, UTransPosition &pos,
         py::bool_ incremental) {
        self.handleTransliterate(text, pos, incremental);
      },
      py::arg("text"), py::arg("pos"), py::arg("incremental"), R"doc(
      Framework method that performs the actual transliteration in subclasses.

      .. important::

         Subclasses must implement the abstract method
         :meth:`._handle_transliterate`, which defines their own
         transliteration algorithm.

      .. note::

         Directly calling this method will transliterate the range
         [*pos.start*, *pos.limit*) without applying the filter. In end-user
         code, call :meth:`.transliterate` instead of this method. In subclass
         code or wrapping transliterators, call
         :meth:`.filtered_transliterate` instead of this method.

      This method handles both incremental and non-incremental transliteration.
      Let *original_start* denote the value of *pos.start* upon entry.

      - If *incremental* is ``False``, this method should transliterate all
        characters between *pos.start* and *pos.limit*. Upon return,
        *pos.start* must equal *pos.limit*.
      - If *incremental* is ``True``, this method should transliterate all
        characters between *pos.start* and *pos.limit* that can be
        unambiguously transliterated, regardless of any future text insertions
        at *pos.limit*. Upon return, *pos.start* should fall within the range
        [*original_start*, *pos.limit*), positioned such that characters in
        [*original_start*, *pos.start*) will not be modified by this
        transliterator in the future, and characters in
        [*pos.start*, *pos.limit*) remain unchanged.

      Implementations of this method must also observe the following invariants:

      - *pos.limit* and *pos.context_limit* must be updated to reflect any
        changes in the length of text between *pos.start* and *pos.limit*.
        The difference *pos.context_limit* − *pos.limit* must remain constant.
      - *pos.context_start* must not change.
      - Upon return, neither *pos.start* nor *pos.limit* may be less than
        *original_start*.
      - Text before *original_start* and text after *pos.limit* must not be
        modified.
      - Text before *pos.context_start* and text after *pos.context_limit* must
        be ignored.
      )doc");

  tl.def(
      "orphan_filter",
      [](Transliterator &self) -> std::optional<UnicodeFilter *> {
        return self.orphanFilter();
      },
      py::return_value_policy::reference, R"doc(
      Return the filter used by this transliterator and disassociate it from
      this transliterator.

      .. seealso::

         :meth:`.adopt_filter`
         :meth:`.get_filter`
      )doc");

  tl.def_static(
      "register_alias",
      [](const icupy::UnicodeStringVariant &alias_id,
         const icupy::UnicodeStringVariant &real_id) {
        Transliterator::registerAlias(icupy::to_unistr(alias_id),
                                      icupy::to_unistr(real_id));
      },
      py::arg("alias_id"), py::arg("real_id"), R"doc(
      Register an alias ID for a real ID.

      This is typically used to create shorter, more memorable aliases for long
      compound IDs.

      .. seealso::

         :meth:`.create_instance`
      )doc");

  // FIXME: Implement "static void registerFactory(const UnicodeString &id,
  //  Factory factory, Token context)".

  // TODO: Deprecate Transliterator.register_instance().
  tl.def_static(
      "register_instance",
      [](Transliterator *adopted_obj) {
        Transliterator::registerInstance(adopted_obj->clone());
      },
      py::arg("adopted_obj").none(false), R"doc(
      Register a transliterator object.

      .. version-deprecated:: 0.24
         Do not use this method. It may be removed in a future release.
      )doc");

  tl.def(
      "_set_id",
      [](PyTransliterator &self, const icupy::UnicodeStringVariant &id) {
        self.setID(icupy::to_unistr(id));
      },
      py::arg("id"), R"doc(
      Change the ID of this transliterator.

      .. note::

         Subclasses shouldn't do this, unless the underlying script behavior
         has changed.

      .. seealso::

         :meth:`.get_id`
      )doc");

  tl.def("_set_maximum_context_length",
         &PyTransliterator::setMaximumContextLength,
         py::arg("max_context_length"), R"doc(
      Change the maximum context length required by this transliterator.

      .. seealso::

         :meth:`.get_maximum_context_length`
      )doc");

  tl.def(
      "to_rules",
      [](const Transliterator &self, UnicodeString &result,
         py::bool_ escape_unprintable) -> UnicodeString & {
        return self.toRules(result, escape_unprintable);
      },
      py::arg("result"), py::arg("escape_unprintable"), R"doc(
      Return a rules string that can be used to create this transliterator.

      If *escape_unprintable* is ``True``, unprintable characters in the rules
      string will be escaped with Unicode escape sequences.

      .. seealso::

         :meth:`.create_from_rules`
         :meth:`UnicodeMatcher.to_pattern`
      )doc");

  tl.def("transliterate",
         py::overload_cast<Replaceable &>(&Transliterator::transliterate,
                                          py::const_),
         py::arg("text"), R"doc(
      Transliterate the entire text.
      )doc")
      .def("transliterate",
           py::overload_cast<Replaceable &, int32_t, int32_t>(
               &Transliterator::transliterate, py::const_),
           py::arg("text"), py::arg("start"), py::arg("limit"), R"doc(
      Transliterate a portion of the text [*start*, *limit*), and return the
      new ending index.
      )doc")
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text,
             UTransPosition &index,
             const icupy::UnicodeStringVariant &insertion) {
            ErrorCode error_code;
            self.transliterate(text, index, icupy::to_unistr(insertion),
                               error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("index"), py::arg("insertion"), R"doc(
      Transliterate the portion of the text buffer that can be unambiguously
      transliterated, where new text has been inserted via keyboard event for
      example.

      *insertion* will be inserted into text at *index.limit*, advancing
      *index.limit* by *insertion.length()*. Then the transliterator will try
      to transliterate characters of text between *index.start* and
      *index.limit*.
      *index.start* will be advanced to the first character that future calls
      to this method will read.

      This method assumes that future calls may insert additional text into the
      buffer. Consequently, it performs only unambiguous transliterations.
      After the final call to this method, some text may remain
      untransliterated, pending further input to resolve ambiguities.
      To complete these pending transliterations, clients should call
      :meth:`.finish_transliteration` after making the last call to this
      method.

      .. seealso::

         :class:`UTransPosition`
         :meth:`._handle_transliterate`
         :meth:`.finish_transliteration`
      )doc")
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text,
             UTransPosition &index, UChar32 insertion) {
            ErrorCode error_code;
            self.transliterate(text, index, insertion, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("index"), py::arg("insertion"), R"doc(
      Transliterate the portion of the text buffer that can be unambiguously
      transliterated, where a new character has been inserted via keyboard
      event for example.
      )doc")
      .def(
          "transliterate",
          [](const Transliterator &self, Replaceable &text,
             UTransPosition &index) {
            ErrorCode error_code;
            self.transliterate(text, index, error_code);
            if (error_code.isFailure()) {
              throw icupy::ICUError(error_code);
            }
          },
          py::arg("text"), py::arg("index"), R"doc(
      Transliterate the portion of the text buffer that can be unambiguously
      transliterated.
      )doc");

  // TODO: Deprecate Transliterator.register_instance().
  tl.def_static(
      "unregister",
      [](const icupy::UnicodeStringVariant &id) {
        Transliterator::unregister(icupy::to_unistr(id));
      },
      py::arg("id"), R"doc(
      Unregister a transliterator object.

      .. version-deprecated:: 0.24
         Do not use this method. It may be removed in a future release.
      )doc");
}
