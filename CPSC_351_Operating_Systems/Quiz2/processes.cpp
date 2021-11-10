#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

/* The prototypes */
void parent();
void child1();
void child2();
void child3();
void child4();
void child5();
void child6();
void child7();

/**
 * The function called by the FIRST child.
 */
void child1()
{
    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());
    //Ask him?
    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 < 0)
    {
      perror("fork");
      exit(-1);
    }
    else if (pid1 == 0)
    {
      child2();
      exit(0);
    }
    else
    {
      pid2 = fork();
      if(pid2 == 0)
      {
        child3();
        exit(0);
      }
      else
      {
        if(wait(NULL) < 0)
        {
          perror("wait");
        }
        if(wait(NULL) < 0)
        {
          perror("wait");
        }
      }
    }

    //TODO: Fill me in
}

/**
 * The function called by the SECOND child.
 */
void child2()
{
    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());

    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 < 0)
    {
      perror("fork");
      exit(-1);
    }
    else if (pid1 == 0)
    {
      child4();
      exit(0);
    }
    else
    {
      pid2 = fork();
      if(pid2 == 0)
      {
        child5();
        exit(0);
      }
      else
      {
        if(wait(NULL) < 0)
        {
          perror("wait");
        }
        if(wait(NULL) < 0)
        {
          perror("wait");
        }
      }
    }
    //TODO: Fill me in

}

/**
 * The function called by the THIRD child.
 */
void child3()
{
    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());
    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 < 0)
    {
      perror("fork");
      exit(-1);
    }
    else if (pid1 == 0)
    {
      child6();
      exit(0);
    }
    else
    {
      pid2 = fork();
      if(pid2 == 0)
      {
        child7();
        exit(0);
      }
      else
      {
        if(wait(NULL) < 0)
        {
          perror("wait");
        }
        if(wait(NULL) < 0)
        {
          perror("wait");
        }
      }
    }
}

/**
 * The function called by the FOURTH child.
 */
void child4()
{

    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());
}


/**
 * The function called by the FIFTH child.
 */
void child5()
{

    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());
}

/**
 * The function called by the SIXTH child.
 */
void child6()
{

    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());
}

/**
 * The function called by the SEVENTH child.
 */
void child7()
{

    fprintf(stderr, "I am child pid=%d; my parent is pid=%d\n", getpid(), getppid());
}

/**
 * The function called by the parent
 */
void parent()
{
    fprintf(stderr, "I am the original parent; my process ID is pid=%d\n", getpid());
    pid_t pid;
    pid = fork();

    if (pid < 0 )
    {
      perror("fork");
      exit(-1);
    }
    else if (pid == 0)
    {
      child1();
      exit(0);
    }
    else
    {
      if(wait(NULL) < 0)
      {
        perror("wait");
      }
    }


}


/**
 * The main function
 */
int main()
{
    parent();
}
