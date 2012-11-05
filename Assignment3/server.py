from bottle import route, run, template, static_file
from db.SQLite import SQLite
from datetime import datetime, timedelta
import json

@route('/')
def index(name='World'):
    return template('content.tpl', content=name)


#
# Views
#
@route('/last24h')
def last24():
    return template('view/content.tpl', content="hello")

@route('/last1h')
def last24():
    return template('view/content.tpl', content="hello")


#
# Data API
#
@route('/json_last24h')
def json_last24h():
    db = SQLite("db/coffee.db")
    today = datetime.now()
    yesterday = datetime.now() - timedelta(days=1)
    # fetch data and format it for javascript friendly format
    records = db.fetch_coffee_by_date(yesterday, today)
    ret = []
    for record in records:
        d = datetime.strptime(record[1].split(".")[0], "%Y-%m-%d %H:%M:%S")
        ret.append((d.year, d.month, d.day, d.hour, d.minute, d.second, record[2]))

    return json.dumps(ret)

@route('/json_last1h')
def json_last24h():
    db = SQLite("db/coffee.db")
    today = datetime.now()
    yesterday = datetime.now() - timedelta(hours=1)
    # fetch data and format it for javascript friendly format
    records = db.fetch_coffee_by_date(yesterday, today)
    ret = []
    for record in records:
        d = datetime.strptime(record[1].split(".")[0], "%Y-%m-%d %H:%M:%S")
        ret.append((d.year, d.month, d.day, d.hour, d.minute, d.second, record[2]))

    return json.dumps(ret)
#
# Statuc files
#
@route('/static/:filename#.+#', name='static')
def static(filename):
    return static_file(filename, root='static')


#
# Run!
#
run(host='localhost', port=8080)
