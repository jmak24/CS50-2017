import helpers
import os
import sys 

from flask import Flask, redirect, render_template, request, url_for
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    # redirects to index if screen_name is missing 
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)
    # redirect to index if not tweets returned 
    if tweets is None:
        return redirect(url_for("index"))
    
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    
    # instantiate the analyzer
    analyzer = Analyzer(positives, negatives)
    
    # call get_user_timeline function 
    tweets = helpers.get_user_timeline(screen_name, 100)
    
    # initialize positive, negative, neutral variables
    positive, negative, neutral = 0, 0 ,0
    
    # iterate through list of tweets
    for tweet in tweets:
        # pass each tweet through the analyzer and get the score
        score = analyzer.analyze(tweet)
        # keep track of positive, negative and neutral tweets
        if score > 0.0:
            positive += 1
        elif score < 0.0:
            negative += 1
        else:
            neutral += 1
    
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
