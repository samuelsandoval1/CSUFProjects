#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class UserLogin {
public:
    UserLogin();
    bool ReadIn(const string& filename);
    size_t NumberOfUsers();
    bool ValidateUser(const string& userName);
    bool AuthenticateUser(const string& userName, const string& passWord);
    string GetPassword(const string& userName);
    size_t WordBucketId(const string& userName);
  private:
    unordered_map<string, string> table_;
};

UserLogin::UserLogin() {
}

bool UserLogin::ReadIn(const string& filename) {
  ifstream users;
  string user_info;
  users.open(filename);
  bool readIn_success = false;
  while (users >> user_info)
  {
      pair<string,string> user_password(user_info, "");
      if (users >> user_info)
      {
        user_password.second = user_info;
        table_.insert(user_password);
        readIn_success = true;
      }
    }
  return readIn_success;
}

size_t UserLogin::NumberOfUsers() {
  return table_.size();
}

bool UserLogin::ValidateUser(const string& userName) {
  std::unordered_map<std::string,std::string>::const_iterator userValid = table_.find (userName);
  if(userValid != table_.end()){
    return true;
  }
  return false;
}

bool UserLogin::AuthenticateUser(const string& userName, const string& passWord) {
  std::unordered_map<std::string,std::string>::const_iterator userSuccess = table_.find(userName);
  if(userSuccess != table_.end()){
    if (userSuccess->second == passWord)
    {
      return true;
    }
  }
  return false;
}

string UserLogin::GetPassword(const string& userName) {
  std::unordered_map<std::string,std::string>::const_iterator got = table_.find (userName);
  if(got != table_.end()){
    return got->second;
  }
  throw range_error("Not implemented yet");
}

size_t UserLogin::WordBucketId(const string& userName) {
  std::unordered_map<std::string,std::string>::const_iterator got = table_.find (userName);
  if(got != table_.end()){
   return table_.bucket(userName);
  }
  throw range_error("No bucket number exists");
}
