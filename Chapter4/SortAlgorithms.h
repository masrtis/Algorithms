#pragma once

#include <queue>
#include <functional>
#include <iterator>
#include <utility>
#include <stack>
#include <iostream>

namespace detail
{
    template <typename It>
    void printRange(It begin, It end)
    {
        if (begin != end)
        {
            const It last(std::prev(end, 1));
            std::cout << "{ ";
            std::copy(begin, last, std::ostream_iterator<typename It::value_type>(std::cout, ", "));
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

    template <typename It>
    class QuicksortStack
    {
    private:
        typedef std::pair<It, It> IterRange;
        std::stack<IterRange> m_stack;
    public:
        void push(IterRange&& range)
        {
            if (std::distance(range.first, range.second) >= 2)
            {
                m_stack.push(std::forward<IterRange>(range));
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

template <typename It, typename Comp>
void selectionSort(It begin, It end, Comp compFunc)
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

template <typename It>
void selectionSort(It begin, It end)
{
    selectionSort(begin, end, std::less<typename It::value_type>());
}

template <typename It, typename Comp>
void heapSort(It begin, It end, Comp compFunc)
{
    detail::heapSort(begin, end, compFunc, std::iterator_traits<It>::iterator_category());
}

template <typename It>
void heapSort(It begin, It end)
{
    heapSort(begin, end, std::less<typename It::value_type>());
}

template <typename It, typename Comp>
void insertionSort(It begin, It end, Comp compFunc)
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

template <typename It>
void insertionSort(It begin, It end)
{
    insertionSort(begin, end, std::less<typename It::value_type>());
}

template <typename It, typename Comp>
void quickSort(It begin, It end, Comp compFunc)
{
    detail::QuicksortStack<It> ranges;
    ranges.push(std::make_pair(begin, end));

    while (!ranges.empty())
    {
        const auto current(ranges.top());
        ranges.pop();

        const It last(std::prev(current.second));
        const It pivot(std::partition(current.first, last, [=](const typename It::value_type& val){ return compFunc(val, *last); }));
        std::iter_swap(pivot, last);

        ranges.push(std::make_pair(current.first, pivot));

        if (pivot != current.second)
        {
            ranges.push(std::make_pair(std::next(pivot), current.second));
        }
    }
}

template <typename It>
void quickSort(It begin, It end)
{
    quickSort(begin, end, std::less<typename It::value_type>());
}
