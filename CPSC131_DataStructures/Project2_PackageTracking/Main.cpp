////////////////////////////////////////////////////////////////////////////////
// Main.cpp
//
// Command-line program that prints package tracking updates with the
// PackageStatus class.
////////////////////////////////////////////////////////////////////////////////

#include <iostream> // cout, endl
#include <string> // stoi

#include "PackageStatus.h"
#include "Serialize.h"

using namespace std;
using namespace PackageTracking;

// Methods of selecting updates
enum class How { Previous, Following, All };

// Print usage information on usage error.
void PrintUsage() {
  cout << "Usage:" << endl << endl
       << "    ./track <FILENAME> <HOW> <INDEX>" << endl << endl
       << "<FILENAME>: a .json file containing tracking info" << endl
       << "<HOW>: one of the following: previous following all" << endl
       << "<INDEX>: the index (starting from 0) to prent before/after (ignored when HOW=all)" << endl << endl;
}

int main(int argc, char* argv[]) {

  // check number of commandline arguments
  if (argc != 4) {
    PrintUsage();
    return 1;
  }

  // copy arguments into string objects
  string filename = argv[1],
    how_string = argv[2],
    index_string = argv[3];

  // read the JSON file
  PackageStatus status;
  try {
    status = PackageStatusFromJSON(filename);
  } catch (std::invalid_argument e) {
    // read failure
    cout << "error: " << e.what() << endl;
    return 1;
  }

  // decode the how argument
  How how;
  if (how_string == "previous") {
    how = How::Previous;
  } else if (how_string == "following") {
    how = How::Following;
  } else if (how_string == "all") {
    how = How::All;
  } else {
    // invalid how
    PrintUsage();
    return 1;
  }

  // decode the index argument
  int index;
  try {
    index = stoi(index_string);
  } catch (exception e) {
    cout << "error: invalid index" << endl;
    return 1;
  }

  // check index range
  if ((index < 0) || (index >= status.Size())) {
    cout << "error: index out of range" << endl;
    return 1;
  }

  // move cursor to chosen index
  for (int i = 0; i < index; i++) {
    status.MoveCursorForward();
  }

  // create output
  string output;
  switch (how) {
  case How::Previous:
    output = status.DescribePreviousUpdates();
    break;
  case How::Following:
    output = status.DescribeFollowingUpdates();
    break;
  case How::All:
    output = status.DescribeAllUpdates();
    break;
  }

  // print output
  cout << output;

  // done, success
  return 0;
}
