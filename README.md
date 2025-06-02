# SnappFood – Console-Based Food Delivery System

## Overview

**SnappFood** is a console-based simulation of an online food delivery platform. The project mimics real-world functionalities such as user registration, login/logout, restaurants, food management, discount codes, and administrative operations. The application supports three roles:

- Admin
- Restaurant
- Customer

It features robust input validation, modular design, and exception handling.

## Features

### User Management

- Customer and Restaurant registration
- Login and logout
- Password change with strength and format checks
- Account deletion

### Restaurant Management

- Add and remove restaurants by Admin
- Add and remove food items (starter, entree, dessert)
- Food price and cost validation
- Restaurant filtering by type

### Discounts and Budget

- Admin can set discount codes for specific customers
- Customers can view and redeem discount codes
- Budget tracking for all users

### Account and Orders

- Customers and restaurants can charge their accounts
- Food menu managed per restaurant
- Customer cart and ordering (partially implemented in structure)

## Code Structure

### Classes

- Snappfood: Main application class
- Admin, User, Customer, Restaurant: User roles with inheritance
- Food, Items, Discount: Supporting data structures
- Exception: Custom exception handler

### Core Functions

- get_command(): Parses and executes user commands
- set_admin_information(): Initializes the admin account
- register_customer, login_user, change_password, logout, etc.

### Validation Utilities

- is_user_pass_correct(): Checks allowed characters
- is_pass_strong(): Ensures password includes lowercase, uppercase, and digits
- is_type_correct(): Validates input type names

## How to Run

1. Compile the project using a C++ compiler:

```bash
g++ snappfood.cpp -o snappfood
```

2. Run the executable:

```bash
./snappfood
```

3. Interact with the program using commands like:

```
register john AbC123
login john AbC123
enter customer menu
charge account 500
```

You will be prompted at the start to set the admin username and password.

## Notes

- The system maintains an internal menu state (login menu, main menu, customer menu, etc.) to control command access.
- Access control is strictly enforced via exceptions.
- This version is single-session and operates via standard input/output.

## Future Improvements

- Implement customer cart operations and order placement
- Add persistent storage (file or database)
- Support for restaurant order history
- GUI or web-based frontend.
