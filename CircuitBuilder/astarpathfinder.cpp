#include "astarpathfinder.h"

AStarPathfinder::AStarPathfinder(const std::vector<std::vector<bool>>& grid)
    : grid(grid), rows(grid.size()), cols(grid[0].size()) {}

std::vector<QPoint> AStarPathfinder::findPath(const QPoint& start, const QPoint& goal) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> openSet;
    std::unordered_map<int, Node*> allNodes;

    auto toKey = [&](const QPoint& p) {
        return p.y() * cols + p.x();
    };

    Node* startNode = new Node{start};
    startNode->h = heuristic(start, goal);

    openSet.push(startNode);
    allNodes[toKey(start)] = startNode;

    std::unordered_set<int> closedSet;

    const std::vector<QPoint> directions = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0} // 4 directions (no diagonals)
    };

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->pos == goal) {
            std::vector<QPoint> path = reconstructPath(current);

            for (auto& [_, node] : allNodes) delete node; // Clean up
            return path;
        }

        closedSet.insert(toKey(current->pos));

        for (const QPoint& dir : directions) {
            QPoint neighborPos = current->pos + dir;

            if (!isValid(neighborPos) || closedSet.count(toKey(neighborPos)))
                continue;

            int tentativeG = current->g + 1;

            Node*& neighborNode = allNodes[toKey(neighborPos)];
            if (!neighborNode) {
                neighborNode = new Node{neighborPos};
                neighborNode->g = tentativeG;
                neighborNode->h = heuristic(neighborPos, goal);
                neighborNode->parent = current;
                openSet.push(neighborNode);
            } else if (tentativeG < neighborNode->g) {
                neighborNode->g = tentativeG;
                neighborNode->parent = current;
                // Re-inserting it will not reorder the heap, but for a small grid this is acceptable
            }
        }
    }

    // Cleanup if no path found
    for (auto& [_, node] : allNodes) delete node;
    return {};
}

std::vector<QPoint> AStarPathfinder::reconstructPath(Node* endNode) {
    std::vector<QPoint> path;
    while (endNode) {
        path.push_back(endNode->pos);
        endNode = endNode->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int AStarPathfinder::heuristic(const QPoint& a, const QPoint& b) {
    return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y()); // Manhattan
}

bool AStarPathfinder::isValid(const QPoint& p) {
    return p.x() >= 0 && p.y() >= 0 && p.y() < rows && p.x() < cols && grid[p.y()][p.x()];
}
