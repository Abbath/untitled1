#ifndef ENGINE_H
#define ENGINE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
class Engine
{
    int size_of_file (FILE *);
    bool noise;
public:
    Engine();
    ~Engine();
    int hide (const char*,const char*,const char*);
    int unhide (const char*,const char*,const char*);
    void setNoise(bool);
};

#endif // ENGINE_H
