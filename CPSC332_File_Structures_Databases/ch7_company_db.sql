SHOW DATABASES;
SHOW TABLES;

DESCRIBE department;

SELECT * FROM department;

DESCRIBE employee;

SELECT * FROM employee;

DESCRIBE dept_locations;

SELECT * FROM dept_locations;

DESCRIBE project;

SELECT * FROM project;


DESCRIBE works_on;

SELECT * FROM works_on;


DESCRIBE dependent;

SELECT * FROM dependent;



CREATE VIEW DEPT_SUMMARY (D, C, Total_s, Average_s)
AS SELECT Dno, COUNT(*), SUM(salary), AVG(salary) 
FROM employee
GROUP BY Dno;
