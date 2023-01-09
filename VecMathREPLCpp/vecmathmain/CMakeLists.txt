add_executable(VecMathMain main.cpp)

add_custom_command(
    TARGET VecMathMain POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory  $<TARGET_FILE_DIR:VecMathMain>/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${VECMATH_COMMON_RESOURCE_DIR}
    $<TARGET_FILE_DIR:VecMathMain>/resources
    )
target_link_libraries(VecMathMain PUBLIC VecMathLib)
if (UNIX)
find_package(Threads REQUIRED)
target_link_libraries(VecMathMain PRIVATE Threads::Threads)
endif()
#set_target_properties(VecMathMain PROPERTIES OUTPUT_NAME "VecMath")
