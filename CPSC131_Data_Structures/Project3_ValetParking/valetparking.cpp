#include "valetparking.h"

// ATTENTION STUDENTS:
// THE CONSTRUCTORS ARE ALREADY COMPLETED.
// YOU NEED TO COMPLETE THE DESTRUCTOR AND ALL OF THE REMAINING FUNCTIONS BELOW.

// CONSTRUCTORS - ALREADY COMPLETE
ValetParking::ValetParking()
    : _numberofstalls(1),
      _stallcapacity(1),
      _parkingstall(_numberofstalls, std::stack<size_t>()),
      _queuecapacity(1),
      _checkout(std::make_unique<std::queue<size_t>>()),
      _currentticket(0),
      _fee(5.0),
      _paid(0.0)
{}

ValetParking::ValetParking(size_t numberofstalls,
                           size_t stallcapacity,
                           size_t queuecapacity,
                           size_t ticketnumber,
                           double fee)
    : _numberofstalls(numberofstalls),
      _stallcapacity(stallcapacity),
      _parkingstall(_numberofstalls, std::stack<size_t>()),
      _queuecapacity(queuecapacity),
      _checkout(std::make_unique<std::queue<size_t>>()),
      _currentticket(ticketnumber),
      _fee(fee),
      _paid(0.0)
{}

// DESTRUCTOR - ALREADY COMPLETE
ValetParking::~ValetParking()
{}

size_t ValetParking::TotalStallSpaces()
{
  return _stallcapacity * _numberofstalls;
}

size_t ValetParking::CarsInStalls()
{
  size_t cars = 0;
  for (auto stall : _parkingstall)
  {
    cars += stall.size();
  }
  return cars;
}

size_t ValetParking::CarsInCheckOut()
{
  return _checkout->size();
}

size_t ValetParking::AvailableStallSpaces()
{
  size_t stall_Spaces = 0;
  for (auto stall : _parkingstall)
  {
    stall_Spaces += (_stallcapacity - stall.size());
  }
  return stall_Spaces;
}

double ValetParking::TotalSales()
{
  return _currentticket * _fee;
}

size_t ValetParking::Pay()
{

  size_t ticket = _checkout->front();
  _checkout->pop();
  _paid += _fee;
  return ticket;
}

double ValetParking::TotalPaid()
{
  return _paid;
}

bool ValetParking::ParkingEmpty()
{
  return (StallEmpty() == true && QueueEmpty() == true) ? true : false;
}

bool ValetParking::ParkingFull()
{
  return (StallFull() == true && QueueFull() == true) ? true : false;
}

bool ValetParking::QueueEmpty()
{
  if(_checkout->empty()){
    return true;
  }
  return false;
}

bool ValetParking::QueueFull()
{
  if (_checkout->size() == _queuecapacity){
      return true;
  }
  return false;
}

bool ValetParking::StallEmpty()
{
  int emptyTracker = 0;
  for (auto stall : _parkingstall)
  {
    if(stall.empty()){
      emptyTracker++;
    }
  }
  return (emptyTracker == _numberofstalls) ? true : false;
}

bool ValetParking::StallFull()
{
  int fullTracker = 0;
  for (auto stall : _parkingstall)
  {
    if(stall.size() == _stallcapacity){
      fullTracker++;
    }
  }
  return (fullTracker == _numberofstalls) ? true : false;
}

size_t ValetParking::GetNextTicket()
{
  return ++_currentticket;
}

size_t ValetParking::CheckIn()
{
  for (auto i = 0; i < _parkingstall.size(); i++)
  {
    auto curr = _parkingstall[i];
    if (curr.size() < _stallcapacity){
      _parkingstall[i].push(GetNextTicket());
      int stallNum = i+1;
      return stallNum;
    }
  }

  return 0;
}

size_t ValetParking::StallNumber(size_t ticket)
{
  size_t stallNum = 0;
  bool ticketFound = false;
  for (auto i = 0; (i < _parkingstall.size()) && !ticketFound; i++)
  {
    auto curr = _parkingstall[i];
    while (!curr.empty() && !ticketFound){
      auto val = curr.top();

      if(val == ticket){
        stallNum = i+1;
        ticketFound = true;
      }
      curr.pop();
    }
  }
  return stallNum;
}

bool ValetParking::CheckOut(size_t stallnumber, size_t ticket)
{
  std::stack<size_t> stallCars;
  std::stack<size_t> ticketValInStack;

  for (auto i = 0; i < _parkingstall.size(); i++)
  {
    if(i+1 == stallnumber){
      while (!(_parkingstall[i].empty())){
        if(ticket == _parkingstall[i].top()){
          ticketValInStack.push(_parkingstall[i].top());
          _parkingstall[i].pop();
        }
        else{
            stallCars.push(_parkingstall[i].top());
            _parkingstall[i].pop();
        }
      }
       while ((!stallCars.empty())){
        _parkingstall[i].push(stallCars.top());
        stallCars.pop();
      }
    }
  }
  if(ticketValInStack.size() < 1){
    return false;
  }

  _checkout->push(ticket);
  return true;
}
