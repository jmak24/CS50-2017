from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Homepage"""
    
    # if user reached route via POST (as by submitting a cash deposit form)
    if request.method == "POST":
        
        # ensure cash amount was submitted
        if not request.form.get("cash_deposit"):
            return apology("input cash amount to deposit")
        
        # store cash amount variable
        cashDeposit = float(request.form.get("cash_deposit"))
        
        # check that cash amount is valid
        if not cashDeposit > 0:
            return apology("cash deposit is not valid")
        
        # update the user's cash field with new amount 
        db.execute("UPDATE users SET cash = cash + :cashDeposit WHERE id = :user_id", cashDeposit=cashDeposit, user_id=session["user_id"])
        
    # retrieve all stocks user holds in positions table
    positions = db.execute("SELECT symbol, shares FROM positions WHERE acct_id = :user_id", user_id=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    
    # if user has no positions, return empty positions table 
    if not positions:
        return render_template("index.html", positions=positions, cash=cash[0]["cash"], balance=cash[0]["cash"])
    
    # for every position in portfolio, perform a lookup and update thier prices & total Market Value 
    i = 0
    while i < len(positions): 
        stock = lookup(positions[i]["symbol"])
        db.execute("UPDATE positions SET price = :price, total = :totalMktVal WHERE acct_id = :acct_id AND symbol = :symbol",
        price=stock["price"], totalMktVal=positions[i]["shares"]*stock["price"], acct_id=session["user_id"], symbol=stock["symbol"])
        i += 1
        
    # retrieve all positions again after update
    positions = db.execute("SELECT p.*, s.name FROM positions p INNER JOIN security s ON p.symbol = s.symbol WHERE acct_id = :user_id", user_id=session["user_id"])
    # calculate the user's account balance (total value of shares + cash)
    totalShareValue = db.execute("SELECT SUM(total) AS totalValue FROM positions p GROUP BY p.acct_id HAVING p.acct_id = :user_id", user_id=session["user_id"])
    balance = cash[0]["cash"] + totalShareValue[0]["totalValue"]
    
    return render_template("index.html", positions=positions, cash=cash[0]["cash"], balance=balance)

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # retrieve all transactiosn from history table
    history = db.execute("SELECT * FROM history WHERE acct_id = :user_id", user_id=session["user_id"])
    
    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a login form)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a quote form)
    if request.method == "POST":
        
        # ensure stock symbol has been inputted
        if not request.form.get("symbol"):
            return apology("stock symbol is required")
        
        # lookup the quote inputted on the form by querying Yahoo 
        # lookup function returns a dict with name, symbol, price
        stock = lookup(request.form.get("symbol"))
        
        # if look up did not find any results, return apology 
        if not stock:
            return apology("the stock symbol was not found or is invalid")
        
        # display the stock details
        return render_template("quoted.html", name=stock["name"], symbol=stock["symbol"], price=stock["price"])
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # if user reached route via POST (as by submitting a registration form)
    if request.method == "POST":
    
        # ensure username was submitted 
        if not request.form.get("username"):
            return apology("Missing Username")
        
        # ensure password was submitted 
        elif not request.form.get("password"):
            return apology("Missing Password")
            
        # ensure password confirmation was submitted 
        elif not request.form.get("confirm_password"):
            return apology("Missing Password Confirmation")
            
        # get username and password from form and store into variables
        username = request.form.get("username")
        password = request.form.get("password")
        passwordConfirm = request.form.get("confirm_password")
        
        # if password and confirmation password strings do not match, return apology 
        if password != passwordConfirm:
            return apology("Passwords do not match")
        
        # apply hash to the password 
        passHash = pwd_context.hash(password)
        
        # check that username does not already exist (is unique)
        result = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        # if user already exists, return apology
        if len(result) > 0: 
            return apology("User is already registered")
        
        # insert user into the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=passHash)
        
        # store their id in session
        session["user_id"] = result[0]["id"]
        
        # log user in automatically
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
    
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST (as by submitting a purchase order)
    if request.method == "POST":
            
        # ensure stock symbol was inputted
        if not request.form.get("symbol"):
            return apology("stock symbol is required")
            
        # ensure amount of shares is inputted
        if not request.form.get("num_shares"):
            return apology("number of shares is required ")
        
        # store the stock and number of shares 
        stock = lookup(request.form.get("symbol"))
        numShares = request.form.get("num_shares")
        
        # if look up did not find any symbol, return apology 
        if not stock:
            return apology("the stock symbol was not found or is invalid")
        
        # retrieve the amount of cash the user has
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        
        # calculate total value of the order
        totalOrderVal = stock["price"] * float(numShares)
        
        # check if user can afford to purchase the order placed
        if cash[0]["cash"] > totalOrderVal:
            # add the stock to the security table if it does not already exist
            secRow = db.execute("SELECT * FROM security WHERE symbol = :symbol", symbol=stock["symbol"])
            if len(secRow) == 0:
                db.execute("INSERT INTO security (symbol, name) VALUES (:symbol, :name)", symbol=stock["symbol"], name=stock["name"])
            
            # if security does not yet exist in the portfolio, create a new position
            posRow = db.execute("SELECT * FROM positions WHERE symbol = :symbol", symbol=stock["symbol"])
            if len(posRow) == 0:
                # insert new position
                db.execute("INSERT INTO positions (acct_id, symbol, shares, price, total) VALUES (:acct_id, :symbol, :shares, :price, :total)", 
                acct_id=session["user_id"], symbol=stock["symbol"], shares=int(numShares), price=stock["price"], total=totalOrderVal)
            # else append to the position's shares and update the price + total market value 
            else:
                # calculate total number of shares (current # shares in position + # shares ordered)
                curShares = db.execute("SELECT shares FROM positions WHERE acct_id = :user_id", user_id=session["user_id"])
                totalShares = curShares[0]["shares"] + int(numShares)
                # update the current position
                db.execute("UPDATE positions SET shares = shares + :numShares, price = :price, total = :totalMktVal WHERE acct_id = :acct_id AND symbol = :symbol",
                acct_id=session["user_id"], symbol=stock["symbol"], numShares=numShares, price=stock["price"], totalMktVal=totalShares*stock["price"])
        # else return apology
        else:
            return apology("insufficient cash to execute this order")
        
        # adjust the user's cash balance after purchase 
        db.execute("UPDATE users SET cash = cash - :total WHERE id = :user_id", total=totalOrderVal, user_id=session["user_id"])
        
        # record transaction in history table
        db.execute("INSERT INTO history (acct_id, symbol, shares, price, date_time) VALUES (:acct_id, :symbol, :shares, :price, CURRENT_TIMESTAMP)",
        acct_id=session["user_id"], symbol=stock["symbol"], shares=numShares, price=stock["price"])
        
        # redirect user to home page
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if user reached route via POST (as by submitting a sale order)
    if request.method == "POST":
            
        # ensure stock symbol was inputted
        if not request.form.get("symbol"):
            return apology("stock symbol is required")
            
        # ensure amount of shares is inputted
        if not request.form.get("num_shares"):
            return apology("number of shares is required ")
        
        # store the stock and number of shares 
        stock = lookup(request.form.get("symbol"))
        numShares = int(request.form.get("num_shares"))
        
        # if look up did not find any symbol, return apology 
        if not stock:
            return apology("the stock symbol was not found or is invalid")
        
        # retrieve amount of shares held for the stock
        curShares = db.execute("SELECT shares FROM positions WHERE symbol = :symbol AND acct_id = :user_id", symbol=stock["symbol"], user_id=session["user_id"])
        
        # calculate total value of the selling transaction 
        totalSellVal = stock["price"] * numShares
        
        # if all shares are sold, remove the stock from the account
        if curShares[0]["shares"] == numShares:
            # execute row delete 
            db.execute("DELETE FROM positions WHERE symbol = :symbol AND acct_id = :user_id", symbol=stock["symbol"], user_id=session["user_id"])
        # if user has shares remaining after sale, update the position 
        elif curShares[0]["shares"] >= numShares: 
            # calculate the number of shares remaining after sale (current # shares in position - # shares sold)
            newSharesAmt = curShares[0]["shares"] - numShares
            # update the position
            db.execute("UPDATE positions SET shares = :newSharesAmt, price = :price, total = :totalMktVal WHERE acct_id = :acct_id AND symbol = :symbol",
            acct_id=session["user_id"], symbol=stock["symbol"], newSharesAmt=newSharesAmt, price=stock["price"], totalMktVal=newSharesAmt*stock["price"])
        # else if the sales order exceeds the number of shares held, return apology
        elif curShares[0]["shares"] < numShares:
            return apology("insufficient number of shares to execute sale order")
        # else the user does not hold that security
        else:
            return apology("you do not hold that security")
        
        # adjust the user's cash balance after sale 
        db.execute("UPDATE users SET cash = cash + :total WHERE id = :user_id", total=totalSellVal, user_id=session["user_id"])
        
        # record transaction in history table
        db.execute("INSERT INTO history (acct_id, symbol, shares, price, date_time) VALUES (:acct_id, :symbol, :shares, :price, CURRENT_TIMESTAMP)",
        acct_id=session["user_id"], symbol=stock["symbol"], shares=numShares*-1, price=stock["price"])
        
        # redirect user to home page
        return redirect(url_for("index"))
    
    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")