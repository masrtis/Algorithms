#pragma once

#include <queue>
#include <functional>
#include <iterator>

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
        std::priority_queue<value_type, std::vector<value_type>, decltype(std::not2(compFunc))> heap(begin, end, std::not2(compFunc));

        for (; begin != end; ++begin)
        {
            *begin = heap.top();
            heap.pop();
        }
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
