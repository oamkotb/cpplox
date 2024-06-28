import os
from typing import List, TextIO, Dict


def parseTypes(
        types: List[List[str]]) -> List[Dict[str, str]]:
    '''Parses the types'''

    splitted_types: List[List[str]] = [type_info.split(":")
                                       for type_info in types]
    parsed_types: List[Dict[str, str]] = [{'class_name': type_info[0].strip(),
                                           'fields': type_info[1].strip()}
                                          for type_info in splitted_types]

    return parsed_types


def defineVisitorHeader(
        header_file: TextIO,
        base_name: str,
        types: List[str]) -> None:
    header_file.write("  struct Visitor\n")
    header_file.write("  {\n")

    for type_info in types:
        header_file.write(f"    virtual R visit{type_info['class_name']}Expr"
                          f"(const {type_info['class_name']} expr) = 0;\n")


def defineTypeHeader(
        header_file: TextIO,
        base_name: str,
        class_name: str,
        fields: str) -> None:
    # Beginning
    header_file.write(f"  class{class_name} : public {base_name}<R>\n")
    header_file.write("{\n")
    header_file.write("public:\n")

    # Constructor
    header_file.write(f"    {class_name}({fields})")

    # Accept method
    header_file.write("    R accept(const Visitor& visitor) override\n")
    header_file.write("{\n")
    header_file.write(f"      return visitor.visitr{class_name}Expr(this);\n")
    header_file.wrtie("\n")

    # declarations
    fields: List[str] = fields.split(",")
    fields: List[str] = [field_info.strip() for field_info in fields]

    for type_decl in fields:
        header_file.write(f"    {type_decl};\n")

    header_file.write("};\n")


def defineHeader(
        header_output_dir: str,
        base_name: str,
        types: List[List[str]]) -> None:

    header_path: str = os.path.join(header_output_dir, f"{base_name}.h")
    with open(header_path, 'w') as header_file:
        # Creating beginning of header file
        header_file.write("#praga once\n")
        header_file.write("\n")
        header_file.write('#include "Token.h"')
        header_file.write("\n")
        header_file.write("template <class R>\n")
        header_file.write(f"class {base_name}{{\n")
        header_file.write("public:\n")

        # Predefinitons for types
        for type_info in types:
            header_file.write(f"  clase {type_info[1]};\n")

        defineVisitorHeader(header_file, base_name, types)

        # The base accept method
        header_file.write("    R accept(const Visitor& visitor) override")

        # The AST Methods
        for type_info in types:
            defineTypeHeader(header_file,
                             base_name,
                             type_info['class_name'],
                             type_info['fields'])

        header_file.write("};\n")


def defineSource(
        source_output_dir: str,
        base_name: str,
        types: List[List[str]]) -> None:

    source_path: str = os.path.join(source_output_dir, f"{base_name}.cc")
    with open(source_path, 'w') as source_file:
        # Creating beginning of source file
        source_file.write(f"#include {base_name}.h\n")
        source_file.write("\n")


def defineAst(
        header_output_dir: str,
        source_output_dir: str,
        base_name: str,
        types: List[str]) -> None:

    parsed_types = parseTypes(types)

    defineHeader(header_output_dir, base_name, parsed_types)
    defineSource(source_output_dir, base_name, parsed_types)


def main():
    header_output_dir = input("Enter the header file directory: ")
    source_output_dir = input("Enter the source file directory: ")

    defineAst(header_output_dir, source_output_dir, "Expr", [
            "Binary  : const Expr*& left, const Token& oper, const Expr*& right",
            "Grouping: const Expr*& expression",
            "Literal : const Token::LiteralValue& value",
            "Unary   : const Token& operator, const Expr*& right"
    ])


if __name__ == "__main__":
    main()
