#pragma once

#include <memory>
#include <exception>

template <typename T>
class ItemIndexPair
{
public:
    ItemIndexPair(const std::shared_ptr<T>& ptr, size_t index)
        : m_itemIndex(std::make_pair(std::weak_ptr<T>(ptr), index))
    {

    }

    bool isValid() const
    {
        return !m_itemIndex.first.expired();
    }

    size_t getIndex() const
    {
        if (isValid())
        {
            return m_itemIndex.second;
        }

        throw std::logic_error("Attempt to get index from invalid handle.");
    }

    T* get() const
    {
        const auto ptr(m_itemIndex.first.lock());
        return ptr.get();
    }

    T& operator*() const
    {
        auto* ptr(get());
        if (ptr)
        {
            return *ptr;
        }

        throw std::logic_error("Attempt to dereference invalid handle.");
    }

    T* operator->() const
    {
        return get();
    }
private:
    std::pair<std::weak_ptr<T>, size_t> m_itemIndex;
};
