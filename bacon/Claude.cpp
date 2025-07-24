#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// 연결 리스트 노드 구조체
struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

// 연결 리스트 클래스
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    // 노드 추가
    void add(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    // 특정 값이 존재하는지 확인
    bool contains(int value) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // 리스트 크기 반환
    int size() {
        int count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    // 리스트 출력
    void print() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
    }

    // 메모리 해제
    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    Node* getHead() { return head; }
};

// 큐 구조체 (BFS용)
class Queue {
private:
    struct QueueNode {
        int data;
        QueueNode* next;
        QueueNode(int value) : data(value), next(nullptr) {}
    };

    QueueNode* front;
    QueueNode* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(int value) {
        QueueNode* newNode = new QueueNode(value);
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    int dequeue() {
        if (front == nullptr) return -1;

        int value = front->data;
        QueueNode* temp = front;
        front = front->next;

        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;
        return value;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

// 동적 배열 클래스 (간단 버전)
class DynamicArray {
private:
    int* data;
    int capacity;
    int count;

public:
    DynamicArray() : capacity(10), count(0) {
        data = new int[capacity];
    }

    void add(int value) {
        if (count >= capacity) {
            capacity *= 2;
            int* newData = new int[capacity];
            for (int i = 0; i < count; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[count++] = value;
    }

    void removeLast() {
        if (count > 0) {
            count--;
        }
    }

    int get(int index) {
        if (index >= 0 && index < count) {
            return data[index];
        }
        return -1;
    }

    int size() { return count; }

    void clear() {
        count = 0;
    }

    ~DynamicArray() {
        delete[] data;
    }
};

// 그래프 클래스
class Graph {
private:
    LinkedList* adjList;  // 인접 리스트
    int numVertices;      // 정점 수

public:
    Graph(int vertices) : numVertices(vertices) {
        adjList = new LinkedList[vertices + 1];  // 1-based indexing
    }

    // 간선 추가 (양방향)
    void addEdge(int u, int v) {
        adjList[u].add(v);
        adjList[v].add(u);
    }

    // BFS로 최단 거리 계산
    int findShortestDistance(int start, int end) {
        bool* visited = new bool[numVertices + 1];
        int* distance = new int[numVertices + 1];

        // 초기화
        for (int i = 0; i <= numVertices; i++) {
            visited[i] = false;
            distance[i] = -1;
        }

        Queue q;
        visited[start] = true;
        distance[start] = 0;
        q.enqueue(start);

        while (!q.isEmpty()) {
            int current = q.dequeue();

            if (current == end) {
                int result = distance[end];
                delete[] visited;
                delete[] distance;
                return result;
            }

            Node* neighbor = adjList[current].getHead();
            while (neighbor != nullptr) {
                int next = neighbor->data;
                if (!visited[next]) {
                    visited[next] = true;
                    distance[next] = distance[current] + 1;
                    q.enqueue(next);
                }
                neighbor = neighbor->next;
            }
        }

        delete[] visited;
        delete[] distance;
        return -1;  // 연결되지 않음
    }

    // 특정 노드에서 3시간 이내 도달 가능한 노드 수 계산
    int countReachableInTime(int start, int maxTime) {
        bool* visited = new bool[numVertices + 1];
        int* distance = new int[numVertices + 1];

        for (int i = 0; i <= numVertices; i++) {
            visited[i] = false;
            distance[i] = -1;
        }

        Queue q;
        visited[start] = true;
        distance[start] = 0;
        q.enqueue(start);

        int reachableCount = 1;  // 자기 자신 포함

        while (!q.isEmpty()) {
            int current = q.dequeue();

            Node* neighbor = adjList[current].getHead();
            while (neighbor != nullptr) {
                int next = neighbor->data;
                if (!visited[next]) {
                    visited[next] = true;
                    distance[next] = distance[current] + 1;
                    if (distance[next] <= maxTime) {
                        reachableCount++;
                        q.enqueue(next);
                    }
                }
                neighbor = neighbor->next;
            }
        }

        delete[] visited;
        delete[] distance;
        return reachableCount;
    }

    // 연결된 컴포넌트 찾기 (DFS)
    void dfs(int vertex, bool* visited, LinkedList& component) {
        visited[vertex] = true;
        component.add(vertex);

        Node* neighbor = adjList[vertex].getHead();
        while (neighbor != nullptr) {
            int next = neighbor->data;
            if (!visited[next]) {
                dfs(next, visited, component);
            }
            neighbor = neighbor->next;
        }
    }

    // Lone wolf 찾기 (연결되지 않은 노드들)
    void findLoneWolves() {
        bool* visited = new bool[numVertices + 1];
        for (int i = 0; i <= numVertices; i++) {
            visited[i] = false;
        }

        LinkedList loneWolves;

        for (int i = 1; i <= numVertices; i++) {
            if (!visited[i]) {
                LinkedList component;
                dfs(i, visited, component);

                // 컴포넌트 크기가 1이면 lone wolf
                if (component.size() == 1) {
                    loneWolves.add(i);
                }
            }
        }

        cout << "2. Lone wolf는? ";
        Node* current = loneWolves.getHead();
        if (current == nullptr) {
            cout << "없음";
        }
        else {
            while (current != nullptr) {
                cout << current->data << "번";
                if (current->next != nullptr) cout << ", ";
                current = current->next;
            }
        }
        cout << endl;

        delete[] visited;
    }

    // 3. 한 명으로 3시간 이내 모두에게 연락 가능한지 확인
    void findSingleOptimalContact() {
        int maxReachable = 0;
        DynamicArray bestNodes;

        // 모든 노드에서 3시간 이내 도달 가능한 수 계산
        for (int i = 1; i <= numVertices; i++) {
            int reachable = countReachableInTime(i, 3);
            if (reachable > maxReachable) {
                maxReachable = reachable;
            }
        }

        // 최대 도달 가능한 노드들 찾기
        for (int i = 1; i <= numVertices; i++) {
            int reachable = countReachableInTime(i, 3);
            if (reachable == maxReachable) {
                bestNodes.add(i);
            }
        }

        cout << "3. 3시간 이내에 모두에게 연락하려면 누구에게 연락해야할까? ";

        if (maxReachable == numVertices) {
            cout << "다음 중 아무나: ";
            for (int i = 0; i < bestNodes.size(); i++) {
                cout << bestNodes.get(i) << "번";
                if (i < bestNodes.size() - 1) cout << ", ";
            }
            cout << " (모든 " << numVertices << "명에게 연락 가능)";
        }
        else {
            cout << "모두에게 연락 불가능. 가장 많이 연락 가능한 사람: ";
            for (int i = 0; i < bestNodes.size(); i++) {
                cout << bestNodes.get(i) << "번";
                if (i < bestNodes.size() - 1) cout << ", ";
            }
            cout << " (최대 " << maxReachable << "명에게 연락 가능)";
        }
        cout << endl;
    }

    // 4. 여러 명으로 3시간 이내 모두에게 연락하는 모든 경우의 수 (단순화 버전)
    void findMultipleOptimalContacts() {
        cout << "4. 3시간 이내에 모두에게 연락하려면 누구 누구에게 연락해야할까? ";

        // 각 노드별 3시간 이내 도달 가능한 노드 수 계산
        int* reachCount = new int[numVertices + 1];
        for (int i = 1; i <= numVertices; i++) {
            reachCount[i] = countReachableInTime(i, 3);
        }

        // 가장 많이 도달할 수 있는 노드들을 우선적으로 선택
        bool* covered = new bool[numVertices + 1];
        for (int i = 1; i <= numVertices; i++) {
            covered[i] = false;
        }

        int* solution = new int[numVertices];
        int solutionCount = 0;
        int totalCovered = 0;

        cout << "계산 중... ";

        while (totalCovered < numVertices && solutionCount < numVertices) {
            int bestNode = -1;
            int bestNewCoverage = 0;

            // 아직 커버되지 않은 노드를 가장 많이 커버할 수 있는 노드 찾기
            for (int candidate = 1; candidate <= numVertices; candidate++) {
                int newCoverage = 0;

                // 이 후보가 새로 커버할 수 있는 노드 수 계산
                bool* tempVisited = new bool[numVertices + 1];
                int* tempDistance = new int[numVertices + 1];

                for (int i = 0; i <= numVertices; i++) {
                    tempVisited[i] = false;
                    tempDistance[i] = -1;
                }

                Queue tempQ;
                tempVisited[candidate] = true;
                tempDistance[candidate] = 0;
                tempQ.enqueue(candidate);

                if (!covered[candidate]) newCoverage++;

                while (!tempQ.isEmpty()) {
                    int current = tempQ.dequeue();

                    Node* neighbor = adjList[current].getHead();
                    while (neighbor != nullptr) {
                        int next = neighbor->data;
                        if (!tempVisited[next]) {
                            tempVisited[next] = true;
                            tempDistance[next] = tempDistance[current] + 1;
                            if (tempDistance[next] <= 3 && !covered[next]) {
                                newCoverage++;
                                tempQ.enqueue(next);
                            }
                            else if (tempDistance[next] <= 3) {
                                tempQ.enqueue(next);
                            }
                        }
                        neighbor = neighbor->next;
                    }
                }

                delete[] tempVisited;
                delete[] tempDistance;

                if (newCoverage > bestNewCoverage) {
                    bestNewCoverage = newCoverage;
                    bestNode = candidate;
                }
            }

            if (bestNode == -1 || bestNewCoverage == 0) {
                break; // 더 이상 커버할 수 없음
            }

            // 선택된 노드로 커버되는 모든 노드 마킹
            solution[solutionCount++] = bestNode;

            bool* visited = new bool[numVertices + 1];
            int* distance = new int[numVertices + 1];

            for (int i = 0; i <= numVertices; i++) {
                visited[i] = false;
                distance[i] = -1;
            }

            Queue q;
            visited[bestNode] = true;
            distance[bestNode] = 0;
            if (!covered[bestNode]) {
                covered[bestNode] = true;
                totalCovered++;
            }
            q.enqueue(bestNode);

            while (!q.isEmpty()) {
                int current = q.dequeue();

                Node* neighbor = adjList[current].getHead();
                while (neighbor != nullptr) {
                    int next = neighbor->data;
                    if (!visited[next]) {
                        visited[next] = true;
                        distance[next] = distance[current] + 1;
                        if (distance[next] <= 3) {
                            if (!covered[next]) {
                                covered[next] = true;
                                totalCovered++;
                            }
                            q.enqueue(next);
                        }
                    }
                    neighbor = neighbor->next;
                }
            }

            delete[] visited;
            delete[] distance;
        }

        if (totalCovered == numVertices) {
            cout << "최소 " << solutionCount << "명 필요: {";
            for (int i = 0; i < solutionCount; i++) {
                cout << solution[i] << "번";
                if (i < solutionCount - 1) cout << ", ";
            }
            cout << "}";
        }
        else {
            cout << "모든 노드를 커버할 수 없습니다. (최대 " << totalCovered << "/" << numVertices << "개 커버 가능)";
        }

        delete[] reachCount;
        delete[] covered;
        delete[] solution;
        cout << endl;
    }

    ~Graph() {
        delete[] adjList;
    }
};

int main() {
    cout << "=== 케빈 베이컨 게임 ===" << endl;

    ifstream inputFile("C:/Users/USER/Downloads/input.txt");
    if (!inputFile.is_open()) {
        cout << "input.txt 파일을 열 수 없습니다!" << endl;
        cout << "파일 경로를 확인해주세요: C:/Users/USER/Downloads/input.txt" << endl;
        return 1;
    }

    cout << "파일을 성공적으로 열었습니다!" << endl;

    int numVertices;
    inputFile >> numVertices;
    cout << "전체 노드 수: " << numVertices << endl;

    if (numVertices <= 0 || numVertices > 10000) {
        cout << "잘못된 노드 수입니다: " << numVertices << endl;
        return 1;
    }

    Graph graph(numVertices);

    // 그래프 구성
    string line;
    getline(inputFile, line); // 첫 번째 줄의 남은 부분 제거

    int edgeCount = 0;
    for (int i = 1; i <= numVertices; i++) {
        if (getline(inputFile, line) && !line.empty()) {
            // 문자열을 파싱하여 연결 정보 추출
            int pos = 0;
            int vertex = 0;
            bool first = true;

            // 공백으로 구분된 숫자들 파싱
            for (int j = 0; j <= (int)line.length(); j++) {
                if (j == (int)line.length() || line[j] == ' ') {
                    if (pos < j) {
                        int num = 0;
                        bool validNum = true;

                        // 숫자 파싱 (안전하게)
                        for (int k = pos; k < j; k++) {
                            if (line[k] >= '0' && line[k] <= '9') {
                                num = num * 10 + (line[k] - '0');
                            }
                            else if (line[k] != ' ') {
                                validNum = false;
                                break;
                            }
                        }

                        if (validNum && num > 0) {
                            if (first) {
                                vertex = num;
                                first = false;
                            }
                            else {
                                graph.addEdge(vertex, num);
                                edgeCount++;
                            }
                        }
                    }
                    pos = j + 1;
                }
            }
        }
    }

    cout << "총 " << edgeCount << "개의 연결을 읽었습니다." << endl;
    inputFile.close();

    cout << "\n=== 케빈 베이컨 게임 결과 ===" << endl;

    // 1. 나(67번)와 너(26번)의 거리
    int distance = graph.findShortestDistance(67, 26);
    cout << "1. 나(67번)와 너(26번)의 거리는? ";
    if (distance == -1) {
        cout << "연결되지 않음";
    }
    else {
        cout << distance << "단계";
    }
    cout << endl;

    // 2. Lone wolf 찾기
    graph.findLoneWolves();

    // 3. 한 명으로 3시간 이내 모두에게 연락
    graph.findSingleOptimalContact();

    // 4. 여러 명으로 3시간 이내 모두에게 연락
    graph.findMultipleOptimalContacts();

    return 0;
}