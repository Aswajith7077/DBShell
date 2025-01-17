 # **Basic Implementation of Database in shell script**
 The script immediately runs authentication function that will keep track of users and
 stores them into a log file.
 A database file consist of contents of database
- Administrators can alone create the database file.
- Other Users cannot create or delete the database file.
- Other Users can create a SQL file and perform queries that modifies the
 contents of the database file.

Then a database shell is created and the shell can perform the following operations:
 1. If the user is a database administrator, he can perform a CREATE USER to create
 a user.
 2. **EXIT**: can terminate the shell
 3. **LOGOUT**: can logout from the database user.
 4. **PERFORM** *filename*: Opens up a new window to write SQL Queries.
 5. **COMPILE** *filename*: Runs the file with SQL Queries.
 6. Any invalid operation will create a log entry with an alert message.
 
The Default administrator is given by,
1. **USERNAME**: SYS
2. **PASSWORD**: *Created by the first user*
 
 For a User Can perform the following operations in a SQL file:
 1. INSERT INTO *table_name* VALUES(*value_1*, *value_2*, *value_3*,...);
    - Inserts the list of values to the given table name
 2. UPDATE INTO *table_name*:
    - Updates a value to the attribute--
    - Can specify a WHERE condition to sort out the records.
 3. SELECT:
    - Should specify the attributes----
    - ***ORDERBY*** sorts the file according to the order specified based on the specified field.
    - ***WHERE CONDITION*** selects the specifies records from the database
    - ***GROUPBY*** groups the records based on a particular field
    - ***HAVING*** conditions for a ***GROUP BY***
 4. DELETE:
    - Deletes all the records by default.
    - WHERE sorts out the records based on the field
   
# **How to Run**

1. Compile the C program,

   ```
   gcc user.c
   ```

2. Then finally run the bash script

   ```
   bash ./sample.sh
   ```


