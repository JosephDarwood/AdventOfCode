#include <fstream>
#include <iostream>
#include <string>
#include <vector> 


//This abomination requires a good input, no funny biz. 
class MathSolver{
public:
	std::vector<std::vector<long long>> mathables;
	std::vector<std::string> rtlMathables;
	std::vector<char> operands; 
	void importSymbols(std::string input){
		if(input == "") return;
		char op = input.at(0);
		operands.push_back(op);
		importSymbols(input.substr(2, input.size()-2));
	}

	void importValues(std::string input, bool start = false){
		if(start) mathables.push_back({});
		if(input == "") return;
		size_t commaPoint = input.find(',');
		if(commaPoint == std::string::npos){
			mathables.back().push_back(stoll(input));
			return;
		}
		mathables.back().push_back(stoll(input.substr(0, commaPoint)));
		importValues(input.substr(commaPoint+1, input.size()-commaPoint-1));
	}
	long long calculateResults(){
		std::vector<long long> mathResults(mathables[0].size(), 0);  
		for(int i = 0; i < mathables[0].size(); i++){
			if(operands[i] == '*') mathResults[i] = 1; 
			for(int j = 0; j < mathables.size(); j++){
				if(operands[i] == '+') mathResults[i] += mathables[j][i];
				if(operands[i] == '*') mathResults[i] *= mathables[j][i];
			}
		}
		long long total = 0;
		for(long long l : mathResults){
			total += l;
		}
		return total;
	}
	void importRTL(std::string input){
		rtlMathables.push_back(input);		
	}
	long long calculateRTL(){
		long long curNumber = 0; 
		std::vector<long long> numberQue;
		std::vector<long long> results;
		for(int column = rtlMathables[0].size()-1; column >= 0; column--){
			for(int row = 0; row < rtlMathables.size(); row++){
				char current = rtlMathables[row].at(column);
				
				if(current == ' ') continue;
				if(current == '*' || current == '+'){
					numberQue.push_back(curNumber); 
					curNumber = 0; 
					long long curTot = 0;
					if(current == '*') curTot = 1;
					for(long long ln : numberQue){
						if(current == '*') curTot *= ln;
						if(current == '+') curTot += ln;
					}
					std::cout << "added " << curTot << " to results\n";
					results.push_back(curTot);
					numberQue.clear();
					continue;
				}
				int number = current - '0';
				curNumber *= 10; 
				curNumber += number;
				
				std::cout << current << '-'; 
			}
			std::cout << "added " << curNumber << " to que" << std::endl;
			if(curNumber != 0)numberQue.push_back(curNumber);
			curNumber = 0; 
		}
		long long finalSum = 0;
		for(long long ln : results){
			finalSum += ln;
		}
		return finalSum;

	}
};

void importMath(std::string input, MathSolver& ms){
	if(input.at(0) == '+' || input.at(0) == '*'){
		return ms.importSymbols(input);
	}
	return ms.importValues(input, true);
}
std::string spaceToComma(std::string input){
	std::string commed = "";
	int firstReal = 0;
	while(input.at(firstReal) == ' ') firstReal++;
	commed.push_back(input.at(firstReal));
	for(int i = firstReal+1; i < input.size(); i++){
		if(commed.back() == ',' && input.at(i) == ' ') continue;
		if(commed.back() != ',' && input.at(i) == ' '){
			commed.push_back(',');
			continue;
		}
		commed.push_back(input.at(i));
	}
	return commed;
}
int main(){
	std::cout << "Hello from day 6!" << std::endl;
	std::string path = "inputs/inputDay6.txt";
	std::fstream f(path, std::fstream::in);
	std::string input; 

	MathSolver ms;
	while(std::getline(f, input)){
		ms.importRTL(input);
		input = spaceToComma(input);
		importMath(input, ms);
	}

	long long result = ms.calculateResults();
	long long rtlResult = ms.calculateRTL();
	std::cout << "Current result: " << result << std::endl;
	std::cout << "Current rtlResult: " << rtlResult << std::endl;
}

