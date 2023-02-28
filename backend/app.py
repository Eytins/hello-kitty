# Store this code in 'app.py' file
from flask import Flask, jsonify, request
from flask_mysqldb import MySQL
import MySQLdb.cursors
import re
from flask_swagger_ui import get_swaggerui_blueprint
from datetime import date


app = Flask(__name__)


SWAGGER_URL = '/swagger'  # URL for exposing Swagger UI (without trailing '/')
API_URL = '/static/swagger.json'  # API url

# Call factory function to create our blueprint
swaggerui_blueprint = get_swaggerui_blueprint(
    # Swagger UI static files will be mapped to '{SWAGGER_URL}/dist/'
    SWAGGER_URL,
    API_URL,
    config={  # Swagger UI config overrides
        'app_name': "Hello-Kitty"
    },
    # oauth_config={  # OAuth config. See https://github.com/swagger-api/swagger-ui#oauth2-configuration .
    #    'clientId': "your-client-id",
    #    'clientSecret': "your-client-secret-if-required",
    #    'realm': "your-realms",
    #    'appName': "your-app-name",
    #    'scopeSeparator': " ",
    #    'additionalQueryStringParams': {'test': "hello"}
    # }
)

app.register_blueprint(swaggerui_blueprint)


app.secret_key = 'your secret key'


app.config['MYSQL_HOST'] = 'ec2-34-246-195-200.eu-west-1.compute.amazonaws.com'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = 'p@ssword'
app.config['MYSQL_DB'] = 'kitty'


mysql = MySQL(app)


@app.route('/getDetails', methods=['GET'])
def get_details():
    id = request.args.get("id")
    if len(id) != 0:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM pets WHERE id = % s', (int(id), ))
        results = cursor.fetchone()
        if results:
            return jsonify(results)
        else:
            return "The cat doesn't exist in database."
    else:
        return "Pet's id could not be null."


@app.route('/addACat', methods=['POST'])
def add_a_cat():
    msg = ''
    if request.method == 'POST' and 'name' in request.form and 'age' in request.form and 'sex' in request.form:
        name = request.form['name']
        age = request.form['age']
        sex = request.form['sex']
        if (len(name) > 0) & (len(age) > 0) & (len(sex) > 0):
            if not re.match(r'[A-Za-z0-9]+', name):
                msg = 'name must contain only characters and numbers !'
            else:
                cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
                cursor.execute(
                    'INSERT INTO pets VALUES (NULL, % s, % s, % s)', (name, age, sex,))
                mysql.connection.commit()
                msg = 'The details of the cat is added into database ！'
        else:
            msg = 'Name, Age, Sex could not be null !'
    elif request.method == 'POST':
        msg = 'Please fill out the form !'
    return msg


@app.route('/updateDetails/<int:id>', methods=['PUT'])
def update_details(id):
    msg = ''
    if request.method == 'PUT':
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM pets WHERE id = % s', (id, ))
        results = cursor.fetchone()
        if results:
            name = request.form['name']
            age = request.form['age']
            sex = request.form['sex']
            try:
                cursor.execute(
                    'UPDATE pets SET NAME =% s, age =% s, sex =% s WHERE id =% s', (name, age, sex, (id, ), ))
                mysql.connection.commit()
                msg = 'The details of the cat is updated ！'
            except:
                return 'There was a problem updating...'
        else:
            msg = "The cat doesn't exist in database."
    return msg


@app.route('/deleteACat/<int:id>', methods=['DELETE'])
def delete_a_cat(id):
    msg = ''
    if request.method == 'DELETE':
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM pets WHERE id = % s', (id, ))
        results = cursor.fetchone()
        if results:
            try:
                cursor.execute('DELETE FROM pets WHERE id =% s', (id, ))
                mysql.connection.commit()
                msg = 'The details of the cat is deleted ！'
            except:
                return 'There was a problem deleting...'
        else:
            msg = "The cat doesn't exist in database."
    return msg


# @app.route('/addWeight', methods=['POST'])
# def add_weight():
#     msg = ''
#     if request.method == 'POST' and 'id' in request.form and 'weight' in request.form and 'date' in request.form:
#         id = request.form['id']
#         weight = request.form['weight']
#         date = request.form['date']
#         if (len(id) > 0) & (len(weight) > 0) & (len(date) > 0):
#             cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
#             cursor.execute(
#                 'REPLACE INTO weight VALUES (NULL, % s, % s, % s)', (id, weight, date,))
#             mysql.connection.commit()
#             msg = 'The details of the cat is added into database ！'
#         else:
#             msg = 'Id, weight, date could not be null !'
#     elif request.method == 'POST':
#         msg = 'Please fill out the form !'
#     return jsonify(msg)


@app.route('/addWeight', methods=['POST'])
def add_weight():
    msg = ''
    if request.method == 'POST':
        req_data = request.get_json()
        if req_data and "id" in req_data and "weight" in req_data:
            id = req_data["id"]
            weight = req_data["weight"]
            today = date.today()
            if id is not None and id > 0 and weight is not None and weight > 0:
                cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
                cursor.execute(
                    'REPLACE INTO weight VALUES (NULL, %s, %s, %s)',
                    (id, weight, today,)
                )
                mysql.connection.commit()
                msg = 'The weight of the cat is added into database!'
            else:
                msg = 'Invalid request data! Please provide valid id, weight and date.'
            feedingDuration = 0
        else:
            msg = 'Invalid request data! Please provide id, weight and date.'
            feedingDuration = 0
    return jsonify({
        'feedingDuration': feedingDuration,
        'message': msg
    })


@app.route('/getWeight', methods=['GET'])
def get_weight():
    id = request.args.get("id")
    if len(id) != 0:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute(
            'SELECT * FROM weight WHERE id = % s ORDER BY weight_date', (int(id), ))
        results = cursor.fetchall()
        if results:
            # format date as "yyyy-MM-dd"
            for row in results:
                row['weight_date'] = row['weight_date'].strftime('%Y-%m-%d')
            # return results as JSON response
            return jsonify({
                "error": False,
                "message": "Success",
                "data": results
            })
        else:
            return jsonify({
                "error": True,
                "message": "No data for the cat.",
                "data": []
            })
    else:
        return jsonify({
            "error": True,
            "message": "Pet's id could not be null.",
            "data": []
        })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=int("8000"))
