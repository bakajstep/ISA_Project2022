//
// Created by sbakaj on 10/11/22.
//

#ifndef ISA_PROJECT2022_ERROR_HPP
#define ISA_PROJECT2022_ERROR_HPP

#include <string>

namespace Error
{
    enum Error
    {
        E_NO_ERR = 0,
        E_PARAMETER = 1,
    };
}

void exit_with_error(int error, std::string message);

#endif //ISA_PROJECT2022_ERROR_HPP
