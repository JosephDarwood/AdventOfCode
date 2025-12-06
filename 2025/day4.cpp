#include <fstream>
#include <iostream> 
#include <string>
#include <vector>

class PaperFactory{
public:
	std::vector<std::vector<int>> factoryFloor; 	
	void addLayer(std::string input){
		std::vector<int> layer;
		for(char c : input){
			if(c == '.') layer.push_back(0);
			if(c == '@') layer.push_back(1);
		}
		this->factoryFloor.push_back(layer);
	}
	int liftAllRolls(){
		bool changing = true;
		int rollsLifted = 0;
		while(changing){
			std::cout << "Current Roll Count" << rollsLifted << std::endl;
			int beforeLift = rollsLifted; 
			rollsLifted += liftableRolls(); 
			if(beforeLift == rollsLifted) changing = false;
		}
		return rollsLifted;
	}
	int liftableRolls(){
		int lifted = 0; 
		for(int i = 0; i < factoryFloor.size(); i++){
			for(int j = 0; j < factoryFloor[i].size(); j++){
				if(this->factoryFloor[i][j] == 1 && liftable(i, j)){
					lifted++;
					this->factoryFloor[i][j] = 0;
				}
			}
		}
		return lifted;
	}
	bool liftable(int x, int y){
		int nearby = 0; 
		std::vector<bool> t = {
			validPos(x+1, y),
			validPos(x+1, y+1),
			validPos(x+1, y-1),
			validPos(x, y+1),
			validPos(x, y-1),
			validPos(x-1, y+1),
			validPos(x-1, y),
			validPos(x-1, y-1)
		};
		for(bool b : t) if (b) nearby++;
		return (nearby >= 5); 
	}
	bool validPos(int x, int y){
		int height = this->factoryFloor.size(); 
		int width = (height > 0) ? this->factoryFloor[0].size() : -1;
		if(x >= height || x < 0) return true; 
		if(y >= width || y < 0) return true;
		return (this->factoryFloor[x][y] == 0);
	}
};


void importFileToFactory(PaperFactory& pf, std::string path){
	std::fstream f(path, std::fstream::in); 
	std::string layer;
	while(std::getline(f, layer)){
		pf.addLayer(layer);
	}
}


int main(){
	std::cout << "Hello from day 4" << std::endl;
	PaperFactory pf; 
	std::string path = "inputs/inputDay4.txt";
	importFileToFactory(pf, path);
	int lifts = pf.liftAllRolls();
	std::cout << "We can lift " << lifts <<std::endl;
	return 1;
}
