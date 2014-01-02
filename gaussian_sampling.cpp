#include <iostream>
#include <stdlib.h>
#include <math.h>

#include <Eigen/Core>

static double uniform_rand(double min, double max){
  double r = double(rand())/RAND_MAX;
  // std::cout << r << std::endl;
  return min+r*(max-min);
}

static double gauss_rand(double mean, double sigma)
{
  double x, y, r2;
  do {
    x = -1.0 + 2.0 * uniform_rand(0.0, 1.0);
        y = -1.0 + 2.0 * uniform_rand(0.0, 1.0);
        r2 = x * x + y * y;
  } while (r2 > 1.0 || r2 == 0.0);
  return mean + sigma * y * sqrt(-2.0 * log(r2) / r2);
} 



int main(int argc, char ** argv){
  
  double mean = 0;
  double sigma = 1;
  
  if(argc>1){
    mean = atof(argv[1]);
    if(argc>2){
      sigma = atof(argv[2]);
    }
  }
  
  unsigned int dist_size = 1000;
  int distance[dist_size];
  for(unsigned int i=0; i<dist_size; i++){
    distance[i] = 0;
  }
  
  unsigned int extractions_number = 1000000;
  
  for(unsigned int i=0; i<extractions_number; i++){
    
    double r = mean - gauss_rand(mean,sigma);
    if(r<0) r = -r;
    
    unsigned int index = r;
    
    //    std::cout << r << " --> " << index << std::endl;
    
    distance[index]++;
    
  }
  
  
  unsigned int last_non_zero_index = 0;
  for(unsigned int i=0; i<dist_size; i++){
    if (distance[i] !=0){
      last_non_zero_index = i;
    }
  }
  
  for(unsigned int i = 0; i<last_non_zero_index; i++){
    std::cout << "within " << i+1 << ": " << (double)distance[i]/(double)extractions_number << std::endl;
  }

  return 0;
}
