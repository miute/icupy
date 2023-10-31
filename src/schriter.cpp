#include "main.hpp"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <unicode/schriter.h>

using namespace icu;

void init_schriter(py::module &m) {
  //
  // icu::ForwardCharacterIterator
  //
  py::class_<ForwardCharacterIterator, UObject> fci(m, "ForwardCharacterIterator");

  py::enum_<decltype(ForwardCharacterIterator::DONE)>(fci, "ForwardCharacterIterator", py::arithmetic())
      .value("DONE", ForwardCharacterIterator::DONE,
             "Value returned by most of *ForwardCharacterIterator*'s functions when the iterator has reached the "
             "limits of its iteration.")
      .export_values();

  fci.def(
      "__eq__",
      [](const ForwardCharacterIterator &self, const ForwardCharacterIterator &other) { return self == other; },
      py::is_operator(), py::arg("other"));

  fci.def(
      "__ne__",
      [](const ForwardCharacterIterator &self, const ForwardCharacterIterator &other) { return self != other; },
      py::is_operator(), py::arg("other"));

  fci.def("hash_code", &ForwardCharacterIterator::hashCode);

  fci.def("has_next", [](ForwardCharacterIterator &self) -> py::bool_ { return self.hasNext(); });

  fci.def("next32_post_inc", &ForwardCharacterIterator::next32PostInc);

  fci.def("next_post_inc", [](ForwardCharacterIterator &self) -> uint16_t { return self.nextPostInc(); });

  //
  // icu::CharacterIterator
  //
  py::class_<CharacterIterator, ForwardCharacterIterator> ci(m, "CharacterIterator");

  //
  // icu::CharacterIterator::EOrigin
  //
  py::enum_<CharacterIterator::EOrigin>(ci, "EOrigin", py::arithmetic(),
                                        "Origin enumeration for the *move()* and *move32()* functions.")
      .value("START", CharacterIterator::kStart)
      .value("CURRENT", CharacterIterator::kCurrent)
      .value("END", CharacterIterator::kEnd)
      .export_values();

  //
  // icu::CharacterIterator
  //
  ci.def("__len__", &CharacterIterator::getLength);

  ci.def("__str__", [](CharacterIterator &self) {
    UnicodeString dest;
    self.getText(dest);
    std::string result;
    dest.toUTF8String(result);
    return result;
  });

  ci.def("clone", &CharacterIterator::clone);

  ci.def("current", [](const CharacterIterator &self) -> uint16_t { return self.current(); });

  ci.def("current32", &CharacterIterator::current32);

  ci.def("end_index", &CharacterIterator::endIndex);

  ci.def("first", [](CharacterIterator &self) -> uint16_t { return self.first(); });

  ci.def("first32", &CharacterIterator::first32);

  ci.def("first32_post_inc", &CharacterIterator::first32PostInc);

  ci.def("first_post_inc", [](CharacterIterator &self) -> uint16_t { return self.firstPostInc(); });

  ci.def("get_index", &CharacterIterator::getIndex);

  ci.def("get_length", &CharacterIterator::getLength);

  ci.def("get_text", &CharacterIterator::getText, py::arg("result"));

  ci.def("has_previous", [](CharacterIterator &self) -> py::bool_ { return self.hasPrevious(); });

  ci.def("last", [](CharacterIterator &self) -> uint16_t { return self.last(); });

  ci.def("last32", &CharacterIterator::last32);

  ci.def("move", &CharacterIterator::move, py::arg("delta"), py::arg("origin"));

  ci.def("move32", &CharacterIterator::move32, py::arg("delta"), py::arg("origin"));

  ci.def("next", [](CharacterIterator &self) -> uint16_t { return self.next(); });

  ci.def("next32", &CharacterIterator::next32);

  ci.def("previous", [](CharacterIterator &self) -> uint16_t { return self.previous(); });

  ci.def("previous32", &CharacterIterator::previous32);

  ci.def(
      "set_index", [](CharacterIterator &self, int32_t position) -> uint16_t { return self.setIndex(position); },
      py::arg("position"));

  ci.def("set_index32", &CharacterIterator::setIndex32, py::arg("position"));

  ci.def("set_to_end", &CharacterIterator::setToEnd);

  ci.def("set_to_start", &CharacterIterator::setToStart);

  ci.def("start_index", &CharacterIterator::startIndex);

  //
  // icu::UCharCharacterIterator
  //
  py::class_<UCharCharacterIterator, CharacterIterator> uci(m, "UCharCharacterIterator");

  /*
  uci.def(py::init<ConstChar16Ptr, int32_t>(), py::arg("text_ptr"), py::arg("length"))
      .def(py::init<ConstChar16Ptr, int32_t, int32_t>(), py::arg("text_ptr"), py::arg("length"), py::arg("position"))
      .def(py::init<ConstChar16Ptr, int32_t, int32_t, int32_t, int32_t>(), py::arg("text_ptr"), py::arg("length"),
           py::arg("text_begin"), py::arg("text_end"), py::arg("position"))
      .def(py::init<const UCharCharacterIterator &>(), py::arg("other"));
  uci.def("set_text", &UCharCharacterIterator::setText, py::arg("new_text"), py::arg("new_text_length"));
  */

  uci.def("__copy__", &UCharCharacterIterator::clone);

  uci.def(
      "__deepcopy__", [](const UCharCharacterIterator &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  uci.def("clone", &UCharCharacterIterator::clone);

  //
  // icu::StringCharacterIterator
  //
  py::class_<StringCharacterIterator, UCharCharacterIterator> sci(m, "StringCharacterIterator");

  sci.def(py::init([](const icupy::UnicodeStringVariant &text_str) {
            return std::make_unique<StringCharacterIterator>(icupy::to_unistr(text_str));
          }),
          py::arg("text_str"))
      .def(py::init([](const icupy::UnicodeStringVariant &text_str, int32_t text_pos) {
             return std::make_unique<StringCharacterIterator>(icupy::to_unistr(text_str), text_pos);
           }),
           py::arg("text_str"), py::arg("text_pos"))
      .def(py::init(
               [](const icupy::UnicodeStringVariant &text_str, int32_t text_begin, int32_t text_end, int32_t text_pos) {
                 return std::make_unique<StringCharacterIterator>(icupy::to_unistr(text_str), text_begin, text_end,
                                                                  text_pos);
               }),
           py::arg("text_str"), py::arg("text_begin"), py::arg("text_end"), py::arg("text_pos"))
      .def(py::init<const StringCharacterIterator &>(), py::arg("other"));

  sci.def("__copy__", &StringCharacterIterator::clone);

  sci.def(
      "__deepcopy__", [](const StringCharacterIterator &self, py::dict &) { return self.clone(); }, py::arg("memo"));

  sci.def("__iter__", [](StringCharacterIterator &self) -> StringCharacterIterator & {
    self.first32();
    return self;
  });

  sci.def("__next__", [](StringCharacterIterator &self) {
    if (self.getIndex() == self.endIndex()) {
      throw py::stop_iteration();
    }
    return static_cast<char32_t>(self.next32PostInc());
  });

  sci.def("__reversed__", [](StringCharacterIterator &self) {
    std::vector<char32_t> result;
    for (auto c = self.last32(); c != CharacterIterator::DONE; c = self.previous32()) {
      result.push_back(c);
    }
    return result;
  });

  sci.def("clone", &StringCharacterIterator::clone);

  sci.def(
      "set_text",
      [](StringCharacterIterator &self, const icupy::UnicodeStringVariant &new_text) {
        self.setText(icupy::to_unistr(new_text));
      },
      py::arg("new_text"));
}
