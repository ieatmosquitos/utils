#include "mutex.cpp"

int main(int argc, char ** argv){
  Mutex m;
  m.lock();
  m.unlock();
  std::cout << "prova effettuata" << std::endl;
}
