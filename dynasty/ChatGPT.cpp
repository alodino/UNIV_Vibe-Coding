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
    KingNode* kings[200]; // �ִ� 200���� �� ����
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
        cout << "1. ������ ���� ������� ����Ͻÿ�:\n";
        printPreorder(root);

        cout << "\n2. ������ ���� �������� ����Ͻÿ�:\n";
        printPostorder(root);

        cout << "\n3. ������ ���� ��� �� ���ΰ�?\n" << kingCount << endl;

        cout << "\n4. ������ �� �߿��� ������ �ļ��� ���������ΰ�?\n";
        findAllDescendants(findKing("����"));

        cout << "\n5. ���� �ļ��� ���� ���� ���� ���� ���������ΰ�?\n";
        findDirectSonsWhoWereNotKings();

        cout << "\n6. ���� �ļ��� ���� �� ���� ���� ���� ���� �����ΰ�?\n";
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

        cout << "\n7. ������ ������ ������ ���� �� ����� �����ΰ�?\n";
        findSiblingsWhoWereKings(findKing("����"));

        cout << "\n8. ������ ���� ������ ��� ����Ͻÿ�:\n";
        printAncestors(findKing("����"));

        cout << "\n9. ���� �ļ��� 2�� �̻� ���� �� ���� �� ���ΰ�?\n" << countKingsWith2OrMoreKingSons() << endl;

        cout << "\n10. ������ ������ �� �� �ļ��ΰ�?\n";
        cout << countGenerations(findKing("����"), findKing("����")) << endl;
    }
};

int main() {
    DynastyTree tree;
    tree.loadFromFile("��������.txt");
    tree.answerQuestions();
    return 0;
}
