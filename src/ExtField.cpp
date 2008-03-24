#include <iostream>
#include <cstdlib>
#include <dlfcn.h>
#include <cassert>
#include "ExtField.h"

using namespace cigma;

// ---------------------------------------------------------------------------

ExtField::ExtField()
{
    handle = 0;
}

ExtField::~ExtField()
{
    close_library();
}

// ---------------------------------------------------------------------------

void ExtField::load_library(std::string filename, std::string prefix)
{
    std::string function_name;
    char *error;

    //handle = dlopen(filename.c_str(), RTLD_LAZY);

    handle = dlopen(filename.c_str(), RTLD_NOW);
    error = dlerror();
    if (!handle)
    {
        std::cerr << error << std::endl;
        // XXX: throw exception
        assert(false);
    }

    /* clear any existing error */
    dlerror();

    function_name = prefix + "_get_dim";
    *(void **)(&ext_get_dim) = dlsym(handle, function_name.c_str());
    error = dlerror();
    if (error != NULL)
    {
        std::cerr << error << std::endl;
        // XXX: throw exception
        assert(false);
    }

    function_name = prefix + "_get_rank";
    *(void **)(&ext_get_rank) = dlsym(handle, function_name.c_str());
    error = dlerror();
    if (error != NULL)
    {
        std::cerr << error << std::endl;
        // XXX: throw exception
        assert(false);
    }

    function_name = prefix + "_eval";
    *(void **)(&ext_eval) = dlsym(handle, function_name.c_str());
    error = dlerror();
    if (error != NULL)
    {
        std::cerr << error << std::endl;
        // XXX: throw exception
        assert(false);
    }

    function_name = prefix + "_batch_eval";
    *(void **)(&ext_batch_eval) = dlsym(handle, function_name.c_str());
    error = dlerror();
    if (error != NULL)
    {
        std::cerr << error << std::endl;
        // XXX: throw exception
        assert(false);
    }
}

void ExtField::close_library()
{
    if (handle != 0)
    {
        dlclose(handle);
        handle = 0;
    }
}

// ---------------------------------------------------------------------------
