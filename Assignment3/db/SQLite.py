import sqlite3
from datetime import datetime, timedelta

class SQLite:
	def __init__(self, path='coffee.db'):
		self.conn = sqlite3.connect(path)
		self.curr = self.conn.cursor()

	def create_coffee_tables(self):
		self.curr.execute("""CREATE TABLE coffee_consumer(
			coffee_consumer_id INTEGER PRIMARY KEY, 
			date TIMESTAMP, 
			consumer TEXT)""")
		self.curr.execute("""CREATE TABLE coffee(
			coffee_id INTEGER PRIMARY KEY, 
			date TIMESTAMP, 
			coffee_value INTEGER)""")
		self.conn.commit()    	
	#
	# Interface for coffee_consumer
	#
	def insert_coffee_consumer(self, date, consumer):
		self.curr.execute('''INSERT INTO coffee_consumer (date, consumer)
			VALUES (?,?)''', (date, consumer))
		self.conn.commit()
	
	def fetch_coffee_consumer_by_date(self, date_start, date_end):
		t = (date_start, date_end,)
		records = self.curr.execute('''SELECT * FROM coffee_consumer
			WHERE date > ? AND date < ?''', t)
		return tuple(records)
	
	def fetch_coffee_consumer_by_id(self, consumer):
		t = (consumer,)
		records = self.curr.execute('''SELECT * FROM coffee_consumer
			WHERE consumer=?''', t)
		return tuple(records)

	#
	# Interface for coffee
	#
	def insert_coffee(self, date, value):
		self.curr.execute('''INSERT INTO coffee (date, coffee_value)
			VALUES (?,?)''', (date, value))
		self.conn.commit()
	
	def fetch_coffee_by_date(self, date_start, date_end):
		t = (date_start, date_end,)
		records = self.curr.execute('''SELECT * FROM coffee
			WHERE date > ? AND date < ?''', t)
		return tuple(records)
	

if __name__=="__main__":
	db = SQLite()
	#db.create_coffee_tables()
	start = datetime.now() - timedelta(hours=1)
	for i in range(60):
		delta = timedelta(minutes=i)
		db.insert_coffee(start + delta, 100 - i)
	
	#records = db.fetch_coffee_consumer_by_id("ME")
	#records = db.fetch_coffee_consumer_by_date("2012-11-04 21:06:15.702119", "2012-11-04 21:14:45.864913")
	records = db.fetch_coffee_by_date("2012-11-04 21:06:15.702119", datetime.now())
	for record in records:
		print record
	