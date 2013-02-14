#pragma once

#include <queue>
#include <functional>
#include <iterator>
#include <utility>
#include <stack>

namespace detail
{
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

    template <typename It, typename Distance>
    It advance(It iter, Distance n)
    {
        std::advance(iter, n);
        return iter;
    }

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
        std::priority_queue<value_type, std::vector<value_type>, decltype(makeSwap(compFunc))> heap(begin, end, makeSwap(compFunc));
        std::for_each(begin, end, [&](value_type& elem) { elem = heap.top(); heap.pop(); });
    }

    template <typename It, typename Comp>
    void heapSort(It begin, It end, Comp compFunc, std::random_access_iterator_tag)
    {
        std::make_heap(begin, end, compFunc);
        std::sort_heap(begin, end, compFunc);
    }
}

template <typename BinFn>
detail::BinarySwap<BinFn> makeSwap(BinFn fn)
{
    return detail::BinarySwap<BinFn>(fn);
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
        detail::heapSort(begin, end, compFunc, std::iterator_traits<It>::iterator_category());
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

struct InsertionSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        It elem(detail::advance(begin, 1));
        It nextElem(detail::advance(elem, 1));
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
            
            if (std::distance(current.first, current.second) < 2)
            {
                continue;
            }

            const It last(detail::advance(current.second, -1));
            const It pivot(std::partition(current.first, last, std::bind2nd(compFunc, *last)));
            std::iter_swap(pivot, last);

            if (pivot != current.first)
            {
                ranges.push(std::make_pair(current.first, pivot));
            }

            if (pivot != current.second)
            {
                ranges.push(std::make_pair(detail::advance(pivot, 1), current.second));
            }
        } while (!ranges.empty());
    }
};

typedef Sorter<HeapSortAlgorithm> HeapSort;
typedef Sorter<SelectionSortAlgorithm> SelectionSort;
typedef Sorter<InsertionSortAlgorithm> InsertionSort;
typedef Sorter<QuickSortAlgorithm> QuickSort;
