from icupy import icu


def test_api() -> None:
    coll = icu.Collator.create_instance(icu.Locale.get_english())
    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_TERTIARY)
    source = icu.UnicodeString("Abcd")

    # [1]
    # icu::CollationKey::CollationKey()
    key = icu.CollationKey()
    coll.get_collation_key(source, key)

    key1 = icu.CollationKey()

    # const uint8_t *icu::CollationKey::getByteArray(int32_t &count)
    values = key.get_byte_array()
    assert isinstance(values, bytes)
    assert len(values) > 0

    # [2]
    # icu::CollationKey::CollationKey(
    #       const uint8_t *values,
    #       int32_t count
    # )
    key2 = icu.CollationKey(values, len(values))
    key2a = icu.CollationKey(values)
    assert key2 == key2a

    # [3]
    # icu::CollationKey::CollationKey(const CollationKey &other)
    key3 = icu.CollationKey(key)

    # [2]
    # UCollationResult icu::CollationKey::compareTo(
    #       const CollationKey &target,
    #       UErrorCode &status
    # )
    assert key.compare_to(key1) == icu.UCollationResult.UCOL_GREATER
    assert key.compare_to(key2) == icu.UCollationResult.UCOL_EQUAL
    assert key.compare_to(key3) == icu.UCollationResult.UCOL_EQUAL

    assert key1.compare_to(key) == icu.UCollationResult.UCOL_LESS
    assert key1.compare_to(key2) == icu.UCollationResult.UCOL_LESS
    assert key1.compare_to(key3) == icu.UCollationResult.UCOL_LESS

    assert key2.compare_to(key) == icu.UCollationResult.UCOL_EQUAL
    assert key2.compare_to(key1) == icu.UCollationResult.UCOL_GREATER
    assert key2.compare_to(key3) == icu.UCollationResult.UCOL_EQUAL

    assert key3.compare_to(key) == icu.UCollationResult.UCOL_EQUAL
    assert key3.compare_to(key1) == icu.UCollationResult.UCOL_GREATER
    assert key3.compare_to(key2) == icu.UCollationResult.UCOL_EQUAL

    # int32_t icu::CollationKey::hashCode(void)
    assert key1.hash_code() != key.hash_code()
    assert key2.hash_code() == key.hash_code()
    assert key3.hash_code() == key.hash_code()

    assert hash(key1) == key1.hash_code()
    assert hash(key2) == key2.hash_code()
    assert hash(key3) == key3.hash_code()

    # UBool icu::CollationKey::isBogus(void)
    assert key1.is_bogus() is False
    assert key2.is_bogus() is False
    assert key3.is_bogus() is False

    # UBool icu::CollationKey::operator!=(const CollationKey &source)
    assert key1 != key

    # UBool icu::CollationKey::operator==(const CollationKey &source)
    assert key2 == key
    assert key3 == key
