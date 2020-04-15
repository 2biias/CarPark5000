#include "Messaging.hpp"
#include "ScopedLocker.hpp"

MsgQueue::MsgQueue(unsigned long maxSize)
: maxSize(maxSize), currentSize(0)
{
  pthread_mutex_init(&sizeMutex, NULL);
  pthread_cond_init(&sizeCond, NULL);
}

void MsgQueue::send(unsigned long id, Message* msg)
{
  ScopedLocker lockthis(&sizeMutex);
  while(currentSize >= maxSize)
  {
    pthread_cond_wait(&sizeCond, &sizeMutex);
  }
  // Do send stuff
  Item item;
  item.id_ = id;
  item.msg_ = msg;
  thisQueue.push(item);

  // Increment number of elements in queue
  currentSize++;
  pthread_cond_signal(&sizeCond);
}

Message* MsgQueue::receive(unsigned long& id)
{
  ScopedLocker lockthis(&sizeMutex);

  // If empty wait for send to push new message
  while(currentSize <= 0)
  {
    pthread_cond_wait(&sizeCond, &sizeMutex);
  }

  // do recieve stuff
  Item item = thisQueue.front();
  id = item.id_;
  Message* msg = item.msg_;
  thisQueue.pop();

  // Decrement number of elements in queue
  currentSize--;
  pthread_cond_signal(&sizeCond);
  return msg;
}

MsgQueue::~MsgQueue()
{
  pthread_mutex_destroy(&sizeMutex);
  pthread_cond_destroy(&sizeCond);
}
