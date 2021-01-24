#include <stdlib.h>

template <typename T>
struct Priority_Queue
{
private:
    T *data;
    size_t rear;
    size_t capacity;
    int (*cmp)(const T &a, const T &b);

public:
    static void init(struct Priority_Queue &q, size_t capacity, int (*cmp)(const T &a, const T &b));
    static void destroy(struct Priority_Queue &q);
    static bool enqueue(struct Priority_Queue &q, const T &e);
    static bool dequeue(struct Priority_Queue &q, T &e);
    static bool top(const struct Priority_Queue &q, T &e);
    static size_t length(const struct Priority_Queue &q);
    static bool empty(const struct Priority_Queue &q);
    static bool full(const struct Priority_Queue &q);
};

template <typename T>
void Priority_Queue<T>::init(struct Priority_Queue<T> &q, size_t capacity, int (*cmp)(const T &a, const T &b))
{
    q.capacity = capacity;
    q.data = (T*)malloc((capacity + 1) * sizeof(T));
    if(!q.data) exit(0);
    q.rear = 1;
    q.cmp = cmp;
    // TODO
}

template <typename T>
void Priority_Queue<T>::destroy
(struct Priority_Queue<T> &q)
{
    free(q.data);
    q.data = NULL;
    q.rear = 1;
    // TODO
}

template <typename T>
bool Priority_Queue<T>::enqueue(struct Priority_Queue<T> &q, const T &e)
{
    if(q.rear == q.capacity + 1) return false;
    q.data[q.rear] = e;
    T elem;
    unsigned int i, j;
    i = j = 1;
    while(j < q.rear){
        for(;i < q.rear;i++){
            if(q.cmp(q.data[i],q.data[i+1]) >= 0){
               elem = q.data[i];
               q.data[i] = q.data[i+1];
               q.data[i+1] = elem;
            }
        }
        j++;
        i = 1;
    }
    q.rear++;
    return true;
    // TODO
}

template <typename T>
bool Priority_Queue<T>::dequeue(struct Priority_Queue<T> &q, T &e)
{
    if(q.rear == 1) return false;
    e = q.data[1];
    q.rear--;
    unsigned int i = 1;
    for(;i < q.rear;i++){
        q.data[i] = q.data[i+1];
    }
    return true;
    // TODO
}

template <typename T>
bool Priority_Queue<T>::top(const struct Priority_Queue<T> &q, T &e)
{
    if (empty(q))
        return false;

    e = q.data[1];
    return true;
}

template <typename T>
size_t Priority_Queue<T>::length(const struct Priority_Queue<T> &q)
{
    return q.rear - 1;
    // TODO
}

template <typename T>
bool Priority_Queue<T>::empty(const struct Priority_Queue<T> &q)
{
    if(q.rear == 1) return true;
    else return false;
    // TODO
}

template <typename T>
bool Priority_Queue<T>::full(const struct Priority_Queue<T> &q)
{
    if(q.rear == q.capacity +1) return true;
    else return false;
    // TODO
}
