import xml.parsers.expat
#from ModelEnumerations import *
from GUI.ModelEnumerations import ShopData
from GUI.ModelEnumerations import ComponentData
from GUI.ModelEnumerations import ComponentPrice


class ConfigurationXMLParser:
	Parser = None
	xml_file = ''
	cShop = None
	cComp = None
	cCompPrice = None
	listShops = []
	listComps = []

	def __init__(self,  xml_file):
		assert(xml_file != "")
		self.xml_file = xml_file
		self.Parser = xml.parsers.expat.ParserCreate()
		self.Parser.CharacterDataHandler = self.char_data
		self.Parser.StartElementHandler = self.start_element
		self.Parser.EndElementHandler = self.end_element


	def start_element(self,  name, attrs):
		#SHOP
		if name == 'SHOP':
			self.cShop = ShopData()
			self.cShop.bActive = 0
			if attrs['active'] == 'true':
				self.cShop.bActive = 1
			pass
		elif name == 'COMPONENT':
			self.cComp = ComponentData()
			self.cComp.bActive = 0
			if attrs['active'] == 'true':
				self.cComp.bActive = 1
			self.cComp.sName = attrs['name']
		elif name == 'DATA':
			self.cCompPrice = ComponentPrice()
			self.cCompPrice.iLastPrice = -1
			if  len(attrs['last_price']) > 0:
				self.cCompPrice.iLastPrice = int(attrs['last_price'])
	def end_element(self,  name):
		if name == 'SHOP':
			self.listShops.append(self.cShop)
			self.cShop = None
		elif name == 'COMPONENT':
			self.listComps.append(self.cComp)
			self.cComp = None
		elif name == 'DATA':
			self.cComp.listPrices.append(self.cCompPrice)
			self.cCompPrice = None
	def char_data(self,  data):
		if self.cShop <> None:
			self.cShop.sName=data
		elif self.cCompPrice <> None:
			self.cCompPrice.sUrl = data

	def parseConfigurationFile(self):
		with open(self.xml_file) as f:
			fileContent = f.readlines()
			content = ' '.join(fileContent)
			self.Parser.Parse(content)