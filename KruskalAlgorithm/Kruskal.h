#pragma once

#include "Kruskal.h"
#include "AdjacencyList.h"
#include "UnionFind.h"

template <typename GraphType, typename EdgeSequence>
GraphType kruskal(EdgeSequence edges, size_t numVertices)
{
    std::sort(
        begin(edges), 
        end(edges),
        [](const typename EdgeSequence::value_type& lhs,
            const typename EdgeSequence::value_type& rhs)
        {
            return *lhs < *rhs;
        });

    UnionFind components(numVertices);
    GraphType minimumSpanningTree;

    for (const auto edgePtr : edges)
    {
        const auto* edge = edgePtr.get();

        const auto startVertex(edge->getStart());
        const auto endVertex(edge->getEnd());

        const auto startIndex = startVertex.getIndex();
        const auto endIndex = endVertex.getIndex();

        if (!components.sameComponent(startIndex, endIndex))
        {
            const auto mstStartVertex(minimumSpanningTree.addVertex(*startVertex));
            const auto mstEndVertex(minimumSpanningTree.addVertex(*endVertex));
            minimumSpanningTree.addEdge(mstStartVertex, mstEndVertex, edge->getWeight());

            components.merge(startIndex, endIndex);
        }
    }

    return minimumSpanningTree;
}
