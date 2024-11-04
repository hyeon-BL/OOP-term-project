## ATM System - Object-Oriented Programming Term Project

This repository contains the source code for an ATM system implemented as an object-oriented programming term project. The project simulates the core functionalities of an Automated Teller Machine (ATM), allowing users to perform various financial transactions.

**Key Features:**

* **Realistic Simulation:**  The system accurately models real-world ATM interactions, including card insertion, PIN verification, cash withdrawals, deposits, and balance inquiries.
* **Object-Oriented Design:** The project utilizes object-oriented principles to create a modular and maintainable codebase, with classes representing key entities such as users, accounts, ATMs, and banks.
* **External Bank Interaction:** The system simulates communication with external banks to retrieve and verify user information, ensuring data security and integrity.
* **Multiple Bank Support:**  The ATM system supports transactions from both primary and non-primary banks, mirroring the functionality of real-world multi-bank ATMs.
* **Transaction Handling:** The system handles various transaction types, including cash withdrawals, deposits, balance inquiries, and potentially bank transfers and check deposits.
* **Cash Management:** The ATM maintains a record of available cash and updates it based on user transactions, reflecting the dynamic nature of ATM cash levels.
* **Security Considerations:** The system incorporates basic security measures such as PIN verification to protect user accounts.

**Project Structure:**

The repository is organized into classes representing core components of the ATM system, including:

* **User:**  Manages user information (account number, password, card details).
* **Account:** Holds account details (balance, transaction history).
* **ATM:**  Simulates the ATM interface and functionalities.
* **Bank:** Represents the external bank system with account data.
* **Card:**  Models ATM cards and their associated functionalities.
* **Transaction:**  Handles different transaction types.

**Further Development:**

Potential future enhancements include:

* **GUI Development:** Implement a graphical user interface for a more user-friendly experience.
* **Advanced Transactions:** Add support for more complex transactions like bank transfers and bill payments.
* **Enhanced Security:** Implement more robust security measures, such as encryption and fraud detection.
* **Admin Functionality:**  Incorporate admin card functionalities for ATM management tasks.

This project provides a solid foundation for understanding object-oriented programming concepts and their application in simulating real-world systems.
