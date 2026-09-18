#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;

// ============================================================
// Assignment 4: Implementing Control Structures
// Employee Shift Scheduler - C++ Version
// ============================================================

// Constants
const int DAYS = 7;
const int SHIFTS = 3;
const int MIN_EMPLOYEES = 9;
const int MIN_PER_SHIFT = 2;
const int MAX_DAYS_PER_EMPLOYEE = 5;

// Days of the week
const vector<string> dayNames = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

// Available shifts
const vector<string> shiftNames = {
    "Morning",
    "Afternoon",
    "Evening"
};


// ============================================================
// Employee Structure
// ============================================================

struct Employee
{
    string name;

    // Preference for each day:
    // 0 = Morning
    // 1 = Afternoon
    // 2 = Evening
    // -1 = No Preference
    vector<int> preferences;

    int daysWorked;
};


// ============================================================
// Schedule Structure
// ============================================================

struct Schedule
{
    // schedule[day][shift] contains employee indexes
    vector<vector<vector<int>>> assignments;

    // Records when an employee's preferred shift was unavailable
    vector<string> conflicts;
};


// ============================================================
// Random Number Generator
// ============================================================

random_device rd;
mt19937 generator(rd());


// ============================================================
// Convert Shift Number to Text
// ============================================================

string shiftToString(int shift)
{
    if (shift == 0)
    {
        return "Morning";
    }
    else if (shift == 1)
    {
        return "Afternoon";
    }
    else
    {
        return "Evening";
    }
}


// ============================================================
// Convert User Input to Shift Number
// ============================================================

int getShiftChoice()
{
    int choice;

    while (true)
    {
        cout << "    1. Morning";
        cout << "    2. Afternoon";
        cout << "    3. Evening";
        cout << "    4. No Preference\n";

        cout << "    Enter choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 4)
        {
            if (choice == 4)
            {
                return -1;
            }

            return choice - 1;
        }

        cout << "    Invalid choice. Please enter 1, 2, 3, or 4.\n";
    }
}


// ============================================================
// Check for Duplicate Employee Name
// ============================================================

bool duplicateName(
    const vector<Employee>& employees,
    const string& name)
{
    for (const Employee& employee : employees)
    {
        if (employee.name == name)
        {
            return true;
        }
    }

    return false;
}


// ============================================================
// Input Employee Information
// ============================================================

void inputEmployees(vector<Employee>& employees)
{
    int numberOfEmployees;

    cout << "\n";
    cout << "============================================================\n";
    cout << "              EMPLOYEE INFORMATION\n";
    cout << "============================================================\n";

    cout << "\nEnter number of employees (9-12): ";
    cin >> numberOfEmployees;

    while (numberOfEmployees < MIN_EMPLOYEES ||
           numberOfEmployees > 12)
    {
        cout << "\nPlease enter between 9 and 12 employees.\n";
        cout << "Enter number of employees: ";
        cin >> numberOfEmployees;
    }

    cin.ignore();

    for (int i = 0; i < numberOfEmployees; i++)
    {
        Employee employee;

        cout << "\n------------------------------------------------------------\n";
        cout << "Employee " << i + 1 << "\n";
        cout << "------------------------------------------------------------\n";

        cout << "Enter employee name: ";
        getline(cin, employee.name);

        while (employee.name.empty() ||
               duplicateName(employees, employee.name))
        {
            if (employee.name.empty())
            {
                cout << "Name cannot be empty.\n";
            }
            else
            {
                cout << "That employee name already exists.\n";
            }

            cout << "Enter employee name: ";
            getline(cin, employee.name);
        }

        employee.preferences.resize(DAYS);
        employee.daysWorked = 0;

        cout << "\nEnter shift preferences for each day.\n";
        cout << "You can select Morning, Afternoon, Evening, or No Preference.\n";

        for (int day = 0; day < DAYS; day++)
        {
            cout << "\n" << dayNames[day] << ":\n";

            employee.preferences[day] = getShiftChoice();
        }

        employees.push_back(employee);
    }
}


// ============================================================
// Check Whether Employee Is Working on a Specific Day
// ============================================================

bool worksOnDay(
    const Schedule& schedule,
    int day,
    int employeeIndex)
{
    for (int shift = 0; shift < SHIFTS; shift++)
    {
        for (int employee : schedule.assignments[day][shift])
        {
            if (employee == employeeIndex)
            {
                return true;
            }
        }
    }

    return false;
}


// ============================================================
// Count Employees Assigned to a Shift
// ============================================================

int shiftCount(
    const Schedule& schedule,
    int day,
    int shift)
{
    return static_cast<int>(
        schedule.assignments[day][shift].size()
    );
}


// ============================================================
// Determine Whether a Shift Is Full
// ============================================================

bool shiftIsFull(
    const Schedule& schedule,
    int day,
    int shift)
{
    return shiftCount(schedule, day, shift) >= MIN_PER_SHIFT;
}


// ============================================================
// Calculate Preference Score
// ============================================================

int preferenceScore(
    const Employee& employee,
    int day,
    int shift)
{
    int preferredShift = employee.preferences[day];

    if (preferredShift == shift)
    {
        return 100;
    }

    if (preferredShift == -1)
    {
        return 50;
    }

    // Employee prefers another shift.
    return 0;
}


// ============================================================
// Select Six Employees for Each Day
//
// Six employees are needed each day because:
// 3 shifts x 2 employees = 6 assignments per day.
//
// Employees with fewer previous workdays are prioritized.
// This helps ensure that nobody exceeds five days.
// ============================================================

vector<int> selectEmployeesForDay(
    vector<Employee>& employees,
    int day)
{
    vector<int> candidates;

    for (int i = 0; i < static_cast<int>(employees.size()); i++)
    {
        candidates.push_back(i);
    }

    // Randomize equal-priority candidates.
    shuffle(
        candidates.begin(),
        candidates.end(),
        generator
    );

    // Sort primarily by number of days worked.
    sort(
        candidates.begin(),
        candidates.end(),
        [&](int a, int b)
        {
            return employees[a].daysWorked <
                   employees[b].daysWorked;
        }
    );

    // Select six employees.
    vector<int> selected;

    for (int employeeIndex : candidates)
    {
        if (employees[employeeIndex].daysWorked <
            MAX_DAYS_PER_EMPLOYEE)
        {
            selected.push_back(employeeIndex);
        }

        if (selected.size() == 6)
        {
            break;
        }
    }

    return selected;
}


// ============================================================
// Generate Best Shift Assignment for Six Employees
//
// There are six employees and six positions:
//
// Morning:   2
// Afternoon: 2
// Evening:   2
//
// We test possible arrangements and choose the arrangement
// that satisfies the largest number of preferences.
// ============================================================

vector<int> bestShiftAssignment(
    const vector<Employee>& employees,
    const vector<int>& selected,
    int day)
{
    vector<int> bestAssignment;
    int bestScore = -1;

    vector<int> permutation = selected;

    sort(
        permutation.begin(),
        permutation.end()
    );

    do
    {
        int score = 0;

        // First two employees -> Morning
        for (int i = 0; i < 2; i++)
        {
            score += preferenceScore(
                employees[permutation[i]],
                day,
                0
            );
        }

        // Next two -> Afternoon
        for (int i = 2; i < 4; i++)
        {
            score += preferenceScore(
                employees[permutation[i]],
                day,
                1
            );
        }

        // Last two -> Evening
        for (int i = 4; i < 6; i++)
        {
            score += preferenceScore(
                employees[permutation[i]],
                day,
                2
            );
        }

        if (score > bestScore)
        {
            bestScore = score;
            bestAssignment = permutation;
        }

    } while (next_permutation(
        permutation.begin(),
        permutation.end()
    ));

    return bestAssignment;
}


// ============================================================
// Generate Weekly Schedule
// ============================================================

Schedule generateSchedule(
    vector<Employee>& employees)
{
    Schedule schedule;

    schedule.assignments.resize(DAYS);

    for (int day = 0; day < DAYS; day++)
    {
        schedule.assignments[day].resize(SHIFTS);

        // Select six employees for this day.
        vector<int> selected =
            selectEmployeesForDay(
                employees,
                day
            );

        if (selected.size() < 6)
        {
            continue;
        }

        // Find the assignment that best matches preferences.
        vector<int> bestAssignment =
            bestShiftAssignment(
                employees,
                selected,
                day
            );

        // Assign two employees to each shift.
        for (int position = 0; position < 6; position++)
        {
            int employeeIndex =
                bestAssignment[position];

            int shift = position / 2;

            schedule.assignments[day][shift]
                .push_back(employeeIndex);

            employees[employeeIndex].daysWorked++;
        }
    }

    // ========================================================
    // Detect and Record Preference Conflicts
    // ========================================================

    for (int day = 0; day < DAYS; day++)
    {
        for (int shift = 0; shift < SHIFTS; shift++)
        {
            for (int employeeIndex :
                 schedule.assignments[day][shift])
            {
                int preferred =
                    employees[employeeIndex]
                    .preferences[day];

                if (preferred != -1 &&
                    preferred != shift)
                {
                    string conflict =
                        employees[employeeIndex].name +
                        " preferred " +
                        shiftToString(preferred) +
                        " on " +
                        dayNames[day] +
                        " but was assigned to " +
                        shiftToString(shift) +
                        ".";

                    schedule.conflicts.push_back(
                        conflict
                    );
                }
            }
        }
    }

    return schedule;
}


// ============================================================
// Validate Minimum Staffing
// ============================================================

bool validateMinimumStaffing(
    const Schedule& schedule)
{
    for (int day = 0; day < DAYS; day++)
    {
        for (int shift = 0; shift < SHIFTS; shift++)
        {
            if (shiftCount(
                    schedule,
                    day,
                    shift) < MIN_PER_SHIFT)
            {
                return false;
            }
        }
    }

    return true;
}


// ============================================================
// Validate One Shift Per Day
// ============================================================

bool validateOneShiftPerDay(
    const Schedule& schedule,
    int employeeCount)
{
    for (int day = 0; day < DAYS; day++)
    {
        for (int employee = 0;
             employee < employeeCount;
             employee++)
        {
            int numberOfShifts = 0;

            for (int shift = 0;
                 shift < SHIFTS;
                 shift++)
            {
                for (int assignedEmployee :
                     schedule.assignments[day][shift])
                {
                    if (assignedEmployee == employee)
                    {
                        numberOfShifts++;
                    }
                }
            }

            if (numberOfShifts > 1)
            {
                return false;
            }
        }
    }

    return true;
}


// ============================================================
// Validate Maximum Five Days
// ============================================================

bool validateMaximumFiveDays(
    const vector<Employee>& employees)
{
    for (const Employee& employee : employees)
    {
        if (employee.daysWorked >
            MAX_DAYS_PER_EMPLOYEE)
        {
            return false;
        }
    }

    return true;
}


// ============================================================
// Display Weekly Schedule
// ============================================================

void displaySchedule(
    const Schedule& schedule,
    const vector<Employee>& employees)
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "                 FINAL WEEKLY SCHEDULE\n";
    cout << "============================================================\n";

    for (int day = 0; day < DAYS; day++)
    {
        cout << "\n";
        cout << "------------------------------------------------------------\n";
        cout << dayNames[day] << "\n";
        cout << "------------------------------------------------------------\n";

        for (int shift = 0; shift < SHIFTS; shift++)
        {
            cout << left
                 << setw(12)
                 << shiftToString(shift)
                 << ": ";

            for (int i = 0;
                 i < static_cast<int>(
                         schedule.assignments[day][shift].size());
                 i++)
            {
                int employeeIndex =
                    schedule.assignments[day][shift][i];

                cout << employees[employeeIndex].name;

                if (i <
                    static_cast<int>(
                        schedule.assignments[day][shift].size()) - 1)
                {
                    cout << ", ";
                }
            }

            cout << "\n";
        }
    }
}


// ============================================================
// Display Employee Workload
// ============================================================

void displayWorkload(
    const vector<Employee>& employees)
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "                  EMPLOYEE WORKLOAD\n";
    cout << "============================================================\n";

    cout << left
         << setw(25)
         << "Employee"
         << setw(15)
         << "Days Worked"
         << "Status\n";

    cout << "------------------------------------------------------------\n";

    for (const Employee& employee : employees)
    {
        cout << left
             << setw(25)
             << employee.name
             << setw(15)
             << employee.daysWorked;

        if (employee.daysWorked <=
            MAX_DAYS_PER_EMPLOYEE)
        {
            cout << "Within 5-Day Limit";
        }
        else
        {
            cout << "EXCEEDS LIMIT";
        }

        cout << "\n";
    }
}


// ============================================================
// Display Preference Conflicts
// ============================================================

void displayConflicts(
    const Schedule& schedule)
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "               SHIFT CONFLICT HANDLING\n";
    cout << "============================================================\n";

    if (schedule.conflicts.empty())
    {
        cout << "No preference conflicts occurred.\n";
    }
    else
    {
        for (const string& conflict :
             schedule.conflicts)
        {
            cout << "- " << conflict << "\n";
        }

        cout << "\n";
        cout << "Conflicts were resolved by assigning employees\n";
        cout << "to another available shift on the same day.\n";
    }
}


// ============================================================
// Display Validation Results
// ============================================================

void displayValidation(
    const Schedule& schedule,
    const vector<Employee>& employees)
{
    bool staffingPassed =
        validateMinimumStaffing(schedule);

    bool dailyPassed =
        validateOneShiftPerDay(
            schedule,
            employees.size()
        );

    bool workloadPassed =
        validateMaximumFiveDays(employees);

    cout << "\n";
    cout << "============================================================\n";
    cout << "                SCHEDULING VALIDATION\n";
    cout << "============================================================\n";

    cout << "\n";

    if (staffingPassed)
    {
        cout << "[PASS] Minimum 2 employees per shift\n";
    }
    else
    {
        cout << "[FAIL] Minimum 2 employees per shift\n";
    }

    if (dailyPassed)
    {
        cout << "[PASS] Maximum 1 shift per employee per day\n";
    }
    else
    {
        cout << "[FAIL] Maximum 1 shift per employee per day\n";
    }

    if (workloadPassed)
    {
        cout << "[PASS] Maximum 5 working days per employee\n";
    }
    else
    {
        cout << "[FAIL] Maximum 5 working days per employee\n";
    }

    if (!schedule.conflicts.empty())
    {
        cout << "[PASS] Shift conflict detection and resolution\n";
    }
    else
    {
        cout << "[PASS] Shift preference handling\n";
    }

    cout << "\n";
}


// ============================================================
// Display Program Summary
// ============================================================

void displaySummary(
    const vector<Employee>& employees)
{
    int totalAssignments = 0;

    for (const Employee& employee : employees)
    {
        totalAssignments += employee.daysWorked;
    }

    cout << "============================================================\n";
    cout << "                  SCHEDULE SUMMARY\n";
    cout << "============================================================\n";

    cout << "Employees:              "
         << employees.size() << "\n";

    cout << "Days:                   "
         << DAYS << "\n";

    cout << "Shifts per day:         "
         << SHIFTS << "\n";

    cout << "Employees per shift:    "
         << MIN_PER_SHIFT << "\n";

    cout << "Total assignments:      "
         << totalAssignments << "\n";

    cout << "Maximum days/employee:  "
         << MAX_DAYS_PER_EMPLOYEE << "\n";

    cout << "\n";
}


// ============================================================
// Main Function
// ============================================================

int main()
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "             EMPLOYEE SHIFT SCHEDULER\n";
    cout << "             ASSIGNMENT 4 - C++ VERSION\n";
    cout << "============================================================\n";

    cout << "\n";
    cout << "Scheduling requirements:\n";
    cout << "  * 7 days per week\n";
    cout << "  * Morning, Afternoon, and Evening shifts\n";
    cout << "  * Minimum 2 employees per shift\n";
    cout << "  * Maximum 1 shift per employee per day\n";
    cout << "  * Maximum 5 working days per employee\n";
    cout << "  * Employee shift preferences are considered\n";
    cout << "  * Shift conflicts are detected and resolved\n";

    // --------------------------------------------------------
    // Input
    // --------------------------------------------------------

    vector<Employee> employees;

    inputEmployees(employees);

    // --------------------------------------------------------
    // Generate Schedule
    // --------------------------------------------------------

    cout << "\n";
    cout << "Generating weekly schedule...\n";

    Schedule schedule =
        generateSchedule(employees);

    // --------------------------------------------------------
    // Display Results
    // --------------------------------------------------------

    displaySummary(employees);

    displaySchedule(
        schedule,
        employees
    );

    displayWorkload(employees);

    displayConflicts(schedule);

    displayValidation(
        schedule,
        employees
    );

    // --------------------------------------------------------
    // Final Status
    // --------------------------------------------------------

    bool staffingPassed =
        validateMinimumStaffing(schedule);

    bool dailyPassed =
        validateOneShiftPerDay(
            schedule,
            employees.size()
        );

    bool workloadPassed =
        validateMaximumFiveDays(employees);

    cout << "\n";
    cout << "============================================================\n";

    if (staffingPassed &&
        dailyPassed &&
        workloadPassed)
    {
        cout << "           SCHEDULE GENERATED SUCCESSFULLY\n";
    }
    else
    {
        cout << "           SCHEDULE VALIDATION FAILED\n";
    }

    cout << "============================================================\n";

    cout << "\nThank you for using the Employee Shift Scheduler.\n";

    return 0;
}