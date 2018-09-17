// Implement your part 1 solution here
// gcc vfork.c -o vfork

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int numberOfArtists = 64;

//char colors[64][64*3];
int colors[64][64*3];

// Add your paint function here
void paint(int workid) {

  int c;

  printf("Child workid = %d\n", workid);
  
  for(int i = 0; i < 64*3; i++){

    // just a little logic to create a red gradient
    c = i % 3;
    if (c==0) {
      colors[workid][i] = 255 - (workid*3);
    } else {
      colors[workid][i] = 0;
    }
  }

}

void writePPM() {

  // Write out the PPM file
  // If you are looking at this solution, it could
  // be better laid out in a 'save function'
  FILE *fp;
  fp = fopen("vfork.ppm","w+");
  fputs("P3\n",fp);
  fputs("64 64\n",fp);
  fputs("255\n",fp);
  for(int i =0; i < 64;i++){
    for(int j =0; j < 64*3; j++){
      fprintf(fp,"%d",colors[i][j]);
      fputs(" ",fp);
    }
    fputs("\n",fp);
  }
  fclose(fp);

}

int main(int argc, char** argv){

  pid_t pid1;
  //int childPids[numberOfArtists];
  //int *myArray = malloc(numberOfArtists * sizeof(int));

  for(int i =0; i < numberOfArtists; i++){
    // (1) Perform a fork
    pid1 = vfork();

    // (2) Make only the child do some work
                
    // (3) Print out the child pid (from the parent--mostly for logging purposes)

    if (pid1 == 0) {
      paint(i);
      exit(0);
    } else {
      //childPids[i] = pid1;
      waitpid(pid1);
    }

  }

  //for(int i =0; i < numberOfArtists; i++){
  //printf("Child pid = %d\n", childPids[i]);
  //}

  //free(myArray);

  writePPM();

  printf("parent is exiting(last artist out!)\n");

  return 0;
}
