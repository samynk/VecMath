add_executable(VecMathMain main.cpp)

add_custom_command(
    TARGET VecMathMain POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${VecMathLibResourceDir}/resources
    $<TARGET_FILE_DIR:VecMathMain>/resources
    )
    target_link_libraries(VecMathMain VecMathLib)
install(TARGETS VecMathMain DESTINATION bin)