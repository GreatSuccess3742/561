# from utils import *
# for loop read file:
# for line in open(input_file_name):
#     print(line)
import pdb
import re

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

def PrintSentence(sentence):
    print('--------- sentence ---------')
    for j in range(0, len(sentence)):
        print('Negate = ', sentence[j].negate)
        print('Predicate {0} = '.format(j) ,sentence[j].name, '\n contains:')
        for k in range(0, len(sentence[j].argument)):
            print('arg {0} = '.format(k), sentence[j].argument[k])

def StandardizeVariable(sentence):
    # Standardize by adding number of the setence in the back of the variable
    for i in range(0,len(sentence)):
        for j in range(0, len(sentence[i])):
            for k in range(0, len(sentence[i][j].argument)):
                if(sentence[i][j].argument[k][0].islower()):
                    sentence[i][j].argument[k] += str(i)

def Resolution(singleQuery,sentence):
    myCount = 0
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
        print('myCount = ',myCount)
        myCount += 1
        for i in range(0, len(tempSentence)):
            for j in range(i, len(tempSentence)): #TODO
                resolvents, flag = Resolve(tempSentence[i],tempSentence[j])
                # if(i == 0 and j == 1):
                #     print('i = {0}, j = {1}'.format(i,j) )
                #     print('tempSentence[i] =')
                #     PrintSentence(tempSentence[i])
                #     print('tempSentence[j] =')
                #     PrintSentence(tempSentence[j])
                #     print('r , f = ',resolvents,flag)
                #     print(len(resolvents))
                #     for ic,c in enumerate(resolvents):
                #         print('c = ',c[ic].name,c[ic].argument,c[ic].negate)
                        
                if(len(resolvents) == 0 and flag == True):
                    print('going to return true')
                    return True

                for k in range(0, len(resolvents)):
                    # print('k = ',k)
                    new.append(resolvents[k])
                     
        strNew = KB2String(new)
        strKB = KB2String(tempSentence)

        # print('new = ', strNew)        
        # print('strKB = ', strKB)

        
        KBtable = {}
        for s in strKB:
            KBtable[s] = 'exist'

        # print(KBtable)
        

        # if can not find new sentence in the old KB, extended KB
        find = True
        count = 0
        for sIndex,s in enumerate(strNew):
            if(KBtable.get(s) == None):
                find = False
                for t in range(0,len(new[sIndex])):
                    # eliminate the number after variable
                    for aIndex, argument in enumerate(new[sIndex][t].argument):
                        if(argument[0].islower()):
                            number = len(strKB) + count
                            new[sIndex][t].argument[aIndex] = re.sub('\d', str(number), argument)
                count += 1
                tempSentence.append(new[sIndex])

        # all the new sentences were found in the old KB
        if(find == True):
            return False
        # print(KBtable)

def KB2String(KB):
    tempKB = KB[:]
    strKB = []

    for i in range(0, len(tempKB)):
        
        # Sort all the sentence by negate and name:
        tempKB[i].sort(key=lambda x: (-x.negate ,x.name), reverse = False)

        # Eliminate all the number after variable
        for j in range(0, len(tempKB[i])):
            # exit()
            for aIndex, argument in enumerate(tempKB[i][j].argument):
                if(argument[0].islower()):
                    tempKB[i][j].argument[aIndex] = re.sub('\d', '', argument)
        # Turn KB into string
        tempStr = []

        for jIndex,j in enumerate(tempKB[i]):
            if(j.negate == True):
                tempStr += '~'

            tempStr.append(j.name)
            tempStr += '('

            for kIndex,k in enumerate(j.argument):
                tempStr.append(k)
                if(kIndex != len(j.argument)-1):
                    tempStr += ','
            tempStr += ')'

            if(jIndex != len(tempKB[i])-1):
                tempStr += '|'
        tempStr = "".join(tempStr)

        # ap
        strKB.append(tempStr)

    return strKB

def Resolve(s1,s2):
    resolvents = []
    # print('s1 = ')
    # PrintSentence(s1)
    # print('s2 = ')
    # PrintSentence(s2)

    flag = False
    for i in range(0,len(s1)):
        for j in range(0,len(s2)):
            if(s1[i].name == s2[j].name and (not s1[i].negate == s2[j].negate )):
                tempSentence = []

                # Unification
                theta = {}
                theta = Unify(s1[i].argument, s2[j].argument, theta)
                if(theta != False):
                    flag = True
                else:
                    continue
                # s1:
                for m in range(0, len(s1)):
                    if(m != i):
                        tempPredicate = Predicate(s1[m].negate, s1[m].name, s1[m].argument)
                        for n in range(0, len(tempPredicate.argument)):
                            # if the substitution exist, then substitue the variable with value
                            if(theta.get(s1[m].argument[n]) != None):
                                tempPredicate.argument[n] = theta.get(tempPredicate.argument[n])
                        
                        tempSentence.append(tempPredicate) # TODO: check data structure

                # s2:
                for m in range(0, len(s2)):
                    if(m != i):
                        tempPredicate = Predicate(s2[m].negate, s2[m].name, s2[m].argument)
                        for n in range(0, len(tempPredicate.argument)):
                            # if the substitution exist, then substitue the variable with value
                            if(theta.get(s2[m].argument[n]) != None):
                                tempPredicate.argument[n] = theta.get(tempPredicate.argument[n])
                        
                        tempSentence.append(tempPredicate) # TODO: check data structure

                if(len(tempSentence) != 0):
                    resolvents.append(tempSentence)

    # print('len(resolvents) = ', len(resolvents))
    # print('resolvents 0 = ', resolvents[0][0].name, resolvents[0][1].name, resolvents[0][2].name)
    # print('resolvents 1 = ', resolvents[1][0].name, resolvents[1][1].name, resolvents[1][2].name)
    # print('resolvents 1 = ', resolvents[1][0].argument, resolvents[1][1].argument, resolvents[1][2].argument)
    return resolvents, flag

def Unify(x, y, theta):
    # x: variable, constant or list
    # y: variable, constant or list
    # theta: substitution built up so far

    # Use False to represent failure:
    if(theta == False):
        return False
    elif(x == y): #TODO: check if x and y content's are the same
        return theta

    # Variable?(x)
    elif(Variable(x)):
        return UnifyVar(x, y, theta)

    # Variable?(y)
    elif(Variable(y)):
        return UnifyVar(y, x, theta)

    # LIST?(x) and LIST?(y)
    elif(List(x) and List(y)):
        return Unify(x[1:], y[1:], Unify(x[0:1], y[0:1], theta))

    else:
        # print('False')
        return False

    # elif ():

def Variable(x):
    # Check whether x is a varaible or not
    
    # x is a list:
    if(len(x) > 1):
        return False

    # x is a constant
    elif(x[0][0].isupper()):
        return False

    else:
        return True

def UnifyVar(var, x, theta):
    # Theta is empty, no substitution
    if(theta.get(var[0]) != None):
        return Unify(theta.get(var), x, theta)
    elif(theta.get(x[0]) != None):
        return Unify(var, theta.get(x), theta)
    else:
        theta[var[0]] = x[0]
        return theta
def List(x):
    # Check whether x is a list or not
    if(len(x) > 1):
        return True
    else:
        return False

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
        print(Resolution(query[qIndex],sentence))





