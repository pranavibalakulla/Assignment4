# Assignment 4 — Employee Shift Scheduler

## Project Overview

This project implements an Employee Shift Scheduler in two programming languages: **Python** and **C++**. The application generates a weekly employee schedule while considering employee shift preferences and required scheduling constraints.

The Python implementation provides a browser-based interface using Flask. The C++ implementation provides a command-line version that runs in the terminal. Both implementations use the same scheduling requirements so that their results can be compared.

## Scheduling Requirements

The scheduler follows these rules:

- 7 days per week.
- 3 shifts per day: Morning, Afternoon, and Evening.
- At least 2 employees are assigned to each shift.
- An employee can work a maximum of 1 shift per day.
- An employee can work a maximum of 5 days per week.
- Employee shift preferences are considered.
- Shift conflicts are detected and resolved when preferences cannot all be satisfied.
- A complete schedule requires at least 9 employees.

With 9 employees, 7 days, and 3 shifts per day with 2 employees per shift:

```
7 × 3 × 2 = 42
```

a total of 42 shift assignments are required.

## Files and Folder Structure

```
Assignment4/
├── employee_scheduler.py
├── employee_scheduler.cpp
├── employee_scheduler.exe
├── python_schedule.png
├── cpp_schedule.png
├── 01_environment_python_cpp_git_versions.png
├── 02_flask_version_check.png
├── 03_assignment4_project_structure.png
├── 04_cpp_program_start.png
├── 05_cpp_employee_input.png
├── 06_cpp_employee_input_continued.png
├── 07_cpp_schedule_generation.png
├── 08_cpp_schedule_summary.png
├── 09_cpp_weekly_schedule_part2.png
├── 10_cpp_weekly_schedule_part3.png
├── 11_cpp_employee_workload.png
├── 12_cpp_conflict_handling.png
├── 13_cpp_conflict_handling_final.png
├── 14_cpp_validation_pass.png
├── 15_python_browser_input_top.png
├── 16_python_browser_input_bottom.png
├── 17_python_browser_input_continued.png
├── 18_python_schedule_generated.png
└── README.md
```

## Software Used

- Python 3.11
- Flask 3.1.0
- C++17
- GCC 16.2.0
- Visual Studio Code
- Git / GitHub
- Web browser (for the Python Flask application)

## Python Version

The Python implementation is contained in `employee_scheduler.py`.

The Python application uses Flask to provide a web interface. When the program is started, the local Flask server runs at:

```
http://127.0.0.1:5000
```

Open this address in a web browser to use the scheduler.

### Run the Python Application

Open a terminal in the `Assignment4` directory and run:

```bash
python employee_scheduler.py
```

The terminal displays the Flask server address. Open the address in a browser and enter the employee information and shift preferences. The browser interface allows the user to generate a weekly schedule and view the schedule summary and final assignments.

## C++ Version

The C++ implementation is contained in `employee_scheduler.cpp`.

The C++ program provides a command-line implementation of the same scheduling problem. It accepts employee names and daily shift preferences, generates a weekly schedule, displays employee workloads, reports scheduling conflicts, and validates the scheduling constraints.

### Compile the C++ Program

From the `Assignment4` directory:

```bash
g++ -std=c++17 employee_scheduler.cpp -o employee_scheduler.exe
```

### Run the C++ Program

```bash
.\employee_scheduler.exe
```

The program prompts the user for the number of employees, employee names, and shift preferences.

The preference choices are:

```
1 = Morning
2 = Afternoon
3 = Evening
4 = No Preference
```

## Validation

The C++ implementation was tested successfully. The validation output confirmed:

```
[PASS] Minimum 2 employees per shift
[PASS] Maximum 1 shift per employee per day
[PASS] Maximum 5 working days per employee
[PASS] Shift conflict detection and resolution
```

The program also reported `SCHEDULE GENERATED SUCCESSFULLY`.

The Python browser application was also successfully run through Flask and displayed the generated weekly schedule in the browser.

## Screenshots

The screenshots in this repository document the implementation and testing process.

The primary screenshots are:

- `python_schedule.png` — final Python/Flask schedule displayed in the browser.
- `cpp_schedule.png` — final C++ schedule and validation output.

Additional screenshots document the software environment, project structure, employee input, schedule generation, workload information, conflict handling, and validation results.

## Comparison of the Implementations

The Python version uses Flask to provide a browser-based user interface, making the scheduler accessible through a web browser on the local computer. The C++ version focuses on the scheduling logic through a terminal-based interface.

Both implementations address the same scheduling constraints and generate a weekly schedule. The two-language implementation demonstrates how the same scheduling problem can be implemented using different programming languages and execution environments.

## How to Use the Project

1. Open the `Assignment4` folder in Visual Studio Code.
2. Confirm that Python, Flask, and the C++ compiler are installed.
3. Run the Python version with `python employee_scheduler.py`.
4. Open `http://127.0.0.1:5000` in a web browser.
5. Enter employee information and shift preferences.
6. Generate and review the weekly schedule.
7. Compile the C++ version with `g++ -std=c++17 employee_scheduler.cpp -o employee_scheduler.exe`.
8. Run the C++ version with `.\employee_scheduler.exe`.
9. Review the generated schedule and validation results.

## Author

Pranavi Balakulla — Assignment 4, Employee Shift Scheduler
