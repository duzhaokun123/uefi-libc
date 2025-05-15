//
// Created by o0kam1 on 2025/05/16.
//

#include <locale.h>

#pragma clang diagnostic ignored "-Wwritable-strings"

lconv _lconv = {
    .decimal_point = ".",
    .thousands_sep = ",",
    .grouping = "\003",

    .mon_decimal_point = ".",
    .mon_thousands_sep = ",",
    .mon_grouping = "\003",
    .positive_sign = "",
    .negative_sign = "",

    .currency_symbol = "$",
    .frac_digits = 2,
    .p_cs_precedes = 1,
    .n_cs_precedes = 1,
    .p_sep_by_space = 0,
    .n_sep_by_space = 0,
    .p_sign_posn = 0,
    .n_sign_posn = 0,

    .int_curr_symbol = "USD",
    .int_frac_digits = 2,
    .int_p_cs_precedes = 1,
    .int_n_cs_precedes = 1,
    .int_p_sep_by_space = 0,
    .int_n_sep_by_space = 0,
    .int_p_sign_posn = 0,
    .int_n_sign_posn = 0,
};

char* setlocale(int category, const char* locale) {
    return nullptr; // no locale support yet
}

lconv* localeconv() {
    return &_lconv;
}


