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
                if (!current->nextNode) return 0;
                current = current->nextNode;
                current->numNode = j + 1;
            }
        }
        current->nextNode = NULL;
    }
    return 1;
}

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

int DepthFirstSearch1(int* graph, int vertexes, int startNode, int endNode) {
    if (!graph || startNode == (endNode - 1) || !vertexes) return 0;

    int* visited = (int*)malloc(sizeof(int) * vertexes), * queue = (int*)malloc(sizeof(int) * vertexes * 2), temp = 0, result = INT_MAX, iQueue = -1, currentNode = startNode, cond = 0;
    if (!visited || !queue) return 0;

    for (int i = 0; i != vertexes; i++) {
        visited[i] = 0;
    }

    do {
        visited[currentNode] = 1;
        temp++;
        for (int i = 0; i != vertexes; i++) {
            if (*(graph + vertexes * currentNode + i) && !visited[i]) {
                cond = 1;
                visited[i] = 1;
                if (i == (endNode - 1)) {
                    result = temp < result ? temp : result;
                    temp--;
                    break;
                }
                queue[++iQueue] = i;
            }
        }
        if (!cond && currentNode == startNode)
            return 0;
        currentNode = queue[iQueue];
        queue[iQueue--] = 0;
    } while (iQueue >= 0);

    free(visited);
    return result;
}
int DepthFirstSearch2(struct node* graph, int vertexes, int startNode, int endNode) {
    if (!graph || startNode == (endNode - 1) || !vertexes) return 0;

    int* visited = (int*)malloc(sizeof(int) * vertexes), * queue = (int*)malloc(sizeof(int) * vertexes * 2), temp = 0, result = INT_MAX, iQueue = -1, iList = -1, currentNode = startNode, cond = 0;
    struct node* node = graph;
    if (!visited || !queue) return 0;

    for (int i = 0; i != vertexes; i++) {
        visited[i] = 0;
    }
    
    do {
        visited[currentNode] = 1;
        node = &graph[currentNode];
        temp++;
        while (node->nextNode) {
            cond = 1;
            node = node->nextNode;
            if (!visited[node->numNode - 1]) {
                visited[node->numNode - 1] = 1;
                if (node->numNode == endNode) {
                    result = temp < result ? temp : result;
                    temp--;
                    break;
                }
                queue[++iQueue] = node->numNode - 1;
            }
        }
        if (!cond && currentNode == startNode)
            return 0;
        currentNode = queue[iQueue];
        queue[iQueue--] = 0;
    } while (iQueue >= 0);

    free(visited);
    return result;
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
        printf("%d->%d|\t%d\t\t\t%d\n", i + 1, vertexEnd, DepthFirstSearch1(adjacencyMatrix, vertexNum, i, vertexEnd), DepthFirstSearch2(adjacencyList, vertexNum, i, vertexEnd));
    }
    free(adjacencyMatrix);
    ClearList(adjacencyList, vertexNum);
    return 0;
}
