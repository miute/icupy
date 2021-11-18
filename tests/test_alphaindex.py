import pytest
from icupy.icu import (
    AlphabeticIndex, ConstVoidPtr, Locale, RuleBasedCollator,
    UAlphabeticIndexLabelType, U_ICU_VERSION_MAJOR_NUM, UnicodeSet,
    UnicodeString,
)


def test_add_labels():
    # [1]
    # AlphabeticIndex &icu::AlphabeticIndex::addLabels(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    index = AlphabeticIndex(Locale("en"))
    result = index.add_labels(Locale("ja", "JP"))
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_bucket_count() > 35

    index = AlphabeticIndex(Locale("en"))
    result = index.add_labels("ja_JP")
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_bucket_count() > 35

    # [2]
    # AlphabeticIndex &icu::AlphabeticIndex::addLabels(
    #       const UnicodeSet &additions,
    #       UErrorCode &status
    # )
    index = AlphabeticIndex(Locale("en"))
    additions = UnicodeSet()
    additions.add(0x410).add(0x415)
    result = index.add_labels(additions)
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    # underflow, A-Z, inflow, 2 Cyrillic, overflow
    assert index.get_bucket_count() == 31


def test_add_record():
    index = AlphabeticIndex(Locale.get_english())
    obj3 = index
    obj4 = {"AlphabeticIndex": index}

    # UBool icu::AlphabeticIndex::nextBucket(UErrorCode &status)
    assert index.next_bucket()

    # UBool icu::AlphabeticIndex::nextRecord(UErrorCode &status)
    assert not index.next_record()

    # AlphabeticIndex &icu::AlphabeticIndex::addRecord(
    #       const UnicodeString &name,
    #       const void *data,
    #       UErrorCode &status
    # )
    result = index.add_record(UnicodeString("Adam"), None)
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)

    result = index.add_record("Baker", None)
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)

    data3 = ConstVoidPtr(obj3)
    result = index.add_record(UnicodeString("Charlie"), data3)
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)

    data4 = ConstVoidPtr(obj4)
    result = index.add_record("Chad", data4)
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)

    # int32_t icu::AlphabeticIndex::getRecordCount(UErrorCode &status)
    assert index.get_record_count() == 4

    # AlphabeticIndex &icu::AlphabeticIndex::resetBucketIterator(
    #       UErrorCode &status
    # )
    result = index.reset_bucket_iterator()
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)

    # int32_t icu::AlphabeticIndex::getBucketRecordCount()
    # const void *icu::AlphabeticIndex::getRecordData()
    # const UnicodeString &icu::AlphabeticIndex::getRecordName()
    assert index.next_bucket()
    assert index.get_bucket_record_count() == 0
    assert not index.next_record()

    assert index.next_bucket()
    assert index.get_bucket_record_count() == 1
    assert index.next_record()
    result = index.get_record_data()
    assert result is None
    result = index.get_record_name()
    assert isinstance(result, UnicodeString)
    assert result == "Adam"
    assert not index.next_record()

    assert index.next_bucket()
    assert index.get_bucket_record_count() == 1
    assert index.next_record()
    result = index.get_record_data()
    assert result is None
    result = index.get_record_name()
    assert isinstance(result, UnicodeString)
    assert result == "Baker"
    assert not index.next_record()

    assert index.next_bucket()
    assert index.get_bucket_record_count() == 2
    assert index.next_record()
    result = index.get_record_data()
    assert isinstance(result, ConstVoidPtr)
    assert result.to_object() == obj4
    result = index.get_record_name()
    assert isinstance(result, UnicodeString)
    assert result == "Chad"

    assert index.next_record()
    result = index.get_record_data()
    assert isinstance(result, ConstVoidPtr)
    assert result.to_object() == obj3
    result = index.get_record_name()
    assert isinstance(result, UnicodeString)
    assert result == "Charlie"
    assert not index.next_record()

    # AlphabeticIndex &icu::AlphabeticIndex::resetRecordIterator()
    result = index.reset_record_iterator()
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)

    assert index.next_record()
    assert index.get_record_name() == "Chad"

    # AlphabeticIndex &icu::AlphabeticIndex::clearRecords(UErrorCode &status)
    result = index.clear_records()
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_record_count() == 0


def test_api():
    # [1]
    # icu::AlphabeticIndex::AlphabeticIndex(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    index = AlphabeticIndex(Locale("en"))
    index2 = AlphabeticIndex("en")

    # int32_t icu::AlphabeticIndex::getBucketCount(UErrorCode &status)
    assert index.get_bucket_count() == 28  # A-Z, underflow, overflow
    assert index2.get_bucket_count() == 28

    # UBool icu::AlphabeticIndex::nextBucket(UErrorCode &status)
    assert index.next_bucket()
    assert index.next_bucket()
    assert index.next_bucket()

    # [1]
    # int32_t icu::AlphabeticIndex::getBucketIndex()
    assert index.get_bucket_index() == 2

    # [2]
    # int32_t icu::AlphabeticIndex::getBucketIndex(
    #       const UnicodeString &itemName,
    #       UErrorCode &status
    # )
    assert index.get_bucket_index(UnicodeString("A")) == 1
    assert index.get_bucket_index("B") == 2

    # const UnicodeString &icu::AlphabeticIndex::getBucketLabel()
    result = index.get_bucket_label()
    assert isinstance(result, UnicodeString)
    assert result == "B"

    # UAlphabeticIndexLabelType icu::AlphabeticIndex::getBucketLabelType()
    assert (index.get_bucket_label_type()
            == UAlphabeticIndexLabelType.U_ALPHAINDEX_NORMAL)

    # const RuleBasedCollator &icu::AlphabeticIndex::getCollator()
    result = index.get_collator()
    assert isinstance(result, RuleBasedCollator)

    # const UnicodeString &icu::AlphabeticIndex::getInflowLabel()
    result = index.get_inflow_label()
    assert isinstance(result, UnicodeString)
    assert result == "\u2026"  # "…"

    # int32_t icu::AlphabeticIndex::getMaxLabelCount()
    assert index.get_max_label_count() == 99

    # const UnicodeString &icu::AlphabeticIndex::getOverflowLabel()
    result = index.get_overflow_label()
    assert isinstance(result, UnicodeString)
    assert result == "\u2026"  # "…"

    # const UnicodeString &icu::AlphabeticIndex::getUnderflowLabel()
    result = index.get_underflow_label()
    assert isinstance(result, UnicodeString)
    assert result == "\u2026"  # "…"

    # AlphabeticIndex &icu::AlphabeticIndex::resetBucketIterator(
    #       UErrorCode &status
    # )
    result = index.reset_bucket_iterator()
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.next_bucket()
    assert index.get_bucket_index() == 0

    # AlphabeticIndex &icu::AlphabeticIndex::setInflowLabel(
    #       const UnicodeString &inflowLabel,
    #       UErrorCode &status
    # )
    result = index.set_inflow_label(UnicodeString("I"))
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_inflow_label() == "I"

    result = index.set_inflow_label("i")
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_inflow_label() == "i"

    # AlphabeticIndex &icu::AlphabeticIndex::setMaxLabelCount(
    #       int32_t maxLabelCount,
    #       UErrorCode &status
    # )
    result = index.set_max_label_count(256)
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_max_label_count() == 256

    # AlphabeticIndex &icu::AlphabeticIndex::setOverflowLabel(
    #       const UnicodeString &overflowLabel,
    #       UErrorCode &status
    # )
    result = index.set_overflow_label(UnicodeString("O"))
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_overflow_label() == "O"

    result = index.set_overflow_label("o")
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_overflow_label() == "o"

    # AlphabeticIndex &icu::AlphabeticIndex::setUnderflowLabel(
    #       const UnicodeString &underflowLabel,
    #       UErrorCode &status
    # )
    result = index.set_underflow_label(UnicodeString("U"))
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_underflow_label() == "U"

    result = index.set_underflow_label("u")
    assert isinstance(result, AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_underflow_label() == "u"


@pytest.mark.skipif(U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51():
    from icupy.icu import Collator

    # [2]
    # icu::AlphabeticIndex::AlphabeticIndex(
    #       RuleBasedCollator *collator,
    #       UErrorCode &status
    # )
    collator = Collator.create_instance(Locale.get_english())
    index = AlphabeticIndex(collator)
    index.add_labels(Locale.get_english())
    assert index.get_bucket_count() == 28

    # const RuleBasedCollator &icu::AlphabeticIndex::getCollator()
    result = index.get_collator()
    assert isinstance(result, RuleBasedCollator)
    assert result == collator

    # ImmutableIndex *icu::AlphabeticIndex::buildImmutableIndex(
    #       UErrorCode &errorCode
    # )
    imm_index = index.build_immutable_index()
    assert isinstance(imm_index, AlphabeticIndex.ImmutableIndex)

    # const Bucket *icu::AlphabeticIndex::ImmutableIndex::getBucket(
    #       int32_t index
    # )
    bucket = imm_index.get_bucket(1)
    assert isinstance(bucket, AlphabeticIndex.Bucket)

    # int32_t icu::AlphabeticIndex::ImmutableIndex::getBucketCount()
    assert imm_index.get_bucket_count() == 28

    # int32_t icu::AlphabeticIndex::ImmutableIndex::getBucketIndex(
    #       const UnicodeString &name,
    #       UErrorCode &errorCode
    # )
    assert imm_index.get_bucket_index(UnicodeString("A")) == 1
    assert imm_index.get_bucket_index("B") == 2

    # const UnicodeString &icu::AlphabeticIndex::Bucket::getLabel()
    result = bucket.get_label()
    assert isinstance(result, UnicodeString)
    assert result == "A"

    # UAlphabeticIndexLabelType icu::AlphabeticIndex::Bucket::getLabelType()
    assert (bucket.get_label_type()
            == UAlphabeticIndexLabelType.U_ALPHAINDEX_NORMAL)
