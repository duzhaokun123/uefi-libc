//
// Created by o0kam1 on 2025/05/16.
//

#ifndef LOCALE_H
#define LOCALE_H

#ifdef __cplusplus
extern "C" {
#endif

#define LC_ALL      0b00111111
#define LC_COLLATE  0b00000001
#define LC_CTYPE    0b00000010
#define LC_MONETARY 0b00000100
#define LC_NUMERIC  0b00001000
#define LC_TIME     0b00010000

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;

    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;

    char* currency_symbol;
    char frac_digits;
    char p_cs_precedes;
    char n_cs_precedes;
    char p_sep_by_space;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;

    char* int_curr_symbol;
    char int_frac_digits;
    char int_p_cs_precedes;
    char int_n_cs_precedes;
    char int_p_sep_by_space;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};

#ifndef __cplusplus
typedef struct lconv lconv;
#endif

char* setlocale(int category, const char* locale);
lconv* localeconv();

#ifdef __cplusplus
}
#endif

#endif //LOCALE_H
