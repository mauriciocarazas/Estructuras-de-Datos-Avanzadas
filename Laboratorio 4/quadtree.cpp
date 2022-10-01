//Quadtree implementation for the 2D range search problem 

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

//Point class
class Point
{
public:
    int x;
    int y;
    Point(int x, int y) : x(x), y(y) {}
};

//Node class
class Node
{
public:
    Point *p;   //point
    Node *nw;   //northwest
    Node *ne;   //northeast 
    Node *sw;   //southwest
    Node *se;   //southeast
    Node(Point *p) : p(p), nw(NULL), ne(NULL), sw(NULL), se(NULL) {} 
};

//Quadtree class
class Quadtree
{
public:
    Node *root;
    Quadtree() : root(NULL) {} 
    void insert(Point *p);
    void rangeSearch(int x1, int y1, int x2, int y2);
    void rangeSearchHelper(Node *n, int x1, int y1, int x2, int y2);
};

//Insert a point into the quadtree
void Quadtree::insert(Point *p) 
{
    if (root == NULL) //if the tree is empty
    {
        root = new Node(p);
        return;
    }
    Node *n = root;
    while (true) //
    {
        if (p->x < n->p->x && p->y < n->p->y)
        {
            if (n->nw == NULL)
            {
                n->nw = new Node(p);
                return;
            }
            n = n->nw;
        }
        else if (p->x >= n->p->x && p->y < n->p->y)
        {
            if (n->ne == NULL)
            {
                n->ne = new Node(p);
                return;
            }
            n = n->ne;
        }
        else if (p->x < n->p->x && p->y >= n->p->y)
        {
            if (n->sw == NULL)
            {
                n->sw = new Node(p);
                return;
            }
            n = n->sw;
        }
        else if (p->x >= n->p->x && p->y >= n->p->y)
        {
            if (n->se == NULL)
            {
                n->se = new Node(p);
                return;
            }
            n = n->se;
        }
    }
}

//Range search
void Quadtree::rangeSearch(int x1, int y1, int x2, int y2)
{
    rangeSearchHelper(root, x1, y1, x2, y2);
}

//Range search helper
void Quadtree::rangeSearchHelper(Node *n, int x1, int y1, int x2, int y2)
{
    if (n == NULL)
        return;
    if (n->p->x >= x1 && n->p->x <= x2 && n->p->y >= y1 && n->p->y <= y2)
        cout << "(" << n->p->x << ", " << n->p->y << ")" << endl;
    rangeSearchHelper(n->nw, x1, y1, x2, y2);
    rangeSearchHelper(n->ne, x1, y1, x2, y2);
    rangeSearchHelper(n->sw, x1, y1, x2, y2);
    rangeSearchHelper(n->se, x1, y1, x2, y2);
}

//Print the quadtree
void printQuadtree(Node *n, int level)
{
    if (n == NULL)
        return;
    for (int i = 0; i < level; i++)
        cout << " ";
    cout << "(" << n->p->x << ", " << n->p->y << ")" << endl;
    printQuadtree(n->nw, level + 1);
    printQuadtree(n->ne, level + 1);
    printQuadtree(n->sw, level + 1);
    printQuadtree(n->se, level + 1);
}

//visualize the quadtree in a 2D plane 

void visualizeQuadtree(Node *n, int level)
{
    if (n == NULL)
        return;
    visualizeQuadtree(n->nw, level + 1);
    visualizeQuadtree(n->ne, level + 1);
    visualizeQuadtree(n->sw, level + 1);
    visualizeQuadtree(n->se, level + 1);
    for (int i = 0; i < level; i++)
        cout << " ";
    cout << "(" << n->p->x << ", " << n->p->y << ")" << endl;
}


//Main function
int main()
{
    Quadtree qt;
    qt.insert(new Point(1, 1));
    qt.insert(new Point(2, 2));
    qt.insert(new Point(8, 8));
    qt.insert(new Point(9, 9));
    qt.insert(new Point(10, 10));
    qt.insert(new Point(11, 11));
    qt.insert(new Point(12, 12));
    qt.insert(new Point(13, 13));
    qt.insert(new Point(14, 14));
    qt.insert(new Point(15, 15));
    qt.insert(new Point(16, 16));
    qt.insert(new Point(56, 56));
    qt.insert(new Point(17, 17));
    qt.insert(new Point(34, 34));
    qt.insert(new Point(41, 41));
    qt.insert(new Point(42, 42));
    qt.insert(new Point(43, 43));
    qt.insert(new Point(44, 44));

    cout << "Quadtree:" << endl;
    printQuadtree(qt.root, 0);
    cout << endl;

    cout << "Range search (1, 1) to (10, 10):" << endl;
    qt.rangeSearch(1, 1, 10, 10);
    cout << endl;

    cout << "Range search (1, 1) to (20, 20):" << endl;
    qt.rangeSearch(1, 1, 20, 20);
    cout << endl;

    //visualize the quadtree
    cout << "Visualize the quadtree:" << endl;
    visualizeQuadtree(qt.root, 0);
    cout << endl;


    return 0;
}
