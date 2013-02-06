#pragma once

#include <queue>
#include <functional>
#include <iterator>

template <typename BinFn>
class BinarySwap : public std::binary_function<typename BinFn::second_argument_type,
                                               typename BinFn::first_argument_type,
                                               bool>
{
public:
    explicit BinarySwap(BinFn fn)
        : m_fn(fn)
    {

    }

    bool operator()(const typename BinFn::second_argument_type& lhs,
                    const typename BinFn::first_argument_type& rhs) const
    {
        return m_fn(rhs, lhs);
    }
private:
    BinFn m_fn;
};

template <typename BinFn>
BinarySwap<BinFn> makeSwap(BinFn fn)
{
    return BinarySwap<BinFn>(fn);
}

template <typename SortAlgorithm>
class Sorter
{
public:
    // Operates on STL iterators to sort a range of elements in any container
    // using operator< as comparison function
    template <typename It>
    void sort(It begin, It end) const
    {
        sort(begin, end, std::less<typename It::value_type>());
    }

    // Operates on STL iterators to sort a range of elements in any container
    // using compFunc as comparison function
    template <typename It, typename Comp>
    void sort(It begin, It end, Comp compFunc) const
    {
        m_algorithm(begin, end, compFunc);
    }
private:
    SortAlgorithm m_algorithm;
};

struct HeapSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        typedef typename It::value_type value_type;
        std::priority_queue<value_type, std::vector<value_type>, decltype(makeSwap(compFunc))> heap(begin, end, makeSwap(compFunc));
        std::for_each(begin, end, [&](value_type& elem) { elem = heap.top(); heap.pop(); });
    }
};

struct SelectionSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        for (; begin != end; ++begin)
        {
            const It minElem(std::min_element(begin, end, compFunc));
            
            if (begin != minElem)
            {
                std::iter_swap(begin, minElem);
            }
        }
    }
};

typedef Sorter<HeapSortAlgorithm> HeapSort;
typedef Sorter<SelectionSortAlgorithm> SelectionSort;
