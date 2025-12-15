#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


struct Machine{
    int size; 
    std::vector<bool> requiredState;
    std::vector<int> requiredJoltage;
    std::vector<std::vector<int>> buttons;
};

std::vector<std::vector<bool>> comboGenerator(std::vector<bool> soFar, int startAt, int flipAfter){
    if(flipAfter == 0) return {soFar}; 
    std::vector<std::vector<bool>> combos; 
    for(int i = startAt; i < soFar.size()-(flipAfter-1); i++){
        std::vector<bool> currentCombo = soFar; 
        currentCombo[i] = true;
        std::vector<std::vector<bool>> spinoffs = comboGenerator(currentCombo, i+1, flipAfter-1);
        combos.insert(combos.end(), spinoffs.begin(), spinoffs.end());
    }
    return combos; 
}

Machine machinateString(std::string input){
    Machine m; 
    int firstIndex = input.find(' '); 
    int lastIndex = input.find_last_of(' ');

    std::string lights = input.substr(1, firstIndex-2); 
    std::string buttons = input.substr(firstIndex+1, lastIndex-firstIndex-1) + ' ';
    std::string joltage = input.substr(lastIndex+2, input.size()-lastIndex-3) + ',';
    //setup lights
    for(char c : lights){
        if(c == '.') m.requiredState.push_back(0);
        if(c == '#') m.requiredState.push_back(1);
    }
    //setup buttons
    std::size_t buttonIndex = buttons.find(' ');
    while(buttonIndex != std::string::npos){
        std::string buttonString = buttons.substr(1, buttonIndex-2) + ',';
        std::vector<int> currentButton;
        std::size_t commaIndex = buttonString.find(',');
        while(commaIndex != std::string::npos){
            std::string number = buttonString.substr(0, commaIndex);
            currentButton.push_back(std::stoi(number));
            buttonString = buttonString.substr(commaIndex+1, buttonString.size()-1-commaIndex);
            commaIndex = buttonString.find(',');
        }
        m.buttons.push_back(currentButton);
        buttons = buttons.substr(buttonIndex+1, buttons.size()-1-buttonIndex); 
        buttonIndex = buttons.find(' ');
    }
    //setup joltage 
    std::size_t commaIndex = joltage.find(',');
    while(commaIndex != std::string::npos){
        std::string jolt = joltage.substr(0, commaIndex);
        m.requiredJoltage.push_back(std::stoi(jolt));
        joltage = joltage.substr(commaIndex+1, joltage.size()-1-commaIndex);
        commaIndex = joltage.find(',');
    }
    if(m.requiredJoltage.size() != m.requiredState.size()) throw std::runtime_error("Bad machine was machinated");
    m.size = m.requiredJoltage.size(); 
    return m; 
}

class MachineManager{
private:
    struct MachineState{
        int machineIndex;
        std::vector<bool> buttonsUsed; 
    };
    bool checkState(const MachineState& state){
        std::vector<bool> currentState = std::vector<bool>(this->machineList[state.machineIndex].requiredState.size(), 0); 
        for(int i = 0; i < state.buttonsUsed.size(); i++){
            if(state.buttonsUsed[i]) for(int j : this->machineList[state.machineIndex].buttons[i]) currentState[j] = !currentState[j];
        }
        return currentState == this->machineList[state.machineIndex].requiredState;
    }
public:
    std::vector<Machine> machineList;
    

    void printMachines(){
        for(const Machine& m : this->machineList){
            std::cout << "Machine info:\nRequired State: ";
            for(const int i : m.requiredState) std::cout << i << ',';
            std::cout << "\nButtons: "; 
            for(const std::vector<int>& v : m.buttons){
                std::cout << "("; 
                for(int i : v) std::cout << i <<',';
                std::cout << ") "; 
            }
            std::cout << "\nJoltage: ";
            for(const int i : m.requiredJoltage) std::cout << i << ',';
            std::cout << '\n';
        }
    }
    int calculateMinOpSum(){
        int opSum = 0; 
        for(int i = 0; i < this->machineList.size(); i++){
            std::cout << "Machine " << i << " has started" << std::endl; 
            opSum += minNeeded(i);
            std::cout << "Machine " << i << " has calculated to total : "<< opSum << std::endl;
        }
        return opSum;
    }
    int minNeeded(int machine){
        std::vector<bool> targetState = this->machineList[machine].requiredState; 
        std::vector<bool> freshState(targetState.size(), false);
        std::vector<bool> freshButton(this->machineList[machine].buttons.size(), false); 
        for(int buttonsToPress = 1; buttonsToPress <= this->machineList[machine].buttons.size(); buttonsToPress++){
            std::vector<std::vector<bool>> permutations = comboGenerator(freshButton, 0, buttonsToPress);
            for(std::vector<bool> buttonCombo : permutations){
                std::vector<bool> trialVector = freshState;
                for(int i = 0; i < buttonCombo.size(); i++){
                    if(!buttonCombo[i]) continue;
                    for(int j : this->machineList[machine].buttons[i]){
                        trialVector[j] = !trialVector[j];
                    }
                }
                if(trialVector == targetState) return buttonsToPress;
            }
        }
        return 1; 
    }
};



int main(){
    std::cout << "Hello from day 10!" << std::endl;
    std::string path = "inputs/inputDay10.txt";
    std::fstream f(path, std::fstream::in);
    std::string input;

    MachineManager mm;
    while(std::getline(f, input)){
        mm.machineList.push_back(machinateString(input));
    }
    mm.printMachines();
    int minimumOperationsSummed = mm.calculateMinOpSum();
    std::cout << "Min op sum: " << minimumOperationsSummed << std::endl;
}
