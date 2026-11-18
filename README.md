# Employee Payroll System

**Programming Fundamentals (CT-175) – Complex Computing Activity (CCA)**

A console-based **Employee Payroll Management System** written in C. It manages full-time, part-time and contract employees: it records their attendance and overtime, calculates each month's salary (including progressive tax and loan deductions), prints payslips, produces departmental reports, and saves all data to CSV files so nothing is lost when the program is closed.

Doing payroll by hand is slow and easy to get wrong, especially when employees are paid differently and tax is charged in slabs. This program automates that work using the concepts taught in the course.

---

## Group Members

| S. No. | Name | Roll Number | Section |
|:------:|------|-------------|:-------:|
| 1 | Furqan Mansoor | CT-26148 | E |
| 2 | Abubakar Azhar | CT-26223 | E |
| 3 | Ahmed Raza | CT-26229 | E |
| 4 | Shahmeer | CT-26240 | E |

**Instructor:** Ms. Maham Ashraf  
**Course:** Programming Fundamentals (CT-175)  
**Semester:** Fall 2026

---

## Features

| No. | Feature | What it does |
|:---:|---------|--------------|
| 1 | Add Employee | Adds a full-time, part-time or contract employee with salary and deduction details. Duplicate IDs are rejected. |
| 2 | List Employees | Shows a formatted list of every employee in the system. |
| 3 | Update / Delete Employee | Edits an employee's details, or deletes the record after confirmation. |
| 4 | Attendance & Overtime | Marks a day present and logs overtime hours (contract employees are not eligible for overtime). |
| 5 | View Attendance Matrix | Shows an employee's attendance for four weeks as a Present/Absent grid. |
| 6 | Search Employee | Finds and displays an employee by ID. |
| 7 | Run Monthly Payroll | Calculates earnings, overtime pay, progressive tax, loan deduction and net salary, then resets attendance and overtime for the next month. |
| 8 | Print Payslip | Prints an itemised payslip for one employee. |
| 9 | Departmental Report | Totals gross pay, tax and deductions per department, with employee counts by type. |
| 10 | Save / Load Data | Saves or loads employee, attendance and payroll data using CSV files. |
| 0 | Exit | Closes the program after confirmation and frees allocated memory. |

All inputs are validated: out-of-range numbers, empty text fields and invalid menu choices are rejected and the user is asked again.

---

## How to Compile and Run

You need a C compiler such as **GCC**.

**Compile**

```bash
gcc Payroll_project.c -o payroll
```

**Run**

```bash
./payroll          # Linux / macOS
payroll.exe        # Windows
```

When you choose **Save data**, the program creates `EMPLOYEE.csv`, `Attendance.csv` and `Payroll.csv` in the folder you run it from. **Load data** reads them back in.

---

## Sample Input and Output

**Scenario:** add one full-time employee, mark two days of attendance, add 4 hours of overtime, run payroll, and print the payslip.

**Input (in order)**

| Step | Menu choice | Values entered |
|------|-------------|----------------|
| Add employee | `1` | ID `101`, Name `Ali Khan`, Department `Production`, Type `1` (full time), Basic `40000`, Health `2000`, House `5000`, Travelling `3000`, Loan `0`, Other deductions `0` |
| Mark attendance | `4` → `1` | ID `101`, Day `1`, Week `1` |
| Mark attendance | `4` → `1` | ID `101`, Day `2`, Week `1` |
| Mark overtime | `4` → `2` | ID `101`, Hours `4` |
| Run payroll | `7` | `y` to confirm |
| Print payslip | `8` | ID `101` |

**Output (payslip)**

```text
======================================================================
Name         :Ali Khan                       Department   :Production
Employee type:Full time                      Employee ID  :101
Basic salary :40000.00                       Loan         :0.00
======================================================================
======================================================================
                               PAYSLIP
======================================================================
Earnings                                                 Amount
----------------------------------------------------------------------
Basic Salary                                             4000.00
Health Allowances                                        2000.00
House Allowances                                         5000.00
Travelling Allowances                                    3000.00
Overtime Pay                                             1500.00
----------------------------------------------------------------------
Gross Salary                                             15500.00
----------------------------------------------------------------------
Deductions                                               Amount
----------------------------------------------------------------------
Tax Deducted                                             1550.00
Loan Deducted                                            0.00
Other Deductions                                         0.00
----------------------------------------------------------------------
Total Deductions                                         1550.00
----------------------------------------------------------------------
Net Salary                                               13950.00
======================================================================
```

**How the numbers come out:** 2 days present × (40000 ÷ 20) = 4000 basic earned; 4 overtime hours × 1.5 × (40000 ÷ 160) = 1500; gross = 4000 + 10000 allowances + 1500 = 15500; tax at 10% on the first 50,000 = 1550; net = 15500 − 1550 = 13950.

**Invalid input example**

```text
Enter your choice:15
Invalid input, must be in between 0 and 11
```

---

## PF Concepts Used

| Concept | Where it is used |
|---------|------------------|
| Variables and data types | `int`, `double` and `char[]` fields in the employee, salary and deductions structs |
| Conditional statements (`if` / `switch`) | `switch(choice)` in `main()`, `switch(employee_type)` in `addemployee`, `update` and `payroll`, and the progressive tax `if–else` chain |
| Loops | `do…while` menu loop in `main()`, `for(;;)` retry loops in `readInt`, `readtext`, `readDouble` and `flag`, and `for` loops over employees and attendance |
| Functions | The program is split into many reusable functions such as `readInt`, `addemployee`, `payroll`, `write` and `read` |
| Arrays and strings | The dynamic employee array, the 2D `att[4][5]` attendance grid, and `char name[50]`, `char department[30]` |
| Structures | `salary`, `deductions` and `employee` structs (with `employee` containing the other two) |
| Pointers and dynamic memory | `malloc()` for the employee array, `realloc()` in `maintain_Memory()`, and pointer parameters such as `update(employee *P)` and `read(employee **K, …)` |
| File handling | `write()` and `read()` use `fopen`, `fprintf`, `fgets` and `strtok` to save and load the CSV files |
| Recursion | `find_employee()` searches for an employee by ID recursively |

---

## Folder Structure

```text
Payroll-System/
├── Payroll_project.c      # Complete source code
├── Project_Report.pdf   # Project report
├── README.md              # This file
├── Flowchart.pdf          # Program flowchart
├── EMPLOYEE.csv           # Created when data is saved
├── Attendance.csv         # Created when data is saved
└── Payroll.csv            # Created when data is saved
```
