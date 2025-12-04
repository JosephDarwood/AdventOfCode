#include <ios>
#include <iostream>
#include <fstream> 
//first, I intend to fully simulate, then maybe go for optimal solve; 


class TurnSim{
public:
	int curTurn;
	int zeroTouches; 
	int zeroPasses; 
	TurnSim(int turn){
		curTurn = turn;
		zeroTouches = 0;
		zeroPasses = 0;
	}
	void turn(std::string turn){
		if(turn[0] == 'L') return this->leftTurn(std::stoi(turn.substr(1, turn.size()-1)));
		if(turn[0] == 'R') return this->rightTurn(std::stoi(turn.substr(1, turn.size()-1))); 
		int nightmare = 0; 
		nightmare = 30/nightmare;
	}
	void leftTurn(int turn){
		this->rightTurn(-turn);  
	}
	void rightTurn(int turn){
		curTurn += turn;
		if(curTurn < 0) zeroPasses++;
		zeroPasses += abs((curTurn)/100); 
		curTurn = curTurn % 100; 
		if(curTurn == 0) zeroTouches++; 
	}
};

class naiveTurner{
public:
	int curTurn; 
	int zeroPasses;
	int zeroTouches;
	naiveTurner(int turn){
		this->curTurn = turn;
		this->zeroTouches = 0;
		this->zeroPasses = 0;
	}
	void turn(std::string turn){
		int neg = (turn[0] == 'L') ? -1 : 1;
		int value = neg * std::stoi(turn.substr(1, turn.size()-1));
		curTurn += value;
		if(curTurn >= 100 && curTurn % 100 == 0) zeroTouches--;
		while(curTurn >= 100 || curTurn < 0){
			if(curTurn < 0) {
				this->zeroPasses++; 
				if(curTurn % 100 == 0) zeroTouches++;
				this->curTurn += 100; 
			}
			if(curTurn >= 100) {
				this->zeroPasses++;
				if(curTurn % 100 == 0) zeroTouches++;
				this->curTurn -= 100; 
			}
		}
		if(this->curTurn == 0){
			this->zeroPasses++;
			this->zeroTouches++;
		}
	}
};
class tardTurner{
public:
	int curTurn;
	int zeroPasses;
	int zeroTouches;
	tardTurner(int turn){
		this->curTurn = turn; 
		this->zeroPasses = 0;
		this->zeroTouches =0;
	}
	void turn(std::string turn){
		int right = (turn[0] == 'R') ? 1 : -1;
		int turns = std::stoi(turn.substr(1, turn.size()-1));
		for(int i = 0; i < turns; i++){
			std::cout << curTurn << std::endl;
			curTurn += right; 
			if(curTurn == 100) curTurn = 0;
			if(curTurn == -1) curTurn = 99; 
			if(curTurn == 0) zeroPasses++;
		}
		if(curTurn == 0) zeroTouches++;
	}
};
int main(){
	std::string path = "inputs/inputDay1.txt";
	std::fstream input{path, std::ios::in};
	TurnSim safe(50);
	naiveTurner safe2(50);
	tardTurner safe3(50);
	std::string turnCode;
	while(std::getline(input, turnCode)){
		safe.turn(turnCode);
		safe2.turn(turnCode);
		safe3.turn(turnCode);

	}
	std::cout << "Naive passed:" << safe2.zeroPasses << " saw:" << safe2.zeroTouches << std::endl;
	std::cout << "tard passed:" << safe3.zeroPasses << " saw:" << safe3.zeroTouches << std::endl;
	std::cout << "init passed" << safe.zeroPasses << " saw:" << safe.zeroTouches << std::endl;
	return safe.zeroTouches;
}

