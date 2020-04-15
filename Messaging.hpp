#ifndef MESSAGING_HPP_
#define MESSAGING_HPP_

#include <queue>
#include <pthread.h>

// Base class for messages
class Message
{
public:
  virtual ~Message(){}
};

// Message abstraction to contain message-ID
struct Item
{
  unsigned long id_;
  Message* msg_;
};

// Messagequeue holding Message abstractions
class MsgQueue
{
public:
  MsgQueue(unsigned long maxSize);
  void send(unsigned long id, Message* msg);
  Message* receive(unsigned long& id);
  ~MsgQueue();
private:
  std::queue<Item> thisQueue;
  unsigned long maxSize;
  unsigned long currentSize;

  pthread_mutex_t sizeMutex;
  pthread_cond_t sizeCond;
};

#endif
