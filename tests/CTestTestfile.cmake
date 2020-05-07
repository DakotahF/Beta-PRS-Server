# CMake generated Testfile for 
# Source directory: /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/tests
# Build directory: /net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(PRS_PRS "/usr/bin/cmake" "-DPRSCALC=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/tests/src/prs-calc" "-DTESTPRS=/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/tests/testprs" "-P" "/net/hunt/home/kotah/prs-server-beta/PRS-methods/prs-toolchain/tests/test_01/prs_compare.cmake")
