#!/usr/bin/env python3

import sys
import os
import helpers
#import nltk

from analyzer import Analyzer
from termcolor import colored

def main():

    # ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./tweets @username")
    
    # create screen_name variable using 2nd argument 
    screen_name = sys.argv[1]
    
    # call get_user_timeline function 
    tweets = helpers.get_user_timeline(screen_name, 50)
    
    # if query to Twitter failed, exit the 
    if tweets is None:
        sys.exit("No query results returned")
    
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    
    # instantiate the analyzer
    analyzer = Analyzer(positives, negatives)
    
    # iterate through list of tweets
    for tweet in tweets:
        # pass each tweet through the analyzer and get the score
        score = analyzer.analyze(tweet)
        # print appropriate result 
        if score > 0.0:
            print(colored("{} {}".format(score, tweet), "green"))
        elif score < 0.0:
            print(colored("{} {}".format(score, tweet), "red"))
        else:
            print(colored("{} {}".format(score, tweet), "yellow"))
        
    
if __name__ == "__main__":
    main()
    
