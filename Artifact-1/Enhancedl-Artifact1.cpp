#include "CourseManager.h"
#include <string>
using namespace std;

int main() {
    CourseManager manager;
    int choice = 0;
    string fileName;
    string courseNumber;

    cout << "Enter the CSV file path: ";
    getline(cin, fileName);

    do {
        cout << "\n1. Load course data\n";
        cout << "2. Print all courses\n";
        cout << "3. Print course information\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(20, '\n');
            choice = 0;
        }

        switch (choice) {
        case 1:
            manager.loadCourses(fileName);
            manager.validatePrerequisites();
            break;

        case 2:
            manager.printCourseList();
            break;

        case 3:
            if (manager.isEmpty()) {
                cout << "Error: No course data loaded." << endl;
                break;
            }
            cout << "Enter course number: ";
            cin >> courseNumber;
            manager.searchCourse(courseNumber);
            break;

        case 9:
            cout << "Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice." << endl;
        }

    } while (choice != 9);

    return 0;
}