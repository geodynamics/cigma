#include <string.h>
#include <hdf5.h>
#include "h5_io.h"
#include "h5_attr.h"
#include "split.h"



hid_t h5_file_create(const char *filename, const char *mode)
{
    hid_t file_id;
    hid_t root;
    herr_t status;
    
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
        return -1;
    }
    
    if (file_id < 0)
    {
        return -2;
    }

    root = H5Gopen(file_id, "/");
    status = set_str_attr(root, "TITLE", "CIGMA file");
    status = set_str_attr(root, "CLASS", "GROUP");
    status = set_str_attr(root, "VERSION", "1.0");
    status = set_str_attr(root, "PYTABLES_FORMAT_VERSION", "1.5");
    status = H5Gclose(root);

    return file_id;
}



hid_t h5_file_open(const char *filename, const char *mode)
{
    /*
     * Open file for reading. Fail if file doesn't exist.
     */
    if (strcmp(mode, "r") == 0)
    {
        hid_t file_id;

        /* Open file in read-only mode */
        file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);

        /* Check for failure */
        if (file_id < 0)
        {
            return -1;
        }

        return file_id;
    }

    /*
     * Open file for writing. If file exists, it is truncated.
     */
    if (strcmp(mode, "w") == 0)
    {
        hid_t file_id;

        file_id = h5_file_create(filename, "w");

        if (file_id < 0)
        {
            return -2;
        }

        return file_id;
    }

    /*
     * Open file for reading and writing. Fail if file doesn't exist.
     */
    if (strcmp(mode, "rw") == 0)
    {
        hid_t file_id;

        /* Open file in read-write mode */
        file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

        /* Check for failure */
        if (file_id < 0)
        {
            return -3;
        }

        return file_id;
    }

    /*
     * Open file for reading and writing. Create the file if necessary.
     */
    if (strcmp(mode, "rw+") == 0)
    {
        hid_t file_id;

        /* See http://hdf.ncsa.uiuc.edu/HDF5/doc/Errors.html */
        herr_t (*old_func)(void *);
        void *old_client_data;
        
        /* Save old error handler */
        H5Eget_auto(&old_func, &old_client_data);

        /* Turn off error handling */
        H5Eset_auto(NULL, NULL);

        /* Open file in read-write mode -- errors suppressed */
        file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
        
        /* Restore error handler */
        H5Eset_auto(old_func, old_client_data);

        /* If opening the file failed, try to create it */
        if (file_id < 0)
        {
            file_id = h5_file_create(filename, "w");

            if (file_id < 0)
            {
                return -4;
            }
        }

        return file_id;
    }

    /* 
     * Exclusively open file for writing. Fail if file already exists.
     */
    if (strcmp(mode, "x") == 0)
    {
        hid_t file_id;

        file_id = h5_file_create(filename, "x");

        if (file_id < 0)
        {
            return -5;
        }
    }


    /*
     * Invalid mode
     */
    return -6;
}



hid_t h5_group_create(hid_t loc_id, const char *name)
{
    hid_t group_id;
    herr_t status;

    group_id = H5Gcreate(loc_id, name, 0);
    if (group_id < 0)
    {
        return -1;
    }

    status = set_str_attr(group_id, "TITLE", "");
    status = set_str_attr(group_id, "CLASS", "GROUP");
    status = set_str_attr(group_id, "VERSION", "1.0");
    status = set_str_attr(group_id, "PYTABLES_FORMAT_VERSION", "1.5");
    
    return group_id;
}



hid_t h5_group_open(hid_t loc_id, const char *name)
{
    return H5Gopen(loc_id, name);
}



hid_t h5_touch_group(hid_t loc_id, const char *name)
{
    hid_t group_id;

    group_id = H5Gopen(loc_id, name);

    if (group_id < 0)
    {
        group_id = h5_group_create(loc_id, name);
        if (group_id < 0)
        {
            return -1;
        }
    }

    return group_id;
}



hid_t h5_touch_path(hid_t loc_id, const char *path)
{
    hid_t group_id;

    /* Temporarily disabling error messages:
     * http://hdf.ncsa.uiuc.edu/HDF5/doc/Errors.html
     */
    herr_t (*old_func)(void *);
    void *old_client_data;

    /* Save old error handler */
    H5Eget_auto(&old_func, &old_client_data);

    /* Turn off error handling */
    H5Eset_auto(NULL, NULL);

    /* Attempt to open the group */
    group_id = H5Gopen(loc_id, path);

    if (group_id < 0)
    {
        herr_t status;
        hid_t parent_id;
        hid_t child_id;
        char **names;
        int i,n;

        split(path, strlen(path), &names, &n, '/');

        // first parent
        parent_id = h5_touch_group(loc_id, names[0]);
        if (parent_id < 0)
        {
            H5Eset_auto(old_func, old_client_data);
            split_free(names, n);
            return -1;
        }

        for (i = 1; i < n; i++)
        {
            // get child id
            child_id = h5_touch_group(parent_id, names[i]);
            if (child_id < 0)
            {
                H5Eset_auto(old_func, old_client_data);
                split_free(names, n);
                return -2;
            }
            // move to next parent
            status = H5Gclose(parent_id);
            parent_id = child_id;
        }

        // return last group
        group_id = parent_id;
    }
    
    /* Restore previous error handler */
    H5Eset_auto(old_func, old_client_data);

    return group_id;
}

