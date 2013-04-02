#pragma once

#include <queue>
#include <functional>
#include <iterator>
#include <utility>
#include <stack>
#include <iostream>

namespace detail
{
    template <typename BiDirIt>
    void printRange(BiDirIt begin, BiDirIt end)
    {
        if (begin != end)
        {
            const It last(std::prev(end));
            std::cout << "{ ";
            std::copy(begin, last, std::ostream_iterator<typename BiDirIt::value_type>(std::cout, ", "));
            std::cout << *last;
            std::cout << " }";
        }
        else
        {
            std::cout << "{ }";
        }

        std::cout << "\n";
    }
    
    template <typename RandIt, typename Comparer>
    void heapSort(RandIt begin, RandIt end, Comparer compFunc, std::random_access_iterator_tag)
    {
        std::make_heap(begin, end, compFunc);
        std::sort_heap(begin, end, compFunc);
    }

    template <typename FwdIt, typename Comparer, typename IterCat>
    void heapSort(FwdIt begin, FwdIt end, Comparer compFunc, IterCat)
    {
        typedef typename FwdIt::value_type value_type;
        
        std::vector<value_type> randomAccessContainer(std::make_move_iterator(begin), std::make_move_iterator(end));
        heapSort(std::begin(randomAccessContainer), std::end(randomAccessContainer), compFunc, std::random_access_iterator_tag());
        std::move(std::begin(randomAccessContainer), std::end(randomAccessContainer), begin);
    }

    template <typename InIt>
    class QuicksortStack
    {
    private:
        typedef std::pair<InIt, InIt> IterRange;
        std::stack<IterRange> m_stack;
    public:
        void push(IterRange&& range)
        {
            if (std::distance(range.first, range.second) >= 2)
            {
                m_stack.push(std::move(range));
            }
        }

        void push(const IterRange& range)
        {
            if (std::distance(range.first, range.second) >= 2)
            {
                m_stack.push(range);
            }
        }

        void pop()
        {
            m_stack.pop();
        }

        IterRange top() const
        {
            return m_stack.top();
        }

        bool empty() const
        {
            return m_stack.empty();
        }
    };
}

template <typename FwdIt, typename Comparer>
void selectionSort(FwdIt begin, FwdIt end, Comparer compFunc)
{
    for (; begin != end; ++begin)
    {
        const auto minElem(std::min_element(begin, end, compFunc));

        if (begin != minElem)
        {
            std::iter_swap(begin, minElem);
        }
    }
}

template <typename FwdIt>
void selectionSort(FwdIt begin, FwdIt end)
{
    selectionSort(begin, end, std::less<typename FwdIt::value_type>());
}

template <typename FwdIt, typename Comparer>
void heapSort(FwdIt begin, FwdIt end, Comparer compFunc)
{
    detail::heapSort(begin, end, compFunc, typename std::iterator_traits<FwdIt>::iterator_category());
}

template <typename FwdIt>
void heapSort(FwdIt begin, FwdIt end)
{
    heapSort(begin, end, std::less<typename FwdIt::value_type>());
}

template <typename FwdIt, typename Comparer>
void insertionSort(FwdIt begin, FwdIt end, Comparer compFunc)
{
    for (auto elem = begin; elem != end; ++elem)
    {
        const auto current(std::move(*elem));
        const auto sortPosition(std::upper_bound(begin, elem, current, compFunc));
        
        std::move_backward(sortPosition, elem, std::next(elem));
        
        *sortPosition = std::move(current);
    }
}

template <typename FwdIt>
void insertionSort(FwdIt begin, FwdIt end)
{
    insertionSort(begin, end, std::less<typename FwdIt::value_type>());
}

template <typename BiDirIt, typename Comparer>
void quickSort(BiDirIt begin, BiDirIt end, Comparer compFunc)
{
    detail::QuicksortStack<BiDirIt> ranges;
    ranges.push(std::make_pair(begin, end));

    while (!ranges.empty())
    {
        const auto current(ranges.top());
        ranges.pop();

        const auto last(std::prev(current.second));
        const auto pivot(std::partition(current.first, last, [=](const typename BiDirIt::value_type& val){ return compFunc(val, *last); }));
        std::iter_swap(pivot, last);

        ranges.push(std::make_pair(current.first, pivot));

        if (pivot != current.second)
        {
            ranges.push(std::make_pair(std::next(pivot), current.second));
        }
    }
}

template <typename BiDirIt>
void quickSort(BiDirIt begin, BiDirIt end)
{
    quickSort(begin, end, std::less<typename BiDirIt::value_type>());
}
