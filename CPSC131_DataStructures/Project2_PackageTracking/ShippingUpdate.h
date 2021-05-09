////////////////////////////////////////////////////////////////////////////////
// ShippingUpdate.h
//
// class ShippingUpdate
////////////////////////////////////////////////////////////////////////////////

#ifndef SHIPPING_UPDATE_H
#define SHIPPING_UPDATE_H

#include <ctime>  // std::time_t
#include <string> // std::string

namespace PackageTracking
{

  // A shipping update represents one event in the delivery
  // process. It has a description such as "Out for delivery"; a
  // location such as "Fullerton, CA US"; and a timestamp, which is a
  // Unix timestamp.
  class ShippingUpdate
  {
  public:
    // Default constructor initializes description and location to
    // empty strings, and timestamp to 0.
    ShippingUpdate() noexcept;

    // Initialization constructor
    ShippingUpdate(const std::string &description,
                   const std::string &location,
                   std::time_t timestamp) noexcept;

    // Accessors
    const std::string &Description() const noexcept;
    const std::string &Location() const noexcept;
    time_t Timestamp() const noexcept;

    // Return a human-readable description of this update.
    // This is:
    //  1. timestamp
    //  2. one space
    //  3. description
    //  4. one space
    //  5. location
    //  6. one newline (\n)
    std::string Describe() const noexcept;

  private:
    std::string description_, location_;
    std::time_t timestamp_;
  };

} // namespace PackageTracking

#endif
