from icupy import (
    CollationKey, Collator, Locale, UCollationResult, UColAttribute,
    UColAttributeValue, UnicodeString
)


def test_api():
    coll = Collator.create_instance(Locale.get_english())
    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_TERTIARY)

    source = UnicodeString("Abcd")
    key = CollationKey()
    coll.get_collation_key(source, key)

    # [1]
    # CollationKey()
    key1 = CollationKey()

    # [2]
    # CollationKey(const uint8_t *values,
    #              int32_t count
    # )
    values = key.get_byte_array()
    assert isinstance(values, list)
    assert len(values) > 0
    assert all(isinstance(x, int) for x in values)
    key2 = CollationKey(values, len(values))

    # [3]
    # CollationKey(const CollationKey &other)
    key3 = CollationKey(key)

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

    assert key1.hash_code() != key.hash_code()
    assert key2.hash_code() == key.hash_code()
    assert key3.hash_code() == key.hash_code()

    assert not key1.is_bogus()
    assert not key2.is_bogus()
    assert not key3.is_bogus()

    assert key1 != key
    assert key2 == key
    assert key3 == key
