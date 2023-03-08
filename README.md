# Adjust

**
#============================================
#Error:Currently the code is not working and I am still trying to debug this code. 
#postgres=# create extension min_to_max ;
#ERROR: incompatible library "/var/lib/pgsql/adjust/min_to_max/min_to_max.so": version mismatch DETAIL: Server is version 13, library is version 9.2. postgres=#.
#============================================

**

To create an aggregate function that returns a text formatted as "min -> max" for a given column, we can follow these steps:

Here's an example implementation:

1.Define the C function that implements the aggregate. This function should accept the values to be aggregated and track the minimum and maximum values seen so far. The function should return a pointer to a struct containing the state of the aggregation.

Create a file called as "min_to_max.c" .

2.Define the SQL aggregate function that uses the C functions. This function specifies the input and output data types, as well as the transition and final functions to use.

In this step we will create a file called as "min_to_max--1.0.sql"

3. Compile the C code into a shared library.

gcc -shared -o min_to_max.so min_to_max.c -I /usr/include/pgsql/server -L /usr/pgsql-13/lib -lpq -fPIC 
gcc -I /usr/include/pgsql/server/ -fPIC -c min_to_max.c -o min_to_max.o

4.Copy the shared library to the PostgreSQL library directory.

sudo cp /var/lib/pgsql/min_to_max/min_to_max.so /usr/share/pgsql/extension
sudo cp /var/lib/pgsql/min_to_max/min_to_max.control /usr/share/pgsql/extension

5. Create a Control file :

The control file is an optional file that can be included with an extension to provide additional metadata and installation instructions. Here's an example control file for the min_to_max extension:

6. Once all the above steps are done then we can connect to our DB server and try to create the extension .

create extension min_to_max;

