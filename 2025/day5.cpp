#include <climits>
#include <fstream>
#include <iostream> 
#include <vector>


class SpoilageTracker{
public:
	std::vector<int> items; 
	std::vector<std::pair<int, int>> freshRanges;
	void addSpoilerRange(int start, int end){
		std::pair<int, int> ins(start, end);
		if(this->freshRanges.size() == 0) freshRanges.push_back({start, end});
		int index = 0; 
		//loop over every index 
		while(index < this->freshRanges.size()){
			std::pair<int, int> check = this->freshRanges[index];

			index++; 
		}

	}
	void addItem(int item){
		items.push_back(item);
	}

};
//-2 is ontop left of base, 2 is ontop right of base
//-1 is left overlap, 0 is full engulf, 1 is right overlap 
int pairOverlap(std::pair<int, int> base, std::pair<int, int> ontop){
	if(base.second < ontop.first) return 2; 
	if(ontop.second < base.first) return -2;
	if(ontop.second <= base.second && ontop.first >= base.first) return 0;
	if(ontop.second >= base.second && ontop.first <= base.first) return 0; 
	if(ontop.second < base.second && ontop.first < base.first && base.second >= ontop.first) return -1; 
	if(ontop.second > base.second && ontop.first > base.first && base.first <= ontop.second) return 1;
	return INT_MAX; //haha eat a exception for letting this code boom; 
}
void testPairOverlap(){
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> testPairs = {
		{{11, 14}, {10, 15}},
		{{10, 15}, {11, 14}},
		{{8, 12}, {10, 15}},
		{{10, 15}, {8, 12}},
		{{1, 5}, {10, 15}},
		{{10, 15}, {20, 25}},
		{{10, 15}, {1, 5}}
	};
	for(std::pair<std::pair<int, int>, std::pair<int, int>> pop : testPairs){
		std::cout << "Testing (" << pop.first.first << "-" << pop.first.second <<") and ("<<pop.second.first<<"-"<<pop.second.second << ") overlaps to:" << pairOverlap(pop.first, pop.second) << std::endl;
	}


}
void readIntoStorage(SpoilageTracker& st, std::string s){
	if(s == "") return;
	size_t dashpoint = s.find('-'); 
	if(dashpoint == std::string::npos){
		st.addItem(std::stoi(s));
		return;
	}
	int start = std::stoi(s.substr(0, dashpoint));
	int end = std::stoi(s.substr(dashpoint+1, s.size()));
	st.addSpoilerRange(start, end);
	return;
}

int main(){
	std::string path = "inputs/sampleDay5.txt";
	std::fstream f(path, std::fstream::in);	
	std::string input;
	SpoilageTracker st;
	while(std::getline(f, input)){
		//readIntoStorage(st, input); 
	}
	std::cout << "Hello from day 5!" << std::endl;
	

	testPairOverlap();	
}
