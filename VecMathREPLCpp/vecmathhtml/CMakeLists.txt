add_executable(VecMathHtml main.cpp)
add_custom_command(
    TARGET VecMathHtml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory  $<TARGET_FILE_DIR:VecMathMain>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${VECMATH_COMMON_RESOURCE_DIR}
    $<TARGET_FILE_DIR:VecMathMain>/resources
    )
target_link_libraries(VecMathHtml PUBLIC VecMathLib)
if (UNIX)
find_package(Threads REQUIRED)
target_link_libraries(VecMathHtml PRIVATE Threads::Threads)
endif()
#set_target_properties(VecMathMain PROPERTIES OUTPUT_NAME "VecMath")
set_target_properties(VecMathHtml PROPERTIES COMPILE_FLAGS
"-sNO_DISABLE_EXCEPTION_CATCHING")
set_target_properties(VecMathHtml PROPERTIES LINK_FLAGS 
"-sEXPORTED_FUNCTIONS=_prompt,_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap -sNO_DISABLE_EXCEPTION_CATCHING --preload-file \"${CMAKE_SOURCE_DIR}/../VecMathCommon/resources\"@resources")