#ifndef CUSTOMQUEUE_H
#define CUSTOMQUEUE_H

#include <queue>


struct queueParams
{
    int id;
    std::string name = "queue";
    size_t size;
    bool isFull;
    bool isEmpty;
};


template<typename T, class Container = std::deque<T>>
class customQueue : public std::queue<T,Container>
{
public:
    void push(T&& value);
    void pop();

    template<typename... Args>
    void emplace(Args&&... args);

    void swap(customQueue& other) noexcept;

    queueParams _params;
};


template<typename T, class Container>
void customQueue<T,Container>::push(T&& value)
{
    std::queue<T,Container>::push( std::forward(value) );
    ++_params.size;
    _params.isEmpty = false;
}

template<typename T, class Container>
void customQueue<T,Container>::pop()
{
    std::queue<T,Container>::pop();
    --_params.size;
    _params.isFull = false;

    if (_params.size == 0)
    {
        _params.isEmpty = true;
    }
}

template<typename T, class Container>
template<typename... Args>
void customQueue<T,Container>::emplace(Args &&... args)
{
    std::queue<T,Container>::emplace( std::forward<Args>(args)... );
    ++_params.size;
    _params.isEmpty = false;
}

template<typename T, class Container>
void customQueue<T,Container>::swap(customQueue &other) noexcept
{
    std::swap(_params, other._params);
    std::queue<T,Container>::swap(other);
}

#endif //CUSTOMQUEUE_H
