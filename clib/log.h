/**
 * author: Brando
 * date: 7/29/22
 */

#ifdef DEBUG

#define DLog(...) printf("[%s:%d]: ",  __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")

#else 

#define DLog(...)

#endif // DEBUG

