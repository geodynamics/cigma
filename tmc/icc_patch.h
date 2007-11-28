#ifndef ICC_PATCH_INCLUDED
#define ICC_PATCH_INCLUDED

#if defined(__ICC)
/* icc doesn't like GNU __extension__ functions */
#undef htons
#undef ntohs
#undef htonl
#undef ntohl
#define htons(x) __bswap_constant_16(x)
#define ntohs(x) __bswap_constant_16(x)
#define htonl(x) __bswap_constant_32(x)
#define ntohl(x) __bswap_constant_32(x)
#endif /* defined(_ICC) */

#endif /* ICC_PATCH_INCLUDED */
