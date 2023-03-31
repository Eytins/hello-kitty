import json
from datetime import date
from db import db, cursor


# Define functions to handle each topic
def add_weight(client, userdata, message):
    # Do something with the message for topic add_weight
    req_data = json.loads(message.payload.decode())
    print(req_data)
    id = req_data['id']
    weight = req_data['weight']
    temperature = req_data['temperature']
    humidity = req_data['humidity']
    today = date.today()
    if id is not None and id > 0 and weight is not None and weight > 0:
        cursor.execute(
            'REPLACE INTO weight VALUES (NULL, %s, %s, %s)',
            (id, weight, today,)
        )
        db.commit()
        msg = 'The weight of the cat is added into database!'
        feedingDuration = algorithm(weight, temperature, humidity)
        client.publish("esp32/aws2esp", feedingDuration)
    else:
        msg = 'Invalid request data! Please provide valid id, weight and date.'
    print(msg)


def algorithm(weight, temperature, humidity):
    return 3
