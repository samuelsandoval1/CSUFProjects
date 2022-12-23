from flask import Flask, render_template, request
from datetime import datetime
import json

app = Flask(__name__)

customerDateRequest = 0

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/search', methods=['POST','GET'])
def search():
    if request.method != 'POST':
        return

    # Read JSON from file
    cars_dict = {}
    with open('src/cars.json', 'r') as f:
        cars_dict = json.load(f)
    dateOne = request.form['dateRange1']
    dateTwo = request.form['dateRange2']
    if len(dateOne) > 1 and len(dateTwo) > 1:
        d1 = datetime.strptime(dateOne, "%Y-%m-%d")
        d2 = datetime.strptime(dateTwo, "%Y-%m-%d")

        delta = d2 - d1
        print(f'Difference is {delta.days} days')

        # seat recognition
        # results = [item for item in cars_dict if carInput in item["Seats"]]
        results = [item for item in cars_dict if item["IsRented"] is False]

        return render_template('results.html', cars=results, datesAmount=delta)
    else:
        return render_template('errorPage.html')


@app.route('/bookingRequest', methods=['POST','GET'])
def bookingRequest():
    print(customerDateRequest)
    # calculate total
    print("in bookingRequest")
    dates = request.form['datesAmount']
    carSelected = request.form['specificCar']
    rentPrice = request.form['rentPrice']
    return render_template('bookingRequest.html', car=carSelected, price=rentPrice, currTot=customerDateRequest,datesAmount=dates)

@app.route('/finalTotal', methods=['POST','GET'])
def finalTotal():
    # includes rental add-on
    # tax is 25%
    customerDateRequest = request.form['datesAmount']
    rentRate = request.form['price']
    rentRate = rentRate.replace('$','')
    cur = int(rentRate)* int(customerDateRequest)
    taxPercent = float(cur) * 0.25
    total = cur + taxPercent
    if 'insuranceAddOn' in request.form:
        total = total + (int(customerDateRequest) * 10)
    return render_template('total.html', priceTotal=total, rateWithDays=cur, taxes=taxPercent) # cur, taxPercent