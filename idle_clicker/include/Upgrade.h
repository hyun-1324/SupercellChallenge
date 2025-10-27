#pragma once

#include <string>

class Upgrade {
public:
    Upgrade(const std::string& name, long long baseCost, long long baseProduction);

    void purchase();

    std::string getName() const { return m_name; }
    int getLevel() const { return m_level; }
    long long getCost() const { return m_cost; }
    long long getProduction() const { return m_production; }

private:
    std::string m_name;
    int m_level;
    long long m_baseCost;
    long long m_cost;
    long long m_baseProduction;
    long long m_production;

    void updateCost();
};
