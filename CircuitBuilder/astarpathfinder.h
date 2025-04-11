#ifndef A_STAR_H
#define A_STAR_H

#include <QPoint>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

struct Node {
    QPoint pos;
    int g = 0;
    int h = 0;
    int f() const { return g + h; }

    Node* parent = nullptr;
};

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->f() > b->f(); // Min-heap
    }
};

class AStarPathfinder {
public:
    AStarPathfinder(const std::vector<std::vector<bool>>& grid);

    std::vector<QPoint> findPath(const QPoint& start, const QPoint& goal);

private:
    std::vector<QPoint> reconstructPath(Node* endNode);
    int heuristic(const QPoint& a, const QPoint& b);
    bool isValid(const QPoint& p);

    const std::vector<std::vector<bool>>& grid;
    int rows, cols;
};

#endif // A_STAR_H
