#include "main.hpp"
#include <unicode/utf.h>

namespace icupy {

int32_t u16strnlen(const uint16_t *p, int32_t maxlen) {
  int32_t n = 0;
  while (n < maxlen && p[n] != 0) {
    n++;
  }
  return n;
}

} // namespace icupy

void init_utf(py::module &m) {
  //
  // utf.h
  //
  m.def(
      "u_is_bmp", [](uint32_t c) -> py::bool_ { return U_IS_BMP(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a BMP code point (U+0000..U+FFFF);
      ``False`` otherwise.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 78)
  m.def(
      "u_is_code_point",
      [](uint32_t c) -> py::bool_ { return U_IS_CODE_POINT(c); }, py::arg("c"),
      R"doc(
      Return ``True`` if this code point is a valid Unicode code point
      (U+0000..U+10FFFF); ``False`` otherwise.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)

  m.def(
      "u_is_lead", [](uint32_t c) -> py::bool_ { return U_IS_LEAD(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a lead surrogate (U+D800..U+DBFF);
      ``False`` otherwise.
      )doc");

#if (U_ICU_VERSION_MAJOR_NUM >= 78)
  m.def(
      "u_is_scalar_value",
      [](uint32_t c) -> py::bool_ { return U_IS_SCALAR_VALUE(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a Unicode scalar value;
      ``False`` otherwise.

      A Unicode scalar value is a non-surrogate code point.

      Only scalar values can be represented in well-formed UTF-8/16/32.
      )doc");
#endif // (U_ICU_VERSION_MAJOR_NUM >= 78)

  m.def(
      "u_is_supplementary",
      [](uint32_t c) -> py::bool_ { return U_IS_SUPPLEMENTARY(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a supplementary code point
      (U+10000..U+10FFFF); ``False`` otherwise.
      )doc");

  m.def(
      "u_is_surrogate",
      [](uint32_t c) -> py::bool_ { return U_IS_SURROGATE(c); }, py::arg("c"),
      R"doc(
      Return ``True`` if this code point is a surrogate (U+D800..U+DFFF);
      ``False`` otherwise.
      )doc");

  m.def(
      "u_is_surrogate_lead",
      [](uint32_t c) -> py::bool_ { return U_IS_SURROGATE_LEAD(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a lead surrogate;
      ``False`` otherwise.
      )doc");

  m.def(
      "u_is_surrogate_trail",
      [](uint32_t c) -> py::bool_ { return U_IS_SURROGATE_TRAIL(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a trail surrogate;
      ``False`` otherwise.
      )doc");

  m.def(
      "u_is_trail", [](uint32_t c) -> py::bool_ { return U_IS_TRAIL(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a trail surrogate (U+DC00..U+DFFF);
      ``False`` otherwise.
      )doc");

  m.def(
      "u_is_unicode_char",
      [](uint32_t c) -> py::bool_ { return U_IS_UNICODE_CHAR(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a Unicode character;
      ``False`` otherwise.
      )doc");

  m.def(
      "u_is_unicode_nonchar",
      [](uint32_t c) -> py::bool_ { return U_IS_UNICODE_NONCHAR(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code point is a Unicode noncharacter;
      ``False`` otherwise.
      )doc");

  //
  // utf16.h
  //
  m.def(
      "u16_back_1",
      [](const py::buffer &s, int32_t start, int32_t i) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        } else if (start < 0 || start >= info.size) {
          throw py::index_error("start out of range: " + std::to_string(start));
        } else if (i <= 0 || i >= info.size) {
          throw py::index_error("index out of range: " + std::to_string(i));
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        U16_BACK_1(p, start, i);
        return i;
      },
      py::arg("s"), py::arg("start"), py::arg("i"), R"doc(
      Move the string offset from one code point boundary to the previous one,
      and return the new offset.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *start* < *i*.

      See Also:
          :func:`u16_back_n`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_back_n",
      [](const py::buffer &s, int32_t start, int32_t i, int32_t n) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        } else if (start < 0 || start >= info.size) {
          throw py::index_error("start out of range: " + std::to_string(start));
        } else if (i <= 0 || i >= info.size) {
          throw py::index_error("index out of range: " + std::to_string(i));
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        U16_BACK_N(p, start, i, n);
        return i;
      },
      py::arg("s"), py::arg("start"), py::arg("i"), py::arg("n"), R"doc(
      Move the string offset from one code point boundary to the n-th one
      before it, and return the new offset.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *start* < *i*.

      See Also:
          :func:`u16_back_1`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
          :meth:`UnicodeString.move_index32`
      )doc");

  m.def(
      "u16_fwd_1",
      [](const py::buffer &s, int32_t i, int32_t limit) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        if (limit < 0) {
          limit = icupy::u16strnlen(p, info.size);
        }
        if (i < 0 || i + 1 >= info.size || i >= limit) {
          throw py::index_error("index out of range: " + std::to_string(i));
        } else if (limit >= info.size) {
          throw py::index_error("limit out of range: " + std::to_string(limit));
        }
        U16_FWD_1(p, i, limit);
        return i;
      },
      py::arg("s"), py::arg("i"), py::arg("limit"), R"doc(
      Advance the string offset from one code point boundary to the next,
      and return the new offset.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *i* < *limit*.

      *limit* can be negative for a NULL-terminated string.

      See Also:
          :func:`u16_fwd_n`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_fwd_n",
      [](const py::buffer &s, int32_t i, int32_t limit, int32_t n) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        if (limit < 0) {
          limit = icupy::u16strnlen(p, info.size);
        }
        if (i < 0 || i + 1 >= info.size || i >= limit) {
          throw py::index_error("index out of range: " + std::to_string(i));
        } else if (limit >= info.size) {
          throw py::index_error("limit out of range: " + std::to_string(limit));
        }
        U16_FWD_N(p, i, limit, n);
        return i;
      },
      py::arg("s"), py::arg("i"), py::arg("limit"), py::arg("n"), R"doc(
      Advance the string offset from one code point boundary to the n-th next
      one, and return the new offset.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *i* < *limit*.

      *limit* can be negative for a NULL-terminated string.

      See Also:
          :func:`u16_fwd_1`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
          :meth:`UnicodeString.move_index32`
      )doc");

  m.def(
      "u16_get",
      [](const py::buffer &s, int32_t start, int32_t i, int32_t limit) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        if (limit < 0) {
          limit = icupy::u16strnlen(p, info.size);
        }
        if (i < 0 || i >= info.size || i >= limit) {
          throw py::index_error("index out of range: " + std::to_string(i));
        } else if (limit >= info.size) {
          throw py::index_error("limit out of range: " + std::to_string(limit));
        }
        uint32_t c;
        U16_GET(p, start, i, limit, c);
        return c;
      },
      py::arg("s"), py::arg("start"), py::arg("i"), py::arg("limit"), R"doc(
      Return the code point from a string at the specified offset.

      If the offset points to the lead or tail surrogate of a supplementary
      code point, read the adjacent matching surrogate as well. Also, if the
      offset points to a single, unpaired surrogate, return that unpaired
      surrogate.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *start* < *i* < *limit*.

      *limit* can be negative for a NULL-terminated string.

      See Also:
          :func:`u16_get_or_fffd`
          :meth:`Replaceable.char32_at`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_get_or_fffd",
      [](const py::buffer &s, int32_t start, int32_t i, int32_t limit) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        if (limit < 0) {
          limit = icupy::u16strnlen(p, info.size);
        }
        if (i < 0 || i >= info.size || i >= limit) {
          throw py::index_error("index out of range: " + std::to_string(i));
        } else if (limit >= info.size) {
          throw py::index_error("limit out of range: " + std::to_string(limit));
        }
        uint32_t c;
        U16_GET_OR_FFFD(p, start, i, limit, c);
        return c;
      },
      py::arg("s"), py::arg("start"), py::arg("i"), py::arg("limit"), R"doc(
      Return the code point from a string at the specified offset.

      If the offset points to the lead or tail surrogate of a supplementary
      code point, read the adjacent matching surrogate as well. Also, if the
      offset points to a single, unpaired surrogate, return U+FFFD.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *start* < *i* < *limit*.

      *limit* can be negative for a NULL-terminated string.

      See Also:
          :func:`u16_get`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_get_supplementary",
      [](uint32_t lead, uint32_t trail) -> uint32_t {
        return U16_GET_SUPPLEMENTARY(lead, trail);
      },
      py::arg("lead"), py::arg("trail"), R"doc(
      Return the supplementary code point value (U+10000..U+10FFFF) from its
      lead and trail surrogates.

      The result is undefined if the input values are not lead and trail
      surrogates.

      Example:
          >>> from icupy import icu
          >>> chr(icu.u16_get_supplementary(0xD83E, 0xDEE0))
          '🫠'  # U+1FAE0 (U+D83E U+DEE0)
      )doc");

  m.def(
      "u16_is_lead", [](uint32_t c) -> py::bool_ { return U16_IS_LEAD(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code unit is a lead surrogate (U+D800..U+DBFF);
      ``False`` otherwise.

      See Also:
          :func:`u16_is_trail`
      )doc");

  m.def(
      "u16_is_single", [](uint32_t c) -> py::bool_ { return U16_IS_SINGLE(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code unit is a single code unit (BMP, not a
      surrogate); ``False`` otherwise.
      )doc");

  m.def(
      "u16_is_surrogate",
      [](uint32_t c) -> py::bool_ { return U16_IS_SURROGATE(c); }, py::arg("c"),
      R"doc(
      Return ``True`` if this code unit is a surrogate (U+D800..U+DFFF);
      ``False`` otherwise.

      See Also:
          :func:`u16_is_surrogate_lead`
          :func:`u16_is_surrogate_trail`
      )doc");

  m.def(
      "u16_is_surrogate_lead",
      [](uint32_t c) -> py::bool_ { return U16_IS_SURROGATE_LEAD(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this surrogate code unit is a lead surrogate;
      ``False`` otherwise.

      See Also:
          :func:`u16_is_surrogate`
      )doc");

  m.def(
      "u16_is_surrogate_trail",
      [](uint32_t c) -> py::bool_ { return U16_IS_SURROGATE_TRAIL(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this surrogate code unit is a trail surrogate;
      ``False`` otherwise.

      See Also:
          :func:`u16_is_surrogate`
      )doc");

  m.def(
      "u16_is_trail", [](uint32_t c) -> py::bool_ { return U16_IS_TRAIL(c); },
      py::arg("c"), R"doc(
      Return ``True`` if this code unit is a trail surrogate (U+DC00..U+DFFF);
      ``False`` otherwise.

      See Also:
          :func:`u16_is_lead`
      )doc");

  m.def(
      "u16_lead",
      [](uint32_t supplementary) -> uint16_t {
        return U16_LEAD(supplementary);
      },
      py::arg("supplementary"), R"doc(
      Return the lead surrogate (U+D800..U+DBFF) for a supplementary code point
      (U+10000..U+10FFFF).

      Example:
          >>> from icupy import icu
          >>> chr(icu.u16_lead(ord("🫠")))  # U+1FAE0 (U+D83E U+DEE0)
          '\ud83e'
      )doc");

  m.def(
      "u16_length", [](uint32_t c) -> int32_t { return U16_LENGTH(c); },
      py::arg("c"), R"doc(
      Return the number of 16-bit code units used to encode this Unicode code
      point.
      )doc");

  m.def(
      "u16_next",
      [](const py::buffer &s, int32_t i, int32_t limit) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        if (limit < 0) {
          limit = icupy::u16strnlen(p, info.size);
        }
        if (i < 0 || i + 1 >= info.size || i >= limit) {
          throw py::index_error("index out of range: " + std::to_string(i));
        } else if (limit >= info.size) {
          throw py::index_error("limit out of range: " + std::to_string(limit));
        }
        uint32_t c;
        U16_NEXT(p, i, limit, c);
        return std::make_tuple(c, i);
      },
      py::arg("s"), py::arg("i"), py::arg("limit"), R"doc(
      Return a tuple of the code point at the specified offset in the string
      and the offset to the next code point boundary.

      If the offset points to the lead surrogate unit of the supplementary code
      point, the following trail surrogate is also read.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *i* < *limit*.

      *limit* can be negative for a NULL-terminated string.

      See Also:
          :func:`u16_next_or_fffd`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_next_or_fffd",
      [](const py::buffer &s, int32_t i, int32_t limit) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        if (limit < 0) {
          limit = icupy::u16strnlen(p, info.size);
        }
        if (i < 0 || i + 1 >= info.size || i >= limit) {
          throw py::index_error("index out of range: " + std::to_string(i));
        } else if (limit >= info.size) {
          throw py::index_error("limit out of range: " + std::to_string(limit));
        }
        uint32_t c;
        U16_NEXT_OR_FFFD(p, i, limit, c);
        return std::make_tuple(c, i);
      },
      py::arg("s"), py::arg("i"), py::arg("limit"), R"doc(
      Return a tuple of the code point at the specified offset in the string
      and the offset to the next code point boundary.

      If the offset points to a single, unpaired surrogate, return U+FFFD as
      the code point.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *i* < *limit*.

      *limit* can be negative for a NULL-terminated string.

      See Also:
          :func:`u16_next`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_prev",
      [](const py::buffer &s, int32_t start, int32_t i) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        } else if (start < 0 || start >= info.size) {
          throw py::index_error("start out of range: " + std::to_string(start));
        } else if (i <= 0 || i >= info.size) {
          throw py::index_error("index out of range: " + std::to_string(i));
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        uint32_t c;
        U16_PREV(p, start, i, c);
        return std::make_tuple(c, i);
      },
      py::arg("s"), py::arg("start"), py::arg("i"), R"doc(
      Move the string offset from one code point boundary to the previous one,
      and return a tuple of the code point at the new offset and the new
      offset.

      If the offset is behind a trail surrogate unit for a supplementary code
      point, the preceding lead surrogate is also read.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *start* < *i*.

      See Also:
          :func:`u16_prev_or_fffd`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_prev_or_fffd",
      [](const py::buffer &s, int32_t start, int32_t i) {
        auto info = s.request();
        if (info.format != py::format_descriptor<uint16_t>::format()) {
          throw py::value_error("unsupported buffer format: " + info.format);
        } else if (start < 0 || start >= info.size) {
          throw py::index_error("start out of range: " + std::to_string(start));
        } else if (i <= 0 || i >= info.size) {
          throw py::index_error("index out of range: " + std::to_string(i));
        }
        auto p = reinterpret_cast<uint16_t *>(info.ptr);
        uint32_t c;
        U16_PREV_OR_FFFD(p, start, i, c);
        return std::make_tuple(c, i);
      },
      py::arg("s"), py::arg("start"), py::arg("i"), R"doc(
      Move the string offset from one code point boundary to the previous one,
      and return a tuple of the code point at the new offset and the new
      offset.

      If the offset points to a single, unpaired surrogate, return U+FFFD as
      the code point.

      *s* must support the buffer protocol, and its format must be ``H``
      (unsigned short).

      *i* must be *start* < *i*.

      See Also:
          :func:`u16_prev`
          :meth:`UnicodeString.get_buffer`
          :meth:`UnicodeString.get_terminated_buffer`
      )doc");

  m.def(
      "u16_trail",
      [](uint32_t supplementary) -> uint16_t {
        return U16_TRAIL(supplementary);
      },
      py::arg("supplementary"), R"doc(
      Return the trail surrogate (U+DC00..U+DFFF) for a supplementary code point
      (U+10000..U+10FFFF).

      Example:
          >>> from icupy import icu
          >>> chr(icu.u16_trail(ord("🫠")))  # U+1FAE0 (U+D83E U+DEE0)
          '\udee0'
      )doc");
}
