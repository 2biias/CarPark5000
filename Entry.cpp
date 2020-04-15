#include "Entry.hpp"
#include "Car.hpp"

Entry::Entry(const int MsgQueueMaxSize, const int MaxCapacityInPark)
: DoorOpen(false), carsEntering(0), carsInPark(0), MaxCapacityInPark(MaxCapacityInPark)
{
  // Allocating messageQueue
  EntryMessageQueue = new MsgQueue(MsgQueueMaxSize);
}

Entry::~Entry() {
  delete EntryMessageQueue;
}

pthread_t* Entry::getThreadHandle() {
  return &threadHandle;
}

MsgQueue* Entry::getMessageQueue() {
  return EntryMessageQueue;
}

void Entry::Dispatch(unsigned long id, Message* msg) {
  switch(id) {
    case EntryEvents::ENTRY_DOOR_OPEN_REQ:
      handleOpenDoorRequest(static_cast<CarParkEntryOpenReq*>(msg));
      break;
    case EntryEvents::CAR_ENTERS_CFM:
      handleCarEntered();
      break;
    case EntryEvents::CAR_LEFT_NOTIFICATION:
      // To keep track of cars in park, decrement when notified
      handleCarLeft();
      break;
    default:
      printf("Entry could not handle message\n");
      break;
  }
}

void Entry::handleOpenDoorRequest(CarParkEntryOpenReq* req) {

  // Deriving MessageQueue to confirm/decline
  MsgQueue* RequestingCarMq = req->RequestingCarMq;

  // If carpark is full, decline the car, otherwise confirm
  if(carsInPark >= MaxCapacityInPark) {
    RequestingCarMq->send(CarEvents::ENTRY_DOOR_OPEN_DCL, nullptr);
  } else {
    carsInPark++;
    RequestingCarMq->send(CarEvents::ENTRY_DOOR_OPEN_CFM, nullptr);
    // Sending confirm back to car
    if(!DoorOpen)
    {
      printf("Entry opens\n");
      DoorOpen=true;
    }
    carsEntering++;
  }

}

void Entry::handleCarEntered() {

  carsEntering--;

  if(!(carsEntering > 0))
  {
    printf("Entry closes\n");
    DoorOpen = false;
  }

}

void Entry::handleCarLeft() {
  carsInPark--;
}
