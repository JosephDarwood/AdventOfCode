#include <climits>
#include <fstream>
#include <iostream> 
#include <vector>

int pairOverlap(std::pair<long long, long long> base, std::pair<long long, long long> ontop);
std::pair<long long, long long> pairFuse(std::pair<long long, long long> a, std::pair<long long, long long> b);
class SpoilageTracker{
public:
	std::vector<long long> items; 
	std::vector<std::pair<long long, long long>> freshRanges;
	void addSpoilerRange(std::pair<long long, long long> addRange){
		std::cout << "Try add " << addRange.first << "," << addRange.second << std::endl; 
		if(this->freshRanges.size() == 0){
			freshRanges.push_back(addRange);
			return;
		}
		int index = 0; 
		bool finding = true;
		//loop over every index 
		while(finding && index < this->freshRanges.size()){
			std::pair<long long, long long> check = this->freshRanges[index];
			int pairState = pairOverlap(check, addRange); 
			switch(pairState){
				case -2:
					finding = false;
				break;
				case -1:
				[[fallthrough]];
				case 0:
				[[fallthrough]];
				case 1:
					std::cout << "Ok now we fuse with " << check.first << "," << check.second << std::endl; 
					freshRanges.erase(freshRanges.begin()+index);
					this->addSpoilerRange(pairFuse(addRange, check)); 
					return; 
				case 2:
					index++; 
			}
		} 
		freshRanges.insert(freshRanges.begin()+index, addRange); 

	}
	void addItem(long long item){
		items.push_back(item);
	}
	int countSpoilage(){
		int spoilage = 0;
		for(long long item : items){
			for(std::pair<long long, long long> range : freshRanges){
				if(item >= range.first && item <= range.second){
					spoilage++; 
					std::cout << "Mark as spoiled: " << item << std::endl;
					break;
				}
			}
		}
		return spoilage;
	}
	long long totalFreshRange(){
		long long freshTotal = 0;
		for(std::pair<long long, long long> range : freshRanges){
			freshTotal += range.second+1; 
			freshTotal -= range.first; 
		}
		return freshTotal;
	}

};
//-2 is ontop left of base, 2 is ontop right of base
//-1 is left overlap, 0 is full engulf, 1 is right overlap 
int pairOverlap(std::pair<long long, long long> base, std::pair<long long, long long> ontop){
	if(base.second < ontop.first) return 2; 
	if(ontop.second < base.first) return -2;
	if(ontop.second <= base.second && ontop.first >= base.first) return 0;
	if(ontop.second >= base.second && ontop.first <= base.first) return 0; 
	if(ontop.second < base.second && ontop.first < base.first && base.second >= ontop.first) return -1; 
	if(ontop.second > base.second && ontop.first > base.first && base.first <= ontop.second) return 1;
	return INT_MAX; //haha eat a exception for letting this code boom; 
}
std::pair<long long, long long> pairFuse(std::pair<long long, long long> a, std::pair<long long, long long> b){
	long long start = (a.first < b.first) ? a.first : b.first; 
	long long end = (a.second > b.second) ? a.second : b.second;
	return {start, end}; 
}

void testPairOverlap(){
	std::vector<std::pair<std::pair<long long, long long>, std::pair<long long, long long>>> testPairs = {
		{{11, 14}, {10, 15}},
		{{10, 15}, {11, 14}},
		{{8, 12}, {10, 15}},
		{{10, 15}, {8, 12}},
		{{1, 5}, {10, 15}},
		{{10, 15}, {20, 25}},
		{{10, 15}, {1, 5}}
	};
	for(std::pair<std::pair<long long, long long>, std::pair<long long, long long>> pop : testPairs){
		std::cout << "Testing (" << pop.first.first << "-" << pop.first.second <<") and ("<<pop.second.first<<"-"<<pop.second.second << ") overlaps to:" << pairOverlap(pop.first, pop.second) << std::endl;
	}


}
void readIntoStorage(SpoilageTracker& st, std::string s){
	if(s == "") return;
	size_t dashpoint = s.find('-'); 
	if(dashpoint == std::string::npos){
		st.addItem(std::stoll(s));
		return;
	}
	long long start = std::stoll(s.substr(0, dashpoint));
	long long end = std::stoll(s.substr(dashpoint+1, s.size()));
	st.addSpoilerRange({start, end});
	return;
}

int main(){
	std::string path = "inputs/inputDay5.txt";
	std::fstream f(path, std::fstream::in);	
	std::string input;
	SpoilageTracker st;
	while(std::getline(f, input)){
		readIntoStorage(st, input); 
	}
	std::cout << "Hello from day 5!" << std::endl;
	std::cout << "Ranges : ";
	for(std::pair<long long, long long> p : st.freshRanges) std::cout << p.first << "-" << p.second << " ";
	std::cout << "\n"; 
	int spoilage = st.countSpoilage();
	long long freshRange = st.totalFreshRange();
	std::cout << "Spoilage: " << spoilage << std::endl;
	std::cout << "Fresh Range Total: " << freshRange << std::endl;
	//testPairOverlap();	
}
