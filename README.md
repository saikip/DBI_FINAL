## **Final Submission**

## **Group Info**
Priyam Saikia 
Ruchika Mishra

## **Problem**
Create, Insert, Run Query and Drop new tables. Run SQL queries in existing tables.

## **Data**
We have run the tpchgen program available from the TPC-H website to generate our own test 
data instances of size 1GB. This data is at the located inside the directory A5/tpch/.

## **Run:** 
* ------------  Instructions to run: ------------  *
1. Login to thunder.cise.ufl.edu .
2. Extract files from Zip folder.
3. Change directory to corresponding extracted folder. 
4. Type ‘make’ without the quotes and enter. 
5. The executables are now ready. The executable for the program created by our code is
   named "entryPoint4_2" and is located in "./bin/".
6. Please follow the below instructions to compile and run the main program or GTests.

To run the program, do:

1. Type './bin/entryPoint4_2' without quotes and press enter.
2. Type any query (sample query given at the end) and press <Control-D>.
3. For eg, we can create, insert into, run some query and drop the partsupp table using the following commands,
    
	CREATE TABLE partsupp (ps_partkey INTEGER, ps_suppkey INTEGER, ps_availqty INTEGER, ps_supplycost DOUBLE, ps_comment STRING) AS HEAP;
	INSERT tpch/partsupp.tbl INTO partsupp;
	SET OUTPUT STDOUT;
	SELECT ps.ps_partkey, ps.ps_suppkey, ps.ps_availqty 
	FROM partsupp AS ps 
	WHERE (ps.ps_partkey < 100) AND (ps.ps_suppkey < 50);
	DROP TABLE partsupp;

4. If you are trying to create a table, make sure 
	a. it's  schema is present in the tcph/catalog
	b. it's .tbl file is present in the tpch/ for loading
	c. the table record (relations and attributes) is also present in ./stats.txt file
	
To run a test case, do:

1. Type './bin/test' without quotes and press enter.
2. Check if all the testcases have executed.

