
///////////////////////////////////////////////////////////////////////////////
// create_json.hpp
//
// Creates JSON file for unit tests output
//
// Last Modified: Apirl 16, 2020
///////////////////////////////////////////////////////////////////////////////

#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

// Creates JSON file for unit test outputs
class result_json_builder {
    private:
        ptree root;
        ptree tests;
    
    public:
        // Add a test case to the JSON file
        void add_test(std::string testName, int result, int max);

        // Add the final total and max possiable points to the JSON file
        void add_final_result(int result, int max);

        // Generate the JSON file
        void generate_json(std::string filename);
};

// Add a test case to the JSON file
void result_json_builder::add_test(std::string testName, int result, int max) {
    ptree testInfo;
    testInfo.put("testName", testName);
    testInfo.put("testResults", result);
    testInfo.put("testMaxPoints", max);

    this->tests.push_back(std::make_pair("", testInfo));
}

// Add the final total and max possiable points to the JSON file
void result_json_builder::add_final_result(int result, int max) {
    this->root.put("finalTotal", result);
    this->root.put("maxPossiable", max);
}

// Generate the JSON file
void result_json_builder::generate_json(std::string filename) {
    // Make sure we are not creating a empty JSON file
    assert(!this->root.empty() && !this->tests.empty());

    // Add the array of tests to the JSON file
    root.add_child("tests", this->tests);

    // Create the JSON file
    write_json(filename, root);
}