import mysql.connector


# Print Data based off a Cursor's primary command
def printData(cursor):
    for i in cursor:
        print(i)


# Accessing all records in the DB
def printDatabaseRecords(cursor):
    cursor.execute("SELECT * FROM artist")
    print("Artist Data: ")
    printData(cursor)
    print("\n")

    cursor.execute("SELECT * FROM art_work")
    print("Art Work Data: ")
    printData(cursor)
    print("\n")

    myCursor.execute("SELECT * FROM art_show")
    print("Art Show Data: ")
    printData(cursor)
    print("\n")

    myCursor.execute("SELECT * FROM customer")
    print("Customer Data: ")
    printData(cursor)
    print("\n")


# Accessing specific record based on attribute values
def specificRecordAccess(cursor, attributeValues):
    cursor.execute(attributeValues)
    print("Attribute Value Records command:", attributeValues, "\n Result: ")
    printData(cursor)
    print("\n")


# Sort and produce a report based off given requirements
def sortReport(cursor):
    cursor.execute("SELECT * FROM artist ORDER BY art_style")
    print("Artist Data: ")
    printData(cursor)
    print("\n")

    cursor.execute("SELECT * FROM art_work ORDER BY art_type")
    print("Art Work Data: ")
    printData(cursor)
    print("\n")

    cursor.execute("SELECT * FROM customer ORDER BY preferences")
    print("Customer Data: ")
    printData(cursor)
    print("\n")


# fetch Artist Matches based off a customer's art preference
def fetchArtMatch(cursor, artStyle):
    cursor.execute("SELECT artist FROM art_show")
    allArtists = [i for i in cursor]
    resultArtist = []
    
    for currentArtist in allArtists:
        currentCommand = "SELECT art_style FROM artist WHERE name='" + ''.join(currentArtist) + "'"
        cursor.execute(currentCommand)
        for art_type in cursor:
            if ''.join(art_type) == artStyle:
                resultArtist.append(currentArtist)
    
    return resultArtist


# Produce a report based off a customer's art preferences and the art at a specific art show
def customerArtPreference(cursor):
    cursor.execute("SELECT * FROM customer WHERE preferences='drawing'")
    print("Customer with a drawing art preference: ")
    printData(cursor)
    artistMatches = fetchArtMatch(cursor, "drawing")
    print("Based on your art preference, you should attend this art show: ")
    for artist in artistMatches:
        cursor.execute("SELECT * FROM art_show WHERE artist='" + ''.join(artist) +"'")
        printData(cursor)
    print("\n")

    cursor.execute("SELECT * FROM customer WHERE preferences='painting'")
    print("Customer with a painting art preference: ")
    printData(cursor)
    artistMatches = fetchArtMatch(cursor, "painting")
    print("Based on your art preference, you should attend this art show: ")
    for artist in artistMatches:
        cursor.execute("SELECT * FROM art_show WHERE artist='" + ''.join(artist) +"'")
        printData(cursor)
    print("\n")

    cursor.execute("SELECT * FROM customer WHERE preferences='sculpture'")
    print("Customer with a sculpture preference: ")
    printData(cursor)
    artistMatches = fetchArtMatch(cursor, "painting")
    print("Based on your art preference, you should attend this art show: ")
    for artist in artistMatches:
        cursor.execute("SELECT * FROM art_show WHERE artist='" + ''.join(artist) +"'")
        printData(cursor)
    print("\n")



# Setup SQL Connector
art_db = mysql.connector.connect(
    host='localhost', user='root', passwd='12345', database='ART_PROJ_PART_2')

myCursor = art_db.cursor()

# Printing records from the DB
print("All records in the DB: \n")
printDatabaseRecords(myCursor)

# Accessing specific records based on attribute values
specificAtt = ["SELECT * FROM artist WHERE art_style='sculpture'", "SELECT * FROM art_work WHERE art_type='painting' AND price='20,000'"]
for attribute in specificAtt:
    specificRecordAccess(myCursor, attribute)



# 1st Report: Sorted according to art style
print("Report sorted alphabetically based off the Art Style \n")
sortReport(myCursor)


# 2nd Report: showing customers whose art preference is the same as shown in any given art show.
print("Report of customers art preference matching to an art show \n")
customerArtPreference(myCursor)



