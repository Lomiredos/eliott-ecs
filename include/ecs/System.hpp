#pragma once
#include <set>
#include "ComponentRegistry.hpp"

namespace ee::ecs {

    class System
    {
        
    public:
        std::set<EntityID> m_entities;
        virtual ~System() = default;
    };

}