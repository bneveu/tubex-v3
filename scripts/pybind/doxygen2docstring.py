# -*- coding: utf-8 -*-
""" doxygen2docstring.py

  This script converts XML file generated by Doxygen (C++)
  into docstring format (Python). The output is a C++ file
  containing char* strings of docstring values, available as
  constants. They can be used with pybind11 for sharing C++
  documentation to Python binding.
  
  Project: tubex-lib
  Author: Simon Rohou
  Date: 2020
"""

import os
import sys
import xml.etree.ElementTree as ET


def anchor_id(memberdef, filename):
  '''
  Extract the anchor id from full string name obtained from Doxygen  <anchor> tag
  '''

  anchor = filename.replace(".h", "") \
                   .replace(".cpp", "") \
                   .replace("tubex_", "") \

  # Return value
  #if memberdef.get("const") == "yes":
  #  anchor += "_const"

  anchor += "_" + "".join(memberdef.find(".//type").itertext()).replace("_", "")

  anchor += "_" + memberdef.find("./name").text

  # List of arguments (if any)
  for param in memberdef.findall("./param"):
    anchor += "_" + get_tags_text(param.find("type")).replace("_", "")

  return anchor.replace("<", "") \
               .replace(">", "") \
               .replace(",", "") \
               .replace(" ", "") \
               .replace(".", "") \
               .replace("std::", "") \
               .replace("ibex::", "") \
               .replace("tubex::", "") \
               .replace("::", "") \
               .replace("@", "_") \
               .replace("|=", "UNIEQ") \
               .replace("&=", "INTEQ") \
               .replace("/=", "DIVEQ") \
               .replace("*=", "MULEQ") \
               .replace("+=", "ADDEQ") \
               .replace("-=", "MINEQ") \
               .replace("==", "EQ") \
               .replace("!=", "NEQ") \
               .replace("=", "AFF") \
               .replace("()", "P") \
               .replace("(", "") \
               .replace(")", "") \
               .replace("&", "") \
               .replace("[]", "B") \
               .replace("friend", "") \
               .replace("~", "destruct_") \
               .replace("__", "_") \
               .replace("*", "") \
               .upper()

def normalize(str):
  '''
  Performs normalization on str: removes C++ conventions, converts
  some math symbols, trim the string, etc.
  '''
  return str.replace("const", "") \
            .replace("std::", "") \
            .replace("ibex::", "") \
            .replace("tubex::", "") \
            .replace("< ", "<") \
            .replace(" >", ">") \
            .replace("*", "") \
            .replace("\\cdot", "??") \
            .replace("\\infty", "???") \
            .replace("\\forall", "???") \
            .replace("\\in[", "??[") \
            .replace("\\in ", "?? ") \
            .replace("\\int", "???") \
            .replace("\\tau", "??") \
            .replace("\\exists ", "??? ") \
            .replace("\\mid ", " | ") \
            .replace("\\delta", "??") \
            .replace("::", ".") \
            .replace("  ", " ") \
            .replace("$", "") \
            .replace("\n", "") \
            .replace("&", "").strip()

def get_tags_text(tag):
  '''
  Return the content of a tag, including the content of its sub-items.
  '''
  return normalize("".join(tag.itertext()))

def sentence(str):
  '''
  Adds a point at the end of str, if not already present.
  '''
  return (str + ".").replace("..",".")

if not os.path.exists(sys.argv[1]):
  print("-- /!\\ Enable to build doc files (generate XML Doxygen files first)")
  quit()

files = os.listdir(sys.argv[1])
for xml_doc in files:

  if "class" not in xml_doc:
    continue

  namespace = "tubex"
  indent = "  "

  # Parsing XML file from Doxygen output (XML mode enabled)

  tree = ET.parse(sys.argv[1] + "/" + xml_doc)
  root = tree.getroot()

  input_filename = root.find(".//location").get("file").split("/")[-1]
  output_filename = input_filename.replace(".h", "_docs.h").replace("tubex_", "tubex_py_")
  output_file_fullpath = sys.argv[2] + "/" + output_filename

  # We do not want to create again the files if nothing has changed since last
  # generation (it would cause unnecessary compilation of python-binding files,
  # quite long). So we compare the modification dates.

  # Actual last update of the XML file
  last_update_time = os.path.getmtime(sys.argv[1] + "/" + xml_doc)
  last_update_time_str = "// Last update of XML file: " + str(last_update_time)

  # Last update of previous generation (if any)
  first_line = ""
  if os.path.isfile(output_file_fullpath):
    with open(output_file_fullpath) as f_prev:
      first_line = f_prev.readline()

  if first_line.strip() == last_update_time_str.strip(): # no update
    continue
  
  with open(output_file_fullpath, 'w') as f:
    
    print(last_update_time_str, file=f) # to check last update for next generations of files
    print("// This file has been generated by CMake and doxygen2docstring.py", file=f)
    print("// From XML file: " + xml_doc + "\n", file=f)

    # Refers to all memberdef containing an <anchor> tag
    for memberdef in root.findall(".//memberdef"):

      if memberdef.find("./definition") == None:
        continue

      doc_id = anchor_id(memberdef, input_filename)

      # Doc string
      print("// " + memberdef.find("./definition").text, end="", file=f)
      argstring = memberdef.find("./argsstring").text
      if argstring:
        print(argstring, end="", file=f)
      print("", file=f)

      print("const char* " +  doc_id +  " = R\"_docs(", end="", file=f)

      # Brief description
      briefdescription = sentence(get_tags_text(memberdef.find("briefdescription")))
      print(briefdescription, file=f)

      # Optional notes
      list_notes = memberdef.findall(".//simplesect[@kind='note']")
      if len(list_notes):
        print("", file=f)
      for note in list_notes:
        print(sentence(get_tags_text(note)), file=f)

      # List of arguments (if any)
      params = memberdef.findall("./param")

      if len(params):
        print("\nArgs:", file=f)

        for param in params:
          param_name = param.find("declname").text
          print(indent + param_name, "(" + get_tags_text(param.find("type")) + "): ", end='', file=f)

          parameterlist = memberdef.find("detaileddescription/para/parameterlist")

          if parameterlist:
            for parameteritem in parameterlist.iter("parameteritem"):
              if parameteritem.find("parameternamelist").find("parametername").text == param_name:
                param_description = sentence(get_tags_text(parameteritem.find("parameterdescription")))
                print(param_description, file=f)

      # Return value (if any)
      return_val = memberdef.find(".//simplesect[@kind='return']")
      if return_val:
        print("\nReturns:", file=f)
        print(indent + sentence(get_tags_text(return_val.find("para"))), file=f)

      # Ending
      print(")_docs\";\n", file=f)