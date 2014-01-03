/*
  This class allows the fast creation of a joystick interface that associates user-defined functions to buttons and axes
  note: makes use of LIBPTHREAD
 */

// TODOS:
//	• remove placeHolderFunction and use "assigned functions" boolean arrays
//	• Keep internal state of the joystick and provide functions for online offset adjusting (useful for defective joypads)

#include <fcntl.h> // definitions
#include <unistd.h> // devices io (read, close, ...)
#include <linux/joystick.h> // js_event structure, definitions
#include <iostream> // cerr, endl
#include <string>
#include <stdlib.h> // exit
#include <pthread.h>

// empty functions for avoiding problems with unassigned buttons/axis
void placeHolderFunction(){};
void placeHolderFunction(int, int){};

// EvolvedJoystick specified later
class EvolvedJoystick;

// joyListener body is specified later
void * joyListener(void *);

class EvolvedJoystick{

  char _device_name[512];
  int _joy_fd;
  int * _axes = NULL;
  char _axes_number;
  char * _buttons = NULL;
  char _buttons_number;
  pthread_t _listenerThread;
  
  void (** _button_press_functions)() ;
  void (** _button_release_functions)() ;
  void (** _axis_functions)(int, int) ;
  
public:
  
  EvolvedJoystick( std::string device_)
  {
  
    _joy_fd = open(device_.c_str(), O_RDONLY);
    if(_joy_fd == -1){
      std::cerr << "ERROR: cannot open device " << device_ << std::endl << "quitting..." << std::endl << std::endl;
      exit(1);
    }
    
    ioctl(_joy_fd, JSIOCGNAME(80), &_device_name);
    ioctl(_joy_fd, JSIOCGAXES, &_axes_number);
    ioctl(_joy_fd, JSIOCGBUTTONS, &_buttons_number);
    
    _axes = (int *) malloc(_axes_number * sizeof(int));
    _buttons = (char *) malloc(_buttons_number * sizeof(char));
    
    _button_press_functions = (void (**)()) malloc(_buttons_number * sizeof (void *));
    for(unsigned int i = 0; i<_buttons_number; i++){
      _button_press_functions[i] = placeHolderFunction;
    }
    _button_release_functions = (void (**)()) malloc(_buttons_number * sizeof (void *));
    for(unsigned int i = 0; i<_buttons_number; i++){
      _button_release_functions[i] = placeHolderFunction;
    }
    
    _axis_functions = (void (**)(int, int)) malloc(_axes_number * sizeof (void *));
    for(unsigned int i = 0; i<_axes_number; i++){
      _axis_functions[i] = placeHolderFunction;
    }
    
  }
  
  ~EvolvedJoystick(){
    if(_joy_fd != -1){
        close(_joy_fd);
	free(_axes);
	free(_buttons);
    }
  }
  
  
  void setButtonPressFunction(unsigned int button_number, void (*function_pointer_)()){
    
    if(button_number > _buttons_number){
      std::cerr << "WARNING: could not assign function to button " << button_number << ": joystick has less buttons" << std::endl;
      return;
    }
    
    _button_press_functions[button_number] = function_pointer_;
  }
  
  void setButtonReleaseFunction(unsigned int button_number, void (*function_pointer_)()){
    
    if(button_number > _buttons_number){
      std::cerr << "WARNING: could not assign function to button " << button_number << ": joystick has less buttons" << std::endl;
      return;
    }
    
    _button_release_functions[button_number] = function_pointer_;
  }
  
  void setAxisFunction(unsigned int axes_number, void (*function_pointer_)(int, int)){
    
    if(axes_number > _axes_number){
      std::cerr << "WARNING: could not assign function to axes " << axes_number << ": joystick has less axes" << std::endl;
      return;
    }
    
    _axis_functions[axes_number] = function_pointer_;
  }
  
  std::string deviceName(){
    return std::string(_device_name);
  }
  
  int deviceFD(){
    return _joy_fd;
  }
  
  int numOfButtons(){
    return (int) _buttons_number;
  }
  
  int numOfAxes(){
    return (int) _axes_number;
  }
  
  void launchListenerThread(){
    pthread_create(&_listenerThread, NULL, &joyListener, this);
  }
  
  void * buttonPressFunctions(){
    return (void *) _button_press_functions;
  }
  
  void * buttonReleaseFunctions(){
    return (void *) _button_release_functions;
  }
  
  void * axesFunctions(){
    return (void *) _axis_functions;
  }
  
};


void * joyListener(void * joyPointer){
  // gather useful infos
  EvolvedJoystick * joy = (EvolvedJoystick *) joyPointer;
  int numButt = joy->numOfButtons();
  int numAxes = joy->numOfAxes();
  void (** buttonPressFunctions)() = (void (**)()) joy->buttonPressFunctions();
  void (** buttonReleaseFunctions)() = (void (**)()) joy->buttonReleaseFunctions();
  void (** axisFunctions)(int, int) = (void (**)(int, int)) joy->axesFunctions();
  int joy_fd = joy->deviceFD();
  
  struct js_event jev;
  
  while(true){
    read(joy_fd, &jev, sizeof(struct js_event));
    
    //    std::cout << "jev.type: " << jev.type << "\tjev.number: " << jev.number << "\tjev.value: " << jev.value;
    
    switch(jev.type){
    case JS_EVENT_BUTTON:
      if (jev.value == 1){
	buttonPressFunctions[jev.number]();
      }
      else{
	buttonReleaseFunctions[jev.number]();	
      }
      break;
      
    case JS_EVENT_AXIS:
      axisFunctions[jev.number](jev.number, jev.value);
      break;
      
    default:
      break;
    }
    
    
  }
}
