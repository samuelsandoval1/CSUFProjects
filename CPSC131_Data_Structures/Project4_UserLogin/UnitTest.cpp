////////////////////////////////////////////////////////////////////////////////
// UnitTest.cpp
//
// Unit tests for the functionality declared in:
//   UserLogin.h
////////////////////////////////////////////////////////////////////////////////

// ATTENTION STUDENTS: DO NOT MODIFY THIS FILE TO MAKE YOUR CODE PASS. WE USE THE ORIGINAL FILE
// WHEN GRADING YOUR ASSIGNMENT.

#include "gtest/gtest.h"

#include "UserLogin.h"

TEST(ReadIn, ReadIn) {

  // non-existant file
  UserLogin failed;
  EXPECT_FALSE(failed.ReadIn("doesnt_exist.txt"));

  // working file
  UserLogin u;
  ASSERT_TRUE(u.ReadIn("userDetails.txt"));
  ASSERT_EQ(4, u.NumberOfUsers());
}

TEST(ValidateUser, ValidateUser) {

  UserLogin empty;
  EXPECT_FALSE(empty.ValidateUser("alice"));
  EXPECT_FALSE(empty.ValidateUser("bob"));
  EXPECT_FALSE(empty.ValidateUser("jack"));
  EXPECT_FALSE(empty.ValidateUser("jill"));
  EXPECT_FALSE(empty.ValidateUser("mary"));
  EXPECT_FALSE(empty.ValidateUser("bopeep"));

  UserLogin u;
  ASSERT_TRUE(u.ReadIn("userDetails.txt"));
  EXPECT_FALSE(u.ValidateUser("alice"));
  EXPECT_FALSE(u.ValidateUser("bob"));
  EXPECT_TRUE(u.ValidateUser("jack"));
  EXPECT_TRUE(u.ValidateUser("jill"));
  EXPECT_TRUE(u.ValidateUser("mary"));
  EXPECT_TRUE(u.ValidateUser("bopeep"));
}

TEST(AuthenticateUser, AuthenticateUser) {

  UserLogin empty;
  EXPECT_FALSE(empty.AuthenticateUser("alice", "12345"));
  EXPECT_FALSE(empty.AuthenticateUser("bob", "12345"));

  EXPECT_FALSE(empty.AuthenticateUser("jack", "broken.crown"));
  EXPECT_FALSE(empty.AuthenticateUser("jill", "tumblin'down"));
  EXPECT_FALSE(empty.AuthenticateUser("mary", "contrary"));
  EXPECT_FALSE(empty.AuthenticateUser("bopeep", "sheep!lost"));

  UserLogin u;
  ASSERT_TRUE(u.ReadIn("userDetails.txt"));

  // nonexistent user
  EXPECT_FALSE(u.AuthenticateUser("alice", "12345"));
  EXPECT_FALSE(u.AuthenticateUser("bob", "12345"));

  // correct username and password
  EXPECT_TRUE(u.AuthenticateUser("jack", "broken.crown"));
  EXPECT_TRUE(u.AuthenticateUser("jill", "tumblin'down"));
  EXPECT_TRUE(u.AuthenticateUser("mary", "contrary"));
  EXPECT_TRUE(u.AuthenticateUser("bopeep", "sheep!lost"));

  // valid username but invalid password
  EXPECT_FALSE(u.AuthenticateUser("jack", "12345"));
  EXPECT_FALSE(u.AuthenticateUser("jill", "broken.crown"));
  EXPECT_FALSE(u.AuthenticateUser("mary", "tumblin'down"));
  EXPECT_FALSE(u.AuthenticateUser("bopeep", "contrary"));
}

TEST(GetPassword, GetPassword) {
  UserLogin u;
  ASSERT_TRUE(u.ReadIn("userDetails.txt"));

  // nonexistent user
  EXPECT_THROW(u.GetPassword("alice"), std::range_error);
  EXPECT_THROW(u.GetPassword("bob"), std::range_error);

  // user exists
  EXPECT_EQ("broken.crown", u.GetPassword("jack"));
  EXPECT_EQ("tumblin'down", u.GetPassword("jill"));
  EXPECT_EQ("contrary", u.GetPassword("mary"));
  EXPECT_EQ("sheep!lost", u.GetPassword("bopeep"));
}

TEST(WordBucketId, WordBucketId) {

  UserLogin u;
  ASSERT_TRUE(u.ReadIn("userDetails.txt"));

  // nonexistent user
  EXPECT_THROW(u.GetPassword("alice"), std::range_error);
  EXPECT_THROW(u.GetPassword("bob"), std::range_error);

  // user exists
  size_t jack, jill, mary, bopeep;
  ASSERT_NO_THROW(jack = u.WordBucketId("jack"));
  ASSERT_NO_THROW(jill = u.WordBucketId("jill"));
  ASSERT_NO_THROW(mary = u.WordBucketId("mary"));
  ASSERT_NO_THROW(bopeep = u.WordBucketId("bopeep"));

  // buckets are in a reasonable range
  // with only 4 keys, it's inconceivable that there are more than 1000 buckets
  const size_t MAX_BUCKET_COUNT = 1000;
  ASSERT_LT(jack, MAX_BUCKET_COUNT);
  ASSERT_LT(jill, MAX_BUCKET_COUNT);
  ASSERT_LT(mary, MAX_BUCKET_COUNT);
  ASSERT_LT(bopeep, MAX_BUCKET_COUNT);

  // buckets are not all the same
  // It's possible that there is a collision, so we do not expect all buckets
  // to be different. However, they should not all be identical.

  // compute the set of buckets
  std::set<size_t> buckets{jack, jill, mary, bopeep};
  ASSERT_GT(buckets.size(), 1);
}
