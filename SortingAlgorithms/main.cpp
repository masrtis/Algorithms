#define BOOST_TEST_MODULE SortAlgorithms
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <algorithm>
#include <iterator>
#include <random>
#include "SortAlgorithms.h"

template <typename RandFunc>
std::string generateRandomString(RandFunc& rng, std::string::size_type size)
{
    std::string result;
    result.reserve(size);
    std::generate_n(std::back_inserter(result), size, [&](){ return static_cast<std::string::value_type>(rng()); });
    return result;
}

template <typename SortType, typename GenerateFunc>
void runTestCase(size_t n, GenerateFunc generator, bool runNSquaredTests)
{
    std::vector<SortType> randomValues;
    randomValues.reserve(n);
    std::generate_n(std::back_inserter(randomValues), n, generator);

    std::vector<SortType> testData(begin(randomValues), end(randomValues));

    {
        boost::timer::auto_cpu_timer t(3);
        std::sort(begin(testData), end(testData));
        std::cout << "Standard sort elapsed CPU time:";
    }

    testData = randomValues;

    {
        boost::timer::auto_cpu_timer t(3);
        heapSort(begin(testData), end(testData));
        std::cout << "Heap sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    std::list<SortType> testList(begin(randomValues), end(randomValues));
    {
        boost::timer::auto_cpu_timer t(3);
        heapSort(begin(testList), end(testList));
        std::cout << "Heap sort (list) elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testList), end(testList)));

    testData = randomValues;

    {
        boost::timer::auto_cpu_timer t(3);
        quickSort(begin(testData), end(testData));
        std::cout << "Quick sort elapsed CPU time:";
    }
    BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

    if (runNSquaredTests)
    {   
        testData = randomValues;
        
        {
            boost::timer::auto_cpu_timer t(3);
            selectionSort(begin(testData), end(testData));
            std::cout << "Selection sort elapsed CPU time:";
        }
        BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

        testData = randomValues;

        {
            boost::timer::auto_cpu_timer t(3);
            selectionSort(begin(testData), end(testData), std::greater<SortType>());
            std::cout << "Selection sort (>) elapsed CPU time:";
        }
        BOOST_CHECK(std::is_sorted(begin(testData), end(testData), std::greater<SortType>()));

        testData = randomValues;

        {
            boost::timer::auto_cpu_timer t(3);
            insertionSort(begin(testData), end(testData));
            std::cout << "Insertion sort elapsed CPU time:";
        }
        BOOST_CHECK(std::is_sorted(begin(testData), end(testData)));

        testData = randomValues;
    }
}

BOOST_AUTO_TEST_CASE(SortRandomStrings)
{
    auto charGenerator(std::bind(std::uniform_int_distribution<>(1, 255), std::mt19937()));
    auto strSizeGenerator(std::bind(std::uniform_int_distribution<>(128, 512), std::mt19937()));
    
    std::cout << "Sort random strings:" << std::endl;
    runTestCase<std::string>(10000, [&]{ return generateRandomString(charGenerator, strSizeGenerator()); }, true);
    std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(SortRandomDieRolls)
{
    auto dieRoll = std::bind(std::uniform_int_distribution<>(1, 32), std::discard_block_engine<std::mt19937, 256, 32>());
    
    std::cout << "Sort random die rolls:" << std::endl;
    runTestCase<std::mt19937::result_type>(100000, dieRoll, true);
    std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(SortPointerIteratorCompilation)
{
    int data[] = {0};

    selectionSort(std::begin(data), std::end(data));
    heapSort(std::begin(data), std::end(data));
    insertionSort(std::begin(data), std::end(data));
    quickSort(std::begin(data), std::end(data));

    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(EmptySort)
{
    std::cout << "Testing empty container sort..." << std::endl;

    std::vector<int> emptyContainer;
    BOOST_CHECK_NO_THROW(selectionSort(begin(emptyContainer), end(emptyContainer)));
    BOOST_CHECK_NO_THROW(heapSort(begin(emptyContainer), end(emptyContainer)));
    BOOST_CHECK_NO_THROW(insertionSort(begin(emptyContainer), end(emptyContainer)));
    BOOST_CHECK_NO_THROW(quickSort(begin(emptyContainer), end(emptyContainer)));

    std::list<int> emptyList;
    BOOST_CHECK_NO_THROW(heapSort(begin(emptyContainer), end(emptyContainer)));
}
