from icupy.icu import (
    CollationKey, Collator, Locale, UCollationResult, UColAttribute,
    UColAttributeValue, UnicodeString
)


def test_api():
    coll = Collator.create_instance(Locale.get_english())
    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_TERTIARY)
    source = UnicodeString("Abcd")

    # [1]
    # icu::CollationKey::CollationKey()
    key = CollationKey()
    coll.get_collation_key(source, key)

    key1 = CollationKey()

    # const uint8_t *icu::CollationKey::getByteArray(int32_t &count)
    values = key.get_byte_array()
    assert isinstance(values, list)
    assert len(values) > 0
    assert all(isinstance(x, int) for x in values)

    # [2]
    # icu::CollationKey::CollationKey(
    #       const uint8_t *values,
    #       int32_t count
    # )
    key2 = CollationKey(values, len(values))

    # [3]
    # icu::CollationKey::CollationKey(const CollationKey &other)
    key3 = CollationKey(key)

    # [2]
    # UCollationResult icu::CollationKey::compareTo(
    #       const CollationKey &target,
    #       UErrorCode &status
    # )
    assert key.compare_to(key1) == UCollationResult.UCOL_GREATER
    assert key.compare_to(key2) == UCollationResult.UCOL_EQUAL
    assert key.compare_to(key3) == UCollationResult.UCOL_EQUAL

    assert key1.compare_to(key) == UCollationResult.UCOL_LESS
    assert key1.compare_to(key2) == UCollationResult.UCOL_LESS
    assert key1.compare_to(key3) == UCollationResult.UCOL_LESS

    assert key2.compare_to(key) == UCollationResult.UCOL_EQUAL
    assert key2.compare_to(key1) == UCollationResult.UCOL_GREATER
    assert key2.compare_to(key3) == UCollationResult.UCOL_EQUAL

    assert key3.compare_to(key) == UCollationResult.UCOL_EQUAL
    assert key3.compare_to(key1) == UCollationResult.UCOL_GREATER
    assert key3.compare_to(key2) == UCollationResult.UCOL_EQUAL

    # int32_t icu::CollationKey::hashCode(void)
    assert key1.hash_code() != key.hash_code()
    assert key2.hash_code() == key.hash_code()
    assert key3.hash_code() == key.hash_code()

    # UBool icu::CollationKey::isBogus(void)
    assert not key1.is_bogus()
    assert not key2.is_bogus()
    assert not key3.is_bogus()

    # UBool icu::CollationKey::operator!=(const CollationKey &source)
    assert key1 != key

    # UBool icu::CollationKey::operator==(const CollationKey &source)
    assert key2 == key
    assert key3 == key
