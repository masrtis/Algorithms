#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <exception>
#include <sstream>

class UnionFind
{
public:
    explicit UnionFind(size_t numVerts)
    {
        m_components.reserve(numVerts);
        size_t parentIdx = 0;
        std::generate_n(std::back_inserter(m_components),
                        numVerts,
                        [&]() { return UnionFindNode(parentIdx++); });
    }

    bool sameComponent(size_t start, size_t end) const
    {
        return findRoot(start) == findRoot(end);
    }

    size_t findRoot(size_t elem) const
    {
        if (elem < m_components.size())
        {
            auto prevParent = elem;
            auto parentIdx = m_components[prevParent].parentIdx;

            while (parentIdx != prevParent)
            {
                prevParent = parentIdx;
                parentIdx = m_components[parentIdx].parentIdx;
            }

            return parentIdx;
        }

        std::ostringstream oss;
        oss << "Element index " << elem << " is out of range of components buffer (size " << m_components.size() << ").";
        throw std::out_of_range(oss.str());
    }

    void merge(size_t start, size_t end)
    {
        const auto startRoot(findRoot(start));
        const auto endRoot(findRoot(end));

        if (startRoot == endRoot)
        {
            return;
        }

        const auto startTreeSize = m_components[startRoot].subtreeSize;
        const auto endTreeSize   = m_components[endRoot].subtreeSize;

        if (startTreeSize < endTreeSize)
        {
            m_components[startRoot].parentIdx = m_components[endRoot].parentIdx;
        }
        else
        {
            m_components[endRoot].parentIdx = m_components[startRoot].parentIdx;
        }

        if (startTreeSize == endTreeSize)
        {
            m_components[startRoot].subtreeSize += 1;
        }
    }
private:
    struct UnionFindNode
    {
        explicit UnionFindNode(size_t parent)
            : parentIdx(parent)
            , subtreeSize(1)
        {

        }

        size_t parentIdx;
        size_t subtreeSize;
    };

    std::vector<UnionFindNode> m_components;
};
