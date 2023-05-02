from py.schema import *
import untangle

class SchemaLoader(object):
    def __init__(self, projectFile):
        out = untangle.parse(projectFile)
        
        self.schema = Schema()
        # loadDataTypes
        for namespace in out.Schema.Namespace:
            self.load_data_types(namespace)
            self.load_enums(namespace)
            
        for namespace in out.Schema.Namespace:
            self.load_fields(namespace)
        
    def load_data_types(self, namespace):
        nsName = namespace['Name']
        resolvedNs = self.schema.get_or_create_namespace(nsName)
        if not ('DataType' in namespace):
            return
  
        for dataType in namespace.DataType:
            dtName = dataType['Name']
            includeFile = dataType['IncludeFile']
            dt = DataType(dtName, DataType.Category.SimpleType, includeFile)
            resolvedNs.add_data_type(dt)
            
    def load_enums(self, namespace):
        nsName = namespace['Name']
        resolvedNs = self.schema.get_or_create_namespace(nsName)
        if not ('Enum' in namespace):
            return
        for enum in namespace.Enum:
            enumName = enum['Name']
            enumStorage = enum['Storage'] if enum['Storage'] else "int"
            keyVal = {}
            for item in enum.Item:
                keyVal[item['Name']] = item['Value']
            newEnum = Enum(enumName, enumStorage, keyVal)
            resolvedNs.add_enum(newEnum)
        

    def load_fields(self, namespace):
        nsName = namespace['Name']
        resolvedNs = self.schema.get_or_create_namespace(nsName)

        if not ('Field' in namespace):
            return
        for field in namespace.Field:

            fldName = field['Name']
            dtTypeName = field['DataType']
            units = field['Units']
            if units == None:
                units  = ''
                
            tag = field["Tag"]

            dataNsName, dataTypeName = Schema.get_namespace_and_type_name(dtTypeName)
            lookUpNs = resolvedNs
            if dataNsName != None:
                lookUpNs = self.schema.get_or_create_namespace(dataNsName)
                
            resolvedDataType = lookUpNs.get_data_type(dataTypeName)
            newField = Field(fldName, tag, resolvedDataType, units)
            resolvedNs.add_field(newField)
