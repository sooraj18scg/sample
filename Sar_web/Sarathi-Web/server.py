from flask import Flask, render_template, flash, request,redirect,session
from flask_sqlalchemy import SQLAlchemy
import json
from datetime import datetime

app = Flask(__name__)



app.secret_key = "super secret key"
#app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://kapil829:chaudhary8279@kapil829.mysql.pythonanywhere-services.com/kapil829$sarathi'
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://root:kapil@localhost/Sarathi'
app.config['SQLALCHEMY_POOL_RECYCLE'] = 299
app.config['SQLALCHEMY_POOL_TIMEOUT'] = 20
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app=app)
app.app_context().push()


class Manager(db.Model):
    id = db.Column(db.Integer,  primary_key=True)
    Longitude = db.Column(db.String(50), nullable=False,)
    Latitude= db.Column(db.String(50), nullable=False)
    time = db.Column(db.String(150),nullable=False)

# @app.route("/")
# def Home():
#     return render_template()

# @app.route("/accidents")
# def Accidents():
#     data = Manager.query.all()
#     db.session.close()
#     print(data)
#     return render_template("Maintable.html",data = data)





@app.route('/api/sendcoord')
def  coordinate():
    #res = requests.get(api_url)
    latlong = request.args.get('latlong')
    if(latlong and len(latlong) > 0):
        coord = latlong.split(" ")
        if(coord and len(coord)>=2):
            manger = Manager(
                Latitude = coord[0],
                Longitude = coord[1],
                time = str(datetime.now()),
                )
            db.session.add(manger)
            db.session.commit()
            db.session.close()
            return "1"
        return "0"
    return "0"

@app.route("/getdata")
def GetData():
    res = {}
    res = Manager.query.all()
    if res:
        return json.dumps(res)
    return "{}"




#app.run(debug=True)