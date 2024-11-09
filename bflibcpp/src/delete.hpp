/**
 * author: Brando
 * date: 8/1/22
 */

#ifndef DELETE_HPP
#define DELETE_HPP

#define Delete(x) if (x != 0) {delete x; x = 0; }

#define BFDelete(x) if (x != 0) {delete x; x = 0; }

#warning "Delete & BFDelete will be deprecated. Please start using BFRelease"

#endif // DELETE_HPP

