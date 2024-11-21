from database import *
import datetime
from sqlalchemy.exc import IntegrityError


def get_choice(prompt, options):
    while True:
        try:
            choice = int(input(prompt))
            if choice in options:
                return choice
            else:
                print("Invalid choice. Please try again.")
        except ValueError:
            print("Please enter a valid number.")


def register_user():
    try:
        username = input("Enter username: ")
        password = input("Enter password: ")

        print("Select role:")
        print("1. Admin")
        print("2. Student")
        print("3. GUB")

        role_choice = get_choice("Enter role (1-3): ", {1, 2, 3})
        role_dict = {1: "Admin", 2: "Student", 3: "GUB"}
        role = role_dict[role_choice]

        user = User(username=username, password=password, role=role)
        session.add(user)
        session.commit()
        print("User registered successfully!")
    except IntegrityError:
        session.rollback()
        print("Username already exists.")
    except Exception as e:
        print(f"An error occurred: {e}")


def login():
    try:
        username = input("Username: ")
        password = input("Password: ")
        user = session.query(User).filter_by(username=username, password=password).first()
        if user:
            print(f"Logged in as {username} ({user.role})")
            return user
        else:
            print("Invalid credentials!")
            return None
    except Exception as e:
        print(f"An error occurred: {e}")
        return None


def admin_actions(admin_id):
    while True:
        try:
            print("\nAdmin Actions:")
            print("1. Count votes")
            print("2. Raise notices")
            print("3. View notices")
            print("4. Add budget")
            print("5. View events")
            print("6. Add candidate")
            print("7. Logout")
            choice = get_choice("Select an action (1-7): ", {1, 2, 3,4,5,6,7})

            if choice==1:
                votesR=session.query(Votes).all()
                for vote in votesR:
                    print(f"Candidate: {vote.cname} \t Votes: {vote.votes}\n")
            elif choice == 2:
                desc = input("Enter notice description: ")
                date_str = input("Enter date (YYYY-MM-DD): ")
                date = datetime.datetime.strptime(date_str, "%Y-%m-%d").date()
                notice = Notices(notice=desc,author_id=admin_id,date=date)
                session.add(notice)
                session.commit()
                print("Notice created successfully!")
            elif choice == 3:
                notices = session.query(Notices).all()
                print("\nNotices:")
                for notice in notices:
                    print(f"Notice id: {notice.id}.\nDate:{notice.date} \nDesc: {notice.notice}\n")
            elif choice == 4:
                try:
                    bugName=input("Enter budget name: ")
                    amount=int(input("Enter amount: "))
                    bug=Budget(name=bugName,amount=amount)
                    session.add(bug)
                    session.commit()
                    print("Budget added successfully!")
                except IntegrityError:
                    session.rollback()
                    print("Budget already exists.")
                except Exception as e:
                    print(f"An error occurred: {e}")
            elif choice==5:
                events = session.query(Events).all()
                print("\nEvents:")
                for event in events:
                    print(f"\nName:{event.name} Date: {event.date}\n")
            elif choice==6:
                cname=input("Enter candidate name:")
                cand=Votes(cname=cname,votes=0)
                session.add(cand)
                session.commit()
            elif choice == 7:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")


def student_actions():
    while True:
        try:
            print("\nStudent Actions:")
            print("1. Cast Votes")
            print("2. View Notices")
            print("3. View Events")
            print("4. Logout")
            choice = get_choice("Select an action (1-4): ", {1, 2,3,4})

            if choice == 1:
                votes=session.query(Votes).all()
                for c in votes:
                    print(f"Candidate name: {c.cname}")
                cname=input("Enter candidate name for whom you want to cast vote:")
                votes=session.query(Votes).filter_by(cname=cname).first()
                votes.votes=votes.votes+1
                session.add(votes)
                session.commit()
                print("Vote casted successfully")
            elif choice == 2:
                notices = session.query(Notices).all()
                print("\nNotices:")
                for notice in notices:
                    print(f"Notice id: {notice.id}.\nDate:{notice.date} \nDesc: {notice.notice}\n")
            elif choice == 3:
                events = session.query(Events).all()
                print("\nEvents:")
                for event in events:
                    print(f"\nName:{event.name} Date: {event.date}\n")
            elif choice == 4:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")


def gub_actions():
    while True:
        try:
            print("\nGUB Actions:")
            print("1. View budget")
            print("2. Organize events")
            print("3. Raise notices")
            print("4. View Notices")
            print("5. View events")
            print("6. Logout")
            choice = get_choice("Select an action (1-6): ", {1, 2,3,4,5,6})

            if choice == 1:
                budget = session.query(Budget).all()
                print("\nBudget:")
                for m in budget:
                    print(f"Name: {m.name} Amount:{m.amount}\n")
            elif choice==2:
                name = input("Enter event name: ")
                date_str = input("Enter date (YYYY-MM-DD): ")
                date = datetime.datetime.strptime(date_str, "%Y-%m-%d").date()
                event = Events(name=name,date=date)
                session.add(event)
                session.commit()
                print("Event created successfully!")
            elif choice == 3:
                desc = input("Enter notice description: ")
                date_str = input("Enter date (YYYY-MM-DD): ")
                date = datetime.datetime.strptime(date_str, "%Y-%m-%d").date()
                notice = Notices(notice=desc,author_id=admin_id,date=date)
                session.add(notice)
                session.commit()
                print("Notice created successfully!")
            elif choice == 4:
                notices = session.query(Notices).all()
                print("\nNotices:")
                for notice in notices:
                    print(f"Notice id: {notice.id}.\nDate:{notice.date} \nDesc: {notice.notice}\n")
            elif choice==5:
                events = session.query(Events).all()
                print("\nEvents:")
                for event in events:
                    print(f"\nName:{event.name} Date: {event.date}\n")
            elif choice == 6:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")


def main():
    print("Welcome to the NITS Gymkhana Body System!")
    while True:
        try:
            print("\n1. Register")
            print("2. Login")
            print("3. Exit")
            choice = get_choice("Select an option (1-3): ", {1, 2, 3})

            if choice == 1:
                register_user()
            elif choice == 2:
                user = login()
                if user:
                    if user.role == "Admin":
                        admin_actions(admin_id=user.id)
                    elif user.role == "Student":
                        student_actions()
                    elif user.role == "GUB":
                        gub_actions()
            elif choice == 3:
                print("Goodbye!")
                break
        except Exception as e:
            print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
