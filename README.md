# Quiz-Core

**A CLI quizzing application for both students and teachers**

Quiz-Core is a comprehensive terminal-based examination and quiz system featuring encrypted question databases, affine-cipher progress tracking, and secure role-based authentication for Students, Teachers, and Administrators.

---

## Features

- **Multi-Engine Quizzing System**:
  - **MCQ Engine**: 4-option multiple choice questions with affine encryption.
  - **True/False Engine**: Binary choice statements scored with instant feedback.
  - **Fill in the Blanks Engine**: Free-form text blanks evaluated with case-insensitive, whitespace-trimmed matching.
- **Role-Based Authentication**:
  - **Student Login**: Authenticate using encrypted class databases (e.g. `CSE-2025-B-B1.dat`) with student roll number and password. Automatically logs quiz attempts and tracks individual progress.
  - **Teacher Login**: Authenticate with department database (e.g. `CSE_teachers.dat`), encrypt new quiz files from plaintext templates, monitor student performance, and export progress reports.
  - **Administrator Dashboard**: Manage, encrypt, decrypt, and append student and teacher login databases.
- **Timer & Scoring System**: Configurable per-question time limits with live countdown tracking, accuracy scoring, and auto-handling of unanswered questions upon timeout.
- **Encrypted Storage**: All question banks, progress records, and credentials are encrypted using modular affine ciphers.

---

## Compilation

Build the application using `make`:

```bash
make clean
make
```

Run the application:

```bash
./quiz-core
```

---

## Question File Formats

### 1. Multiple Choice Questions (MCQ)
Each question consists of 6 lines:
```
[Question Statement]
[Option A]
[Option B]
[Option C]
[Option D]
[Correct Option: A/B/C/D]
```

*Example (`Questions/mcq_sample.txt`):*
```
What is the cardinality of the set {a, b, c}?
A) 2
B) 3
C) 4
D) 0
B
```

### 2. True / False Questions (TF)
Each question consists of 2 lines:
```
[Statement]
[Answer: T or F]
```

*Example (`Questions/tf_sample.txt`):*
```
A binary search tree has an average search time complexity of O(log n).
T
A stack is a First-In-First-Out (FIFO) data structure.
F
```

### 3. Fill in the Blanks Questions (FIB)
Each question consists of 2 lines:
```
[Statement with blank (e.g. _______)]
[Expected Answer]
```

*Example (`Questions/blank_sample.txt`):*
```
The data structure that operates on a Last-In-First-Out (LIFO) basis is called a _______.
stack
In computer science, CPU stands for Central _______ Unit.
Processing
```

---

## Default Login Credentials

- **Students (`CSE-2025-B-B1.dat`)**:
  - Roll: `81`, Password: `JzK7RcxHS0` (Zawad Ibrahim)
  - Roll: `68`, Password: `PcbnaR8` (Maksudur Rahman)
  - Roll: `77`, Password: `yiwWrxc7` (Imran Ahmed)
- **Teachers (`CSE_teachers.dat`)**:
  - Name: `Karim Mia`, Password: `Dib9dWuBx`
  - Name: `Mohsin Ali`, Password: `Bib6dUtIp`
  - Name: `Ayon Saha`, Password: `7HJmxIXvx`
- **Administrator**:
  - Password: `admin`
