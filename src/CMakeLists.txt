cmake_minimum_required(VERSION 3.7)

cmake_policy(SET CMP0074 NEW)

project(icu LANGUAGES CXX)

set(DESTDIR ${PROJECT_SOURCE_DIR}/icupy)

add_subdirectory(third_party/pybind11)

find_package(ICU REQUIRED COMPONENTS dt i18n io uc)

pybind11_add_module(
    ${PROJECT_NAME}
    appendable.cpp
    char16ptr.cpp
    coleitr.cpp
    datefmt.cpp
    dtfmtsym.cpp
    dtintrv.cpp
    dtitvfmt.cpp
    dtitvinf.cpp
    dtptngen.cpp
    dtrule.cpp
    fieldpos.cpp
    fmtable.cpp
    format.cpp
    formattedvalue.cpp
    fpositer.cpp
    gregocal.cpp
    idna.cpp
    localebuilder.cpp
    localematcher.cpp
    locid.cpp
    main.cpp
    normalizer2.cpp
    parseerr.cpp
    parsepos.cpp
    rbbi.cpp
    regex.cpp
    resbund.cpp
    schriter.cpp
    smpdtfmt.cpp
    sortkey.cpp
    strenum.cpp
    stsearch.cpp
    tblcoll.cpp
    timezone.cpp
    translit.cpp
    tzfmt.cpp
    tznames.cpp
    tzrule.cpp
    tztrans.cpp
    ubidi.cpp
    ubrk.cpp
    ucal.cpp
    uchar.cpp
    ucnv.cpp
    ucnv_cb.cpp
    ucnv_err.cpp
    ucol.cpp
    ucpmap.cpp
    ucsdet.cpp
    udat.cpp
    udatpg.cpp
    udisplaycontext.cpp
    uenum.cpp
    uformattedvalue.cpp
    uidna.cpp
    uloc.cpp
    uniset.cpp
    unistr.cpp
    unorm2.cpp
    uregex.cpp
    ures.cpp
    uscript.cpp
    usearch.cpp
    uset.cpp
    ustring.cpp
    utext.cpp
    utrans.cpp
    utypes.cpp
    uversion.cpp
    voidptr.cpp
)

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_17
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    MODULE_NAME=${PROJECT_NAME}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ICU::i18n
    ICU::io
    ICU::uc
)

pybind11_extension(${PROJECT_NAME})
pybind11_strip(${PROJECT_NAME})

install(
    TARGETS
    ${PROJECT_NAME}
    LIBRARY
    DESTINATION ${DESTDIR}
)
