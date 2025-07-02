// yep, it's just this... Didn't feel like using stdbool.h from Microsoft.
#ifdef __cplusplus
extern "C" {
#endif

#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifdef __cplusplus
}
#endif
