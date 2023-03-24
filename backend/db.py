import MySQLdb

db = MySQLdb.connect(
    host="ec2-34-246-195-200.eu-west-1.compute.amazonaws.com",
    user="root",
    password="p@ssword",
    database="kitty"
)
cursor = db.cursor(MySQLdb.cursors.DictCursor)
