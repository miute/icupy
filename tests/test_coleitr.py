from icupy.icu import (
    Collator, CollationElementIterator, Locale, StringCharacterIterator,
    UColAttribute, UColAttributeValue, UnicodeString,
)


def test_api():
    coll = Collator.create_instance(Locale("es"))

    # CollationElementIterator *
    # icu::RuleBasedCollator::createCollationElementIterator(
    #       const UnicodeString &source
    # )
    source = UnicodeString("cha")
    it1 = coll.create_collation_element_iterator(source)
    assert isinstance(it1, CollationElementIterator)
    assert it1.get_offset() == 0

    # int32_t icu::CollationElementIterator::next(UErrorCode &status)
    order1 = it1.next()
    assert order1 != CollationElementIterator.NULLORDER
    order2 = it1.next()
    assert order2 != CollationElementIterator.NULLORDER
    order3 = it1.next()
    assert order3 != CollationElementIterator.NULLORDER
    assert it1.next() == CollationElementIterator.NULLORDER

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
    assert not CollationElementIterator.is_ignorable(order1)
    assert not CollationElementIterator.is_ignorable(order2)
    assert not CollationElementIterator.is_ignorable(order3)

    # int32_t icu::CollationElementIterator::previous(UErrorCode &status)
    it1.set_offset(3)
    assert it1.get_offset() == 3
    assert it1.previous() == order3
    assert it1.previous() == order2
    assert it1.previous() == order1
    assert it1.previous() == CollationElementIterator.NULLORDER

    # icu::CollationElementIterator::CollationElementIterator(
    #       const CollationElementIterator &other
    # )
    it2 = CollationElementIterator(it1)
    assert it1 == it2
    assert not (it1 != it2)

    # [1]
    # void icu::CollationElementIterator::setText(
    #       CharacterIterator &str,
    #       UErrorCode &status
    # )
    source = StringCharacterIterator(UnicodeString("ca"))
    it2.set_text(source)
    it1.reset()
    assert not (it1 == it2)
    assert it1 != it2

    source = StringCharacterIterator(UnicodeString("cha"))
    it2.set_text(source)
    assert it1 == it2
    assert not (it1 != it2)

    # [2]
    # void icu::CollationElementIterator::setText(
    #       const UnicodeString &str,
    #       UErrorCode &status
    # )
    source = UnicodeString("cha")
    it2.set_text(source)
    assert it1 == it2
    assert not (it1 != it2)

    it2.set_text("cha")
    assert it1 == it2
    assert not (it1 != it2)

    # int32_t icu::CollationElementIterator::primaryOrder(int32_t order)
    assert (CollationElementIterator.primary_order(order1)
            == CollationElementIterator.primary_order(it2.next()))
    assert (CollationElementIterator.primary_order(order2)
            == CollationElementIterator.primary_order(it2.next()))
    assert (CollationElementIterator.primary_order(order3)
            == CollationElementIterator.primary_order(it2.next()))

    # int32_t icu::CollationElementIterator::secondaryOrder(int32_t order)
    it2.reset()
    assert (CollationElementIterator.secondary_order(order1)
            == CollationElementIterator.secondary_order(it2.next()))
    assert (CollationElementIterator.secondary_order(order2)
            == CollationElementIterator.secondary_order(it2.next()))
    assert (CollationElementIterator.secondary_order(order3)
            == CollationElementIterator.secondary_order(it2.next()))

    # int32_t icu::CollationElementIterator::tertiaryOrder(int32_t order)
    it2.reset()
    assert (CollationElementIterator.tertiary_order(order1)
            == CollationElementIterator.tertiary_order(it2.next()))
    assert (CollationElementIterator.tertiary_order(order2)
            == CollationElementIterator.tertiary_order(it2.next()))
    assert (CollationElementIterator.tertiary_order(order3)
            == CollationElementIterator.tertiary_order(it2.next()))

    # int32_t icu::CollationElementIterator::strengthOrder(int32_t order)
    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_PRIMARY)
    assert it1.strength_order(order1) == order1 & 0xffff0000
    assert it1.strength_order(order2) == order2 & 0xffff0000
    assert it1.strength_order(order3) == order3 & 0xffff0000

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_SECONDARY)
    assert it1.strength_order(order1) == order1 & 0xffffff00
    assert it1.strength_order(order2) == order2 & 0xffffff00
    assert it1.strength_order(order3) == order3 & 0xffffff00

    coll.set_attribute(UColAttribute.UCOL_STRENGTH,
                       UColAttributeValue.UCOL_TERTIARY)
    assert it1.strength_order(order1) == order1
    assert it1.strength_order(order2) == order2
    assert it1.strength_order(order3) == order3
