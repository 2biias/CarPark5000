#ifndef CARPARK5000_HPP_
#define CARPARK5000_HPP_


#include "Entry.hpp"
#include "Exit.hpp"


// Abstraction of the whole CarPark. Consists of entry and exit (composition)
class CarPark5000
{
public:
  CarPark5000(const int MsgQueueMaxSize, const int MaxCapacityInPark);
  ~CarPark5000();
  Entry* Entry_;
  Exit* Exit_;
};

#endif
