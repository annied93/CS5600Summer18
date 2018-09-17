// Implement your part 2 solution here
// gcc -lpthread threads.c -o threads



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

long numberOfArtists = 64;

//char colors[64][64*3];
long colors[64][64*3];

// Add your paint function here
void *paint(void *workid) {

  long i;

  int c;

  long tid;
  tid = (long)workid;

  printf("Child workid = %d\n", workid);
  for(i=0; i<64*3; i++){

    // just a little logic to create a red gradient
    c = (i+1) % 3;
    if (c==0) {
      colors[tid][i] = 255 - (tid*3);
    } else {
      colors[tid][i] = 0;
    }

  }

}

void writePPM() {

  // Write out the PPM file
  // If you are looking at this solution, it could
  // be better laid out in a 'save function'

  int i;
  int j;

  FILE *fp;
  fp = fopen("threads.ppm","w+");
  fputs("P3\n",fp);
  fputs("64 64\n",fp);
  fputs("255\n",fp);
  for(i =0; i < 64;i++){
    for(j =0; j < 64*3; j++){
      fprintf(fp,"%d",colors[i][j]);
      fputs(" ",fp);
    }
    fputs("\n",fp);
  }
  fclose(fp);

}

int main(int argc, char** argv){

  long t;

  //pid_t pid1;
  int rc;

  int childPids[numberOfArtists];
  //int *myArray = malloc(numberOfArtists * sizeof(int));

  pthread_t threads[numberOfArtists];

  for(t=0; t<numberOfArtists; t++){
    // (1) Perform a fork
    //pid1 = vfork();
    //printf("In main: creating thread %ld\n", i);
    rc = pthread_create(&threads[t], NULL, paint, (void *)t);
   
    //printf("Return code from pthread_create() is %d\n", rc);
   
    // (2) Make only the child do some work

    // (3) Print out the child pid (from the parent--mostly for logging purpose\s)

  }

//for(int i =0; i < numberOfArtists; i++){
//  printf("Child pid = %d\n", childPids[i]);
// }

//free(myArray);

writePPM();

pthread_exit(NULL);

printf("parent is exiting(last artist out!)\n");

return 0;
}
