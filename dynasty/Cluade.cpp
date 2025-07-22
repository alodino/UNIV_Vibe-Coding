
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// 문자열 클래스 (STL string 대신)
class MyString {
private:
    char* data;
    int length;
    int capacity;

public:
    MyString() : data(nullptr), length(0), capacity(0) {
        data = new char[1];
        data[0] = '\0';
        capacity = 1;
    }

    MyString(const char* str) {
        length = strlen(str);
        capacity = length + 1;
        data = new char[capacity];
        strcpy(data, str);
    }

    MyString(const MyString& other) {
        length = other.length;
        capacity = other.capacity;
        data = new char[capacity];
        strcpy(data, other.data);
    }

    ~MyString() {
        delete[] data;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            capacity = other.capacity;
            data = new char[capacity];
            strcpy(data, other.data);
        }
        return *this;
    }

    MyString& operator=(const char* str) {
        delete[] data;
        length = strlen(str);
        capacity = length + 1;
        data = new char[capacity];
        strcpy(data, str);
        return *this;
    }

    bool operator==(const MyString& other) const {
        return strcmp(data, other.data) == 0;
    }

    bool operator==(const char* str) const {
        return strcmp(data, str) == 0;
    }

    const char* c_str() const {
        return data;
    }

    int size() const {
        return length;
    }
};

// 노드 클래스
class Node {
public:
    MyString name;
    Node* firstChild;
    Node* nextSibling;
    Node* parent;

    Node(const MyString& kingName) : name(kingName), firstChild(nullptr), nextSibling(nullptr), parent(nullptr) {}

    ~Node() {
        // 자식들과 형제들은 따로 관리됨
    }
};

// 동적 배열 클래스 (STL vector 대신)
template<typename T>
class MyVector {
private:
    T* data;
    int size_;
    int capacity_;

public:
    MyVector() : data(nullptr), size_(0), capacity_(0) {}

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& item) {
        if (size_ == capacity_) {
            int newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
            T* newData = new T[newCapacity];
            for (int i = 0; i < size_; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity_ = newCapacity;
        }
        data[size_++] = item;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int size() const {
        return size_;
    }

    void clear() {
        size_ = 0;
    }
};

// 스택 클래스 (STL stack 대신)
template<typename T>
class MyStack {
private:
    T* data;
    int top_;
    int capacity_;

public:
    MyStack() : data(nullptr), top_(-1), capacity_(0) {}

    ~MyStack() {
        delete[] data;
    }

    void push(const T& item) {
        if (top_ + 1 == capacity_) {
            int newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
            T* newData = new T[newCapacity];
            for (int i = 0; i <= top_; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity_ = newCapacity;
        }
        data[++top_] = item;
    }

    T pop() {
        if (top_ >= 0) {
            return data[top_--];
        }
        return T(); // 기본값 반환
    }

    bool empty() const {
        return top_ == -1;
    }

    int size() const {
        return top_ + 1;
    }
};

// 조선왕조 트리 클래스
class JoseonDynastyTree {
private:
    Node* root;
    MyVector<Node*> allNodes;
    MyVector<MyString> kingNames;

public:
    JoseonDynastyTree() : root(nullptr) {}

    ~JoseonDynastyTree() {
        for (int i = 0; i < allNodes.size(); i++) {
            delete allNodes[i];
        }
    }

    Node* findNode(const MyString& name) {
        for (int i = 0; i < allNodes.size(); i++) {
            if (allNodes[i]->name == name) {
                return allNodes[i];
            }
        }
        return nullptr;
    }

    Node* createNode(const MyString& name) {
        Node* node = new Node(name);
        allNodes.push_back(node);
        return node;
    }

    void addChild(Node* parent, Node* child) {
        if (parent->firstChild == nullptr) {
            parent->firstChild = child;
        }
        else {
            Node* temp = parent->firstChild;
            while (temp->nextSibling != nullptr) {
                temp = temp->nextSibling;
            }
            temp->nextSibling = child;
        }
        child->parent = parent;
    }

    void loadData(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "파일을 열 수 없습니다: " << filename << endl;
            return;
        }

        char line[200];
        bool isFirstLine = true;

        while (file.getline(line, sizeof(line))) {
            if (strlen(line) == 0) continue;

            char* token = strtok(line, " ");
            MyString childName(token);

            if (isFirstLine) {
                // 첫 번째 줄은 루트 노드
                root = createNode(childName);
                kingNames.push_back(childName);
                isFirstLine = false;
            }
            else {
                // 두 번째 토큰은 부모 이름
                token = strtok(nullptr, " ");
                if (token != nullptr) {
                    MyString parentName(token);

                    Node* parent = findNode(parentName);
                    Node* child = findNode(childName);

                    if (child == nullptr) {
                        child = createNode(childName);
                        kingNames.push_back(childName);
                    }
                    if (parent == nullptr) {
                        parent = createNode(parentName);
                    }

                    addChild(parent, child);
                }
            }
        }
        file.close();
    }

    void printLineage(Node* node) {
        if (node == nullptr) return;

        cout << node->name.c_str() << " : ";

        Node* child = node->firstChild;
        while (child != nullptr) {
            cout << child->name.c_str() << " ";
            child = child->nextSibling;
        }
        cout << endl;

        if (node->firstChild != nullptr) {
            printLineage(node->firstChild);
        }

        if (node->nextSibling != nullptr) {
            printLineage(node->nextSibling);
        }
    }

    void printLineage() {
        cout << "조선 왕조 트리 (이름: 자식들):" << endl;
        printLineage(root);
        cout << endl;
    }

    // 1. 조선의 왕을 순서대로 출력
    void printKingsInOrder(Node* node) {
        if (node == nullptr) return;

        cout << node->name.c_str() << " ";

        Node* child = node->firstChild;
        while (child != nullptr) {
            printKingsInOrder(child);
            child = child->nextSibling;
        }
    }

    void question1() {
        cout << "조선의 왕을 순서대로 출력:" << endl;
        printKingsInOrder(root);
        cout << endl;
    }

    // 2. 조선의 왕을 역순으로 출력
    void collectKings(Node* node, MyStack<Node*>& stack) {
        if (node == nullptr) return;

        stack.push(node);

        Node* child = node->firstChild;
        while (child != nullptr) {
            collectKings(child, stack);
            child = child->nextSibling;
        }
    }

    void question2() {
        cout << "조선의 왕을 역순으로 출력:" << endl;
        MyStack<Node*> stack;
        collectKings(root, stack);

        while (!stack.empty()) {
            Node* node = stack.pop();
            cout << node->name.c_str() << " ";
        }
        cout << endl;
    }

    // 3. 조선의 왕은 모두 몇 명인가?
    void question3() {
        cout << "조선의 왕은 모두 몇 명인가?" << endl;
        cout << kingNames.size() << "명" << endl;
    }

    // 4. 인조의 후손 출력
    void printDescendants(Node* node) {
        if (node == nullptr) return;

        Node* child = node->firstChild;
        while (child != nullptr) {
            cout << child->name.c_str() << " ";
            printDescendants(child);
            child = child->nextSibling;
        }
    }

    void question4() {
        cout << "조선의 왕 중에서 인조의 후손:" << endl;
        Node* injo = findNode(MyString("인조"));
        if (injo != nullptr) {
            printDescendants(injo);
        }
        else {
            cout << "인조를 찾을 수 없습니다.";
        }
        cout << endl;
    }

    // 5. 직계 후손이 왕이 되지 못한 왕
    bool isKing(const MyString& name) {
        for (int i = 0; i < kingNames.size(); i++) {
            if (kingNames[i] == name) {
                return true;
            }
        }
        return false;
    }

    void findKingsWithNoKingChildren(Node* node, MyVector<MyString>& result) {
        if (node == nullptr) return;

        if (isKing(node->name)) {
            bool hasKingChild = false;
            Node* child = node->firstChild;
            while (child != nullptr) {
                if (isKing(child->name)) {
                    hasKingChild = true;
                    break;
                }
                child = child->nextSibling;
            }

            if (!hasKingChild) {
                result.push_back(node->name);
            }
        }

        Node* child = node->firstChild;
        while (child != nullptr) {
            findKingsWithNoKingChildren(child, result);
            child = child->nextSibling;
        }
    }

    void question5() {
        cout << "직계 후손이 왕이 되지 못한 왕:" << endl;
        MyVector<MyString> result;
        findKingsWithNoKingChildren(root, result);

        for (int i = 0; i < result.size(); i++) {
            cout << result[i].c_str() << " ";
        }
        cout << endl;
    }

    // 6. 직계 후손이 왕이 된 수가 가장 많은 왕
    void findKingWithMostKingChildren(Node* node, MyString& maxKing, int& maxCount) {
        if (node == nullptr) return;

        if (isKing(node->name)) {
            int kingChildCount = 0;
            Node* child = node->firstChild;
            while (child != nullptr) {
                if (isKing(child->name)) {
                    kingChildCount++;
                }
                child = child->nextSibling;
            }

            if (kingChildCount > maxCount) {
                maxCount = kingChildCount;
                maxKing = node->name;
            }
        }

        Node* child = node->firstChild;
        while (child != nullptr) {
            findKingWithMostKingChildren(child, maxKing, maxCount);
            child = child->nextSibling;
        }
    }

    void question6() {
        cout << "직계 후손이 왕이 된 수가 가장 많은 왕:" << endl;
        MyString maxKing;
        int maxCount = 0;
        findKingWithMostKingChildren(root, maxKing, maxCount);
        cout << maxKing.c_str() << " (" << maxCount << "명)" << endl;
    }

    // 7. 정종의 형제로 조선의 왕이 된 사람
    void findJeongjongSiblings(Node* node, MyVector<MyString>& siblings) {
        if (node == nullptr) return;

        bool hasJeongjong = false;
        Node* child = node->firstChild;
        while (child != nullptr) {
            if (child->name == "정종") {
                hasJeongjong = true;
                break;
            }
            child = child->nextSibling;
        }

        if (hasJeongjong) {
            Node* sibling = node->firstChild;
            while (sibling != nullptr) {
                if (!(sibling->name == "정종") && isKing(sibling->name)) {
                    siblings.push_back(sibling->name);
                }
                sibling = sibling->nextSibling;
            }
        }

        Node* next = node->firstChild;
        while (next != nullptr) {
            findJeongjongSiblings(next, siblings);
            next = next->nextSibling;
        }
    }

    void question7() {
        cout << "정종의 형제로 조선의 왕이 된 사람:" << endl;
        MyVector<MyString> siblings;
        findJeongjongSiblings(root, siblings);

        for (int i = 0; i < siblings.size(); i++) {
            cout << siblings[i].c_str() << " ";
        }
        cout << endl;
    }

    // 8. 순종의 직계 선조 출력
    void question8() {
        cout << "순종의 직계 선조:" << endl;
        Node* sunjong = findNode(MyString("순종"));
        if (sunjong != nullptr) {
            Node* current = sunjong->parent;
            while (current != nullptr) {
                cout << current->name.c_str() << " ";
                current = current->parent;
            }
        }
        else {
            cout << "순종을 찾을 수 없습니다.";
        }
        cout << endl;
    }

    // 9. 직계 후손이 2명 이상 왕이 된 왕의 수
    int countParentsWithMultipleKingChildren(Node* node) {
        if (node == nullptr) return 0;

        int count = 0;
        int kingChildCount = 0;

        Node* child = node->firstChild;
        while (child != nullptr) {
            if (isKing(child->name)) {
                kingChildCount++;
            }
            child = child->nextSibling;
        }

        if (kingChildCount >= 2) {
            count = 1;
        }

        child = node->firstChild;
        while (child != nullptr) {
            count += countParentsWithMultipleKingChildren(child);
            child = child->nextSibling;
        }

        return count;
    }

    void question9() {
        cout << "직계 후손이 2명 이상 왕이 된 왕의 수:" << endl;
        int count = countParentsWithMultipleKingChildren(root);
        cout << count << "명" << endl;
    }

    // 10. 예종은 태종의 몇 대 후손인가?
    int getDepthFrom(Node* current, const MyString& targetName, int depth) {
        if (current == nullptr) return -1;

        if (current->name == targetName) {
            return depth;
        }

        Node* child = current->firstChild;
        while (child != nullptr) {
            int foundDepth = getDepthFrom(child, targetName, depth + 1);
            if (foundDepth != -1) {
                return foundDepth;
            }
            child = child->nextSibling;
        }

        return -1;
    }

    void question10() {
        cout << "예종은 태종의 몇 대 후손인가?" << endl;
        Node* taejong = findNode(MyString("태종"));
        if (taejong != nullptr) {
            int depth = getDepthFrom(taejong, MyString("예종"), 0);
            if (depth != -1) {
                cout << depth << "대 후손입니다." << endl;
            }
            else {
                cout << "태종부터 예종까지 연결 경로가 없습니다." << endl;
            }
        }
        else {
            cout << "태종을 찾을 수 없습니다." << endl;
        }
    }

    void processQuestions(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "질문 파일을 열 수 없습니다: " << filename << endl;
            return;
        }

        int numQuestions;
        file >> numQuestions;
        file.ignore(); // 개행문자 무시

        cout << "총 " << numQuestions << "개의 질문을 처리합니다." << endl << endl;

        char line[500];
        for (int i = 1; i <= numQuestions; i++) {
            if (file.getline(line, sizeof(line))) {
                cout << "[Q" << i << "] " << line << endl;

                switch (i) {
                case 1: question1(); break;
                case 2: question2(); break;
                case 3: question3(); break;
                case 4: question4(); break;
                case 5: question5(); break;
                case 6: question6(); break;
                case 7: question7(); break;
                case 8: question8(); break;
                case 9: question9(); break;
                case 10: question10(); break;
                default:
                    cout << "처리할 수 없는 질문입니다." << endl;
                    break;
                }
                cout << endl;
            }
        }
        file.close();
    }
};

int main() {
    JoseonDynastyTree tree;

    // 조선왕조 데이터 로드
    tree.loadData("조선왕조.txt");

    // 트리 구조 출력
    tree.printLineage();

    // 질문 처리
    tree.processQuestions("quiz.txt");

    return 0;
}
