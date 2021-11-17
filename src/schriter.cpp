#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/schriter.h>

using namespace icu;

void init_schriter(py::module &m) {
  // icu::ForwardCharacterIterator
  py::class_<ForwardCharacterIterator, UObject> fci(m, "ForwardCharacterIterator");

  py::enum_<decltype(ForwardCharacterIterator::DONE)>(fci, "ForwardCharacterIterator", py::arithmetic())
      .value("DONE", ForwardCharacterIterator::DONE,
             "Value returned by most of *ForwardCharacterIterator*'s functions when the iterator has reached the "
             "limits of its iteration.")
      .export_values();

  // icu::CharacterIterator
  py::class_<CharacterIterator, ForwardCharacterIterator> ci(m, "CharacterIterator");

  py::enum_<CharacterIterator::EOrigin>(ci, "EOrigin", py::arithmetic(),
                                        "Origin enumeration for the *move()* and *move32()* functions.")
      .value("START", CharacterIterator::kStart)
      .value("CURRENT", CharacterIterator::kCurrent)
      .value("END", CharacterIterator::kEnd)
      .export_values();

  ci.def("__copy__", &CharacterIterator::clone)
      .def(
          "__deepcopy__", [](const CharacterIterator &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def("__len__", &CharacterIterator::getLength)
      .def("__str__", [](CharacterIterator &self) {
        UnicodeString dest;
        self.getText(dest);
        std::string result;
        dest.toUTF8String(result);
        return result;
      });
  ci.def("end_index", &CharacterIterator::endIndex);
  ci.def("first32_post_inc", &CharacterIterator::first32PostInc);
  ci.def("first_post_inc", [](CharacterIterator &self) -> uint16_t { return self.firstPostInc(); });
  ci.def("get_index", &CharacterIterator::getIndex);
  ci.def("get_length", &CharacterIterator::getLength);
  ci.def("set_to_end", &CharacterIterator::setToEnd);
  ci.def("set_to_start", &CharacterIterator::setToStart);
  ci.def("start_index", &CharacterIterator::startIndex);

  // icu::UCharCharacterIterator
  py::class_<UCharCharacterIterator, CharacterIterator> uci(m, "UCharCharacterIterator");
  /*
  uci.def(py::init<ConstChar16Ptr, int32_t>(), py::arg("text_ptr"), py::arg("length"))
      .def(py::init<ConstChar16Ptr, int32_t, int32_t>(), py::arg("text_ptr"), py::arg("length"), py::arg("position"))
      .def(py::init<ConstChar16Ptr, int32_t, int32_t, int32_t, int32_t>(), py::arg("text_ptr"), py::arg("length"),
           py::arg("text_begin"), py::arg("text_end"), py::arg("position"))
      .def(py::init<const UCharCharacterIterator &>(), py::arg("that"));
  uci.def("set_text", &UCharCharacterIterator::setText, py::arg("new_text"), py::arg("new_text_length"));
  */
  uci.def(py::self != py::self, py::arg("other")).def(py::self == py::self, py::arg("other"));
  uci.def("__copy__", &UCharCharacterIterator::clone)
      .def(
          "__deepcopy__", [](const UCharCharacterIterator &self, py::dict) { return self.clone(); }, py::arg("memo"));
  uci.def("clone", &UCharCharacterIterator::clone);
  uci.def("current", [](const UCharCharacterIterator &self) -> uint16_t { return self.current(); });
  uci.def("current32", &UCharCharacterIterator::current32);
  uci.def("first", [](UCharCharacterIterator &self) -> uint16_t { return self.first(); });
  uci.def("first32", &UCharCharacterIterator::first32);
  uci.def("get_text", &UCharCharacterIterator::getText, py::arg("result"));
  uci.def("has_next", &UCharCharacterIterator::hasNext);
  uci.def("has_previous", &UCharCharacterIterator::hasPrevious);
  uci.def("hash_code", &UCharCharacterIterator::hashCode);
  uci.def("last", [](UCharCharacterIterator &self) -> uint16_t { return self.last(); });
  uci.def("last32", &UCharCharacterIterator::last32);
  uci.def("move", &UCharCharacterIterator::move, py::arg("delta"), py::arg("origin"));
  uci.def("move32", &UCharCharacterIterator::move32, py::arg("delta"), py::arg("origin"));
  uci.def("next", [](UCharCharacterIterator &self) -> uint16_t { return self.next(); });
  uci.def("next_post_inc", [](UCharCharacterIterator &self) -> uint16_t { return self.nextPostInc(); });
  uci.def("next32", &UCharCharacterIterator::next32);
  uci.def("next32_post_inc", &UCharCharacterIterator::next32PostInc);
  uci.def("previous", [](UCharCharacterIterator &self) -> uint16_t { return self.previous(); });
  uci.def("previous32", &UCharCharacterIterator::previous32);
  uci.def(
      "set_index", [](UCharCharacterIterator &self, int32_t position) -> uint16_t { return self.setIndex(position); },
      py::arg("position"));
  uci.def("set_index32", &UCharCharacterIterator::setIndex32, py::arg("position"));

  // icu::StringCharacterIterator
  py::class_<StringCharacterIterator, UCharCharacterIterator> sci(m, "StringCharacterIterator");
  sci.def(py::init<const UnicodeString &>(), py::arg("text_str"))
      .def(
          // const char16_t *text_str -> const UnicodeString &text_str
          py::init([](const char16_t *text_str) { return std::make_unique<StringCharacterIterator>(text_str); }),
          py::arg("text_str"))
      .def(py::init<const UnicodeString &, int32_t>(), py::arg("text_str"), py::arg("text_pos"))
      .def(
          // const char16_t *text_str -> const UnicodeString &text_str
          py::init([](const char16_t *text_str, int32_t text_pos) {
            return std::make_unique<StringCharacterIterator>(text_str, text_pos);
          }),
          py::arg("text_str"), py::arg("text_pos"))
      .def(py::init<const UnicodeString &, int32_t, int32_t, int32_t>(), py::arg("text_str"), py::arg("text_begin"),
           py::arg("text_end"), py::arg("text_pos"))
      .def(
          // const char16_t *text_str -> const UnicodeString &text_str
          py::init([](const char16_t *text_str, int32_t text_begin, int32_t text_end, int32_t text_pos) {
            return std::make_unique<StringCharacterIterator>(text_str, text_begin, text_end, text_pos);
          }),
          py::arg("text_str"), py::arg("text_begin"), py::arg("text_end"), py::arg("text_pos"))
      .def(py::init<const StringCharacterIterator &>(), py::arg("that"))
      .def(py::self != py::self, py::arg("other"))
      .def(py::self == py::self, py::arg("other"));
  sci.def("__copy__", &StringCharacterIterator::clone)
      .def(
          "__deepcopy__", [](const StringCharacterIterator &self, py::dict) { return self.clone(); }, py::arg("memo"))
      .def("__iter__",
           [](StringCharacterIterator &self) -> StringCharacterIterator & {
             self.first32();
             return self;
           })
      .def("__next__",
           [](StringCharacterIterator &self) {
             if (self.getIndex() == self.endIndex()) {
               throw py::stop_iteration();
             }
             return static_cast<char32_t>(self.next32PostInc());
           })
      .def("__reversed__", [](StringCharacterIterator &self) {
        std::vector<char32_t> result;
        for (auto c = self.last32(); c != CharacterIterator::DONE; c = self.previous32()) {
          result.push_back(c);
        }
        return result;
      });
  sci.def("clone", &StringCharacterIterator::clone);
  sci.def("get_text", &StringCharacterIterator::getText, py::arg("result"));
  sci.def("set_text", py::overload_cast<const UnicodeString &>(&StringCharacterIterator::setText), py::arg("new_text"))
      .def(
          // const char16_t *new_text -> const UnicodeString &new_text
          "set_text", [](StringCharacterIterator &self, const char16_t *new_text) { self.setText(new_text); },
          py::arg("new_text"));
}
