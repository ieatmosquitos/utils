#include <iostream>
#include <vector>
#include <stdlib.h> // malloc

inline int absolute(const int & value){
	if(value < 0) return -value;
	return value;
}

void bresenham(const int & x0,const int & y0, const int & x1, const int & y1, std::vector<std::pair<int, int> > & linepoints){
	bool moreX = true;
	int deltax = absolute(x1-x0);
	int deltay = absolute(y1-y0);
	
	// degenerate cases: purely horizontal or vertical line
	if(deltay == 0){
		int y = y0;
		int xstep = (x1>x0 ? 1 : -1);
		for (int x=x0; x!=x1; x+=xstep){
			linepoints.push_back(std::pair<int, int>(x,y));
		}
		return;
	}

	if(deltax == 0){
		int x = x0;
		int ystep = (y1>y0 ? 1 : -1);
		for (int y=y0; y!=y1; y+=ystep){
			linepoints.push_back(std::pair<int, int>(x,y));
		}
		return;
	}
	

	int xstep = (x1>x0 ? 1 : -1);
	int ystep = (y1>y0 ? 1 : -1);

	// check orientation of the line
	if(deltax < deltay){
		moreX = false;
	}
	
	if(moreX){
		int error = deltax/2;
		int y = y0;
		for (int x = x0; x!=x1; x+=xstep){
			linepoints.push_back(std::pair<int, int>(x, y));
			error -= deltay;
			if (error < 0){
				y += ystep;
				error += deltax;
			}
		}
	}
	else{
		int error = deltay/2;
		int x = x0;
		for (int y = y0; y!=y1; y+=ystep){
			linepoints.push_back(std::pair<int, int>(x, y));
			error -= deltax;
			if (error < 0){
				x += xstep;
				error += deltay;
			}
		}
		
	}

	return;
}

int main(int argc, char ** argv){
	if(argc < 5){
		std::cout << "Usage: bresenham <x0> <y0> <x1> <y1>" << std::endl;
		return 1;
	}

	int rows = 15;
	int cols = 20;

	int x0 = atoi(argv[1]);
	int y0 = atoi(argv[2]);
	int x1 = atoi(argv[3]);
	int y1 = atoi(argv[4]);

	int ** map;
	map = (int**)malloc(sizeof(int*) * 20);
	for(unsigned int i=0; i<rows; i++){
		map[i] = (int*)malloc(sizeof(int) * cols);
	}
	// put everything on the map to 0
	for (unsigned int r=0; r<rows; r++){
		for(unsigned int c=0; c<cols; c++){
			map[r][c] = 0;
		}
	}

	std::vector<std::pair<int, int> > line;

	bresenham(x0, y0, x1, y1, line);

	for(unsigned int i=0; i<line.size(); i++){
		int x = line[i].first;
		int y = line[i].second;
		
		if(x < cols && x > 0 && y < rows && y>0){
			map[y][x] = 1;
		}
	}

	std::cout << "map:" << std::endl;
	for (unsigned int r=0; r<rows; r++){
		std::cout << "\t";
		for(unsigned int c=0; c<cols; c++){
			if(map[r][c]){
				std::cout << map[r][c];
			}
			else{
				std::cout << "_";
			}
				
			std::cout << "\t";
		}
		std::cout << std::endl << std::endl;
	}

	return 0;
}
