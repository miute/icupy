#include "main.hpp"
#include <sstream>
#include <unicode/utrans.h>

void init_utrans(py::module &m) {
  //
  // enum UTransDirection
  //
  py::enum_<UTransDirection>(m, "UTransDirection", py::arithmetic(), R"doc(
Direction constant indicating the direction in a transliterator, e.g., the
forward or reverse rules of a RuleBasedTransliterator.

Specified when a transliterator is opened. An "A-B" transliterator
transliterates A to B when operating in the forward direction, and B to A when
operating in the reverse direction.
      )doc")
      .value("UTRANS_FORWARD", UTRANS_FORWARD, R"doc(
             ``UTRANS_FORWARD`` means from <source> to <target> for a
             transliterator with ID <source>-<target>.

             For a transliterator opened using a rule, it means forward
             direction rules, e.g., "A > B".
             )doc")
      .value("UTRANS_REVERSE", UTRANS_REVERSE, R"doc(
             ``UTRANS_REVERSE`` means from <target> to <source> for a
             transliterator with ID <source>-<target>.

             For a transliterator opened using a rule, it means reverse
             direction rules, e.g., "A < B".
             )doc")
      .export_values();

  //
  // struct UTransPosition
  //
  py::class_<UTransPosition> tp(m, "UTransPosition", R"doc(
      Structure that stores position information during incremental
      transliteration.

      .. note::

         All index values in this structure must be at the boundaries of code
         points. That is, they must not occur between two code units of a
         surrogate pair. If any index splits a surrogate pair, the result is
         undefined.

      .. seealso::

         :class:`Transliterator`
      )doc");

  tp.def(py::init<int32_t, int32_t, int32_t, int32_t>(),
         py::arg("context_start") = 0, py::arg("context_limit") = 0,
         py::arg("start") = 0, py::arg("limit") = 0, R"doc(
      Initialize a ``UTransPosition`` instance with the specified values.

      The values must be in the range:
      *context_start* <= *start* <= *limit* <= *context_limit*.
      )doc");

  tp.def("__repr__", [](const UTransPosition &self) {
    std::stringstream ss;
    ss << "UTransPosition(";
    ss << "context_start=" << self.contextStart;
    ss << ", context_limit=" << self.contextLimit;
    ss << ", start=" << self.start;
    ss << ", limit=" << self.limit;
    ss << ")";
    return ss.str();
  });

  tp.def_readwrite("context_limit", &UTransPosition::contextLimit, R"doc(
      int: Get or set the ending index, exclusive, of the context to be
      considered for a transliteration operation.
      )doc");

  tp.def_readwrite("context_start", &UTransPosition::contextStart, R"doc(
      int: Get or set the beginning index, inclusive, of the context to be
      considered for a transliteration operation.
      )doc");

  tp.def_readwrite("limit", &UTransPosition::limit, R"doc(
      int: Get or set the ending index, exclusive, of the text to be
      transliterated.
      )doc");

  tp.def_readwrite("start", &UTransPosition::start, R"doc(
      int: Get or set the beginning index, inclusive, of the text to be
      transliterated.
      )doc");
}
