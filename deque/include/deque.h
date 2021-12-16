#pragma once
#include <vector>
#include <stdexcept>

template <typename T>
class Deque
{
public:
    Deque() = default;
    ~Deque() = default;

    bool Empty() const;
    size_t Size() const;

    T &operator[](size_t index);
    const T &operator[](size_t index) const;

    T &At(size_t index);
    const T &At(size_t index) const;

    T &Front();
    const T &Front() const;

    T &Back();
    const T &Back() const;

    void PushBack(const T &value);
    void PushFront(const T &value);

private:
    std::vector<T> m_pushFront{};
    std::vector<T> m_pushBack{};
};

template <typename T>
bool Deque<T>::Empty() const
{
    return (m_pushBack.empty() && m_pushFront.empty());
}

template <typename T>
size_t Deque<T>::Size() const
{
    return (m_pushBack.size() + m_pushFront.size());
}

template <typename T>
T &Deque<T>::operator[](size_t index)
{
    return const_cast<T &>(static_cast<const Deque &>(*this)[index]);
}

template <typename T>
const T &Deque<T>::operator[](size_t index) const
{
    if (index < m_pushFront.size())
        return m_pushFront[(m_pushFront.size() - index - 1)];

    index -= m_pushFront.size();

    return m_pushBack[index];
}

template <typename T>
T &Deque<T>::At(size_t index)
{
    return const_cast<T &>(static_cast<const Deque &>(*this).At(index));
}

template <typename T>
const T &Deque<T>::At(size_t index) const
{
    if (index < m_pushFront.size())
        return m_pushFront[m_pushFront.size() - index - 1];

    index -= m_pushFront.size();

    if ((index + 1) > m_pushBack.size())
        throw std::out_of_range("");

    return m_pushBack[index];
}

template <typename T>
T &Deque<T>::Front()
{
    return m_pushFront.empty() ? m_pushBack.front() : m_pushFront.back();
}

template <typename T>
const T &Deque<T>::Front() const
{
    return m_pushFront.empty() ? m_pushBack.front() : m_pushFront.back();
}

template <typename T>
T &Deque<T>::Back()
{
    return m_pushBack.empty() ? m_pushFront.front() : m_pushBack.back();
}

template <typename T>
const T &Deque<T>::Back() const
{
    return m_pushBack.empty() ? m_pushFront.front() : m_pushBack.back();
}

template <typename T>
void Deque<T>::PushBack(const T &value)
{
    m_pushBack.push_back(value);
}

template <typename T>
void Deque<T>::PushFront(const T &value)
{
    m_pushFront.push_back(value);
}
