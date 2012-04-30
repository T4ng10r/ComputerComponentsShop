from xml.parsers.expat import *


def start_element(name, attrs):
    print 'Start element:', name, attrs
def end_element(name):
    print 'End element:', name
def char_data(data):
    print 'Character data:', repr(data)    


def parseConfigurationFile(filePath):
    with open(filePath) as f:
        fileContent = f.readlines()
    
        #p = xml.parsers.expat.ParserCreate()
        p = ParserCreate()

        p.StartElementHandler = start_element
        p.EndElementHandler = end_element
        p.CharacterDataHandler = char_data
        content = ' '.join(fileContent)
        p.Parse(content)

