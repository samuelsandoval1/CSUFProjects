
################################################################################
# specific to this project
################################################################################

TEST_CPP = UnitTest.cpp
TEST_EXECUTABLE = UnitTest
TEST_XML = UnitTest.xml
RUBRIC_JSON = rubric.json

build: rubricscore UnitTest track

track: dependencies ShippingUpdate.o PackageStatus.o Serialize.o Main.cpp
	clang++ --std=c++17 -Wall -g ShippingUpdate.o PackageStatus.o Serialize.o Main.cpp -o track

UnitTest: dependencies ShippingUpdate.o PackageStatus.o Serialize.o UnitTest.cpp
	clang++ --std=c++17 -Wall -g -lpthread -lgtest_main -lgtest -lpthread ShippingUpdate.o PackageStatus.o Serialize.o UnitTest.cpp -o UnitTest

ShippingUpdate.o: ShippingUpdate.h ShippingUpdate.cpp
	clang++ --std=c++17 -Wall -c -g ShippingUpdate.cpp -o ShippingUpdate.o

PackageStatus.o: ShippingUpdate.h PackageStatus.h PackageStatus.cpp
	clang++ --std=c++17 -Wall -c -g PackageStatus.cpp -o PackageStatus.o

Serialize.o: /usr/include/nlohmann/json.hpp ShippingUpdate.h PackageStatus.h Serialize.h Serialize.cpp
	clang++ --std=c++17 -Wall -c -g Serialize.cpp -o Serialize.o

clean:
	rm -f rubricscore ${TEST_XML} resultOutput.json ShippingUpdate.o PackageStatus.o Serialize.o UnitTest track

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

