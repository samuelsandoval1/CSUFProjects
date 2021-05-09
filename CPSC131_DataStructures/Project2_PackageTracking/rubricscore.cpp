
///////////////////////////////////////////////////////////////////////////////
// rubricscore.cpp
//
// Program that cross-references googletest XML output against a scoring
// rubric in JSON, and prints out a grade score based on how many tests
// passsed.
//
// Author: Kevin Wortman (kwortman@fullerton.edu)
// Last Modified: February 21, 2020
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "create_json.hpp"

// Default file name for json results
const std::string RESULT_JSON_FILENAME("resultOutput.json");

// Exception thrown when parsing JSON or XML fail.
class parse_exception : public std::exception {
private:
  std::string what_;

public:

  // what is a human-readable description of the problem.
  parse_exception(const std::string& what)
  : what_(what) { }

  virtual const char* what() const noexcept { return what_.c_str(); }
};

// testsuite is a googletest suite, parsed from googletext output XML.
class testsuite {
private:
  std::string name_;
  unsigned tests_, failures_, disabled_, errors_, time_;

public:

  testsuite(std::string&& name,
            unsigned tests,
            unsigned failures,
            unsigned disabled,
            unsigned errors,
            unsigned time)
  : name_(name),
    tests_(tests),
    failures_(failures),
    disabled_(disabled),
    errors_(errors),
    time_(time) { }

  testsuite()
  : testsuite(std::string(), 0, 0, 0, 0, 0) { }

  const std::string& name() const { return name_; }
  unsigned tests() const { return tests_; }
  unsigned failures() const { return failures_; }
  unsigned disabled() const { return disabled_; }
  unsigned errors() const { return errors_; }
  unsigned time() const { return time_; }
};

// test_results is a map from a test suite's name to its testsuite object.
using test_results = std::map<std::string, testsuite>;

// Parse the googletext XML output at googletest_xml_path.
// Throws parse_exception on I/O or parse error.
test_results load_test_results(const std::string& googletest_xml_path) {

  boost::property_tree::ptree tree;
  try {
    boost::property_tree::read_xml(googletest_xml_path, tree);
  } catch (boost::property_tree::xml_parser_error e) {
    throw parse_exception("error parsing XML: " + e.message());
  }

  test_results result;
  try {
    auto testsuites = tree.get_child("testsuites");
    for (auto& child : testsuites) {
      if (child.first == "testsuite") {
        auto& attributes = child.second.get_child("<xmlattr>");
        std::string name = attributes.get("name", "");
        unsigned tests = attributes.get<unsigned>("tests", 0),
                 failures = attributes.get<unsigned>("failures", 0),
                 disabled = attributes.get<unsigned>("disabled", 0),
                 errors = attributes.get<unsigned>("errors", 0),
                 time = attributes.get<unsigned>("time", 0);
        if (name.empty()) {
          throw parse_exception("error parsing XML: a <testsuite> has no name=");
        }
        result[name] = testsuite(std::move(name), tests, failures, disabled, errors, time);
      }
    }
  } catch (boost::property_tree::ptree_error e) {
    throw parse_exception(std::string("error decoding XML: ") + e.what());
  }

  if (result.empty()) {
    throw parse_exception("error parsing XML: does not contain any <testsuite> nodes");
  }

  return result;
}

// A rubric item is one testsuite name and the number of points earned when
// that testsuite passes. It is parsed from a rubric JSON file.
class rubric_item {
private:
  std::string name_;
  unsigned points_;

public:

  rubric_item(std::string&& name, unsigned points)
  : name_(name), points_(points) { }

  rubric_item()
  : rubric_item(std::string(), 0) { }

  const std::string& name() const { return name_; }
  unsigned points() const { return points_; }
};

// A rubric is a vector of rubric_item objects.
using rubric = std::vector<rubric_item>;

// Parse a rubric from the JSON file at json_path.
// The file must contain one non-empty JSON dictionary, where each key is a
// testsuite name, and each value is a positive integer number of points.
// Throws parse_exception on I/O error, parse error, or if the JSON does not
// match that format.
rubric load_rubric(const std::string& json_path) {

  boost::property_tree::ptree tree;
  try {
    boost::property_tree::read_json(json_path, tree);
  } catch (boost::property_tree::json_parser_error e) {
    throw parse_exception("error parsing JSON: " + e.message());
  }

  rubric result;
  for (auto& child : tree) {
    auto& name = child.first;
    auto points = child.second.get_value<unsigned>(0);
    if (0 == points) {
      throw parse_exception("key '" + name + "' does not map to a positive integer");
    }
    result.emplace_back(std::string(name), points);
  }

  if (result.empty()) {
    throw parse_exception("JSON does not contain any rubric items");
  }

  return result;
}

// A rubric_item_score is the result of grading one rubric_item. It contains
// a non-owning pointer to the rubric_item that was evaluated, and a bool value
// for whether the corresponding testsuite passed or not.
class rubric_item_score {
private:
  const rubric_item* item_;
  bool is_correct_;

public:

  // item must not be nullptr.
  rubric_item_score(const rubric_item* item, bool is_correct)
  : item_(item), is_correct_(is_correct) {
    assert(nullptr != item);
    assert(is_valid());
  }

  // Default constructor, so this class can be stored in containers. item is
  // initialized to nullptr, which leaves this object invalid.
  rubric_item_score()
  : item_(nullptr), is_correct_(false) {
    assert(!is_valid());
  }

  // Returns true if this object is valid (has a non-nullptr item pointer).
  bool is_valid() const { return bool(item_); }

  // Accessors.
  // This object must be valid.
  const rubric_item& item() const {
    assert(is_valid());
    return *item_;
  }
  bool is_correct() const {
    assert(is_valid());
    return is_correct_;
  }

  // Return the number of possible points that could be earned for this
  // rubric item.
  // This object must be valid.
  unsigned possible_points() const {
    assert(is_valid());
    return item_->points();
  }

  // Return the number of points that were earned on this rubric item.
  // This object must be valid.
  unsigned earned_points() const {
    assert(is_valid());
    if (is_correct_) {
      return possible_points();
    } else {
      return 0;
    }
  }
};

// An overall rubric score is a vector of rubric_item_score objects.
using rubric_score = std::vector<rubric_item_score>;

// The core business logic: combine a rubric (from instructor-supplied JSON)
// with tests results (from automated googletest) to create a rubric_score
// object representing the grade on the submission.
// Throws parse_error if any of the testsuite names in the rubric do not exist
// in the test_result object. (This is usually a typo in the rubric JSON).
rubric_score evaluate_score(const rubric& the_rubric,
                            const test_results& the_results) {

  rubric_score result;
  // Note that we loop in the same order as the rubric as specified in the JSON
  // file. Presumably that is the order the end-user prefers. The test_result
  // is a std::map that will always be in strict alphabetical order, which is
  // probably not the desired order.
  for (auto& item : the_rubric) {
    auto& name = item.name();
    auto suite_iter = the_results.find(name);
    if (suite_iter == the_results.end()) {
      throw parse_exception("testsuite '" + name +
                            "' from rubric cannot be found in googletest result XML");
    }
    auto& suite = suite_iter->second;
    assert(name == suite.name());
    bool worked = (0 == suite.failures());
    result.emplace_back(&item, worked);
  }
  return result;
}

// Pretty-print a rubric_score and grade summary to standard output.
// the_score must not be empty.
void print_score(const rubric_score& the_score) {

  assert(!the_score.empty());

  // Create JSON file for results
  result_json_builder result_json;

  // horizontal rule
  static const auto line = std::string(79, '=');

  std::cout << line << std::endl
            << "RUBRIC SCORE" << std::endl
            << line << std::endl;

  // calculate the width of the testsuite name column
  unsigned name_width = 0;
  for (auto& score : the_score) {
    name_width = std::max(name_width, unsigned(score.item().name().size()));
  }
  assert(name_width > 0);

  // print each rubric item
  for (auto& score : the_score) {
    std::cout << std::left
              << std::setw(name_width + 4) << score.item().name()
              << std::right
              << std::setw(4) << score.earned_points()
              << " / "
              << std::setw(4) << score.possible_points()
              << std::endl;
    
    // Record test's name, points, and possible points to JSON file
    result_json.add_test(score.item().name(), score.earned_points(), score.possible_points());
  }

  // add up the total score
  unsigned total_earned_points = 0, total_possible_points = 0;
  for (auto& score : the_score) {
    total_earned_points += score.earned_points();
    total_possible_points += score.possible_points();
  }

  // print a summary
  std::cout << line << std::endl
            << "TOTAL = "
            << std::setw(4) << total_earned_points
            << " / "
            << std::setw(4) << total_possible_points
            << std::endl;

  std::cout << line << std::endl << std::endl;

  // Add final results and maximum possible points to JSON file
  result_json.add_final_result(total_earned_points, total_possible_points);

  // Generate JSON file to local directory
  std::string filename = "resultOutput.json";
  result_json.generate_json(filename);
}

int main(int argc, char* argv[]) {

  // convert arguments to std::string
  std::vector<std::string> arguments(argv, argv + argc);

  if (arguments.size() != 3) {
    std::cout << "rubricscore usage:" << std::endl << std::endl
              << "    rubricscore <RUBRIC-JSON-PATH> <GTEST-XML-PATH>"
              << std::endl
              << std::endl;
    return 1;
  }

  auto& json_path = arguments[1],
        xml_path = arguments[2];

  rubric the_rubric;
  try {
    the_rubric = load_rubric(json_path);
  } catch (parse_exception e) {
    std::cerr << "rubricscore: error loading rubric JSON '" << json_path
              << "': " << e.what() << std::endl;
    return 1;
  }

  test_results the_results;
  try {
    the_results = load_test_results(xml_path);
  } catch (parse_exception e) {
    std::cerr << "rubricscore: error loading googletest XML '" << xml_path
              << "': " << e.what() << std::endl;
    return 1;
  }

  rubric_score the_score;
  try {
    the_score = evaluate_score(the_rubric, the_results);
  } catch (parse_exception e) {
    std::cerr << "rubricscore: " << e.what() << std::endl;
    return 1;
  }

  print_score(the_score);

  return 0;
}
