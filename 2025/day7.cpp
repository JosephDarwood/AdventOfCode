#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int cti(char c) {
    if (c == '^')
        return 2;
    if (c == '|')
        return 3;
    if (c == '.')
        return 0;
    if (c == 'S')
        return 1;
    return -1;
}

class ManifoldManager {
  public:
    std::vector<std::vector<int>> safeManifold;
    std::vector<std::vector<int>> workManifold;
    void insertRow(std::string inputRow) {
        safeManifold.push_back({});
        for (char c : inputRow)
            safeManifold.back().push_back(cti(c));
    }
    int simulateRows(int source) {
        int splits = 0;
        for (int i = 0; i < this->workManifold[source].size(); i++) {
            int falling = this->workManifold[source][i];
            int destination = this->workManifold[source+1][i];
            switch(falling){
                case 2:
                case 0:
                    break;
                case 1:
                case 3:
                    switch(destination){
                        case 0:
                            this->workManifold[source+1][i] = 3;
                            break;
                        case 3:
                            break;
                        case 2:
                            std::cout << "found a split! col: " << i << " rows: " << source << source+1 << std::endl;
                            splits++; 
                            if( i+1 < workManifold[source+1].size() ) workManifold[source+1][i+1] = 3; 
                            if( i-1 >= 0) workManifold[source+1][i-1] = 3; 
                            break;
                        case 1:
                            break;
                    }
                    break;
            }
        }
        return splits;
    }
    int calculateSplits() {
        this->workManifold = this->safeManifold;
        int splits = 0;
        for (int i = 0; i < this->workManifold.size() - 1; i++) {
            splits += simulateRows(i);
        }
        return splits;
    }
    long long calculateTimelines(){
        std::pair<int, int> startCords = this->findSource(); //{y, x}
        std::vector<std::vector<long long>> storedSplits(this->safeManifold.size(), std::vector<long long>(this->safeManifold[0].size(), -1));

        std::function<long long(std::pair<int, int>)> myLambda = [&] (std::pair<int, int> p) -> long long{
            std::cout << "entering myLamda at " << p.first <<","<<p.second << std::endl;
            while(true){
                int state = intat(p); 
                if(state == -1) return 1; 
                if(storedSplits[p.first][p.second] != -1) return storedSplits[p.first][p.second];
                if(state == 2){
                    std::cout << "we're splitting here" << std::endl;
                    long long splitTimelines = myLambda({p.first+1, p.second+1}) + myLambda({p.first+1, p.second-1});
                    std::cout << "we're at " << splitTimelines << std::endl;
                    storedSplits[p.first][p.second] = splitTimelines; 
                    return splitTimelines;  
                }
                p.first++; 
            }
            return 0; 
        };
        return myLambda(startCords);
    }
    int intat(std::pair<int, int> cords){
        if(cords.first < 0 || cords.first >= this->safeManifold.size()) return -1;
        if(cords.second < 0 || cords.second >= this->safeManifold[cords.first].size()) return -1; 
        return this->safeManifold[cords.first][cords.second]; 
    }
    std::pair<int, int> findSource(){
        for(int i = 0; i < this->safeManifold.size(); i++){
            for(int j = 0; j < this->safeManifold[i].size(); j++){
                if(this->safeManifold[i][j] == 1) return {i, j}; 
            }
        }
        return {-1, -1};
    }
    
};
int main() {

    std::string path = "inputs/inputDay7.txt";
    std::fstream f(path, std::fstream::in);
    std::string input;
    ManifoldManager mm;
    while (std::getline(f, input)) {
        mm.insertRow(input);
    }
    int foundSplits = mm.calculateSplits();
    std::cout << "We found :" << foundSplits << std::endl;
    long long foundTimelines = mm.calculateTimelines(); 
    std::cout << "We found :" << foundTimelines << " timelines!" << std::endl;
    std::cout << "Hello from day 7!" << std::endl;
}
