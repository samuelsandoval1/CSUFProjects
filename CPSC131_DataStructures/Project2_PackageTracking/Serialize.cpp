////////////////////////////////////////////////////////////////////////////////
// Serialize.cpp
//
// Reading a PackageStatus object from a JSON file.
////////////////////////////////////////////////////////////////////////////////

#include <fstream> // std::ifstream

#include <nlohmann/json.hpp>

#include "Serialize.h"

namespace PackageTracking {

PackageStatus PackageStatusFromJSON(const std::string &path) {

  using json = nlohmann::json;

  std::ifstream f(path);
  if (!f) {
    throw std::invalid_argument("could not open \"" + path + "\"");
  }

  json root;
  try {
    f >> root;
  } catch (json::parse_error e) {
    throw std::invalid_argument("JSON parse error");
  }

  try {

    std::string tracking_number = root.at("tracking_number");
    json updates = root.at("updates");

    PackageStatus result(tracking_number);

    for (auto &update : updates) {
      std::string description = update[0], location = update[1];
      time_t timestamp = update[2];

      result.AddUpdate(description, location, timestamp);
    }

    return result;

  } catch (json::exception e) {
    throw std::invalid_argument("JSON is missing entries");
  }
}

} // namespace PackageTracking
