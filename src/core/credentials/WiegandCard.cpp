/*
    Copyright (C) 2014-2016 Islog

    This file is part of Leosac.

    Leosac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Leosac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/credentials/WiegandCard.hpp"
#include "exception/ModelException.hpp"
#include "tools/log.hpp"
#include <boost/algorithm/string.hpp>

using namespace Leosac;
using namespace Leosac::Cred;


const std::string &WiegandCard::card_id() const
{
    return card_id_;
}

int WiegandCard::nb_bits() const
{
    return nb_bits_;
}

uint64_t WiegandCard::to_raw_int() const
{
    auto card_num_hex = boost::replace_all_copy(card_id_, ":", "");

    static_assert(sizeof(decltype(std::stoull(card_num_hex, nullptr, 16))) ==
                      sizeof(uint64_t),
                  "stoul not big enough");

    uint64_t tmp      = std::stoull(card_num_hex, nullptr, 16);
    int trailing_zero = (64 - nb_bits_) % 8;
    tmp >>= trailing_zero;
    return tmp;
}

uint64_t WiegandCard::to_int() const
{
    switch (nb_bits_)
    {
    case 26:
        return to_wiegand_26();
    case 34:
        return to_wiegand_34();
    default:
        INFO("Not using format to convert WiegandCard to integer because no format "
             "match.");
        return to_raw_int();
    }
}

uint64_t WiegandCard::to_wiegand_26() const
{
    assert(nb_bits_ == 26);
    assert(card_id_.size() == 2 * 4 + 3);

    uint64_t tmp = to_raw_int();
    // Drop the last bit (parity) from the raw frame.
    tmp >>= 1;
    // keep 16 bits
    tmp &= 0xFFFF;
    return tmp;
}

uint64_t WiegandCard::to_wiegand_34() const
{
    assert(nb_bits_ == 34);
    assert(card_id_.size() == 2 * 5 + 4);

    uint64_t tmp = to_raw_int();
    // Drop the last bit (parity) from the raw frame.
    tmp >>= 1;
    // keep 24 bits
    tmp &= 0xFFFFFF;
    return tmp;
}

void WiegandCard::nb_bits(int i)
{
    WiegandCardValidator::validate_nb_bits(i);
    nb_bits_ = i;
}

void WiegandCard::card_id(const std::string &id)
{
    WiegandCardValidator::validate_card_id(id);
    card_id_ = id;
}

void WiegandCardValidator::validate(const IWiegandCard &card)
{
    validate_card_id(card.card_id());
    validate_nb_bits(card.nb_bits());
}

void WiegandCardValidator::validate_card_id(const std::string &card_id)
{
    bool fail = false;
    char c;
    std::istringstream ss(card_id);
    while (true)
    {
        c = 0;
        for (int i = 0; i < 2; ++i)
        {
            ss >> c;
            if (!isxdigit(c) || !ss.good())
                fail = true;
        }
        ss >> c;
        if (!ss.good())
            break;
        if (c != ':')
            fail = true;
    }
    if (fail)
    {
        throw ModelException("data/attributes/cardId",
                             "Card id must have aa:bb:cc:11 format.");
    }
}

void WiegandCardValidator::validate_nb_bits(int nb)
{
    if (nb <= 0)
    {
        throw ModelException("data/attributes/nbBits",
                             "The number of bits must be > 0");
    }
}
