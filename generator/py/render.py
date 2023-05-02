import py.schema as schema
import cogapp
import os
from typing import *

def prepareDir(outPath, ns, elementType):
    nsPath = '/'.join(ns.components)
    outDir = os.path.join(outPath, nsPath,  elementType )
    if not os.path.exists(outDir):
        os.makedirs(outDir)
    return outDir
                                
class FieldRenderer(object):
    def __init__(self, cog,  field, schema):
        self.cog = cog
        self.field = field
        self.schema = schema
        
    def gen_namespace(self):
        return self.field.namespace.get_name()
      
    def gen_includes(self, gen):
        file = self.field.get_data_type().get_include_file()
        if file != None:
            gen.outl("""#include <{file}>""".format(file=file))
            
    def generate(self, out : str):
         globals = {'field' : self.field, 'schema' : self.schema, 'renderer' : self}
         self.cog.processFile('templates/field.cog', out, globals=globals)        


class EnumRenderer(object):
    def __init__(self, cog,  enum : schema.Enum, schema):
        self.cog = cog
        self.enum = enum
        self.schema = schema
                
    def gen_namespace(self):
        return self.enum.namespace.get_name()
    
    def gen_name_2_val(self) -> str:
        lines = []
        for k,v in self.enum.get_key_val().items():
            lines.append ( """boost::mp11::mp_list< TYPE_STRING("{name}"), enum_constant< data_type::{name} > >""".format(name=str(k)))
            
        return ',\n'.join(lines)
    
    def gen_val_2_name(self) -> str:
        lines = []
        for k,v in self.enum.get_key_val().items():
            lines.append ( """boost::mp11::mp_list< enum_constant< data_type::{name} >, TYPE_STRING("{name}")""".format(name=str(k)))
            
        return ',\n'.join(lines)
                  
    def gen_enum_vals(self) -> str:
        lines = []
        for k,v in self.enum.get_key_val().items():
            lines.append ( """ {key} = {value}""".format(key=str(k), value = v))
            
        return ',\n'.join(lines)

    def gen_to_string_view(self) -> str:
        lines = []
        for k,v in self.enum.get_key_val().items():
            lines.append ( """case data_type::{name}: return "{name}"sv;""".format(name=str(k)))
        return '\n'.join(lines)
    
    
    def gen_from_string_view(self) -> str:
        lines = []
        for k,v in self.enum.get_key_val().items():
            lines.append ( """case TypeLib::string_hash("{name}"sv): if (s == "{name}"sv) {{ val_ = data_type::{name};}} break;
        """.format(name=str(k)))
        return '\n'.join(lines)
    
    def generate(self, out : str):
         globals = {'enum' : self.enum, 'schema' : self.schema, 'renderer' : self}
         self.cog.processFile('templates/enum.cog', out, globals=globals)        

class Renderer(object):
    def __init__(self, schema : schema.Schema):
        self.schema = schema
        self.cog = cogapp.Cog()
        self.cog.options.bDeleteCode = True
            
    def generate(self, outPath : str):
        for nsName, ns in self.schema.all_namespaces().items():
            for field in ns.all_fields():
                prefix = prepareDir(outPath, ns, 'Fields')
                outFile = os.path.join( prefix, field.get_name() + ".h")
                FieldRenderer(self.cog, field, self.schema).generate(outFile)
            for enum in ns.all_enums():
                prefix = prepareDir(outPath, ns, 'Enums')
                outFile = os.path.join( prefix, enum.get_name() + ".h") 
                EnumRenderer(self.cog, enum, self.schema).generate(outFile)              