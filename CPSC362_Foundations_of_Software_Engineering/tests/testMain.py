import unittest
import json


def generalCarSearch(searchP, carLabel):
    cars = {}
    with open('../src/cars.json', 'r') as f:
        cars = json.load(f)
    results = []
    for item in cars:
        if type(searchP) is bool:
            if searchP is not item[carLabel]:
                continue
            results.append(item["Name"])
        elif type(searchP) is int and searchP is item[carLabel] or type(searchP) is not int and searchP in item[carLabel]: # seat case check
            results.append(item["Name"])
    if not results:
        results.append("No vehicles match your result")
    return results

class FlaskTest(unittest.TestCase):
    def testMidsizeCarType(self):
        res = generalCarSearch("Midsize", "CarType")
        self.assertEqual(len(res), 4)
    
    def testSedanCarType(self):
        res = generalCarSearch("Sedan", "CarType")
        self.assertEqual(len(res), 13)
    
    def testRVCarType(self):
        res = generalCarSearch("RV", "CarType")
        self.assertEqual(res, ["No vehicles match your result"])
    
    def testFourSeats(self):
        res = generalCarSearch(4, "Seats")
        self.assertEqual(len(res), 2)
    
    def testCarAvailable(self):
        res = generalCarSearch(False, "IsRented")
        self.assertEqual(len(res), 9)

    def testCarUnavailable(self):
        res = generalCarSearch(True, "IsRented")
        self.assertEqual(len(res), 10)
    

if __name__ == '__main__':
    unittest.main()
