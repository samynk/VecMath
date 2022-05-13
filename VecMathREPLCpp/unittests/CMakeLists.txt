
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

add_executable(UnitTests UnitTestMain.cpp  "AssignmentTests.h" "TestUtil.h" "UnitTestQuaternion.h" "UnitTestVector.h" "UnitTestFunctions.h")
target_link_libraries(UnitTests PRIVATE VecMathLib Catch2::Catch2WithMain)

#configure_file(week1.json week1.json)
#configure_file(week2.json week2.json)
#configure_file(week3.json week3.json)
#configure_file(week4.json week4.json)
#configure_file(week5.json week5.json)

add_custom_target(UnitTestsRun
  COMMAND UnitTests -d yes
  COMMENT "running tests"
)
