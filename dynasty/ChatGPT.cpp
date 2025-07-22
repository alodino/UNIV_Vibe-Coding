#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    string name;
    Node* parent;
    Node* children[100];
    int childCount;

    Node(const string& n) {
        name = n;
        parent = nullptr;
        childCount = 0;
    }

    void addChild(Node* child) {
        children[childCount++] = child;
    }
};

// 전역 변수
Node* kings[200]; // 왕 이름 → 노드 포인터 저장용
string kingNames[200]; // 이름들 순서 보존
int kingCount = 0;

Node* findNode(const string& name) {
    for (int i = 0; i < kingCount; ++i) {
        if (kings[i]->name == name) return kings[i];
    }
    return nullptr;
}

Node* createOrGetNode(const string& name) {
    Node* node = findNode(name);
    if (node != nullptr) return node;

    Node* newNode = new Node(name);
    kings[kingCount++] = newNode;
    return newNode;
}

void readData() {
    ifstream file("조선왕조.txt");
    string child, parent;

    while (file >> child) {
        Node* childNode = createOrGetNode(child);

        if (file.peek() == '\n' || file.eof()) {
            continue; // 첫 줄: 태조 혼자
        }

        file >> parent;
        Node* parentNode = createOrGetNode(parent);
        childNode->parent = parentNode;
        parentNode->addChild(childNode);
    }
}

// 질문 1
void printInOrder(Node* root) {
    if (root == nullptr) return;
    cout << root->name << '\n';
    for (int i = 0; i < root->childCount; ++i) {
        printInOrder(root->children[i]);
    }
}

// 질문 2
void printReverseOrder(Node* root) {
    if (root == nullptr) return;
    for (int i = root->childCount - 1; i >= 0; --i) {
        printReverseOrder(root->children[i]);
    }
    cout << root->name << '\n';
}

// 질문 3
int countAllKings() {
    return kingCount;
}

// 질문 4
void printDescendants(Node* root, const string& target) {
    if (root == nullptr) return;
    if (root->name == target) {
        for (int i = 0; i < root->childCount; ++i) {
            printInOrder(root->children[i]);
        }
        return;
    }
    for (int i = 0; i < root->childCount; ++i) {
        printDescendants(root->children[i], target);
    }
}

// 질문 5
void printNoSuccessors() {
    for (int i = 0; i < kingCount; ++i) {
        bool hasKingChild = false;
        for (int j = 0; j < kings[i]->childCount; ++j) {
            if (findNode(kings[i]->children[j]->name)) {
                hasKingChild = true;
                break;
            }
        }
        if (!hasKingChild) {
            cout << kings[i]->name << '\n';
        }
    }
}

// 질문 6
void kingWithMostSuccessors() {
    int maxCount = 0;
    string kingName;
    for (int i = 0; i < kingCount; ++i) {
        int count = 0;
        for (int j = 0; j < kings[i]->childCount; ++j) {
            if (findNode(kings[i]->children[j]->name)) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            kingName = kings[i]->name;
        }
    }
    cout << kingName << '\n';
}

// 질문 7
void printSiblingsWhoBecameKings(const string& target) {
    Node* node = findNode(target);
    if (!node || !node->parent) return;
    Node* father = node->parent;
    for (int i = 0; i < father->childCount; ++i) {
        if (father->children[i] != node &&
            findNode(father->children[i]->name)) {
            cout << father->children[i]->name << '\n';
        }
    }
}

// 질문 8
void printAncestors(Node* node) {
    if (node->parent == nullptr) return;
    printAncestors(node->parent);
    cout << node->parent->name << '\n';
}

// 질문 9
int countKingsWith2OrMoreKingChildren() {
    int count = 0;
    for (int i = 0; i < kingCount; ++i) {
        int kingChild = 0;
        for (int j = 0; j < kings[i]->childCount; ++j) {
            if (findNode(kings[i]->children[j]->name)) {
                kingChild++;
            }
        }
        if (kingChild >= 2) count++;
    }
    return count;
}

// 질문 10
int generationGap(const string& ancestor, const string& descendant) {
    Node* desc = findNode(descendant);
    int count = 0;
    while (desc && desc->name != ancestor) {
        desc = desc->parent;
        count++;
    }
    return (desc) ? count : -1;
}

int main() {
    readData();

    cout << "1. 순서대로 출력\n";
    printInOrder(findNode("태조"));
    cout << "\n2. 역순 출력\n";
    printReverseOrder(findNode("태조"));
    cout << "\n3. 왕 수: " << countAllKings() << '\n';
    cout << "\n4. 인조의 후손:\n";
    printDescendants(findNode("태조"), "인조");
    cout << "\n5. 후계자 없는 왕:\n";
    printNoSuccessors();
    cout << "\n6. 직계 후손 가장 많은 왕:\n";
    kingWithMostSuccessors();
    cout << "\n7. 정종의 형제로 왕 된 사람:\n";
    printSiblingsWhoBecameKings("정종");
    cout << "\n8. 순종의 선조:\n";
    printAncestors(findNode("순종"));
    cout << "\n9. 2명 이상 후손 왕: " << countKingsWith2OrMoreKingChildren() << "명\n";
    cout << "\n10. 예종은 태종의 " << generationGap("태종", "예종") << "대 후손\n";

    return 0;
}
