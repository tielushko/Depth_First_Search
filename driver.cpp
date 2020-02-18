#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <limits>

using namespace std;

using Graph = std::map<string, vector<string>>; 
using Location = std::map<string, tuple<int, int, bool>>;

Graph graph;
Location locationMap;

void DFS(string start, string finish);

int main() {
    
    //initializing variables
    ifstream in;
    string cityName;
    int xC, yC, numFollows;
    string connectionName;
    
    in.open("locations.txt");

    //file error check 
    if(!in.is_open()) 
        cerr << "couldn't open the file"; 
    
    //extraction from locations.txt
    while(in >> cityName >> xC >> yC) {
        locationMap.insert({cityName, make_tuple(xC, yC, false)}); 
    }
    
    //;)
    in.close();
     
    //extraction from connections.txt w/error check
    in.open("connections.txt");
    if(!in.is_open()) 
        cerr << "couldn't open the file"; 
    
    while (in >> cityName >> numFollows) {
        vector<string> myVec;
        for (int i = 0; i < numFollows; i++) {
            in >> connectionName;
            myVec.push_back(connectionName); 
        }
        graph.insert(pair<string, vector<string>>(cityName, myVec)); 
    }

    //;)
    in.close();

    //getting input from user
    string startLocation, endLocation;
    string path;
    cout << "Please enter your starting location: " << endl;
    cin >> startLocation;
    
    //error check
    while(locationMap.count(startLocation) == 0) {
        cout << "The starting location does not exist! Please, enter the correct start!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> startLocation;
    }

    cout << "Please enter your ending location: " << endl;
    cin >> endLocation;

    //error check
    while(locationMap.count(endLocation) == 0) {
        cout << "The ending location does not exist! Please, enter the correct end!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> endLocation;
    }

    //run DFS
    DFS(startLocation, endLocation); 

    return 0;    
}

void DFS(string start, string finish) {
    //initializing variables
    deque<string> pathDeque; 
    double totalDistance = 0.0; 
    
    pathDeque.push_back(start); 
    
    get<2>(locationMap[pathDeque.back()]) = true;

    //if the finish is the start...
    if (start == finish) {
        cout << pathDeque.back() << " to " << pathDeque.back() << " distance 0\n";
    }

    //constructing the deque with the path
    while (pathDeque.back() != finish) {
        string minimum = "zzzzzzzzZZZZZZZZZZZZZZZ";
        bool allVisited = true;
        
        //traverse the location's neighbours to push the minimum into the deque
        for (string i: graph[pathDeque.back()]) {
            for(string j:graph[pathDeque.back()])
                if(get<2>(locationMap[j])==true){}
                else{allVisited = false;}
                
            if (i < minimum && get<2>(locationMap[i]) == false) {
                minimum = i; 
            } 
        }
        if(allVisited == true){
            pathDeque.pop_back();
            continue;
        }else{
            pathDeque.push_back(minimum);
            get<2>(locationMap[minimum]) = true;
        }
    }

    //loop responsible for output of the path and proper format
    while (pathDeque.size() > 1) {
        int xC = get<0>(locationMap[pathDeque.front()]), yC = get<1>(locationMap[pathDeque.front()]);
        cout << pathDeque.front();
        pathDeque.pop_front(); 
        double distance = sqrt(pow((get<0>(locationMap[pathDeque.front()]) - xC), 2) + pow((get<1>(locationMap[pathDeque.front()]) - yC), 2));
        cout << " to " << pathDeque.front() << " distance " << distance << endl;
        totalDistance += distance;
    }

    //clean up ;)
    pathDeque.pop_front();

    //come on now, really?
    if(totalDistance == 0)
        cout << "I'm already here?" << endl;
    
    //output the total distance
    cout << "Total Distance: " << totalDistance;
}