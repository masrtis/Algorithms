#define BOOST_TEST_MODULE ADMChapter4
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <algorithm>
#include <iterator>
#include <random>
#include "SortAlgorithms.h"

template <typename RandFunc>
std::string generateRandomString(RandFunc rng, std::string::size_type size)
{
    std::string result;
    result.reserve(size);
    std::generate_n(std::back_inserter(result), size, [&](){ return static_cast<std::string::value_type>(rng()); });
    return result;
}

BOOST_AUTO_TEST_CASE(SortRandomStrings)
{
    std::vector<std::string> testData;
    const size_t n = 1000000;
    testData.reserve(n);

    auto charGenerator(std::bind(std::uniform_int_distribution<>(1, 255), std::mt19937()));
    auto strSizeGenerator(std::bind(std::uniform_int_distribution<>(128, 1024), std::mt19937()));
    std::generate_n(std::back_inserter(testData), n, [&](){ return generateRandomString(charGenerator, strSizeGenerator()); });

    {
        boost::timer::auto_cpu_timer t(3);
        std::sort(begin(testData), end(testData));
        std::cout << "Standard sort elapsed CPU time:";
    }

    std::mt19937 rng;
    std::shuffle(begin(testData), end(testData), rng);

    //{
    //    boost::timer::auto_cpu_timer t(3);
    //    selectionSort(begin(testData), end(testData));
    //    std::cout << "Selection sort elapsed CPU time:";
    //}
    //BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    //{
    //    boost::timer::auto_cpu_timer t(3);
    //    selectionSort(begin(testData), end(testData), std::greater<std::string>());
    //    std::cout << "Selection sort (>) elapsed CPU time:";
    //}
    //BOOST_CHECK(std::is_sorted(begin(testData), end(testData), std::greater<std::string>()));

    std::list<std::string> testList(begin(testData), end(testData));
    {
        boost::timer::auto_cpu_timer t(3);
        heapSort(begin(testData), end(testData));
        std::cout << "Heap sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    {
        boost::timer::auto_cpu_timer t(3);
        heapSort(begin(testList), end(testList));
        std::cout << "Heap sort (list) elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testList), end(testList)));

    std::shuffle(begin(testData), end(testData), rng);

    //{
    //    boost::timer::auto_cpu_timer t(3);
    //    insertionSort(begin(testData), end(testData));
    //    std::cout << "Insertion sort elapsed CPU time:";
    //}
    //BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    {
        boost::timer::auto_cpu_timer t(3);
        quickSort(begin(testData), end(testData));
        std::cout << "Quick sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));
}

BOOST_AUTO_TEST_CASE(SortRandomDieRolls)
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

    {
        boost::timer::auto_cpu_timer t(3);
        selectionSort(begin(testData), end(testData));
        std::cout << "Selection sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    {
        boost::timer::auto_cpu_timer t(3);
        selectionSort(begin(testData), end(testData), std::greater<std::mt19937::result_type>());
        std::cout << "Selection sort (>) elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData), std::greater<std::mt19937::result_type>()));

    {
        boost::timer::auto_cpu_timer t(3);
        heapSort(begin(testData), end(testData));
        std::cout << "Heap sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    std::list<std::mt19937::result_type> testList(begin(testData), end(testData));
    {

        boost::timer::auto_cpu_timer t(3);
        heapSort(begin(testList), end(testList));
        std::cout << "Heap sort (list) elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testList), end(testList)));

    std::shuffle(begin(testData), end(testData), rng);

    {
        boost::timer::auto_cpu_timer t(3);
        insertionSort(begin(testData), end(testData));
        std::cout << "Insertion sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::shuffle(begin(testData), end(testData), rng);

    {
        boost::timer::auto_cpu_timer t(3);
        quickSort(begin(testData), end(testData));
        std::cout << "Quick sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::vector<int> emptyContainer;
    quickSort(begin(emptyContainer), end(emptyContainer));
}
