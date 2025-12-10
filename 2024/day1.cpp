#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class ListDiffer{
public:
	std::vector<int> leftList; 
	std::vector<int> rightList; 
	long long totalDiff(){

	}
};

void mySorter(std::vector<int>&  list){
	std::vector<int> newlist; 
	for(int i : list){
		int insertIndex = 0; 
		while(insertIndex < newlist.size() && newlist[insertIndex] < i){
			insertIndex++; 
		}
		newlist.insert(newlist.begin()+insertIndex, i);
	}
	list = newlist;
}

int main(){
	std::string path = "inputs/inputDay1.txt";
	std::fstream f(path, std::fstream::in);
	std::string input;
	ListDiffer ld;
	while(getline(f, input)){
		int firstSpace = input.find(' ');
		int lastInt = firstSpace; 
		while(input.at(lastInt) == ' ') lastInt++; 
		ld.leftList.push_back(stoi(input.substr(0, firstSpace)));
		ld.rightList.push_back(stoi(input.substr(lastInt, input.size()-lastInt)));
	}
	std::cout << "Hello from day 1!" << std::endl;
	std::cout << "Diff is: " << ld.totalDiff() << std::endl;
}
