# 💻 C++ OOP Projects & Systems

<p align="left">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/Paradigm-OOP-orange?style=for-the-badge" alt="OOP" />
  <img src="https://img.shields.io/badge/Architecture-Modular%20(.h%2F.cpp)-blue?style=for-the-badge" alt="Modular" />
</p>

## 📌 Overview

A collection of console-based management systems built using **C++** and **Object-Oriented Programming (OOP)** principles. The focus of this repository is code organization, class interaction, modular structure (`.h` and `.cpp`), and persistent storage using File I/O.

---

## 🛠 Concepts Implemented

- **Encapsulation & Data Hiding:** Protecting class attributes via `private` access modifiers and getters/setters.
- **Abstraction:** Separating declarations (`.h`) from implementation (`.cpp`).
- **Friend Classes & Functions:** Granting controlled access between closely related subsystem classes.
- **Modular Project Structure:** Organizing code into multi-file projects rather than single-file scripts.
- **File I/O:** Reading and writing system state to disk for persistence.

---

## 🗂 Projects

| Project | Status | Description | Key Modules / Classes |
| :--- | :---: | :--- | :--- |
| **Askme System** | 🟡 Improving | Q&A platform for managing users, questions, and threads (~650 lines). | `User`, `Question`, `QuestionManager` |
| **Online Book Reader** | 🟡 In Progress | Digital reading platform with user sessions, book navigation, and reading history. | `User`, `Book`, `ReadingSession`, `ReaderManager` |
| **Library Borrowing System** | ⏳ Planned | Tracks books, member profiles, borrowing limits, and delay fines. | `Book`, `User`, `BorrowManager` |
| **Car Rental System** | ⏳ Planned | Fleet operations, customer management, and rental calculation. | `Car`, `Customer`, `RentalContract` |
| **Clinic Management System** | ⏳ Planned | Schedules doctor appointments, medical history, and prescriptions. | `Doctor`, `Patient`, `Appointment` |
| **Supermarket Inventory & POS** | ⏳ Planned | Inventory management, product tracking, and receipt generation. | `Product`, `Inventory`, `Invoice` |
| **University Management** | ⏳ Planned | Handles student course registration, prerequisites, and GPA. | `Student`, `Course`, `EnrollmentManager` |

---

## 📂 Project Structure Standard

All projects in this repository follow a modular multi-file format:

```text
ProjectFolder/
│
├── Header Files (.h)         --> Declarations & Class Layouts
│   ├── ClassName.h
│
├── Implementation (.cpp)     --> Definition of Functions
│   ├── ClassName.cpp
│
└── main.cpp                  --> Entry Point & CLI Logic
