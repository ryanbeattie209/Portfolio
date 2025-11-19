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

## 📥 Installation Instructions
1. Clone the repository:
   ```bash
   git clone <repo-url>
   ```
2. Navigate to the project directory:
   ```bash
   cd welsh-language-learning-app
   ```
3. Configure database settings in `application.properties`.
4. Build and run the application:
   ```bash
   mvn spring-boot:run
   ```

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
