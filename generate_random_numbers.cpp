#include <cstdlib>
#include <iostream>
#include <stdio.h>
// #include <string>
#include <sstream>

int main(int argc, char ** argv){
  
  if(argc > 1){
    srand(atoi(argv[1]));
  }
  
  double random_doubles[1000000];
  
  std::cout << "array size: " << sizeof(random_doubles)/sizeof(double) << std::endl;
  
  unsigned int iterations = 0;
  
  std::cout << "RAND_MAX is " << RAND_MAX << std::endl;
  
  for(unsigned int i=0; i<sizeof(random_doubles)/sizeof(double); i++){
    iterations++;
    random_doubles[i] = (double)rand()/RAND_MAX;
  }
  
  std::cout << "iterations: " << iterations << std::endl;
  std::cout << "last element: " << random_doubles[sizeof(random_doubles)/sizeof(double) - 1] << std::endl;
  
  
  // create and write the output file
  FILE * outfile = fopen("random_numbers", "w");
  
  if(outfile == NULL){
    std::cerr << "ERROR! cannot write output file." << std::endl;
    exit(1);
  }
  
  for (unsigned int i=0; i<sizeof(random_doubles)/sizeof(double); i++){
    std::ostringstream ss;
    ss << random_doubles[i];
    fputs(ss.str().c_str(),outfile);
    fputs("\n", outfile);
  }
  
  return 0;
}
