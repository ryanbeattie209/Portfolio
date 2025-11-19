# Welsh Language Learning App

## 📌 Project Overview
A language learning application similar to Duolingo, focused on English ↔ Welsh translation. Built using Java for backend and HTML/CSS for the web interface, with MySQL for data storage.

## ✅ Key Features
- **Secure Login**: Username (no restrictions) and password (must include uppercase, number, special character).
- **Admin Functions**:
  - Add, edit, delete users and manage roles.
  - Add, edit, delete nouns (English/Welsh words with gender).
- **Role Management**: Roles include ADMIN, USER, and LECT.
- **Data Loading**: Initial nouns and permissions are preloaded.
- **Security**: Passwords encrypted using PasswordEncoder; Spring Security for authentication and authorization.

## 🛠 Technology Stack
- **Backend**: Java (Spring Boot, Spring Security)
- **Frontend**: HTML, CSS
- **Database**: MySQL
- **Frameworks**: Spring MVC, JPA

## 🚀 Usage Guide
- **Student**: Take tests, submit answers, view results.
- **Instructor**: View all results, manage nouns.
- **Admin**: Manage users, permissions, and nouns.

## 🔐 Security Details
- Passwords are encrypted using `PasswordEncoder`.
- Spring Security ensures authentication and role-based authorization.
- Roles: ADMIN, USER, LECT.

## 👥 Contributors
- Ryan Beattie
- Patryk Labuda
- Sanjana Sharma
