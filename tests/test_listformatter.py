import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)

from icupy.icu import ListFormatter, Locale, UnicodeString


def test_api():
    fmt = ListFormatter.create_instance(Locale("en", "US"))

    # [2]
    # UnicodeString &icu::ListFormatter::format(
    #       const UnicodeString items[],
    #       int32_t n_items,
    #       UnicodeString &appendTo,
    #       UErrorCode &errorCode
    # )
    items = [
        UnicodeString("Alice"),
        UnicodeString("Bob"),
        UnicodeString("Charlie"),
        UnicodeString("Delta"),
    ]
    append_to = UnicodeString()
    result = fmt.format(items, len(items), append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Alice, Bob, Charlie, and Delta"

    append_to.remove()
    result = fmt.format(items, -1, append_to)
    assert isinstance(result, UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Alice, Bob, Charlie, and Delta"


def test_create_instance():
    # [1]
    # static ListFormatter *icu::ListFormatter::createInstance(
    #       const Locale &locale,
    #       UErrorCode &errorCode
    # )
    fmt1 = ListFormatter.create_instance(Locale("en", "US"))
    assert isinstance(fmt1, ListFormatter)

    fmt1a = ListFormatter.create_instance("en-US")
    assert isinstance(fmt1a, ListFormatter)

    # [3]
    # static ListFormatter *icu::ListFormatter::createInstance(
    #       UErrorCode &errorCode
    # )
    fmt3 = ListFormatter.create_instance()
    assert isinstance(fmt3, ListFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_create_instance_67():
    from icupy.icu import UListFormatterType, UListFormatterWidth

    # [2]
    # static ListFormatter *icu::ListFormatter::createInstance(
    #       const Locale &locale,
    #       UListFormatterType type,
    #       UListFormatterWidth width,
    #       UErrorCode &errorCode
    # )
    fmt2 = ListFormatter.create_instance(
        Locale("en", "US"),
        UListFormatterType.ULISTFMT_TYPE_UNITS,
        UListFormatterWidth.ULISTFMT_WIDTH_SHORT)
    assert isinstance(fmt2, ListFormatter)

    fmt2a = ListFormatter.create_instance(
        "en-US",
        UListFormatterType.ULISTFMT_TYPE_UNITS,
        UListFormatterWidth.ULISTFMT_WIDTH_SHORT)
    assert isinstance(fmt2a, ListFormatter)


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_format_strings_to_value():
    from icupy.icu import (
        ConstrainedFieldPosition, FormattedList, FormattedValue,
        UFieldCategory, UListFormatterField, UnicodeStringAppendable,
    )

    assert issubclass(FormattedList, FormattedValue)
    fmt = ListFormatter.create_instance("en")

    # FormattedList icu::ListFormatter::formatStringsToValue(
    #       const UnicodeString items[],
    #       int32_t n_items,
    #       UErrorCode &errorCode
    # )
    items = [
        UnicodeString("hello"),
        UnicodeString("wonderful"),
        UnicodeString("world"),
    ]
    fl = fmt.format_strings_to_value(items, len(items))
    assert isinstance(fl, FormattedList)

    # Appendable &icu::FormattedList::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = UnicodeString("x")
    appendable = UnicodeStringAppendable(s)
    result = fl.append_to(appendable)
    assert isinstance(result, UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "xhello, wonderful, and world"

    # UBool icu::FormattedList::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = ConstrainedFieldPosition()
    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
    assert cfpos.get_field() == 0
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 5)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == UListFormatterField.ULISTFMT_ELEMENT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 5)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == UListFormatterField.ULISTFMT_LITERAL_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (5, 7)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
    assert cfpos.get_field() == 1
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 16)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == UListFormatterField.ULISTFMT_ELEMENT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 16)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == UListFormatterField.ULISTFMT_LITERAL_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (16, 22)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
    assert cfpos.get_field() == 2
    assert (cfpos.get_start(), cfpos.get_limit()) == (22, 27)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == UListFormatterField.ULISTFMT_ELEMENT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (22, 27)

    assert not fl.next_position(cfpos)

    # UnicodeString icu::FormattedList::toString(UErrorCode &status)
    result = fl.to_string()
    assert isinstance(result, UnicodeString)
    assert result == "hello, wonderful, and world"

    # UnicodeString icu::FormattedList::toTempString(UErrorCode &status)
    result = fl.to_temp_string()
    assert isinstance(result, UnicodeString)
    assert result == "hello, wonderful, and world"

    fl = fmt.format_strings_to_value(items, -1)  # n_items is optional
    assert fl.to_temp_string() == "hello, wonderful, and world"

    fl = fmt.format_strings_to_value(items)  # n_items is optional
    assert fl.to_temp_string() == "hello, wonderful, and world"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_list_formatter_52():
    fmt = ListFormatter.create_instance(Locale("en", "US"))
    assert isinstance(fmt, ListFormatter)

    # [1]
    # icu::ListFormatter::ListFormatter(const ListFormatter &)
    _ = ListFormatter(fmt)
