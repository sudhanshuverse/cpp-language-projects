# 🎓 Student Management System

- The Student Management System is a console-based C++ application to manage student records using a binary file.

- It allows users to add, view, update, and delete student details like roll number, name, course, and year.

- Designed with color-coded outputs and robust input validation for a better terminal experience.

## 🚀 Demo

#### 📸 Screenshots

![App Screenshot](https://github.com/your-username/Student-Management-System/blob/main/screenshot.png)  
*(Replace with your real screenshot or GitHub image link)*

## ✨ Features

- 📋 Add New Student – Enter roll, name, course, and year with proper validation.

- 🗑️ Delete Student – Remove a student using their roll number.

- ✏️ Update Record – Modify existing student data.

- 👁️ View All Students – See all records neatly formatted.

- 🔒 Input Validation – Prevents entry of invalid data (e.g., letters in roll no, special chars in names).

- 🎨 Colored Terminal Interface – Makes the console visually engaging using ANSI escape codes.

## 🕹️ How to Use

- **Add Student** – Choose option 1, then enter:
  - Roll Number: Numeric only (e.g., `101`)
  - Name: Alphabets and spaces only (e.g., `Sudhanshu Kumar`)
  - Course: Alphanumeric (e.g., `BCA`)
  - Year: FY, SY, TY or valid year (e.g., `2025`)

- **View Students** – Choose option 4 to display all records.

- **Update Record** – Option 3 allows editing based on roll number.

- **Delete Student** – Choose option 2 and enter the roll to remove.

- **Exit** – Option 5 exits the application.

## 📜 Application Rules

- Roll number must be unique and numeric.

- All fields must follow the required format (e.g., no digits in name, valid year, etc.).

- File `student.dat` is used to store all records in binary mode.

- If no record is found, the system shows an appropriate error.

- Proper validation and error handling ensure robust performance.

## 🛠️ Tech Stack

| Technology | Purpose                                  |
|------------|------------------------------------------|
| C++        | Core language for logic and structure    |
| STL        | File handling using `<fstream>`          |
| Regex      | Input validation for name, year, course  |
| ANSI Colors| Terminal output colorization             |


## 📧 Feedback

If you have any feedback or suggestions, feel free to reach out at:

**📬 Email:** sudhanshu95084145@gmail.com

---

🌟 *Star the repository on GitHub if you found it helpful!* 🌟
