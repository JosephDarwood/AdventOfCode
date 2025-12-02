#include <ios>
#include <iostream>
#include <fstream> 
//first, I intend to fully simulate, then maybe go for optimal solve; 


class TurnSim{
public:
	int curTurn;
	int zeroTouches; 
	TurnSim(int turn){
		curTurn = turn;
		zeroTouches = 0; 
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
		curTurn = curTurn % 100; 
		if(curTurn == 0) zeroTouches++; 
	}
};


int main(){
	std::string path = "inputs/inputDay1.txt";
	std::fstream input{path, std::ios::in};
	TurnSim safe(50);
	std::string turnCode;
	while(std::getline(input, turnCode)){
		safe.turn(turnCode);
	}
	std::cout << "Exited!" << std::endl;
	std::cout << "Answer is " << safe.zeroTouches << std::endl;
	return safe.zeroTouches;
}

