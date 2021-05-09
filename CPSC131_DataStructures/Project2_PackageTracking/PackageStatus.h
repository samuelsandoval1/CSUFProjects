////////////////////////////////////////////////////////////////////////////////
// PackageStatus.h
//
// class PackageStatus
////////////////////////////////////////////////////////////////////////////////

#ifndef PACKAGE_STATUS_H
#define PACKAGE_STATUS_H

#include <stdexcept> // std::invalid_argument, std::logic_error
#include <list>
#include <memory>
#include <string> // std::string


#include "ShippingUpdate.h"

namespace PackageTracking {

  // PackageStatus represents the entire sequence of events of
  // shipping one parcel.
  //
  // It has the tracking number like "1Z4310X3YW25357495", and a list
  // of ShippingUpdate objects, in chronological order
  // (oldest-to-newest).
  //
  // PackageStatus also has a concept of a *cursor* that points to one
  // particular update. When there are no updates, the cursor location
  // is undefined. When the first update is added, the cursor points
  // to that first update. When there are multiple updates, the cursor
  // can move forward and backward through the update list.
  class PackageStatus {
  public:

    PackageStatus() noexcept;

    PackageStatus(const std::string& tracking_number) noexcept;

    // Accessors
    const std::string& TrackingNumber() const noexcept;

    // Size and emptiness. These refer to the number of updates.
    int Size() const noexcept;
    bool Empty() const noexcept;

    void AddUpdate(const std::string& description,
		   const std::string& location,
		   std::time_t timestamp);

    bool MoveCursorBackward() noexcept;

    bool MoveCursorForward() noexcept;

    const ShippingUpdate& GetCursor() const;

    std::string DescribeCursorUpdate();

    std::string DescribePreviousUpdates();

    std::string DescribeFollowingUpdates();

    std::string DescribeAllUpdates();

  private:
    std::list<ShippingUpdate> updates_;
    std::string tracking_number_;

    std::list<ShippingUpdate>::iterator cursor_;
    std::list<ShippingUpdate>::iterator last_update;
  };

}

#endif
