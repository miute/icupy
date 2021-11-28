from functools import partial

from icupy.icu import (
    ucsdet_close, ucsdet_detect, ucsdet_detect_all,
    ucsdet_enable_input_filter, ucsdet_get_all_detectable_charsets,
    ucsdet_get_confidence, ucsdet_get_language, ucsdet_get_name,
    ucsdet_get_uchars, ucsdet_is_input_filter_enabled, ucsdet_open,
    ucsdet_set_declared_encoding, ucsdet_set_text,
    uenum_close, uenum_count, uenum_next,
)
from icupy.utils import gc


def test_api():
    with gc(ucsdet_open(), ucsdet_close) as ucsd:
        en = ucsdet_get_all_detectable_charsets(ucsd)
        assert uenum_count(en) > 0
        charsets = iter(partial(uenum_next, en), None)
        assert "UTF-8" in charsets
        uenum_close(en)

        utf8 = (
            "\u3042\u308b\u65e5\u306e\u66ae\u65b9\u306e\u4e8b\u3067\u3042"
            "\u308b\u3002\u4e00\u4eba\u306e\u4e0b\u4eba\u304c\u3001\u7f85"
            "\u751f\u9580\u306e\u4e0b\u3067\u96e8\u3084\u307f\u3092\u5f85"
            "\u3063\u3066\u3044\u305f\u3002\u5e83\u3044\u9580\u306e\u4e0b"
            "\u306b\u306f\u3001\u3053\u306e\u7537\u306e\u307b\u304b\u306b"
            "\u8ab0\u3082\u3044\u306a\u3044\u3002\u305f\u3060\u3001\u6240"
            "\u3005\u4e39\u5857\u306e\u5265\u3052\u305f\u3001\u5927\u304d"
            "\u306a\u5186\u67f1\u306b\u3001\u87cb\u87c0\u304c\u4e00\u5339"
            "\u3068\u307e\u3063\u3066\u3044\u308b\u3002\u7f85\u751f\u9580"
            "\u304c\u3001\u6731\u96c0\u5927\u8def\u306b\u3042\u308b\u4ee5"
            "\u4e0a\u306f\u3001\u3053\u306e\u7537\u306e\u307b\u304b\u306b"
            "\u3082\u3001\u96e8\u3084\u307f\u3092\u3059\u308b\u5e02\u5973"
            "\u7b20\u3084\u63c9\u70cf\u5e3d\u5b50\u304c\u3001\u3082\u3046"
            "\u4e8c\u4e09\u4eba\u306f\u3042\u308a\u305d\u3046\u306a\u3082"
            "\u306e\u3067\u3042\u308b\u3002\u305d\u308c\u304c\u3001\u3053"
            "\u306e\u7537\u306e\u307b\u304b\u306b\u306f\u8ab0\u3082\u3044"
            "\u306a\u3044\u3002"
        )

        ucsdet_set_text(ucsd, utf8, -1)
        ucsm = ucsdet_detect(ucsd)
        assert ucsm
        assert len(ucsdet_get_language(ucsm)) == 0
        assert ucsdet_get_name(ucsm) == "UTF-8"

        text_in = utf8.encode("shift_jis")
        ucsdet_set_text(ucsd, text_in, len(text_in))
        ucsm = ucsdet_detect(ucsd)
        assert ucsm
        assert ucsdet_get_language(ucsm) == "ja"
        assert ucsdet_get_name(ucsm) == "Shift_JIS"

        matches = ucsdet_detect_all(ucsd)
        assert isinstance(matches, list)
        assert len(matches) > 0
        assert ucsdet_get_confidence(matches[0]) == 100
        if len(matches) >= 2:
            assert (ucsdet_get_confidence(matches[0])
                    > ucsdet_get_confidence(matches[1]))
        assert ucsdet_get_language(matches[0]) == "ja"
        assert ucsdet_get_name(matches[0]) == "Shift_JIS"

        buf = ucsdet_get_uchars(matches[0])
        assert isinstance(buf, str)
        assert buf == utf8

        assert not ucsdet_is_input_filter_enabled(ucsd)
        assert not ucsdet_enable_input_filter(ucsd, True)
        assert ucsdet_is_input_filter_enabled(ucsd)

        ucsdet_set_declared_encoding(ucsd, "UTF-8", -1)
