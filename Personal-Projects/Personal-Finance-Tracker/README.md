# 💰 Budget Tracker

A personal finance management application built with Flask and SQLite. Track your income, expenses, accounts, and budgets all in one place with a beautiful, intuitive interface.

## Features

### Core Functionality
- **Transaction Management**: Record and categorize income and expenses
- **Multiple Accounts**: Track checking, savings, credit cards, and investment accounts
- **Multi-Currency Support**: USD, EUR, GBP, JPY, CAD, and AUD
- **Data Export**: Export all transactions to CSV format
- **Tags System**: Organize transactions with custom tags

### Dashboard & Analytics
- Real-time financial overview with total income, expenses, and net savings
- Account balance tracking across all accounts
- Budget status monitoring with color-coded alerts
- Recent transaction history

### Security
- User authentication with secure password hashing
- Session-based login system
- Per-user data isolation

## Tech Stack

- **Backend**: Flask (Python)
- **Database**: SQLite3
- **Frontend**: JavaScript, HTML5, CSS3
- **Visualization**: Matplotlib (for charts)
- **Security**: Werkzeug (password hashing)

## Installation

### Prerequisites
- Python 3.7 or higher
- pip (Python package manager)

### Setup Steps

1. **Clone the repository**
```bash
git clone <repository-url>
cd budget-tracker
```

2. **Install dependencies**
```bash
pip install flask matplotlib werkzeug
```

3. **Run the application**
```bash
python app.py
```

4. **Access the application**
Open your browser and navigate to:
```
http://localhost:5000
```

## Project Structure

```
budget-tracker/
│
├── app.py                 # Flask backend with all routes and database logic
├── templates/
│   ├── index.html        # Main dashboard interface
│   └── login.html        # Login and registration page
├── static/
│   └── icon.png          # Application icon (optional)
└── budget_tracker.db     # SQLite database (created automatically)
```

## Database Schema

### Users Table
- `id`: Primary key
- `username`: Unique username
- `password`: Hashed password
- `currency`: User's preferred currency

### Transactions Table
- `id`: Primary key
- `user_id`: Foreign key to users
- `account_id`: Foreign key to accounts
- `date`: Transaction timestamp
- `category`: Transaction category
- `description`: Transaction description
- `amount`: Transaction amount
- `type`: Income or expense
- `tags`: Optional comma-separated tags

### Accounts Table
- `id`: Primary key
- `user_id`: Foreign key to users
- `name`: Account name
- `type`: Account type (checking, savings, credit, investment)
- `balance`: Current account balance

### Budget Limits Table
- `id`: Primary key
- `user_id`: Foreign key to users
- `category`: Budget category
- `limit_amount`: Budget limit
- `period`: Budget period (monthly, weekly, yearly)

### Savings Goals Table
- `id`: Primary key
- `user_id`: Foreign key to users
- `name`: Goal name
- `target_amount`: Target amount
- `current_amount`: Current progress
- `deadline`: Goal deadline

## Usage Guide

### First Time Setup
1. Navigate to the login page
2. Click "Register" to create a new account
3. Choose your preferred currency
4. Login with your credentials

### Adding Transactions
1. Select an account from the dropdown
2. Choose transaction type (Income/Expense)
3. Enter category, description, and amount
4. Optionally add tags for organization
5. Click "Add Income" or "Add Expense"

### Managing Accounts
1. Click "+ Add" in the Accounts section
2. Enter account name and type
3. Set initial balance
4. Submit to create the account

### Setting Budget Limits
1. Click "+ Add" in the Budget Limits section
2. Enter category name
3. Set limit amount and period
4. Track spending against limits with visual progress bars

### Exporting Data
Click the "📥 Export" button in the header to download all transactions as a CSV file.

## Configuration

### Changing the Secret Key
For production deployment, update the secret key in `app.py`:
```python
app.secret_key = 'your-secure-random-secret-key-here'
```

### Database Location
By default, the database is created as `budget_tracker.db` in the project root. To change this, update all `sqlite3.connect()` calls in `app.py`.

## API Endpoints

### Authentication
- `POST /login` - User login
- `POST /register` - User registration
- `GET /logout` - User logout

### Transactions
- `GET /api/transactions` - Get all user transactions
- `POST /api/transactions` - Create new transaction
- `PUT /api/transactions/<id>` - Update transaction
- `DELETE /api/transactions/<id>` - Delete transaction

### Accounts
- `GET /api/accounts` - Get all user accounts
- `POST /api/accounts` - Create new account

### Budget Management
- `GET /api/budget-limits` - Get budget limits
- `POST /api/budget-limits` - Set budget limit
- `GET /api/budget-status` - Get current budget status

### Settings
- `GET /api/user-settings` - Get user settings
- `POST /api/user-settings` - Update user settings

### Export
- `GET /api/export-csv` - Export transactions to CSV

## Security Considerations

- **Production Deployment**: Change the secret key before deploying
- **HTTPS**: Use HTTPS in production to protect user credentials
- **Environment Variables**: Store sensitive configuration in environment variables
- **Database Backups**: Regularly backup the SQLite database

## Future Enhancements

Potential features for future versions:
- Recurring transaction support
- Advanced reporting and analytics
- Bill reminders
- Investment portfolio tracking
- Mobile responsive design improvements
- Dark mode theme

## License

This project is provided as-is for personal and educational use.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Support

For questions or issues, please open an issue on the repository or contact the maintainer.
