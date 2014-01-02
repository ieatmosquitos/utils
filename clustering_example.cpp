#include <iostream>
#include "clustering.cpp"

int main(int argc, char ** argv){
  
  double v[] = {1,1,2,2,1.4,1.4,-2.8,-2.8,-3,-3,-2,-2,20,-20,22,-22,25,25,19,-19};
  int labels[10];
  
  int my_opinion = stableFindClusters(v,2,labels,10);
  
  if(my_opinion == -1){
    std::cout << "WE ARE ALL GOING TO DIE!!!!!!!" << std::endl;
  }
  
  std::cout << std::endl;
  
  std::cout << "in my opinion, there are " << my_opinion << " clusters" << std::endl;
  std::cout << "my opinion labels: <";
  for(unsigned int n=0; n<10; n++){
    std::cout << " " << labels[n];
  }
  std::cout << " >" << std::endl;
  
  std::cout << std::endl;
  return 0;
}
