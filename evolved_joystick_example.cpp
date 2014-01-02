/*
  Usage example for the EvolvedJoystick class
  note: "mutex.cpp" is also exploited here
 */

#include "evolved_joystick.cpp"
#include <iostream>
#include "mutex.cpp"

int _shared_number=0;
Mutex _mutex;

void example_function1(){
  std::cout << "function1" << std::endl;
  _mutex.lock();
  _shared_number = 1;
  _mutex.unlock();
}

void example_function2(){
  std::cout << "function2" << std::endl;
  _mutex.lock();
  _shared_number = 2;
  _mutex.unlock();
}

void example_function3(){
  std::cout << "function3" << std::endl;
  _mutex.lock();
  _shared_number = 3;
  _mutex.unlock();
}

void axis_function1(int value){
  std::cout << "axis1: " << value << std::endl;
}

void released_function(){
  std::cout << "released a button" << std::endl;
}

int main(int argc, char ** argv){
  
  std::cout << std::endl;
  
  EvolvedJoystick joy("/dev/input/js0");
  
  std::cout << "device name: " << joy.deviceName() << std::endl;
  std::cout << "number of buttons: " << joy.numOfButtons() << std::endl;
  std::cout << "number of axis: " << joy.numOfAxes() << std::endl;
  
  joy.setButtonPressFunction(0, example_function1);
  joy.setButtonPressFunction(1, example_function2);
  joy.setButtonPressFunction(2, example_function3);
  
  joy.setButtonReleaseFunction(0, released_function);
  joy.setButtonReleaseFunction(1, released_function);
  joy.setButtonReleaseFunction(2, released_function);
  
  joy.setAxisFunction(0, axis_function1);
  
  joy.launchListenerThread();
  
  while(true){
    std::cout << "timeout!" << std::endl << "last pressed button: ";
    _mutex.lock();
    std::cout << _shared_number << std::endl;
    _mutex.unlock();
    sleep(3);
  }
}
