#ifndef __UTILS_H__
#define __UTILS_H__

uint64_t mapRuntimeToIterationcount (double runtime, int (*func)(uint64_t numiterations));
void parseOptions (const char *options, void (*parser) (int));

#endif /* __UTILS_H__ */
