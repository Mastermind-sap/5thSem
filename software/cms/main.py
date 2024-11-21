from models import User, Conference, Paper, Review, session
import datetime
from sqlalchemy.exc import IntegrityError

# Helper function to safely get user choice with validation
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

# Register user
def register_user():
    try:
        username = input("Enter username: ")
        password = input("Enter password: ")

        print("Select role:")
        print("1. Organizer")
        print("2. Author")
        print("3. Reviewer")
        print("4. Attendee")

        role_choice = get_choice("Enter role (1-4): ", {1, 2, 3, 4})
        role_dict = {1: "Organizer", 2: "Author", 3: "Reviewer", 4: "Attendee"}
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

# Login user
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

# Organizer actions
def organizer_actions():
    while True:
        try:
            print("\nOrganizer Actions:")
            print("1. Create Conference")
            print("2. View Conferences")
            print("3. Logout")
            choice = get_choice("Select an action (1-3): ", {1, 2, 3})

            if choice == 1:
                name = input("Enter conference name: ")
                date_str = input("Enter date (YYYY-MM-DD): ")
                date = datetime.datetime.strptime(date_str, "%Y-%m-%d").date()
                location = input("Enter location: ")
                conference = Conference(name=name, date=date, location=location)
                session.add(conference)
                session.commit()
                print("Conference created successfully!")
            elif choice == 2:
                conferences = session.query(Conference).all()
                print("\nConferences:")
                for conf in conferences:
                    print(f"{conf.id}. {conf.name} on {conf.date} at {conf.location}")
            elif choice == 3:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")

# Author actions
def author_actions(author_id):
    while True:
        try:
            print("\nAuthor Actions:")
            print("1. Submit Paper")
            print("2. Logout")
            choice = get_choice("Select an action (1-2): ", {1, 2})

            if choice == 1:
                conferences = session.query(Conference).all()
                if not conferences:
                    print("No conferences available.")
                    continue

                print("\nAvailable Conferences:")
                for conf in conferences:
                    print(f"{conf.id}. {conf.name} on {conf.date} at {conf.location}")
                
                conf_id = get_choice("Select conference ID to submit paper to: ", {conf.id for conf in conferences})
                conference = session.query(Conference).get(conf_id)

                title = input("Enter paper title: ")
                abstract = input("Enter paper abstract: ")
                paper = Paper(title=title, abstract=abstract, status="Pending", author_id=author_id)
                paper.conference_id = conference.id  # Associate paper with the conference
                session.add(paper)
                session.commit()
                print("Paper submitted successfully!")
            elif choice == 2:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")

# Reviewer actions
def reviewer_actions():
    while True:
        try:
            print("\nReviewer Actions:")
            print("1. Review Paper")
            print("2. Logout")
            choice = get_choice("Select an action (1-2): ", {1, 2})

            if choice == 1:
                papers = session.query(Paper).filter_by(status="Pending").all()
                if not papers:
                    print("No papers to review.")
                    continue

                print("\nPending Papers:")
                for i, paper in enumerate(papers):
                    print(f"{i + 1}. {paper.title} - {paper.abstract}")
                paper_choice = get_choice("Select a paper number to review: ", {i + 1 for i in range(len(papers))}) - 1

                paper = papers[paper_choice]
                
                print("\nReview Options:")
                print("1. Accept")
                print("2. Reject")
                review_choice = get_choice("Select an action (1-2): ", {1, 2})
                
                review_status = "Accepted" if review_choice == 1 else "Rejected"
                review_comment = input("Review comments: ")

                paper.status = review_status
                review = Review(paper_id=paper.id, review_status=review_status, review_comment=review_comment)
                session.add(review)
                session.commit()
                print("Paper reviewed successfully!")
            elif choice == 2:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")

# Attendee actions
def attendee_actions():
    while True:
        try:
            print("\nAttendee Actions:")
            print("1. View Conferences")
            print("2. View Published Papers")
            print("3. Logout")
            choice = get_choice("Select an action (1-3): ", {1, 2, 3})

            if choice == 1:
                conferences = session.query(Conference).all()
                print("Available Conferences:")
                for conf in conferences:
                    print(f"{conf.name} on {conf.date} at {conf.location}")
            elif choice == 2:
                published_papers = session.query(Paper).filter_by(status="Accepted").all()
                if not published_papers:
                    print("No published papers available.")
                else:
                    print("\nPublished Papers:")
                    for paper in published_papers:
                        conference = session.query(Conference).get(paper.conference_id)
                        print(f"{paper.title} in {conference.name} - {paper.abstract}")
            elif choice == 3:
                print("Logging out...")
                break
        except Exception as e:
            print(f"An error occurred: {e}")

# Main Program Loop
def main():
    print("Welcome to the Conference Management System!")
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
                    if user.role == "Organizer":
                        organizer_actions()
                    elif user.role == "Author":
                        author_actions(user.id)
                    elif user.role == "Reviewer":
                        reviewer_actions()
                    elif user.role == "Attendee":
                        attendee_actions()
            elif choice == 3:
                print("Goodbye!")
                break
        except Exception as e:
            print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
