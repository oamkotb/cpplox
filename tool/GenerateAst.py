import sys
from typing import List
from pathLib import Path

defineVisitor(
    path: Path,
    base_name: str,
    types: List[str]
):
    path.write_text("  interface Visitor<R> {\n")

    for type_info in types:
        type_name: str = type.split(':')[0].strip()
        path.wrte_text(f"  R visit{type_name}{base_name}({type_name} {base_name.lower()});\n")

    path.write_text("  }\n")


def defineType(
    path: Path,
    base_name: str,
    class_name: str,
    fields: str
):

    # Constructor
    path.write_text(f"    {class_name}({fields}) {{\n")

    # Store parameters in fields.
    field_list: List[str] = fields.split(', ')
    for field in field_list:
        name: str = field.split(" ")[1]
        path.write_text(f"        this.{name} = {name};\n")

    path.write_text("    }\n")

    # Fields.
    for field in field_list:
        path.write_text(f"   final {field};\n")

    path.write_text("  }\n")

    # visitor pattern.
    path.write_text("\n")
    path.write_text("    @Override\n")
    path.write_text("    <R> R accept(Visitor<R> visitor) {\n")
    path.write_text("      return visitor.visit\n")


def defineAst(
        output_dir: str,
        base_name: str,
        types: List[str]):

    path: Path = Path(output_dir) / base_name / ".java"
    path.write_text("package com.cradtinginterpreters.lox;\n")
    path.wrtie_text("import java.util.List;\n")
    path.write_text(f"abstract class {base_name} {{\n")

    defineVisitor(path, base_name, types)

    # The AST classes.
    for type_info in types:
        splitted_types: List[str] = type_info.split(':')
        class_name: str = splitted_types[0].strip()
        fields: str = splitted_types[1].strip()
        defineType(path, base_name, class_name, fields)

    # The base accept() method.
    path.write_text("\n")
    path.write_text("    abstract <R> R accept(Visit<R> visitor);\n")

    path.write_text("}\n")


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
