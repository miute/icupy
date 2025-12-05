cmake_minimum_required(VERSION 3.15...4.0)

cmake_policy(SET CMP0074 NEW)

project(icu LANGUAGES CXX)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(PYBIND11_FINDPYTHON ON)
set(DESTDIR icupy CACHE STRING "The directory where the binaries will be written. (e.g., path/to/icupy/src/icupy)")
message(STATUS "DESTDIR: ${DESTDIR}")

if(WIN32)
    # Set the ICU_ROOT environment variable to the ICU installation directory
    if(NOT DEFINED ENV{ICU_ROOT})
        set(ENV{ICU_ROOT} C:/icu)
    endif()
    message(STATUS "ICU_ROOT: $ENV{ICU_ROOT}")
endif()

if(MSVC)
    add_compile_options(/MP)    # Build with multiple processes
endif()

find_package(ICU REQUIRED COMPONENTS dt i18n io uc)
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module)
add_subdirectory(src/third_party/pybind11)

python_add_library(
    ${PROJECT_NAME}
    MODULE
    src/alphaindex.cpp
    src/appendable.cpp
    src/caniter.cpp
    src/casemap.cpp
    src/char16ptr.cpp
    src/coleitr.cpp
    src/compactdecimalformat.cpp
    src/currpinf.cpp
    src/currunit.cpp
    src/datefmt.cpp
    src/dcfmtsym.cpp
    src/decimfmt.cpp
    src/displayoptions.cpp
    src/dtfmtsym.cpp
    src/dtintrv.cpp
    src/dtitvfmt.cpp
    src/dtitvinf.cpp
    src/dtptngen.cpp
    src/dtrule.cpp
    src/edits.cpp
    src/errorcode.cpp
    src/fieldpos.cpp
    src/filteredbrk.cpp
    src/fmtable.cpp
    src/format.cpp
    src/formattedvalue.cpp
    src/fpositer.cpp
    src/gender.cpp
    src/gregocal.cpp
    src/icudataver.cpp
    src/idna.cpp
    src/listformatter.cpp
    src/localebuilder.cpp
    src/localematcher.cpp
    src/locdspnm.cpp
    src/locid.cpp
    src/main.cpp
    src/measfmt.cpp
    src/measunit.cpp
    src/measure.cpp
    src/messagepattern.cpp
    src/msgfmt.cpp
    src/normalizer2.cpp
    src/nounit.cpp
    src/numberformatter.cpp
    src/numberrangeformatter.cpp
    src/numfmt.cpp
    src/numsys.cpp
    src/parseerr.cpp
    src/parsepos.cpp
    src/plurfmt.cpp
    src/plurrule.cpp
    src/rbbi.cpp
    src/rbnf.cpp
    src/regex.cpp
    src/region.cpp
    src/reldatefmt.cpp
    src/resbund.cpp
    src/schriter.cpp
    src/scientificnumberformatter.cpp
    src/selfmt.cpp
    src/simplenumberformatter.cpp
    src/smpdtfmt.cpp
    src/sortkey.cpp
    src/strenum.cpp
    src/stringoptions.cpp
    src/stsearch.cpp
    src/tblcoll.cpp
    src/timezone.cpp
    src/tmunit.cpp
    src/translit.cpp
    src/tzfmt.cpp
    src/tznames.cpp
    src/tzrule.cpp
    src/tztrans.cpp
    src/ubidi.cpp
    src/ubiditransform.cpp
    src/ubrk.cpp
    src/ucal.cpp
    src/uchar.cpp
    src/ucnv.cpp
    src/ucnv_cb.cpp
    src/ucnv_err.cpp
    src/ucol.cpp
    src/ucpmap.cpp
    src/ucsdet.cpp
    src/ucurr.cpp
    src/udat.cpp
    src/udatpg.cpp
    src/udisplaycontext.cpp
    src/udisplayoptions.cpp
    src/uenum.cpp
    src/uformattedvalue.cpp
    src/ugender.cpp
    src/uidna.cpp
    src/uldnames.cpp
    src/ulistformatter.cpp
    src/uloc.cpp
    src/ulocdata.cpp
    src/uniset.cpp
    src/unistr.cpp
    src/unistrvec.cpp
    src/unorm2.cpp
    src/unounclass.cpp
    src/unum.cpp
    src/unumberformatter.cpp
    src/unumberrangeformatter.cpp
    src/upluralrules.cpp
    src/uregex.cpp
    src/uregion.cpp
    src/ureldatefmt.cpp
    src/ures.cpp
    src/uscript.cpp
    src/usearch.cpp
    src/uset.cpp
    src/usetiter.cpp
    src/ushape.cpp
    src/usimplenumberformatter.cpp
    src/uspoof.cpp
    src/usprep.cpp
    src/ustring.cpp
    src/utext.cpp
    src/utf.cpp
    src/utfiterator.cpp
    src/utmscale.cpp
    src/utrans.cpp
    src/utypes.cpp
    src/uversion.cpp
    src/voidptr.cpp
    WITH_SOABI
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

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${Python3_INCLUDE_DIRS}
    src/third_party/pybind11/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    pybind11::headers
    ICU::i18n
    ICU::io
    ICU::uc
)

install(
    TARGETS
    ${PROJECT_NAME}
    LIBRARY
    DESTINATION ${DESTDIR}
)
