//============================================================================
// Name        : findpath.cpp
// Author      : Andrey Sudnik
// Version     : 0.1
// Copyright   :
// Description : A* path finding algorithm
//============================================================================

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <stdlib.h>

using namespace std;

typedef unsigned int uint;


class Coordinate {
  public:
    int x,y;
    Coordinate() {}
    Coordinate(int _x,int _y) { x=_x;y=_y;};
    bool operator==(const Coordinate &other) const {
        if (x==other.x && y==other.y) return true;
        return false;
    }
    bool operator!=(const Coordinate &other) const {
        if (x!=other.x || y!=other.y) return true;
        return false;
       }


};

class Node{
private:
    Coordinate _neighbors[8];

public:
    Coordinate position;

    Node *parent;
    int costFromStart;
    int costToEnd;
    int type;
    Node(int x,int y,int t) {
        position.x=x;
        position.y=y;
        type=t;
    }
    Node() {
            position.x=-1;
            position.y=-1;
            type=-1;
        }
    int fullCost(){ return costFromStart+costToEnd; }

    Coordinate* neighbors() {
        _neighbors[0]= Coordinate(position.x+1,position.y);
        _neighbors[1]= Coordinate(position.x+1,position.y+1);
        _neighbors[2]= Coordinate(position.x,position.y+1);
        _neighbors[3]= Coordinate(position.x-1,position.y+1);
        _neighbors[4]= Coordinate(position.x-1,position.y);
        _neighbors[5]= Coordinate(position.x-1,position.y-1);
        _neighbors[6]= Coordinate(position.x,position.y-1);
        _neighbors[7]= Coordinate(position.x+1,position.y-1);
        return _neighbors;
    }
};

bool compare_cost (Node* first, Node* second) {
    if (first->fullCost() > second->fullCost()) return false;
    return true;
}

int heuristic_cost(Node* first,Node* second){
    return (abs(first->position.x - second->position.x) + abs(first->position.y - second->position.y))*10;
}


bool findpath(Node* startNode,Node* endNode,vector< vector<Node*> > nodeMap){
    list<Node*> closedList;
    list<Node*> openList;
    startNode->parent=NULL;
    startNode->costFromStart=0;
    startNode->costToEnd=heuristic_cost(startNode,endNode);
    openList.push_back(startNode);
    bool found = false;
    while (openList.size()>0 && !found) {
        openList.sort(compare_cost);
        //cout<<"Open List Costs: ";
        //for (list<Node*>::iterator it = openList.begin(); it != openList.end(); it++){
            //cout<<(*it)->fullCost()<<" ";
        //}
        //cout<<"\nCurrent Node Cost:"<<currentNode->fullCost()<<endl;
        //cout<<"Current Node:"<<currentNode->position.x<<" "<<currentNode->position.y<<endl;
        Node* currentNode=openList.front();
        openList.pop_front();
        closedList.push_back(currentNode);
        for (int i=0;i<8;++i){
            int nx=(currentNode->neighbors())[i].x;
            int ny=(currentNode->neighbors())[i].y;
            if (nx>=0 && ny>=0 && (uint)nx<nodeMap[ny].size() && (uint)ny<nodeMap.size() && nodeMap[ny][nx]->type!=1 ){ //if in map and walkable than it is neighbor
                bool in_closed=false;
                for (list<Node*>::iterator it = closedList.begin(); it != closedList.end() && !in_closed; it++){ //found neighbor node in closed list
                    if ( (*it)->position == nodeMap[ny][nx]->position ) in_closed=true;
                }
                if (!in_closed){ //if neighbor node not in closed list do all shit
                    bool in_open=false;
                    uint Len;
                    if (i%2!=0) Len=15; //walk diagonal
                    else Len=10; // walk straight
                    int tentativeCost=currentNode->costFromStart+Len;
                    bool tentativeCostBetter=true;
                    for (list<Node*>::iterator it = openList.begin(); it != openList.end() && !in_open; it++){ //found neighbor node in open list
                        if ( (*it)->position == nodeMap[ny][nx]->position ) in_open=true;
                    }
                    if (!in_open){
                        openList.push_back(nodeMap[ny][nx]);
                        tentativeCostBetter=true;
                    } else if (tentativeCost < nodeMap[ny][nx]->costFromStart) {
                        tentativeCostBetter=true;
                    } else
                        tentativeCostBetter=false;
                    if (tentativeCostBetter) nodeMap[ny][nx]->parent=currentNode;
                    nodeMap[ny][nx]->costFromStart=tentativeCost;
                    nodeMap[ny][nx]->costToEnd=heuristic_cost(nodeMap[ny][nx],endNode);
                    if (nodeMap[ny][nx]->position==endNode->position)
                    	found=true;
                }
            }
        }
    }
    return found;
}

int main() {
 const uint MAP_SIZE=10;
 string map[MAP_SIZE]={ "           s         *     ",
                        "     ********** *******     ",
                        "**  **             *    *   ",
                        "        *    *         *   ",
                        "***** *****  ************* ",
                        "      *   *  *             ",
                        "  ***** * ****   *   *********",
                        "        *        ***          ",
                        "******* ******** *x********   ",
                        "                 *            "
    };

      vector< vector<Node*> > nodeMap;
      Node *startNode=NULL;
      Node *endNode=NULL;
      for (uint i=0;i<MAP_SIZE;++i){
         vector<Node*> row;
         for (uint j=0;j<map[i].size();++j){
             Node *node=new Node(j,i,0);
             row.push_back(node);
             if (map[i].at(j)=='*') {
                 node->type=1;
             } else if (map[i].at(j)=='s'){
                 startNode=node;
             } else if (map[i].at(j)=='x'){
                 endNode=node;
             }
         }
         nodeMap.push_back(row);
      }

    cout<<"Start: "<<startNode->position.x<<" "<<startNode->position.y<<endl;
    cout<<"End: "<<endNode->position.x<<" "<<endNode->position.y<<endl;

    if (findpath(startNode,endNode,nodeMap)){
        Node* parent=nodeMap[endNode->position.y][endNode->position.x];
        while (parent->parent!=NULL){
            map[parent->position.y][parent->position.x]='@';
            parent=parent->parent;
        }
    }
    for (uint i=0;i<MAP_SIZE;++i)
        cout<<"|"<<map[i]<<"|"<<endl;
}
