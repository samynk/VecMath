
antlr_target(VecMathLexer grammar/VecMathLexer.g4 LEXER
             PACKAGE VecMath)
antlr_target(VecMathParser grammar/VecMathParser.g4 PARSER
             PACKAGE VecMath
             DEPENDS_ANTLR VecMathLexer
             COMPILE_FLAGS -lib ${ANTLR_VecMathLexer_OUTPUT_DIR} -listener)

include_directories(${ANTLR_VecMathLexer_OUTPUT_DIR})
include_directories(${ANTLR_VecMathParser_OUTPUT_DIR})

add_executable(VecMath main.cpp
grammar/VecMathLexer.g4
grammar/VecMathParser.g4
               ${ANTLR_VecMathLexer_CXX_OUTPUTS}
               ${ANTLR_VecMathParser_CXX_OUTPUTS} "VecMathListener.h" "VecMathListener.cpp" "FValue.h" "FValue.cpp" "IMatrix.h" "IMatrix.cpp" "Scalar.h" "Scalar.cpp" "Vector2D.h" "Vector2D.cpp" "Vector3D.h" "Vector3D.cpp" "Quaternion.h" "Quaternion.cpp" "VecMathErrorListener.h" "VecMathErrorListener.cpp")
target_link_libraries(VecMath antlr4_static)


message(${ANTLR_VecMathLexer_OUTPUT_DIR})

install(TARGETS VecMath DESTINATION bin)


