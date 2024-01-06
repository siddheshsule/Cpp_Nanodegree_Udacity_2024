#include "route_planner.h"
#include <algorithm>

// Constructor for RoutePlanner class
RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Finding closest nodes to start and end coordinates in the model
    start_node = &model.FindClosestNode(start_x, start_y);
    end_node = &model.FindClosestNode(end_x, end_y);
}

// Calculate heuristic value (H) for a given node
float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {    
    return node->distance(*end_node);    
}

// Add neighbors of the current node to explore
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for(auto neighbor : current_node->neighbors) {
        // Update properties of neighbors (parent, h_value, g_value)
        neighbor->parent = current_node;
        neighbor->h_value = CalculateHValue(neighbor);
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        open_list.emplace_back(neighbor);
        neighbor->visited = true;        
    }
}

// Select the next node for exploration based on heuristic and path cost values
RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(), open_list.end(), [](const RouteModel::Node *a, const RouteModel::Node *b){return (a->h_value + a->g_value) > (b->h_value + b->g_value);});
    RouteModel::Node *lowest = open_list.back();
    open_list.pop_back();
    return lowest;
}

// Construct the final path from start to end node
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while(current_node->parent) {
        auto gValue = current_node->g_value;
        path_found.push_back(*current_node);
        distance += (*current_node).distance(*current_node->parent);
        current_node = current_node->parent;
        current_node->g_value += gValue;
    }
    path_found.push_back(*current_node);
    std::reverse(path_found.begin(), path_found.end());
    distance *= m_Model.MetricScale(); // Convert distance to meters based on map scale
    return path_found;
}

// Perform the A* search algorithm
void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    start_node->visited = true;
    current_node = start_node;
    open_list.push_back(start_node);

    while(open_list.size() > 0) {
        // Check if the current node is the end node
        if (current_node == end_node) {
            // Construct the final path if the end node is reached
            m_Model.path = ConstructFinalPath(current_node);
            return;
        }
        // Explore neighbors and update the current node
        AddNeighbors(current_node);
        current_node = NextNode();
    }
    return;
}
