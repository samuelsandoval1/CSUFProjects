
################################################################################
# specific to this project
################################################################################

TEST_CPP = UnitTest.cpp
TEST_EXECUTABLE = UnitTest
TEST_XML = UnitTest.xml
RUBRIC_JSON = rubric.json

build: rubricscore UnitTest

UnitTest: dependencies valetparking.o UnitTest.cpp
	clang++ --std=c++17 -Wall -g -lpthread -lgtest_main -lgtest -lpthread valetparking.o UnitTest.cpp -o UnitTest

valetparking.o: valetparking.h valetparking.cpp
	clang++ --std=c++17 -Wall -c -g valetparking.cpp -o valetparking.o

clean:
	rm -f rubricscore ${TEST_XML} resultOutput.json valetparking.o UnitTest

################################################################################
# boilerplate
################################################################################

test: rubricscore ${TEST_EXECUTABLE}
	# delete stale test results from previous runs
	rm -f ${TEST_XML}
	# || true allows make to continue the build even if some tests fail
	./${TEST_EXECUTABLE} --gtest_output=xml:${TEST_XML} || true
	./rubricscore ${RUBRIC_JSON} ${TEST_XML}

rubricscore: dependencies create_json.hpp rubricscore.cpp
	clang++ --std=c++11 -Wall rubricscore.cpp -o rubricscore

dependencies: /usr/include/nlohmann/json.hpp /usr/include/boost/property_tree/xml_parser.hpp

/usr/include/nlohmann/json.hpp:
	@echo -e "nlohmann::json library not installed\n"
	@echo -e "Installing nlohmann-json3-dev. Please provide the password when asked\n"
	@sudo apt-get -y install nlohmann-json3-dev

/usr/include/boost/property_tree/xml_parser.hpp:
	@echo -e "libboost library not installed\n"
	@echo -e "Installing libboost-dev. Please provide the password when asked\n"
	@sudo apt-get -y install libboost-dev

/usr/lib/libgtest.a:
	@echo -e "google test library not installed\n"
	@echo -e "Installing libgtest-dev. Please provide the password when asked\n"
	@sudo apt-get -y install libgtest-dev cmake
	@sudo apt-get install cmake # install cmake
	@echo -e "\nConfiguring libgtest-dev\n"
	@cd /usr/src/gtest; sudo cmake CMakeLists.txt; sudo make; sudo cp *.a /usr/lib
	@echo -e "Finished installing google test library\n"

