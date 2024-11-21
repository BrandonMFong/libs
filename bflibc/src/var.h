/**
 * author: brando
 * date: 11/20/24
 *
 * The following helps find the type of a variable.
 *
 * ref:
 * 	https://stackoverflow.com/a/57409808/12135693
 */

#ifndef VAR_H
#define VAR_H

#define Type(x) _Generic((x),                   \
                         int: "int",            \
                         short: "short",        \
                         long: "long",          \
                         char: "char",          \
                         float: "float",        \
                         double: "double",      \
                         default: "unknown"     \
                         )

#endif // VAR_H

