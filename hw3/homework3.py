# from utils import *
# for loop read file:
# for line in open(input_file_name):
#     print(line)
import pdb

class Predicate(object):
    """docstring for Predicate"""
    def __init__(self, negate, name, argument):
        super(Predicate, self).__init__()
        self.negate = negate
        self.name = name
        self.argument = argument
        
def FindPredicateName(predicate):
    pLen = len(predicate)
    for i in range(0,pLen):
        if(predicate[i] == '('):
            pivot = i
            break
    if(predicate[0] == '~'):
        pName = predicate[1:pivot]
    else:
        pName = predicate[0:pivot]
    return pName

def FindPredicateArgument(predicate):
    pLen = len(predicate)
    for i in range(0, pLen):
        if(predicate[i] == '('):
            left = i
        if(predicate[i] == ')'):
            right = i
    tempArgument = predicate[left+1:right]
    pArgument = tempArgument.split(',')
    return pArgument


# Main fucntion:
if __name__ == '__main__':
    input_file_name = 'input.txt'
    output_file_name = 'output.txt'

    with open(input_file_name, 'r') as input_f:
        for line in input_f:
            read_data = line.strip().replace(' ', '').replace('\n','')
            num_of_query = int(read_data)
            tempQuery = []
            tempSentence = []
            for i in range(0,num_of_query):
                tempQuery.append(input_f.readline().strip().replace(' ', ''))

            # Store the data into the tempSentence 
            num_of_sentence = int(input_f.readline())
            for i in range(0,num_of_sentence):
                tempSentence.append(input_f.readline().strip().replace(' ', ''))
                tempSentence[i] = tempSentence[i].split('|')


    input_f.closed

    # Organizing Query
    print('len(tempQuery) = ', len(tempQuery))
    print('tempQuery = ',tempQuery)
    query = []
    for i in range(0, len(tempQuery)):
        if(tempQuery[i][0] == '~'):
            qNegate = True
        else:
            qNegate = False
        qName = FindPredicateName(tempQuery[i])
        qArgument = FindPredicateArgument(tempQuery[i])
        myQuery = Predicate(qNegate, qName, qArgument)
        query.append(myQuery)
    x = 1
    print('query = ',query[x].negate , query[x].name, query[x].argument)

    # Turn the tempSentence into sentence, sentence[i] is a list of predicate
    sentence = []
    predicate = []
    for i in range(0,len(tempSentence)):
        for j in range(0, len(tempSentence[i])):
            if(tempSentence[i][j][0] == '~'):
                pNegate = True
            else:
                pNegate = False
            pName = FindPredicateName(tempSentence[i][j])
            
            pArgument = FindPredicateArgument(tempSentence[i][j])
            myPredicate = Predicate(pNegate, pName, pArgument)
            predicate.append(myPredicate)

        #TODO: not sure if this is hard copy or not
        sentence.append(predicate[:])
        predicate = []


    # Do resolution:




