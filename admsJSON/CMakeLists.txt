INCLUDE_DIRECTORIES(.)
#ADD_DEFINITIONS()
ADD_LIBRARY(admsElement_JSON adms.cc
verilogaYacc.cc verilogaLex.cc
json_serializer.cc
adms_message.cc
adms_nature.cc
adms_analogfunction.cc
adms_attribute.cc
adms_node.cc
adms_branch.cc
adms_variable.cc
adms_source.cc
adms_math.cc
adms_simulator.cc
adms_admsmain.cc
adms_discipline.cc
adms_module.cc
adms_math.cc
adms_analog.cc
adms_range.cc
adms_lexval.cc
adms_yaccval.cc
preprocessorYacc.cc preprocessorLex.cc preprocessorMain.cc adms_slist.cc
verilogaYacc.cc verilogaLex.cc
json_serializer.cc
adms_instance.cc
adms_block.cc
)

SET_TARGET_PROPERTIES(admsElement_JSON PROPERTIES COMPILE_DEFINITIONS insideElement)


SET_SOURCE_FILES_PROPERTIES(preprocessorYacc.cc preprocessorYacc.hh preprocessorLex.cc PROPERTIES GENERATED TRUE)
SET_SOURCE_FILES_PROPERTIES(verilogaYacc.cc verilogaYacc.hh verilogaLex.cc PROPERTIES GENERATED TRUE)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.cc ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.hh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${BISON_EXECUTABLE}
    ARGS -by -d ${BISON_APIPREFIX}preprocessor${BISON_APIPOSTFIX} -opreprocessorYacc.cc preprocessorYacc.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.y
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorLex.cc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${FLEX_EXECUTABLE}
    ARGS -Ppreprocessor -opreprocessorLex.cc preprocessorLex.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.hh ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorLex.l
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.cc ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.hh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${BISON_EXECUTABLE}
    ARGS -by -d ${BISON_APIPREFIX}veriloga${BISON_APIPOSTFIX} -overilogaYacc.cc verilogaYacc.y
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.y
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/verilogaLex.cc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${FLEX_EXECUTABLE}
    ARGS -Pveriloga -overilogaLex.cc verilogaLex.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.hh ${CMAKE_CURRENT_SOURCE_DIR}/verilogaLex.l
)


ADD_EXECUTABLE(admsJSON admsJSON.cc)
TARGET_LINK_LIBRARIES(admsJSON admsElement_JSON)

