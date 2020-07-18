#if 0
/* Assignment 2 Red Black Trees 25/04/2017
cout << "Press I(Insert) D(Delete) R(FindRank) C(Ith Rank) S(Search) E(Exit)" << endl;
    Tested in Visual Studio 2013 for inputs of range 100000;
    set test value in main function to true to check for the automated data
else
insert from console
    TESTED_OK
*/


#include<iostream>
#include<time.h>
using namespace std;
#define Blank &blank
enum Color { Red, Black };
//to support all kind of key value pairs T and U
template<class T, class U>
class RBnode
{
public:
    T key_;
    U data_;
    RBnode* left;
    RBnode* right;
    RBnode* parent;
    Color c;
    int size; //augmeted variable which stores both the size of left and right subtrees
    RBnode()
    {
        left = NULL;
        right = NULL;
        parent = NULL;
        size = 0;
        data_ = NULL;
        key_ = 0;
        c = Black;
    }
};

typedef  RBnode<int, int>* SENTINELPTR;
RBnode<int, int> blank;
SENTINELPTR blankptr = &blank;
template<class T, class U>
class RBTree
{
public:
    RBnode<T, U>* root;
    RBTree()
    {
        root = NULL;
    }
    void inordertraverseutil(RBnode<T, U>* current)
    {
        if (current == blankptr)
        {
            return;
        }
        inordertraverseutil(current->left);
        cout << current->key_ << ", ";
        inordertraverseutil(current->right);
    }
    void inordertraverse()
    {
        if (root == blankptr)
            return;
        RBnode<T, U>* current = root;
        inordertraverseutil(current);
    }
    RBnode<T, U>* inordersuccesor(RBnode<T, U>* x) {
        while (x->left != blankptr)
            x = x->left;
        return x;
    }
    void leftRotate(RBnode<T, U>* x); //O(1)
    void rightRotate(RBnode<T, U>* x);//O(1)
    void fixInsert(RBnode<T, U>* x);//O(1) //O(logn) sometimes
    void fixDelete(RBnode<T, U>* x);//O(1) //O(logn)
    void insertRB(T key, U data); //O(logn)
    void deleteRB(T key); //O(logn)
    void copynode(RBnode<T, U>* pare, RBnode<T, U>* child)
    {
        //copies child to parent
        if (pare->parent == blankptr)
        {
            root = child;
        }
        else if (pare == pare->parent->left)
        {
            pare->parent->left = child;
        }
        else
        {
            pare->parent->right = child;
        }
        child->parent = pare->parent;
    }
    RBnode<T, U>* searchRB(T key)
    {
        RBnode<T, U>* current = root;
        while (current != blankptr)
        {
            if (current->key_ == key)
            {
                return current;
            }
            else if (current->key_ > key)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }

        }
        return blankptr;
    }
    RBnode<T, U>* FindithRank(int i) //O(logn)
    {
        int k = root->left->size + 1;
        RBnode<T, U>* current = root;
        while (current != blankptr && k != i)
        {
            if (k < i)
            {
                i = i - k;
                current = current->right;
            }
            else
            {
                current = current->left;
            }
            if (current == blankptr)
                return blankptr;
            k = current->left->size + 1;
        }
        return current;
    }
    int FindRank(T key) //O(logn)
    {
        RBnode<T, U>* current = root;
        int count = 0;
        while (current != blankptr)
        {
            if (current->key_ < key)
            {
                count += current->left->size + 1;
                current = current->right;
            }
            else if (current->key_ > key)
            {
                current = current->left;
            }
            else
            {
                return count + current->left->size + 1; //assuming we need the rank node count not less count (-1 if we want less)
            }
        }
        return -1;
    }
};
template<class T, class U>
void RBTree<T, U> ::leftRotate(RBnode<T, U>* x)
{
    RBnode<T, U>* y = x->right;
    x->right = y->left;
    if (y->left != blankptr)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent != blankptr)
    {
        if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
    }
    else
    {
        root = y;
    }
    y->left = x;
    x->parent = y;
    //the child size remains as of parent where parent as it gets a left node of right it increases so set again same as right
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

template<class T, class U>
void RBTree<T, U> ::rightRotate(RBnode<T, U>* x)
{
    RBnode<T, U>* y = x->left;
    x->left = y->right;
    if (y->right != blankptr)
    {
        y->right->parent = x;
    }
    //first fix parent parent
    y->parent = x->parent;
    if (x->parent != blankptr)
    {
        if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
    }
    else
    {
        root = y;
    }
    y->right = x;
    x->parent = y;
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;

}
template<class T, class U>
void RBTree<T, U> ::fixInsert(RBnode<T, U>* x)
{
    while (x->parent->c == Red)
    {
        if (x->parent->parent->left == x->parent)
        {
            RBnode<T, U>* uncle = x->parent->parent->right;
            if (uncle->c == Red)
            {
                //interchange
                uncle->c = Black;
                x->parent->c = Black;
                x->parent->parent->c = Red;
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->right)
                {
                    x = x->parent;
                    leftRotate(x);
                }
                x->parent->c = Black;
                x->parent->parent->c = Red;
                rightRotate(x->parent->parent);
            }

        }
        else
        {
            //mirror
            RBnode<T, U>* uncle = x->parent->parent->left;
            if (uncle->c == Red)
            {
                //interchange
                uncle->c = Black;
                x->parent->c = Black;
                x->parent->parent->c = Red;
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->left)
                {
                    x = x->parent;
                    rightRotate(x);
                }
                x->parent->c = Black;
                x->parent->parent->c = Red;
                leftRotate(x->parent->parent);
            }



        }


    }

    root->c = Black;
}
template<class T, class U>
void RBTree<T, U> ::insertRB(T key, U data)
{
    RBnode<T, U>* x = new RBnode<T, U>();
    x->key_ = key;
    x->data_ = data;
    RBnode<T, U>* current = root;
    RBnode<T, U>* parent = blankptr;
    while (current != blankptr)
    {
        parent = current;
        parent->size++;
        if (current->key_ == key) // no duplicates
        {
            RBnode<T, U>* temp = current;
            while (current != root)
            {
                current->size--;
                current = current->parent;
            }
            current->size--;
            delete x;
            return;
        }
        if (current->key_ < key)
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }
    }
    x->parent = parent;
    if (parent == blankptr)
    {
        root = x;
    }
    else
    {
        if (parent->key_ > key)
        {
            parent->left = x;
        }
        else
        {
            parent->right = x;
        }

    }
    x->left = blankptr;
    x->right = blankptr;
    x->c = Red;
    x->size = 1;
    fixInsert(x);
}
template<class T, class U>
void RBTree<T, U> ::fixDelete(RBnode<T, U>* x)
{
    while (x != root && x->c == Black)
    {
        if (x == x->parent->left)
        {
            RBnode<T, U>* sibling = x->parent->right;
            if (sibling->c == Red)
            {
                sibling->c = Black;
                x->parent->c = Red;
                leftRotate(x->parent);
                sibling = x->parent->right;
            }
            if (sibling->left->c == Black && sibling->right->c == Black)
            {
                sibling->c = Red;
                x = x->parent;
            }
            else {

                if (sibling->right->c == Black)
                {
                    sibling->left->c = Black;
                    sibling->c = Red;
                    rightRotate(sibling);
                    sibling = x->parent->right;
                }
                sibling->c = x->parent->c; //??
                x->parent->c = Black;
                sibling->right->c = Black;
                leftRotate(x->parent);
                x = root;
            }
        }
        else
        {
            RBnode<T, U>* sibling = x->parent->left;
            if (sibling->c == Red)
            {
                sibling->c = Black;
                x->parent->c = Red;
                rightRotate(x->parent);
                sibling = x->parent->left;
            }
            if (sibling->left->c == Black && sibling->right->c == Black)
            {
                sibling->c = Red;
                x = x->parent;
            }
            else
            {
                if (sibling->left->c == Black)
                {
                    sibling->right->c = Black;
                    sibling->c = Red;
                    leftRotate(sibling);
                    sibling = x->parent->left;
                }
                sibling->c = x->parent->c; //??
                x->parent->c = Black;
                sibling->left->c = Black;
                rightRotate(x->parent);
                x = root;
            }
        }


    }
    x->c = Black;
}
template<class T, class U>
void RBTree<T, U> ::deleteRB(T key)
{

    RBnode<T, U>* x = searchRB(key);
    Color color = x->c;
    if (x == blankptr)
    {
        return;
    }
    if (x != root)
    {

        //reduce the size of all the nodes parents of the deleted node
        RBnode<T, U>* y = x->parent;
        while (y != root)
        {
            y->size--;
            y = y->parent;
        }
        root->size--; //root
    }
    RBnode<T, U>* y, * z;
    y = x;
    if (x->left == blankptr)
    {
        //no size change in this case as we just copy size of its children
        z = x->right;
        copynode(x, x->right);

    }
    else if (x->right == blankptr)
    {
        z = x->left;
        copynode(x, x->left);
    }
    else
    {
        y = inordersuccesor(x->right);
        color = y->c;
        z = y->right; //no left for inordersuccesor
        if (y->parent == x) //immediate parent;
        {
            z->parent = y;
        }
        else
        {
            copynode(y, y->right);
            y->right = x->right; //fix the inordesucc and deletednode
            y->right->parent = y;
            //reduce the parent size until it reaches the node to be deleted for inordersuccesor
            RBnode<T, U>* t = z->parent;
            while (t != y) //only 1 time to change size
            {
                t->size--;
                t = t->parent;
            }
            y->size = y->left->size + 1;
        }
        copynode(x, y);
        y->left = x->left;
        y->left->parent = y;
        y->c = color;
        y->size = y->left->size + y->right->size + 1; //copy the size of left and right as in insert fixup
    }
    if (color == Black)
        fixDelete(z);
}
void testcode(RBTree<int, int>* rb)
{
    for (int i = 1; i < 100000; i++)
    {
        rb->insertRB(i, i);
    }
    cout << "67 before deletion" << rb->FindRank(67) << endl;
    rb->deleteRB(50);
    rb->deleteRB(43);
    rb->deleteRB(98);
    if (rb->searchRB(98) != blankptr)
    {
        cout << "hello found" << endl;

    }
    else
    {
        cout << "not found" << endl;
    }
    if (rb->searchRB(95) != blankptr)
    {
        cout << "hello found" << endl;

    }
    else
    {
        cout << "not found" << endl;
    }

    cout << "67 after deletion" << rb->FindRank(67) << endl;



}
int main()
{
    srand(time(NULL));
    blank.c = Black;
    blank.data_ = 0;
    blank.key_ = 0;
    blank.left = &blank;
    blank.right = &blank;
    blank.parent = &blank;
    blank.size = 0;
    RBTree<int, int>* rb = new RBTree<int, int>();
    rb->root = blankptr;
    bool test = false;
    if (test)
    {
        testcode(rb);
    }

    char ch;
    cout << "Press I(Insert) D(Delete) R(FindRank) C(Ith Rank) S(Search) E(Exit)" << endl;
    while (cin >> ch)
    {
        switch (ch)
        {
        case 'I':
        case 'i':
        {
            int x = rand() % 100;
            //data can be stored if needed in second parameter
            rb->insertRB(x, x);
            cout << "Inserted" << endl;
        }
        break;
        case 'D':
        case 'd':
        {
            int x = rand() % 100;
            rb->deleteRB(x);
            cout << "Deleted" << endl;
        }
        break;
        case 'R':
        case 'r':
        {
            int x = rand() % 100;
            cout << rb->FindRank(x) << endl;
        }
        break;
        case 'S':
        case 's':
        {
            int x = rand() % 100;
            RBnode<int, int>* current = rb->searchRB(x);
            if (current != blankptr)
                cout << "Found it " << current->data_ << endl;
            else
                cout << "No Such Key Exists!!" << endl;
        }
        break;
        case 'C':
        case 'c':
        {
            int x = rand() % 100;
            RBnode<int, int>* current = rb->FindithRank(x);
            if (current != blankptr)
                cout << "Rank of Node is: " << current->key_ << endl;
            else
                cout << "No Such Rank Exists!!" << endl;
        }
        break;
        case 'E':
        case 'e':
        {
            exit(0);
        }
        break;
        default:
            exit(0);
            break;
        }


    }

}

#endif // 0
