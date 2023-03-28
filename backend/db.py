import MySQLdb

db = MySQLdb.connect(
    host="34.246.195.200",
    user="root",
    password="p@ssword",
    database="kitty"
)
cursor = db.cursor(MySQLdb.cursors.DictCursor)
