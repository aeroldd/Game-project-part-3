#include "pathfinding.h"
#include "entity.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Define a node structure for BFS.
typedef struct BFSNode {
    Position pos; // Grid position
    struct BFSNode *prev;
} BFSNode;

// Helper: check if a position is valid for traversal.
// target is allowed even if occupied.
static bool isValid(RoomGrid *room, Position pos, Position target) {
    // Check bounds.
    if (pos.x < 0 || pos.x >= room->width || pos.y < 0 || pos.y >= room->height)
        return false;
    // Do not traverse walls.
    if (room->tiles[pos.y][pos.x]->symbol == '#')
        return false;
    // If there's an entity at this position (other than the target), do not use it.
    Entity *ent = getEntityAtGridPosition(room, pos);
    if (ent && !(pos.x == target.x && pos.y == target.y))
        return false;
    return true;
}

Position* findShortestPath(RoomGrid *room, Position start, Position target, int *pathLength) {
    // If already at the target, return an empty path.
    if (start.x == target.x && start.y == target.y) {
        *pathLength = 0;
        return NULL;
    }
    
    bool visited[room->height][room->width];
    memset(visited, 0, sizeof(visited));

    int queueCapacity = room->width * room->height;
    BFSNode **queue = malloc(queueCapacity * sizeof(BFSNode *));
    int front = 0, rear = 0;

    BFSNode *startNode = malloc(sizeof(BFSNode));
    startNode->pos = start;
    startNode->prev = NULL;
    queue[rear++] = startNode;
    visited[start.y][start.x] = true;

    BFSNode *targetNode = NULL;
    Position directions[4] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

    while (front < rear) {
        BFSNode *current = queue[front++];
        
        if (current->pos.x == target.x && current->pos.y == target.y) {
            targetNode = current;
            break;
        }

        for (int i = 0; i < 4; i++) {
            Position next = { current->pos.x + directions[i].x,
                              current->pos.y + directions[i].y };
            
            if (next.x < 0 || next.x >= room->width || next.y < 0 || next.y >= room->height)
                continue;
            if (visited[next.y][next.x])
                continue;
            if (!isValid(room, next, target))
                continue;
            
            visited[next.y][next.x] = true;

            BFSNode *neighbor = malloc(sizeof(BFSNode));
            neighbor->pos = next;
            neighbor->prev = current;
            queue[rear++] = neighbor;
        }
    }

    // If no path found, return NULL.
    if (!targetNode) {
        *pathLength = 0;
        free(queue);
        return NULL;
    }

    // Backtrack to build the path.
    int pathSize = 0;
    BFSNode *current = targetNode;
    while (current) {
        pathSize++;
        current = current->prev;
    }

    Position *path = malloc(pathSize * sizeof(Position));
    current = targetNode;
    for (int i = pathSize - 1; i >= 0; i--) {
        path[i] = current->pos;
        current = current->prev;
    }

    *pathLength = pathSize;

    // Free allocated nodes in queue.
    for (int i = 0; i < rear; i++) {
        free(queue[i]);
    }
    free(queue);

    return path;
}