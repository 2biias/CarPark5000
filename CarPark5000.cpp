#include "CarPark5000.hpp"

CarPark5000::CarPark5000(const int MsgQueueMaxSize, const int MaxCapacityInPark) {
  Entry_ = new Entry(MsgQueueMaxSize, MaxCapacityInPark);
  Exit_ = new Exit(MsgQueueMaxSize);
}
CarPark5000::~CarPark5000() {
  // By fullfilling 'complete' memory management, this becomes a composition
  delete Entry_;
  delete Exit_;
}
