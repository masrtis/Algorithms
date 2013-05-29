#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <string>

#include "FwdDecl.h"
#include "ItemIndexPair.h"
#include "Vertex.h"
#include "Edge.h"
#include "Kruskal.h"

class AdjacencyList
{
public:
    typedef Vertex vertex_type;
    typedef std::string vertex_id_type;
    typedef std::vector<std::shared_ptr<vertex_type>> vertex_container;
    typedef ItemIndexPair<vertex_type> vertex_pointer;
    
    typedef Edge<vertex_pointer> edge_type;
    typedef std::vector<std::shared_ptr<edge_type>> edge_container;
    typedef ItemIndexPair<edge_type> edge_pointer;

    size_t getNumVertices() const
    {
        return m_vertices.size();
    }

    size_t getNumEdges() const
    {
        return m_edges.size();
    }

    AdjacencyList kruskal() const
    {
        return ::kruskal<AdjacencyList>(m_edges, m_vertices.size());
    }

    vertex_pointer findVertex(const vertex_type& v) const
    {
        return findVertex(v.getId());
    }

    vertex_pointer findVertex(vertex_id_type id) const
    {
        const auto vertexIt(std::find_if(begin(m_vertices), 
                                         end(m_vertices), 
                                         [&id](const std::shared_ptr<vertex_type>& v) { return v->getId() == id; } ));
        
        vertex_pointer result(std::shared_ptr<vertex_type>(nullptr), 0);

        if (vertexIt != end(m_vertices))
        {
            result = vertex_pointer(*vertexIt, std::distance(begin(m_vertices), vertexIt));
        }

        return result;
    }

    edge_pointer findEdge(const vertex_pointer& startVertex, const vertex_pointer& endVertex, size_t weight) const
    {
        return findEdge(startVertex->getId(), endVertex->getId(), weight);
    }

    edge_pointer findEdge(vertex_id_type startId, vertex_id_type endId, size_t weight) const
    {
        const auto edgeIt(std::find_if(begin(m_edges),
                                       end(m_edges),
                                       [&startId, &endId, weight](const std::shared_ptr<edge_type>& e)
                                       { return weight  == e->getWeight()      &&
                                                startId == e->getStart()->getId() &&
                                                endId   == e->getEnd()->getId();
                                       } ));

        edge_pointer result(std::shared_ptr<edge_type>(nullptr), 0);

        if (edgeIt != end(m_edges))
        {
            result = edge_pointer(*edgeIt, std::distance(begin(m_edges), edgeIt));
        }

        return result;
    }

    vertex_pointer addVertex(vertex_type v)
    {
        auto existingVertex(findVertex(v));

        if (!existingVertex.isValid())
        {
            m_vertices.push_back(std::make_shared<vertex_type>(std::move(v)));
            existingVertex = vertex_pointer(m_vertices.back(), m_vertices.size() - 1);
        }

        return existingVertex;
    }

    edge_pointer addEdge(vertex_pointer start, vertex_pointer end, size_t weight)
    {
        auto existingEdge(findEdge(start, end, weight));

        if (!existingEdge.isValid())
        {
            m_edges.push_back(std::make_shared<edge_type>(std::move(start), std::move(end), weight));
            existingEdge = edge_pointer(m_edges.back(), m_edges.size() - 1);
        }

        return existingEdge;
    }
private:
    vertex_container    m_vertices;
    edge_container      m_edges;
};
