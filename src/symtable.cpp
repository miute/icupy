#include "main.hpp"
#include <pybind11/stl.h>
#include <unicode/parsepos.h>
#include <unicode/symtable.h>
#include <unicode/unifunct.h>

using namespace icu;

class PySymbolTable : public SymbolTable {
public:
  PySymbolTable() = default;

  ~PySymbolTable() = default;

  const UnicodeString *lookup(const UnicodeString &s) const override {
    PYBIND11_OVERRIDE_PURE_NAME(const UnicodeString *, SymbolTable, "lookup",
                                lookup, s);
  }

  const UnicodeFunctor *lookupMatcher(UChar32 ch) const override {
    PYBIND11_OVERRIDE_PURE_NAME(const UnicodeFunctor *, SymbolTable,
                                "lookup_matcher", lookupMatcher, ch);
  }

  UnicodeString parseReference(const UnicodeString &s, ParsePosition &pos,
                               int32_t limit) const override {
    PYBIND11_OVERRIDE_PURE_NAME(UnicodeString, SymbolTable, "parse_reference",
                                parseReference, s, pos, limit);
  }
};

void init_symtable(py::module &m) {
  //
  // class icu::SymbolTable
  //
  py::class_<SymbolTable, PySymbolTable> st(m, "SymbolTable", R"doc(
      Abstract base class for symbol tables used in :class:`UnicodeSet` pattern
      parsing.

      ``SymbolTable`` provides two kinds of lookup functions: symbol name to
      value, and character to :class:`UnicodeMatcher`. It also defines parsing
      rules for symbol references beginning with :attr:`SYMBOL_REF`.

      .. important::

         Subclasses must implement the following abstract methods:
         :meth:`.lookup`,
         :meth:`.lookup_matcher`,
         and :meth:`.parse_reference`.

      .. seealso::

         :meth:`UnicodeSet.__init__`
         :meth:`UnicodeSet.apply_pattern`

      .. rubric:: Example

      .. code-block:: python

         from icupy import icu
         class TestSymbolTable(icu.SymbolTable):
             def __init__(self) -> None:
                 super().__init__()
                 self._symbols: dict[icu.UnicodeString, icu.UnicodeString] = {}
             def add(self, var: str, value: str) -> None:
                 self._symbols[icu.UnicodeString(var)] = icu.UnicodeString(value)
             def lookup(self, s: icu.UnicodeString) -> icu.UnicodeString | None:
                 return self._symbols.get(s)
             def lookup_matcher(self, ch: int) -> icu.UnicodeFunctor | None:
                 _ = ch
                 return None
             def parse_reference(
                 self, text: icu.UnicodeString, pos: icu.ParsePosition, limit: int
             ) -> icu.UnicodeString:
                 start = pos.get_index()
                 i = start
                 result = icu.UnicodeString()
                 while i < limit:
                     c = text.char_at(i)
                     if (i == start and not icu.u_is_id_start(c)) or not icu.u_is_id_part(c):
                         break
                     i += 1
                 if i == start:
                     return result
                 pos.set_index(i)
                 text.extract_between(start, i, result)
                 return result

         symbols = TestSymbolTable()
         symbols.add("VOWEL", "[aeiouAEIOU]")
         symbols.add("DIGIT", "[0-9]")

         pos = icu.ParsePosition()
         us = icu.UnicodeSet("[$VOWEL $DIGIT xyz]", pos, icu.USET_IGNORE_SPACE, symbols)
         us.contains("a")  # True
         us.contains("1")  # True
         us.contains("x")  # True
         us.contains("b")  # False

      .. rubric:: Attributes

      .. autoattribute:: SYMBOL_REF

         The prefix character code for symbolic names.

         When the parser encounters this character, it calls
         :meth:`.parse_reference` with the position immediately following
         ``SYMBOL_REF`` as an argument.
      )doc");

  st.def_property_readonly_static("SYMBOL_REF",
                                  [](const py::object & /* self */) -> int32_t {
                                    return SymbolTable::SYMBOL_REF;
                                  });

  st.def(py::init<>(), R"doc(
      Initialize a ``SymbolTable`` instance.

      .. important::

         Only available in subclasses.
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  st.def(
      "lookup",
      [](const SymbolTable &self, const UnicodeString &s)
          -> std::optional<const UnicodeString *> { return self.lookup(s); },
      py::arg("s"), R"doc(
      Lookup the characters associated with the specified string *s* and return
      it; otherwise, return ``None``.

      .. important::

         The returned ``UnicodeString`` object must outlive the symbol table
         object.
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  st.def(
      "lookup_matcher",
      [](const SymbolTable &self,
         UChar32 ch) -> std::optional<const UnicodeFunctor *> {
        return self.lookupMatcher(ch);
      },
      py::arg("ch"), R"doc(
      Lookup the :class:`UnicodeMatcher` associated with the specified
      character *ch* and return it; otherwise, return ``None``.

      *ch* is a 32-bit code point from 0 to 0x10FFFF inclusive.

      .. important::

         The returned ``UnicodeFunctor`` object must outlive the symbol table
         object.
      )doc");

  // for docstring only
  // pybind11 throws RuntimeError if not implemented in Python subclass
  st.def("parse_reference", &SymbolTable::parseReference, py::arg("text"),
         py::arg("pos"), py::arg("limit"), R"doc(
      Parse a symbol reference name from the specified substring
      *text[pos.get_index(), limit]* and return the parsed name; otherwise,
      return the empty string.

      The index of *pos* is updated to the index following the last parsed
      character after parsing. If parsing fails, *pos* remains unchanged.
      )doc");
}
