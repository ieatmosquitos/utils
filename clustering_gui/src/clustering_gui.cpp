#include <iostream>
#include <unistd.h>

#include "../../clustering.cpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

sf::RenderWindow _window;
sf::View _camera;
sf::Rect<float> _camera_rect;
sf::Image _backgroundImage;
sf::Sprite _backgroundSprite;
sf::Image _landmarkImage;
sf::Sprite _landmarkSprite;
sf::Image _meansImage;
sf::Sprite _meansSprite;

int window_width = 1024;
int window_height = 700;

std::vector<double *> samples;
double * means;
int clustersNum;

void clusterize(){
  double v[samples.size()*2];
  
  // populate v
  for(unsigned int s=0; s<samples.size(); s++){
    unsigned int vindex = s*2;
    v[vindex] = samples[s][0];
    v[vindex+1] = samples[s][1];
  }
  
  
  int labels[samples.size()];
  clustersNum = stableFindClusters(v, 2, labels, &means, samples.size(), 20);
  
  std::cout << std::endl << "in my opinion, there are " << clustersNum << " clusters." << std::endl;
  std::cout << "The centroids are located in: ";
  for(unsigned int i=0; i<clustersNum; i++){
    std::cout << " <" << means[i*2] << ", " << means[i*2 + 1] << " >";
  }
  std::cout << std::endl;
  
}


void quit(){
  for(unsigned int i=0; i<samples.size(); i++){
    free(samples[i]);
  }
  
  free(means);
  
  exit(0);
}


void init(){
  _backgroundImage.LoadFromFile("assets/wood-puzzle-floor.png");
  _backgroundSprite.SetImage(_backgroundImage);

  _landmarkImage.LoadFromFile("assets/landmark.png");
  _landmarkSprite.SetImage(_landmarkImage);
  _landmarkSprite.SetCenter(_landmarkSprite.GetSize().x/2, _landmarkSprite.GetSize().y/2);
  
  _meansImage.LoadFromFile("assets/means.png");
  _meansSprite.SetImage(_meansImage);
  _meansSprite.SetCenter(_meansSprite.GetSize().x/2, _meansSprite.GetSize().y/2);
  
  
  _window.Create(sf::VideoMode(window_width,window_height,32), "CLUSTERING");
  _camera_rect = sf::Rect<float>(0,0,window_width,window_height);
  
  means = (double *) malloc(sizeof(double));
  
  clustersNum = 0;
}


void draw(){
  _window.Clear();
  
  _camera.SetFromRect(_camera_rect);
  
  // draw background
  _window.Draw(_backgroundSprite);
  
  // draw samples
  for(unsigned int i=0; i<samples.size(); i++){
    _landmarkSprite.SetPosition(samples[i][0] + window_width/2, window_height/2 - samples[i][1]);
    _window.Draw(_landmarkSprite);
  }
  
  // draw means
  for(unsigned int c=0; c<clustersNum; c++){
    unsigned int mindex = c*2;
    _meansSprite.SetPosition(means[mindex] + window_width/2, window_height/2 - means[mindex+1]);
    _window.Draw(_meansSprite);
  }
  
  _window.Display();
}


void mouseLeftPressed(float x, float y){
  double * s = (double*) malloc(2*sizeof(double));
  s[0] = x - (double)window_width/2;
  s[1] = (double)window_height/2 - y;
  samples.push_back(s);
  std::cout << "point in " << s[0] << ", " << s[1] << std::endl; 
}


int handleEvents(){
  sf::Event event;
  while(_window.GetEvent(event)){
    switch(event.Type){
    case sf::Event::KeyPressed:
      switch(event.Key.Code){
      case sf::Key::Escape:
	quit();
	break;
      case sf::Key::C:
	clusterize();
	break;
      default:
	break;
      }	// end of key switch
    case sf::Event::MouseButtonPressed:
      if(event.MouseButton.Button == sf::Mouse::Left){
	mouseLeftPressed(event.MouseButton.X, event.MouseButton.Y);
      }
      break;
    default:
      break;
    }
  }
  
  return 0;
}


int main(int argc, char ** argv){
  
  init();
  
  while(true){
    handleEvents();
    
    draw();
    
    usleep(1000);
  }
  
  return 0;
}
