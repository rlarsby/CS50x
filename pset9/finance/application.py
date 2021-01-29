import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # find owned stocks and group them by type of stock
    owned_stocks = db.execute(
        "SELECT symbol AS Symbol, name AS Name, SUM(quantity) AS Shares, buy_value, total FROM stocks WHERE user_id = ? GROUP BY symbol, name", session["user_id"])

    # check if user has any stocks otherwise redirect to /buy
    if not owned_stocks:
        return render_template("empty.html")

    # find how much money the user has
    balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = usd(balance[0]["cash"])

    # create variable to calculate cash + value of shares
    tot_sum = balance[0]["cash"]

    # update the current value of stock and the total value in the query
    for stock in owned_stocks:

        current_value = lookup(stock["Symbol"])

        tot = usd(stock["Shares"]*current_value["price"])

        tot_sum = tot_sum + (stock["Shares"]*current_value["price"])

        stock.update({'buy_value': current_value, 'total': tot})

        if tot == "$0.00":
            del stock

    tot_sum = usd(tot_sum)

    return render_template("index.html", owned_stocks=owned_stocks, cash=cash, current_value=current_value, tot_sum=tot_sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        buy = request.form.get("symbol")
        quantity = request.form.get("shares")
        buy_quote = lookup(buy)

        # check for input
        if bool(buy) == False:
            return apology("Please insert stock name")

        # check for valid input
        if buy_quote == None:
            return apology("Invalid input")

        # check for valid quantity
        if int(quantity) <= 0:
            return apology("Invalid quantity")

        # check for a high enough balance
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        price = buy_quote['price']*float(quantity)

        if price > float(balance[0]["cash"]):
            return apology("Insufficient funds")

        # update current balance
        db.execute(f"UPDATE users SET cash=? - {price} WHERE id = ?", float(balance[0]["cash"]), session["user_id"])

        # insert purchase into table stocks
        db.execute("INSERT INTO stocks (user_id, name, buy_value, symbol, quantity) VALUES(?, ?, ?, ?, ?)",
                    session["user_id"], buy_quote['name'], buy_quote['price'], buy_quote['symbol'], quantity)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    stocks = db.execute("SELECT * FROM stocks WHERE user_id = ?", session["user_id"])

    return render_template("history.html", stocks=stocks)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    """Get stock quote."""

    if request.method == "POST":

        stock = request.form.get("symbol")

        # check for input
        if bool(stock) == False:
            return apology("Please insert stock name")

        quote = lookup(stock)

        # check for successful input
        if quote == None:
            return apology("Unable to find quote")

        return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        existing_username = db.execute("SELECT * FROM users WHERE username LIKE ?", username)
        hashed_password = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # check if username already exists
        if bool(existing_username) == True:
            return apology("Username already exists")

        # check if there is a username
        elif bool(username) == False:
            return apology("No username")

        # check if there is a password
        elif bool(password) == False:
            return apology("Must have a password")

        # check if passwords match
        elif password != confirmation:
            return apology("Passwords do not match")

        # insert new user into database
        new_user = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashed_password)

        # session id from the finished login route above
        session["user_id"] = new_user

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        sell = request.form.get("symbol")
        quantity = request.form.get("shares")
        sell_quote = lookup(sell)

        # check for input
        if bool(sell) == False:
            return apology("Please insert stock name")

        # check for valid input
        if sell_quote == None:
            return apology("Invalid input")

        # check if user has stock
        stock = db.execute("SELECT symbol FROM stocks WHERE user_id = ? AND symbol LIKE ? GROUP BY symbol",
                            session["user_id"], sell)
        if not stock:
            return apology("You do not own this stock")

        # check if user has enough stocks
        quantity_check = db.execute(
            "SELECT symbol, SUM(quantity) FROM stocks WHERE user_id = ? AND symbol LIKE ? GROUP BY symbol", session["user_id"], sell)
        check = quantity_check[0]["SUM(quantity)"]
        if int(check) < int(quantity):
            return apology("Not enough stocks")

        # update amount of stocks
        db.execute("INSERT INTO stocks (user_id, name, buy_value, symbol, quantity) VALUES(?, ?, ?, ?, ?)",
                    session["user_id"], sell_quote['name'], sell_quote['price'], sell_quote['symbol'], -int(quantity))

        # update users money
        price = sell_quote['price']*float(quantity)
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        db.execute(f"UPDATE users SET cash = ? + {price} WHERE id = ?", float(balance[0]["cash"]), session["user_id"])

        return redirect("/")
        # return render_template("/sell.html", quantity_check=quantity_check, check=check)

    else:
        return render_template("sell.html")


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """Add money to account"""

    if request.method == "POST":

        cash = request.form.get("cash")
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        new_balance = float(balance[0]["cash"]) + float(cash)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session["user_id"])

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
