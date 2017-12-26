#ifndef BELONGS_H
#define BELONGS_H
#include <cstdint>
/* If the element e belongs to the set s, then this function returns non-zero value;
 * else this function returns zero. The internal representation of the set is the value of
 * the type uint64_t: if bit with #i is equal to 1, then the number i belongs to the set;
 * else the number i doesn't belong to set. */
inline uint64_t belongs(uint64_t e, uint64_t s){
    return s & (1ULL << e);
}
#endif
