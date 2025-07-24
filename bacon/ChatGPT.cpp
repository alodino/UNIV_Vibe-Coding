#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX = 1000;

// ���� ����Ʈ ���
struct Node {
    int data;
    Node* next;
};

// ť ����ü
struct Queue {
    int items[MAX];
    int front, rear;

    void init() { front = rear = 0; }
    bool isEmpty() { return front == rear; }
    void enqueue(int value) { if (rear < MAX) items[rear++] = value; }
    int dequeue() { return isEmpty() ? -1 : items[front++]; }
};

// �׷��� ����ü
struct Graph {
    Node* adj[MAX];
    int size;

    void init(int n) {
        size = n + 1;
        for (int i = 0; i < size; ++i) adj[i] = nullptr;
    }

    void addEdge(int u, int v) {
        Node* node = new Node{ v, adj[u] };
        adj[u] = node;
    }
};

// ���ᵵ ���� �Լ�
int countReachable(Graph& g, int start, int n, int maxDepth) {
    bool visited[MAX] = {};
    Queue q; q.init();
    q.enqueue(start);
    visited[start] = true;

    int count = 1;
    int level = 0;

    while (!q.isEmpty() && level < maxDepth) {
        int limit = q.rear;
        while (q.front < limit) {
            int curr = q.dequeue();
            for (Node* temp = g.adj[curr]; temp; temp = temp->next) {
                if (!visited[temp->data]) {
                    visited[temp->data] = true;
                    ++count;
                    q.enqueue(temp->data);
                }
            }
        }
        ++level;
    }

    return count;
}

// ���� ��� 3�ܰ� �� �ִ� ������ ����
void findBestSingleConnectors(Graph& g, int n) {
    int totalConnectable = 0;
    for (int i = 1; i <= n; ++i) if (g.adj[i]) ++totalConnectable;

    int best = -1, bestCount = -1;
    int top5[5] = {}, scores[5] = {};

    for (int i = 1; i <= n; ++i) {
        if (!g.adj[i]) continue;
        int reach = countReachable(g, i, n, 3);
        if (reach == totalConnectable) {
            cout << "3. 3�ܰ� �̳� ��ü ���� ������ ���� ���: " << i << endl;
            return;
        }

        for (int j = 0; j < 5; ++j) {
            if (reach > scores[j]) {
                for (int k = 4; k > j; --k) {
                    scores[k] = scores[k - 1];
                    top5[k] = top5[k - 1];
                }
                scores[j] = reach;
                top5[j] = i;
                break;
            }
        }
    }

    cout << "3. ���� ��ҷ� 3�ܰ� �̳� ��ü ���� �Ұ���. �ְ� ������ ���� 5�� ���:\n";
    for (int i = 0; i < 5 && top5[i] != 0; ++i) {
        cout << "  ��� " << top5[i] << " - ���ᵵ: " << scores[i] << endl;
    }
}

// �ִ� ��� ��� �Լ�
int printPath(int parent[], int start, int end) {
    if (start == end) { cout << start; return 0; }
    int dist = printPath(parent, start, parent[end]);
    cout << " - " << end;
    return dist + 1;
}

// BFS �ִ� ��� ���
void shortestPath(Graph& g, int start, int end) {
    bool visited[MAX] = {};
    int parent[MAX];
    for (int i = 0; i < MAX; ++i) parent[i] = -1;

    Queue q; q.init();
    q.enqueue(start);
    visited[start] = true;

    while (!q.isEmpty()) {
        int curr = q.dequeue();
        for (Node* temp = g.adj[curr]; temp; temp = temp->next) {
            if (!visited[temp->data]) {
                visited[temp->data] = true;
                parent[temp->data] = curr;
                q.enqueue(temp->data);
                if (temp->data == end) break;
            }
        }
    }

    if (!visited[end]) cout << "��ΰ� �������� �ʽ��ϴ�." << endl;
    else {
        int dist = printPath(parent, start, end);
        cout << "\n�Ÿ�: " << dist << endl;
    }
}

// ���� ��� ���� Lone Wolf Ž��
void countComponentsAndLoneWolves(Graph& g, int n) {
    bool visited[MAX] = {};
    int components = 0, loneWolves = 0;
    int loneWolfList[MAX];
    int lwCount = 0;

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            if (!g.adj[i]) {
                loneWolfList[lwCount++] = i;
                ++loneWolves;
                ++components;
                continue;
            }
            Queue q; q.init(); q.enqueue(i); visited[i] = true;
            while (!q.isEmpty()) {
                int curr = q.dequeue();
                for (Node* temp = g.adj[curr]; temp; temp = temp->next) {
                    if (!visited[temp->data]) {
                        visited[temp->data] = true;
                        q.enqueue(temp->data);
                    }
                }
            }
            ++components;
        }
    }

    cout << "Lone Wolves: " << loneWolves << " (";
    for (int i = 0; i < lwCount; ++i) {
        if (i > 0) cout << ", ";
        cout << loneWolfList[i];
    }
    cout << ")" << endl;
    cout << "Total connected components: " << components << endl;
}

// 4�� - 3�ܰ� �̳� ��ü ��Ҹ� Ŀ���ϴ� �ּ� ��� ����
void findThreeStepCoverSet(Graph& g, int n) {
    bool covered[MAX] = {};
    bool selected[MAX] = {};
    int total = 0;
    for (int i = 1; i <= n; ++i) if (g.adj[i]) ++total;
    int coveredCount = 0;

    while (coveredCount < total) {
        int bestNode = -1, bestCover = -1;
        bool visited[MAX];

        for (int i = 1; i <= n; ++i) {
            if (selected[i]) continue;

            for (int k = 0; k < MAX; ++k) visited[k] = false;
            Queue q; q.init();
            q.enqueue(i); visited[i] = true;

            int level = 0;
            while (!q.isEmpty() && level < 3) {
                int limit = q.rear;
                while (q.front < limit) {
                    int curr = q.dequeue();
                    for (Node* temp = g.adj[curr]; temp; temp = temp->next) {
                        if (!visited[temp->data]) {
                            visited[temp->data] = true;
                            q.enqueue(temp->data);
                        }
                    }
                }
                ++level;
            }

            int newlyCovered = 0;
            for (int j = 1; j <= n; ++j) {
                if (visited[j] && !covered[j] && g.adj[j]) ++newlyCovered;
            }

            if (newlyCovered > bestCover) {
                bestCover = newlyCovered;
                bestNode = i;
            }
        }

        if (bestNode == -1) break;

        selected[bestNode] = true;

        for (int k = 0; k < MAX; ++k) visited[k] = false;
        Queue q; q.init();
        q.enqueue(bestNode); visited[bestNode] = true;

        int level = 0;
        while (!q.isEmpty() && level < 3) {
            int limit = q.rear;
            while (q.front < limit) {
                int curr = q.dequeue();
                for (Node* temp = g.adj[curr]; temp; temp = temp->next) {
                    if (!visited[temp->data]) {
                        visited[temp->data] = true;
                        if (!covered[temp->data] && g.adj[temp->data]) {
                            covered[temp->data] = true;
                            ++coveredCount;
                        }
                        q.enqueue(temp->data);
                    }
                }
            }
            ++level;
        }
    }

    cout << "4. 3�ܰ� �̳� ��� ��Ҹ� Ŀ���ϴ� �ּ� ��� ����: ";
    for (int i = 1; i <= n; ++i) {
        if (selected[i]) cout << i << " ";
    }
    cout << endl;
}

int main() {
    ifstream file("input.txt");
    if (!file) { cout << "������ �� �� �����ϴ�." << endl; return 1; }

    int n; file >> n;
    Graph g; g.init(n);

    for (int i = 0; i < n; ++i) {
        int from, to;
        file >> from;
        while (file.peek() != '\n' && file >> to) {
            g.addEdge(from, to);
        }
        file.ignore();
    }

    cout << "1. 67���� 26�� �ִ� ���: ";
    shortestPath(g, 67, 26);

    cout << "\n2. Lone Wolf �� ���� ���� ��:\n";
    countComponentsAndLoneWolves(g, n);

    cout << "\n3. ���� ��� ��� �ִ� ���ᵵ �˻�:\n";
    findBestSingleConnectors(g, n);

    cout << "\n4. ���� ��� ��� ��ü Ŀ���� ��� ����:\n";
    findThreeStepCoverSet(g, n);

    return 0;
}
