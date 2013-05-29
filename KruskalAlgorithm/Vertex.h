#pragma once

#include <string>

class Vertex
{
public:
    explicit Vertex(std::string id)
        : m_id(std::move(id))
    {

    }

    std::string getId() const
    {
        return m_id;
    }
private:
    std::string m_id;
};
