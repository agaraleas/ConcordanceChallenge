include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(TestFiles 
	"ConcordanceTest.cpp" 
	"OutputFormattingsTest.cpp"
	"TextDocumentTravellerTest.cpp"
)

add_executable(ConcordanceTest ${TestFiles})

target_link_libraries(ConcordanceTest
  PRIVATE
  GTest::GTest
  Concordance)

add_test(ConcordanceGTests ConcordanceTest)