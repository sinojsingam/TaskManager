#pragma once

#include <iostream>
#include <set>

namespace apiMod {
    /** 
     * Custom Request Handler
     */
    std::string sanitizeText(
            std::string &original_text,
            const char replacement_char='_',
            std::set<char> extra_checks={});


    void run();
}
