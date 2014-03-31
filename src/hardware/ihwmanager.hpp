/**
 * \file ihwmanager.hpp
 * \author Thibault Schueller <thibault.schueller@islog.com>
 * \brief hardware managing class interface
 */

#ifndef IHWMANAGER_HPP
#define IHWMANAGER_HPP

class GPIO;

class IHWManager
{
public:
    virtual ~IHWManager() {}
    virtual GPIO*   reserveGPIO(int id) = 0;
};

#endif // IHWMANAGER_HPP
