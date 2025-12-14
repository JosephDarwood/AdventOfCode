#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Point{ 
    int x;
    int y;
    int z; 
};
struct Connection{
    int aIndex;
    int bIndex;
    double distance; 
};
struct Node{
    Node* parent;
    std::vector<Node*> children;
    int pointIndex;
};
double dbp(Point a, Point b){
    return std::sqrt(std::pow(a.x-b.x, 2) + std::pow(a.y-b.y, 2) + std::pow(a.z-b.z, 2));
}

Point stp(std::string input){
    std::string numholder = "";
    std::vector<int> nums;
    for(char c : input){
        if(c == ','){
            int temp = stoi(numholder);
            numholder = ""; 
            nums.push_back(temp);
        } else {
            numholder += c; 
        }
    }
    int temp = stoi(numholder);
    nums.push_back(temp);

    return {nums[0], nums[1], nums[2]};
}

class CircuitManager{
    public:
        std::vector<Point> points;
        std::vector<Node*> nodes; 
        std::vector<Connection> sortedConnects;
        void loadConnection(int connectionLimit = 1000){
            int conLim = (connectionLimit > 0) ? connectionLimit : (points.size()+1)*(points.size()+1)/2;
            std::cout << "Connection Limit is at " << conLim << " from " << connectionLimit << std::endl;
            this->sortedConnects.clear();
            this->sortedConnects.reserve(conLim+1);
            for(int i = 0; i < this->points.size(); i++){
                for(int j = i+1; j < this->points.size(); j++){
                    double distance = dbp(points[i], points[j]);
                    //std::cout << "Hi its cringe" << i << j << std::endl;
                    Connection connection = {i, j, distance};
                    this->insertConnection(connection);
                    if(this->sortedConnects.size() == connectionLimit+1) sortedConnects.pop_back();
                }
            }
        }
        void insertConnection(Connection connection){
            int left = 0; 
            int right = this->sortedConnects.size()-1;
            while(left < right){
                int middle = (left)+(right-left/2); 
                if(this->sortedConnects[middle].distance == connection.distance){ left = middle; right = middle;}
                if(this->sortedConnects[middle].distance < connection.distance) left = middle+1; 
                if(this->sortedConnects[middle].distance > connection.distance) right = middle-1; 
            }
            this->sortedConnects.insert(this->sortedConnects.begin()+left, connection);
            return;
        }
        void loadNodes(){
            this->nodes.clear();
            this->nodes.reserve(this->points.size());
            for(int i = 0; i< this->points.size(); i++){
                Node* newNode = new Node();
                newNode->parent = newNode; 
                newNode->pointIndex = i;
                this->nodes.push_back(newNode);
            }
            return;
        }
        long long getSize(){
            this->loadNodes();
            std::cout << "Now Setting Connectins" << std::endl;
            for(Connection c : this->sortedConnects){
                Node* na = this->find(c.aIndex);
                Node* nb = this->find(c.bIndex);
                na->parent = nb; 
                nb->children.push_back(na); 
            }
            std::cout << "Now getting sizes of root nodes" << std::endl;
            std::vector<long long> circuitSizes; 
            for(Node* n : this->nodes){
                std::cout << "Trying point node " << n->pointIndex << std::endl;
                if(n->parent != n) continue; 
                circuitSizes.push_back(count(n));
            }
            std::sort(circuitSizes.begin(), circuitSizes.end());
            int n = circuitSizes.size();
            return circuitSizes[n-1] * circuitSizes[n-2] * circuitSizes[n-3];
        }
        Node* find(int index){
            Node* start = this->nodes[index];
            while(start != start->parent) start = start->parent;
            return start;
        }
        long long count(Node* n){
            std::cout << "Try count " << n->pointIndex << std::endl;
            long long c = 1; 
            for(Node* child : n->children){
                if(child == n) continue;
                c+=count(child);
            }
            return c; 
        }
        int getLastConnection(){
            std::cout << "Now loading -1000 connections" << std::endl;
            this->loadConnection(-1000);
            std::cout << "Now loading nodes" << std::endl;
            this->loadNodes();
            std::cout << "Now loading reps" << std::endl; 
            std::unordered_set<int> reps; 
            for(int i = 0; i < this->nodes.size(); i++) reps.insert(i); 
            for(Connection c : this->sortedConnects){
                Node* na = this->find(c.aIndex);
                Node* nb = this->find(c.bIndex);
                if(na == nb) continue;
                na->parent = nb; 
                nb->children.push_back(na); 
                reps.erase(na->pointIndex);
                if(reps.size() == 1) return this->points[c.aIndex].x * this->points[c.bIndex].x; 
            }
            return -1; 
        }

};

int main(){

    std::cout << "Hello from day 8!" << std::endl;
    std::string path = "inputs/inputDay8.txt";
    std::fstream f(path, std::fstream::in);
    std::string input; 
    CircuitManager cm; 
    while(std::getline(f, input)){
        cm.points.push_back(stp(input));
    }
    std::cout << cm.points.size() << std::endl;
    cm.loadConnection();
    for(Connection c : cm.sortedConnects){
        std::cout << c.aIndex << "," << c.bIndex << ',' << c.distance << std::endl;
    }
    long long circSize = cm.getSize();
    std::cout << "Circ size " << circSize << std::endl;
    int lastConnect = cm.getLastConnection();
    std::cout << "Last connection X product " << lastConnect << std::endl;
} 
