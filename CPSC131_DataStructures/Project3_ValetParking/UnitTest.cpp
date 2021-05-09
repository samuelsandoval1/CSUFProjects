////////////////////////////////////////////////////////////////////////////////
// UnitTest.cpp
//
// Unit tests for the functionality declared in:
//   valetparking.h
////////////////////////////////////////////////////////////////////////////////

// ATTENTION STUDENTS: DO NOT MODIFY THIS FILE TO MAKE YOUR CODE PASS. WE USE THE ORIGINAL FILE
// WHEN GRADING YOUR ASSIGNMENT.

#include "gtest/gtest.h"

#include "valetparking.h"

TEST(Empty, Empty) { // passage_1
  ValetParking v;
  
  EXPECT_EQ(1, v.TotalStallSpaces());
  EXPECT_EQ(1, v.AvailableStallSpaces());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(0.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());
  EXPECT_TRUE(v.ParkingEmpty());
  EXPECT_FALSE(v.ParkingFull());
  EXPECT_TRUE(v.StallEmpty());
  EXPECT_FALSE(v.StallFull());
  EXPECT_TRUE(v.QueueEmpty());
  EXPECT_FALSE(v.QueueFull());
}

TEST(OneCheckIn, OneCheckIn) { // passage_2
  ValetParking v;
  ASSERT_EQ(1, v.CheckIn());
  
  EXPECT_EQ(1, v.TotalStallSpaces());
  EXPECT_EQ(0, v.AvailableStallSpaces());
  EXPECT_EQ(1, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(5.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());
  EXPECT_FALSE(v.ParkingEmpty());
  EXPECT_FALSE(v.ParkingFull());
  EXPECT_FALSE(v.StallEmpty());
  EXPECT_TRUE(v.StallFull());
  EXPECT_TRUE(v.QueueEmpty());
  EXPECT_FALSE(v.QueueFull());
}

TEST(CheckInCheckOut, CheckInCheckOut) { // passage_3
  ValetParking v;
  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(1, v.StallNumber(1));
  ASSERT_TRUE(v.CheckOut(1, 1));
  
  EXPECT_EQ(1, v.TotalStallSpaces());
  EXPECT_EQ(1, v.AvailableStallSpaces());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(1, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(5.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());
  EXPECT_FALSE(v.ParkingEmpty());
  EXPECT_FALSE(v.ParkingFull());
  EXPECT_TRUE(v.StallEmpty());
  EXPECT_FALSE(v.StallFull());
  EXPECT_FALSE(v.QueueEmpty());
  EXPECT_TRUE(v.QueueFull());
}

TEST(ThreeStalls, ThreeStalls) {
  // passage_5
  ValetParking v(3, 2, 6, 0, 5);
  
  ASSERT_EQ(1, v.CheckIn());
  EXPECT_EQ(6, v.TotalStallSpaces());
  EXPECT_EQ(5, v.AvailableStallSpaces());
  EXPECT_EQ(1, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());

  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(2, v.CheckIn());
  ASSERT_EQ(2, v.CheckIn());
  ASSERT_EQ(3, v.CheckIn());

  EXPECT_FALSE(v.StallFull());

  ASSERT_EQ(3, v.CheckIn());

  EXPECT_EQ(6, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(30.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());
  EXPECT_FALSE(v.ParkingEmpty());
  EXPECT_FALSE(v.ParkingFull());
  EXPECT_FALSE(v.StallEmpty());
  EXPECT_TRUE(v.StallFull());
  EXPECT_TRUE(v.QueueEmpty());
  EXPECT_FALSE(v.QueueFull());

  EXPECT_EQ(3, v.StallNumber(5));
  EXPECT_EQ(1, v.StallNumber(2));
  EXPECT_EQ(2, v.StallNumber(4));

  // passage_6
  ASSERT_TRUE(v.CheckOut(1, 2));
  EXPECT_EQ(5, v.CarsInStalls());
  EXPECT_EQ(1, v.CarsInCheckOut());
  ASSERT_TRUE(v.CheckOut(1, 1));
  EXPECT_EQ(4, v.CarsInStalls());
  EXPECT_EQ(2, v.CarsInCheckOut());
  ASSERT_TRUE(v.CheckOut(2, 4));
  EXPECT_EQ(3, v.CarsInStalls());
  EXPECT_EQ(3, v.CarsInCheckOut());
  ASSERT_TRUE(v.CheckOut(2, 3));
  EXPECT_EQ(2, v.CarsInStalls());
  EXPECT_EQ(4, v.CarsInCheckOut());
  ASSERT_TRUE(v.CheckOut(3, 6));
  EXPECT_EQ(1, v.CarsInStalls());
  EXPECT_EQ(5, v.CarsInCheckOut());
  ASSERT_TRUE(v.CheckOut(3, 5));
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(6, v.CarsInCheckOut());

  EXPECT_FALSE(v.QueueEmpty());
  EXPECT_TRUE(v.QueueFull());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());

  // passage_7 (kinda)
  EXPECT_EQ(6, v.AvailableStallSpaces());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(6, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());
  
  ASSERT_EQ(2, v.Pay());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(5, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(5.0, v.TotalPaid());

  ASSERT_EQ(1, v.Pay());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(4, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(10.0, v.TotalPaid());

  ASSERT_EQ(4, v.Pay());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(3, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(15.0, v.TotalPaid());

  ASSERT_EQ(3, v.Pay());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(2, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(20.0, v.TotalPaid());

  ASSERT_EQ(6, v.Pay());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(1, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(25.0, v.TotalPaid());

  ASSERT_EQ(5, v.Pay());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(30.0, v.TotalPaid());

  EXPECT_TRUE(v.ParkingEmpty());
  EXPECT_FALSE(v.ParkingFull());
  EXPECT_TRUE(v.StallEmpty());
  EXPECT_FALSE(v.StallFull());
  EXPECT_TRUE(v.QueueEmpty());
  EXPECT_FALSE(v.QueueFull());
  EXPECT_EQ(6, v.TotalStallSpaces());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(30.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(30.0, v.TotalPaid());
}

TEST(ThirtyStalls, ThirtyStalls) { // passage_8
  ValetParking v( 30, 20, 100, 0, 5) ;

  EXPECT_EQ(600, v.TotalStallSpaces());
  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(1, v.CheckIn());
  ASSERT_EQ(1, v.CheckIn());
  EXPECT_DOUBLE_EQ(30.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(0.0, v.TotalPaid());
  EXPECT_EQ(594, v.AvailableStallSpaces());
  
  ASSERT_TRUE(v.CheckOut(1, 2));
  ASSERT_TRUE(v.CheckOut(1, 5));
  EXPECT_EQ(4, v.CarsInStalls());
  EXPECT_EQ(2, v.CarsInCheckOut());

  ASSERT_TRUE(v.CheckOut(1, 3));
  EXPECT_EQ(597, v.AvailableStallSpaces());

  ASSERT_TRUE(v.CheckOut(1, 4));
  ASSERT_TRUE(v.CheckOut(1, 1));
  EXPECT_EQ(5, v.CarsInCheckOut());
  EXPECT_EQ(1, v.CarsInStalls());

  ASSERT_TRUE(v.CheckOut(1, 6));
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(6, v.CarsInCheckOut());
  
  ASSERT_EQ(2, v.Pay());
  EXPECT_EQ(600, v.AvailableStallSpaces());
  EXPECT_EQ(5, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(30.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(5.0, v.TotalPaid());

  ASSERT_EQ(5, v.Pay());
  ASSERT_EQ(3, v.Pay());
  ASSERT_EQ(4, v.Pay());
  ASSERT_EQ(1, v.Pay());
  ASSERT_EQ(6, v.Pay());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_TRUE(v.ParkingEmpty());
  EXPECT_FALSE(v.ParkingFull());
  EXPECT_TRUE(v.StallEmpty());
  EXPECT_FALSE(v.StallFull());
  EXPECT_TRUE(v.QueueEmpty());
  EXPECT_FALSE(v.QueueFull());
  EXPECT_EQ(600, v.TotalStallSpaces());
  EXPECT_EQ(0, v.CarsInStalls());
  EXPECT_EQ(0, v.CarsInCheckOut());
  EXPECT_DOUBLE_EQ(30.0, v.TotalSales());
  EXPECT_DOUBLE_EQ(30.0, v.TotalPaid());
}
