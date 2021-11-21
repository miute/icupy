from functools import partial

import pytest
from icupy.icu import U_ICU_VERSION_MAJOR_NUM

if U_ICU_VERSION_MAJOR_NUM < 59:
    pytest.skip("ICU4C<59", allow_module_level=True)

from icupy.icu import Edits, ErrorCode, UErrorCode


# From icu/source/test/intltest/strcase.cpp
def test_api_59():
    # [1]
    # icu::Edits::Edits()
    edits = Edits()

    # void icu::Edits::add_replace(
    #       int32_t oldLength,
    #       int32_t newLength
    # )
    edits.add_replace(2, 1)
    edits.add_replace(2, 1)
    edits.add_replace(2, 1)
    edits.add_replace(0, 10)
    edits.add_replace(100, 0)
    edits.add_replace(3000, 4000)
    edits.add_replace(100000, 100000)

    # void icu::Edits::add_unchanged(int32_t unchangedLength)
    edits.add_unchanged(1)
    edits.add_unchanged(10000)
    edits.add_unchanged(2)
    edits.add_unchanged(0)

    # UBool icu::Edits::copyErrorTo(UErrorCode &outErrorCode)
    out_error_code = ErrorCode()
    assert not edits.copy_error_to(out_error_code)
    assert out_error_code == UErrorCode.U_ZERO_ERROR

    out_error_code.set(UErrorCode.U_ILLEGAL_ARGUMENT_ERROR)
    assert edits.copy_error_to(out_error_code)
    assert out_error_code == UErrorCode.U_ILLEGAL_ARGUMENT_ERROR

    # UBool icu::Edits::hasChanges()
    assert edits.has_changes()

    # int32_t icu::Edits::lengthDelta()
    assert edits.length_delta() == -3 + 10 - 100 + 1000

    # Iterator icu::Edits::getCoarseChangesIterator()
    ei = edits.get_coarse_changes_iterator()
    assert isinstance(ei, Edits.Iterator)

    # Iterator icu::Edits::getCoarseIterator()
    ei = edits.get_coarse_iterator()
    assert isinstance(ei, Edits.Iterator)

    # Iterator icu::Edits::getFineChangesIterator()
    ei = edits.get_fine_changes_iterator()
    assert isinstance(ei, Edits.Iterator)

    # Iterator icu::Edits::getFineIterator()
    ei = edits.get_fine_iterator()
    assert isinstance(ei, Edits.Iterator)

    # void icu::Edits::reset()
    edits.reset()
    assert not edits.has_changes()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_api_60():
    other = Edits()
    other.add_unchanged(1)
    other.add_unchanged(10000)
    other.add_replace(0, 0)
    other.add_unchanged(2)
    other.add_replace(2, 1)
    other.add_replace(2, 1)
    other.add_replace(2, 1)
    other.add_replace(0, 10)
    other.add_replace(100, 0)
    other.add_replace(3000, 4000)
    other.add_replace(100000, 100000)

    # [2]
    # icu::Edits::Edits(const Edits &other)
    edits = Edits(other)

    # int32_t icu::Edits::numberOfChanges()
    assert edits.number_of_changes() == 7

    # Edits &icu::Edits::mergeAndAppend(
    #       const Edits &ab,
    #       const Edits &bc,
    #       UErrorCode &errorCode
    # )
    edits.reset()
    ab = Edits()
    bc = Edits()
    expected = Edits()

    ab.add_unchanged(2)
    bc.add_unchanged(2)
    expected.add_unchanged(2)

    ab.add_replace(3, 2)
    bc.add_replace(2, 1)
    expected.add_replace(3, 1)

    ab.add_unchanged(5)
    bc.add_unchanged(3)
    expected.add_unchanged(3)

    ab.add_replace(4, 3)
    bc.add_replace(3, 2)
    ab.add_replace(4, 3)
    bc.add_replace(3, 2)
    ab.add_replace(4, 3)
    bc.add_replace(3, 2)
    bc.add_unchanged(4)
    expected.add_replace(14, 8)

    ab.add_unchanged(2)
    expected.add_unchanged(2)

    ab.add_replace(0, 5)
    ab.add_replace(0, 2)
    bc.add_replace(7, 0)

    ab.add_replace(1, 2)
    bc.add_replace(2, 3)
    expected.add_replace(1, 3)

    ab.add_replace(1, 0)
    ab.add_replace(2, 0)
    ab.add_replace(3, 0)
    expected.add_replace(1, 0)
    expected.add_replace(2, 0)
    expected.add_replace(3, 0)

    ab.add_unchanged(2)
    bc.add_unchanged(1)
    expected.add_unchanged(1)

    bc.add_replace(0, 4)
    bc.add_replace(0, 5)
    bc.add_replace(0, 6)
    expected.add_replace(0, 4)
    expected.add_replace(0, 5)
    expected.add_replace(0, 6)

    bc.add_replace(2, 2)
    ab.add_replace(1, 0)
    ab.add_replace(2, 0)
    ab.add_replace(3, 0)
    ab.add_replace(4, 1)
    expected.add_replace(11, 2)

    ab.add_replace(5, 6)
    bc.add_replace(3, 3)
    bc.add_replace(0, 4)
    bc.add_replace(0, 5)
    bc.add_replace(0, 6)
    bc.add_replace(3, 7)
    expected.add_replace(5, 25)

    ab.add_replace(4, 4)
    ab.add_replace(3, 0)
    ab.add_unchanged(2)
    bc.add_replace(2, 2)
    bc.add_replace(4, 0)
    expected.add_replace(9, 2)

    ab.add_replace(0, 2)
    bc.add_replace(1, 1)
    bc.add_replace(0, 8)
    bc.add_unchanged(4)
    expected.add_replace(0, 10)

    ab.add_unchanged(3)
    expected.add_unchanged(3)

    ab.add_replace(2, 0)
    ab.add_replace(4, 0)
    ab.add_replace(6, 0)
    bc.add_replace(0, 1)
    bc.add_replace(0, 3)
    bc.add_replace(0, 5)
    expected.add_replace(0, 1)
    expected.add_replace(0, 3)
    expected.add_replace(0, 5)
    expected.add_replace(2, 0)
    expected.add_replace(4, 0)
    expected.add_replace(6, 0)

    ab.add_unchanged(1)
    bc.add_unchanged(1)
    expected.add_unchanged(1)

    result = edits.merge_and_append(ab, bc)
    assert isinstance(result, Edits)
    assert id(result) == id(edits)

    ei1 = edits.get_fine_iterator()
    ei2 = expected.get_fine_iterator()
    t1 = list(iter(partial(ei1.next), False))
    t2 = list(iter(partial(ei2.next), False))
    assert t1 == t2


def test_edits_iterator_59():
    edits = Edits()
    edits.add_unchanged(1)
    edits.add_unchanged(10000)
    edits.add_replace(0, 0)
    edits.add_unchanged(2)
    edits.add_replace(2, 1)
    edits.add_unchanged(0)
    edits.add_replace(2, 1)
    edits.add_replace(2, 1)
    edits.add_replace(0, 10)
    edits.add_replace(100, 0)
    edits.add_replace(3000, 4000)
    edits.add_replace(100000, 100000)

    # Iterator icu::Edits::getCoarseChangesIterator()
    ei = edits.get_coarse_changes_iterator()
    assert isinstance(ei, Edits.Iterator)

    exp_src_index = exp_dest_index = exp_repl_index = 0
    exp_old_length = 10003
    exp_new_length = 10003

    # int32_t icu::Edits::Iterator::destinationIndex()
    assert ei.destination_index() == exp_dest_index

    # UBool icu::Edits::Iterator::findSourceIndex(
    #       int32_t i,
    #       UErrorCode &errorCode
    # )
    assert ei.find_source_index(exp_src_index)

    # UBool icu::Edits::Iterator::hasChange()
    assert not ei.has_change()

    # int32_t icu::Edits::Iterator::newLength()
    assert ei.new_length() == exp_new_length

    # int32_t icu::Edits::Iterator::oldLength()
    assert ei.old_length() == exp_old_length

    # int32_t icu::Edits::Iterator::replacementIndex()
    assert ei.replacement_index() == exp_repl_index

    # int32_t icu::Edits::Iterator::sourceIndex()
    assert ei.source_index() == exp_src_index

    # UBool icu::Edits::Iterator::next(UErrorCode &errorCode)
    assert ei.next()
    exp_src_index += exp_old_length
    exp_dest_index += exp_new_length
    exp_old_length = 103106
    exp_new_length = 104013

    assert ei.destination_index() == exp_dest_index
    assert ei.has_change()
    assert ei.new_length() == exp_new_length
    assert ei.old_length() == exp_old_length
    assert ei.replacement_index() == exp_repl_index
    assert ei.source_index() == exp_src_index

    assert not ei.next()


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 60, reason="ICU4C<60")
def test_edits_iterator_60():
    from icupy.icu import CaseMap

    src = "abcßDeF"
    edits = Edits()
    CaseMap.fold(0, src, -1, edits)
    ei = edits.get_fine_iterator()

    # int32_t icu::Edits::Iterator::destinationIndexFromSourceIndex(
    #       int32_t i,
    #       UErrorCode &errorCode
    # )
    indices = []
    for i in range(len(src)):
        assert ei.find_source_index(i)
        indices.append(ei.destination_index_from_source_index(i))
    assert indices == [0, 1, 2, 3, 5, 6, 7]

    # UBool icu::Edits::Iterator::findDestinationIndex(
    #       int32_t i,
    #       UErrorCode &errorCode
    # )
    indices = []
    for i in range(len(src)):
        assert ei.find_destination_index(i)
        indices.append(ei.source_index())
    assert indices == [0, 0, 0, 3, 3, 4, 5]

    # int32_t icu::Edits::Iterator::sourceIndexFromDestinationIndex(
    #       int32_t i,
    #       UErrorCode &errorCode
    # )
    indices = []
    for i in range(len(src)):
        assert ei.find_destination_index(i)
        indices.append(ei.source_index_from_destination_index(i))
    assert indices == [0, 1, 2, 3, 4, 4, 5]
