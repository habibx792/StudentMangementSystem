#ifndef STDNODE_H
#define STDNODE_H

template <class T>
class StdNode
{
protected:
    T data;
    StdNode<T>* left;
    StdNode<T>* right;

public:
    // Constructor (your style)
    StdNode(const T& d)
    {
        data = d;
        left = NULL;
        right = NULL;
    }

    // ===== Getters =====
    T getData() const
    {
        return data;
    }

    StdNode<T>* getLeft() const
    {
        return left;
    }

    StdNode<T>* getRight() const
    {
        return right;
    }

    // ===== Setters =====
    void setData(const T& d)
    {
        data = d;
    }

    void setLeft(StdNode<T>* node)
    {
        left = node;
    }

    void setRight(StdNode<T>* node)
    {
        right = node;
    }
};

#endif
