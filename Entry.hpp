#ifndef ENTRY_H
#define ENTRY_H

#include <pthread.h>
#include <stdio.h>

#include "Messaging.hpp"

enum EntryEvents {
  ENTRY_DOOR_OPEN_REQ, CAR_ENTERS_CFM, CAR_LEFT_NOTIFICATION
};

// Entry class
class Entry
{
public:
  Entry(const int MsgQueueMaxSize, const int MaxCapacityInPark);
  ~Entry();

  pthread_t* getThreadHandle();
  MsgQueue* getMessageQueue();

  struct CarParkEntryOpenReq : public Message
  {
    MsgQueue* RequestingCarMq;
  };

  void Dispatch(unsigned long id, Message* msg);
  void handleOpenDoorRequest(CarParkEntryOpenReq* req);
  void handleCarEntered();
  void handleCarLeft();
private:
  pthread_t threadHandle;
  MsgQueue* EntryMessageQueue;
  bool DoorOpen;
  int carsEntering;
  int carsInPark;
  int MaxCapacityInPark;
};

#endif
