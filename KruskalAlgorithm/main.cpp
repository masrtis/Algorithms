#define BOOST_TEST_MODULE KruskalAlgorithm
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <type_traits>

#include <memory>

#include "AdjacencyList.h"

BOOST_AUTO_TEST_CASE(MoveWeakPtr)
{
    std::shared_ptr<int> ptr(std::make_shared<int>(5));
    std::weak_ptr<int> weakptr(ptr);
    std::weak_ptr<int> movedWeakPtr(std::move(weakptr));

    BOOST_CHECK(weakptr.expired());
    BOOST_CHECK(!movedWeakPtr.expired());
}


BOOST_AUTO_TEST_CASE(EmptyGraph)
{
    const AdjacencyList graph;
    const auto mst(graph.kruskal());

    BOOST_CHECK_EQUAL(graph.getNumVertices(), 0u);
    BOOST_CHECK_EQUAL(graph.getNumEdges(), 0u);
}

BOOST_AUTO_TEST_CASE(MspCutWikipedia)
{
    AdjacencyList graph;

    const auto a = graph.addVertex(Vertex("A"));
    const auto b = graph.addVertex(Vertex("B"));
    const auto c = graph.addVertex(Vertex("C"));
    const auto d = graph.addVertex(Vertex("D"));
    const auto e = graph.addVertex(Vertex("E"));
    const auto f = graph.addVertex(Vertex("F"));

    typedef Edge<std::remove_const<decltype(a)>::type> Edge;

    graph.addEdge(a, b, 1);
    graph.addEdge(a, d, 3);
    graph.addEdge(b, c, 6);
    graph.addEdge(b, d, 5);
    graph.addEdge(b, e, 1);
    graph.addEdge(c, e, 5);
    graph.addEdge(c, f, 2);
    graph.addEdge(d, e, 1);
    graph.addEdge(e, f, 4);

    const auto mst(graph.kruskal());

    BOOST_CHECK_EQUAL(mst.getNumVertices(), 6u);
    BOOST_CHECK_EQUAL(mst.getNumEdges(), 5u);

    const auto mstA = mst.findVertex("A");
    BOOST_CHECK(mstA.isValid());
    
    const auto mstB = mst.findVertex("B");
    BOOST_CHECK(mstB.isValid());
    
    const auto mstC = mst.findVertex("C");
    BOOST_CHECK(mstC.isValid());
    
    const auto mstD = mst.findVertex("D");
    BOOST_CHECK(mstD.isValid());

    const auto mstE = mst.findVertex("E");
    BOOST_CHECK(mstE.isValid());

    const auto mstF = mst.findVertex("F");
    BOOST_CHECK(mstF.isValid());

    BOOST_CHECK(mst.findEdge("A", "B", 1).isValid());
    BOOST_CHECK(mst.findEdge("B", "E", 1).isValid());
    BOOST_CHECK(mst.findEdge("C", "F", 2).isValid());
    BOOST_CHECK(mst.findEdge("D", "E", 1).isValid());
    BOOST_CHECK(mst.findEdge("E", "F", 4).isValid());
}
