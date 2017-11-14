# from utils import *
# for loop read file:
# for line in open(input_file_name):
#     print(line)
import pdb

class KB(object):
    """docstring for KB"""
    def __init__(self):
        super(KB, self).__init__()
        self.sentence = []

class Sentence(object):
    """docstring for Sentence"""
    def __init__(self):
        super(Sentence, self).__init__()
        self.predicate = []
        
class Predicate:
    def __init__(self,name):
        super(Predicate, self).__init__()
        self.name = name
        self.argument = []
class Argument:
    def __init__(self, name, attribute):
        super(Argument, self).__init__()
        self.name = name
        self.attribute = attribute

def Parser(sBuffer):
    print('parser:')

    # Initialize myKB:

    myKB = KB()

    # Sentence index
    sIndex = 0

    # Predicate index
    pIndex = 0

    # Argument index
    aIndex = 0

    mySentence = Sentence()
    myKB.sentence.append(mySentence)

    myPredicate = Predicate('')
    myKB.sentence[sIndex].predicate.append(myPredicate)

    
    stack = []
    

    # Check if parsing inside the bracket or not
    inBracket = False;

    for index in range(0,len(sBuffer)):
        # Not: push "~" into the sentence as part of the PREDICATE
        if(sBuffer[index] == '~'):
            # pdb.set_trace()
            stack += sBuffer[index]
            

        # Upper case letter
        # Push letter into stack
        elif(sBuffer[index].isupper() and sBuffer[index].isalpha()):
            stack += sBuffer[index]

        # Lower case letter
        # Push letter into stack
        elif(sBuffer[index].islower() and sBuffer[index].isalpha()):
            stack += sBuffer[index]

        # Left Parenthesis, pop content in the stack out as PREDICATE
        # things after Left Praenthesis should be CONSTANT or VARIABLE
        elif(sBuffer[index] == '('):
            inBracket = True
            tempStack = []
            tempStack += stack
            tempPredicate = Predicate(tempStack)
            if(pIndex == 0):
                myKB.sentence[sIndex].predicate[pIndex] = tempPredicate

                myArgument = Argument('','')
                myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)
            else:
            	myKB.sentence[sIndex].predicate.append(tempPredicate)
            stack = []

        # Comma: separate the CONSTANT or VARIABLES
        elif(sBuffer[index] == ','):
            if(stack[0].islower()):
                tempStack = []
                tempStack += stack
                tempArgument = Argument(tempStack,'CONSTANT')

                if(pIndex == 0):
                    myArgument = Argument('','')
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)

                    myKB.sentence[sIndex].predicate[pIndex].argument[aIndex] = tempArgument
                    # print('here = ',myKB.sentence[0].predicate[0].argument[0].name)
                else:
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)
                stack = []
            elif(stack[0].isupper()):
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'VARIABLE')

                if(pIndex == 0):
                    myKB.sentence[sIndex].predicate[pIndex].argument[aIndex] = myArgument
                else:            	
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)
                stack = []
            aIndex += 1
        # Right Parenthesis: ending of predicate
        elif(sBuffer[index] == ')'):
            inBracket = False
            if(stack[0].isupper()):
                # Insert argument into predicate
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'CONSTANT')
                if(pIndex == 0):
                    myArgument = Argument('','')
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)

                    myKB.sentence[sIndex].predicate[pIndex].argument[aIndex] = myArgument
                else:
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)

                stack = []
            elif(stack[0].islower()):
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'VARIABLE')
                if(pIndex == 0):
                    myArgument = Argument('','')
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)

                    myKB.sentence[sIndex].predicate[pIndex].argument[aIndex] = myArgument
                else:
                    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)
                stack = []
        
        # OR: add another predicate into the sentence
        elif(sBuffer[index] == '|'):
            pIndex += 1

            myPredicate = Predicate('')
            myKB.sentence[sIndex].predicate.append(myPredicate)

            myArgument = Argument('','')
            myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)

            print('add predicate, current predicate number = {0}'.format(pIndex))

        # EOL
        elif(sBuffer[index] == '\n'):

            sIndex += 1
            pIndex = 0

            # Construct a new sentence holder
            mySentence = Sentence()
            myKB.sentence.append(mySentence)

            myPredicate = Predicate('')
            myKB.sentence[sIndex].predicate.append(myPredicate)

            myArgument = Argument('','')
            myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)

            print('add one sentence, current sentence number = {0}'.format(sIndex))
        else:
            print('*******ELSE**********')

    sIndex = 0;
    pIndex = 0;
    aIndex = 0;
    for sentence in myKB.sentence:
        for predicate in myKB.sentence[sIndex].predicate:
            print('sentence {0}, predicate {1} = '.format(sIndex, pIndex),myKB.sentence[sIndex].predicate[pIndex].name)
            for argument in myKB.sentence[sIndex].predicate[pIndex].argument:
                print('argument = ', myKB.sentence[sIndex].predicate[pIndex].argument[aIndex].name)
                aIndex += 1
            pIndex += 1
            aIndex = 0;

        sIndex += 1
        pIndex = 0;

    # exit()
    return myKB





# Main fucntion:
if __name__ == '__main__':
    input_file_name = 'input.txt'
    output_file_name = 'output.txt'

    with open(input_file_name, 'r') as input_f:
        for line in input_f:
            # read_data = line.replace('\n','').replace(' ', '')
            read_data = line.strip().replace(' ', '')
            num_of_query = int(read_data)
            query = []
            sBuffer = []
            for i in range(0,num_of_query):
                query += input_f.readline().replace(' ', '')
                # print(query)
            num_of_sentence = int(input_f.readline())
            for i in range(0,num_of_sentence):
                # sentence.append(input_f.readline())
                sBuffer += input_f.readline().replace(' ', '')
    input_f.closed
    # print(query[0])
    # print(sentence)
    # print(len(sentence))

    # stack += sentence
    # print(len(sentence))
    # a = len(sentence)
    
        # print('sentence {0} = '.format(index),sentence[index])
        # print('stack = ', stack)
    myKB = Parser(sBuffer)