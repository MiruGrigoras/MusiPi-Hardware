import mysql.connector
from mysql.connector import errorcode

#cod luat de pe github.com/sanpingz/mysql-connector

config = {
    'user':  'root',
    'password': 'parola',
    'host': '127.0.0.1',
    'database': 'musipi'
}

cnx = cur = None
try:
    cnx = mysql.connector.connect(**config)
except mysql.connector.Error as err:
    if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
        print('The access has been denied')
    elif err.errno == errorcode.ER_BAD_DB_ERROR:
        print('The required database does not exist')
    else:
        print('Error: ', err)
else:
    cur = cnx.cursor()
    cur.execute('SELECT * FROM doors;')
    for row in cur.fetchall():
        print(row[3])
finally:
    if cur:
        cur.close()
    if cnx:
        cnx.close()
    
 