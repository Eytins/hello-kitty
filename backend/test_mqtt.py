from flask import Flask, request, jsonify
import json
from datetime import date
import MySQLdb.cursors
from app import mqtt_client
from app import mysql


app = Flask(__name__)


# Define functions to handle each topic
def add_weight(client, userdata, message):
    # Do something with the message for topic add_weight
    req_data = message.get_json()
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


# Define functions to handle each topic
def flask_test(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    print(
        'Received message on topic: {topic} with payload: {payload}'.format(**data))


topic_handlers = {
    'add_weight': add_weight,
    '/flask/mqtt': flask_test
}


@mqtt_client.on_connect()
def handle_connect(client, userdata, flags, rc):
    if rc == 0:
        print('Connected successfully')
        for topic in topic_handlers.keys():
            mqtt_client.subscribe(topic)  # subscribe topic
    else:
        print('Bad connection. Code:', rc)


@mqtt_client.on_message()
def handle_mqtt_message(client, userdata, message):
    topic = message.topic
    if topic in topic_handlers:
        topic_handlers[topic](client, userdata, message)
    else:
        print(f'No handler found for topic {topic}')


@app.route('/publish', methods=['POST'])
def publish_message():
    request_data = request.get_json()
    publish_result = mqtt_client.publish(
        request_data['topic'], json.dumps({'msg': request_data['msg']}))
    return jsonify({'code': publish_result[0]})
