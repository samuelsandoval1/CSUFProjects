////////////////////////////////////////////////////////////////////////////////
// UnitTest.cpp
//
// Unit tests for the functionality declared in:
//   ShippingUpdate.h
//   PackageStatus.h
//   Serialize.h
////////////////////////////////////////////////////////////////////////////////

#include "gtest/gtest.h"

#include "ShippingUpdate.h"
#include "PackageStatus.h"
#include "Serialize.h"

using namespace PackageTracking;

TEST(GivenCode, ShippingUpdate) {

  // default constructor
  ShippingUpdate deflt; // "default" is a keyword
  EXPECT_EQ("", deflt.Description());
  EXPECT_EQ("", deflt.Location());
  EXPECT_EQ(0, deflt.Timestamp());
}

TEST(ShippingUpdate, InitializationConstructor) {

  ShippingUpdate a("x", "y", 123);
  EXPECT_EQ("x", a.Description());
  EXPECT_EQ("y", a.Location());
  EXPECT_EQ(123, a.Timestamp());

  ShippingUpdate b("cats", "dogs", 1000);
  EXPECT_EQ("cats", b.Description());
  EXPECT_EQ("dogs", b.Location());
  EXPECT_EQ(1000, b.Timestamp());
}

TEST(ShippingUpdate, Describe) {

  ShippingUpdate a("x", "y", 123);
  EXPECT_EQ("123 x y\n", a.Describe());
  
  ShippingUpdate b("cats", "dogs", 1000);
  EXPECT_EQ("1000 cats dogs\n", b.Describe());
}

TEST(PackageStatusConstructorsAccessors, PackageStatusConstructorsAccessors) {

  // Default constructor
  PackageStatus deflt; // "default" is a keyword
  EXPECT_EQ("", deflt.TrackingNumber());
  EXPECT_EQ(0, deflt.Size());
  EXPECT_TRUE(deflt.Empty());

  // Initialization constructor
  PackageStatus init("Z123");
  EXPECT_EQ("Z123", init.TrackingNumber());
  EXPECT_EQ(0, init.Size());
  EXPECT_TRUE(init.Empty());
}

TEST(AddUpdate, AddUpdate) {

  // start empty
  PackageStatus ps("Z123");
  EXPECT_EQ(0, ps.Size());
  EXPECT_TRUE(ps.Empty());
  
  // add first update
  EXPECT_NO_THROW(ps.AddUpdate("d1", "l1", 100));
  EXPECT_EQ(1, ps.Size());
  EXPECT_FALSE(ps.Empty());
  // cursor is now at the just-added update
  EXPECT_EQ("d1", ps.GetCursor().Description());

  // add second update
  EXPECT_NO_THROW(ps.AddUpdate("d2", "l2", 200));
  EXPECT_EQ(2, ps.Size());
  EXPECT_FALSE(ps.Empty());
  // cursor hasn't moved and is still on the first update
  EXPECT_EQ("d1", ps.GetCursor().Description());

  // add third update
  EXPECT_NO_THROW(ps.AddUpdate("d3", "l3", 300));
  EXPECT_EQ(3, ps.Size());
  EXPECT_FALSE(ps.Empty());
  // cursor hasn't moved and is still on the first update
  EXPECT_EQ("d1", ps.GetCursor().Description());

  // out-of-order timestamp, should throw an exception
  EXPECT_THROW(ps.AddUpdate("d4", "l4", 299), std::invalid_argument);
}

TEST(ReadingJSON, PackageStatusFromJSON) {

  static const std::string tracking{"1Z4310X3YW25357495"};

  PackageStatus p0;
  ASSERT_NO_THROW(p0 = PackageStatusFromJSON("package_0.json"));
  EXPECT_EQ(0, p0.Size());
  EXPECT_EQ(tracking, p0.TrackingNumber());

  PackageStatus p1;
  ASSERT_NO_THROW(p1 = PackageStatusFromJSON("package_1.json"));
  EXPECT_EQ(1, p1.Size());
  
  PackageStatus p3;
  ASSERT_NO_THROW(p3 = PackageStatusFromJSON("package_3.json"));
  EXPECT_EQ(3, p3.Size());
  
  PackageStatus p8;
  ASSERT_NO_THROW(p8 = PackageStatusFromJSON("package_8.json"));
  EXPECT_EQ(8, p8.Size());
}

TEST(CursorMotion, CursorMotion) {

  // moving in an empty list has no effect
  PackageStatus p0;
  EXPECT_FALSE(p0.MoveCursorBackward());
  EXPECT_FALSE(p0.MoveCursorBackward());
  EXPECT_FALSE(p0.MoveCursorForward());
  EXPECT_FALSE(p0.MoveCursorForward());
  // GetCursor throws when empty
  EXPECT_THROW(p0.GetCursor(), std::logic_error);

  PackageStatus p1;
  ASSERT_NO_THROW(p1 = PackageStatusFromJSON("package_1.json"));
  EXPECT_EQ(1515978000, p1.GetCursor().Timestamp());
  // move backward, still at the only update
  EXPECT_FALSE(p1.MoveCursorBackward());
  EXPECT_EQ(1515978000, p1.GetCursor().Timestamp());
  // move forward, still at the only update
  EXPECT_FALSE(p1.MoveCursorForward());
  EXPECT_EQ(1515978000, p1.GetCursor().Timestamp());

  PackageStatus p3;
  ASSERT_NO_THROW(p3 = PackageStatusFromJSON("package_3.json"));
  EXPECT_EQ(1515978000, p3.GetCursor().Timestamp());
  // move backward, still at the first
  EXPECT_FALSE(p3.MoveCursorBackward());
  EXPECT_EQ(1515978000, p3.GetCursor().Timestamp());
  // move forward to the second update
  EXPECT_TRUE(p3.MoveCursorForward());
  EXPECT_EQ(1516111440, p3.GetCursor().Timestamp());
  // move forward to the third update
  EXPECT_TRUE(p3.MoveCursorForward());
  EXPECT_EQ(1516188120, p3.GetCursor().Timestamp());
  // move forward, still at the third update
  EXPECT_FALSE(p3.MoveCursorForward());
  EXPECT_EQ(1516188120, p3.GetCursor().Timestamp());
  // move backward to the second update
  EXPECT_TRUE(p3.MoveCursorBackward());
  EXPECT_EQ(1516111440, p3.GetCursor().Timestamp());
  // move backward to the first update
  EXPECT_TRUE(p3.MoveCursorBackward());
  EXPECT_EQ(1515978000, p3.GetCursor().Timestamp());
  // move backward, still at the first
  EXPECT_FALSE(p3.MoveCursorBackward());
  EXPECT_EQ(1515978000, p3.GetCursor().Timestamp());

  PackageStatus p8;
  ASSERT_NO_THROW(p8 = PackageStatusFromJSON("package_8.json"));
  // move backward, still at the first
  EXPECT_FALSE(p8.MoveCursorBackward());
  EXPECT_EQ(1515978000, p8.GetCursor().Timestamp());
  // move forward to the second update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516111440, p8.GetCursor().Timestamp());
  // move forward to the third update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516188120, p8.GetCursor().Timestamp());
  // move forward to the fourth update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516366740, p8.GetCursor().Timestamp());
  // move forward to the fifth update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516392780, p8.GetCursor().Timestamp());
  // move forward to the sixth update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516410060, p8.GetCursor().Timestamp());
  // move forward to the seventh update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516441740, p8.GetCursor().Timestamp());
  // move forward to the eighth update
  EXPECT_TRUE(p8.MoveCursorForward());
  EXPECT_EQ(1516468200, p8.GetCursor().Timestamp());
  // move forward, still at the eighth update
  EXPECT_FALSE(p8.MoveCursorForward());
  EXPECT_EQ(1516468200, p8.GetCursor().Timestamp());
  // move backward to the seventh update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1516441740, p8.GetCursor().Timestamp());
  // move backward to the sixth update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1516410060, p8.GetCursor().Timestamp());
  // move backward to the fifth update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1516392780, p8.GetCursor().Timestamp());
  // move backward to the fourth update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1516366740, p8.GetCursor().Timestamp());
  // move backward to the third update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1516188120, p8.GetCursor().Timestamp());
  // move forward to the second update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1516111440, p8.GetCursor().Timestamp());
  // move backward to the first update
  EXPECT_TRUE(p8.MoveCursorBackward());
  EXPECT_EQ(1515978000, p8.GetCursor().Timestamp());
  // move backward, still at the first
  EXPECT_FALSE(p8.MoveCursorBackward());
  EXPECT_EQ(1515978000, p8.GetCursor().Timestamp());
}

TEST(PackageStatusDescribe, PackageStatusDescribe) {

  // empty
  PackageStatus empty;
  EXPECT_THROW(empty.DescribePreviousUpdates(), std::logic_error);
  EXPECT_THROW(empty.DescribeFollowingUpdates(), std::logic_error);
  EXPECT_EQ("", empty.DescribeAllUpdates());

  static const std::string descr1{"1515978000 Package has left seller facility and is in transit to carrier N/A\n"},
    descr2{"1516111440 Shipment arrived at Amazon facility Hebron, KENTUCKY US\n"},
    descr3{"1516188120 Shipment departed from Amazon facility Hebron, KENTUCKY US\n"};
  
  PackageStatus p1;
  ASSERT_NO_THROW(p1 = PackageStatusFromJSON("package_1.json"));
  EXPECT_EQ("", p1.DescribePreviousUpdates());
  EXPECT_EQ(descr1, p1.DescribeFollowingUpdates());
  EXPECT_EQ(descr1, p1.DescribeAllUpdates());

  PackageStatus p3;
  ASSERT_NO_THROW(p3 = PackageStatusFromJSON("package_3.json"));
  // cursor on first update
  EXPECT_EQ("", p3.DescribePreviousUpdates());
  EXPECT_EQ(descr1 + descr2 + descr3, p3.DescribeFollowingUpdates());
  EXPECT_EQ(descr1 + descr2 + descr3, p3.DescribeAllUpdates());
  // cursor on second update
  ASSERT_TRUE(p3.MoveCursorForward());
  EXPECT_EQ(descr1, p3.DescribePreviousUpdates());
  EXPECT_EQ(descr2 + descr3, p3.DescribeFollowingUpdates());
  EXPECT_EQ(descr1 + descr2 + descr3, p3.DescribeAllUpdates());
  // cursor on third update
  ASSERT_TRUE(p3.MoveCursorForward());
  EXPECT_EQ(descr1 + descr2, p3.DescribePreviousUpdates());
  EXPECT_EQ(descr3, p3.DescribeFollowingUpdates());
  EXPECT_EQ(descr1 + descr2 + descr3, p3.DescribeAllUpdates());
}
