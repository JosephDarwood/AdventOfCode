#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class VoltManager{
public:
	std::vector<std::vector<int>> batteryBank; 
	void addBank(std::string s){
		std::vector<int> currentBank; 
		for(char c : s){
			currentBank.push_back(c-'0');
		}
		batteryBank.push_back(currentBank);
	}
	int getTotal(){
		int total = 0;
		for(std::vector<int> v : this->batteryBank) total += voltFromLine(v);
		return total;
	}
	long long getNewTotal(){
		long long total = 0; 
		for(std::vector<int> v : this->batteryBank){
			total += voltFromN(v, 12, 0);
			std::cout << total << std::endl;
		}
		return total;
	}
	int voltFromLine(std::vector<int> battery){
		std::vector<int> leftMax = battery;
		for(int i = 1; i < battery.size(); i++){
			leftMax[i] = std::max(leftMax[i], leftMax[i-1]);
		}
		int maxCombo = 0;
		for(int i = 1; i < battery.size(); i++){
			maxCombo = std::max(maxCombo, battery[i]+(leftMax[i-1]*10));
		}
		return maxCombo;

	}
	long long tenPow(int power){
		long long total = 1; 
		for(int i = 0; i < power; i++){
			total *= 10; 
		}
		return total;
	}
	long long voltFromN(std::vector<int> input, int length, int start){
		if(length == 0) return 0; 
		length -= 1; 
		int firstMaxInd = start;
		for(int i = start; i < input.size()-length; i++){
			if(input[i] > input[firstMaxInd]) firstMaxInd = i; 
		}
		//std::cout << input[firstMaxInd] * tenPow(length) << std::endl;
		return ((long long)input[firstMaxInd] * tenPow(length)) + voltFromN(input, length, firstMaxInd+1);  
	}

};

int main(){
	std::cout<< "Hello from day 3" << std::endl;
	std::string path = "inputs/inputDay3.txt";
	std::fstream f(path, std::fstream::in);
	std::cout << f.is_open();
	VoltManager vm; 
	std::string input;
	while(getline(f, input)){
		vm.addBank(input);
	}
	int total = vm.getTotal();
	long long t = vm.getNewTotal();
	std::cout << "longest is " << t << std::endl;
	//std::cout << "total is " << total << std::endl;
	return 1;
}
