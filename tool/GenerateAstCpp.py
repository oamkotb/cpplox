import os
from typing import List, TextIO


def defineVisitor(
        header_file: TextIO,
        source_file: TextIO,
        base_name: str,
        types: List[str]) -> None:
    # TODO: Write implementation of visitor creation
    ...


def defineAst(
        header_output_dir: str,
        source_output_dir: str,
        base_name: str,
        types: List[str]) -> None:

    header_path: str = os.path.join(header_output_dir, f"{base_name}.h")
    source_path: str = os.path.join(source_output_dir, f"{base_name}.cc")

    with open(header_path, 'w') as header_file:
        with open(source_path, 'w') as source_file:
            source_file.write("#praga once\n")
            source_file.write("\n")
            source_file.write(f"#include {base_name}.h\n")
            source_file.write("template <class R>\n")

            source_file.write()
            defineVisitor(header_file, source_file, base_name, types)


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
