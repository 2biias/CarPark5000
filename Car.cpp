#include "Car.hpp"
//#include "Entry.hpp"

Car::Car(size_t id, const int MsgQueueMaxSize) : ID(id)
{
  // Allocating messageQueue
  CarMessageQueue = new MsgQueue(MsgQueueMaxSize);
}

Car::~Car()
{
  delete CarMessageQueue;
}

size_t Car::getID() {
  return ID;
}

void Car::SetCarParkToUse(CarPark5000* ParkToUse) {
  carParkToUse = ParkToUse;
}

pthread_t* Car::getThreadHandle() {
  return &threadHandle;
}

MsgQueue* Car::getMessageQueue() {
  return CarMessageQueue;
}

void Car::Dispatch(unsigned long id, Message* msg) {

  switch(id) {
    case CarEvents::INDICATE_SPAWN:
      HandleCarSpawn();
      break;
    case CarEvents::ENTRY_DOOR_OPEN_CFM:
      HandleCarDrivingIn();
      break;
    case CarEvents::ENTRY_DOOR_OPEN_DCL:
      HandleCarDeclined();
      break;
    case CarEvents::EXIT_DOOR_OPEN_CFM:
      HandleCarDrivingOut();
      break;
    default:
      printf("Car could not handle message\n");
      break;
  }
}

void Car::HandleCarSpawn() {
  // Indicate spawn by requesting entry to open

  Entry::CarParkEntryOpenReq* request = new Entry::CarParkEntryOpenReq;
  request->RequestingCarMq = CarMessageQueue;

  printf("Car %lu requesting to enter Carpark5000.\n", getID());

  //Send request to Entry
  carParkToUse->Entry_->getMessageQueue()->send(EntryEvents::ENTRY_DOOR_OPEN_REQ, request);
}

void Car::HandleCarDrivingIn() {
  printf("Car %lu Enters. Staying for a while.\n", getID());
  // Car confirms enter
  carParkToUse->Entry_->getMessageQueue()->send(EntryEvents::CAR_ENTERS_CFM, nullptr);

  // Simulating of car staying for a while
  size_t randelay_ms = 1000000 + rand() % 5000000;
  usleep(randelay_ms);

  // Car wants to leave, sending open request to exit door

  Exit::CarParkExitOpenReq* request = new Exit::CarParkExitOpenReq;
  request->RequestingCarMq = CarMessageQueue;

  // Request the exit door to open
  carParkToUse->Exit_->getMessageQueue()->send(ExitEvents::EXIT_DOOR_OPEN_REQ, request);
}

void Car::HandleCarDeclined() {

  printf("Car %lu Declined. Park full.\n", getID());
  // Wait for a while before trying to enter again
  RespawnCar();
}


void Car::HandleCarDrivingOut() {
  printf("Car %lu leaving CarPark5000.\n", getID());
  // Car confirms leave
  carParkToUse->Exit_->getMessageQueue()->send(ExitEvents::CAR_LEAVES_CFM, nullptr);
  carParkToUse->Entry_->getMessageQueue()->send(EntryEvents::CAR_LEFT_NOTIFICATION, nullptr);
  // Respawn car
  RespawnCar();
}

void Car::RespawnCar() {

  // Simulating some time before car respawns
  size_t randelay_ms = 1000000 + rand() % 5000000;
  usleep(randelay_ms);

  HandleCarSpawn();
}
