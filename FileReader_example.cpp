// this file is to show a simple example on the use of the FileReader class

#include "FileReader.cpp"
#include <string>
#include <iostream>

int main(int argc, char** argv){
  
  if(argc < 2){
    std::cout << "Usage: example <filename>" << std::endl;
    return 0;
  }
  
  std::string filename(argv[1]);
  
  FileReader fr(filename);
  if(!fr.is_open()){
    std::cout << "cannot open file " << filename << ", quitting." << std::endl;
    return 1;
  }
  
  std::vector<std::string> textline;
  fr.readLine(&textline);
  while(fr.good()){
    for(unsigned int i=0; i<textline.size(); i++){
      if (i>0) std::cout << " ";
      std::cout << textline[i]; 
    }
    std::cout << std::endl;
    
    textline.clear();
    fr.readLine(&textline);
  }
  
  return 0;
}
