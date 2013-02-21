#define BOOST_TEST_MODULE ADMChapter4
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <algorithm>
#include <iterator>
#include <random>
#include "SortAlgorithms.h"

BOOST_AUTO_TEST_CASE(Problem4_37)
{
    std::vector<std::mt19937::result_type> testData;
    const size_t n = 100000;
    testData.reserve(n);

    auto dieRoll = std::bind(std::uniform_int_distribution<>(1, 32), std::discard_block_engine<std::mt19937, 256, 32>());
    std::generate_n(std::back_inserter(testData), n, dieRoll);

    {
        boost::timer::auto_cpu_timer t(3);
        std::sort(begin(testData), end(testData));
        std::cout << "Standard sort elapsed CPU time:";
    }
    
    std::mt19937 rng;
    std::shuffle(begin(testData), end(testData), rng);

    SelectionSort selectionSort;
    {
        boost::timer::auto_cpu_timer t(3);
        selectionSort.sort(begin(testData), end(testData));
        std::cout << "Selection sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    {
        boost::timer::auto_cpu_timer t(3);
        selectionSort.sort(begin(testData), end(testData), std::greater<std::mt19937::result_type>());
        std::cout << "Selection sort (>) elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData), std::greater<std::mt19937::result_type>()));

    HeapSort heapSort;
    {
        boost::timer::auto_cpu_timer t(3);
        heapSort.sort(begin(testData), end(testData));
        std::cout << "Heap sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    std::list<std::mt19937::result_type> testList(begin(testData), end(testData));
    {

        boost::timer::auto_cpu_timer t(3);
        heapSort.sort(begin(testList), end(testList));
        std::cout << "Heap sort (list) elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testList), end(testList)));

    std::shuffle(begin(testData), end(testData), rng);

    InsertionSort insertionSort;
    {
        boost::timer::auto_cpu_timer t(3);
        insertionSort.sort(begin(testData), end(testData));
        std::cout << "Insertion sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    QuickSort quickSort;
    {
        boost::timer::auto_cpu_timer t(3);
        quickSort.sort(begin(testData), end(testData));
        std::cout << "Quick sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));
}
