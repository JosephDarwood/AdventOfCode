#include <iostream>
#include <vector>
#include <string>
#include <fstream>



class RangeKeeper{
public:
	std::vector<std::pair<std::string, std::string>> ranges;
	void fillRanges(std::string input){
		size_t commaIndex = input.find(',');
		if(commaIndex == std::string::npos){
			addRange(input);
			return;
		}
		addRange(input.substr(0, commaIndex)); 
		fillRanges(input.substr(commaIndex+1, input.size()-1-commaIndex));
	}
	void addRange(std::string input){
		int dashIndex = input.find('-');
		std::string first = input.substr(0, dashIndex);
		std::string second = input.substr(dashIndex+1, input.size()-1-dashIndex);
		ranges.push_back({first, second});
	}
	

	std::pair<long long, long long> sumRanges(){
		long long sum1 = 0;
		long long sum2 = 0; 
		for(std::pair<std::string, std::string> p : ranges){
			std::pair<long long, long long> temp = sumRange(p);
			sum1 += temp.first;
			sum2 += temp.second;
		}
		return {sum1, sum2};
	}
	std::pair<long long, long long> sumRange(std::pair<std::string, std::string> range){
		std::cout << "start range: " << range.first << ":" << range.second << std::endl; 
		long long start = std::stoll(range.first);
		long long end = std::stoll(range.second);

		long long sum1 = 0;
		long long sum2 = 0;

		for(long long i = start; i <= end; i++){
			std::string curr = std::to_string(i); 

			if(naiveInvalid(curr)) sum1 += i; 
			if(newInvalid(curr)){
				sum2 += i;
				std::cout << i << std::endl;
			}
		
		}
		return {sum1 ,sum2};
	}
	
	bool naiveInvalid(std::string input){
		if(input.size()%2 != 0) return false; 
		return input.substr(0, input.size()/2) == input.substr(input.size()/2, input.size());
	}
	bool newInvalid(std::string input){
		for(int i = 1; i <= input.size()/2; i++){
			if(input.size() % i != 0) continue;
			std::vector<std::string> seen;
			for(int j = 0; j * i < input.size(); j++){
				int start = j*i; 
				seen.push_back(input.substr(start, i));
			}
			std::string base = seen[0];
			bool valid = true; 
			for(std::string s : seen){
				valid = valid && s==base;
			}
			if(valid) return valid;
		}
		return false;
	}
};

int main(){
	std::cout << "hello from day 2" << std::endl;
	std::fstream f("inputs/inputDay2.txt", std::fstream::in);
	std::string day2input; 
	std::getline(f, day2input);

	RangeKeeper rk;
	rk.fillRanges(day2input);

	for(auto p : rk.ranges){
		std::cout << p.first << "=" << p.second <<  std::endl;
	}
	std::pair<long long, long long> result = rk.sumRanges();
	std::cout << "naive result: " << result.first << std::endl;
	std::cout << "new result: " << result.second << std::endl;
	return 1;
}
