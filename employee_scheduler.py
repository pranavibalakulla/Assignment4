from flask import Flask, request, render_template_string
import random

app = Flask(__name__)

DAYS = [
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
]

SHIFTS = ["Morning", "Afternoon", "Evening"]

MIN_EMPLOYEES = 9
MIN_PER_SHIFT = 2
MAX_DAYS_PER_EMPLOYEE = 5


HTML = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Employee Shift Scheduler</title>

    <style>
        * {
            box-sizing: border-box;
        }

        body {
            margin: 0;
            font-family: Arial, sans-serif;
            background: #f4f7fb;
            color: #1f2937;
        }

        .header {
            background: linear-gradient(135deg, #1e3a8a, #2563eb);
            color: white;
            padding: 35px 20px;
            text-align: center;
        }

        .header h1 {
            margin: 0 0 10px;
            font-size: 34px;
        }

        .header p {
            margin: 0;
            font-size: 16px;
        }

        .container {
            width: 94%;
            max-width: 1400px;
            margin: 30px auto;
        }

        .card {
            background: white;
            border-radius: 14px;
            padding: 25px;
            margin-bottom: 25px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.08);
        }

        .card h2 {
            margin-top: 0;
            color: #1e3a8a;
        }

        .notice {
            background: #eff6ff;
            border-left: 5px solid #2563eb;
            padding: 15px;
            margin-bottom: 20px;
            border-radius: 6px;
        }

        .employee-grid {
            overflow-x: auto;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            min-width: 900px;
        }

        th {
            background: #1e3a8a;
            color: white;
            padding: 12px;
            text-align: center;
        }

        td {
            padding: 10px;
            border-bottom: 1px solid #e5e7eb;
            text-align: center;
        }

        td:first-child {
            font-weight: bold;
            text-align: left;
        }

        input[type="text"],
        select {
            width: 100%;
            padding: 9px;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            background: white;
        }

        .button-area {
            text-align: center;
            margin-top: 25px;
        }

        button {
            border: none;
            padding: 13px 25px;
            border-radius: 8px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            margin: 5px;
        }

        .generate {
            background: #2563eb;
            color: white;
        }

        .generate:hover {
            background: #1d4ed8;
        }

        .reset {
            background: #e5e7eb;
            color: #1f2937;
        }

        .error {
            background: #fee2e2;
            border-left: 5px solid #dc2626;
            color: #991b1b;
            padding: 15px;
            margin-bottom: 20px;
            border-radius: 6px;
        }

        .success {
            background: #dcfce7;
            border-left: 5px solid #16a34a;
            color: #166534;
            padding: 15px;
            margin-bottom: 20px;
            border-radius: 6px;
        }

        .schedule-table {
            overflow-x: auto;
        }

        .schedule-table table {
            min-width: 1000px;
        }

        .schedule-table td {
            vertical-align: top;
            min-width: 200px;
            height: 100px;
        }

        .employee {
            display: block;
            background: #eff6ff;
            border-radius: 6px;
            padding: 7px;
            margin: 4px 0;
            font-weight: bold;
        }

        .day-name {
            background: #dbeafe;
            color: #1e3a8a;
            font-weight: bold;
            font-size: 16px;
        }

        .stats {
            display: flex;
            flex-wrap: wrap;
            gap: 15px;
            margin-bottom: 20px;
        }

        .stat {
            flex: 1;
            min-width: 180px;
            background: #f8fafc;
            padding: 18px;
            border-radius: 10px;
            text-align: center;
        }

        .stat-number {
            font-size: 28px;
            font-weight: bold;
            color: #2563eb;
        }

        .footer {
            text-align: center;
            padding: 25px;
            color: #64748b;
            font-size: 14px;
        }

        @media (max-width: 700px) {
            .header h1 {
                font-size: 26px;
            }

            .container {
                width: 98%;
            }

            .card {
                padding: 15px;
            }
        }
    </style>
</head>

<body>

<div class="header">
    <h1>Employee Shift Scheduler</h1>
    <p>Weekly employee scheduling with shift preferences and scheduling constraints</p>
</div>

<div class="container">

    {% if error %}
        <div class="error">
            <strong>Error:</strong> {{ error }}
        </div>
    {% endif %}

    {% if schedule %}
        <div class="success">
            <strong>Schedule generated successfully!</strong>
            All shifts contain at least two employees, no employee works more
            than one shift per day, and no employee works more than five days.
        </div>

        <div class="card">
            <h2>Schedule Summary</h2>

            <div class="stats">
                <div class="stat">
                    <div class="stat-number">{{ employee_count }}</div>
                    <div>Employees</div>
                </div>

                <div class="stat">
                    <div class="stat-number">7</div>
                    <div>Days</div>
                </div>

                <div class="stat">
                    <div class="stat-number">3</div>
                    <div>Shifts per Day</div>
                </div>

                <div class="stat">
                    <div class="stat-number">42</div>
                    <div>Total Assignments</div>
                </div>
            </div>
        </div>

        <div class="card">
            <h2>Final Weekly Schedule</h2>

            <div class="schedule-table">
                <table>
                    <tr>
                        <th>Day</th>
                        <th>Morning</th>
                        <th>Afternoon</th>
                        <th>Evening</th>
                    </tr>

                    {% for day in days %}
                    <tr>
                        <td class="day-name">{{ day }}</td>

                        {% for shift in shifts %}
                        <td>
                            {% for employee in schedule[day][shift] %}
                                <span class="employee">{{ employee }}</span>
                            {% endfor %}
                        </td>
                        {% endfor %}
                    </tr>
                    {% endfor %}
                </table>
            </div>
        </div>

        <div class="card">
            <h2>Employee Workload</h2>

            <div class="schedule-table">
                <table>
                    <tr>
                        <th>Employee</th>
                        <th>Days Worked</th>
                        <th>Status</th>
                    </tr>

                    {% for employee, count in workload.items() %}
                    <tr>
                        <td>{{ employee }}</td>
                        <td>{{ count }}</td>
                        <td>
                            {% if count <= 5 %}
                                Within 5-Day Limit
                            {% else %}
                                Exceeds Limit
                            {% endif %}
                        </td>
                    </tr>
                    {% endfor %}
                </table>
            </div>
        </div>

        <div class="card">
            <h2>Scheduling Notes</h2>
            <ul>
                <li>Each shift has at least two employees.</li>
                <li>An employee can work only one shift per day.</li>
                <li>An employee can work a maximum of five days per week.</li>
                <li>Employee preferences are considered during assignment.</li>
                <li>If a preferred shift is full, another available shift is used.</li>
                <li>Random selection is used when multiple employees have equal priority.</li>
            </ul>
        </div>

        <div class="button-area">
            <a href="/">
                <button class="reset">Create Another Schedule</button>
            </a>
        </div>

    {% else %}

        <div class="card">
            <h2>Enter Employee Information</h2>

            <div class="notice">
                <strong>Important:</strong>
                At least 9 employees are required for a complete weekly schedule.
                The company requires 2 employees for each of 3 shifts across 7 days,
                while each employee can work a maximum of 5 days.
            </div>

            <form method="POST">

                <div class="employee-grid">
                    <table>
                        <tr>
                            <th>Employee Name</th>
                            {% for day in days %}
                                <th>{{ day }}</th>
                            {% endfor %}
                        </tr>

                        {% for i in range(12) %}
                        <tr>
                            <td>
                                <input
                                    type="text"
                                    name="name_{{ i }}"
                                    placeholder="Employee {{ i + 1 }}"
                                >
                            </td>

                            {% for day in days %}
                            <td>
                                <select name="pref_{{ i }}_{{ loop.index0 }}">
                                    <option value="">No Preference</option>
                                    <option value="Morning">Morning</option>
                                    <option value="Afternoon">Afternoon</option>
                                    <option value="Evening">Evening</option>
                                </select>
                            </td>
                            {% endfor %}
                        </tr>
                        {% endfor %}
                    </table>
                </div>

                <div class="button-area">
                    <button type="submit" class="generate">
                        Generate Weekly Schedule
                    </button>
                </div>

            </form>
        </div>

        <div class="card">
            <h2>Scheduling Rules</h2>

            <ul>
                <li>Company operates seven days per week.</li>
                <li>Available shifts are Morning, Afternoon, and Evening.</li>
                <li>Each employee works no more than one shift per day.</li>
                <li>Each employee works no more than five days per week.</li>
                <li>Each shift requires at least two employees per day.</li>
                <li>Additional employees are selected randomly when necessary.</li>
                <li>Shift conflicts are handled by assigning another available shift.</li>
            </ul>
        </div>

    {% endif %}

</div>

<div class="footer">
    Assignment 4 - Implementing Control Structures | Python Employee Scheduler
</div>

</body>
</html>
"""


def create_schedule(employees):
    """
    Generate a weekly schedule.

    The algorithm uses:
    - conditionals
    - loops
    - branching
    - random selection
    - preference handling
    - workload constraints
    """

    schedule = {
        day: {
            shift: []
            for shift in SHIFTS
        }
        for day in DAYS
    }

    workload = {
        employee["name"]: 0
        for employee in employees
    }

    # Process each day.
    for day_index, day in enumerate(DAYS):

        # Employees already working today.
        working_today = set()

        # Randomize equal-priority choices.
        shuffled_employees = employees.copy()
        random.shuffle(shuffled_employees)

        # Assign each shift.
        for shift in SHIFTS:

            # Find employees who:
            # 1. have not worked today
            # 2. have not reached 5 days
            available = [
                employee
                for employee in shuffled_employees
                if employee["name"] not in working_today
                and workload[employee["name"]] < MAX_DAYS_PER_EMPLOYEE
            ]

            # Give preference to employees who requested this shift.
            preferred = [
                employee
                for employee in available
                if employee["preferences"][day_index] == shift
            ]

            # Sort candidates by workload.
            preferred.sort(
                key=lambda employee: workload[employee["name"]]
            )

            available.sort(
                key=lambda employee: workload[employee["name"]]
            )

            selected = []

            # First select employees who preferred this shift.
            for employee in preferred:
                if len(selected) >= MIN_PER_SHIFT:
                    break

                if employee["name"] not in working_today:
                    selected.append(employee)

            # If fewer than two preferred employees are available,
            # select other available employees.
            if len(selected) < MIN_PER_SHIFT:

                remaining = [
                    employee
                    for employee in available
                    if employee["name"] not in {
                        selected_employee["name"]
                        for selected_employee in selected
                    }
                ]

                # Randomize candidates with the same workload.
                random.shuffle(remaining)

                remaining.sort(
                    key=lambda employee: workload[employee["name"]]
                )

                for employee in remaining:
                    if len(selected) >= MIN_PER_SHIFT:
                        break

                    selected.append(employee)

            # Store assignments.
            for employee in selected:
                name = employee["name"]

                schedule[day][shift].append(name)
                workload[name] += 1
                working_today.add(name)

        # Verify that all three shifts have the required staffing.
        for shift in SHIFTS:
            if len(schedule[day][shift]) < MIN_PER_SHIFT:
                raise ValueError(
                    f"Unable to staff {day} {shift} shift."
                )

    return schedule, workload


@app.route("/", methods=["GET", "POST"])
def index():

    if request.method == "POST":

        employees = []
        names_seen = set()

        # Read employee information from the form.
        for i in range(12):

            name = request.form.get(
                f"name_{i}",
                ""
            ).strip()

            if not name:
                continue

            # Prevent duplicate employee names.
            if name.lower() in names_seen:
                return render_template_string(
                    HTML,
                    days=DAYS,
                    shifts=SHIFTS,
                    schedule=None,
                    error=f"Duplicate employee name: {name}",
                    workload=None,
                    employee_count=0
                )

            names_seen.add(name.lower())

            preferences = []

            for day_index in range(7):
                preference = request.form.get(
                    f"pref_{i}_{day_index}",
                    ""
                )

                preferences.append(preference)

            employees.append({
                "name": name,
                "preferences": preferences
            })

        # At least 9 employees are required.
        if len(employees) < MIN_EMPLOYEES:
            return render_template_string(
                HTML,
                days=DAYS,
                shifts=SHIFTS,
                schedule=None,
                error=(
                    f"Please enter at least {MIN_EMPLOYEES} employees. "
                    f"The current number is {len(employees)}."
                ),
                workload=None,
                employee_count=len(employees)
            )

        try:
            schedule, workload = create_schedule(employees)

        except ValueError as error:
            return render_template_string(
                HTML,
                days=DAYS,
                shifts=SHIFTS,
                schedule=None,
                error=str(error),
                workload=None,
                employee_count=len(employees)
            )

        return render_template_string(
            HTML,
            days=DAYS,
            shifts=SHIFTS,
            schedule=schedule,
            workload=workload,
            error=None,
            employee_count=len(employees)
        )

    return render_template_string(
        HTML,
        days=DAYS,
        shifts=SHIFTS,
        schedule=None,
        error=None,
        workload=None,
        employee_count=0
    )


if __name__ == "__main__":
    print("=" * 60)
    print("EMPLOYEE SHIFT SCHEDULER")
    print("=" * 60)
    print("Starting Flask web application...")
    print("Open your browser and go to:")
    print("http://127.0.0.1:5000")
    print("=" * 60)

    app.run(
        debug=True,
        host="127.0.0.1",
        port=5000
    )