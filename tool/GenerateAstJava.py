import sys
import os
from typing import List, TextIO


def defineVisitor(
    file: TextIO,
    base_name: str,
    types: List[str]
):
    file.write("  interface Visitor<R> {\n")

    for type_info in types:
        type_name: str = type_info.split(':')[0].strip()
        file.write(f"  R visit{type_name}{base_name}({type_name} {base_name.lower()});\n")

    file.write("  }\n")


def defineType(
    file: TextIO,
    base_name: str,
    class_name: str,
    fields: str
):

    # Constructor
    file.write(f"    {class_name}({fields}) {{\n")

    # Store parameters in fields.
    field_list: List[str] = fields.split(', ')
    for field in field_list:
        name: str = field.split(" ")[1]
        file.write(f"        this.{name} = {name};\n")

    file.write("    }\n")

    # Fields.
    for field in field_list:
        file.write(f"   final {field};\n")

    file.write("  }\n")

    # visitor pattern.
    file.write("\n")
    file.write("    @Override\n")
    file.write("    <R> R accept(Visitor<R> visitor) {\n")
    file.write(f"      return visitor.visit{class_name}{base_name}(this);\n")
    file.write("    }")


def defineAst(
        output_dir: str,
        base_name: str,
        types: List[str]
):

    path: str = os.path.join(output_dir, f"{base_name}.java")
    with open(path, 'w') as file:
        file.write("package com.cradtinginterpreters.lox;\n")
        file.write("import java.util.List;\n")
        file.write(f"abstract class {base_name} {{\n")

        defineVisitor(file, base_name, types)

        # The AST classes.
        for type_info in types:
            splitted_types: List[str] = type_info.split(':')
            class_name: str = splitted_types[0].strip()
            fields: str = splitted_types[1].strip()
            defineType(file, base_name, class_name, fields)

        # The base accept() method.
        file.write("\n")
        file.write("    abstract <R> R accept(Visit<R> visitor);\n")

        file.write("}\n")


def main():
    if (len(sys.argv) != 2):
        raise RuntimeError("Usage: python3 GenerateAST.py <output directory>")
        sys.exit(64)

    outputDir: str = sys.argv[1]
    defineAst(outputDir, "Expr", [
            "Binary  : Expr left, Token operator, Expr right",
            "Grouping: Expr expression",
            "Literal : Object value",
            "Unary   : Token operator, Expr right"
    ])


if __name__ == "__main__":
    main()
