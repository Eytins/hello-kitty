from flask import request, jsonify
import json
from datetime import date
from db import db, cursor


# Define functions to handle each topic
def add_weight(client, userdata, message):
    # Do something with the message for topic add_weight
    req_data = json.loads(message.payload.decode())
    id = req_data['msg']['id']
    weight = req_data['msg']["weight"]
    today = date.today()
    print(id, weight, today)
    if id is not None and id > 0 and weight is not None and weight > 0:
        # cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        # cursor.execute("SET max_allowed_pa​​cket=1073741824")
        cursor.execute(
            'REPLACE INTO weight VALUES (NULL, %s, %s, %s)',
            (id, weight, today,)
        )
        # mysql.connection.commit()
        db.commit()
        msg = 'The weight of the cat is added into database!'
    else:
        msg = 'Invalid request data! Please provide valid id, weight and date.'
    feedingDuration = 0
    # return jsonify({
    #     'feedingDuration': feedingDuration,
    #     'message': msg
    # })
