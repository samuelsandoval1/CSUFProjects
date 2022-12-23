# Austin Sohn, Justin Sohn, Samuel Sandoval
# main.py

import sys
from database.database import DatabaseClass
from datetime import datetime
from PyQt6 import QtWidgets
from PyQt6.QtWidgets import QDialog, QMessageBox
from PyQt6.uic import loadUi
import tkinter as tk
from tkinter import *
from tkinter import filedialog
from tkinter.messagebox import showinfo

class GUI(QDialog):
  def __init__(self):
    self.table = "tasks"
    self.subTable = "subtasks"
    super(GUI,self).__init__()
    loadUi("./UI/window.ui",self)
    self.addGoalButton.clicked.connect(self.addGoalFunction)
    self.removeGoalButton.clicked.connect(self.removeGoalFunction)
    self.addSubGoalButton.clicked.connect(self.addSubTask)
    self.removeSubGoalButton.clicked.connect(self.removeSubTask)
    self.showCompletedGoalButton.clicked.connect(self.showCompletedGoals)
    self.completeGoal.clicked.connect(self.completeTheTask)
    self.showSubGoalButton.clicked.connect(self.showSubGoals)
    self.importButton.clicked.connect(self.importGoal)
    self.exportButton.clicked.connect(self.exportGoal)
    self.showPointsButton.clicked.connect(self.getPoints)
    self.database = DatabaseClass()
    self.reminders()
    # outputs contents of db on startup
    self.database.outputDB(self.table)
    print("----------------")
    self.database.outputDB("subtasks")

  def addsPresetGoals(self):
    l = self.database.listDB(self.table)
    l2 = self.getCompletedTasks()
    taskList = []
    for e in l:
      taskList.append(e["task"])
    l3 = [x for x in taskList if x not in l2]
    for x in l3:
      self.goalList_Widget.addItem(x)
      
  def addGoalFunction(self):
    #Typing in the Textbox and clicking add button to add to the List
    task = self.goalInputBox.toPlainText()
    if len(task) < 1:
      self.messageboxCreate("Add Goal Error", "Make sure to type your goal in before selecting add goal") 
      return
    self.goalInputBox.setPlainText(task)
    self.goalList_Widget.addItem(task)
    self.goalInputBox.clear()
    start_date = self.startDateInputBox.toPlainText()
    end_date = self.endDateInputBox.toPlainText()
    self.startDateInputBox.clear()
    self.endDateInputBox.clear()
    task_id = int(self.database.highestTaskID(self.table)) + 1
    status = 0
    params = {"task_id":task_id, "task":task, "start_date":start_date, "end_date":end_date, "status":status}
    self.database.addTask(self.table, params)
    self.database.outputDB(self.table)

  def removeGoalFunction(self):
    #clicking on the Goal and clicking the remove button
    try:
      clicked = self.goalList_Widget.currentRow()
      task = self.goalList_Widget.selectedItems()
      task = task[0].text()
      self.goalList_Widget.takeItem(clicked)
      task_id = self.database.findTaskID(self.table,task)
      if(task_id):
        self.database.removeTask(self.table,task_id)
        self.database.outputDB(self.table)
    except IndexError:
      print("error")
      self.messageboxCreate("Removing Error", "Error: Cannot remove a blank goal, please select an existing goal to remove.")

  def addSubTask(self):
    task = self.goalList_Widget.selectedItems()
    task = task[0].text()
    id = self.database.findTaskID(self.table,task)
    subtask = self.goalInputBox.toPlainText()
    self.goalInputBox.setPlainText(subtask)
    self.subgoalList_Widget.addItem(subtask)
    self.goalInputBox.clear()
    subtask_id = int(self.database.highestTaskID(self.subTable)) + 1
    params = {"subtask_id":subtask_id, "id":id, "subtask": subtask}
    self.database.addTask(self.subTable, params)    
    self.database.outputDB(self.subTable)

  def removeSubTask(self):
    try:
      clicked = self.subgoalList_Widget.currentRow()
      subtask = self.subgoalList_Widget.selectedItems()
      subtask = subtask[0].text()
      self.subgoalList_Widget.takeItem(clicked)
      subtask_id = self.database.findTaskID(self.table,subtask)
      #need to have task id for subtask?
      if(subtask_id):
        self.database.removeTask(self.table,subtask_id)
        self.database.outputDB(self.table)
    except IndexError:
      print("error")
      self.messageboxCreate("Removing Error", "Error: Cannot remove a blank subgoal, please select an existing subgoal to remove.")
  
  def showSubGoals(self):
    self.subgoalList_Widget.clear()
    task = self.goalList_Widget.selectedItems()
    task = task[0].text()
    id = self.database.findTaskID(self.table,task)
    subtask = self.database.traverseSubTaskDB(id)
    # print(subtask)
    for e in subtask:
      self.subgoalList_Widget.addItem(e)
  
  def messageboxCreate(self, winTitle, genText):
    msg = QtWidgets.QMessageBox()
    msg.setIcon(QtWidgets.QMessageBox.Icon.Information)
    msg.setText(winTitle)
    msg.setInformativeText(genText)
    msg.setWindowTitle(winTitle)
    msg.setStandardButtons(QtWidgets.QMessageBox.StandardButton.Ok)
    returnValue = msg.exec()

  # Imports text file from inputted filepath and creates goals
  def importGoal(self):
    filepath = self.goalInputBox.toPlainText()
    self.goalInputBox.setPlainText(filepath)

    if len(filepath) < 1:
      self.messageboxCreate("Import Error", "Type the filepath into the box above and then select import again.")
    else:
      self.goalInputBox.clear()
      goalsFile = open(filepath , 'r')
      status = 0
      start_date = "10/30/22" # change to input later
      end_date = "11/30/22" # change to input later
      for task in goalsFile:
        task_id = int(self.database.highestTaskID(self.table)) + 1
        params = {"task_id":task_id, "task":task, "start_date":start_date, "end_date":end_date, "status":status}
        self.database.addTask(self.table,params)
      self.goalList_Widget.clear()
      self.addsPresetGoals()
      self.messageboxCreate("Import Completed", "Goals file has been imported.")

  # Exports goal into a text file 
  def exportGoal(self):
    try:
      task = self.goalList_Widget.selectedItems()
      task = task[0].text()
      with open('outputGoals.txt', 'w') as f:
          f.write(task)
          print("Exported Task")
      self.messageboxCreate("Export Completed", "Task has been exported.")
    
    except IndexError:
      self.messageboxCreate("Export Error", "Error: Select a goal before clicking export")      
      print("Error: Select a goal before clicking export") 

  # Reminders users about their goals
  def reminders(self):
    l = self.database.listDB(self.table)
    for task in l:
      d1 = datetime.strptime(task['end_date'], "%m/%d/%y").date()
      d2 = datetime.now().date()

      delta =  d2 - d1 
      if int(delta.days) >= -2 and task['task'] != None:
        # reminder
        msg = "Don't forget to compete your "
        msg += task['task']
        msg += ', the deadline is coming up!'
        self.messageboxCreate("Goal Reminder", msg)   
  
  def getCompletedTasks(self):
    completedList = self.database.findCompletedTask()
    #print(completedList)
    return completedList

  def completeTheTask(self):
    clicked = self.goalList_Widget.currentRow()
    task = self.goalList_Widget.selectedItems()
    task = task[0].text()
    id = self.database.findTaskID(self.table,task)
    self.database.completeTask(id)
    self.goalList_Widget.takeItem(clicked)

  def showCompletedGoals(self):
    completedTasks = self.getCompletedTasks()
    msgBox = QMessageBox()
    msgBox.setWindowTitle("Completed Tasks")
    task = ""
    print(completedTasks)
    if completedTasks:
      for e in completedTasks:
        task += (e + "\n")
      msgBox.setText(task) 
    else:
      msgBox.setText("Completed task list is empty")
    msgBox.exec()

  def getPoints(self):
    points = 0
    # creates the file if it doesn't exist
    with open('./resources/points.txt', 'w+') as f:
      # adds points
      l = self.getCompletedTasks()
      for line in l:
        points += 1
      f.write(str(points))
    msgBox = QMessageBox()
    msgBox.setWindowTitle("Completed Tasks")
    msgBox.setText(str(points))
    msgBox.exec()

def main():
  app = QtWidgets.QApplication([])
  widget = GUI()
  widget.setWindowTitle("Goal List")
  widget.addsPresetGoals()
  widget.show()

  sys.exit(app.exec())
if __name__ == "__main__":
  main()
