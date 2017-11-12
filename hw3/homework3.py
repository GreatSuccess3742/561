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

    mySentence = Sentence()
    myKB.sentence.append(mySentence)

    myPredicate = Predicate('')
    myKB.sentence[sIndex].predicate.append(myPredicate)

    myArgument = Argument('','')
    myKB.sentence[sIndex].predicate[pIndex].argument.append(myArgument)
    exit()
    stack = []
    

    # Check if parsing inside the query or not
    inBracket = False;

    for index in range(0,len(sBuffer)):
        # Not: push "~" into the sentence as part of the PREDICATE
        if(sBuffer[index] == '~'):
            # pdb.set_trace()
            stack += sBuffer[index]
            #stack += sentence[index]

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
            tempstack = []
            tempstack += stack
            tempPredicate = Predicate(tempstack)
            print(myKB.sentence[sIndex].predicate[pIndex])
            exit()
            stack = []

        # Comma: separate the CONSTANT or VARIABLES
        elif(sBuffer[index] == ','):
            if(stack[0].islower()):
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'CONSTANT')
                print('myArgument = ', myArgument.name,'attribute =',myArgument.attribute)
                stack = []
            elif(stack[0].isupper()):
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'VARIABLE')
                print('myArgument = ', myArgument.name,'attribute =',myArgument.attribute)
                stack = []

        # Right Parenthesis: ending of predicate
        elif(sBuffer[index] == ')'):
            inBracket = False
            if(stack[0].islower()):
                # Insert argument into predicate
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'CONSTANT')


                stack = []
            elif(stack[0].isupper()):
                tempStack = []
                tempStack += stack
                myArgument = Argument(tempStack,'VARIABLE')
                print('myArgument = ', myArgument.name,'attribute =',myArgument.attribute)
                stack = []
        
        # OR: add another predicate into the sentence
        elif(sBuffer[index] == '|'):
            pIndex += 1
            print('add predicate, current predicate number = {0}'.format(pIndex))

        # EOL
        elif(sBuffer[index] == '\n'):
            sIndex += 1
            print('add one sentence, current sentence number = {0}'.format(sIndex))
        else:
            print('*******ELSE**********')



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
    sentence = [123]
    # print(query[0])
    # print(sentence)
    # print(len(sentence))

    # stack += sentence
    # print(len(sentence))
    # a = len(sentence)
    
        # print('sentence {0} = '.format(index),sentence[index])
        # print('stack = ', stack)
    Parser(sBuffer)


