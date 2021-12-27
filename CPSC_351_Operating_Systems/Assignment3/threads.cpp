#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <random>
#include <cstdlib>
#include <map>
#include <iterator>
#include <iostream>
using namespace std;

map<int, int> rMap;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
void* randomBinaryTree(void* arg)
{
  int rnumber;

  for(int j = 0; j < 100; j++)
  {
    rnumber = rand() % 100001;
    if (pthread_mutex_lock(&m1) < 0)
    {
      perror("pthread_mutex_lock");
      exit(-1);
    }
    rMap.insert(pair<int, int>(rnumber, j));
    if (pthread_mutex_unlock(&m1) < 0)
    {
      perror("pthread_mutex_lock");
      exit(-1);
    }
  }

  return NULL;
}


int main(int argc, char *argv[])
{
  int thread_count = atoi(argv[1]);
  pthread_t tid[thread_count];
  srand (time(NULL));
  for(int i = 0; i < thread_count; i++)
  {
    if (pthread_create(&tid[i], NULL, randomBinaryTree, NULL) != 0)
    {
      perror("pthread_create");
      exit(-1);
    }
  }
  for(int i = 0; i < thread_count; i++)
  {
    if(pthread_join(tid[i], NULL) != 0)
    {
      perror("pthread_join");
      exit(-1);
    }
  }
  map<int, int>::iterator iPtr;
  for(iPtr = rMap.begin(); iPtr != rMap.end(); ++iPtr)
  {
    cout << iPtr->first <<  " ";
  }
  cout << endl;
}
