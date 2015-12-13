ADD_SUBDIRECTORY(admst)
IF (WIN32)
SET(DIFFCMD C:/cygwin/bin/diff)
ELSE (WIN32)
SET(DIFFCMD /usr/bin/diff)
ENDIF (WIN32)
SET(DIFFARGS --brief -w)


MACRO(adms_build_test arg1)
ADD_TEST(NAME ${arg1}_build
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMAND ${ADMST_SH} ${arg1}
)
ENDMACRO(adms_build_test)
MACRO(adms_build_json arg1)
ADD_TEST(NAME ${arg1}_build_json
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMAND ${CMAKE_BINARY_DIR}/admsJSON/admsJSON -I ../include  -o${arg1}.json -f${arg1}
)
ENDMACRO(adms_build_json)

MACRO(adms_import_json arg1)
ADD_TEST(NAME ${arg1}_import_json
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMAND python ${CMAKE_SOURCE_DIR}/scripts/test_import.py ${arg1}.json
)
set_tests_properties(${arg1}_import_json PROPERTIES DEPENDS ${arg1}_build_json)
ENDMACRO(adms_import_json)

MACRO(adms_diff_json arg1)
ADD_TEST(NAME ${arg1}_diff_json
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMAND ${DIFFCMD} ${DIFFARGS} ${arg1}.json ${arg1}.result.json
)
set_tests_properties(${arg1}_diff_json PROPERTIES DEPENDS ${arg1}_import_json)
ENDMACRO(adms_diff_json)

MACRO(adms_tests arg1)
adms_build_json(${arg1})
adms_import_json(${arg1})
adms_diff_json(${arg1})
ENDMACRO(adms_tests)

### The test directories
ADD_SUBDIRECTORY(ekv)
ADD_SUBDIRECTORY(hicum0)
ADD_SUBDIRECTORY(hicum2)
ADD_SUBDIRECTORY(mextram)
ADD_SUBDIRECTORY(psp102)
