import sys
from sqlalchemy.exc import SQLAlchemyError
from models import Student, Menu, Attendance, Bill, Feedback
from database import Session, init_db
from datetime import date

# Initialize database
init_db()

def main():
    print("Welcome to the Hostel Mess Management System")
    while True:
        print("\nMain Menu:")
        print("1. Register for Mess")
        print("2. View Menu")
        print("3. Mark Attendance")
        print("4. Generate/View Bill")
        print("5. Submit/View Feedback")
        print("6. Exit")
        
        choice = input("Select an option: ")
        
        if choice == '1':
            register_for_mess()
        elif choice == '2':
            view_menu()
        elif choice == '3':
            mark_attendance()
        elif choice == '4':
            generate_view_bill()
        elif choice == '5':
            submit_view_feedback()
        elif choice == '6':
            print("Exiting...")
            break
        else:
            print("Invalid option. Please try again.")

def register_for_mess():
    session = Session()
    try:
        name = input("Enter your name to register: ")
        student = Student(name=name, registered=True)
        session.add(student)
        session.commit()
        print(f"{name} registered successfully.")
    except SQLAlchemyError as e:
        session.rollback()
        print(f"Error: {e}")
    finally:
        session.close()

def view_menu():
    session = Session()
    try:
        menu = session.query(Menu).first()
        if menu:
            print("Today's Menu:")
            print(menu.details)
        else:
            print("Menu not available.")
    except SQLAlchemyError as e:
        print(f"Error: {e}")
    finally:
        session.close()

def mark_attendance():
    session = Session()
    try:
        student_id = int(input("Enter your student ID to mark attendance: "))
        student = session.query(Student).get(student_id)
        if student and student.registered:
            attendance = Attendance(student_id=student_id, date=date.today(), attended=True)
            session.add(attendance)
            session.commit()
            print("Attendance marked.")
        else:
            print("Student not registered.")
    except (SQLAlchemyError, ValueError) as e:
        session.rollback()
        print(f"Error: {e}")
    finally:
        session.close()

def generate_view_bill():
    session = Session()
    try:
        student_id = int(input("Enter your student ID to view bill: "))
        bill = session.query(Bill).filter_by(student_id=student_id).first()
        if bill:
            print(f"Your bill amount is: {bill.amount}")
        else:
            print("No bill available.")
    except (SQLAlchemyError, ValueError) as e:
        print(f"Error: {e}")
    finally:
        session.close()

def submit_view_feedback():
    session = Session()
    try:
        student_id = int(input("Enter your student ID to submit/view feedback: "))
        student = session.query(Student).get(student_id)
        if student:
            print("1. Submit Feedback")
            print("2. View Feedback")
            feedback_choice = input("Select an option: ")
            if feedback_choice == '1':
                content = input("Enter your feedback: ")
                feedback = Feedback(student_id=student_id, content=content)
                session.add(feedback)
                session.commit()
                print("Feedback submitted.")
            elif feedback_choice == '2':
                feedbacks = session.query(Feedback).filter_by(student_id=student_id).all()
                print("Your feedback:")
                for fb in feedbacks:
                    print(f"- {fb.content}")
            else:
                print("Invalid option.")
        else:
            print("Student not found.")
    except (SQLAlchemyError, ValueError) as e:
        session.rollback()
        print(f"Error: {e}")
    finally:
        session.close()

if __name__ == "__main__":
    main()
