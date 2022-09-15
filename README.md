# Concordance
Info: Repository which contains source code of the concordance challenge
Fire-Up: In order to create a CMake solution, run 'python boot.py' 
Structure: Concordance project consists of the following structure:

-Concordance
 |
 |_exec
 |     |_main.cpp
 |     
 |_src
 |     |_<CONCORDANCE_SOURCES>
 |     |_include
 |              |_<CONCORDANCE_HEADERS>
 | 
 |_test
 |     |_<UNIT_TESTS(gtest)>

 src subdirectory creates a 'concordance library' to be used both by 'exec' executable and unit testings
 Some auxiliary files are also included in this repo (.gitignore and boot.py)
