#ifndef EXIT_H
#define EXIT_H

#include <pthread.h>
#include <stdio.h>
#include "Messaging.hpp"


enum ExitEvents {
  EXIT_DOOR_OPEN_REQ, CAR_LEAVES_CFM
};

// Exit class
class Exit
{
public:
  Exit(const int MsgQueueMaxSize);
  ~Exit();
  pthread_t* getThreadHandle();
  MsgQueue* getMessageQueue();

  struct CarParkExitOpenReq : public Message
  {
    MsgQueue* RequestingCarMq;
  };

  void Dispatch(unsigned long id, Message* msg);
  void handleOpenDoorRequest(CarParkExitOpenReq* req);
  void handleCarLeft();

private:
  pthread_t threadHandle;
  MsgQueue* ExitMessageQueue;
  bool DoorOpen;
  int carsLeaving;
};

#endif
