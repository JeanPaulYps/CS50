from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    Alines = a.split("\n")
    Blines = b.split("\n")
    output = set()

    for i in range (min (len (Alines), len (Blines) ) ):
        if (Alines[i] == Blines[i]):
            output.add(Alines[i])

    return list(output)


def sentences(a, b):
    """Return sentences in both a and b"""
    Asentences = sent_tokenize(a)
    Bsentences = sent_tokenize(b)
    output = set()

    for i in range (min( len(Asentences), len(Bsentences) ) ):
        if (Asentences[i] == Bsentences[i]):
            output.add(Asentences[i])

    return list(output)

def getSubstrings(text, n):
    numSubstrings = len(text)-(n+1)
    output = []
    for i in range(numSubstrings):
        start = i
        end = i + n
        output.append(text[start:end])
    return output

def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    Asubstrings = getSubstrings(a, n)
    Bsubstrings = getSubstrings(b, n)
    maxNumElements = (max( len(Asubstrings), len(Bsubstrings) ) )
    output = set()

    if (len(a) > len(b)):
        for word in Asubstrings:
            if (word in Bsubstrings):
                output.add(word)
    else:
        for word in Bsubstrings:
            if (word in Asubstrings):
                output.add(word)

    return list(output)
