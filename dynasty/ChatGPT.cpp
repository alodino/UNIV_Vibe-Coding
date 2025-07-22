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

// ���� ����
Node* kings[200]; // �� �̸� �� ��� ������ �����
string kingNames[200]; // �̸��� ���� ����
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
    ifstream file("��������.txt");
    string child, parent;

    while (file >> child) {
        Node* childNode = createOrGetNode(child);

        if (file.peek() == '\n' || file.eof()) {
            continue; // ù ��: ���� ȥ��
        }

        file >> parent;
        Node* parentNode = createOrGetNode(parent);
        childNode->parent = parentNode;
        parentNode->addChild(childNode);
    }
}

// ���� 1
void printInOrder(Node* root) {
    if (root == nullptr) return;
    cout << root->name << '\n';
    for (int i = 0; i < root->childCount; ++i) {
        printInOrder(root->children[i]);
    }
}

// ���� 2
void printReverseOrder(Node* root) {
    if (root == nullptr) return;
    for (int i = root->childCount - 1; i >= 0; --i) {
        printReverseOrder(root->children[i]);
    }
    cout << root->name << '\n';
}

// ���� 3
int countAllKings() {
    return kingCount;
}

// ���� 4
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

// ���� 5
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

// ���� 6
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

// ���� 7
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

// ���� 8
void printAncestors(Node* node) {
    if (node->parent == nullptr) return;
    printAncestors(node->parent);
    cout << node->parent->name << '\n';
}

// ���� 9
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

// ���� 10
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

    cout << "1. ������� ���\n";
    printInOrder(findNode("����"));
    cout << "\n2. ���� ���\n";
    printReverseOrder(findNode("����"));
    cout << "\n3. �� ��: " << countAllKings() << '\n';
    cout << "\n4. ������ �ļ�:\n";
    printDescendants(findNode("����"), "����");
    cout << "\n5. �İ��� ���� ��:\n";
    printNoSuccessors();
    cout << "\n6. ���� �ļ� ���� ���� ��:\n";
    kingWithMostSuccessors();
    cout << "\n7. ������ ������ �� �� ���:\n";
    printSiblingsWhoBecameKings("����");
    cout << "\n8. ������ ����:\n";
    printAncestors(findNode("����"));
    cout << "\n9. 2�� �̻� �ļ� ��: " << countKingsWith2OrMoreKingChildren() << "��\n";
    cout << "\n10. ������ ������ " << generationGap("����", "����") << "�� �ļ�\n";

    return 0;
}
