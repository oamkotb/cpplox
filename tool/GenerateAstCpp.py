import os
from typing import List, TextIO


def defineVisitor(
        header_file: TextIO,
        base_name: str,
        types: List[str]) -> None:
    # TODO: Write implementation of visitor creation
    header_file.write("    struct Visitor {\n")
    for type_info in types:
        type_name: str = type_info.split(':')[0].strip()
        header_file.write(f"        virtual R visit{type_name}Expr(const class {type_name}& expr)=0;\n")
    header_file.write("    };\n")


def defineType(
        header_file: TextIO,
        source_file: TextIO,
        base_name: str,
        class_name: str,
        fields: str) -> None:

    # Beginning
    header_file.write(f"    class {class_name} : public {base_name}<R> {{\n")
    header_file.write("    public:\n")

    # Constructor
    header_file.write(f"        {class_name}({fields});\n")

    # Accept
    header_file.write(f"    R accept(const typename {base_name}<R>::Visitor& visitor) const override;\n")

    # Variables
    fields_list: List[str] = fields.split(',')
    for field in fields_list:
        header_file.write(f"{field.strip()};\n")

    header_file.write("};\n")


def defineAst(
        header_output_dir: str,
        source_output_dir: str,
        base_name: str,
        types: List[str]) -> None:

    header_path: str = os.path.join(header_output_dir, f"{base_name}.h")
    source_path: str = os.path.join(source_output_dir, f"{base_name}.cc")

    with open(header_path, 'w') as header_file:
        with open(source_path, 'w') as source_file:
            # Creating beginning of header file
            header_file.write("#praga once\n")
            header_file.write("\n")
            header_file.write('#include "Token.h"')
            header_file.write("\n")
            header_file.write("template <class R>\n")
            header_file.write(f"class {base_name}{{\n")
            header_file.write("public:\n")

            # Creating beginning of source file
            source_file.write(f"#include {base_name}.h\n")
            source_file.write("\n")

            defineVisitor(header_file, base_name, types)

            # The base accept() method
            header_file.write("    virtual R accept(const Visitor& visitor) const = 0;\n")

            # The AST classes.
            for type_info in types:
                splitted_types: List[str] = type_info.split(':')
                class_name: str = splitted_types[0].strip()
                fields: str = splitted_types[1].strip()
                defineType(header_file, source_file, base_name, class_name, fields)

            header_file.write("};\n")


def main():
    header_output_dir = input("Enter the header file directory: ")
    source_output_dir = input("Enter the source file directory: ")

    defineAst(header_output_dir, source_output_dir, "Expr", [
            "Binary  : Expr left, Token operator, Expr right",
            "Grouping: Expr expression",
            "Literal : Object value",
            "Unary   : Token operator, Expr right"
    ])


if __name__ == "__main__":
    main()
