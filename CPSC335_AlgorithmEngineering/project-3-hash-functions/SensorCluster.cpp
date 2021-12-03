#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include<string>  
#include "SensorCluster.hpp"

using std::string;
using std::ifstream;
using std::cout;
using std::endl;

// function to return the hash value based on the first digit
unsigned int hashfct1(unsigned int nic) {
  std::string s = std::to_string(nic);
  char firstDig = s[0];
  std::stringstream strValue;
  strValue << firstDig;

  unsigned int result;
  strValue >> result;
  return result;
}

// function to return the hash value based on the second digit
unsigned int hashfct2(unsigned int nic) {
  std::string s = std::to_string(nic);
  char digit = s[1];
  std::stringstream strValue;
  strValue << digit;

  unsigned int result;
  strValue >> result;
  return result;
}

// function to return the hash value based on the third digit
unsigned int hashfct3(unsigned int nic) {
  std::string s = std::to_string(nic);
  char digit = s[2];
  std::stringstream strValue;
  strValue << digit;

  unsigned int result;
  strValue >> result;
  return result;
}

// function to return the hash value based on the fourth digit
unsigned int hashfct4(unsigned int nic) {
  std::string s = std::to_string(nic);
  char digit = s[3];
  std::stringstream strValue;
  strValue << digit;

  unsigned int result;
  strValue >> result;
  return result;
}

// function to return the hash value based on the fifth digit
unsigned int hashfct5(unsigned int nic) {
  std::string s = std::to_string(nic);
  char digit = s[4];
  std::stringstream strValue;
  strValue << digit;

  unsigned int result;
  strValue >> result;
  return result;
}

// function to return the hash value based on the fourth digit
unsigned int hashfct6(unsigned int nic) {
  std::string s = std::to_string(nic);
  char digit = s[5];
  std::stringstream strValue;
  strValue << digit;

  unsigned int result;
  strValue >> result;
  return result;
}

// Constructor for struct Item
Item::Item(string itemName, unsigned int nic):itemName_(itemName), nic_(nic)
{};

// Load information from a text file with the given filename
// THIS FUNCTION IS COMPLETE
void SensorNIC::readTextfile(string filename) {
  ifstream myfile(filename);

  if (myfile.is_open()) {
    cout << "Successfully opened file " << filename << endl;
    string itemName;
    unsigned int nic;
    while (myfile >> itemName >> nic) {
			if (itemName.size() > 0)
      	addItem(itemName, nic);
    }
    myfile.close();
  }
  else
    throw std::invalid_argument("Could not open file " + filename);
}

void SensorNIC::addItem(string itemName, unsigned int nic) {
  hT1[nic] = Item(itemName, nic);
  hT2[nic] = Item(itemName, nic);
  hT3[nic] = Item(itemName, nic);
  hT4[nic] = Item(itemName, nic);
  hT5[nic] = Item(itemName, nic);
  hT6[nic] = Item(itemName, nic);
}

bool SensorNIC::removeItem(unsigned int nic) {
    if(hT1.find(nic) != hT1.end()){
      hT1.erase(nic);
    } else {
      return false;
    }
    if(hT2.find(nic) != hT2.end()){
      hT2.erase(nic);
    } else {
      return false;
    }
    if(hT3.find(nic) != hT3.end()){
      hT3.erase(nic);
    } else {
      return false;
    }
    if(hT4.find(nic) != hT4.end()){
      hT4.erase(nic);
    } else {
      return false;
    }
    if(hT5.find(nic) != hT5.end()){
      hT5.erase(nic);
    } else {
      return false;
    }
    if(hT6.find(nic) != hT6.end()){
      hT6.erase(nic);
    } else {
      return false;
    }
    return true;
   
}

unsigned int SensorNIC::loadBalance(CustomHashTable table){
  unsigned int biggestBucket = table.bucket_size(0);
  unsigned int smallestBucket = table.bucket_size(0);
	 for (unsigned i=0; i<10; ++i) {
    if(table.bucket_size(i) > biggestBucket){
      biggestBucket = table.bucket_size(i);
    }
    if(table.bucket_size(i) < smallestBucket){
      smallestBucket = table.bucket_size(i);
    }
  }
	return biggestBucket - smallestBucket;
}

unsigned int SensorNIC::bestHashing() {
  std::vector<unsigned int> loadList;
  
  loadList.push_back(loadBalance(hT1));
  loadList.push_back(loadBalance(hT2));
  loadList.push_back(loadBalance(hT3));
  loadList.push_back(loadBalance(hT4));
  loadList.push_back(loadBalance(hT5));
  loadList.push_back(loadBalance(hT6));
  unsigned int minLoadIndex = 0;
  unsigned int minLoad = loadList[0];

  for(int i = 0; i < loadList.size(); i++){
    if(!minLoad){
      minLoad = loadList[i];
      minLoadIndex = i;
    }
    if(loadList[i] < minLoad){
      minLoad = loadList[i];
      minLoadIndex = i;
    }
  }
  return minLoadIndex + 1;
}

// ALREADY COMPLETED
size_t SensorNIC::size() {
    if ((hT1.size() != hT2.size()) || (hT1.size() != hT3.size()) || (hT1.size() != hT4.size()) || (hT1.size() != hT5.size())|| (hT1.size() != hT6.size()) )
  	throw std::length_error("Hash table sizes are not the same");
    
	return hT1.size();
}
