import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
        actions = db.execute("SELECT symbolAction,nameAction,sum(numberAction) \
                    FROM portfolio WHERE userID = :userID GROUP BY symbolAction;",
                    userID = session["user_id"] )

        result = []
        for action in actions:
            if (action["sum(numberAction)"] > 0):
                actioninfo = lookup(action["symbolAction"])
                totalPrice = float(actioninfo["price"]) * float(action["sum(numberAction)"])
                totalPrice = round(totalPrice, 2)
                action["priceAction"] = actioninfo["price"]
                action["total"] = totalPrice
                result.append(action)

        actions = result

        userCash = db.execute("SELECT cash FROM users WHERE id = :userID"
                    , userID = session["user_id"] )
        userCash = float(userCash[0]["cash"])

        userInfo = {}
        userInfo["symbolAction"] = "CASH"
        userInfo["nameAction"] = ""
        userInfo["priceAction"] = ""
        userInfo["sum(numberAction)"] = ""
        userInfo["total"] = userCash

        actions.append(userInfo)
        return render_template("index.html", actions = actions)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Insert symbol")

        if not request.form.get("shares"):
            return apology("Insert shares")

        action = lookup(request.form.get("symbol"))

        if not action:
            return apology("action doesn't exist")

        cash = db.execute("SELECT cash FROM users WHERE id = :userID", userID = session["user_id"])
        cash = float(cash[0]["cash"])
        if (cash > action["price"] * int(request.form.get("shares")) ):
            db.execute("INSERT INTO portfolio (userID, nameAction, symbolAction, numberAction, priceAction) \
                            VALUES (:user, :nameAct, :symbAct, :numberAct, :priceAct)",
                        user = session["user_id"],
                        nameAct =action["name"],
                        symbAct = action["symbol"],
                        priceAct = action["price"],
                        numberAct=request.form.get("shares") )
            cash = cash - ( action["price"] * float(request.form.get("shares")))
            db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash = cash, user = session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    return jsonify("TODO")


@app.route("/history")
@login_required
def history():
    actions = db.execute("SELECT symbolAction, priceAction, numberAction, buyHour  \
                   FROM portfolio WHERE userID = :userID ",
                   userID = session["user_id"])
    return render_template("history.html",actions = actions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide input", 403)

        quote = lookup(request.form.get("symbol"))

        if not quote:
            return apology("quote doesn't exist")

        return render_template("quoteInfo.html", name = quote["name"],symbol = quote["symbol"], price = quote["price"])
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password confirmation was submitted
        elif not request.form.get("passwordConf"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Verify the password matches
        if (request.form.get("password") != request.form.get("passwordConf")):
            return apology("Password doesn't match")

        # User already taken
        if (len(rows) != 0):
            return apology("username taken", 403)

        hashPassword = generate_password_hash(request.form.get("password"))

        userid = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashPassword)",
                    username = request.form.get("username"),hashPassword = hashPassword)

        session["user_id"] = userid
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Insert symbol")

        if not request.form.get("number"):
            return apology("Insert number")

        DBinfo = db.execute("SELECT SUM(numberAction) FROM portfolio WHERE userID = :userID \
                            AND symbolAction = :nameAct",
                            userID = session["user_id"],
                            nameAct = request.form.get("symbol"))

        totalActions = int(DBinfo[0]["SUM(numberAction)"])

        print(totalActions)
        print(request.form.get("number"))

        if totalActions <  int(request.form.get("number")):
            return apology("You can't afford it")

        action = lookup(request.form.get("symbol"))
        db.execute("INSERT INTO portfolio (userID, nameAction, symbolAction, numberAction, priceAction) \
                    VALUES (:user, :nameAct, :symbAct, :numberAct, :priceAct)",
                    user = session["user_id"],
                    nameAct =action["name"],
                    symbAct = action["symbol"],
                    priceAct =  action["price"],
                    numberAct= (-1) * int(request.form.get("number")) )

        cash = db.execute("SELECT cash FROM users WHERE id = :userID", userID = session["user_id"])
        cash = float(cash[0]["cash"])
        cash = cash  + ( action["price"] * float(request.form.get("number")))
        db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash = cash, user = session["user_id"])

        return redirect ("/")
    else:
        actions = db.execute("SELECT symbolAction, sum(numberAction) \
                    FROM portfolio WHERE userID = :userID GROUP BY symbolAction;",
                    userID = session["user_id"] )
        result = []
        for action in actions:
            if (action["sum(numberAction)"] > 0):
                result.append(action)

        return render_template("sell.html",actions = result)

@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    if request.method == "POST":
        cash = db.execute("SELECT cash FROM users WHERE id = :userID", userID = session["user_id"])
        cash = float(cash[0]["cash"])
        cash = cash  + float(request.form.get("cash"))
        db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash = cash, user = session["user_id"])
        return redirect("/")
    else:
        return render_template("add.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


