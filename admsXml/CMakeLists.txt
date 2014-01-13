INCLUDE_DIRECTORIES(.)
ADD_DEFINITIONS(-DHAVE_CONFIG_H)
ADD_LIBRARY(admsElement adms.c)
SET_TARGET_PROPERTIES(admsElement PROPERTIES COMPILE_DEFINITIONS insideElement)


ADD_LIBRARY(admsPreprocessor preprocessorYacc.c preprocessorLex.c preprocessorMain.c)
SET_SOURCE_FILES_PROPERTIES(preprocessorYacc.c preprocessorYacc.h preprocessorLex.c PROPERTIES GENERATED TRUE)
SET_TARGET_PROPERTIES(admsPreprocessor PROPERTIES COMPILE_DEFINITIONS insidePreprocessor)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.c ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND bison
    ARGS -by -d -ppreprocessor preprocessorYacc.y
    COMMAND mv
    ARGS -f y.tab.c preprocessorYacc.c 
    COMMAND mv
    ARGS -f y.tab.h preprocessorYacc.h 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorYacc.y
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/preprocessorLex.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND flex
    ARGS -Ppreprocessor -opreprocessorLex.c preprocessorLex.l
    DEPENDS preprocessorYacc.h
)

ADD_LIBRARY(admsVeriloga verilogaYacc.c verilogaLex.c)
SET_SOURCE_FILES_PROPERTIES(verilogaYacc.c verilogaYacc.h verilogaLex.c PROPERTIES GENERATED TRUE)
SET_TARGET_PROPERTIES(admsVeriloga PROPERTIES COMPILE_DEFINITIONS insideVeriloga)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.c ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND bison
    ARGS -by -d -pveriloga verilogaYacc.y
    COMMAND mv
    ARGS -f y.tab.c verilogaYacc.c 
    COMMAND mv
    ARGS -f y.tab.h verilogaYacc.h 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/verilogaYacc.y
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/verilogaLex.c
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND flex
    ARGS -Pveriloga -overilogaLex.c verilogaLex.l
    DEPENDS verilogaYacc.h
)


ADD_LIBRARY(admsAdmstpath admstpathYacc.c)
SET_SOURCE_FILES_PROPERTIES(admstpathYacc.c admstpathYacc.h admstpathLex.c PROPERTIES GENERATED TRUE)
SET_TARGET_PROPERTIES(admsAdmstpath PROPERTIES COMPILE_DEFINITIONS insideadmstpathYacc)
ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/admstpathYacc.c ${CMAKE_CURRENT_SOURCE_DIR}/admstpathYacc.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND bison
    ARGS -by -padmstpath admstpathYacc.y
    COMMAND mv
    ARGS -f y.tab.c admstpathYacc.c 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/admstpathYacc.y
)


ADD_EXECUTABLE(admsXml admsXml.c)
TARGET_LINK_LIBRARIES(admsXml admsElement admsVeriloga admsPreprocessor admsAdmstpath -lm)
ADD_EXECUTABLE(admsCheck admsCheck.c)
TARGET_LINK_LIBRARIES(admsCheck admsElement)

