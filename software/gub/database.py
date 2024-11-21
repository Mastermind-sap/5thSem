from sqlalchemy import create_engine, Column, Integer, String, ForeignKey, Date
from sqlalchemy.orm import relationship, sessionmaker, declarative_base

Base = declarative_base()


class User(Base):
    __tablename__ = "users"
    id = Column(Integer, primary_key=True)
    username = Column(String, unique=True, nullable=False)
    password = Column(String, nullable=False)
    role = Column(String, nullable=False)


class Votes(Base):
    __tablename__ = "votes"
    id = Column(Integer, primary_key=True)
    cname = Column(String, nullable=False)
    votes = Column(Integer, nullable=False)


class Budget(Base):
    __tablename__ = "budget"
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False,unique=True)
    amount = Column(Integer, nullable=False)



class Notices(Base):
    __tablename__ = "notices"
    id = Column(Integer, primary_key=True)
    author_id = Column(Integer, ForeignKey("users.id"), nullable=False)
    notice=Column(String,nullable=False)
    date=Column(Date,nullable=True)


class Events(Base):
    __tablename__ = "events"
    id = Column(Integer, primary_key=True)
    name=Column(String,nullable=False)
    date=Column(Date,nullable=True)


engine = create_engine("sqlite:///gub.db")
Base.metadata.create_all(engine)
Session = sessionmaker(bind=engine)
session = Session()
