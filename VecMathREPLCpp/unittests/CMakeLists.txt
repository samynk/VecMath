
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

add_executable(UnitTests UnitTestMain.cpp  "AssignmentTests.h" "TestUtil.h" "UnitTestQuaternion.h" "UnitTestVector.h" "UnitTestFunctions.h" "UnitTestOperator.h" "UnitTestComplex.h" "UnitTestConstants.h" "UnitTestMemory.h")
target_link_libraries(UnitTests PRIVATE VecMathLib Catch2::Catch2WithMain)

if (UNIX)
find_package(Threads REQUIRED)
target_link_libraries(UnitTests PRIVATE Threads::Threads)
endif()


add_custom_target(UnitTestsRun
  COMMAND UnitTests -d yes
  COMMENT "running tests"
)
