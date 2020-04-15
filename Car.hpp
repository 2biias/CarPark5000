#ifndef CAR_H
#define CAR_H

#include <unistd.h>
#include <memory>
#include <pthread.h>
#include "Messaging.hpp"

#include "CarPark5000.hpp"

#include <stdio.h>

enum CarEvents {
  INDICATE_SPAWN, ENTRY_DOOR_OPEN_CFM, ENTRY_DOOR_OPEN_DCL, EXIT_DOOR_OPEN_CFM
};

// Car class
class Car : public std::enable_shared_from_this<Car>
{
public:
  Car(size_t id, const int MsgQueueMaxSize);
  ~Car();
  void SetCarParkToUse(CarPark5000* ParkToUse);
  MsgQueue* getMessageQueue();
  pthread_t* getThreadHandle();
  size_t getID();

  void Dispatch(unsigned long id, Message* msg);
  void HandleCarSpawn();
  void HandleCarDrivingIn();
  void HandleCarDeclined();
  void HandleCarDrivingOut();
  void RespawnCar();
private:
  CarPark5000* carParkToUse;
  pthread_t threadHandle;
  MsgQueue* CarMessageQueue;
  size_t ID;
};

#endif
