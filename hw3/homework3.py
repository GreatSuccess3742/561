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

def PrintKB(sentence):
    for i in range(0,len(sentence)):
        print('--------- sentence {0} = '.format(i), '---------')
        for j in range(0, len(sentence[i])):
            print('Negate = ', sentence[i][j].negate)
            print('Predicate {0} = '.format(j) ,sentence[i][j].name, '\n contains:')
            for k in range(0, len(sentence[i][j].argument)):
                print('arg {0} = '.format(k), sentence[i][j].argument[k])

def StandardizeVariable(sentence):
    # Standardize by adding number of the setence in the back of the variable
    for i in range(0,len(sentence)):
        for j in range(0, len(sentence[i])):
            for k in range(0, len(sentence[i][j].argument)):
                if(sentence[i][j].argument[k][0].islower()):
                    sentence[i][j].argument[k] += str(i)

def Resolve(s1,s2):
    print('here = ',s1[1].name)
    print('here = ',s2[0].name)

def Resolution(singleQuery,sentence):
    negateQuery = Predicate(singleQuery.negate, singleQuery.name, singleQuery.argument)
    tempSentence = sentence[:]

    # Negate the Query and put it into KB:
    negateQuery.negate = not negateQuery.negate
    tempPredicate = []
    dummy = []
    tempPredicate = Predicate(negateQuery.negate, negateQuery.name, negateQuery.argument)
    dummy.append(tempPredicate)
    tempSentence.append(dummy)

    new = []
    while(1):
        for i in range(0, len(tempSentence)):
            for j in range(i+1, len(tempSentence)): #TODO

                Resolve(tempSentence[i],tempSentence[j])
                exit()



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

    # Standardize Variables:
    StandardizeVariable(sentence)

    # Do resolution:
    for qIndex in range(0,len(query)):
        Resolution(query[qIndex],sentence)





