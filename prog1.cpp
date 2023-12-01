#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <queue>


struct node {
    int numNode;
    struct node* nextNode;
};

void ClearLine(struct node* list, int vertexes) {
    if (!list) return;
    if (list->nextNode)
        ClearLine(list->nextNode, vertexes);
    free(list);
}

void ClearList(struct node* list, int vertexes) {
    if (!list || !vertexes) return;
    if (vertexes > 1)
        for (int i = 0; i != vertexes; i++) {
            ClearLine((list + i)->nextNode, vertexes);
        }
    free(list);
}


int* CreateGraph(int vertexes) {
    int* Mtrx = (int*)malloc(sizeof(int) * vertexes * vertexes);
    if (!Mtrx) return 0;
    for (int i = 0; i != vertexes; i++) {
        *(Mtrx + i * vertexes + i) = 0;
        for (int j = 0; j < i; j++) {
            *(Mtrx + i * vertexes + j) = rand() % 2;
            *(Mtrx + j * vertexes + i) = *(Mtrx + i * vertexes + j);
        }
    }
    return Mtrx;
}

int ModifyGraph(struct node* list, int* graph, int vertexes) {
    if (!list || !graph) return 0;
    for (int i = 0; i != vertexes; i++) {
        struct node* current = list + i;
        current->numNode = i + 1;
        for (int j = 0; j != vertexes; j++) {
            if (*(graph + i * vertexes + j)) {
                current->nextNode = (struct node*)malloc(sizeof(struct node));
                current = current->nextNode;
                current->numNode = j + 1;
            }
        }
        current->nextNode = NULL;
    }
    return 1;
}

int breadthFirstSearchAdjacencyMatrix(int* matrix, int startNode, int numNodes, int endNode) {
    if (numNodes <= 0 || (startNode + 1) == endNode) return 0;
    int* visited = (int*)malloc(sizeof(int) * numNodes), count = 0;

    if (!visited) return 0;
    std::queue<int>queue;
    for (int i = 0; i < numNodes; i++) {
        visited[i] = 0;
    }
    visited[startNode] = 1;
    queue.push(startNode);

    while (!queue.empty()) {
        int currentNode = queue.front();

        queue.pop();
        count++;
        if (*(matrix + currentNode * numNodes + endNode - 1)) {
            free(visited);
            return count;
        }

        for (int i = 0; i < numNodes; i++) {
            if (*(matrix + currentNode * numNodes + i) && !visited[i]) {
                visited[i] = 1;
                queue.push(i);
            }
        }
    }
    free(visited);
    return 0;
}

int breadthFirstSearchAdjacencyList(struct node* adjacencyList, int startNode, int numNodes, int endNode) {
    if (numNodes <= 0 || (startNode + 1) == endNode) return 0;
    int* visited = (int*)malloc(sizeof(int) * numNodes), count = 0;

    if (!visited) return 0;
    std::queue<int>queue;
    for (int i = 0; i != numNodes; i++) {
        visited[i] = 0;
    }
    visited[startNode] = 1;
    queue.push(startNode);

    while (!queue.empty()) {
        int currentNode = queue.front();
        struct node* current = adjacencyList + currentNode;
        //printf("%d ", currentNode + 1);

        queue.pop();
        count++;
        do {
            if ((current)->numNode == endNode) {
                free(visited);
                return count;
            }
            if (!visited[(current)->numNode - 1]) {
                visited[(current)->numNode - 1] = 1;
                queue.push((current)->numNode - 1);
            }
            current = current->nextNode;
        } while (current);
    }
    free(visited);
    return 0;
}

void ShowGraph(int* graph, int vertexes) {
    if (!graph) return;
    for (int i = 0; i != vertexes; i++) {
        for (int j = 0; j != vertexes; j++) {
            printf("%i ", *(graph + i * vertexes + j));
        }
        printf("|%i", i + 1);
        printf("\n");
    }
}

void ShowList(struct node* list, int vertexes) {
    if (!list) return;
    printf("\n");
    for (int i = 0; i != vertexes; i++) {
        struct node* current = list + i;
        while (current) {
            printf("|%i|\t->", current->numNode);
            current = current->nextNode;
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "RUS");
    int vertexNum = 0, vertexEnd = 0;
    printf("Кол-во вершин: ");
    if (!scanf("%d", &vertexNum) || vertexNum <= 0) return 1;
    vertexNum = vertexNum > 99 ? 99 : vertexNum;
    int* adjacencyMatrix = CreateGraph(vertexNum);
    if (!adjacencyMatrix) return 1;
    struct node* adjacencyList = (struct node*)malloc(sizeof(struct node) * vertexNum);
    if (!ModifyGraph(adjacencyList, adjacencyMatrix, vertexNum)) return 1;
    system("cls");
    ShowGraph(adjacencyMatrix, vertexNum);
    ShowList(adjacencyList, vertexNum);
    printf("Поиск расcтояний к вершине: ");
    if (!scanf("%d", &vertexEnd) || vertexEnd > vertexNum || vertexEnd <= 0) {
        printf("Нет такой вершины");
        return 1;
    }
    printf("\tМатрица смежности:\tСписок смежности\n");
    for (int i = 0; i != vertexNum; i++) {
        printf("%d->%d|\t%d\t\t\t%d\n", i + 1, vertexEnd, breadthFirstSearchAdjacencyMatrix(adjacencyMatrix, i, vertexNum, vertexEnd), breadthFirstSearchAdjacencyList(adjacencyList, i, vertexNum, vertexEnd));
    }
    free(adjacencyMatrix);
    ClearList(adjacencyList, vertexNum);
    return 0;
}