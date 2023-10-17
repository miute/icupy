cmake_minimum_required(VERSION 3.7)

cmake_policy(SET CMP0074 NEW)

project(icu LANGUAGES CXX)

set(DESTDIR ${PROJECT_SOURCE_DIR}/icupy)

add_subdirectory(third_party/pybind11)

find_package(ICU REQUIRED COMPONENTS dt i18n io uc)

pybind11_add_module(
    ${PROJECT_NAME}
    alphaindex.cpp
    appendable.cpp
    caniter.cpp
    casemap.cpp
    char16ptr.cpp
    coleitr.cpp
    compactdecimalformat.cpp
    currpinf.cpp
    currunit.cpp
    datefmt.cpp
    dcfmtsym.cpp
    decimfmt.cpp
    displayoptions.cpp
    dtfmtsym.cpp
    dtintrv.cpp
    dtitvfmt.cpp
    dtitvinf.cpp
    dtptngen.cpp
    dtrule.cpp
    edits.cpp
    errorcode.cpp
    fieldpos.cpp
    filteredbrk.cpp
    fmtable.cpp
    format.cpp
    formattedvalue.cpp
    fpositer.cpp
    gender.cpp
    gregocal.cpp
    icudataver.cpp
    idna.cpp
    listformatter.cpp
    localebuilder.cpp
    localematcher.cpp
    locdspnm.cpp
    locid.cpp
    main.cpp
    measfmt.cpp
    measunit.cpp
    measure.cpp
    messagepattern.cpp
    msgfmt.cpp
    normalizer2.cpp
    nounit.cpp
    numberformatter.cpp
    numberrangeformatter.cpp
    numfmt.cpp
    numsys.cpp
    parseerr.cpp
    parsepos.cpp
    plurfmt.cpp
    plurrule.cpp
    rbbi.cpp
    rbnf.cpp
    regex.cpp
    region.cpp
    reldatefmt.cpp
    resbund.cpp
    schriter.cpp
    scientificnumberformatter.cpp
    selfmt.cpp
    simplenumberformatter.cpp
    smpdtfmt.cpp
    sortkey.cpp
    strenum.cpp
    stringoptions.cpp
    stsearch.cpp
    tblcoll.cpp
    timezone.cpp
    tmunit.cpp
    translit.cpp
    tzfmt.cpp
    tznames.cpp
    tzrule.cpp
    tztrans.cpp
    ubidi.cpp
    ubiditransform.cpp
    ubrk.cpp
    ucal.cpp
    uchar.cpp
    ucnv.cpp
    ucnv_cb.cpp
    ucnv_err.cpp
    ucol.cpp
    ucpmap.cpp
    ucsdet.cpp
    ucurr.cpp
    udat.cpp
    udatpg.cpp
    udisplaycontext.cpp
    udisplayoptions.cpp
    uenum.cpp
    uformattedvalue.cpp
    ugender.cpp
    uidna.cpp
    uldnames.cpp
    ulistformatter.cpp
    uloc.cpp
    ulocdata.cpp
    uniset.cpp
    unistr.cpp
    unistrvec.cpp
    unorm2.cpp
    unounclass.cpp
    unum.cpp
    unumberformatter.cpp
    unumberrangeformatter.cpp
    upluralrules.cpp
    uregex.cpp
    uregion.cpp
    ureldatefmt.cpp
    ures.cpp
    uscript.cpp
    usearch.cpp
    uset.cpp
    usetiter.cpp
    ushape.cpp
    usimplenumberformatter.cpp
    uspoof.cpp
    usprep.cpp
    ustring.cpp
    utext.cpp
    utmscale.cpp
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

install(
    TARGETS
    ${PROJECT_NAME}
    LIBRARY
    DESTINATION ${DESTDIR}
)
