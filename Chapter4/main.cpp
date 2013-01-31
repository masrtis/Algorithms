#define BOOST_TEST_MODULE ADMChapter4
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <algorithm>
#include <iterator>
#include <random>

#include "SortAlgorithms.h"

BOOST_AUTO_TEST_CASE(Problem4_37)
{
    HeapSort heapSort;
    SelectionSort selectionSort;

    std::vector<std::mt19937::result_type> testData;
    const size_t n = 1000000;
    testData.reserve(n);

    std::mt19937 rng;
    std::generate_n(std::back_inserter(testData), n, rng);

    {
        boost::timer::auto_cpu_timer t(3);
        heapSort.sort(begin(testData), end(testData));
        std::cout << "Heap sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);
    
    {
        boost::timer::auto_cpu_timer t(3);
        selectionSort.sort(begin(testData), end(testData));
        std::cout << "Selection sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));
}
