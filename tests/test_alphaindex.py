import pytest

from icupy import icu


def test_add_labels() -> None:
    # [1]
    # AlphabeticIndex &icu::AlphabeticIndex::addLabels(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    index = icu.AlphabeticIndex(icu.Locale("en"))
    result = index.add_labels(icu.Locale("ja", "JP"))
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_bucket_count() > 35

    index = icu.AlphabeticIndex(icu.Locale("en"))
    result = index.add_labels("ja_JP")
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_bucket_count() > 35

    # [2]
    # AlphabeticIndex &icu::AlphabeticIndex::addLabels(
    #       const UnicodeSet &additions,
    #       UErrorCode &status
    # )
    index = icu.AlphabeticIndex(icu.Locale("en"))
    additions = icu.UnicodeSet()
    additions.add(0x410).add(0x415)
    result = index.add_labels(additions)
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    # underflow, A-Z, inflow, 2 Cyrillic, overflow
    assert index.get_bucket_count() == 31


def test_add_record() -> None:
    index = icu.AlphabeticIndex(icu.Locale.get_english())
    obj3 = index
    obj4 = {"AlphabeticIndex": index}

    # UBool icu::AlphabeticIndex::nextBucket(UErrorCode &status)
    assert index.next_bucket() is True

    # UBool icu::AlphabeticIndex::nextRecord(UErrorCode &status)
    assert index.next_record() is False

    # AlphabeticIndex &icu::AlphabeticIndex::addRecord(
    #       const UnicodeString &name,
    #       const void *data,
    #       UErrorCode &status
    # )
    result = index.add_record(icu.UnicodeString("Adam"), None)
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)

    result = index.add_record("Baker", None)
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)

    data3 = icu.ConstVoidPtr(obj3)
    result = index.add_record(icu.UnicodeString("Charlie"), data3)
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)

    data4 = icu.ConstVoidPtr(obj4)
    result = index.add_record("Chad", data4)
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)

    # int32_t icu::AlphabeticIndex::getRecordCount(UErrorCode &status)
    assert index.get_record_count() == 4

    # AlphabeticIndex &icu::AlphabeticIndex::resetBucketIterator(
    #       UErrorCode &status
    # )
    result = index.reset_bucket_iterator()
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)

    # int32_t icu::AlphabeticIndex::getBucketRecordCount()
    # const void *icu::AlphabeticIndex::getRecordData()
    # const UnicodeString &icu::AlphabeticIndex::getRecordName()
    assert index.next_bucket() is True
    assert index.get_bucket_record_count() == 0
    assert index.next_record() is False

    assert index.next_bucket() is True
    assert index.get_bucket_record_count() == 1
    assert index.next_record() is True
    result = index.get_record_data()
    assert result is None
    result = index.get_record_name()
    assert isinstance(result, icu.UnicodeString)
    assert result == "Adam"
    assert index.next_record() is False

    assert index.next_bucket() is True
    assert index.get_bucket_record_count() == 1
    assert index.next_record() is True
    result = index.get_record_data()
    assert result is None
    result = index.get_record_name()
    assert isinstance(result, icu.UnicodeString)
    assert result == "Baker"
    assert index.next_record() is False

    assert index.next_bucket() is True
    assert index.get_bucket_record_count() == 2
    assert index.next_record() is True
    result = index.get_record_data()
    assert isinstance(result, icu.ConstVoidPtr)
    assert result.to_object() == obj4
    result = index.get_record_name()
    assert isinstance(result, icu.UnicodeString)
    assert result == "Chad"

    assert index.next_record() is True
    result = index.get_record_data()
    assert isinstance(result, icu.ConstVoidPtr)
    assert result.to_object() == obj3
    result = index.get_record_name()
    assert isinstance(result, icu.UnicodeString)
    assert result == "Charlie"
    assert index.next_record() is False

    # AlphabeticIndex &icu::AlphabeticIndex::resetRecordIterator()
    result = index.reset_record_iterator()
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)

    assert index.next_record() is True
    assert index.get_record_name() == "Chad"

    # AlphabeticIndex &icu::AlphabeticIndex::clearRecords(UErrorCode &status)
    result = index.clear_records()
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_record_count() == 0


def test_api() -> None:
    # [1]
    # icu::AlphabeticIndex::AlphabeticIndex(
    #       const Locale &locale,
    #       UErrorCode &status
    # )
    index = icu.AlphabeticIndex(icu.Locale("en"))
    index2 = icu.AlphabeticIndex("en")

    # int32_t icu::AlphabeticIndex::getBucketCount(UErrorCode &status)
    assert index.get_bucket_count() == 28  # A-Z, underflow, overflow
    assert index2.get_bucket_count() == 28

    # UBool icu::AlphabeticIndex::nextBucket(UErrorCode &status)
    assert index.next_bucket() is True
    assert index.next_bucket() is True
    assert index.next_bucket() is True

    # [1]
    # int32_t icu::AlphabeticIndex::getBucketIndex()
    assert index.get_bucket_index() == 2

    # [2]
    # int32_t icu::AlphabeticIndex::getBucketIndex(
    #       const UnicodeString &itemName,
    #       UErrorCode &status
    # )
    assert index.get_bucket_index(icu.UnicodeString("A")) == 1
    assert index.get_bucket_index("B") == 2

    # const UnicodeString &icu::AlphabeticIndex::getBucketLabel()
    result = index.get_bucket_label()
    assert isinstance(result, icu.UnicodeString)
    assert result == "B"

    # UAlphabeticIndexLabelType icu::AlphabeticIndex::getBucketLabelType()
    assert index.get_bucket_label_type() == icu.UAlphabeticIndexLabelType.U_ALPHAINDEX_NORMAL

    # const RuleBasedCollator &icu::AlphabeticIndex::getCollator()
    result = index.get_collator()
    assert isinstance(result, icu.RuleBasedCollator)

    # const UnicodeString &icu::AlphabeticIndex::getInflowLabel()
    result = index.get_inflow_label()
    assert isinstance(result, icu.UnicodeString)
    assert result == "\u2026"  # "…"

    # int32_t icu::AlphabeticIndex::getMaxLabelCount()
    assert index.get_max_label_count() == 99

    # const UnicodeString &icu::AlphabeticIndex::getOverflowLabel()
    result = index.get_overflow_label()
    assert isinstance(result, icu.UnicodeString)
    assert result == "\u2026"  # "…"

    # const UnicodeString &icu::AlphabeticIndex::getUnderflowLabel()
    result = index.get_underflow_label()
    assert isinstance(result, icu.UnicodeString)
    assert result == "\u2026"  # "…"

    # AlphabeticIndex &icu::AlphabeticIndex::resetBucketIterator(
    #       UErrorCode &status
    # )
    result = index.reset_bucket_iterator()
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.next_bucket() is True
    assert index.get_bucket_index() == 0

    # AlphabeticIndex &icu::AlphabeticIndex::setInflowLabel(
    #       const UnicodeString &inflowLabel,
    #       UErrorCode &status
    # )
    result = index.set_inflow_label(icu.UnicodeString("I"))
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_inflow_label() == "I"

    result = index.set_inflow_label("i")
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_inflow_label() == "i"

    # AlphabeticIndex &icu::AlphabeticIndex::setMaxLabelCount(
    #       int32_t maxLabelCount,
    #       UErrorCode &status
    # )
    result = index.set_max_label_count(256)
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_max_label_count() == 256

    # AlphabeticIndex &icu::AlphabeticIndex::setOverflowLabel(
    #       const UnicodeString &overflowLabel,
    #       UErrorCode &status
    # )
    result = index.set_overflow_label(icu.UnicodeString("O"))
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_overflow_label() == "O"

    result = index.set_overflow_label("o")
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_overflow_label() == "o"

    # AlphabeticIndex &icu::AlphabeticIndex::setUnderflowLabel(
    #       const UnicodeString &underflowLabel,
    #       UErrorCode &status
    # )
    result = index.set_underflow_label(icu.UnicodeString("U"))
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_underflow_label() == "U"

    result = index.set_underflow_label("u")
    assert isinstance(result, icu.AlphabeticIndex)
    assert id(result) == id(index)
    assert index.get_underflow_label() == "u"


@pytest.mark.skipif(icu.U_ICU_VERSION_MAJOR_NUM < 51, reason="ICU4C<51")
def test_api_51() -> None:
    # [2]
    # icu::AlphabeticIndex::AlphabeticIndex(
    #       RuleBasedCollator *collator,
    #       UErrorCode &status
    # )
    collator = icu.Collator.create_instance(icu.Locale.get_english())
    index = icu.AlphabeticIndex(collator)
    index.add_labels(icu.Locale.get_english())
    assert index.get_bucket_count() == 28

    # const RuleBasedCollator &icu::AlphabeticIndex::getCollator()
    result = index.get_collator()
    assert isinstance(result, icu.RuleBasedCollator)
    assert result == collator

    # ImmutableIndex *icu::AlphabeticIndex::buildImmutableIndex(
    #       UErrorCode &errorCode
    # )
    imm_index = index.build_immutable_index()
    assert isinstance(imm_index, icu.AlphabeticIndex.ImmutableIndex)

    # const Bucket *icu::AlphabeticIndex::ImmutableIndex::getBucket(
    #       int32_t index
    # )
    bucket = imm_index.get_bucket(1)
    assert isinstance(bucket, icu.AlphabeticIndex.Bucket)

    # int32_t icu::AlphabeticIndex::ImmutableIndex::getBucketCount()
    assert imm_index.get_bucket_count() == 28

    # int32_t icu::AlphabeticIndex::ImmutableIndex::getBucketIndex(
    #       const UnicodeString &name,
    #       UErrorCode &errorCode
    # )
    assert imm_index.get_bucket_index(icu.UnicodeString("A")) == 1
    assert imm_index.get_bucket_index("B") == 2

    # const UnicodeString &icu::AlphabeticIndex::Bucket::getLabel()
    result = bucket.get_label()
    assert isinstance(result, icu.UnicodeString)
    assert result == "A"

    # UAlphabeticIndexLabelType icu::AlphabeticIndex::Bucket::getLabelType()
    assert bucket.get_label_type() == icu.UAlphabeticIndexLabelType.U_ALPHAINDEX_NORMAL
