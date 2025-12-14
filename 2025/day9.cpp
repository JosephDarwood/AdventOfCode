#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

std::pair<int, int> stp(std::string input){
    int comma = input.find(','); 
    return {std::stoi(input.substr(0, comma)), std::stoi(input.substr(comma+1, input.size()-1-comma))};
}

long long ptr(std::pair<int, int> a, std::pair<int, int> b){
    long long l = abs(a.first - b.first)+1;
    long long w = abs(a.second - b.second)+1;
    return l*w;
}

struct Line{
    int axis; 
    std::pair<int, int> range; 
};

class RectangleComputer{
public:
    std::vector<std::pair<int, int>> refrencePoints;
    std::vector<Line> firstEqRanges; 
    std::vector<Line> secondEqRanges;
    long long largestRectangle(){
        long long maxRect = 0;
        for(int i = 0; i<this->refrencePoints.size(); i++){
            for(int j = i+1; j<this->refrencePoints.size(); j++){
                long long curRect = ptr(this->refrencePoints[i], this->refrencePoints[j]);
                if(curRect > maxRect) maxRect = curRect;
            }
        }
        return maxRect;
    }
    void refrenceInsert(std::pair<int, int> input){
        std::pair<int, int>& back = (this->refrencePoints.size() > 0 ) ? this->refrencePoints.back() : input;
        if(input.first != back.first && input.second != back.second) throw std::runtime_error("Bad insert, the input and the back aren't connected\n");
        markGreens(back, input);
        this->refrencePoints.push_back(input);
    }
    void markGreens(std::pair<int, int> a, std::pair<int, int> b){
        if(a.first == b.first){
            int big = (a.second > b.second) ? a.second : b.second;
            int small = (a.second > b.second) ? b.second : a.second; 
            this->firstEqRanges.push_back({a.first, {small, big}});
        } else if (a.second == b.second) {
            int big = (a.first > b.first) ? a.first : b.first;
            int small = (a.first > b.first) ? b.first : a.first;
            this->secondEqRanges.push_back({b.second, {small, big}});
        } else {
            throw std::runtime_error("A and B not compatible for markGreens()"); 
        }
    }
    void finalizeList(){
        std::pair<int, int>& a = this->refrencePoints.front();
        std::pair<int, int>& b = this->refrencePoints.back();
        markGreens(a, b);
    }
    long long largestBoundedRectangle(){
        long long maxRect = 0; 
        for(int i = 0; i<this->refrencePoints.size(); i++){
            for(int j = i+1; j < this->refrencePoints.size(); j++){
                const std::pair<int, int> ipair = this->refrencePoints[i]; 
                const std::pair<int, int> jpair = this->refrencePoints[j];
                if(validateRectangle(ipair, jpair)){
                    long long curRect = ptr(ipair, jpair);
                    maxRect = std::max(curRect, maxRect);
                } 
            }
        }
        return maxRect;
    }
    bool validateRectangle(const std::pair<int, int>& p1, const std::pair<int, int>& p2){
        int hf = std::max(p1.first, p2.first); 
        int lf = std::min(p1.first, p2.first);
        int hs = std::max(p1.second, p2.second);
        int ls = std::min(p1.second, p2.second);
        Line highFirst = {hf,{ls, hs}}; 
        Line lowFirst = {lf,{ls, hs}}; 
        Line highSecond = {hs,{lf, hf}};
        Line lowSecond = {ls,{lf, hf}};
        //GENERAL LOGIC
        //check crossing lines
        //check interior lines
        for(const Line& storedLine : this->firstEqRanges){
           if(lineCross(storedLine, highSecond)) return false;
           if(lineCross(storedLine, lowSecond)) return false;
           if(betweenLines(storedLine, lowFirst, highFirst)) return false; 
        
        }
        for(const Line& storedLine : this->secondEqRanges){
            if(lineCross(storedLine, highFirst)) return false;
            if(lineCross(storedLine, lowFirst)) return false;
            if(betweenLines(storedLine, lowSecond, highSecond)) return false;
        }
        return true;
    }
    bool lineCross(Line a, Line b){
        return (a.axis > b.range.first && a.axis < b.range.second) && (b.axis > a.range.first && b.axis < a.range.second);
    }
    bool betweenLines(Line check, Line low, Line high){
        if(check.axis > low.axis && check.axis < high.axis){
            if(check.range.first > low.range.first &&  check.range.first < low.range.second) return true; 
            if(check.range.second > low.range.first && check.range.second < low.range.second) return true; 
        }
        return false;
    }

};



int main(){
    std::string path = "inputs/inputDay9.txt"; 
    std::fstream f(path, std::fstream::in);
    std::string input; 
    RectangleComputer rc;
    while(getline(f, input)){
        rc.refrenceInsert(stp(input)); 
    }
    rc.finalizeList();
    std::cout << "Hello from day 9!" << std::endl;
    std::cout << "Refrence Points collected" << rc.refrencePoints.size() << std::endl;
    std::cout << "Size of largest rectangle" << rc.largestRectangle() << std::endl;
    std::cout << "Interior Rectangle " << rc.largestBoundedRectangle() << std::endl;
}
