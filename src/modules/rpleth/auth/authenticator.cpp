/**
 * \file authenticator.cpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief Authenticator class implementation
 */

#include "authenticator.hpp"

#include <sstream>
#include <iomanip>

void Authenticator::serialize(boost::property_tree::ptree& node)
{
    boost::property_tree::ptree& child = node.add("auth", std::string());

    child.put("user", "test"); // FIXME
    for (const auto& csn : _csnList)
    {
        boost::property_tree::ptree ptre;

        ptre.put("", serializeCard(csn));
        child.add_child("card", ptre);
    }
}

void Authenticator::deserialize(boost::property_tree::ptree& node)
{
    for (const auto& v : node)
    {
        if (v.first == "card")
            deserializeCard(v.second.data());
    }
}

bool Authenticator::hasAccess(const CSN& csn)
{
    for (const auto& c : _csnList)
    {
        bool match = true;
        if (c.size() != csn.size())
            continue;
        for (std::size_t i = 0; i < c.size(); ++i)
        {
            if (csn[i] != c[i])
            {
                match = false;
                break;
            }
        }
        if (match)
            return (true);
    }
    return (false);
}

void Authenticator::authorizeCard(const Authenticator::CSN& csn)
{
    _csnList.push_back(csn);
}

std::string Authenticator::serializeCard(const Authenticator::CSN& csn)
{
    std::ostringstream  oss;

    for (std::size_t i = 0; i < csn.size(); ++i)
    {
        if (i > 0)
            oss << ':';
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned>(csn[i]);
    }
    return (oss.str());
}

void Authenticator::deserializeCard(const std::string& card)
{
    std::string         csnString(card);
    std::istringstream  iss;
    CSN                 csn;
    unsigned            byte;

    std::replace(csnString.begin(), csnString.end(), ':', ' ');
    iss.str(csnString);
    while (iss >> std::hex >> byte)
        csn.push_back(byte);
    authorizeCard(csn);
}
