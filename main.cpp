#include <unistd.h>
#include <iostream>

#include "Car.hpp"
#include "number.hpp"

const int globalQueueSize = 10;
const int MaxCapacityInPark = 5;

void* CarThread(void* arg) {

  // Derive shared_ptr from existing shared_ptr object to increment reference counter
  std::shared_ptr<Car> CarPtr = static_cast<Car*>(arg)->shared_from_this();
  MsgQueue* CarMessageQueue = CarPtr->getMessageQueue();

  unsigned long id;

  for(;;) {
    usleep(10);
    // Recieve from messageQueue
    Message* msg = CarMessageQueue->receive(id);
    // Dispatch message
    CarPtr->Dispatch(id, msg);
    delete msg;
  }
}

void* EntryThread(void* arg) {

  // Get reference to Entry and derive its messagequeue
  Entry* EntryPtr = static_cast<Entry*>(arg);
  MsgQueue* EntryMessageQueue = EntryPtr->getMessageQueue();

  unsigned long id;

  for(;;) {
    usleep(10);
    Message* msg = EntryMessageQueue->receive(id);
    // Dispatch message
    EntryPtr->Dispatch(id, msg);
    delete msg;
  }
  //Clean up is done by main returning (static CarPark with destructor)
}

void* ExitThread(void* arg) {

  Exit* ExitPtr = static_cast<Exit*>(arg);
  MsgQueue* ExitMessageQueue = ExitPtr->getMessageQueue();

  unsigned long id;

  for(;;) {
    usleep(10);
      Message* msg = ExitMessageQueue->receive(id);
      // Dispatch message
      ExitPtr->Dispatch(id, msg);
      delete msg;
  }
  //Clean up is done by main returning (static CarPark with destructor)
}

int main(int argc, char* argv[]){

  //Checking number of cars
  if(!is_number(argv[1])){
    std::cout << "Usage: CarPark <number of cars between 1 and 255>" << std::endl;
    return 0;
  }
  size_t numcars = std::stoi(argv[1]);
  if(!(numcars > 0 && numcars < 256)){
    std::cout << "Usage: CarPark <number of cars between 1 and 255>" << std::endl;
    return 0;
  }

  // Create the CarPark which consists of an entry and exit.
  CarPark5000 CarPark(globalQueueSize, MaxCapacityInPark);

  //Creating Entry guard thread
  //Entry* EntryObj = new Entry(globalQueueSize);
  pthread_create(CarPark.Entry_->getThreadHandle(), NULL, EntryThread, CarPark.Entry_);

  //Creating Exit guard thread
  //Exit* ExitObj = new Exit(globalQuemueSize);
  pthread_create(CarPark.Exit_->getThreadHandle(), NULL, ExitThread, CarPark.Exit_);

  std::cout << "Welcome to CarPark5000. Our capacity is " << 10 << " cars." << std::endl;

  //Create car threads
  std:: cout << "Creating " << numcars << " car-threads.\n\n" << std::endl;

  std::vector<std::shared_ptr<Car>> CarVector;
  for(size_t i = 0; i < numcars; i++){
    CarVector.push_back(std::make_shared<Car>(i+1, globalQueueSize));
    // Associating the car with the carpark
    CarVector[i]->SetCarParkToUse(&CarPark);
    pthread_create(CarVector[i]->getThreadHandle(), NULL, CarThread, CarVector[i].get());
    CarVector[i]->getMessageQueue()->send(CarEvents::INDICATE_SPAWN, nullptr);
  }

  //Joing entry and exit thread
  pthread_join(*(CarPark.Entry_->getThreadHandle()), NULL);
  pthread_join(*(CarPark.Exit_->getThreadHandle()), NULL);
  for(size_t i = 0; i < numcars; i++){
    pthread_join(*(CarVector[i]->getThreadHandle()), NULL);
  }

}
