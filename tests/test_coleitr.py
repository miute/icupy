from icupy import icu


def test_api():
    coll = icu.Collator.create_instance(icu.Locale("es"))

    # CollationElementIterator *
    # icu::RuleBasedCollator::createCollationElementIterator(
    #       const UnicodeString &source
    # )
    source = icu.UnicodeString("cha")
    it1 = coll.create_collation_element_iterator(source)
    assert isinstance(it1, icu.CollationElementIterator)
    assert it1.get_offset() == 0

    # int32_t icu::CollationElementIterator::next(UErrorCode &status)
    order1 = it1.next()
    assert order1 != icu.CollationElementIterator.NULLORDER
    order2 = it1.next()
    assert order2 != icu.CollationElementIterator.NULLORDER
    order3 = it1.next()
    assert order3 != icu.CollationElementIterator.NULLORDER
    assert it1.next() == icu.CollationElementIterator.NULLORDER

    # int32_t icu::CollationElementIterator::getOffset(void)
    assert it1.get_offset() != 0
    it1.reset()
    assert it1.get_offset() == 0

    # void icu::CollationElementIterator::setOffset(
    #       int32_t newOffset,
    #       UErrorCode &status
    # )
    it1.set_offset(1)
    assert list(it1) == [order1, order2, order3]

    # int32_t icu::CollationElementIterator::getMaxExpansion(int32_t order)
    assert it1.get_max_expansion(order1) == 1
    assert it1.get_max_expansion(order2) == 1
    assert it1.get_max_expansion(order3) == 1

    # UBool icu::CollationElementIterator::isIgnorable(int32_t order)
    assert icu.CollationElementIterator.is_ignorable(order1) is False
    assert icu.CollationElementIterator.is_ignorable(order2) is False
    assert icu.CollationElementIterator.is_ignorable(order3) is False

    # int32_t icu::CollationElementIterator::previous(UErrorCode &status)
    it1.set_offset(3)
    assert it1.get_offset() == 3
    assert it1.previous() == order3
    assert it1.previous() == order2
    assert it1.previous() == order1
    assert it1.previous() == icu.CollationElementIterator.NULLORDER

    # icu::CollationElementIterator::CollationElementIterator(
    #       const CollationElementIterator &other
    # )
    it2 = icu.CollationElementIterator(it1)
    assert it1 == it2
    assert not (it1 != it2)

    # [1]
    # void icu::CollationElementIterator::setText(
    #       CharacterIterator &str,
    #       UErrorCode &status
    # )
    source = icu.StringCharacterIterator(icu.UnicodeString("ca"))
    it2.set_text(source)
    it1.reset()
    assert not (it1 == it2)
    assert it1 != it2

    source = icu.StringCharacterIterator(icu.UnicodeString("cha"))
    it2.set_text(source)
    assert it1 == it2
    assert not (it1 != it2)

    # [2]
    # void icu::CollationElementIterator::setText(
    #       const UnicodeString &str,
    #       UErrorCode &status
    # )
    source = icu.UnicodeString("cha")
    it2.set_text(source)
    assert it1 == it2
    assert not (it1 != it2)

    it2.set_text("cha")
    assert it1 == it2
    assert not (it1 != it2)

    # int32_t icu::CollationElementIterator::primaryOrder(int32_t order)
    assert icu.CollationElementIterator.primary_order(
        order1
    ) == icu.CollationElementIterator.primary_order(it2.next())
    assert icu.CollationElementIterator.primary_order(
        order2
    ) == icu.CollationElementIterator.primary_order(it2.next())
    assert icu.CollationElementIterator.primary_order(
        order3
    ) == icu.CollationElementIterator.primary_order(it2.next())

    # int32_t icu::CollationElementIterator::secondaryOrder(int32_t order)
    it2.reset()
    assert icu.CollationElementIterator.secondary_order(
        order1
    ) == icu.CollationElementIterator.secondary_order(it2.next())
    assert icu.CollationElementIterator.secondary_order(
        order2
    ) == icu.CollationElementIterator.secondary_order(it2.next())
    assert icu.CollationElementIterator.secondary_order(
        order3
    ) == icu.CollationElementIterator.secondary_order(it2.next())

    # int32_t icu::CollationElementIterator::tertiaryOrder(int32_t order)
    it2.reset()
    assert icu.CollationElementIterator.tertiary_order(
        order1
    ) == icu.CollationElementIterator.tertiary_order(it2.next())
    assert icu.CollationElementIterator.tertiary_order(
        order2
    ) == icu.CollationElementIterator.tertiary_order(it2.next())
    assert icu.CollationElementIterator.tertiary_order(
        order3
    ) == icu.CollationElementIterator.tertiary_order(it2.next())

    # int32_t icu::CollationElementIterator::strengthOrder(int32_t order)
    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_PRIMARY)
    assert it1.strength_order(order1) == order1 & 0xFFFF0000
    assert it1.strength_order(order2) == order2 & 0xFFFF0000
    assert it1.strength_order(order3) == order3 & 0xFFFF0000

    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_SECONDARY)
    assert it1.strength_order(order1) == order1 & 0xFFFFFF00
    assert it1.strength_order(order2) == order2 & 0xFFFFFF00
    assert it1.strength_order(order3) == order3 & 0xFFFFFF00

    coll.set_attribute(icu.UColAttribute.UCOL_STRENGTH, icu.UColAttributeValue.UCOL_TERTIARY)
    assert it1.strength_order(order1) == order1
    assert it1.strength_order(order2) == order2
    assert it1.strength_order(order3) == order3
