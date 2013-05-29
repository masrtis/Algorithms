#pragma once

template <typename VertexPointer>
class Edge
{
public:
    Edge(VertexPointer start, VertexPointer end, size_t weight)
        : m_start(std::move(start))
        , m_end(std::move(end))
        , m_weight(weight)
    {

    }

    VertexPointer getStart() const
    {
        return m_start;
    }

    VertexPointer getEnd() const
    {
        return m_end;
    }

    size_t getWeight() const
    {
        return m_weight;
    }
private:
    VertexPointer m_start;
    VertexPointer m_end;

    size_t m_weight;
};

template <typename VertexPointer>
bool operator<(const Edge<VertexPointer>& lhs, const Edge<VertexPointer>& rhs)
{
    return lhs.getWeight() < rhs.getWeight();
}

template <typename VertexPointer>
bool operator>(const Edge<VertexPointer>& lhs, const Edge<VertexPointer>& rhs)
{
    return rhs < lhs;
}

template <typename VertexPointer>
bool operator<=(const Edge<VertexPointer>& lhs, const Edge<VertexPointer>& rhs)
{
    return !(lhs > rhs);
}

template <typename VertexPointer>
bool operator>=(const Edge<VertexPointer>& lhs, const Edge<VertexPointer>& rhs)
{
    return !(lhs < rhs);
}
