import pytest

from icupy import icu

if icu.U_ICU_VERSION_MAJOR_NUM < 50:
    pytest.skip("ICU4C<50", allow_module_level=True)


def test_api() -> None:
    fmt = icu.ListFormatter.create_instance(icu.Locale("en", "US"))

    # [2]
    # UnicodeString &icu::ListFormatter::format(
    #       const UnicodeString items[],
    #       int32_t n_items,
    #       UnicodeString &appendTo,
    #       UErrorCode &errorCode
    # )
    items = [
        icu.UnicodeString("Alice"),
        icu.UnicodeString("Bob"),
        icu.UnicodeString("Charlie"),
        icu.UnicodeString("Delta"),
    ]
    append_to = icu.UnicodeString()
    result = fmt.format(items, len(items), append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Alice, Bob, Charlie, and Delta"

    append_to.remove()
    result = fmt.format(items, -1, append_to)
    assert isinstance(result, icu.UnicodeString)
    assert id(result) == id(append_to)
    assert result == "Alice, Bob, Charlie, and Delta"


def test_create_instance() -> None:
    # [1]
    # static ListFormatter *icu::ListFormatter::createInstance(
    #       const Locale &locale,
    #       UErrorCode &errorCode
    # )
    fmt1 = icu.ListFormatter.create_instance(icu.Locale("en", "US"))
    assert isinstance(fmt1, icu.ListFormatter)

    fmt1a = icu.ListFormatter.create_instance("en-US")
    assert isinstance(fmt1a, icu.ListFormatter)

    # [3]
    # static ListFormatter *icu::ListFormatter::createInstance(
    #       UErrorCode &errorCode
    # )
    fmt3 = icu.ListFormatter.create_instance()
    assert isinstance(fmt3, icu.ListFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 67, reason="ICU4C<67")
def test_create_instance_67() -> None:
    # [2]
    # static ListFormatter *icu::ListFormatter::createInstance(
    #       const Locale &locale,
    #       UListFormatterType type,
    #       UListFormatterWidth width,
    #       UErrorCode &errorCode
    # )
    fmt2 = icu.ListFormatter.create_instance(
        icu.Locale("en", "US"),
        icu.UListFormatterType.ULISTFMT_TYPE_UNITS,
        icu.UListFormatterWidth.ULISTFMT_WIDTH_SHORT,
    )
    assert isinstance(fmt2, icu.ListFormatter)

    fmt2a = icu.ListFormatter.create_instance(
        "en-US",
        icu.UListFormatterType.ULISTFMT_TYPE_UNITS,
        icu.UListFormatterWidth.ULISTFMT_WIDTH_SHORT,
    )
    assert isinstance(fmt2a, icu.ListFormatter)


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 64, reason="ICU4C<64")
def test_format_strings_to_value() -> None:
    assert issubclass(icu.FormattedList, icu.FormattedValue)

    # icu::FormattedList::FormattedList()
    fl = icu.FormattedList()
    with pytest.raises(icu.ICUError) as exc_info:
        _ = fl.to_string()
    assert exc_info.value.args[0] == icu.UErrorCode.U_INVALID_STATE_ERROR

    fmt = icu.ListFormatter.create_instance("en")

    # FormattedList icu::ListFormatter::formatStringsToValue(
    #       const UnicodeString items[],
    #       int32_t n_items,
    #       UErrorCode &errorCode
    # )
    items = [
        icu.UnicodeString("hello"),
        icu.UnicodeString("wonderful"),
        icu.UnicodeString("world"),
    ]
    fl = fmt.format_strings_to_value(items, len(items))
    assert isinstance(fl, icu.FormattedList)

    # Appendable &icu::FormattedList::appendTo(
    #       Appendable &appendable,
    #       UErrorCode &status
    # )
    s = icu.UnicodeString("x")
    appendable = icu.UnicodeStringAppendable(s)
    result = fl.append_to(appendable)
    assert isinstance(result, icu.UnicodeStringAppendable)
    assert id(result) == id(appendable)
    assert s == "xhello, wonderful, and world"

    # UBool icu::FormattedList::nextPosition(
    #       ConstrainedFieldPosition &cfpos,
    #       UErrorCode &status
    # )
    cfpos = icu.ConstrainedFieldPosition()
    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
    assert cfpos.get_field() == 0
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 5)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == icu.UListFormatterField.ULISTFMT_ELEMENT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (0, 5)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == icu.UListFormatterField.ULISTFMT_LITERAL_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (5, 7)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
    assert cfpos.get_field() == 1
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 16)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == icu.UListFormatterField.ULISTFMT_ELEMENT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (7, 16)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == icu.UListFormatterField.ULISTFMT_LITERAL_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (16, 22)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST_SPAN
    assert cfpos.get_field() == 2
    assert (cfpos.get_start(), cfpos.get_limit()) == (22, 27)

    assert fl.next_position(cfpos)
    assert cfpos.get_category() == icu.UFieldCategory.UFIELD_CATEGORY_LIST
    assert cfpos.get_field() == icu.UListFormatterField.ULISTFMT_ELEMENT_FIELD
    assert (cfpos.get_start(), cfpos.get_limit()) == (22, 27)

    assert not fl.next_position(cfpos)

    # UnicodeString icu::FormattedList::toString(UErrorCode &status)
    result = fl.to_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "hello, wonderful, and world"

    # UnicodeString icu::FormattedList::toTempString(UErrorCode &status)
    result = fl.to_temp_string()
    assert isinstance(result, icu.UnicodeString)
    assert result == "hello, wonderful, and world"

    fl = fmt.format_strings_to_value(items, -1)  # n_items is optional
    assert fl.to_temp_string() == "hello, wonderful, and world"

    fl = fmt.format_strings_to_value(items)  # n_items is optional
    assert fl.to_temp_string() == "hello, wonderful, and world"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 52, reason="ICU4C<52")
def test_list_formatter_52() -> None:
    fmt = icu.ListFormatter.create_instance(icu.Locale("en", "US"))
    assert isinstance(fmt, icu.ListFormatter)

    # [1]
    # icu::ListFormatter::ListFormatter(const ListFormatter &)
    _ = icu.ListFormatter(fmt)
