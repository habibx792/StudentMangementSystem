#define _HAS_STD_BYTE 0
#ifndef STDNODE_H
#define STDNODE_H

template <class T>
class StdNode
{
protected:
    T data;
    StdNode<T> *left;
    StdNode<T> *right;

public:
    StdNode(const T &d)
    {
        data = d;
        left = nullptr;
        right = nullptr;
    }

    // Getter and setter methods
    T getData() const { return data; }
    StdNode<T> *getLeft() const { return left; }
    StdNode<T> *getRight() const { return right; }

    void setData(const T &d) { data = d; }
    void setLeft(StdNode<T> *l) { left = l; }
    void setRight(StdNode<T> *r) { right = r; }
};

#endif // STDNODE_H