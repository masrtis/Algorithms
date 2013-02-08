#pragma once

#include <queue>
#include <functional>
#include <iterator>

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

struct InsertionSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        const std::reverse_iterator<It> rEnd(begin);
        for (++begin; begin != end; ++begin)
        {
            It elem(begin);
            std::reverse_iterator<It> prevElem(elem);
            
            for (; prevElem != rEnd; ++prevElem)
            {
                if (compFunc(*elem, *prevElem))
                {
                    std::iter_swap(elem, prevElem);
                    elem = prevElem.base();
                    --elem;
                }
            }
        }
    }
};

struct QuickSortAlgorithm
{
    template <typename It, typename Comp>
    void operator()(It begin, It end, Comp compFunc) const
    {
        //detail::printRange(begin, end);

        if (std::distance(begin, end) < 2)
        {
            //std::cout << "Sorted range: ";
            //detail::printRange(begin, end);
            return;
        }

        const It pivot(std::partition(begin, detail::advance(end, -1), std::bind2nd(compFunc, *detail::advance(end, -1))));
        std::iter_swap(pivot, detail::advance(end, -1));

        //std::cout << "First half: ";
        //detail::printRange(begin, pivot);

        //std::cout << "Second half: ";
        //detail::printRange(pivot, end);

        if (pivot != begin)
        {
            (*this)(begin, pivot, compFunc);
        }

        if (pivot != end)
        {
            (*this)(detail::advance(pivot, 1), end, compFunc);
        }

        //std::cout << "Sorted range: ";
        //detail::printRange(begin, end);
    }
};

typedef Sorter<HeapSortAlgorithm> HeapSort;
typedef Sorter<SelectionSortAlgorithm> SelectionSort;
typedef Sorter<InsertionSortAlgorithm> InsertionSort;
typedef Sorter<QuickSortAlgorithm> QuickSort;
