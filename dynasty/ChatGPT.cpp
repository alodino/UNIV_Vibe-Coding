#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct KingNode {
    string name;
    KingNode* parent;
    KingNode* firstChild;
    KingNode* nextSibling;
};

class DynastyTree {
private:
    KingNode* root;
    KingNode* kings[200]; // 최대 200명의 왕 가정
    int kingCount;

    KingNode* findKing(const string& name) {
        for (int i = 0; i < kingCount; ++i) {
            if (kings[i]->name == name) return kings[i];
        }
        return nullptr;
    }

    void addChild(KingNode* parent, KingNode* child) {
        child->nextSibling = parent->firstChild;
        parent->firstChild = child;
    }

    void printPreorder(KingNode* node) {
        if (!node) return;
        cout << node->name << endl;
        printPreorder(node->firstChild);
        printPreorder(node->nextSibling);
    }

    void printPostorder(KingNode* node) {
        if (!node) return;
        printPostorder(node->firstChild);
        cout << node->name << endl;
        printPostorder(node->nextSibling);
    }

    void findAllDescendants(KingNode* node) {
        if (!node) return;
        for (int i = 0; i < kingCount; ++i) {
            KingNode* temp = kings[i];
            KingNode* ancestor = temp->parent;
            while (ancestor) {
                if (ancestor == node) {
                    cout << temp->name << endl;
                    break;
                }
                ancestor = ancestor->parent;
            }
        }
    }

    void findDirectSonsWhoWereNotKings() {
        for (int i = 0; i < kingCount; ++i) {
            KingNode* p = kings[i];
            bool hasKingChild = false;
            for (int j = 0; j < kingCount; ++j) {
                if (kings[j]->parent == p) {
                    hasKingChild = true;
                    break;
                }
            }
            if (!hasKingChild) cout << p->name << endl;
        }
    }

    int countKingChildren(KingNode* parent) {
        int cnt = 0;
        for (int i = 0; i < kingCount; ++i) {
            if (kings[i]->parent == parent) ++cnt;
        }
        return cnt;
    }

    void findSiblingsWhoWereKings(KingNode* target) {
        if (!target || !target->parent) return;
        KingNode* sibling = target->parent->firstChild;
        while (sibling) {
            if (sibling != target) cout << sibling->name << endl;
            sibling = sibling->nextSibling;
        }
    }

    void printAncestors(KingNode* node) {
        while (node->parent) {
            cout << node->parent->name << endl;
            node = node->parent;
        }
    }

    int countKingsWith2OrMoreKingSons() {
        int result = 0;
        for (int i = 0; i < kingCount; ++i) {
            if (countKingChildren(kings[i]) >= 2) ++result;
        }
        return result;
    }

    int countGenerations(KingNode* desc, KingNode* ancestor) {
        int gen = 0;
        while (desc && desc != ancestor) {
            desc = desc->parent;
            ++gen;
        }
        return (desc == ancestor) ? gen : -1;
    }

public:
    DynastyTree() : root(nullptr), kingCount(0) {}

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            string child, parent;
            size_t pos = line.find(' ');
            if (pos == string::npos) {
                child = line;
                parent = "";
            }
            else {
                child = line.substr(0, pos);
                parent = line.substr(pos + 1);
            }

            KingNode* childNode = findKing(child);
            if (!childNode) {
                childNode = new KingNode{ child, nullptr, nullptr, nullptr };
                kings[kingCount++] = childNode;
            }

            if (!parent.empty()) {
                KingNode* parentNode = findKing(parent);
                if (!parentNode) {
                    parentNode = new KingNode{ parent, nullptr, nullptr, nullptr };
                    kings[kingCount++] = parentNode;
                }
                childNode->parent = parentNode;
                addChild(parentNode, childNode);
            }
            else {
                root = childNode;
            }
        }
    }

    void answerQuestions() {
        cout << "1. 조선의 왕을 순서대로 출력하시오:\n";
        printPreorder(root);

        cout << "\n2. 조선의 왕을 역순으로 출력하시오:\n";
        printPostorder(root);

        cout << "\n3. 조선의 왕은 모두 몇 명인가?\n" << kingCount << endl;

        cout << "\n4. 조선의 왕 중에서 인조의 후손은 누구누구인가?\n";
        findAllDescendants(findKing("인조"));

        cout << "\n5. 직계 후손이 왕이 되지 못한 왕은 누구누구인가?\n";
        findDirectSonsWhoWereNotKings();

        cout << "\n6. 직계 후손이 왕이 된 수가 가장 많은 왕은 누구인가?\n";
        string maxKing;
        int maxChildren = 0;
        for (int i = 0; i < kingCount; ++i) {
            int cnt = countKingChildren(kings[i]);
            if (cnt > maxChildren) {
                maxChildren = cnt;
                maxKing = kings[i]->name;
            }
        }
        cout << maxKing << endl;

        cout << "\n7. 정종의 형제로 조선의 왕이 된 사람은 누구인가?\n";
        findSiblingsWhoWereKings(findKing("정종"));

        cout << "\n8. 순종의 직계 선조를 모두 출력하시오:\n";
        printAncestors(findKing("순종"));

        cout << "\n9. 직계 후손이 2명 이상 왕이 된 왕은 몇 명인가?\n" << countKingsWith2OrMoreKingSons() << endl;

        cout << "\n10. 예종은 태종의 몇 대 후손인가?\n";
        cout << countGenerations(findKing("예종"), findKing("태종")) << endl;
    }
};

int main() {
    DynastyTree tree;
    tree.loadFromFile("조선왕조.txt");
    tree.answerQuestions();
    return 0;
}
