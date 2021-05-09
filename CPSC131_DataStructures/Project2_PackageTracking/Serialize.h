////////////////////////////////////////////////////////////////////////////////
// Serialize.h
//
// Reading a PackageStatus object from a JSON file.
////////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdexcept> // std::invalid_argument
#include <string> // std::string

#include <nlohmann/json.hpp>

#include "PackageStatus.h"

namespace PackageTracking {

  // throws std::invalid_argument if the file cannot be loaded
  PackageStatus PackageStatusFromJSON(const std::string& path);
  
}

#endif
