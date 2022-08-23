#include "AStartPathFinding.hpp"
#include <list>

void AStarPathFinding(Node* start, Node* end, std::vector<Node*>& map_ref) {
    // reset all cells
    for (int i = 0; i < map_ref.size(); i++) {
        map_ref[i]->parent = nullptr;
        map_ref[i]->GlobalDistance = INFINITY;
        map_ref[i]->LocalDistance = INFINITY;
        map_ref[i]->visited = false;
    }
    auto distance = [](Node* a, Node* b) // For convenience
    {
        return sqrtf((a->position.x - b->position.x) * (a->position.x - b->position.x) + (a->position.y - b->position.y) * (a->position.y - b->position.y));
    };

    auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
    {
        return distance(a, b);
    };

    // begin starting from the gosts position
    Node* CurrentNode = start;
    CurrentNode->LocalDistance = 0;
    CurrentNode->GlobalDistance = heuristic(start, end);
    // create a container where we will keep all upchecked cells in case of finding a better path
    std::list<Node*> ToCheckList;
    ToCheckList.push_back(CurrentNode);

    while (!ToCheckList.empty() && CurrentNode != end) {
        // sort the list by global dist (ascending)
        ToCheckList.sort([](const Node* a, const Node* b) {return a->GlobalDistance < b->GlobalDistance; });
        // pop out if  visited 
        while (!ToCheckList.empty() && ToCheckList.front()->visited) {
            ToCheckList.pop_front();
        }
        // empty->break
        if (ToCheckList.empty()) {
            break;
        }

        // reset the current node
        CurrentNode = ToCheckList.front();
        // set it as visited to check only once
        CurrentNode->visited = true;

        // check each neighbour of the node
        for (auto neighbour : CurrentNode->Neighbours) {
            if (!neighbour->visited && !neighbour->wall) {
                ToCheckList.push_back(neighbour);
            }
            // calculate new LocalDist
            float PotentialDist = CurrentNode->LocalDistance + distance(CurrentNode, neighbour);
            if (PotentialDist < neighbour->LocalDistance) {
                neighbour->LocalDistance = PotentialDist;
                neighbour->parent = CurrentNode;
                neighbour->GlobalDistance = neighbour->LocalDistance + heuristic(neighbour, end);
            }
        }


    }

}