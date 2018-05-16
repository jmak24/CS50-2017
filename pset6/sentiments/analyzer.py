import nltk

class Analyzer():
    """Implements sentiment analysis."""
    
    # 2nd & 3rd arg - paths of text files containing the positive and negative words
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        # create set for both positive and negative words
        self.positive_set = set()
        self.negative_set = set()
        
        # open positives text file as pf
        with open(positives) as pf:
            # for every line in text file pf
            for line in pf:
                # if line does not start with ';' and is not empty
                if not line.strip(' ').startswith(';') and line != "":
                    # omit leading/trailing whitespaces and then cut the '\n'
                    word = line.strip(' ').rstrip('\n')
                    # load each word into positive_set
                    self.positive_set.add(word)
        
        # open negatives text file as nf
        with open(negatives) as nf:
            # for every line in text file pf
            for line in nf:
                # if line does not start with ';' and is not empty
                if not line.strip(' ').startswith(';') and line != "":
                    # omit leading/trailing whitespaces and then cut the '\n'
                    word = line.strip(' ').rstrip('\n')
                    # load each word into positive_set
                    self.negative_set.add(word)
    
    # 2nd arg - string to be analyzed for sentiment 
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # instantiate the tokenizer 
        tokenizer = nltk.tokenize.TweetTokenizer()
        # break up the text into a list of words 
        tokens = tokenizer.tokenize(text)
        
        # initialize score variable
        score = 0
        
        # iterate through words within token list
        for word in tokens:
            # turn word into lowercase
            word = word.lower()
            # if word is in the positive data set
            if word in self.positive_set:
                # increment sentiment score by 1
                score += 1
            # if word is in the negative data set
            elif word in self.negative_set:
                # decrement sentiment score by 1
                score -= 1
        
        return score
