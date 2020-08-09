# XML-Reader
Application consists of main menu and table view. Table view shows all records from the table in database. User can edit/remove/export records and import XML files. All changed are updated in the database.
## Implementation description
Application is built on Model/View architecture. Database (SQLite) is stored in a local file. Import, export and working with database are performed in separate threads. Communication between threads is implemented with signal-slot connections.
