////////////////////////////////////////////////////////////////////////////////
// PackageStatus.cpp
//
// class PackageStatus
////////////////////////////////////////////////////////////////////////////////

#include "PackageStatus.h"
#include <algorithm>
#include <iterator>

namespace PackageTracking
{

  PackageStatus::PackageStatus() noexcept : updates_(0)
  {
    tracking_number_ = "";
  }

  PackageStatus::PackageStatus(const std::string &tracking_number) noexcept : updates_(0), tracking_number_(tracking_number)
  {
  }

  const std::string &PackageStatus::TrackingNumber() const noexcept
  {
    return tracking_number_;
  }

  int PackageStatus::Size() const noexcept
  {
    return updates_.size();
  }

  bool PackageStatus::Empty() const noexcept
  {
    return updates_.size() == 0;
  }

  void PackageStatus::AddUpdate(const std::string &description,
                                const std::string &location,
                                std::time_t timestamp)
  {
    if (timestamp < 0)
    {
      throw std::invalid_argument("timestamp is invalid");
    }
    if (Size() > 0)
    {
      if (timestamp >= last_update->Timestamp())
      {
        updates_.push_back(ShippingUpdate(description, location, timestamp));
        last_update++;
      }
      else
      {
        throw std::invalid_argument("timestamp is invalid");
      }
    }
    else
    {
      updates_.push_back(ShippingUpdate(description, location, timestamp));
      cursor_ = updates_.begin();
      last_update = cursor_;
    }
  }

  bool PackageStatus::MoveCursorBackward() noexcept
  {
    if (Empty() || cursor_ == updates_.begin())
    {
      return false;
    }
    cursor_--;
    return true;
  }

  bool PackageStatus::MoveCursorForward() noexcept
  {
    if (Empty() || cursor_ == last_update)
    {
      return false;
    }
    cursor_ = std::next(cursor_, 1);

    return true;
  }

  const ShippingUpdate &PackageStatus::GetCursor() const
  {

    if (Empty())
    {
      throw std::logic_error("not implemented");
    }
    return *cursor_;
  }

  std::string PackageStatus::DescribeCursorUpdate()
  {
    if (Empty())
    {
      throw std::logic_error("Updates Container is empty");
    }
    return cursor_->Describe();
  }

  std::string PackageStatus::DescribePreviousUpdates()
  {
    if (Empty())
    {
      throw std::logic_error("Updates Container is empty");
    }
    std::string previous_Updates = "";
    std::list<ShippingUpdate>::iterator it;
    for (it = updates_.begin(); it != cursor_; it++)
    {
      previous_Updates += it->Describe();
    }
    return previous_Updates;
  }

  std::string PackageStatus::DescribeFollowingUpdates()
  {
    if (Empty())
    {
      throw std::logic_error("List is empty");
    }
    std::string concatenated_Updates = "";
    std::list<ShippingUpdate>::iterator it;
    for (it = cursor_; it != updates_.end(); it++)
    {
      concatenated_Updates += it->Describe();
    }
    return concatenated_Updates;
  }

  std::string PackageStatus::DescribeAllUpdates()
  {
    if (Empty())
    {
      return "";
    }
    std::string concatenated_Updates = "";
    std::list<ShippingUpdate>::iterator it;
    for (it = updates_.begin(); it != updates_.end(); it++)
    {
      concatenated_Updates += it->Describe();
    }
    return concatenated_Updates;
  }

} // namespace PackageTracking
