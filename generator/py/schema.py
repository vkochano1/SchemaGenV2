from typing import *

class DataType(object):
    class Category(object):
        SimpleType = 1
        Enum  =  2
        ComplexType = 3
        
    def __init__(self, name : str, category : Category, includeFile : str = None):
        self.name = name
        self.category = category
        self.includeFile = includeFile
        self.namespace = None
    
    def assign_namespace(self, namespace):
        self.namespace = namespace
        
    def get_name(self):
        return self.name
    
    def get_class_name(self):
        return self.namespace.get_name() + '::' + self.name
    
    def get_include_file(self):
        return self.includeFile
    
class Enum(DataType):
    def __init__(self, name, storage , keyVal):
        super().__init__(name, DataType.Category.Enum)
        self.storage = storage
        self.keyVal = keyVal
        self.name = name
        
    def get_key_val(self):
        return  self.keyVal
    
    def get_storage(self) -> str:
        return self.storage
      
    def get_class_name(self):
        return super().get_class_name() + "_enum"
    
    def assign_namespace(self, namespace):
        self.includeFile = '/'.join(namespace.components) + '/Enums/' + self.name + ".h"
        return super().assign_namespace(namespace)
    
class Field(object):
    def __init__(self, name, tag : int,  dataType : DataType, units : str):
        self.dataType = dataType
        self.name = name
        self.tag = tag
        self.units = units
        
    def assign_namespace(self, namespace):
        self.namespace = namespace
        
    def get_data_type(self):
        return self.dataType
    
    def get_name(self):
        return self.name
    
    def get_tag(self) -> int:
        return self.tag
    
    def get_units(self) -> str:
        return self.units
    
class Namespace(object):
    def __init__(self, pathName : str):
        self.components = pathName.split("::")
        # DataTypes = basic/user types, containers
        self.dataTypes  = []
        self.dataTypesByName = {}
   
        # Fields
        self.fields     = []
        self.fieldsByName = {}
        
        # Enums  
        self.enums = []
        self.enumsByName = {}
        
        # Containers
        self.containers = []
        self.containersByName = {}

    def get_name(self):
        return  '::'.join(self.components)
                
    def all_fields (self):
        return self.fields

    def all_enums (self):
        return self.enums
         
    def add_data_type(self, dataType):
        dataType.assign_namespace(self)
        self.dataTypesByName[dataType.get_name()] = dataType
        self.dataTypes.append(dataType)
    
    def add_field(self, field):
        field.assign_namespace(self)
        self.fieldsByName[field.get_name()] = field
        self.fields.append(field)

    def add_enum(self, enum):
        #enum.assign_namespace(self)
        self.enumsByName[enum.get_name()] = enum
        self.add_data_type(enum)
        self.enums.append(enum)

    def get_data_type(self, dataTypeName : str):
        return self.dataTypesByName[dataTypeName]
    
    def get_field_Name(self, fieldName : str):
        return self.fieldsByName[fieldName]
        

class Schema (object):        
    def __init__(self):
        self.namespaces = {}    
        
    def all_namespaces(self):
        return self.namespaces
    
    def get_or_create_namespace(self, fullName):
        namespace = self.namespaces.get(fullName)
        if not namespace:
            newNamespace = Namespace(fullName)
            self.namespaces[fullName] = newNamespace
            return newNamespace
        
        return namespace
    
    @staticmethod
    def get_namespace_and_type_name(fullName):
        components = fullName.split('::')
        if len(components) == 1:
            return (None, fullName)
        return ("::".join(components[:-1]), components[-1])
    
    def resolve_data_type(self, fullName : str):
        namespaceStr, dataTypeName = Schema.get_namespace_and_type_name(fullName)
        resolvedNamespace = self.namespaces[namespaceStr]    
        return resolvedNamespace.get_data_type(dataTypeName)
    
    def resolve_field(self, fullName):
        namespaceStr, fieldName = Schema.get_namespace_and_type_name(fullName)
        resolvedNamespace = self.namespaces[namespaceStr]    
        return resolvedNamespace.get_field(fieldName)
