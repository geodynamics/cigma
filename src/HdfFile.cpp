#include <cstring>
#include <cassert>

#include "HdfFile.h"
#include "HdfAttr.h"


// ---------------------------------------------------------------------------

HdfFile::HdfFile()
{
    file_id = -1;
}

HdfFile::~HdfFile()
{
    close();
}

int HdfFile::close()
{
    herr_t status;

    if (file_id >= 0)
    {
        status = H5Fclose(file_id);
    }

    file_id = -1;

    return (int)status;
}


// ---------------------------------------------------------------------------

int HdfFile::create(const char *filename, const char *mode)
{
    if (strcmp(mode, "w") == 0)
    {
        /* Create file by truncating (i.e. overwriting previous file) */
        file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    }
    else if (strcmp(mode, "x") == 0)
    {
        /* Create file exclusively (i.e. fail if it already exists) */
        file_id = H5Fcreate(filename, H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        assert(false);
        return -1;
    }

    if (file_id < 0)
    {
        return -2;
    }

    hid_t root;
    herr_t status;

    root = H5Gopen(file_id, "/");
    status = HdfAttr::set_string(root, "TITLE", "Cigma file"); // TODO: add title to argument list
    status = HdfAttr::set_string(root, "CLASS", "GROUP");
    status = HdfAttr::set_string(root, "VERSION", "1.0");
    status = HdfAttr::set_string(root, "PYTABLES_FORMAT_VERSION", "1.5");
    status = HdfAttr::set_string(root, "CigmaVersion", "0.9.1");
    status = H5Gclose(root);

    return 0;
}


int HdfFile::open(const char *filename, const char *mode)
{
    /*
     * Open file for reading. Fail if file doesn't exist.
     */
    if (strcmp(mode, "r") == 0)
    {
        /* Open file in read-only mode */
        file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);

        /* Check for failure */
        if (file_id < 0)
        {
            return -1;
        }

        return 0;
    }

    /*
     * Open file for writing. If file exists, it is truncated.
     */
    if (strcmp(mode, "w") == 0)
    {
        this->create(filename, "w");

        if (file_id < 0)
        {
            return -2;
        }

        return 0;
    }

    /*
     * Open file for reading and writing. Fail if file doesn't exist.
     */
    if (strcmp(mode, "rw") == 0)
    {
        /* Open file in read-write mode */
        file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

        if (file_id < 0)
        {
            return -3;
        }

        return 0;
    }

    /*
     * Open file for reading and writing. Create the file if necessary.
     */
    if (strcmp(mode, "rw+") == 0)
    {
        /* See http://hdf.ncsa.uiuc.edu/HDF5/doc/Errors.html */
        herr_t (*old_func)(void *);
        void *old_client_data;

        /* Save old error handler */
        H5Eget_auto(&old_func, &old_client_data);

        /* Turn off error handling */
        H5Eset_auto(NULL, NULL);

        /* Open file in read-write mode -- errors supprssed */
        file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

        /* Restore error handler */
        H5Eset_auto(old_func, old_client_data);

        /* If opening the file failed, try to create it */
        if (file_id < 0)
        {
            this->create(filename, "w");

            if (file_id < 0)
            {
                return -4;
            }
        }

        return 0;
    }

    /*
     * Exclusively open file for writing. Fail if file already exists.
     */
    if (strcmp(mode, "x") == 0)
    {
        this->create(filename, "x");

        if (file_id < 0)
        {
            return -5;
        }

        return file_id;
    }

    /*
     * Invalid mode was provided.
     */
    return -6;
}


// ---------------------------------------------------------------------------
