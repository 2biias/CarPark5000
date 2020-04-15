#include "Exit.hpp"
#include "Car.hpp"

Exit::Exit(const int MsgQueueMaxSize)
: DoorOpen(false), carsLeaving(0)
{
  // Allocating messageQueue
  ExitMessageQueue = new MsgQueue(MsgQueueMaxSize);
}

Exit::~Exit() {
  delete ExitMessageQueue;
}

pthread_t* Exit::getThreadHandle() {
  return &threadHandle;
}

MsgQueue* Exit::getMessageQueue() {
  return ExitMessageQueue;
}

void Exit::Dispatch(unsigned long id, Message* msg) {

  switch(id) {
    case ExitEvents::EXIT_DOOR_OPEN_REQ:
        handleOpenDoorRequest(static_cast<CarParkExitOpenReq*>(msg));
      break;
    case ExitEvents::CAR_LEAVES_CFM:
        handleCarLeft();
      break;
    default:
      printf("Exit could not handle message\n");
      break;
  }
}

void Exit::handleOpenDoorRequest(CarParkExitOpenReq* req) {

  // deriving messagequeue from request
  MsgQueue* RequestingCarMq = req->RequestingCarMq;

  if(!DoorOpen)
  {
    printf("Exit opens\n");
    DoorOpen=true;
  }
  carsLeaving++;

  RequestingCarMq->send(CarEvents::EXIT_DOOR_OPEN_CFM, nullptr);
}

void Exit::handleCarLeft() {
  carsLeaving--;
  if(!(carsLeaving > 0))
  {
    printf("Exit closes\n");
    DoorOpen = false;
  }
}
