# from utils import *
# for loop read file:
# for line in open(input_file_name):
#     print(line)

def Parser(sentence):
    print('parser')
    for index in range(0,len(sentence)):
        # Upper case letter
        if(sentence[index].isupper() and sentence[index].isalpha()):
            print('{} is upper'.format(sentence[index]))
        # Lower case letter
        elif(sentence[index].islower() and sentence[index].isalpha()):
            print('{} is not upper'.format(sentence[index]))
        # Left Parenthesis
        elif(sentence[index] == '('):
            print('(')
        # Right Parenthesis
        elif(sentence[index] == ')'):
            print(')')
        # Comma
        elif(sentence[index] == ','):
            print(',')
        # OR
        elif(sentence[index] == '|'):
            print('or')
        # Not
        elif(sentence[index] == '~'):
            print('not')
        # EOL
        elif(sentence[index] == '\n'):
            print('EOL')
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
            sentence = []
            for i in range(0,num_of_query):
                query += input_f.readline().replace(' ', '')
                # print(query)
            num_of_sentence = int(input_f.readline())
            for i in range(0,num_of_sentence):
                # sentence.append(input_f.readline())
                sentence += input_f.readline().replace(' ', '')
    input_f.closed

    # print(query[0])
    # print(sentence)
    # print(len(sentence))

    stack = []
    # stack += sentence
    # print(len(sentence))
    # a = len(sentence)
    
        # print('sentence {0} = '.format(index),sentence[index])
        # print('stack = ', stack)
    Parser(sentence)
    print('1234')


