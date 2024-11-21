from sqlalchemy import create_engine, Column, Integer, String, ForeignKey, Date
from sqlalchemy.orm import relationship, sessionmaker, declarative_base

Base = declarative_base()

# User model
class User(Base):
    __tablename__ = "users"
    id = Column(Integer, primary_key=True)
    username = Column(String, unique=True, nullable=False)
    password = Column(String, nullable=False)
    role = Column(String, nullable=False)

# Conference model
class Conference(Base):
    __tablename__ = "conferences"
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    date = Column(Date, nullable=False)
    location = Column(String, nullable=False)
    papers = relationship("Paper", back_populates="conference")

# Paper model with conference_id as a foreign key
class Paper(Base):
    __tablename__ = "papers"
    id = Column(Integer, primary_key=True)
    title = Column(String, nullable=False)
    abstract = Column(String, nullable=False)
    status = Column(String, nullable=False)
    author_id = Column(Integer, ForeignKey("users.id"), nullable=False)
    conference_id = Column(Integer, ForeignKey("conferences.id"))  # Link to Conference
    conference = relationship("Conference", back_populates="papers")

# Review model
class Review(Base):
    __tablename__ = "reviews"
    id = Column(Integer, primary_key=True)
    paper_id = Column(Integer, ForeignKey("papers.id"), nullable=False)
    review_status = Column(String, nullable=False)
    review_comment = Column(String, nullable=True)

# Database setup
engine = create_engine("sqlite:///conference_management.db")
Base.metadata.create_all(engine)
Session = sessionmaker(bind=engine)
session = Session()
