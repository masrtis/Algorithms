#pragma once

#include <queue>
#include <functional>
#include <iterator>
#include <utility>
#include <stack>

namespace detail
{
    template <typename It>
    void printRange(It begin, It end)
    {
        if (begin != end)
        {
            const It last(detail::advance(end, -1));
            std::cout << "{ ";
            std::copy(begin, last, std::ostream_iterator<It::value_type>(std::cout, ", "));
            std::cout << *last;
            std::cout << " }";
        }
        else
        {
            std::cout << "{ }";
        }

        std::cout << "\n";
    }

    template <typename It, typename Comp, typename IterCat>
    void heapSort(It begin, It end, Comp compFunc, IterCat)
    {
        typedef typename It::value_type value_type;
        
        const auto predicate = [=](const typename Comp::second_argument_type& lhs, const typename Comp::first_argument_type& rhs)
        {
            return compFunc(rhs, lhs);
        };

        std::priority_queue<value_type, std::vector<value_type>, decltype(predicate)> heap(begin, end, predicate);
        std::for_each(begin, end, [&](value_type& elem) { elem = heap.top(); heap.pop(); });
    }

    template <typename It, typename Comp>
    void heapSort(It begin, It end, Comp compFunc, std::random_access_iterator_tag)
    {
        std::make_heap(begin, end, compFunc);
        std::sort_heap(begin, end, compFunc);
    }
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

struct HeapSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        detail::heapSort(begin, end, compFunc, std::iterator_traits<It>::iterator_category());
    }
};

struct InsertionSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        It elem(std::next(begin));
        It nextElem(std::next(elem));
        for (; nextElem != end; ++nextElem)
        {
            std::inplace_merge(begin, elem, nextElem, compFunc);
            elem = nextElem;
        }

        std::inplace_merge(begin, elem, nextElem, compFunc);
    }
};

struct QuickSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        typedef std::pair<It, It> IterRange;
        std::stack<IterRange> ranges;
        ranges.push(std::make_pair(begin, end));

        do
        {
            const IterRange current(ranges.top());
            ranges.pop();

            const It last(std::prev(current.second));
            const It pivot(std::partition(current.first, last, [=](const It::value_type& val){ return compFunc(val, *last); }));
            std::iter_swap(pivot, last);

            if (std::distance(current.first, pivot) >= 2)
            {
                ranges.push(std::make_pair(current.first, pivot));
            }

            if (std::distance(pivot, current.second) >= 3)
            {
                ranges.push(std::make_pair(std::next(pivot), current.second));
            }
        } while (!ranges.empty());
    }
};

typedef Sorter<SelectionSortAlgorithm> SelectionSort;
typedef Sorter<HeapSortAlgorithm> HeapSort;
typedef Sorter<InsertionSortAlgorithm> InsertionSort;
typedef Sorter<QuickSortAlgorithm> QuickSort;
