# Employee Payroll System

**Programming Fundamentals (CT-175) – Complex Computing Activity (CCA)**

A console-based **Employee Payroll Management System** written in C. It manages full-time, part-time and contract employees: it records their attendance and overtime, calculates each month's salary (including progressive tax and loan deductions), prints payslips, produces departmental and monthly reports, and saves all data to CSV files so nothing is lost when the program is closed.

The system has a **secure login** with two roles: an **Admin** who has full access, and an **Employee** who can only view his own payslips and records. Every payroll run is also stored in a **month-wise payroll history**, so the records of previous months are never lost.

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

## What's New in This Version

| New addition | Description |
|--------------|-------------|
| Admin / Employee login | The program starts with a login screen. The admin needs a password; an employee logs in with his ID and password. |
| Employee portal | A logged-in employee can only see his own data: payslips, payroll history, attendance and password change. |
| Month-wise payroll history | Running payroll now asks for the **month and year** and stores every employee's result in `History.csv`. Old months are never overwritten, so a year or more of data is kept. |
| Previous-month payslips | A payslip can be printed for any saved month. |
| Payroll history report | Shows one employee's month-by-month record with totals and averages (performance view). |
| Monthly payroll report | Shows all employees for one chosen month with company totals. |
| Auto save / auto load | Everything is saved automatically after payroll, and saved data is loaded automatically at start-up. |
| Safer payroll | Payroll cannot be run twice for the same month, and an attendance summary is shown for confirmation before it runs. |

---

## Features

### Login

| Role | How to log in | Access |
|------|---------------|--------|
| Admin | Choose `1` and enter the admin password | Full access to every feature below |
| Employee | Choose `2` and enter employee ID and password | Own payslips, history, attendance and password only |

- The admin password is set in the source code (`#define ADMIN_PASSWORD`). The default is **`admin123`**.
- The employee password is set by the admin when the employee is added (it can be reset later from the Update option).
- Only 3 wrong password attempts are allowed. On Windows the password is shown as `*` while typing.

### Admin Menu

| No. | Feature | What it does |
|:---:|---------|--------------|
| 1 | Add Employee | Adds a full-time, part-time or contract employee with salary, deduction details and a login password. Duplicate IDs are rejected. |
| 2 | List Employees | Shows a formatted list of every employee in the system. |
| 3 | Update / Delete Employee | Edits an employee's details (including the login password), or deletes the record after confirmation. |
| 4 | Attendance & Overtime | Marks a day present and logs overtime hours (contract employees are not eligible for overtime). |
| 5 | View Attendance Matrix | Shows an employee's attendance for four weeks as a Present/Absent grid. |
| 6 | Search Employee | Finds and displays an employee by ID. |
| 7 | Run Monthly Payroll | Asks for the month and year, optionally lets attendance/overtime be marked, shows an attendance summary, then calculates earnings, overtime pay, progressive tax, loan deduction and net salary. The result of every employee is stored in the history, and attendance and overtime are reset for the next month. |
| 8 | Print Payslip | Prints an itemised payslip of any employee for any saved month. |
| 9 | Departmental Report | Totals gross pay, tax and deductions per department, with employee counts by type (based on the latest payroll). |
| 10 | Save / Load Data | Saves or loads employee, attendance and payroll data using CSV files. |
| 11 | Payroll History of an Employee | Month-by-month table of one employee with total gross, tax and net, average net, average attendance and total overtime. |
| 12 | Monthly Payroll Report | Report of all employees for one chosen month with company totals. |
| 0 | Logout | Logs out after confirmation and can save all data to the files. |

### Employee Menu

| No. | Feature | What it does |
|:---:|---------|--------------|
| 1 | View My Latest Payslip | Shows the payslip of the most recent payroll. |
| 2 | View Payslip of a Previous Month | Shows the payslip of any month that has been saved. |
| 3 | View My Payroll History | Shows the employee's own month-wise record and totals. |
| 4 | View My Attendance | Shows the attendance grid of the current month. |
| 5 | Change My Password | Changes the password after checking the old one. |
| 0 | Logout | Returns to the login screen. |

All inputs are validated: out-of-range numbers, empty text fields, commas in text fields (they would break the CSV files) and invalid menu choices are rejected and the user is asked again.

---

## How to Compile and Run

You need a C compiler such as **GCC** (or Dev-C++ on Windows).

**Compile**

```bash
gcc Payroll_project.c -o payroll
```

**Run**

```bash
./payroll          # Linux / macOS
payroll.exe        # Windows
```

The program creates `EMPLOYEE.csv`, `Attendance.csv` and `Payroll.csv` when data is saved, and `History.csv` every time payroll is run. All files are created in the folder you run the program from. When the program starts again it loads this saved data automatically.

**Default login:** Admin password is `admin123`.

---

## Sample Input and Output

**Scenario:** log in as admin, add one full-time employee, run payroll for November 2026 (marking two days of attendance and 4 hours of overtime), then print the payslip and view the payroll history.

**Input (in order)**

| Step | Menu choice | Values entered |
|------|-------------|----------------|
| Login | `1` | Admin password `admin123` |
| Add employee | `1` | ID `101`, Name `Ali Khan`, Department `Production`, Password `ali123`, Type `1` (full time), Basic `40000`, Health `2000`, House `5000`, Travelling `3000`, Loan `0`, Other deductions `0` |
| Run payroll | `7` | `y` to confirm, Month `11`, Year `2026` |
| Mark attendance (asked during payroll) | `y` → `1` | ID `101`, Day `1`, Week `1` |
| Mark attendance (asked during payroll) | `y` → `1` | ID `101`, Day `2`, Week `1` |
| Mark overtime (asked during payroll) | `y` → `2` | ID `101`, Hours `4` |
| Finish marking | `n` | Then `y` to proceed with payroll |
| Print payslip | `8` | ID `101`, Month `11`, Year `2026` |
| View history | `11` | ID `101` |

**Output (payslip)**

```text
Records available for: Nov 2026

======================================================================
Name         : Ali Khan             Department  : Production
Employee type: Full time            Employee ID : 101
Basic salary : 40000.00             Loan left   : 0.00
Days present : 2                    Overtime hrs: 4.0
======================================================================
                       PAYSLIP - November 2026
======================================================================
Earnings                                                        Amount
----------------------------------------------------------------------
Basic Salary                                                   4000.00
Health Allowances                                              2000.00
House Allowances                                               5000.00
Travelling Allowances                                          3000.00
Overtime Pay                                                   1500.00
----------------------------------------------------------------------
Gross Salary                                                  15500.00
----------------------------------------------------------------------
Deductions                                                      Amount
----------------------------------------------------------------------
Tax Deducted                                                   1550.00
Loan Deducted                                                     0.00
Other Deductions                                                  0.00
----------------------------------------------------------------------
Total Deductions                                               1550.00
----------------------------------------------------------------------
Net Salary                                                    13950.00
======================================================================
```

**Output (payroll history)**

```text
========================== PAYROLL HISTORY ===========================
Employee: Ali Khan (ID 101)
----------------------------------------------------------------------
Month     Days  OT hrs   Gross        Tax          Net
----------------------------------------------------------------------
Nov 2026  2     4.0      15500.00     1550.00      13950.00
----------------------------------------------------------------------
Months on record                                                     1
Total gross earned                                            15500.00
Total tax paid                                                 1550.00
Total net received                                            13950.00
Average net per month                                         13950.00
Average days present (out of 20)                                   2.0
Total overtime hours                                               4.0
======================================================================
```

**How the numbers come out:** 2 days present × (40000 ÷ 20) = 4000 basic earned; 4 overtime hours × 1.5 × (40000 ÷ 160) = 1500; gross = 4000 + 10000 allowances + 1500 = 15500; tax at 10% on the first 50,000 = 1550; net = 15500 − 1550 = 13950.

**Invalid input example**

```text
Enter your choice:15
Invalid input, must be in between 0 and 13
```

**Wrong password example**

```text
Enter admin password: ****
Wrong password. 2 attempt(s) left.
```

**Running the same month twice**

```text
Payroll for November 2026 has already been processed and saved.
It cannot be run twice (loans and deductions would be applied again).
```

---

## Data Files

| File | Created when | Contents |
|------|--------------|----------|
| `EMPLOYEE.csv` | Data is saved / payroll is run | Employee details, deductions and login password |
| `Attendance.csv` | Data is saved / payroll is run | Current month's attendance of every employee |
| `Payroll.csv` | Data is saved / payroll is run | Result of the latest payroll run |
| `History.csv` | Payroll is run | **One row per employee per month** – the permanent payroll record of all previous months |

---

## PF Concepts Used

| Concept | Where it is used |
|---------|------------------|
| Variables and data types | `int`, `double` and `char[]` fields in the employee, salary, deductions and record structs |
| Conditional statements (`if` / `switch`) | `switch(choice)` in the admin and employee portals, `switch(employee_type)` in `addemployee`, `update` and `payroll`, the login checks and the progressive tax `if–else` chain |
| Loops | `do…while` menu loops, `for(;;)` retry loops in `readInt`, `readtext`, `readDouble` and `flag`, the login attempt loop, and `for` loops over employees, attendance and history records |
| Functions | The program is split into many reusable functions such as `readInt`, `admin_login`, `employee_login`, `addemployee`, `payroll`, `run_payroll`, `payslip_record`, `write` and `read` |
| Arrays and strings | The dynamic employee array, the 2D `att[4][5]` attendance grid, the `month_names` string array, and `char name[50]`, `char department[30]`, `char password[20]` |
| String functions | `strcmp` (password check), `strcpy`, `strncpy`, `strchr`, `strlen`, `strcspn`, `strtok` |
| Structures | `salary`, `deductions`, `employee`, `record` (one month's payroll of one employee) and `history` (dynamic array of records) |
| Pointers and dynamic memory | `malloc()` for the employee array and the history, `realloc()` in `maintain_Memory()` and `history_add()`, and pointer parameters such as `update(employee *P)`, `read(employee **K, …)` and `history *H` |
| File handling | `write()`, `read()`, `history_save()` and `history_load()` use `fopen`, `fprintf`, `fgets` and `strtok` to save and load the CSV files |
| Recursion | `find_employee()` searches for an employee by ID recursively |
| Sorting | `history_add()` inserts each record in the right place so the history always stays in month order |

---

## Folder Structure

```text
Payroll-System/
├── Payroll_project.c      # Complete source code
├── Project_Report.pdf     # Project report
├── README.md              # This file
├── Flowchart.pdf          # Program flowchart
├── EMPLOYEE.csv           # Created when data is saved
├── Attendance.csv         # Created when data is saved
├── Payroll.csv            # Created when data is saved
└── History.csv            # Created when payroll is run (all months)
```
