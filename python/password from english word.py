# Nishant Shekhar Soni
# 31st dec 2021 1:41 PM

from nltk.corpus import words as nltk_words
from itertools import permutations, combinations

# creation of this dictionary would be done outside of 
# the function because you only need to do it once.
# using dictinary for fast answer.

def is_english_word(word):
    dictionary = dict.fromkeys(nltk_words.words(), None)
    try:
        x = dictionary[word]
        return True
    except KeyError:
        return False

def per(string):
    words = list(permutations( string ) )
    wordlist = []
    for i in words:
        wrd = ''
        for j in i:
            wrd += j
        wordlist.append(wrd)
    return wordlist

def combi(string):
    arr = list(string)
    combilist = []
    for r in range(2,len(string)+1):
        ncr = combinations(arr, r)
        combilist += ncr
    return combilist

def pnc_eng(string):
    all_words = []
    found_word = []
    combilist = combi(string)
    for wrd in combilist:
        all_words += per(wrd)
    
    for wrd in all_words:
        if is_english_word(wrd):
            found_word.append(wrd)
    return found_word

my_hexa = pnc_eng('abc')

for word in my_hexa:
    print(word)