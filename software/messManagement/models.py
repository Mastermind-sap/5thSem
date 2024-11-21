from sqlalchemy import Column, Integer, String, ForeignKey, Boolean, Date, Text
from sqlalchemy.orm import relationship
from database import Base

class Student(Base):
    __tablename__ = 'students'
    id = Column(Integer, primary_key=True)
    name = Column(String, nullable=False)
    registered = Column(Boolean, default=False)

class Menu(Base):
    __tablename__ = 'menu'
    id = Column(Integer, primary_key=True)
    details = Column(Text, nullable=False)

class Attendance(Base):
    __tablename__ = 'attendance'
    id = Column(Integer, primary_key=True)
    student_id = Column(Integer, ForeignKey('students.id'))
    date = Column(Date, nullable=False)
    attended = Column(Boolean, default=False)

class Bill(Base):
    __tablename__ = 'bills'
    id = Column(Integer, primary_key=True)
    student_id = Column(Integer, ForeignKey('students.id'))
    amount = Column(Integer, nullable=False)

class Feedback(Base):
    __tablename__ = 'feedback'
    id = Column(Integer, primary_key=True)
    student_id = Column(Integer, ForeignKey('students.id'))
    content = Column(Text, nullable=False)
