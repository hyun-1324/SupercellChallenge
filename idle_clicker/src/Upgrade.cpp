#include "../include/Upgrade.h"
#include <cmath>

Upgrade::Upgrade(const std::string& name, long long baseCost, long long baseProduction)
    : m_name(name)
    , m_level(0)
    , m_baseCost(baseCost)
    , m_cost(baseCost)
    , m_baseProduction(baseProduction)
    , m_production(0)
{
}

void Upgrade::purchase() {
    m_level++;
    m_production += m_baseProduction;
    updateCost();
}

void Upgrade::updateCost() {
    // 비용은 1.15배씩 증가 (Cookie Clicker 방식)
    m_cost = static_cast<long long>(m_baseCost * std::pow(1.15, m_level));
}
