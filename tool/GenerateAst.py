import os
import sys
from typing import List, TextIO, Dict

class Param:
    def __init__(self, value):
        self.value = value
        self.is_pointer = self.isPointer(value)
        self.value = self.value.split(f"{'*' if self.is_pointer else '&'}")[1].strip()

    def isPointer(self, value: str) -> bool:
        return '*' in value

def defineConstructor(
        file: TextIO,
        class_name: str,
        fields: str) -> None:
    parameters: List[str] = fields.split(',')
    parameters: List[Param] = [Param(param)
                             for param in parameters]
    file.write(f"  {class_name}({fields}):\n")
    file.write('    ')
    for i, param in enumerate(parameters, start=1):
        file.write(f"{param.value}({param.value})")
        if i == len(parameters):
            file.write(' {}\n')
        else:
            file.write(', ')

    # Destructor
    if any(map(lambda x: x.is_pointer ,parameters)):
        file.write('\n')
        file.write(f'  ~{class_name}()\n')
        file.write('  {\n')
        for param in parameters:
            if param.is_pointer:
                file.write(f'    delete {param.value};\n')
        file.write('  }\n')


def parseTypes(
        types: List[List[str]]) -> List[Dict[str, str]]:
    '''Parses the types'''

    splitted_types: List[List[str]] = [type_info.split(":", maxsplit=1)
                                       for type_info in types]
    parsed_types: List[Dict[str, str]] = [{'class_name': type_info[0].strip(),
                                           'fields': type_info[1].strip()}
                                          for type_info in splitted_types]

    return parsed_types


def defineVisitor(
        file: TextIO,
        base_name: str,
        types: List[str]) -> None:
    file.write("  struct Visitor\n")
    file.write("  {\n")

    for type_info in types:
        file.write(f"    virtual R visit{type_info['class_name']}{base_name}"
                   f"(const {base_name}<R>::{type_info['class_name']}& {base_name.lower()}) = 0;\n")
    file.write("  };\n")


def defineType(
        file: TextIO,
        base_name: str,
        class_name: str,
        fields: str) -> None:
    # Beginning
    file.write('template <class R>\n')
    file.write(f"class {base_name}<R>::{class_name} : public {base_name}<R>\n")
    file.write("{\n")
    file.write("public:\n")

    # Constructor
    defineConstructor(file, class_name, fields)
    file.write('\n')

    # Accept method
    file.write(f'  R accept({base_name}<R>::Visitor& visitor) const override\n')
    file.write('  {\n')
    file.write(f'    return visitor.visit{class_name}{base_name}(*this);\n')
    file.write('  }\n')
    file.write('\n')

    # declarations
    fields: List[str] = fields.split(",")
    fields: List[str] = [field_info.strip().replace("&", "")
                         for field_info in fields]

    for type_decl in fields:
        file.write(f"  {type_decl};\n")

    file.write("};\n")


def defineAst(
        output_dir: str,
        base_name: str,
        types: List[str]) -> None:

    types: List[Dict[str, str]] = parseTypes(types)
    file_path: str = os.path.join(output_dir, f"{base_name}.h")
    with open(file_path, 'w') as file:
        # Creating beginning of header file
        file.write("#pragma once\n")
        file.write("\n")
        file.write('#include "Token.h"\n')
        file.write("\n")
        file.write("template <class R>\n")
        file.write(f"class {base_name}\n")
        file.write("{\n")
        file.write("public:\n")

        # Virtual destructor to allow for proper cleanup of derived classes.
        file.write(f"  virtual ~{base_name}() = default;\n")
        file.write("\n")

        # Predefinitons for types
        for type_info in types:
            file.write(f"  class {type_info['class_name']};\n")
        file.write('\n')

        defineVisitor(file, base_name, types)
        file.write('\n')

        # The base accept method
        file.write("  virtual R accept(Visitor& visitor) const = 0;\n")
        file.write('};\n')
        file.write('\n')

        # The AST Methods
        for i, type_info in enumerate(types, start=1):
            defineType(file,
                       base_name,
                       type_info['class_name'],
                       type_info['fields'])
            if i != len(types):
                file.write('\n')



def main() -> None:
    if (len(sys.argv) != 2):
        raise RuntimeError("Usage: python3 GenerateAst.py <output director>")
        sys.exit(64)

    output_dir: str = sys.argv[1]

    defineAst(output_dir, "Expr", [
            "Assign      : const Token& name, const std::shared_ptr<const Expr<R>>& value",
            "Binary      : const std::shared_ptr<const Expr<R>>& left, const Token& oper, const std::shared_ptr<const Expr<R>>& right",
            "Grouping    : const std::shared_ptr<const Expr<R>>& expression",
            "Literal     : const LiteralValue& value",
            "Logical     : const std::shared_ptr<const Expr<R>>& left, const Token& oper, const std::shared_ptr<const Expr<R>>& right",
            "Unary       : const Token& oper, const std::shared_ptr<const Expr<R>>& right",
            "Ternary     : const std::shared_ptr<const Expr<R>>& condition, const std::shared_ptr<const Expr<R>>& then_branch, const std::shared_ptr<const Expr<R>>& else_branch",
            "Variable    : const Token& name"
    ])

    defineAst(output_dir, "Stmt",[
            "Block      : const std::vector<std::shared_ptr<const Stmt<R>>>& statements",
            "Expression : const std::shared_ptr<const Expr<R>>& expression",
            "If         : const std::shared_ptr<const Expr<R>>& condition, const std::shared_ptr<const Stmt<R>>& then_branch," +
                        " const std::shared_ptr<const Stmt<R>>& else_branch",
            "Print      : const std::shared_ptr<const Expr<R>>& expression",
            "Var        : const Token& name, const std::shared_ptr<const Expr<R>>& initializer",
            "While      : const std::shared_ptr<const Expr<R>>& condition, const std::shared_ptr<const Stmt<R>>& body",
            "Jump      : const Token& keyword",
    ])
    
if __name__ == "__main__":
    main()